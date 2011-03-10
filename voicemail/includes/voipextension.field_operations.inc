<?php

/**
 * Find the voicemailbox field associated to the content types
 * If a particular content type is specified then the function will only return the field name for selected content type
 * @param $node_type
 *   Having content type value
 * @return
 *   An array containing content type as key and voicemail field name as value
 */
function _voicemail_get_content_type_voicemail_field($node_type = '') {
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
        if ($field_details['widget']['type'] == 'voicemail_widget' && $field_details['widget']['module'] == 'voicemail') {
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
 * Delete the voicemail field, if any other module wants to delete.
 * e.g. If voipextension fields gets deleted then its necessary to delete
 * voicemail as well.
 * @param $node_type
 *   A valid content type.
 * @param $field_name
 *   The field name to be deleted, if not available then this function
 *   will automatically calls the helper function to find the field name.
 */
function voicemail_delete_mailbox_field($node_type, $field_name = '') {
  // Need to load the CCK include file where content_field_instance_create() is defined.
  module_load_include('inc', 'content', 'includes/content.crud');
  if (!$field_name) {
    $field_name = _voicemail_get_content_type_voicemail_field($node_type);
    $field_name = $field_name[$node_type];
  }
  content_field_instance_delete($field_name, $node_type);
}