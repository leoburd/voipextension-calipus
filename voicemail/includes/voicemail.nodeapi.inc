<?php
/**
 * @file
 *
 * hook_nodeapi() implementation.
 * This file is only loaded when required by hook_nodeapi().
 */

switch ($op) {
  // the $node->content array has been rendered, so the node body or teaser is filtered and now contains HTML. This op should only be
  // used when text substitution, filtering, or other raw text operations are necessary.
  case 'alter':
    break;
  // The node is being deleted.
  case 'delete':
    break;
  // The revision of the node is deleted. You can delete data associated with that revision.
  case 'delete revision':
    break;
  // The node has just been created (inserted in the database).
  case 'insert':
  // The node has just been updated in the database.
  case 'update':
    // If the current node is having the feature of voicemail box.
    $voicemail_field = _voicemail_get_content_type_voicemail_field($node->type);
    $voicemail_field = $voicemail_field[$node->type];
    if (isset($node->$voicemail_field)) {
      module_load_include('inc', 'voipextension', 'includes/voipextension.field_operations');
      $voipextension_field = _voipextension_get_content_type_voipextension_field($node->type);
      $voipextension_field = $voipextension_field[$node->type];
      if (isset($node->$voipextension_field)) {
        $voicemailbox = array();
        $voipextension_field_array = $node->$voipextension_field;
        $voicemail_field_array = $node->$voicemail_field;
        if (isset($voipextension_field_array[0]['veid']) && $voipextension_field_array[0]['veid'] != '') {
          $voicemailbox['veid'] = $voipextension_field_array[0]['veid'];
        }
        if (isset($voicemail_field_array[0]['voicemail_status'])) {
          $voicemailbox['is_enabled'] = $voicemail_field_array[0]['voicemail_status'];
        }
        if (isset($voicemail_field_array[0]['fid'])) {
          $voicemailbox['audio_greetings_fid'] = $voicemail_field_array[0]['fid'];
        }

        if (isset($voicemailbox['veid']) && $voicemailbox['veid'] != '') {
          voicemail_save($voicemailbox);
        }
      }
    }
    break;
  // The node is about to be loaded from the database. This hook can be used to load additional data at this time.
  case 'load':
    break;
  // The node is about to be shown on the add/edit form.
  case 'prepare':
    break;
  // The node is being cloned for translation. Load additional data or copy values from $node->translation_source.
  case 'prepare translation':
    break;
  // # Prepare a node view for printing. Used for printer-friendly view in book_module # "rss item": An RSS feed is generated. The module
  // can return properties to be added to the RSS item generated for this node. See comment_nodeapi() and upload_nodeapi() for examples.
  // The $node passed can also be modified to add or remove contents to the feed item.
  case 'print':
    break;
  // The node is displayed as a search result. If you want to display extra information with the result, return it.
  case 'rss item':
    break;
  // The node passed validation and is about to be saved. Modules may use this to make changes to the node before it is saved to the
  // database.
  case 'search result':
    break;
  // The node passed validation and is about to be saved. Modules may use this to make changes to the node before it is saved to the database.
  case 'presave':
    break;
  // The node is being indexed. If you want additional information to be indexed which is not already visible through nodeapi "view",
  // then you should return it here.
  case 'update index':
    break;
  // The user has just finished editing the node and is trying to preview or submit it. This hook can be used to check the node data.
  // Errors should be set with form_set_error().
  case 'validate':
    break;
  // The node content is being assembled before rendering. The module may add elements $node->content prior to rendering. This hook will
  // be called after hook_view(). The format of $node->content is the same as used by Forms API.
  case 'view':
    $voicemail_field = _voicemail_get_content_type_voicemail_field($node->type);
    $voicemail_field = $voicemail_field[$node->type];
    if (isset($node->content[$voicemail_field])) {
      module_load_include('inc', 'voipextension', 'includes/voipextension.field_operations');
      $voipextension_field = _voipextension_get_content_type_voipextension_field($node->type);
      $voipextension_field = $voipextension_field[$node->type];
      if (isset($node->$voipextension_field)) {
        $voipextension_field_array = $node->$voipextension_field;
        if (isset($voipextension_field_array[0]['veid'])) {
          $voicemailbox = voicemail_load_settings($voipextension_field_array[0]['veid']);
          if (isset($voicemailbox->veid)) {
            if ($voicemailbox->is_enabled) {
              $voicemail_status = t('Enabled');
            }
            else {
              $voicemail_status = t('Disabled');
            }
            $node->content['voicemail_status'] = array(
                  '#value' => t('<b>Voicemail Status:</b> %voicemail_status', array('%voicemail_status' => $voicemail_status)),
                  '#weight' => $node->content[$voicemail_field]['#weight'] + 1
                );
          }
        }
      }
    }
    break;
}
