<?php
/**
 * @file
 *
 * Voip extension class for performing operations on the voip extension fields
 */
class VoipExtension {
  private $veid;
  private $extensionNid;
  private $extensionUid;
  private $audioNameFid;
  private $audioDescriptionFid;
  private $audioGreetingFid;

  public $extensionName;
  public $scriptName;
  public $extensionEnabled;
  public $extensionListed;



  /**
   * Constructor of VoipExtension class.
   *
   * Sets the value to private and public variables.
   * @param $veid
   *   Voip Extension number
   * @param $nid
   *   Node id
   * @param $uid
   *   User id.
   */
  public function  __construct($veid = '', $nid = '', $uid = '') {
    if (isset($veid)) {
      $voip_extension = self::loadExtension($veid);
    }
    elseif (isset($nid)) {
      $voip_extension = self::getVoipExtensionFromNid($nid);
    }
    elseif (isset($uid)) {
      $voip_extension = self::getVoipExtensionFromUid($uid);
    }
    
    if (isset($voip_extension) && is_array($voip_extension) && count($voip_extension) > 1) {
      $this->veid = $voip_extension['veid'];
      if (isset($voip_extension['node'])) {
        $this->extensionNid = $voip_extension['node']->nid;
      }

      if (isset($voip_extension['user'])) {
        $this->extensionUid = $voip_extension['user']->uid;
      }

      $this->extensionName       = $voip_extension['extension_name'];
      $this->scriptName          = $voip_extension['script_name'];
      $this->extensionEnabled    = $voip_extension['extension_status'];
      $this->extensionListed     = $voip_extension['extension_listed'];
      $this->audioNameFid        = $voip_extension['audio_name_fid'];
      $this->audioDescriptionFid = $voip_extension['audio_description_fid'];
      $this->audioGreetingFid    = $voip_extension['audio_greeting_fid'];
    }
    else {
			throw new Exception(t('VoipExtension failed to initialize due to invalid parameters'));
    }
  }

