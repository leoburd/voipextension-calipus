<?php
/**
 * @file
 *
 * Voice Mail Box API
 */

// Include VoIP Extension API.
module_load_include('inc', 'voipextension', 'includes/voipextension.api');

class VoicemailBox {
  private $veid;
  private $extensionNid;
  private $extensionUid;

  public $sendNotifications;
  public $isEnabled;
  public $audioGreetingsFid;
  public $status;

  public function __construct($veid = '', $nid = '', $uid = '') {
    if (isset($veid) && trim($veid) != '' && ctype_digit((string) $veid)) {
      $voip = self::loadVoiceMail($veid);
    }
    elseif (isset($nid) && trim($nid) != '' && ctype_digit((string) $nid)) {
      $voip = self::getVoicemailFromNid($nid);
    }
    elseif (isset($uid) && trim($uid) != '' && ctype_digit((string) $uid)) {
      $voip = self::getVoicemailFromUid($uid);
    }

    if (isset($voip->voicemail)) {
      $voicemail = $voip->voicemail;
      $voipextension = $voip->voipextension;
    }

    if (isset($voicemail->veid) && $voicemail->veid > 0) {

      $this->veid = $voicemail->veid;
      if (isset($voipextension->node)) {
        $this->extensionNid = $voipextension->node->nid;
      }

      if (isset($voipextension->user)) {
        $this->extensionUid = $voipextension->user->uid;
      }

      $this->sendNotifications = $voicemail->send_notifications;
      $this->isEnabled = $voicemail->is_enabled;
      $this->audioGreetingsFid = $voicemail->audio_greetings_fid;
      $this->status = $voicemail->status;
    }
    else {
      throw new Exception(t('Voicemail failed to initialize due to invalid parameters'));
    }
  }

  /**
   * Call the helper function to get Voicemail data and VoIP Load function to
   * get voipextension data
   *
   * @param $veid
   *   VoIP Extension Number
   * @return
   *   An object having voicemail and voipextension data.
   */
  public static function loadVoiceMail($veid) {
    module_load_include('inc', 'voicemail', 'includes/voicemail.helper');

    $voicemail['voicemail'] = voicemail_load($veid);
    if ($voicemail) {
      $voicemail['voipextension'] = VoipExtension::loadExtension($veid);
    }
    return (object) $voicemail;
  }

  /**
   * Find Voicemail data from the Node id, First find the VoIP Extension
   * associated with that node and the call the load function of this class.
   *
   * @param $nid
   *   A node id
   * @return
   *   An object holding voicemail and voipextension data if success else
   *   Boolean FALSE
   */
  public static function getVoicemailFromNid($nid) {
    module_load_include('inc', 'voipextension', 'includes/voipextension.helper');

    $veid = _voipextension_get_veid_from_nid($nid);
    if ($veid) {
      return self::loadVoiceMail($veid);
    }
    else {
      return FALSE;
    }
  }

  /**
   * Find Voicemail Data from the User id specified as a parameter to this function.
   * Find the node/profile corresponding the user and then the VoIP Extension
   *
   * @param $uid
   *   A valid User ID.
   * @return
   *   An object holding voicemail and voipextension data if success else
   *   Boolean FALSE
   */
  public static function getVoicemailFromUid($uid) {
    module_load_include('inc', 'voipextension', 'includes/voipextension.helper');
    static $veid, $cached_uid;
    if (!$cached_uid || $cached_uid != $uid) {
      $veid = '';
      $cached_uid = $uid;
      if (function_exists('content_profile_get_types')) {
        foreach (content_profile_get_types('names') as $type => $type_name) {
          $node = content_profile_load($type, $uid);
          if ($node) {
            $veid = _voipextension_get_veid_from_nid($node->nid);
          }

          if ($veid) {
            break 1;
          }
        }
      }
    }

    if ($veid) {
      return self::loadVoiceMail($veid);
    }
    else {
      return FALSE;
    }
  }

  /**
   * Check that Voicemail is active or not.
   *
   * @return
   *   Boolean TRUE if success else FALSE
   */
  public function isActive() {
    module_load_include('inc', 'voipextension', 'includes/voipextension.helper');

    $veid = $this->veid;
    $nid = _voipextension_get_nid_from_veid($veid);
    if ($nid) {
      $node = node_load(array('nid' => $nid));
      if ($node && isset($node->type) && variable_get('voip_voicemail_use_'. $node->type, FALSE)) {
        $status_field = VOICEMAIL_STATUS_FIELD_NAME;
        $status_field_array = $node->$status_field;
        return $status_field_array[0]['value'];
      }
    }
    return FALSE;
  }

