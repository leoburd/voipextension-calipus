<?php
/**
 * @file
 *
 * forms hooks and alterations implemented by VoIP Extension module.
 */

/**
 * Makes the user register and the profile form, as per the instructions in the settings of content profile module
 *
 * @param $type
 *   A string content type of the content.
 * @param $form
 *   An array of user register form
 * @param $form_state
 *   An array of collected post/stored values for the user register form.
 */
function voipextension_modify_profile_form($type, &$form, &$form_state) {
  // Fetch profile node and add in its form.
  $node = content_profile_load($type, $form['#uid']);
  if (!$node) {
    $node = array('uid' => 0, 'name' => '', 'type' => $type);
  }

  // Get the original node form.
  $node_form = drupal_retrieve_form($type .'_node_form', $form_state, $node);
  drupal_prepare_form($type .'_node_form', $node_form, $form_state);

  foreach ($node_form as $key => $value) {
    if (in_array($key, array('#id', 'form_id', 'form_token', '#token', '#action', '#parameters', 'author'))) {
      continue;
    }
    if (in_array($key, array('#submit', '#validate'))) {
      $form[$key] = array_merge($form[$key], $value);
      continue;
    }
    if (in_array($key, array('nid', 'vid', 'created', 'type', 'changed', 'buttons'))) {
      $form[$key] = $value;
    }
  }

  // If non-CCK form elements are hidden, only copy over the CCK stuff
  if (in_array('other', content_profile_get_settings($type, 'registration_hide'))) {
    foreach ($node_form['#field_info'] as $field_name => $info) {
      if (isset($node_form[$field_name])) {
        $form[$field_name] = $node_form[$field_name];
      }
    }
    // Copy over any fieldgroups
    $keys = array_keys($node_form);
    foreach ($keys as $key) {
      if (stristr($key, 'group_')) {
        $form[$key] = $node_form[$key];
      }
    }
    // Add the title
    $form['title'] = $node_form['title'];

    // Set this to the values of one node, as it might be need by some #ahah callbacks
    $form['#node'] = $node_form['#node'];
    $form['type'] = $node_form['type'];
  }
  else {
    foreach (array('uid', 'name', 'author', 'buttons', 'language', '#theme', 'options') as $key) {
      unset($node_form[$key]);
    }
    $form = $node_form;
  }

  // Hide fields as configured
  foreach (content_profile_get_settings($type, 'registration_hide') as $field_name) {
    if (module_exists('fieldgroup') && ($group_name = _fieldgroup_field_get_group($type, $field_name))) {
      unset($form[$group_name][$field_name]);
      if (count(element_children($form[$group_name])) == 0) {
        unset($form[$group_name]);
      }
    }
    else {
      unset($form[$field_name]);
    }
  }

  $form += array('#field_info' => array());
  $form['#field_info'] += $node_form['#field_info'];


  $form['author']['name']['#default_value'] = $form['account']['name']['#default_value'];
  unset($form['uid']);
  unset($form['name']);
  unset($form['submit']);
  unset($form['delete']);

  // Add custom submit and validation handlers.
  $form['#submit'][] = 'voipextension_user_modify_submit';
  $form['#validate'][] = 'voipextension_user_modify_validate';

  // Remove user.module submit handler here, we only want it to be triggered on save, not on preview.
  // We will call it ourselves from our save handler.
  $form['#submit'] = array_diff($form['#submit'], array('user_profile_form_submit'));
  $form['#validate'] = array_diff($form['#validate'], array('user_profile_form_validate'));

  // Also remove node validation.  We will call node_validate ourselves from voipext_user_modify_validate()
  $form['#validate'] = array_diff($form['#validate'], array('node_form_validate'));

  $form['#redirect'] = 'user/'. $form['#uid'] .'/edit';

  $weight = -10;
  foreach ($form as $key => $value) {
    if (is_array($value) && isset($value['#weight'])) {
      $form[$key]['#weight'] = $weight;
      $weight++;
    }
  }

  module_load_include('inc', 'voipextension', 'includes/voipextension.api');
  $voip_extension = VoipExtension::getVoipExtensionFromUid($form['#uid']);
  if (!$voip_extension) {
    $form[VOIPEXTENSION_GROUP_FIELD_NAME][VOIPEXTENSION_EXTENSION_LISTED]['#default_value'][0]['value'] = 1;
  }
}


