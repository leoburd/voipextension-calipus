<?php
/**
 * @file
 *
 * Voicemail Database operations.
 */


/**
 * Save the voicemailbox settings to the database for each voip extension.
 * @param $voicemailbox
 *   An array having all the required information to setup voicemail.
 * @return
 *   An integer having the number of rows affected usually one if it is
 *   success else boolean false
 */
function voicemail_save($voicemailbox) {
  if (isset($voicemailbox['veid']) && trim($voicemailbox['veid']) != '') {
    $voicemail_settings = voicemail_load_settings($voicemailbox['veid']);
    // Already exists so just update the voicemailbox settings.
    if (is_object($voicemail_settings) && isset($voicemail_settings->veid)) {
      $args = array();
      $args[] = $voicemailbox['send_notifications'] ? 1 : 0;
      $args[] = $voicemailbox['is_enabled'] ? 1 : 0;
      $args[] = $voicemailbox['audio_greetings_fid'] ? $voicemailbox['audio_greetings_fid'] : 0;
      $args[] = $voicemail_settings->veid;

      db_query('UPDATE {voicemailbox} SET send_notifications = %d, is_enabled = %d, audio_greetings_fid = %d WHERE veid = %d', $args);
      return db_affected_rows();
    }
    else {
      // Insert a new entry.
      $args = array();
      $args[] = $voicemailbox['veid'];
      $args[] = $voicemailbox['send_notifications'] ? 1 : 0;
      $args[] = $voicemailbox['is_enabled'] ? 1 : 0;
      $args[] = $voicemailbox['audio_greetings_fid'] ? $voicemailbox['audio_greetings_fid'] : 0;

      db_query('INSERT INTO {voicemailbox} (veid, send_notifications, is_enabled, audio_greetings_fid) VALUES (%d, %d, %d, %d)', $args);
      return db_affected_rows();
    }
  }

  return FALSE;
}

/**
 * Load the voicemail related settings.
 *
 * @param $veid
 *   An integer holding voip extension number
 *
 * @return
 *   The selected row of voip extension in an object format if success else boolean FALSE.
 */
function voicemail_load_settings($veid) {
  $query = 'SELECT * FROM {voicemailbox} WHERE veid = %d';
  $result = db_query($query, $veid);
  if ($result) {
    return db_fetch_object($result);
  }
  else {
    return FALSE;
  }
}

/**
 * Save the message sent from sender to the reciever.
 *
 * @param $voicemail_message
 *   An array holding the audio file id.
 * @return
 *   An integer having the message id or boolean FALSE.
 */
function voicemail_messages_save($voicemail_message) {
  $result = db_query('INSERT INTO {voicemail_messages} (audio_message_fid) VALUES (%d)', $voicemail_message['audio_message_fid']);
  if ($result) {
    return db_last_insert_id('voicemail_messages', 'mid');
  }
  else {
    return FALSE;
  }
}

/**
 * Save the message entries to the outbox/sent folder.
 *
 * @param $voicemail_outbox
 *   An array having voipextension number and message id.
 * @return
 *   An autoincremented key if saved properly else FALSE.
 */
function voicemail_outbox_save($voicemail_outbox) {
  $query = 'INSERT INTO {voicemail_outbox} (veid, mid, sent_on) VALUES (%d, %d, %d)';
  $result = db_query($query, $voicemail_outbox['veid'], $voicemail_outbox['mid'], time());
  if ($result) {
    return db_last_insert_id('voicemail_outbox', 'id');
  }
  else {
    return FALSE;
  }
}

/**
 * Save the message attributes in the Inbox/Recieved folder.
 *
 * @param $voicemail_inbox
 *   An array having the voipextension number, message id and other attributes
 *   required for inbox folder.
 * @return
 *   An autoincremented key if saved else FALSE.
 */
function voicemail_inbox_save($voicemail_inbox) {
  $query = 'INSERT INTO {voicemail_inbox} (veid, mid, caller_name, caller_number, is_new, received_on) VALUES (%d, %d, "%s", "%s", %d, %d)';
  $args = array();
  $args[] = $voicemail_inbox['veid'];
  $args[] = $voicemail_inbox['mid'];
  $args[] = $voicemail_inbox['caller_name'];
  $args[] = $voicemail_inbox['caller_number'];
  $args[] = 1;
  $args[] = time();

  $result = db_query($query, $args);
  if ($result) {
    return db_last_insert_id('voicemail_inbox', 'id');
  }
  else {
    return FALSE;
  }
}