  /**
   * Audio Greeting File id for the selected VoIP Extension.
   *
   * @return
   *   A valid file id if success else boolean FALSE.
   */
  public function getAudioGreeting() {
    module_load_include('inc', 'voipextension', 'includes/voipextension.helper');

    $veid = $this->veid;
    $nid = _voipextension_get_nid_from_veid($veid);
    if ($nid) {
      $node = node_load(array('nid' => $nid));
      if ($node && isset($node->type) && variable_get('voip_voicemail_use_'. $node->type, FALSE)) {
        $audio_file_field = VOICEMAIL_AUDIO_FIELD_NAME;
        $audio_file_field_array = $node->$audio_file_field;
        return $audio_file_field_array[0]['fid'];
      }
    }
    return FALSE;
  }

  /**
   * Save the Voicemail variables programatically by calling save function of
   * this API.
   *
   * $this->audioGreetingsFid --- Its the audio greeting file id associated
   * with VoIP Extension
   *
   * $this->isEnabled --- Voicemail status
   *
   * $this->veid --- VoIP Extension associated with Voicemail (It can't be
   * changed, Unique Key)
   *
   * $this->sendNotifications --- Send Notifications or not (0/1)
   *
   * @return type
   */
  public function save() {
    module_load_include('inc', 'voipextension', 'includes/voipextension.field_operations');
    module_load_include('inc', 'voipextension', 'includes/voipextension.helper');
    module_load_include('inc', 'voicemail', 'includes/voicemail.helper');

    $veid = $this->veid;
    $nid = _voipextension_get_nid_from_veid($veid);
    if ($nid) {
      $node = node_load(array('nid' => $nid));
      if ($node && isset($node->type) && variable_get('voip_voicemail_use_'. $node->type, FALSE)) {
        $audio_file_field = VOICEMAIL_AUDIO_FIELD_NAME;
        $audio_file_field_array = $node->$audio_file_field;

        $status_field = VOICEMAIL_STATUS_FIELD_NAME;
        $status_field_array = $node->$status_field;

        if (isset($audio_file_field_array[0]['fid'])) {
          $audio_file_field_array[0]['fid'] = $this->audioGreetingsFid;
        }
        else {
          $audio_file_field_array = array(
            0 => array(
              'fid' => $this->audioGreetingsFid
            )
          );
        }
        $node->$audio_file_field = $audio_file_field_array;

        if (isset($status_field[0]['value'])) {
          $status_field_array = $this->isEnabled;
        }
        else {
          $status_field_array = array(
            0 => array(
              'value' => $this->isEnabled
            )
          );
        }
        $node->$status_field = $status_field_array;

        $node = node_submit($node);
        node_save($node);

        $voicemail = array();
        $voicemail['veid'] = $this->veid;
        $voicemail['send_notifications'] = $this->sendNotifications;
        $voicemail['is_enabled'] = $this->isEnabled;
        $voicemail['audio_greetings_fid'] = $this->audioGreetingsFid;
        $voicemail['status'] = isset($this->status) ? $this->status : 1;

        voicemail_save($voicemail);
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Find the sent messages from a selected Voice Mail Box
   * @param $criteria
   *   An array having some set of conditions.
   * $criteria['caller_name'] --- Search on the basis of caller name
   * $criteria['caller_number'] --- Caller Number filteration
   *
   * Dates from which to which the messages are sent from specific Voice Mail Box
   * $criteria['sent_from_date'] --- Date in Y-m-d format
   * $criteria['sent_to_date'] --- Date in Y-m-d format
   *
   * @param $start
   *   An integer value to set the stating pointer of the results from the Query
   * @param type $limit
   *   Limit the number of records returned by the search Query.
   *
   * @return
   *   Objects of Sent messages if Success else Boolean FALSE
   */
  public function getSentMessages($criteria, $start = 0, $limit = 10) {
    $veid = $this->veid;
    $args = array();

    $query_select = 'SELECT * FROM {voicemail_sent} WHERE veid = %d';
    $query_select_count = 'SELECT COUNT(mid) AS total FROM {voicemail_sent}';
    $args[] = $veid;
    $where = ' WHERE veid = %d';
    if (isset($criteria['caller_name']) && trim($criteria['caller_name']) != '') {
      $where .= ' AND caller_name LIKE "%s%%"';
      $args[] = $criteria['caller_name'];
    }

    if (isset($criteria['caller_number']) && trim($criteria['caller_number']) != '') {
      $where .= ' AND caller_number = "%s"';
      $args[] = $criteria['caller_number'];
    }

    if (isset($criteria['sent_from_date']) && trim($criteria['sent_to_date']) != '') {
      $sent_from_date = strtotime($criteria['sent_from_date']);
      $sent_to_date = strtotime($criteria['sent_to_date']);

      $where .= ' AND (sent_on >= %d AND sent_on <= %d)';
      $args[] = $sent_from_date;
      $args[] = $sent_to_date;
    }

    $order_by = ' ORDER BY mid DESC';
    $query = $query_select . $where . $order_by;
    $query_count = $query_select_count . $where;

    $total_records_available = db_result(db_query($query_count, $args));

    if ($total_records_available > 0) {
      $results = new stdClass();
      $results->total_records = $total_records_available;
      $results->sent_messages = array();
      $result = db_query_range($query, $args, $start, $limit);
      while ($row = db_fetch_object($result)) {
        $results->sent_messages[$row->mid] = $row;
      }
      return $results;
    }
    return FALSE;
  }


  /**
   * Find the received messages in a selected Voice Mail Box
   * @param $criteria
   *   An array having some set of conditions.
   * $criteria['caller_name'] --- Search on the basis of caller name
   * $criteria['caller_number'] --- Caller Number filteration
   *
   * $criteria['is_new'] --- Find the new messages or read messages, if this
   * is not set then it finds all the messages
   *
   * Dates from which to which the messages are received in a specific Voice Mail Box
   * $criteria['received_from_date'] --- Date in Y-m-d format
   * $criteria['received_to_date'] --- Date in Y-m-d format
   *
   * @param $start
   *   An integer value to set the stating pointer of the results from the Query
   * @param type $limit
   *   Limit the number of records returned by the search Query.
   *
   * @return
   *   Objects of Received messages if Success else Boolean FALSE
   */

  public function getInboxMessages($criteria, $start = 0, $limit = 10) {
    $veid = $this->veid;
    $args = array();

    $query_select = 'SELECT vs.*, vi.is_new, vi.received_on FROM {voicemail_inbox} AS vi INNER JOIN {voicemail_sent} AS vs ON';
    $query_select .= ' vi.mid = vs.mid';

    $query_select_count = 'SELECT COUNT(vi.id) AS total FROM {voicemail_inbox} AS vi INNER JOIN {voicemail_sent} AS vs';
    $query_select_count .= ' ON vi.mid = vs.mid';

    $where = ' WHERE vi.veid = %d';
    $args[] = $veid;
    if (isset($criteria['caller_name']) && trim($criteria['caller_name']) != '') {
      $where .= ' AND vs.caller_name LIKE "%s%%"';
      $args[] = $criteria['caller_name'];
    }

    if (isset($criteria['caller_number']) && trim($criteria['caller_number']) != '') {
      $where .= ' AND vs.caller_number = "%s"';
      $args[] = $criteria['caller_number'];
    }

    if (isset($criteria['is_new']) && trim($criteria['is_new']) != '') {
      $where .= ' AND vi.is_new = %d';
      $args[] = $criteria['is_new'];
    }

    if (isset($criteria['received_from_date']) && trim($criteria['received_to_date']) != '') {
      $received_from_date = strtotime($criteria['received_from_date']);
      $received_to_date = strtotime($criteria['received_to_date']);

      $where .= ' AND (vi.received_on >= %d AND vi.received_on <= %d)';
      $args[] = $received_from_date;
      $args[] = $received_to_date;
    }
    $order_by = ' ORDER BY vs.mid DESC';

    $query = $query_select . $where . $order_by;
    $query_count = $query_select_count . $where;

    $total_records_available = db_result(db_query($query_count, $args));

    if ($total_records_available > 0) {
      $results = new stdClass();
      $results->total_records = $total_records_available;
      $results->received_messages = array();
      $result = db_query_range($query, $args, $start, $limit);
      while ($row = db_fetch_object($result)) {
        $results->received_messages[$row->mid] = $row;
      }
      return $results;
    }
    return FALSE;
  }

  /**
   * Find the messages sent from one Voice Mail Box to another Mail Box
   * @param $from_veid
   *   An integer having From VoIP Extension
   * @param $to_veid
   *   An integer holding To VoIP Extension
   *
   * @param $start
   *   An integer value to set the stating pointer of the results from the Query
   * @param type $limit
   *   Limit the number of records returned by the search Query.
   * @return
   *   A set of objects if success else Boolean FALSE
   */
  public static function getFromToMessages($from_veid, $to_veid, $start = 0, $limit = 10) {
    $args = array();

    $query_select = 'SELECT vs.*, vi.is_new, vi.received_on FROM {voicemail_inbox} AS vi INNER JOIN {voicemail_sent} AS vs ON';
    $query_select .= ' vi.mid = vs.mid';

    $query_select_count = 'SELECT COUNT(vi.id) AS total FROM {voicemail_inbox} AS vi INNER JOIN {voicemail_sent} AS vs';
    $query_select_count .= ' ON vi.mid = vs.mid';

    $where = ' WHERE vs.veid = %d AND vi.veid = %d';

    $args[] = $from_veid;
    $args[] = $to_veid;
    // Order by sent descending.
    $order_by = ' ORDER BY vs.mid DESC';

    $query = $query_select . $where . $order_by;
    $query_count = $query_select_count . $where;

    $total_records_available = db_result(db_query($query_count, $args));

    if ($total_records_available > 0) {
      $results = new stdClass();
      $results->total_records = $total_records_available;
      $results->messages_sent_from_to = array();
      $result = db_query_range($query, $args, $start, $limit);
      while ($row = db_fetch_object($result)) {
        $results->messages_sent_from_to[$row->mid] = $row;
      }
      return $results;
    }
    return FALSE;
  }
}