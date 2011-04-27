<?php
/**
 * @file
 *
 * Helper file to do all Database level operations on Voicemail tables
 */

/**
 * Save helper for Voicemail
 * It checks that if voicemail already exists for the provided VoIP Extension
 * then simply updates the Voicemail otherwise make a new entry
 *
 * @param $voicemail
 *   An array having all the required information to save Voicemail
 * @return
 *   Boolean TRUE if saved else FALSE
 */
function voicemail_save($voicemail) {
  $voicemail_obj = voicemail_load($voicemail['veid']);
  // Update Query
  if (isset($voicemail_obj->veid)) {
    $query = 'UPDATE {voicemailbox} SET send_notifications = %d, is_enabled = %d, audio_greetings_fid = %d, status = %d';
    $query .= ' WHERE veid = %d';

    $args = array();
    $args[] = $voicemail['send_notifications'] ? 1 : 0;
    $args[] = $voicemail['is_enabled'];
    $args[] = $voicemail['audio_greetings_fid'];
    $args[] = $voicemail['status'] ? 1 : 0;
    $args[] = $voicemail['veid'];
  }
  else { // Insert Query
    $query = 'INSERT INTO {voicemailbox} (veid, send_notifications, is_enabled, audio_greetings_fid, status) VALUES (%d, %d, %d';
    $query .= ', %d, %d)';
    $args = array();
    $args[] = $voicemail['veid'];
    $args[] = $voicemail['send_notifications'] ? 1 : 0;
    $args[] = $voicemail['is_enabled'];
    $args[] = $voicemail['audio_greetings_fid'];
    $args[] = $voicemail['status'] ? 1 : 0;
  }
  $result = db_query($query, $args);

  if (db_affected_rows() > 0) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Load the Voicemail data in an object from the VoIP Extension provided
 * @param $veid
 *   An integer valued VoIP Extension number
 * @return
 *   An object.
 */
function voicemail_load($veid) {
  $query = 'SELECT * FROM {voicemailbox} WHERE veid = %d';
  $voicemail = db_fetch_object(db_query($query, $veid));
  return $voicemail;
}

/**
 * Deletes the Voicemail data.
 * @param $veid
 *   An integer valued VoIP Extension number
 * @return
 *   A boolean TRUE if success else FALSE
 */
function voicemail_delete($veid) {
  db_query('DELETE FROM {voicemailbox} WHERE veid = %d', $veid);
  if (db_affected_rows() > 0) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}