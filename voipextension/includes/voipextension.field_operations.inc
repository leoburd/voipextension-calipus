<?php
/**
 * @file
 *
 * This file handles all the operations to be performed on cck fields used in
 * the system. Some are helper functions and some are main functions.
 *
 * These functions are only called when needed, so this file is loaded
 * dynamically.
 */

/**
 * Delete all the fields created by VoIP extension module, including VoIP directory fields as well.
 */
function voipextension_delete_fields() {
  module_load_include('inc', 'content', 'includes/content.admin');

  $form_id = 'content_field_remove_form';
  $fieldset_form_id = 'fieldgroup_remove_group';
  $fields = array(VOIPEXTENSION_EXTENSION_MAIN_FIELD, VOIPEXTENSION_EXTENSION_LISTED, VOIPEXTENSION_AUDIO_NAME, VOIPEXTENSION_AUDIO_DESCRIPTION, VOIPEXTENSION_AUDIO_GREETING);
  $form_state = array();
  $node_types = node_get_types();
  foreach ($node_types as $node_type => $node_type_info) {
    if (variable_get('voip_extension_use_'. $node_type, FALSE)) {
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
      drupal_execute($fieldset_form_id, $form_state, $node_type, VOIPEXTENSION_GROUP_FIELD_NAME);
      drupal_execute($fieldset_form_id, $form_state, $node_type, VOIPEXTENSION_VOIPDIRECTORY_GROUP_FIELD_NAME);

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
function voipextension_change_fields_status($status, $selected_node_type = '') {
  $node_types = node_get_types();
  foreach ($node_types as $node_type => $node_type_info) {
    if ($selected_node_type && $selected_node_type != $node_type) {
      continue 1;
    }
    _voipextension_change_extension_main_field_status($node_type, $status);
    _voipextension_change_extension_listed_field_status($node_type, $status);
    _voipextension_change_audio_fields_status($node_type, $status);
  }
}


/**
 * Find the voipextension field associated to the content types
 * If a particular content type is specified then the function will only return the field name for selected content type
 * @param $node_type
 *   Having content type value
 * @return
 *   An array containing content type as key and VoIP extension field name as value
 */
function _voipextension_get_content_type_voipextension_field($node_type = '') {
  $node_types = node_get_types();
  $node_type_field = array();

  $nodetype = '';
  $param = FALSE;
  if (isset($node_type) && $node_type != '' && array_key_exists($node_type, $node_types)) {
    $nodetype = $node_type;
    $param = TRUE;
  }

  $content_info = _content_type_info(TRUE);
  foreach ($node_types as $type => $type_details) {
    if ($nodetype != '') {
      if ($type != $nodetype) {
        continue 1;
      }
    }
    else {
      $nodetype = $type;
    }

    $content_type_details = $content_info['content types'][$nodetype];
    if (isset($content_type_details['fields']) && is_array($content_type_details['fields'])) {
      foreach ($content_type_details['fields'] as $field_name => $field_details) {
        if ($field_details['type'] == 'voipextension' && $field_details['module'] == 'voipextension') {
          $node_type_field[$nodetype] = $field_name;
          if ($param) {
            break 2;
          }
          else {
            break 1;
          }
        }
      }
    }
    $nodetype = '';
  }
  return $node_type_field;
}

/**
 * Helper function to verify that a field of type X and of module Y exists in the given content type or not.
 * @param $content_type
 * @param $field_type
 * @param $module_name
 * @return boolean
 */
function _voipextension_check_cck_field_exists($content_type, $field_type, $module_name) {
  $info = _content_type_info(TRUE);
  $content_type_details = $info['content types'][$content_type];
  if (isset($content_type_details['fields']) && is_array($content_type_details['fields'])) {
    foreach ($content_type_details['fields'] as $field_name => $field_details) {
      if ($field_details['type'] == $field_type && $field_details['module'] == $module_name) {
        return $field_details;
      }
    }
  }
  return FALSE;
}

/**
 * Create Main VoIP Extension field.
 * @param type $node_type
 */
function _voipextension_create_extension_main_field($node_type) {
  // Need to load the CCK include file where content_field_instance_create() is defined.
  module_load_include('inc', 'content', 'includes/content.crud');

  $field = _voipextension_main_field_definition($node_type);
  if (!content_fields($field['field_name'], $node_type)) {
    $field['type_name'] = $node_type;
    content_field_instance_create($field);
  }
}

/**
 * Change the status of VoIP Extension field.
 */
function _voipextension_change_extension_main_field_status($node_type, $status = 1) {
  db_query("UPDATE {". content_field_tablename() ."} SET active = %d WHERE field_name = '%s' AND module = '%s'", $status, VOIPEXTENSION_EXTENSION_MAIN_FIELD, 'voipextension');
  db_query("UPDATE {". content_instance_tablename() ."} SET widget_active = %d WHERE field_name = '%s' AND type_name = '%s' AND widget_module = '%s'", $status, VOIPEXTENSION_EXTENSION_MAIN_FIELD, $node_type, 'voipextension');
  content_clear_type_cache(TRUE);
}


/**
 * Create an optionwidget field from the provided cck field definition in the
 * cck_fields file.
 *
 * Uses the core content module crud operations to create a field.
 * @param $node_type
 *   A content type in which this field needs to be created.
 */
function _voipextension_create_extension_listed_field($node_type) {
  // Need to load the CCK include file where content_field_instance_create() is defined.
  module_load_include('inc', 'content', 'includes/content.crud');

  $field = _voipextension_extension_listed_definition();
  if (!content_fields($field['field_name'], $node_type)) {
    $field['type_name'] = $node_type;
    content_field_instance_create($field);
  }
}

/**
 * Change the status of the extension listed widget to the status of the
 * voipextension module.
 *
 * If the module is enable that means this field is enabled else disabled.
 * @param $node_type
 *   A valid content type.
 * @param $status
 *   Boolean value having the status, enabled/disabled.
 */
function _voipextension_change_extension_listed_field_status($node_type, $status = 1) {
  db_query("UPDATE {". content_field_tablename() ."} SET active = %d WHERE field_name = '%s' AND module = '%s'", $status, VOIPEXTENSION_EXTENSION_LISTED, 'number');
  db_query("UPDATE {". content_instance_tablename() ."} SET widget_active = %d WHERE field_name = '%s' AND type_name = '%s' AND widget_module = '%s'", $status, VOIPEXTENSION_EXTENSION_LISTED, $node_type, 'optionwidgets');
  content_clear_type_cache(TRUE);
}

/**
 * Setup and created the provided audio cck fields from the cck_fields file.
 * Uses the core crud operations of the content module.
 * @param $node_type
 *   The content type in which these fields needs to be created.
 */
function _voipextension_create_audio_fields($node_type) {
  // Need to load the CCK include file where content_field_instance_create() is defined.
  module_load_include('inc', 'content', 'includes/content.crud');

  $fields_to_create = _voipextension_audio_fields_definitions();
  foreach ($fields_to_create as $key => $field) {
    if (!content_fields($field['field_name'], $node_type)) {
      $field['type_name'] = $node_type;
      content_field_instance_create($field);
    }
  }
}

/**
 * Change the status of the cck audio fields to the status of the voipextensio module.
 * @param $node_type
 *   A valid content type, in which the audio fields needs to be changed.
 * @param $status
 *   Boolean value for the status, enabled/disabled
 */
function _voipextension_change_audio_fields_status($node_type, $status = 1) {
  $fields = _voipextension_audio_fields_definitions();
  foreach ($fields as $key => $field) {
    db_query("UPDATE {". content_field_tablename() ."} SET active = %d WHERE field_name = '%s' AND module = '%s'", $status, $field['field_name'], 'filefield');
    db_query("UPDATE {". content_instance_tablename() ."} SET widget_active = %d WHERE field_name = '%s' AND type_name = '%s' AND widget_module = '%s'", $status, $field['field_name'], $node_type, 'audiofield');
  }
  content_clear_type_cache(TRUE);
}

/**
 * Create a fieldset/fieldgroup for holding all the voipextension fields from
 * the provided cck field definition.
 * @param $node_type
 *   The content type in which this field needs to be created.
 * @return
 *   Saved status
 */
function _voipextension_create_fieldset_group($node_type, $group) {
  return fieldgroup_save_group($node_type, $group);
}

/**
 * Store and returns the VoIP directory field names.
 * These are the standard field names and can't be changed.
 * @param $key
 *   The field name required.
 * @return
 *   An array or a field required for some operation.
 */
function _voipextension_directory_field_names($key = '') {
  $voip_directory_fields = array(
    'VOIPEXTENSION_EXTENSION_LISTED' => 'field_voipextension_listed',
    'VOIPEXTENSION_AUDIO_NAME' => 'field_voipextension_aname',
    'VOIPEXTENSION_AUDIO_DESCRIPTION' => 'field_voipextension_adesc',
    'VOIPEXTENSION_AUDIO_GREETING' => 'field_voipextension_agreet'
  );

  if (isset($voip_directory_fields[$key])) {
    return $voip_directory_fields[$key];
  }
  else {
    return $voip_directory_fields;
  }
}