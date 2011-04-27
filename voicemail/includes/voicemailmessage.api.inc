<?php
/**
 * @file
 *
 * Voice Mail Message API to do operations on the Messages sent and Received
 */

class VoicemailMessage {
  // Message Status Read/Unread
  const STATUS_NEW = 1;
  const STATUS_READ = 0;

  public $mid; // Message Id
  public $veid; // VoIP Extension Id
  public $audioMessageFid; // Audio message Fid
  public $callerName; // Caller Name from the VoIP Extension
  public $callerNumber; // Caller Phone Number if any
  public $sentOn; // Message sent date

  function __construct($mid = '') {
    if (isset($mid) && trim($mid) != '' && ctype_digit((string) $mid)) {
      $message = self::loadMessage($mid);
      $this->mid = $mid;
      $this->veid = $message->veid;
      $this->audioMessageFid = $message->audio_message_fid;
      $this->callerName = $message->caller_name;
      $this->callerNumber = $message->caller_number;
      $this->sentOn = $message->sent_on;
    }
  }

  /**
   * Finds all the details of selected message
   *
   * @param $mid
   *   An integer having message id.
   *
   * @return
   *   An object having all the details of the message if success else Boolean FALSE
   */
  public static function loadMessage($mid) {
    $query = 'SELECT * FROM {voicemail_sent} WHERE mid = %d';
    $result = db_query($query, $mid);
    if ($result) {
      return db_fetch_object($result);
    }
    else {
      return FALSE;
    }
  }

  /**
   * Save the message that has to sent to the Recepient
   *
   * @return
   *   Message id if succeeds else FALSE
   */
  public function saveSentMessage() {
    $args = array();
    if (isset($this->mid) && $this->mid != '' && ctype_digit((string) $this->mid)) {
      $query = 'UPDATE {voicemail_sent} SET veid = %d, audio_message_fid = %d, caller_name = "%s", caller_number = "%s"';
      $query .= ' WHERE mid = %d';

      $args[] = $this->veid;
      $args[] = $this->audioMessageFid;
      $args[] = $this->callerName;
      $args[] = $this->callerNumber;
      $args[] = $this->mid;

      db_query($query, $args);
      if ($row_updated = db_affected_rows()) {
        return $row_updated;
      }
    }
    else {
      $query = 'INSERT INTO {voicemail_sent} (veid, audio_message_fid, caller_name, caller_number, sent_on) VALUES (%d, %d,';
      $query .= ' "%s", "%s", %d)';

      $args[] = $this->veid;
      $args[] = $this->audioMessageFid;
      $args[] = $this->callerName;
      $args[] = $this->callerNumber;
      $args[] = time();

      db_query($query, $args);
      if ($last_mid = db_last_insert_id('voicemail_sent', 'mid')) {
        return $last_mid;
      }
    }
    return FALSE;
  }

  /**
   * Save the message to the recipent MailBox
   * @param $inbox_id
   *   Inbox Message id
   * @param $is_new
   *   Message is new or read already
   *
   * @return
   *   Inbox message id or FALSE
   */
  public function saveInboxMessage($inbox_mid = '', $is_new = self::STATUS_NEW) {
    $args = array();
    if (isset($inbox_mid) && $inbox_mid != '' && ctype_digit((string) $inbox_mid)) {
      $query = 'UPDATE {voicemail_inbox} SET veid = %d, mid = %d, is_new = %d WHERE id = %d';

      $args[] = $this->veid;
      $args[] = $this->mid;
      $args[] = $is_new;
      $args[] = $inbox_mid;

      db_query($query, $args);
      if ($row_updated = db_affected_rows()) {
        return $row_updated;
      }
    }
    else {
      $query = 'INSERT INTO {voicemail_inbox} (veid, mid, is_new, received_on) VALUES (%d, %d, %d, %d)';

      $args[] = $this->veid;
      $args[] = $this->mid;
      $args[] = $is_new;
      $args[] = time();

      db_query($query, $args);
      if ($last_id = db_last_insert_id('voicemail_inbox', 'id')) {
        return $last_id;
      }
    }
    return FALSE;
  }

  /**
   * Delete a Voice Mail sent message, If a message is deleted from Sent
   * folder then all the messages corresponding to Inbox will also be deleted
   *
   * @param $mid
   *   An integer message id.
   * @param $veid
   *   An integer VoIP Extension Number
   *
   * @return
   *   Boolean TRUE or FALSE
   */
  public static function deleteSentMessage($mid = '', $veid = '') {
    if (isset($mid) && $mid != '' && ctype_digit((string) $mid)) {
      db_query('DELETE FROM {voicemail_sent} WHERE mid = %d', $mid);
      self::deleteInboxMessage('', $mid, '');
      if (db_affected_rows()) {
        return TRUE;
      }
      else {
        return FALSE;
      }
    }
    elseif (isset($veid) && $veid != '' && ctype_digit((string) $veid)) {
      db_query('DELETE FROM {voicemail_sent} WHERE veid = %d', $veid);
      self::deleteInboxMessage('', '', $veid);
      if (db_affected_rows()) {
        return TRUE;
      }
      else {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }

  /**
   * Delete a message from Inbox folder.
   * @param $inbox_mid
   *   Inbox message id, An integer value
   * @param $mid
   *   Message id from the Sent folder
   * @param $veid
   *   An integer VoIP Extension
   * @return
   *   Boolean TRUE or FALSE
   */
  public static function deleteInboxMessage($inbox_mid = '', $mid = '', $veid = '') {
    if (isset($inbox_mid) && $inbox_mid != '' && ctype_digit((string) $inbox_mid)) {
      db_query('DELETE FROM {voicemail_inbox} WHERE id = %d', $inbox_mid);
      if (db_affected_rows()) {
        return TRUE;
      }
      else {
        return FALSE;
      }
    }
    elseif (isset($mid) && $mid != '' && ctype_digit((string) $mid)) {
      db_query('DELETE FROM {voicemail_inbox} WHERE mid = %d', $mid);
      if (db_affected_rows()) {
        return TRUE;
      }
      else {
        return FALSE;
      }
    }
    elseif (isset($veid) && $veid != '' && ctype_digit((string) $veid)) {
      db_query('DELETE FROM {voicemail_inbox} WHERE veid = %d', $veid);
      if (db_affected_rows()) {
        return TRUE;
      }
      else {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }
}

