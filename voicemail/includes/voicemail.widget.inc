<?php
/**
 * @file
 * voipvoicemail widget hooks and callbacks.
 */

/**
 * Implementation of CCK's hook_widget_settings($op = 'form').
 */
function voicemail_widget_settings_form($widget) {
  $form = module_invoke('filefield', 'widget_settings', 'form', $widget);

  $form['file_extensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Permitted upload file extensions'),
    '#default_value' => is_string($widget['file_extensions']) ? $widget['file_extensions'] :'mp3 wav',
    '#size' => 64,
    '#description' => t('Extensions a user can upload to this field. Separate extensions with a space and do not include the leading dot. Leaving this blank will allow users to upload a file with any extension.'),
    '#weight' => 1,
  );

  return $form;
}

/**
 * Implementation of CCK's hook_widget_settings($op = 'save').
 */
function voicemail_widget_settings_save($widget) {
  $filefield_settings = module_invoke('filefield', 'widget_settings', 'save', $widget);
  return $filefield_settings;
}

/**
 * Element #value_callback function.
 */
function voicemail_widget_value($element, $edit = FALSE) {
  $field = filefield_widget_value($element, $edit);

  return $field;
}

/**
 * FormAPI theme function. Theme the output of an image field.
 */
function theme_voicemail_widget($element) {
  return theme('form_element', $element, $element['#children']);
}

/**
 * Process an individual element.
 *
 * Build the form element. When creating a form using FAPI #process,
 * note that $element['#value'] is already set.
 *
 * The $fields array is in $form['#field_info'][$element['#field_name']].
 */
function voicemail_widget_process($element, $edit, $form_state, $form) {
  $element['voicemail_status'] = array(
    '#title' => t('Status'),
    '#type' => 'radios',
    '#options' => array(1 => t('Enabled'), 0 => t('Disabled')),
    '#default_value' => 1
  );
  return $element;
}
