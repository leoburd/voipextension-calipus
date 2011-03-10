<?php
/**
 * @file
 *
 * Admin page callback file for the voip extension module.
 */

/**
 * Configure voip extension settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function voipextension_admin_settings() {
  drupal_set_title(t('Default Voip Extension Settings'));
  $form['voipextension'] = array(
    '#title' => t('Voip Extension Settings'),
    '#type' => 'fieldset'
  );

  $form['voipextension']['voipextension_script_name'] = array(
    '#title' => t('Default Script Name'),
    '#type' => 'textfield',
    '#size' => 30,
    '#maxlength' => 255,
    '#default_value' => variable_get('voipextension_script_name', '')
  );

  $form['voipextension']['voipextension_status'] = array(
    '#title' => t('Default Extension Status'),
    '#type' => 'radios',
    '#options' => array(1 => 'Enable', 0 => 'Disable'),
    '#default_value' => variable_get('voipextension_status', 1)
  );

  return system_settings_form($form);
}