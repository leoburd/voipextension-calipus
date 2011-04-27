<?php
/**
 * @file
 *
 * NodeAPI implementation here.
 * This file is called in the hook_nodeapi function from the module.
 */

switch ($op) {
  // the $node->content array has been rendered, so the node body or teaser is filtered and now contains HTML. This op should only be
  // used when text substitution, filtering, or other raw text operations are necessary.
  case 'alter':
    break;
  // The node is being deleted.
  case 'delete':
    // Delete the Voicemail data also.
    $voipextension_field = VOIPEXTENSION_EXTENSION_MAIN_FIELD;
    $voipextension_field_array = isset($node->$voipextension_field) ? $node->$voipextension_field : array();
    if (isset($voipextension_field_array[0]['veid'])) {
      $veid = $voipextension_field_array[0]['veid'];
      voicemail_delete($veid);
    }

    break;
  // The revision of the node is deleted. You can delete data associated with that revision.
  case 'delete revision':

    break;
  // The node has just been created (inserted in the database).
  case 'insert':
  // The node has just been updated in the database.
  case 'update':
    $voipextension_field = VOIPEXTENSION_EXTENSION_MAIN_FIELD;
    $voipextension_field_array = isset($node->$voipextension_field) ? $node->$voipextension_field : array();
    $voicemail_greeting_field = VOICEMAIL_AUDIO_FIELD_NAME;
    $voicemail_greeting_field_array = isset($node->$voicemail_greeting_field) ? $node->$voicemail_greeting_field : array();
    $voicemail_status_field = VOICEMAIL_STATUS_FIELD_NAME;
    $voicemail_status_field_array = isset($node->$voicemail_status_field) ? $node->$voicemail_status_field : array();

    if (isset($voipextension_field_array[0]['veid'])) {
      $veid = $voipextension_field_array[0]['veid'];
      $audio_greetings_fid = 0;
      if (isset($voicemail_greeting_field_array[0]['fid'])) {
        $audio_greetings_fid = $voicemail_greeting_field_array[0]['fid'];
      }

      $voicemail_is_enabled = 0;
      if (isset($voicemail_status_field_array[0]['value'])) {
        $voicemail_is_enabled = $voicemail_status_field_array[0]['value'];
      }

      $voicemail = array();
      $voicemail['veid'] = $veid;
      $voicemail['is_enabled'] = $voicemail_is_enabled;
      $voicemail['audio_greetings_fid'] = $audio_greetings_fid;
      $voicemail['status'] = 1;
      voicemail_save($voicemail);
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
  // The node has just been updated in the database.
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

    break;
}
