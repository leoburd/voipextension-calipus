<?php
/**
 * @file
 *
 * CCK Field definitions for Voicemail Module
 */

/**
 * Audio Greeting Audio File Field definition
 *
 * @return
 *   An array having CCK representation of the Audio field
 */
function _voicemail_audio_greeting_definition() {
  return array (
    'field_name' => VOICEMAIL_AUDIO_FIELD_NAME,
    'type_name' => 'story',
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' =>
      array (
        'format' => 'audiofield_embedded',
        'exclude' => 0,
      ),
      'full' =>
      array (
        'format' => 'audiofield_embedded',
        'exclude' => 0,
      ),
      4 =>
      array (
        'format' => 'audiofield_embedded',
        'exclude' => 0,
      ),
      'token' =>
      array (
        'format' => 'audiofield_embedded',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '0',
    'multiple' => '0',
    'db_storage' => '1',
    'module' => 'filefield',
    'active' => '1',
    'locked' => '0',
    'columns' =>
    array (
      'fid' =>
      array (
        'type' => 'int',
        'not null' => false,
        'views' => true,
      ),
      'list' =>
      array (
        'type' => 'int',
        'size' => 'tiny',
        'not null' => false,
        'views' => true,
      ),
      'data' =>
      array (
        'type' => 'text',
        'serialize' => true,
        'views' => true,
      ),
    ),
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'widget' =>
    array (
      'file_extensions' => 'mp3',
      'file_path' => '',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'label' => 'Mailbox greeting message',
      'weight' => '3',
      'description' => '',
      'type' => 'audiofield_widget',
      'module' => 'audiofield',
    ),
  );
}

/**
 * Option widget with status (Yes/No)
 *
 * @return
 *   An array having CCK representation of Options Widget
 */
function _voicemail_status_field_defintion() {
  return array (
    'field_name' => VOICEMAIL_STATUS_FIELD_NAME,
    'type_name' => 'story',
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      4 =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'number_integer',
    'required' => '1',
    'multiple' => '0',
    'db_storage' => '1',
    'module' => 'number',
    'active' => '1',
    'locked' => '0',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'int',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'prefix' => '',
    'suffix' => '',
    'min' => '0',
    'max' => '1',
    'allowed_values' => '1|Yes
  0|No',
    'allowed_values_php' => '',
    'widget' =>
    array (
      'default_value' =>
      array (
        0 =>
        array (
          'value' => '0',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Voicemail box status',
      'weight' => '4',
      'description' => '',
      'type' => 'optionwidgets_buttons',
      'module' => 'optionwidgets',
    ),
  );
}

/**
 * Voicemail Fieldset/Group for containing Voicemail fields
 *
 * @return
 *   An array having CCK representation of the fieldset.
 */
function _voicemail_fieldset_group_definition() {
  return array (
    'label' => 'Voice Mail Settings',
    'group_type' => 'standard',
    'settings' =>
    array (
      'form' =>
      array (
        'style' => 'fieldset_collapsible',
        'description' => '',
      ),
      'display' =>
      array (
        'description' => '',
        'teaser' =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        4 =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'token' =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '3',
    'group_name' => VOICEMAIL_GROUP_FIELD_NAME,
  );
}