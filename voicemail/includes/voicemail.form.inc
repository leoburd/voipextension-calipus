<?php
/**
 * @file
 *
 * Custom form alterations.
 */


/**
 * content_field_overview hook_form_validate().
 */
function voicemail_form_content_field_overview_validate($form, &$form_state) {
  if (isset($form_state['clicked_button']['#post']['_add_new_field']) && $form_state['clicked_button']['#post']['_add_new_field']['type'] == 'filefield'
      && $form_state['clicked_button']['#post']['_add_new_field']['widget_type'] == 'voicemail_widget') {
    module_load_include('inc', 'voipextension', 'includes/voipextension.field_operations');
    if (!_voipextension_check_cck_field_exists($form['#type_name'], 'voipextension', 'voipextension')) {
      form_set_error('_add_new_field', t('Please first create a voipextension field for creating voicemail widget.'));
    }
  }
}

/**
 * Submit handler to set a new field's formatter to "audiofield_embedded".
 */
function voicemail_form_content_field_overview_submit($form, &$form_state) {
  if (isset($form_state['fields_added']['_add_new_field']) && isset($form['#type_name'])) {
    $new_field = $form_state['fields_added']['_add_new_field'];
    $node_type = $form['#type_name'];
    $field = content_fields($new_field, $node_type);
    if ($field['widget']['module'] == 'voicemail') {
      foreach ($field['display_settings'] as $display_type => $display_settings) {
        if ($field['display_settings'][$display_type]['format'] == 'default') {
          $field['display_settings'][$display_type]['format'] = 'audiofield_embedded';
        }
      }
      content_field_instance_update($field);
    }
  }
}