/**
 * Validates the user edit form
 */
function voipextension_user_modify_validate($form, &$form_state) {

  // Make sure the language setting saves.
  if ($form_state['clicked_button']['#post']['language']) {
    $form_state['values']['language'] = $form_state['clicked_button']['#post']['language'];
  }

  user_profile_form_validate($form, $form_state);
  $node = $form['#node'];
  if ($form_state['clicked_button']['#id'] === 'edit-submit') {
      $node = $node->_account;
  }
  require_once drupal_get_path('module', 'node') .'/node.pages.inc';
  $node->changed = strtotime(date("Y-m-d H:i:s"));
  unset($node->name);
  node_validate($node, $form);
}

/**
 * Submits the user edit form
 */
function voipextension_user_modify_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] === 'edit-submit') {
    user_profile_form_submit($form, $form_state);
    $node = $form['#node'];
    $node->title = $node->name;
//    $node = node_submit($node, $form);
//    node_save($node, $form);
  }
}

/**
 * Submit the user registration form.
 *
 * It checks that user is having permission of VoIP extension, if have then submit the content profile form also.
 */
function voipextension_user_register_submit($form, &$form_state) {
  $uid = $form_state['user']->uid;
  if ($uid > 0) {
    if (user_access(VOIPEXTENSION_HAVE_PERSONAL_EXTENSION, $form_state['user'])) {
      content_profile_registration_user_register_submit($form, $form_state);
    }
  }
}

/**
 * Validate hook for content_field_overview form.
 * Checks that, no content type can have more than one voipextension field.
 * @param $form
 * @param $form_state
 */
function voipextension_content_field_overview_form_validate($form, &$form_state) {
  if (isset($form_state['clicked_button']['#post']['_add_new_field']['field_name']) &&
      $form_state['clicked_button']['#post']['_add_new_field']['field_name'] != '') {
    $field_name = 'field_'. $form_state['clicked_button']['#post']['_add_new_field']['field_name'];
    if ($field_name == VOIPEXTENSION_EXTENSION_MAIN_FIELD) {
      form_set_error('_add_new_field', t('Please choose some other field name. This is reserved to be use by VoIP Extension module.'));
    }

    module_load_include('inc', 'voipextension', 'includes/voipextension.field_operations');
    if (in_array($field_name, _voipextension_directory_field_names())) {
      form_set_error('_add_new_field', t('Please choose some different field name. This is reserved to be used by VoIP extension module.'));
    }
  }

  if (isset($form_state['clicked_button']['#post']['_add_new_group']['group_name']) &&
      $form_state['clicked_button']['#post']['_add_new_group']['group_name'] != '') {
    $group_name = 'group_'. $form_state['clicked_button']['#post']['_add_new_group']['group_name'];
    if ($group_name == VOIPEXTENSION_GROUP_FIELD_NAME || $group_name == VOIPEXTENSION_VOIPDIRECTORY_GROUP_FIELD_NAME) {
      form_set_error('_add_new_group', t('Please choose some different group name. This is reserved to be used by VoIP extension module.'));
    }
  }

  if (isset($form_state['clicked_button']['#post']['_add_new_field']) && is_array($form_state['clicked_button']['#post']['_add_new_field']) && isset($form_state['clicked_button']['#post']['_add_new_field']['type']) && $form_state['clicked_button']['#post']['_add_new_field']['type'] != '' && isset($form_state['clicked_button']['#post']['_add_new_field']['widget_type']) && $form_state['clicked_button']['#post']['_add_new_field']['widget_type'] != '' && $form_state['clicked_button']['#post']['_add_new_field']['type'] == 'voipextension' && $form_state['clicked_button']['#post']['_add_new_field']['widget_type'] == 'voipextension') {
    if (_voipextension_check_cck_field_exists($form['#type_name'], 'voipextension', 'voipextension')) {
      form_set_error('_add_new_field', t('Only one VoIP extension cck field can be added to one content type'));
    }
  }
}


/**
 * Remove or hide VoIP directory audio and status fields from other content types in the existing cck fields section.
 */
