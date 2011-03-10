<?php
/**
 * @file
 * 
 * This is an helper file having all the functions required to do operations 
 * on the voipextension field.
 */

/**
 * Adjust a settings array.
 * This will add any missing pieces and will set up requirements.
 */
function voipextension_normalize_settings(&$settings, $required = TRUE) {
  if (!isset($settings['form'])) {
    $settings['form'] = array();
  }

  if (!isset($settings['form']['fields'])) {
    $settings['form']['fields'] = array();
  }

  // Merge defaults in.
  $dummy = array();
  $ds = voipextension_invoke_voipextensionapi($dummy, 'defaults');
  foreach ($ds as $k => $v) {
    if (!isset($settings['form']['fields'][$k])) {
      $settings['form']['fields'][$k] = array();
    }
    $settings['form']['fields'][$k] = array_merge($v, $settings['form']['fields'][$k]);
  }

  // Adjust collection settings if the entire voipextension is "optional."
  if (!$required) {
    // Relax non-required settings.
    foreach ($settings['form']['fields'] as $k => $v) {
      if (isset($v['collect'])) {
        if ($v['collect'] == 2) {
          // Required -> Optional.
          $settings['form']['fields'][$k]['collect'] = 1;
        }
      }
    }
  }
}


/**
 * Returns an empty voip extension object based on the given settings.
 */
function voipextension_empty_voipextension($settings) {
  $voipextension = array();
  $defaults = voipextension_invoke_voipextensionapi($voipextension, 'defaults');
  if (isset($settings['form']['fields'])) {
    foreach ($settings['form']['fields'] as $k => $v) {
      if (isset($defaults[$k])) {
        $defaults[$k] = array_merge($defaults[$k], $v);
      }
    }
  }

  foreach ($defaults as $k => $v) {
    if (isset($v['default'])) {
      $voipextension[$k] = $v['default'];
    }
  }
  return $voipextension;
}

/**
 * Find the voipextension field names by calling voipextension API
 * @return
 *   An array having all the available field names from the voipextension api.
 */
function voipextension_field_names() {
  static $fields;
  if (empty($fields)) {
    $dummy = array();
    $fields = voipextension_invoke_voipextensionapi($dummy, 'fields');
  }
  return $fields;
}

/**
 * Load a single voip extension by veid.
 *
 * @param $veid
 *   Voip Extension ID to load.
 * @return
 *   A voip extension array.
 */
function voipextension_load_voipextension($veid) {
  static $cached_veid, $voipextension;
  
  if (!$cached_veid || $cached_veid != $veid) {
    $voipextension = array();
    $voipextension = db_fetch_array(db_query('SELECT * FROM {voipextensions} WHERE veid = %d', $veid));
    if (empty($voipextension)) {
      $voipextension = array('veid' => $veid);
    }
    $voipextension = array_merge($voipextension, voipextension_invoke_voipextensionapi($voipextension, 'load', $veid));
  }
  return $voipextension;
}

/**
 * Save associated voipextensions.
 *
 * @param $voipextensions
 *   The associated voipextensions.
 *   You can pass an empty array to remove all voipextension references associated
 *   with the given criteria. This is useful if you are about to delete an object,
 *   and need Voip Extension to clean up any voipextensions that are no longer referenced.
 *
 * @param $criteria
 *   An array of instance criteria to save as.
 *   Example: array('genid' => 'some_3333')
 */
