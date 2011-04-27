<?php
/**
 * @file
 *
 * Default views for voicemail module.
 */

/**
 * Implementation of hook_views_default_views().
 */
function voicemail_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'voicemail_sent_messages';
  $view->description = 'Sent Voice mail Messages';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'voicemail_sent';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'mid' => array(
      'label' => 'Message Id',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 1,
      'id' => 'mid',
      'table' => 'voicemail_sent',
      'field' => 'mid',
      'relationship' => 'none',
    ),
    'sent_on' => array(
      'label' => 'Sent On',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'sent_on',
      'table' => 'voicemail_sent',
      'field' => 'sent_on',
      'relationship' => 'none',
    ),
    'caller_name' => array(
      'label' => 'Caller Name',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'caller_name',
      'table' => 'voicemail_sent',
      'field' => 'caller_name',
      'relationship' => 'none',
    ),
    'caller_number' => array(
      'label' => 'Caller Number',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'caller_number',
      'table' => 'voicemail_sent',
      'field' => 'caller_number',
      'relationship' => 'none',
    ),
    'audio_message_fid' => array(
      'label' => 'Audio Message',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'audio_message_fid',
      'table' => 'voicemail_sent',
      'field' => 'audio_message_fid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'veid' => array(
      'default_action' => 'ignore',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'veid',
      'table' => 'voicemail_sent',
      'field' => 'veid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
        '5' => 0,
        '3' => 0,
        '4' => 0,
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_user' => 0,
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'page' => 0,
        'profile' => 0,
        'story' => 0,
        'test1' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '1' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Voicemail Sent Messages');
  $handler->override_option('empty', 'Sorry, No sent messages yet.');
  $handler->override_option('empty_format', '2');
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('use_pager', '1');
  $handler->override_option('distinct', 1);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 1,
    'order' => 'desc',
    'columns' => array(
      'sent_on' => 'sent_on',
      'veid' => 'veid',
      'audio_message_fid' => 'audio_message_fid',
      'caller_name' => 'caller_name',
      'caller_number' => 'caller_number',
    ),
    'info' => array(
      'sent_on' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'veid' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'audio_message_fid' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'caller_name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'caller_number' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'sent_on',
  ));
  $views[$view->name] = $view;


  $view = new view;
  $view->name = 'voicemail_received_messages';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'voicemail_inbox';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'received_on' => array(
      'label' => 'Received On',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'received_on',
      'table' => 'voicemail_inbox',
      'field' => 'received_on',
      'relationship' => 'none',
    ),
    'veid' => array(
      'label' => 'Veid',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 1,
      'id' => 'veid',
      'table' => 'voicemail_inbox',
      'field' => 'veid',
      'relationship' => 'none',
    ),
    'caller_name' => array(
      'label' => 'Caller Name',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'caller_name',
      'table' => 'voicemail_sent',
      'field' => 'caller_name',
      'relationship' => 'none',
    ),
    'caller_number' => array(
      'label' => 'Caller Number',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'caller_number',
      'table' => 'voicemail_sent',
      'field' => 'caller_number',
      'relationship' => 'none',
    ),
    'is_new' => array(
      'label' => 'Is New',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'is_new',
      'table' => 'voicemail_inbox',
      'field' => 'is_new',
      'relationship' => 'none',
    ),
    'audio_message_fid' => array(
      'label' => 'Audio Message',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'audio_message_fid',
      'table' => 'voicemail_sent',
      'field' => 'audio_message_fid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'veid' => array(
      'default_action' => 'ignore',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'veid',
      'table' => 'voicemail_inbox',
      'field' => 'veid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
        '5' => 0,
        '3' => 0,
        '4' => 0,
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_user' => 0,
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'page' => 0,
        'profile' => 0,
        'story' => 0,
        'test1' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '1' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Voicemail Received Messages');
  $handler->override_option('empty', 'Sorry, There is no message for you.');
  $handler->override_option('empty_format', '2');
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('use_pager', '1');
  $handler->override_option('distinct', 1);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 1,
    'order' => 'desc',
    'columns' => array(
      'received_on' => 'received_on',
      'veid' => 'veid',
      'caller_name' => 'caller_name',
      'mid' => 'mid',
      'is_new' => 'is_new',
      'audio_message_fid' => 'audio_message_fid',
      'caller_number' => 'caller_number',
    ),
    'info' => array(
      'received_on' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'veid' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'caller_name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'mid' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'is_new' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'audio_message_fid' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'caller_number' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'mid',
  ));
  $views[$view->name] = $view;

  return $views;
}