function voipextension_content_field_overview_form_alter(&$form, &$form_state) {
  // Remove the VoIP Extension widget from the Drop Down
  if (isset($form['_add_new_field']['type']['#options']['voipextension'])) {
    // Unset the widget type first.
    if (isset($form['_add_new_field']['widget_type']['#options'][$form['_add_new_field']['type']['#options']['voipextension']]['voipextension'])) {
      unset($form['_add_new_field']['widget_type']['#options'][$form['_add_new_field']['type']['#options']['voipextension']]);
    }
    unset($form['_add_new_field']['type']['#options']['voipextension']);
  }

  // Delete VoIP Extension widget type from Existing fields.
  if (isset($form['_add_existing_field']['widget_type']['#options']) && is_array($form['_add_existing_field']['widget_type']['#options']) && count($form['_add_existing_field']['widget_type']['#options']) > 0) {
    foreach ($form['_add_existing_field']['widget_type']['#options'] as $option_index => $options) {
      if (is_array($options) && count($options) > 0) {
        if (isset($options['voipextension'])) {
          unset($form['_add_existing_field']['widget_type']['#options'][$option_index]);
          break 1;
        }
      }
    }
  }

  // Remove VoIP Extension field from existing fields
  if (isset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_EXTENSION_MAIN_FIELD])) {
    unset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_EXTENSION_MAIN_FIELD]);
  }

  // Remove VoIP Extension Listed in Phone Directory from existing fields
  if (isset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_EXTENSION_LISTED])) {
    unset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_EXTENSION_LISTED]);
  }
  // Remove Audio name field from existing fields
  if (isset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_AUDIO_NAME])) {
    unset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_AUDIO_NAME]);
  }
  // Remove Audio description field from existing fields
  if (isset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_AUDIO_DESCRIPTION])) {
    unset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_AUDIO_DESCRIPTION]);
  }
  // Remove Audio greeting field from existing fields
  if (isset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_AUDIO_GREETING])) {
    unset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_AUDIO_GREETING]);
  }
}

function voipextension_node_type_form_alter(&$form, &$form_state) {
  $voip_extension_weight = 33; // default weight of the fieldset.
  $content_type = $form['#node_type']->type;
  if (isset($form['submit']['#weight'])) {
    $voip_extension_weight = $form['submit']['#weight'] - 0.99;
  }

  $form['voip_extension'] = array(
    '#type' => 'fieldset',
    '#title' => t('VoIP Extension'),
    '#group' => 'additional_settings',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => $voip_extension_weight
  );

  $form['voip_extension']['voip_extension_use'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use this content type as a VoIP Extension'),
    '#default_value' => variable_get('voip_extension_use_'. $content_type, FALSE),
    '#attributes' => array(
      'onclick' => 'javascript: if (jQuery(this).attr("checked")) {
          jQuery("fieldset#node_type_form_voip_extension_settings_id").show("slow");
          jQuery("fieldset#node_type_form_voip_directory_configuration_id").show("slow");
        }
        else {
          jQuery("fieldset#node_type_form_voip_extension_settings_id").hide("fast");
          jQuery("fieldset#node_type_form_voip_directory_configuration_id").hide("fast");
        };'
    )
  );

  $voip_extension_settings_style = 'display:none;';
  if (variable_get('voip_extension_use_'. $content_type, FALSE)) {
    $voip_extension_settings_style = 'display:block;';
  }
    // VoIP Extension field settings.
  $form['voip_extension']['voip_extension_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('VoIP Extension Default Settings'),
    '#group' => 'additional_settings',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array(
      'style' => $voip_extension_settings_style,
      'id' => 'node_type_form_voip_extension_settings_id'
    )
  );

  $default_script_name = variable_get('default_script_name_'. $content_type, variable_get('voipextension_script_name',''));
  $form['voip_extension']['voip_extension_settings']['default_script_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Script Name'),
    '#maxlength' => 100,
    '#size' => 30,
    '#default_value' => $default_script_name
  );

  $default_extension_status = variable_get('default_extension_status_'. $content_type, variable_get('voipextension_status', 0));
  $form['voip_extension']['voip_extension_settings']['default_extension_status'] = array(
    '#type' => 'radios',
    '#title' => t('Default Extension status'),
    '#options' => array(1 => t('Enabled'), 0 => t('Disabled')),
    '#default_value' => $default_extension_status ? $default_extension_status : 0
  );

  if (variable_get('voip_extension_use_'. $content_type, FALSE)) {
    $form['voip_extension']['voip_extension_fields_configuration'] = array(
      '#type' => 'fieldset',
      '#title' => t('VoIP Directory Fields Configuration'),
      '#group' => 'additional_settings',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#attributes' => array(
        'id' => 'node_type_form_voip_directory_configuration_id'
      )
    );

    $voipextension_configuration_options = '<div class="voipextension-'. $content_type .'configuration-options">';
    $voipextension_configuration_options .= l(t('Configure Extension listed in phone directory'), 'admin/content/node-type/'. $content_type .'/fields/'. VOIPEXTENSION_EXTENSION_LISTED);
    $voipextension_configuration_options .= '<br />';
    $voipextension_configuration_options .= l(t('Configure Audio name Field'), 'admin/content/node-type/'. $content_type .'/fields/'. VOIPEXTENSION_AUDIO_NAME);
    $voipextension_configuration_options .= '<br />';
    $voipextension_configuration_options .= l(t('Configure Audio Description Field'), 'admin/content/node-type/'. $content_type .'/fields/'. VOIPEXTENSION_AUDIO_DESCRIPTION);
    $voipextension_configuration_options .= '<br />';
    $voipextension_configuration_options .= l(t('Configure Audio Greeting'), 'admin/content/node-type/'. $content_type .'/fields/'. VOIPEXTENSION_AUDIO_GREETING);
    $voipextension_configuration_options .= '<br />';
    $voipextension_configuration_options .= '</div>';

    $form['voip_extension']['voip_extension_fields_configuration']['configuration_options'] = array(
      '#type' => 'markup',
      '#value' => $voipextension_configuration_options
    );
  }

  $form['#submit'][] = 'voipextension_node_type_form_submit';
}