function voipextension_save_voipextensions(&$voipextensions, $criteria) {
  $veid_array = array();
  if (isset($voipextensions) && is_array($voipextensions) && !empty($criteria) && is_array($criteria)) {
    foreach (array_keys($voipextensions) as $key) {
      $veid_array[] = voipextension_save($voipextensions[$key], TRUE, $criteria);
    }
    $columns = array();
    $placeholders = array();
    $qfrags = array();
    $args = array();
    foreach (array('nid' => '%d', 'vid' => '%d', 'uid' => '%d', 'genid' => "'%s'") as $key => $placeholder) {
      if (isset($criteria[$key])) {
        $columns[] = $key;
        $placeholders[] = $placeholder;
        $args[] = $criteria[$key];
        $qfrags[] = "$key = $placeholder";
      }
    }
    $querybase = 'FROM {voipextensions_instance} WHERE '. implode(' AND ', $qfrags);

    $old_veids = array();
    $new_veids = array();
    // Find affected lids.
    $query = "SELECT veid $querybase";
    $result = db_query($query, $args);
    while ($t = db_fetch_object($result)) {
      $old_veids[] = $t->veid;
    }

    $query = "DELETE $querybase";
    db_query($query, $args);
    // Tack on the lid.
    $columns[] = 'veid';
    $placeholders[] = '%d';

    foreach ($voipextensions as $voipextension) {
      // Don't save "empty" voipextensions.
      // voipextension_save() explicitly returns FALSE for empty voipextensions,
      // so it should be ok to rely on the data type.
      if ($voipextension['veid'] !== FALSE) {
        $args[] = $voipextension['veid'];
        $new_veids[] = $voipextension['veid'];
        db_query('INSERT INTO {voipextensions_instance} ('. implode(', ', $columns) .') VALUES ('. implode(', ', $placeholders) .')', $args);
        array_pop($args);
      }
    }

    // Check anything that dropped a reference during this operation.
    foreach (array_diff($old_veids, $new_veids) as $check) {
      // An instance may have been deleted. Check reference count.
      $count = db_result(db_query('SELECT COUNT(*) FROM {voipextensions_instance} WHERE veid = %d', $check));
      if ($count !== FALSE && $count == 0) {
        watchdog('voipextension', 'Deleting unreferenced voipextension with VEID %veid.', array('%veid' => $check));
        $voipextension = array('veid' => $check);
        voipextension_invoke_voipextensionapi($voipextension, 'delete');
        db_query('DELETE FROM {voipextensions} WHERE veid = %d', $voipextension['veid']);
      }
    }
  }
  return $veid_array;
}


/**
 * Save a voipextension.
 *
 * This is the central function for saving a voipextension.
 * @param $voipextension
 *   Voip Extension array to save.
 * @param $cow
 *   Copy-on-write, i.e. whether or not to assign a new veid if something changes.
 * @param $criteria
 *   Instance criteria. If the only instances known by voipextension match
 *   the criteria, the veid will be reused, regardless of $cow status. If no criteria
 *   is provided, there will be no attempt to reuse veids.
 * @return
 *   The veid of the saved voipextension, or FALSE if the voipextension is considered "empty."
 */
function voipextension_save(&$voipextension, $cow = TRUE, $criteria = array()) {
  // Quick settings fixup.
  if (!isset($voipextension['voipextension_settings'])) {
    $voipextension['voipextension_settings'] = array();
  }
  voipextension_normalize_settings($voipextension['voipextension_settings']);

  // If there's already a veid, we're editing an old voipextension. Load it in.
  $old_veid = voipextension_empty_voipextension($voipextension['voipextension_settings']);
  if (isset($voipextension['veid']) && !empty($voipextension['veid'])) {
    $old_veid = (array)voipextension_load_voipextension($voipextension['veid']);
  }

  // Pull in fields that hold data currently not editable directly by the user.
  $voipextension = array_merge($old_veid, $voipextension);

  // If we are in COW mode, we *probabaly* need to make a new veid.
  if ($cow) {
    if (isset($voipextension['veid']) && $voipextension['veid']) {
      if (!empty($criteria)) {
        // Check for other instances.
        $columns = array();
        $placeholders = array();
        $qfrags = array();
        $args = array();
        foreach (array('nid' => '%d', 'vid' => '%d', 'uid' => '%d', 'genid' => "'%s'") as $key => $placeholder) {
          if (isset($criteria[$key])) {
            $columns[] = $key;
            $placeholders[] = $placeholder;
            $args[] = $criteria[$key];
            $qfrags[] = "$key = $placeholder";
          }
        }
        $querybase = 'FROM {voipextensions_instance} WHERE '. implode(' AND ', $qfrags);

        $associated = db_result(db_query("SELECT COUNT(*) $querybase", $args));
        $all = db_result(db_query("SELECT COUNT(*) FROM {voipextensions_instance} WHERE veid = %d", $voipextension['veid']));
        if ($associated != $all) {
          // If there were a different number of instances than instances matching the criteria,
          // we need a new VEID.
          unset($voipextension['veid']);
        }
      }
      else {
        // Criteria was not provided, we need a new VEID.
        unset($voipextension['veid']);
      }
    }
  }

  if (!empty($voipextension['veid'])) {
    watchdog('voipextension', 'Conserving veid %veid due to uniqueness.', array('%veid' => $voipextension['veid']));
    drupal_write_record('voipextensions', $voipextension, array('veid'));
  }
  else {
    unset($voipextension['veid']);
    drupal_write_record('voipextensions', $voipextension);
  }

  voipextension_invoke_voipextensionapi($voipextension, 'save');
  return $voipextension['veid'];
}

