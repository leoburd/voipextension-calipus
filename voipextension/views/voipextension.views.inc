<?php
/**
 * @file
 * 
 * Views 2 support for Voip Extension.
 */

/**
 * Implementation of hook_views_data().
 */
function voipextension_views_data() {
  // ----------------------------------------------------------------
  // voipextensions table -- basic table information.

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.

  $data['voipextensions']['table']['group'] = t('Voip Extension');

  // Advertise this table as a possible base table
  $data['voipextensions']['table']['base'] = array(
    'field' => 'veid',
    'title' => t('Voip Extension'),
    'help' => t('Voip Extensions are addresses of phone numbers.'),
    'weight' => -10,
  );

  $data['voipextensions']['table']['join'] = array(
    // Voip Extension links to node through voipextensions_instance via lid.
    'node' => array(
      'left_table' => 'voipextension_instance',
      'left_field' => 'veid',
      'field' => 'veid',
    ),
    // Voip Extension links to node_revisions through voipextensions_instance via lid.
    'node_revisions' => array(
      'left_table' => 'voipextension_instance',
      'left_field' => 'veid',
      'field' => 'veid',
    ),
    // Voip Extension links to users through voipextensions_instance via lid.
    'users' => array(
      'left_table' => 'voipextension_instance',
      'left_field' => 'veid',
      'field' => 'veid',
    ),
  );

  // ----------------------------------------------------------------
  // voipextensions table -- fields

  // veid
  $data['voipextensions']['veid'] = array(
    'title' => t('Veid'),
    'help' => t('The Voip Extension ID/Number of the voip extension node/user.'),
    // Information for displaying the veid
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Information for accepting a veid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => FALSE,
    ),
    // Information for sorting on a veid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['voipextensions']['extension_name'] = array(
    'title' => t('Extension Name'),
    'help' => t('The name of the voip extension.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['voipextensions']['script_name'] = array(
    'title' => t('Script Name'),
    'help' => t('The script name associated to voip extension.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['voipextensions']['extension_status'] = array(
    'title' => t('Extension Status'),
    'help' => t('status of a voip extension.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['voipextensions']['extension_listed'] = array(
    'title' => t('Extension Listed in Phone directory'),
    'help' => t('status of a voip extension in phone directory.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['voipextensions']['audio_name_fid'] = array(
    'title' => t('Audio name: Reference to {files}.fid'),
    'help' => t('Audio name: Reference to {files}.fid.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['voipextensions']['audio_description_fid'] = array(
    'title' => t('Audio description: Reference to {files}.fid'),
    'help' => t('Audio description: Reference to {files}.fid.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['voipextensions']['audio_greeting_fid'] = array(
    'title' => t('Audio greeting: Reference to {files}.fid'),
    'help' => t('Audio greeting: Reference to {files}.fid.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['voipextensions_instance']['table']['group']  = t('Voip Extension');

  $data['voipextensions_instance']['table']['join'] = array(
    'voipextensions' => array(
      'left_field' => 'veid',
      'field' => 'veid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node_revisions' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );


  // Tell the base tables about voipextensions.
  $data['node']['table']['join']['voipextensions'] = array(
    'left_table' => 'voipextensions_instance',
    'left_field' => 'vid',
    'field' => 'vid',
  );
  $data['node_revisions']['table']['join']['voipextensions'] = array(
    'left_table' => 'voipextensions_instance',
    'left_field' => 'vid',
    'field' => 'vid',
  );
  $data['users']['table']['join']['voipextensions'] = array(
    'left_table' => 'voipextensions_instance',
    'left_field' => 'uid',
    'field' => 'uid',
  );

  return $data;
}