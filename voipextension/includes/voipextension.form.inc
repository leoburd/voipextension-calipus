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
    module_load_include('inc', 'voipextension', 'includes/voipextension.field_operations');
    if (in_array($field_name, _voipextension_directory_field_names())) {
      form_set_error('_add_new_field', t('Please choose some different field name. This is reserved to be used by VoIP extension module.'));
    }
  }

  if (isset($form_state['clicked_button']['#post']['_add_new_group']['group_name']) &&
      $form_state['clicked_button']['#post']['_add_new_group']['group_name'] != '') {
    $group_name = 'group_'. $form_state['clicked_button']['#post']['_add_new_group']['group_name'];
    if ($group_name == VOIPEXTENSION_GROUP_FIELD_NAME) {
      form_set_error('_add_new_group', t('Please choose some different group name. This is reserved to be used by VoIP extension module.'));
    }
  }

  if (isset($form_state['clicked_button']['#post']['_add_new_field']) && is_array($form_state['clicked_button']['#post']['_add_new_field']) &&
      isset($form_state['clicked_button']['#post']['_add_new_field']['type']) && $form_state['clicked_button']['#post']['_add_new_field']['type'] != ''
      && isset($form_state['clicked_button']['#post']['_add_new_field']['widget_type']) &&
      $form_state['clicked_button']['#post']['_add_new_field']['widget_type'] != '' &&
      $form_state['clicked_button']['#post']['_add_new_field']['type'] == 'voipextension' &&
      $form_state['clicked_button']['#post']['_add_new_field']['widget_type'] == 'voipextension') {
    if (_voipextension_check_cck_field_exists($form['#type_name'], 'voipextension', 'voipextension')) {
      form_set_error('_add_new_field', t('Only one VoIP extension cck field can be added to one content type'));
    }
  }
}

/**
 * Submit hook for content_field_overview form.
 * Create and setup the related or VoIP directory fields, if the current created field is voipextension field.
 *
 * @param $form
 * @param $form_state
 */
function voipextension_content_field_overview_form_submit($form, &$form_state) {
  if ((isset($form_state['clicked_button']['#post']['_add_existing_field']) && is_array($form_state['clicked_button']['#post']['_add_existing_field']) &&
      isset($form_state['clicked_button']['#post']['_add_existing_field']['widget_type']) &&
      $form_state['clicked_button']['#post']['_add_existing_field']['widget_type'] != '' &&
      $form_state['clicked_button']['#post']['_add_existing_field']['widget_type'] == 'voipextension') ||
      (isset($form_state['clicked_button']['#post']['_add_new_field']) && is_array($form_state['clicked_button']['#post']['_add_new_field']) &&
      isset($form_state['clicked_button']['#post']['_add_new_field']['type']) && $form_state['clicked_button']['#post']['_add_new_field']['type'] != ''
      && isset($form_state['clicked_button']['#post']['_add_new_field']['widget_type']) &&
      $form_state['clicked_button']['#post']['_add_new_field']['widget_type'] != '' &&
      $form_state['clicked_button']['#post']['_add_new_field']['type'] == 'voipextension' &&
      $form_state['clicked_button']['#post']['_add_new_field']['widget_type'] == 'voipextension')) {

    if (isset($form_state['clicked_button']['#post']['_add_new_field']['widget_type']) &&
        $form_state['clicked_button']['#post']['_add_new_field']['widget_type'] == 'voipextension') {
      $form_state['values']['_add_new_field']['parent'] = VOIPEXTENSION_GROUP_FIELD_NAME;
    }
    if (isset($form_state['clicked_button']['#post']['_add_existing_field']['widget_type']) &&
        $form_state['clicked_button']['#post']['_add_existing_field']['widget_type'] == 'voipextension') {
      $form_state['values']['_add_existing_field']['parent'] = VOIPEXTENSION_GROUP_FIELD_NAME;
    }

    $content_type = $form['#type_name'];
    _voipextension_create_extension_listed_field($content_type);
    _voipextension_create_voipextension_group($content_type);
    _voipextension_create_audio_fields($content_type);

    $voip_field_name = 'field_'. $form_state['clicked_button']['#post']['_add_new_field']['field_name'];

    $group_name = VOIPEXTENSION_GROUP_FIELD_NAME;
    $fields = array(VOIPEXTENSION_EXTENSION_LISTED, VOIPEXTENSION_AUDIO_NAME, VOIPEXTENSION_AUDIO_DESCRIPTION, VOIPEXTENSION_AUDIO_GREETING);
    // Assign the voip extension fields to the selcted field group.
    foreach ($fields as $index => $field_name) {
      $form_values = array();
      $form_values['type_name']  = $content_type;
      $form_values['group']      = $group_name;
      $form_values['field_name'] = $field_name;
      fieldgroup_update_fields($form_values);
    }
  }
}

/**
 * Submit hook of content_field_remove form.
 * If the current deleted field is a voipextension field then delete all the
 * related VoIP directory fields as well from the selcted content type.
 * @param $form
 * @param $form_state
 */
function voipextension_content_field_remove_form_submit($form, &$form_state) {
  if (isset($form['field_name']['#value']) && isset($form['type_name']['#value'])) {
    $content_field_info = content_fields($form['field_name']['#value'], $form['type_name']['#value']);
    if (isset($content_field_info['type']) && $content_field_info['type'] == 'voipextension' && isset($content_field_info['module']) &&
        $content_field_info['module'] == 'voipextension') {
      _voipextension_delete_extension_listed_field($content_field_info['type_name']);
      _voipextension_delete_audio_fields($content_field_info['type_name']);
      _voipextension_delete_voipextension_group($content_field_info['type_name']);

      $fields = array(VOIPEXTENSION_EXTENSION_LISTED, VOIPEXTENSION_AUDIO_NAME, VOIPEXTENSION_AUDIO_DESCRIPTION, VOIPEXTENSION_AUDIO_GREETING);
      // Delete the voip extension fields from the selcted field group.
      foreach ($fields as $index => $field_name) {
        $form_values = array();
        $form_values['type_name']  = $content_field_info['type_name'];
        $form_values['field_name'] = $field_name;
        fieldgroup_update_fields($form_values);
      }
    }
  }
}

/**
 * Remove or hide VoIP directory audio and status fields from other content types in the existing cck fields section.
 */
function voipextension_content_field_overview_form_alter(&$form, &$form_state) {
  if (isset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_EXTENSION_LISTED])) {
    unset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_EXTENSION_LISTED]);
  }
  if (isset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_AUDIO_NAME])) {
    unset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_AUDIO_NAME]);
  }
  if (isset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_AUDIO_DESCRIPTION])) {
    unset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_AUDIO_DESCRIPTION]);
  }
  if (isset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_AUDIO_GREETING])) {
    unset($form['_add_existing_field']['field_name']['#options'][VOIPEXTENSION_AUDIO_GREETING]);
  }
}