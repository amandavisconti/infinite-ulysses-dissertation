<?php

/**
 * @file
 * Provides support for the Views module.
 */

/**
 * Implements hook_views_default_views().
 */
function bookmarkorganizerslideshow_views_default_views() {

  $views = array();

  $view = new view;
  $view->name = 'bookmarkorganizer_slideshow';
  $view->description = '';
  $view->tag = 'bookmarkorganizer';
  $view->base_table = 'node';
  $view->human_name = 'bookmarkorganizer_slideshow';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'folder';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'jcarousel';
  $handler->display->display_options['style_options']['wrap'] = '0';
  $handler->display->display_options['style_options']['skin'] = 'bookmarkorganizer';
  $handler->display->display_options['style_options']['visible'] = '1';
  $handler->display->display_options['style_options']['scroll'] = '1';
  $handler->display->display_options['style_options']['auto'] = '0';
  $handler->display->display_options['style_options']['autoPause'] = 1;
  $handler->display->display_options['style_options']['animation'] = 'fast';
  $handler->display->display_options['style_options']['easing'] = '';
  $handler->display->display_options['style_options']['vertical'] = 0;
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['view_mode'] = 'full';
  $handler->display->display_options['row_options']['links'] = 0;
  $handler->display->display_options['row_options']['comments'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 0;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['body']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['body']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['body']['alter']['external'] = 0;
  $handler->display->display_options['fields']['body']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['body']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['body']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['body']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['body']['alter']['html'] = 0;
  $handler->display->display_options['fields']['body']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['body']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['body']['hide_empty'] = 0;
  $handler->display->display_options['fields']['body']['empty_zero'] = 0;
  $handler->display->display_options['fields']['body']['field_api_classes'] = 0;
  /* Sort criterion: Bookmark Organizer: Weight */
  $handler->display->display_options['sorts']['weight']['id'] = 'weight';
  $handler->display->display_options['sorts']['weight']['table'] = 'bookmarkorganizer_item';
  $handler->display->display_options['sorts']['weight']['field'] = 'weight';
  /* Contextual filter: Bookmark Organizer: User ID */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'bookmarkorganizer_item';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['uid']['not'] = 0;
  /* Contextual filter: Bookmark Organizer: Folder */
  $handler->display->display_options['arguments']['folder']['id'] = 'folder';
  $handler->display->display_options['arguments']['folder']['table'] = 'bookmarkorganizer_item';
  $handler->display->display_options['arguments']['folder']['field'] = 'folder';
  $handler->display->display_options['arguments']['folder']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['folder']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['folder']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['folder']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['folder']['not'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'bookmarkorganizer/view';

  /* Display: Attachment */
  $handler = $view->new_display('attachment', 'Attachment', 'attachment_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_collage_thumbnail']['id'] = 'field_collage_thumbnail';
  $handler->display->display_options['fields']['field_collage_thumbnail']['table'] = 'field_data_field_collage_thumbnail';
  $handler->display->display_options['fields']['field_collage_thumbnail']['field'] = 'field_collage_thumbnail';
  $handler->display->display_options['fields']['field_collage_thumbnail']['label'] = '';
  $handler->display->display_options['fields']['field_collage_thumbnail']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_collage_thumbnail']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_collage_thumbnail']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_collage_thumbnail']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_collage_thumbnail']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_collage_thumbnail']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_collage_thumbnail']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_collage_thumbnail']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_collage_thumbnail']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_collage_thumbnail']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_collage_thumbnail']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_collage_thumbnail']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_collage_thumbnail']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_collage_thumbnail']['empty_zero'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 0;
  $handler->display->display_options['displays'] = array(
    'default' => 0,
    'page_1' => 0,
  );
  $handler->display->display_options['attachment_position'] = 'after';

  /* Display: Title */
  $handler = $view->new_display('attachment', 'Title', 'attachment_2');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'bookmarkorganizer-slideshow-title';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = 0;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Bookmark Organizer: Folder */
  $handler->display->display_options['relationships']['folder_relation']['id'] = 'folder_relation';
  $handler->display->display_options['relationships']['folder_relation']['table'] = 'bookmarkorganizer_item';
  $handler->display->display_options['relationships']['folder_relation']['field'] = 'folder_relation';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bookmark Organizer: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'bookmarkorganizer_item';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['relationship'] = 'folder_relation';
  $handler->display->display_options['fields']['label']['label'] = '';
  $handler->display->display_options['fields']['label']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['label']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['label']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['label']['alter']['external'] = 0;
  $handler->display->display_options['fields']['label']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['label']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['label']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['label']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['label']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['label']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['label']['alter']['html'] = 0;
  $handler->display->display_options['fields']['label']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['label']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['label']['hide_empty'] = 0;
  $handler->display->display_options['fields']['label']['empty_zero'] = 0;
  $handler->display->display_options['displays'] = array(
    'page_1' => 'page_1',
    'default' => 0,
  );

  $views[$view->name] = $view;
  return $views;
}