/**
 * Save the voip directory information after saving the basic voipextension info
 * @param $node
 *   A node object
 * @param $veid_array
 *   An array having the recently created voipextension auto increment ids.
 */
function _voipextension_save_directory_data($node, $veid_array) {
  foreach ($veid_array as $index => $veid) {
    $query = 'UPDATE {voipextensions} SET extension_listed = %d, audio_name_fid = %d, audio_description_fid = %d, audio_greeting_fid = %d WHERE ';
    $query .= 'veid = %d';
    $args = array();

    $extension_listed_key = _voipextension_directory_field_names('VOIPEXTENSION_EXTENSION_LISTED');
    if (isset($node->$extension_listed_key)) {
      $extension_listed = $node->$extension_listed_key;
      $args[] = $extension_listed[0]['value'];
    }
    else {
      $args[] = 0;
    }

    $audio_name_key = _voipextension_directory_field_names('VOIPEXTENSION_AUDIO_NAME');
    if (isset($node->$audio_name_key)) {
      $audio_name = $node->$audio_name_key;
      $args[] = $audio_name[0]['fid'] ? $audio_name[0]['fid'] : 0;
    }
    else {
      $args[] = 0;
    }

    $audio_description_key = _voipextension_directory_field_names('VOIPEXTENSION_AUDIO_DESCRIPTION');
    if (isset($node->$audio_description_key)) {
      $audio_description = $node->$audio_description_key;
      $args[] = $audio_description[0]['fid'] ? $audio_description[0]['fid'] : 0;
    }
    else {
      $args[] = 0;
    }

    $audio_greeting_key = _voipextension_directory_field_names('VOIPEXTENSION_AUDIO_GREETING');
    if (isset($node->$audio_greeting_key)) {
      $audio_greeting= $node->$audio_greeting_key;
      $args[] = $audio_greeting[0]['fid'] ? $audio_greeting[0]['fid'] : 0;
    }
    else {
      $args[] = 0;
    }
    $args[] = $veid;
    $result = db_query($query, $args);
  }
}

/**
 * Create a file object/array for the selected fid.
 * @param $fid
 *   An integer file id.
 * @return
 *   An array having all the information about the file.
 */
function _voipextension_get_file_info($fid) {
  $query = 'SELECT * FROM {files} WHERE fid = %d AND status = %d';
  $result = db_query($query, $fid, 1);
  return db_fetch_array($result);
}

/**
 * Find the voip extension id corresponding to the node id.
 * @param $nid
 *   A node id.
 * @return
 *   An integer value having the voip extension number if found else NULL
 */
function _voipextension_get_veid_from_nid($nid) {
  static $veid, $cached_nid;

  if (!$cached_nid || $cached_nid != $nid) {
    $cached_nid = $nid;
    $query = 'SELECT veid FROM {voipextensions_instance} WHERE nid = %d ORDER BY vid DESC';
    $veid = db_result(db_query_range($query, $nid, 0, 1));
  }

  return $veid;
}

/**
 * Find a node id associated with the voip extension id.
 * @param $veid
 *   Voip Extension id.
 * @return
 *   An integer having the node id if found else NULL.
 */
function _voipextension_get_nid_from_veid($veid) {
  static $nid, $cached_veid;

  if (!$cached_veid || $cached_veid != $veid) {
    $cached_veid = $veid;
    $query = 'SELECT nid FROM {voipextensions_instance} WHERE veid = %d ORDER BY vid DESC ';
    $nid = db_result(db_query_range($query, $veid, 0, 1));
  }
  return $nid;
}

/**
 * Deletes the voipextension associated with the node.
 */
function voipextension_delete($veid) {
  $result = db_query('DELETE FROM {voipextensions_instance} WHERE veid = %d', $veid);
  if ($result) {
    return db_query('DELETE FROM {voipextensions} WHERE veid = %d', $veid);
  }
  return FALSE;
}

/**
 * Deletes the voipextension instance associated with the node revision.
 */
function voipextension_instance_revision_delete($veid, $vid) {
  return db_query('DELETE FROM {voipextensions_instance} WHERE veid = %d AND vid = %d', $veid, $vid);
}