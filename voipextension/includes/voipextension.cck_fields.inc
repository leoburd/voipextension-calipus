<?php
/**
 * @file
 *
 * CCK field definitions of the VoIP directory fields.
 * This file contains the helper functions having cck field deinitions.
 * The voipextension module calls the function of this file to add/edit/delete
 * the custom cck fields
 *
 */

/**
 * VoIP Extension CCK Field
 * @param $node_type
 * @return
 *   An array having cck definition of the voip extension.
 */
function _voipextension_main_field_definition($node_type) {
  $default_script_name = variable_get('default_script_name_'. $node_type, variable_get('voipextension_script_name', ''));
  $default_extension_status = variable_get('default_extension_status_'. $node_type, variable_get('voipextension_status', 0));

  return array (
    'field_name' => VOIPEXTENSION_EXTENSION_MAIN_FIELD,
    'type_name' => $node_type,
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
    'type' => 'voipextension',
    'required' => '0',
    'multiple' => '0',
    'db_storage' => '1',
    'module' => 'voipextension',
    'active' => '1',
    'locked' => '0',
    'columns' =>
    array (
      'veid' =>
      array (
        'type' => 'int',
        'unsigned' => true,
        'not null' => false,
      ),
    ),
    'veid' => '',
    'widget' =>
    array (
      'default_value' =>
      array (
        0 =>
        array (
          'script_name' => $default_script_name,
          'extension_status' => $default_extension_status,
          'cck_preview_in_progress' => true,
          'voipextension_settings' =>
          array (
            'form' =>
            array (
              'fields' =>
              array (
                'script_name' =>
                array (
                  'default' => $default_script_name,
                  'collect' => 1,
                  'weight' => 4,
                ),
                'extension_status' =>
                array (
                  'default' => $default_extension_status,
                  'collect' => 1,
                  'weight' => 5,
                ),
                'cck_preview_in_progress' =>
                array (
                  'default' => true,
                ),
                'voipextension_settings' =>
                array (
                  'default' =>
                  array (
                    'form' =>
                    array (
                      'fields' =>
                      array (
                        'veid' =>
                        array (
                          'default' => false,
                        ),
                        'extension_name' =>
                        array (
                          'default' => 'Extension Name',
                          'collect' => 1,
                          'weight' => 2,
                        ),
                        'script_name' =>
                        array (
                          'default' => $default_script_name,
                          'collect' => 1,
                          'weight' => 4,
                        ),
                        'extension_status' =>
                        array (
                          'default' => $default_extension_status,
                          'collect' => 1,
                          'weight' => 5,
                        ),
                      ),
                    ),
                  ),
                ),
                'extension_name' =>
                array (
                  'default' => 'Extension Name',
                  'collect' => 1,
                  'weight' => 2,
                ),
                'veid' =>
                array (
                  'default' => false,
                ),
              ),
            ),
          ),
          'extension_name' => 'Extension Name',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'VoIP Settings',
      'weight' => '3',
      'description' => '',
      'type' => 'voipextension',
      'module' => 'voipextension',
    ),
  );
}

/**
 * Contains the fieldgroup cck field structure.
 * This is the exported form of cck field.
 *
 * @return
 *   An array having fielgroup cck structure.
 */
function _voipextension_extension_group_defintion() {
  return array (
    'label' => 'VoIP Extension Settings',
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
    'weight' => '7',
    'group_name' => VOIPEXTENSION_GROUP_FIELD_NAME,
  );
}

/**
 * VoIP Directory fields will be enclosed under this group.
 *
 * @return
 *   An array of the cck fieldgroup representation.
 */
function _voipextension_voipdirectory_settings_group() {
  return array (
    'label' => 'VoIP Directory Settings',
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
    'weight' => '13',
    'group_name' => VOIPEXTENSION_VOIPDIRECTORY_GROUP_FIELD_NAME
  );
}

/**
 * CCK definition of the option widget from number module.
 *
 * @return
 *   An array having the option elements in structured way.
 */
function _voipextension_extension_listed_definition() {
  return array (
    'field_name' => VOIPEXTENSION_EXTENSION_LISTED,
    'type_name' => 'page',
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
    'allowed_values' => '0|No
  1|Yes',
    'allowed_values_php' => '',
    'widget' =>
    array (
      'default_value' =>
      array (
        0 =>
        array (
          'value' => '1',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'List extension in phone directory?',
      'weight' => '3',
      'description' => '',
      'type' => 'optionwidgets_buttons',
      'module' => 'optionwidgets',
    ),
  );
}



/**
 * Store and returns field definitions, from audiofield cck fields
 * It stores three cck fields, Audio name, Audio description and Audio greeting
 * @param $field_type
 *   If the field type is specified then it returns only specific audio field.
 * @return
 *   An array having the structured format of all or the required audio cck fields.
 */
function _voipextension_audio_fields_definitions($field_type = '') {
  $fields = array();
  $fields['audio_name'] =  array (
    'label' => 'Audio name',
    'field_name' => VOIPEXTENSION_AUDIO_NAME,
    'type' => 'filefield',
    'widget_type' => 'audiofield_widget',
    'change' => 'Change basic information',
    'weight' => '8',
    'file_extensions' => 'mp3',
    'progress_indicator' => 'bar',
    'file_path' => '',
    'max_filesize_per_file' => '',
    'max_filesize_per_node' => '',
    'description' => '',
    'group' => VOIPEXTENSION_GROUP_FIELD_NAME,
    'required' => 0,
    'multiple' => '0',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'op' => 'Save field settings',
    'module' => 'filefield',
    'widget_module' => 'audiofield',
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
  );

  $fields['audio_description'] = array (
    'label' => 'Audio Description',
    'field_name' => VOIPEXTENSION_AUDIO_DESCRIPTION,
    'type' => 'filefield',
    'widget_type' => 'audiofield_widget',
    'change' => 'Change basic information',
    'weight' => '9',
    'file_extensions' => 'mp3',
    'progress_indicator' => 'bar',
    'file_path' => '',
    'max_filesize_per_file' => '',
    'max_filesize_per_node' => '',
    'description' => '',
    'group' => VOIPEXTENSION_GROUP_FIELD_NAME,
    'required' => 0,
    'multiple' => '0',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'op' => 'Save field settings',
    'module' => 'filefield',
    'widget_module' => 'audiofield',
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
  );

  $fields['audio_greeting'] = array (
    'label' => 'Audio Greeting',
    'field_name' => VOIPEXTENSION_AUDIO_GREETING,
    'type' => 'filefield',
    'widget_type' => 'audiofield_widget',
    'change' => 'Change basic information',
    'weight' => '10',
    'file_extensions' => 'mp3',
    'progress_indicator' => 'bar',
    'file_path' => '',
    'max_filesize_per_file' => '',
    'max_filesize_per_node' => '',
    'description' => '',
    'group' => VOIPEXTENSION_GROUP_FIELD_NAME,
    'required' => 0,
    'multiple' => '0',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'op' => 'Save field settings',
    'module' => 'filefield',
    'widget_module' => 'audiofield',
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
  );

  if (isset($fields[$field_type])) {
    return $fields[$field_type];
  }
  else {
    return $fields;
  }
}