  /**
   * Load the voip extension related data.
   * @param $veid
   *   voip extension id.
   * @return
   *   An array having all the information of voip extension and the corresponding user and the node.
   *   Returns boolean false if no data is available.
   */
  public static function loadExtension($veid) {
    require_once dirname(__FILE__) .'/voipextension.helper.inc';
    $voip_extension = voipextension_load_voipextension($veid);
    if (is_array($voip_extension) && count($voip_extension) > 1) {
      $nid = _voipextension_get_nid_from_veid($voip_extension['veid']);
      if ($nid) {
        $node = node_load(array('nid' => $nid));
        if ($node) {
          $voip_extension['node'] = $node;
          if (is_content_profile($node->type)) {
            $voip_extension['user'] = user_load(array('uid' => $node->uid));
          }
        }
      }
      return $voip_extension;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Find voip extension information from the node id.
   * @param $nid
   *   Node id.
   * @return
   *   An array having the voip extension data.
   */
  public static function getVoipExtensionFromNid($nid) {
    require_once dirname(__FILE__) .'/voipextension.helper.inc';
    $veid = _voipextension_get_veid_from_nid($nid);
    if ($veid) {
      return self::loadExtension($veid);
    }
    else {
      return FALSE;
    }
  }

  /**
   * Find the voip extension data for the selected user.
   * If the same user comes in the query then the data is shown by static
   * cached array.
   *
   * @param $uid
   *   Seleted User id.
   * @return
   *   An integer Voip Extension number/id.
   */
  public static function getVoipExtensionFromUid($uid) {
    static $veid, $cached_uid;
    if (!$cached_uid || $cached_uid != $uid) {
      $veid       = '';
      $cached_uid = $uid;
      if (function_exists('content_profile_get_types')) {
        foreach (content_profile_get_types('names') as $type => $type_name) {
          $node = content_profile_load($type, $uid);
          if ($node) {
            require_once dirname(__FILE__) .'/voipextension.helper.inc';
            $veid = _voipextension_get_veid_from_nid($node->nid);
          }

          if ($veid) {
            break 1;
          }
        }
      }
    }
    
    // If veid is stored/cached then call loadExtension.
    if ($veid) {
      return self::loadExtension($veid);
    }
    else {
      return FALSE;
    }
  }

  /**
   * Check that the current assigned voip extension number is of user or not.
   * 
   * @return
   *   TRUE if it is a user extension else FALSE.
   */
  public function isUserExtension() {
    $veid = $this->veid;
    require_once dirname(__FILE__) .'/voipextension.helper.inc';
    $nid = _voipextension_get_nid_from_veid($veid);
    if ($nid) {
      $node = node_load(array('nid' => $nid));
      return is_content_profile($node->type);
    }
    else {
      return FALSE;
    }
  }

  /**
   * Check that the current assigned voip extension number is of node or not.
   * 
   * @return
   *   TRUE if it is a node extension else FALSE.
   */
  public function isNodeExtension() {
    return !$this->isUserExtension();
  }

  /**
   * Find the extension name associated with the current assigned voip extension number.
   * 
   * @return
   *   A voip extension name if found else FALSE.
   */
  public function getExtensionName() {
    return $this->extensionName;
  }

  /**
   * Find the script name associated with the current assigned voip extension number.
   * 
   * @return
   *   A script name if found else FALSE.
   */
  public function getScriptName() {
    return $this->scriptName;
  }

  /**
   * Find the extension status associated with the current assigned voip extension number.
   * 
   * @return
   *   Boolean value having the status of the voip extension. TRUE/FALSE
   */
  public function getExtensionStatus() {
    return $this->extensionEnabled;
  }

  /**
   * Find that the extension is listed in phone directory or not for the
   * current assigned voip extension number.
   * 
   * @return
   *   Boolean value having the status of the voip extension. TRUE/FALSE
   */
  public function getExtensionListedStatus() {
    return $this->extensionListed;
  }

  /**
   * Find the audio files info attached with the voip extension.
   * Use the helper function to find the file related information.
   *
   * @param $fid
   *   File id
   * @return
   *   An array having the file related information.
   */
  private static function getVoipExtensionFileInfo($fid) {
    require_once dirname(__FILE__) .'/voipextension.helper.inc';
    return _voipextension_get_file_info($fid);
  }

  /**
   * Get Audio name field details for the currently selected voip extension number.
   * Call the helper function of voipextension module to get all the info of the file.
   *
   * @return
   *   An array having all the information of the audio name file or FALSE if no data.
   */
  public function getExtensionAudioName() {
    $audio_name_fid = $this->audioNameFid;
    if ($audio_name_fid) {
      return self::getVoipExtensionFileInfo($audio_name_fid);
    }
    return FALSE;
  }

  /**
   * Get Audio description field details for the currently selected voip extension number.
   * Call the helper function of voipextension module to get all the info of the file.
   *
   * @return
   *   An array having all the information of the audio description file or FALSE if no data.
   */
  public function getExtensionAudioDescription() {
    $audio_description_fid = $this->audioDescriptionFid;
    if ($audio_description_fid) {
      return self::getVoipExtensionFileInfo($audio_description_fid);
    }
    return FALSE;
  }

  /**
   * Get Audio greeting field details for the currently selected voip extension number.
   * Call the helper function of voipextension module to get all the info of the file.
   *
   * @return
   *   An array having all the information of the audio greeting file or FALSE if no data.
   */
  public function getExtensionAudioGreeting() {
    $audio_greeting_fid = $this->audioGreetingFid;
    if ($audio_greeting_fid) {
      return self::getVoipExtensionFileInfo($audio_greeting_fid);
    }
    return FALSE;
  }

  /**
   * Find the node id attached with the voip extension.
   * @return
   *   An integer value having the node id.
   */
  public function getExtensionNid() {
    return $this->extensionNid;
  }

  /**
   * If the current loaded extension instruction is a user extension then it
   * returns the user id associated with that extension
   *
   * @return
   *   An integer value having the user id.
   */
  public function getExtensionUid() {
    return $this->extensionUid;
  }

  /**
   * Save the voip extension attributes, It saves only public attributes.
   *
   * @return
   *   TRUE if the save is successfull else FALSE
   */
  public function save() {
    $attributes = array();
    if (isset($this->extensionName) && trim($this->extensionName) != '') {
      $attributes['extension_name'] = $this->extensionName;
    }

    if (isset($this->scriptName) && trim($this->scriptName) != '') {
      $attributes['script_name'] = $this->scriptName;
    }

    if (isset($this->extensionEnabled) && trim($this->extensionEnabled) != '') {
      $attributes['extension_status'] = $this->extensionEnabled;
    }
    
    if (isset($this->extensionListed) && trim($this->extensionListed) != '') {
      $attributes['extension_listed'] = $this->extensionListed;
    }

    require_once dirname(__FILE__) .'/voipextension.field_operations.inc';
    require_once dirname(__FILE__) .'/voipextension.helper.inc';
    require_once dirname(__FILE__) .'/voipextension.constants.inc';

    $veid = $this->veid;
    $nid = _voipextension_get_nid_from_veid($veid);
    if ($nid) {
      $node = node_load(array('nid' => $nid));
      if ($node && isset($node->type)) {
        // Check that the selected node is having the voip extension associated or not.
        $node_type_field = _voipextension_get_content_type_voipextension_field($node->type);

        if ($node_type_field && isset($node_type_field[$node->type])) {
          $field_name = $node_type_field[$node->type];
          $temp_field_data = $node->$field_name;
          foreach ($attributes as $attr_name => $attr_value) {
            if (isset($temp_field_data[0][$attr_name])) {
              $temp_field_data[0][$attr_name] = $attr_value;
              $node->$field_name = $temp_field_data;
            }
          }
          // This is a different cck field, so needs to be handled separately.
          if (isset($attributes['extension_listed'])) {
            $extension_listed_field = VOIPEXTENSION_EXTENSION_LISTED;
            if (isset($node->$extension_listed_field)) {
              $extension_listed_field_array = $node->$extension_listed_field;
              $extension_listed_field_array[0]['value'] = $attributes['extension_listed'];
              $node->$extension_listed_field = $extension_listed_field_array;
            }
          }
          // Save the node along with the updated attributes.
          $node = node_submit($node);
          node_save($node);
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /*
   * Returns a list of extensions that match specified criteria
   *
   * @param $criteria
   *   array with the query criteria for the audio entries:
   *     ['extension_name'] :: first letters of the name of the extension to
   *     be retrieved. NULL = any
   *     ['extension_status'] :: 1 = return active extensions, 0 = non-active,
   *     NULL = any
   *     ['extension_listed'] :: 1 = return extensions listed in the diretory,
   *     0 = not listed, NULL = any
   *     ['get_user_extensions'] :: TRUE = user extensions are to be returned
   *     ['get_node_extensions'] :: TRUE = node extensions are to be returned
   *     ['type_contents'] :: specifies the type of node extensions to be
   *     returned, or NULL for any
   *
   * @param $start
   *   the directory row to start from
   *
   * @param $limit
   *   the maximum number of directory rows to fetch
   *
   * @return
   *   An array having the list of voipextensions if success else Boolean FALSE.
   */
  public static function search($criteria, $start = 0, $limit = 10) {
    $select = 'SELECT DISTINCT(v.veid) AS veid FROM ';
    $select_count = 'SELECT COUNT(DISTINCT(v.veid)) AS total FROM ';

    $joins = ' {voipextensions} AS v INNER JOIN {voipextensions_instance} AS vei ON v.veid = vei.veid ';
    $joins .= ' INNER JOIN {node_revisions} AS nr ON vei.vid = nr.vid INNER JOIN {node} AS n ON nr.nid = n.nid ';

    $where = ' WHERE 1=1 ';
    $args = array();
    if (isset($criteria['extension_name'])) {
      $where .= ' AND v.extension_name LIKE "%s%%" ';
      $args[] = $criteria['extension_name'];
    }
    
    if (isset($criteria['script_name'])) {
      $where .= ' AND v.script_name LIKE "%s%%" ';
      $args[] = $criteria['script_name'];
    }

    if (isset($criteria['extension_status'])) {
      $where .= ' AND v.extension_status = %d ';
      $args[] = $criteria['extension_status'] ? 1 : 0;
    }

    if (isset($criteria['extension_listed'])) {
      $where .= ' AND v.extension_listed = %d ';
      $args[] = $criteria['extension_listed'] ? 1 : 0;
    }

    $user_content_types = content_profile_get_types('names');
    $user_content_types = array_keys($user_content_types);
    
    if (isset($criteria['get_user_extensions']) && is_array($user_content_types) && count($user_content_types) > 0) {
      $where .= ' AND n.type IN ('. db_placeholders($user_content_types, 'varchar') .') ';
      foreach ($user_content_types as $index => $content_type) {
        $args[] = $content_type;
      }
    }
    elseif (isset($criteria['get_node_extensions'])) {
      if (is_array($user_content_types) && count($user_content_types) > 0) {
        $where .= ' AND n.type NOT IN ('. db_placeholders($user_content_types, 'varchar') .') ';
        foreach ($user_content_types as $index => $content_type) {
          $args[] = $content_type;
        }
      }
    }

    if (isset($criteria['type_contents']) && is_array($criteria['type_contents']) && count($criteria['type_contents']) > 0) {
      $where .= ' AND n.type IN ('. db_placeholders($criteria['type_contents'], 'varchar') .') ';
      foreach ($criteria['type_contents'] as $index => $content_type) {
        $args[] = $content_type;
      }
    }

    $where .= ' AND n.status = %d ';
    $args[] = 1;
    $order_by = ' ORDER BY v.veid DESC ';
    $query = $select . $joins . $where . $order_by;
    $query_count = $select_count . $joins . $where;

    $total_records_available = db_result(db_query($query_count, $args));
    
    if ($total_records_available > 0) {
      $results = array();
      $results['total_records'] = $total_records_available;
      $results['data'] = array();
      $result = db_query_range($query, $args, $start, $limit);
      while ($row = db_fetch_object($result)) {
        $results['data'][$row->veid] = self::loadExtension($row->veid);
      }
      return $results;
    }
    return FALSE;
  }
}