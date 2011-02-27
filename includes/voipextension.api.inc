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
   * Set the extension name value corresponding to the voip extension.
   *
   * @return
   *   TRUE if the extension name field gets saved else FALSE.
   */
  public function setExtensionName() {
    if (isset($this->extensionName) && trim($this->extensionName) != '') {
      $attributes = array();
      $attributes['extension_name'] = $this->extensionName;
      return $this->setAttributes($attributes);
    }
    else {
			throw new Exception(t('Extension name field value is missing.'));
    }
  }

  /**
   * Set the script name value corresponding to the voip extension.
   *
   * @return
   *   TRUE if the script name field gets saved else FALSE.
   */
  public function setscriptName() {
    if (isset($this->scriptName) && trim($this->scriptName) != '') {
      $attributes = array();
      $attributes['script_name'] = $this->scriptName;
      return $this->setAttributes($attributes);
    }
    else {
			throw new Exception(t('Script name field value is missing.'));
    }
  }

  /**
   * Set the extension status value corresponding to the voip extension.
   *
   * @return
   *   TRUE if the extension status field gets saved else FALSE.
   */
  public function setextensionEnabled() {
    if (isset($this->extensionEnabled) && trim($this->extensionEnabled) != '') {
      $attributes = array();
      $attributes['extension_status'] = $this->extensionEnabled;
      return $this->setAttributes($attributes);
    }
    else {
			throw new Exception(t('Extension status field value is missing.'));
    }
  }

  /**
   * Set the extension listed status value corresponding to the voip extension.
   *
   * @return
   *   TRUE if the extension listed status field gets saved else FALSE.
   */
  public function setExtensionListed() {
    if (isset($this->extensionListed) && trim($this->extensionListed) != '') {
      $attributes = array();
      $attributes['extension_listed'] = $this->extensionListed;
      return $this->setAttributes($attributes);
    }
    else {
			throw new Exception(t('Extension listed field value is missing.'));
    }
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

    return $this->setAttributes($attributes);
  }

  /**
   * Save multiple attirbutes of the voip extension.
   * @param $attributes
   *   An array having the attributes in the key value pair.
   *   Key is the column name of voipextensions table.
   *
   * @return
   *   TRUE if the save is successfull else FALSE
   */
  private function setAttributes($attributes) {
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
}