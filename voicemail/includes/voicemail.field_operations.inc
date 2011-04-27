<?php
/**
 * @file
 *
 * Voicemail field operations
 */

/**
 * Delete all the fields created by VoIP extension module, including VoIP directory fields as well.
 */
function voicemail_delete_fields() {
  module_load_include('inc', 'content', 'includes/content.admin');

  $form_id = 'content_field_remove_form';
  $fieldset_form_id = 'fieldgroup_remove_group';
  $fields = array(VOICEMAIL_AUDIO_FIELD_NAME, VOICEMAIL_STATUS_FIELD_NAME);
  $form_state = array();
  $node_types = node_get_types();
  foreach ($node_types as $node_type => $node_type_info) {
    if (variable_get('voip_voicemail_use_'. $node_type, FALSE)) {
      foreach ($fields as $index => $field_name) {
        $form_state['values'] = array();
        $form_state['values'] = array(
          'type_name' => $node_type,
          'field_name' => $field_name,
          'confirm' => 1,
          'op' => 'Remove',
          'submit' => 'Remove',
          'form_id' => $form_id
        );
        // Submit the form using these values.
        drupal_execute($form_id, $form_state, $node_type, $field_name);
      }

      $form_state['values'] = array();
      // Delete VoIP Extension fieldset/group
      $form_state['values'] = array(
        'confirm' => 1,
        'op' => 'Remove',
        'submit' => 'Remove'
      );
      drupal_execute($fieldset_form_id, $form_state, $node_type, VOICEMAIL_GROUP_FIELD_NAME);

      foreach ($fields as $index => $field_name) {
        $form_values = array();
        $form_values['type_name']  = $node_type;
        $form_values['field_name'] = $field_name;
        fieldgroup_update_fields($form_values);
      }
    }
  }
}

/**
 * Change the status of all the existing fields implemented by voipextension module.
 */
function voicemail_change_fields_status($status, $selected_node_type = '') {
  $node_types = node_get_types();
  foreach ($node_types as $node_type => $node_type_info) {
    if ($selected_node_type && $selected_node_type != $node_type) {
      continue 1;
    }
    _voipextension_change_audio_greeting_field_status($node_type, $status);
    _voicemail_change_status_field_status($node_type, $status);
  }
}

/**
 * Create Main VoIP Extension field.
 * @param $node_type
 */
function _voicemail_create_audio_greeting_field($node_type) {
  // Need to load the CCK include file where content_field_instance_create() is defined.
  module_load_include('inc', 'content', 'includes/content.crud');

  $field = _voicemail_audio_greeting_definition();
  if (!content_fields($field['field_name'], $node_type)) {
    $field['type_name'] = $node_type;
    content_field_instance_create($field);
  }
}

/**
 * Change the status of VoIP Extension field.
 */
function _voipextension_change_audio_greeting_field_status($node_type, $status = 1) {
  db_query("UPDATE {". content_field_tablename() ."} SET active = %d WHERE field_name = '%s' AND module = '%s'", $status, VOICEMAIL_AUDIO_FIELD_NAME, 'filefield');
  db_query("UPDATE {". content_instance_tablename() ."} SET widget_active = %d WHERE field_name = '%s' AND type_name = '%s' AND widget_module = '%s'", $status, VOICEMAIL_AUDIO_FIELD_NAME, $node_type, 'audiofield');
  content_clear_type_cache(TRUE);
}

/**
 * Create option widget status field from the available CCK definition.
 *
 * @param $node_type
 */
function _voicemail_create_status_field($node_type) {
  // Need to load the CCK include file where content_field_instance_create() is defined.
  module_load_include('inc', 'content', 'includes/content.crud');

  $field = _voicemail_status_field_defintion();
  if (!content_fields($field['field_name'], $node_type)) {
    $field['type_name'] = $node_type;
    content_field_instance_create($field);
  }
}

/**
 * Change the status of the Voicemail status field
 *
 * @param $node_type
 * @param $status
 */
function _voicemail_change_status_field_status($node_type, $status = 1) {
  db_query("UPDATE {". content_field_tablename() ."} SET active = %d WHERE field_name = '%s' AND module = '%s'", $status, VOICEMAIL_STATUS_FIELD_NAME, 'number');
  db_query("UPDATE {". content_instance_tablename() ."} SET widget_active = %d WHERE field_name = '%s' AND type_name = '%s' AND widget_module = '%s'", $status, VOICEMAIL_STATUS_FIELD_NAME, $node_type, 'optionwidgets');
  content_clear_type_cache(TRUE);
}

/**
 * Create Voicemail group/fieldset for containing Voicemail fields.
 *
 * @param $node_type
 * @param $group
 * @return
 *   A boolean TRUE if created else FALSE
 */
function _voicemail_create_fieldset_group($node_type, $group) {
  return fieldgroup_save_group($node_type, $group);
}