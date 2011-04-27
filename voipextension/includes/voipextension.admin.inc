<?php
/**
 * @file
 *
 * Admin page callback file for the VoIP extension module.
 */

/**
 * Configure VoIP extension settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function voipextension_admin_settings() {
  drupal_set_title(t('Default VoIP Extension Settings'));
  $form['voipextension'] = array(
    '#title' => t('VoIP Extension Settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
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
    '#options' => array(1 => t('Enable'), 0 => t('Disable')),
    '#default_value' => variable_get('voipextension_status', 1)
  );

  return system_settings_form($form);
}

function voipextension_manage_extensions(&$form_state) {
  $form = array();

  $help_message = 'If you would like to Add VoIP Extensions to your existing users and nodes then you can use "Add VoIP Extensions"';
  $help_message .= ' option from here. This will assigns the VoIP Extensions to your current active users and nodes';

  $form['help'] = array(
    '#type' => 'markup',
    '#value' => '<div>'. t($help_message) .'</div>'
  );

  $form['add_extensions'] = array(
    '#type' => 'submit',
    '#value' => t('Add VoIP Extensions for Users and Nodes')
  );
  return $form;
}

/**
 * Add VoIP Extensions for users and nodes.
 */
function voipextension_manage_extensions_submit($form, &$form_state) {
  module_load_include('inc', 'voipextension', 'includes/voipextension.helper');
  module_load_include('inc', 'voipextension', 'includes/voipextension.api');
  $content_type_info = _content_type_info(TRUE);

  switch ($form_state['values']['op']) {
    case 'Add VoIP Extensions for Users and Nodes':
      $users_updated_counter = 0;
      $nodes_updated_counter = 0;
      $voipextension_listed_field = VOIPEXTENSION_EXTENSION_LISTED;

      $available_content_types = node_get_types();
      $voipextension_content_types = array();
      foreach ($available_content_types as $node_type => $node_type_details) {
        if (variable_get('voip_extension_use_'. $node_type, FALSE)) {
          $voipextension_content_types[$node_type] = $node_type_details;
        }
      }

      // If not even a single content type is available having VoIP Extension
      // attached to it, then simply throw an error message.
      if (count($voipextension_content_types) <= 0) {
        drupal_set_message(t('Sorry, There is no content type available for VoIP Extension'), 'error');
        return FALSE;
      }

// First iterate for Users only

      // If a content profile exists and thats available as a content type.
      $content_profile = voipextension_user_profile_content_type();
      if (array_key_exists($content_profile, $voipextension_content_types)) {
        $content_type_details = $content_type_info['content types'][$content_profile];

        $user_query = 'SELECT u.*, GROUP_CONCAT(ur.rid ORDER BY ur.rid ASC SEPARATOR ",") AS roles_assigned FROM {users} AS u ';
        $user_query .= 'LEFT JOIN {users_roles} AS ur ON u.uid = ur.uid WHERE status = %d GROUP BY u.uid ORDER BY u.uid ASC';
        $result = db_query($user_query, 1);
        while ($row = db_fetch_object($result)) {
          $temp_account = '';
          $default_user_roles = array(
            DRUPAL_ANONYMOUS_RID => NULL,
            DRUPAL_AUTHENTICATED_RID => NULL,
          );
          $temp_account = $row;
          if (isset($temp_account->roles_assigned) && $temp_account->roles_assigned != '') {
            $temp_roles_array = explode(',', $temp_account->roles_assigned);
            foreach ($temp_roles_array as $index => $role_id) {
              $default_user_roles[$role_id] = NULL;
            }
            $temp_account->roles = $default_user_roles;
          }
          else {
            $temp_account->roles = $default_user_roles;
          }

          if (user_access(VOIPEXTENSION_HAVE_PERSONAL_EXTENSION, $temp_account)) {
            $voip_extension = VoipExtension::getVoipExtensionFromUid($temp_account->uid);
            // Voip Extension doesn't exists for the user and the user is having the proper permissions.
            if (!$voip_extension) {
              // Create a voip extension for the selected user.
              if (isset($content_type_details['fields']) && is_array($content_type_details['fields'])) {
                foreach ($content_type_details['fields'] as $field_name => $field_details) {
                  if ($field_details['type'] == 'voipextension' && $field_details['module'] == 'voipextension') {
                    $user_node = content_profile_load($content_profile, $temp_account->uid);
                    if (isset($user_node->nid)) {
                      if ((isset($user_node->$field_name) && is_array($user_node->$field_name) && count($user_node->$field_name) <= 0) || !isset($user_node->$field_name)) {
                        $voip_extension_fields_array = array();
                        $user_node->$field_name = array();

                        $voip_extension_fields_array[0]['extension_name'] = $user_node->title;
                        $voip_extension_fields_array[0]['script_name'] = $field_details['widget']['default_value'][0]['script_name'];
                        $voip_extension_fields_array[0]['extension_status'] = $field_details['widget']['default_value'][0]['extension_status'];
                        $voip_extension_fields_array[0]['extension_listed'] = 0;
                        $voip_extension_fields_array[0]['audio_name_fid'] = 0;
                        $voip_extension_fields_array[0]['audio_description_fid'] = 0;
                        $voip_extension_fields_array[0]['audio_greeting_fid'] = 0;

                        $user_node->$field_name = $voip_extension_fields_array;
                      }

                      if (isset($user_node->$voipextension_listed_field)) {
                        $voipextension_listed_field_array = $user_node->$voipextension_listed_field;
                        $voipextension_listed_field_array[0]['value'] = 0;
                        $user_node->$voipextension_listed_field = $voipextension_listed_field_array;
                      }
                      else {
                        $user_node->$voipextension_listed_field = array(
                          0 => array(
                            'value' => 0
                          )
                        );
                      }
                      $user_node = node_submit($user_node);
                      node_save($user_node);
                    }
                    else {
                      $new_node = new stdClass();
                      $new_node->title = $temp_account->name;
                      $new_node->body = '';
                      $new_node->uid = $temp_account->uid;
                      $new_node->name = $temp_account->name;
                      $new_node->type = $content_profile;
                      $new_node->format = 1;     // 1 means filtered html, 2 means full html, 3 is php
                      $new_node->status = 1;     // 1 means published
                      $new_node->promote = 0;

                      if (!isset($new_node->$field_name)) {
                        $voip_extension_fields_array = array();
                        $new_node->$field_name = array();

                        $voip_extension_fields_array[0]['extension_name'] = $new_node->title;
                        $voip_extension_fields_array[0]['script_name'] = $field_details['widget']['default_value'][0]['script_name'];
                        $voip_extension_fields_array[0]['extension_status'] = $field_details['widget']['default_value'][0]['extension_status'];
                        $voip_extension_fields_array[0]['extension_listed'] = 0;
                        $voip_extension_fields_array[0]['audio_name_fid'] = 0;
                        $voip_extension_fields_array[0]['audio_description_fid'] = 0;
                        $voip_extension_fields_array[0]['audio_greeting_fid'] = 0;

                        $new_node->$field_name = $voip_extension_fields_array;
                      }

                      if (isset($new_node->$voipextension_listed_field)) {
                        $voipextension_listed_field_array = $new_node->$voipextension_listed_field;
                        $voipextension_listed_field_array[0]['value'] = 0;
                        $new_node->$voipextension_listed_field = $voipextension_listed_field_array;
                      }
                      else {
                        $new_node->$voipextension_listed_field = array(
                          0 => array(
                            'value' => 0
                          )
                        );
                      }
                      $new_node = node_submit($new_node);
                      node_save($new_node);
                    }
                    $users_updated_counter++;
                  }
                }
              }
            }
          }
        }
      }

      // Filter out user profiles from the content types.
      $voipextension_node_content_types = array();
      foreach ($voipextension_content_types as $node_type => $node_type_details) {
        if (!is_content_profile($node_type)) {
          $voipextension_node_content_types[$node_type] = $node_type_details;
        }
      }

      if (count($voipextension_node_content_types) > 0) {
        foreach ($voipextension_node_content_types as $node_type => $node_type_details) {

          $content_type_details = array();
          $content_type_details = $content_type_info['content types'][$node_type];

          if (isset($content_type_details['fields']) && is_array($content_type_details['fields'])) {
            foreach ($content_type_details['fields'] as $field_name => $field_details) {
              if ($field_details['type'] == 'voipextension' && $field_details['module'] == 'voipextension') {

                $query = 'SELECT nid FROM {node} WHERE status = %d AND type = "%s" ORDER BY nid ASC';
                $result = db_query($query, 1, $node_type);
                while ($node_data = db_fetch_object($result)) {
                  $nid = $node_data->nid;
                  $node = node_load(array('nid' => $nid));
                  if ((isset($node->$field_name) && is_array($node->$field_name) && count($node->$field_name) <= 0) || !isset($node->$field_name)) {
                    $voip_extension_fields_array = array();
                    $node->$field_name = array();

                    $voip_extension_fields_array[0]['extension_name'] = $node->title;
                    $voip_extension_fields_array[0]['script_name'] = $field_details['widget']['default_value'][0]['script_name'];
                    $voip_extension_fields_array[0]['extension_status'] = $field_details['widget']['default_value'][0]['extension_status'];
                    if (isset($node->$voipextension_listed_field)) {
                      $extension_listed_default = $node->$voipextension_listed_field;
                      $voip_extension_fields_array[0]['extension_listed'] = $extension_listed_default[0]['value'];
                    }
                    else {
                      $voip_extension_fields_array[0]['extension_listed'] = 0;
                    }
                    $voip_extension_fields_array[0]['audio_name_fid'] = 0;
                    $voip_extension_fields_array[0]['audio_description_fid'] = 0;
                    $voip_extension_fields_array[0]['audio_greeting_fid'] = 0;

                    $node->$field_name = $voip_extension_fields_array;

                    if (isset($node->$voipextension_listed_field)) {
                      $voipextension_listed_field_array = $node->$voipextension_listed_field;
                      $voipextension_listed_field_array[0]['value'] = 0;
                      $node->$voipextension_listed_field = $voipextension_listed_field_array;
                    }
                    else {
                      $node->$voipextension_listed_field = array(
                        0 => array(
                          'value' => 0
                        )
                      );
                    }
                    $node = node_submit($node);
                    node_save($node);
                    $nodes_updated_counter++;
                  }
                }
              }
            }
          }
        }
      }

      drupal_set_message(t('@users_updated_counter Users and @nodes_updated_counter Nodes have been assigned the VoIP Extension number', array('@users_updated_counter' => $users_updated_counter, '@nodes_updated_counter' => $nodes_updated_counter)));
      break;
  }
}