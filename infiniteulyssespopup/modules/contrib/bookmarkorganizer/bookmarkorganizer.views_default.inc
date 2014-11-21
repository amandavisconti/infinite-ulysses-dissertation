<?php

/**
 * @file
 * Provides support for the Views module.
 */

/**
 * Implements hook_views_default_views().
 */
function bookmarkorganizer_views_default_views() {

  $views = array();

  $view = new view;
  $view->name = 'bookmarkorganizer_previews';
  $view->description = 'Display preview list of user\'s bookmarks.';
  $view->tag = 'bookmarkorganizer';
  $view->base_table = 'node';
  $view->human_name = 'My bookmarks previews';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'folder';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['links'] = 0;
  $handler->display->display_options['row_options']['comments'] = 0;
  /* Sort criterion: Bookmark Organizer: Weight */
  $handler->display->display_options['sorts']['weight']['id'] = 'weight';
  $handler->display->display_options['sorts']['weight']['table'] = 'bookmarkorganizer_item';
  $handler->display->display_options['sorts']['weight']['field'] = 'weight';
  /* Argument: Bookmark Organizer: User ID */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'bookmarkorganizer_item';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['uid']['style_plugin'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['uid']['not'] = 0;
  /* Argument: Bookmark Organizer: Folder */
  $handler->display->display_options['arguments']['folder']['id'] = 'folder';
  $handler->display->display_options['arguments']['folder']['table'] = 'bookmarkorganizer_item';
  $handler->display->display_options['arguments']['folder']['field'] = 'folder';
  $handler->display->display_options['arguments']['folder']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['folder']['style_plugin'] = 'default_summary';
  $handler->display->display_options['arguments']['folder']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['folder']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['folder']['not'] = 0;
  $translatables['bookmarkorganizer_previews'] = array(
    t('Defaults'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort By'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('All'),
  );

  $views[$view->name] = $view;
  return $views;
}