<?php
/**
 * @file
 *
 * Views 2 support for Voice Mail
 */

/**
 * Implementation of hook_views_data().
 */
function voicemail_views_data() {
  // ----------------------------------------------------------------
  // voicemail_sent table -- basic table information.

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.

  $data['voicemail_sent']['table']['group'] = t('VoiceMail Sent');

  // Advertise this table as a possible base table
  $data['voicemail_sent']['table']['base'] = array(
    'field' => 'mid',
    'title' => t('VoiceMail Sent'),
    'help' => t('Messages sent have unique message id associated with them.'),
  );

  $data['voicemail_sent']['table']['join'] = array(
    // VoiceMail Outbox links to node through voipextensions_instance via veid.
    'node' => array(
      'left_table' => 'voipextension_instance',
      'left_field' => 'veid',
      'field' => 'veid',
    ),
    // VoiceMail Outbox links to node_revisions through voipextensions_instance via veid.
    'node_revisions' => array(
      'left_table' => 'voipextension_instance',
      'left_field' => 'veid',
      'field' => 'veid',
    ),
    // VoiceMail Outbox links to users through voipextensions_instance via veid.
    'users' => array(
      'left_table' => 'voipextension_instance',
      'left_field' => 'veid',
      'field' => 'veid',
    )
  );

  // ----------------------------------------------------------------
  // voipextensions table -- fields

  $data['voicemail_sent']['mid'] = array(
    'title' => t('Message Id'),
    'help' => t('A unique integer message id.'),
    // Information for displaying the mid
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Information for accepting a mid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => FALSE,
    ),
    // Information for sorting on a mid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['voicemail_sent']['veid'] = array(
    'title' => t('Veid'),
    'help' => t('The VoIP Extension ID/Number of the VoIP extension node/user.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['voicemail_sent']['audio_message_fid'] = array(
    'title' => t('Audio Message'),
    'help' => t('Audio message sent in the message.'),
    // Information for displaying the audio_message_fid
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Information for accepting a audio_message_fid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => FALSE,
    ),
    // Information for sorting on a audio_message_fid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['voicemail_sent']['caller_name'] = array(
    'title' => t('Caller Name'),
    'help' => t('Caller Name of the voicemail.'),
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

  $data['voicemail_sent']['caller_number'] = array(
    'title' => t('Caller Number'),
    'help' => t('Caller Number of the voicemail.'),
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

  $data['voicemail_sent']['sent_on'] = array(
    'title' => t('Sent On'),
    'help' => t('Date and time on which voicemail was sent.'),
    // Information for displaying the sent_on
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Information for accepting a sent_on as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => FALSE,
    ),
    // Information for sorting on a sent_on.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  $data['voicemail_inbox']['table']['group'] = t('VoiceMail Received');

  // Advertise this table as a possible base table
  $data['voicemail_inbox']['table']['base'] = array(
    'field' => 'id',
    'title' => t('VoiceMail Received'),
    'help' => t('Messages received from voicemail senders.'),
  );

  $data['voicemail_inbox']['table']['join'] = array(
    // VoiceMail Outbox links to node through voipextensions_instance via veid.
    'node' => array(
      'left_table' => 'voipextension_instance',
      'left_field' => 'veid',
      'field' => 'veid',
    ),
    // VoiceMail Outbox links to node_revisions through voipextensions_instance via veid.
    'node_revisions' => array(
      'left_table' => 'voipextension_instance',
      'left_field' => 'veid',
      'field' => 'veid',
    ),
    // VoiceMail Outbox links to users through voipextensions_instance via veid.
    'users' => array(
      'left_table' => 'voipextension_instance',
      'left_field' => 'veid',
      'field' => 'veid',
    )
  );

  $data['voicemail_inbox']['mid'] = array(
    'title' => t('Message Id'),
    'help' => t('A unique integer message id.'),
    // Information for displaying the mid
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Information for accepting a mid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => FALSE,
    ),
    // Information for sorting on a mid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['voicemail_inbox']['veid'] = array(
    'title' => t('Veid'),
    'help' => t('The VoIP Extension ID/Number of the VoIP extension node/user.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['voicemail_inbox']['is_new'] = array(
    'title' => t('Is New'),
    'help' => t('The message received is new or read already.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['voicemail_inbox']['received_on'] = array(
    'title' => t('Received On'),
    'help' => t('The date and time on which this message was received.'),
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


  $data['voipextensions_instance']['table']['group']  = t('VoiceMail Received');

  $data['voipextensions_instance']['table']['join'] = array(
    'voicemail_sent' => array(
      'left_field' => 'veid',
      'field' => 'veid',
    ),
    'voicemail_inbox' => array(
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

  // Tell the base tables about voicemail_sent.
  $data['node']['table']['join']['voicemail_sent'] = array(
    'left_table' => 'voipextensions_instance',
    'left_field' => 'vid',
    'field' => 'vid',
  );
  $data['node_revisions']['table']['join']['voicemail_sent'] = array(
    'left_table' => 'voipextensions_instance',
    'left_field' => 'vid',
    'field' => 'vid',
  );
  $data['users']['table']['join']['voicemail_sent'] = array(
    'left_table' => 'voipextensions_instance',
    'left_field' => 'uid',
    'field' => 'uid',
  );

  // Tell the base tables about voicemail_sent.
  $data['node']['table']['join']['voicemail_inbox'] = array(
    'left_table' => 'voipextensions_instance',
    'left_field' => 'vid',
    'field' => 'vid',
  );
  $data['node_revisions']['table']['join']['voicemail_inbox'] = array(
    'left_table' => 'voipextensions_instance',
    'left_field' => 'vid',
    'field' => 'vid',
  );
  $data['users']['table']['join']['voicemail_inbox'] = array(
    'left_table' => 'voipextensions_instance',
    'left_field' => 'uid',
    'field' => 'uid',
  );
  $data['voicemail_sent']['table']['join']['voicemail_inbox'] = array(
//    'left_table' => 'voipextensions_instance',
    'left_field' => 'mid',
    'field' => 'mid',
  );

  return $data;
}