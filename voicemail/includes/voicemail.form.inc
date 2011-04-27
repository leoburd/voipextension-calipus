<?php
/**
 * @file
 *
 * All form related changes/operations should be done here.
 */

/**
 * Alter node type form to support Voicemail feature
 *
 * @param $form
 * @param $form_state
 */
function voicemail_node_type_form_alter(&$form, &$form_state) {
  $voip_voicemail_weight = 34; // default weight of the fieldset.
  $content_type = $form['#node_type']->type;
  if (isset($form['submit']['#weight'])) {
    $voip_voicemail_weight = $form['submit']['#weight'] - 0.98;
  }

  // VoIP Extension field exists, so Voicemail can be attached to this content type.
  if (variable_get('voip_extension_use_'. $content_type, FALSE)) {
    $form['voip_voicemail'] = array(
      '#type' => 'fieldset',
      '#title' => t('Voice Mail'),
      '#group' => 'additional_settings',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => $voip_voicemail_weight
    );

    $form['voip_voicemail']['voip_voicemail_use'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use this content type as a Voice Mail'),
      '#default_value' => variable_get('voip_voicemail_use_'. $content_type, FALSE),
      '#attributes' => array(
        'onclick' => 'javascript: if (jQuery(this).attr("checked")) {
            jQuery("fieldset#node_type_form_voip_voicemail_configuration_id").show("slow");
          }
          else {
            jQuery("fieldset#node_type_form_voip_voicemail_configuration_id").hide("fast");
          };'
      )
    );

    if (variable_get('voip_voicemail_use_'. $content_type, FALSE)) {
      $form['voip_voicemail']['voip_voicemail_fields_configuration'] = array(
        '#type' => 'fieldset',
        '#title' => t('VoIP Voice Mail Fields Configuration'),
        '#group' => 'additional_settings',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#attributes' => array(
          'id' => 'node_type_form_voip_voicemail_configuration_id'
        )
      );

      $voicemail_configuration_options = '<div class="voipvoicemail-'. $content_type .'configuration-options">';
      $voicemail_configuration_options .= l(t('Configure Audio Greeting'), 'admin/content/node-type/'. $content_type .'/fields/'. VOICEMAIL_AUDIO_FIELD_NAME);
      $voicemail_configuration_options .= '<br />';
      $voicemail_configuration_options .= l(t('Configure Voicemail Status'), 'admin/content/node-type/'. $content_type .'/fields/'. VOICEMAIL_STATUS_FIELD_NAME);
      $voicemail_configuration_options .= '<br />';
      $voicemail_configuration_options .= '</div>';

      $form['voip_voicemail']['voip_voicemail_fields_configuration']['voicemail_configuration_options'] = array(
        '#type' => 'markup',
        '#value' => $voicemail_configuration_options
      );
    }
  }
  $form['#submit'][] = 'voicemail_node_type_form_submit';
}


function voicemail_node_type_form_submit($form, &$form_state) {
  // Voicemail field doesn't exists yet.
  if (!isset($form_state['values']['voip_voicemail_use'])) {
    return TRUE;
  }

  module_load_include('inc', 'voicemail', 'includes/voicemail.helper');

  $content_type = $form_state['values']['type'];
  if ($form_state['values']['voip_voicemail_use']) {
    voicemail_change_fields_status(1, $content_type);
//    voicemail_update_status($content_type, 1);

    $group = _voicemail_fieldset_group_definition();
    _voicemail_create_fieldset_group($content_type, $group);

    _voicemail_create_audio_greeting_field($content_type);
    _voicemail_create_status_field($content_type);

    $group_name = VOICEMAIL_GROUP_FIELD_NAME;
    $fields = array(VOICEMAIL_AUDIO_FIELD_NAME, VOICEMAIL_STATUS_FIELD_NAME);
    // Assign the voip extension fields to the selcted field group.
    foreach ($fields as $index => $field_name) {
      $form_values = array();
      $form_values['type_name']  = $content_type;
      $form_values['group']      = $group_name;
      $form_values['field_name'] = $field_name;
      fieldgroup_update_fields($form_values);
    }

    drupal_set_message(t('Voice Mail fields have been associated with %content_type', array('%content_type' => $content_type)));
  }
  else {
    voicemail_change_fields_status(0, $content_type);
//    voicemail_update_status($content_type, 0);
  }

  if (!$form_state['values']['voip_extension_use']) {
    voicemail_change_fields_status(0, $content_type);
  }
}

/**
 * Validates the Manage fields form to check that the voicemail fields should not be created manually.
 * 
 * @param $form
 * @param $form_state
 */
function voicemail_content_field_overview_form_validate($form, &$form_state) {
  if (isset($form_state['clicked_button']['#post']['_add_new_field']['field_name']) &&
      $form_state['clicked_button']['#post']['_add_new_field']['field_name'] != '') {
    $field_name = 'field_'. $form_state['clicked_button']['#post']['_add_new_field']['field_name'];
    if ($field_name == VOICEMAIL_AUDIO_FIELD_NAME || $field_name == VOICEMAIL_STATUS_FIELD_NAME) {
      form_set_error('_add_new_field', t('Please choose some other field name.This is reserved to be used by Voice Mail module.'));
    }
  }

  if (isset($form_state['clicked_button']['#post']['_add_new_group']['group_name']) &&
      $form_state['clicked_button']['#post']['_add_new_group']['group_name'] != '') {
    $group_name = 'group_'. $form_state['clicked_button']['#post']['_add_new_group']['group_name'];
    if ($group_name == VOICEMAIL_GROUP_FIELD_NAME) {
      form_set_error('_add_new_group', t('Please choose some different group name.This is reserved to be used by Voice Mail module.'));
    }
  }
}