function voipextension_node_type_form_submit($form, &$form_state) {
  // VoIP Extension field doesn't exists yet.
  if (!isset($form_state['values']['voip_extension_use'])) {
    return TRUE;
  }

  module_load_include('inc', 'voipextension', 'includes/voipextension.helper');

  $content_type = $form_state['values']['type'];
  if ($form_state['values']['voip_extension_use']) {
    voipextension_change_fields_status(1, $content_type);
    voipextension_update_extensions_status($content_type, 1);

    _voipextension_create_extension_main_field($content_type);
    $group = _voipextension_extension_group_defintion();
    _voipextension_create_fieldset_group($content_type, $group);

    $group = _voipextension_voipdirectory_settings_group();
    _voipextension_create_fieldset_group($content_type, $group);

    _voipextension_create_extension_listed_field($content_type);
    _voipextension_create_audio_fields($content_type);

    $group_name = VOIPEXTENSION_GROUP_FIELD_NAME;
    $fields = array(VOIPEXTENSION_EXTENSION_MAIN_FIELD);
    // Assign the voip extension fields to the selcted field group.
    foreach ($fields as $index => $field_name) {
      $form_values = array();
      $form_values['type_name']  = $content_type;
      $form_values['group']      = $group_name;
      $form_values['field_name'] = $field_name;
      fieldgroup_update_fields($form_values);
    }

    $group_name = VOIPEXTENSION_VOIPDIRECTORY_GROUP_FIELD_NAME;
    $fields = array(VOIPEXTENSION_EXTENSION_LISTED, VOIPEXTENSION_AUDIO_NAME, VOIPEXTENSION_AUDIO_DESCRIPTION, VOIPEXTENSION_AUDIO_GREETING);
    // Assign the voip extension fields to the selcted field group.
    foreach ($fields as $index => $field_name) {
      $form_values = array();
      $form_values['type_name']  = $content_type;
      $form_values['group']      = $group_name;
      $form_values['field_name'] = $field_name;
      fieldgroup_update_fields($form_values);
    }

    drupal_set_message(t('VoIP Extension field have been associated with %content_type', array('%content_type' => $content_type)));
  }
  else {
    voipextension_change_fields_status(0, $content_type);
    voipextension_update_extensions_status($content_type, 0);
  }
}