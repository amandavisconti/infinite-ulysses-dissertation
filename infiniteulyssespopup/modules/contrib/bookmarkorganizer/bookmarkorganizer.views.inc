<?php

/**
 * @file
 * Provides support for the Views module.
 */


function bookmarkorganizer_views_data() {
  // How to get information about the folder of a bookmark
  $tables['bookmarkorganizer_folder']['table']['group'] = t('Bookmark Organizer');

  $tables['bookmarkorganizer_folder']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Folders'),
    'help' => t("Folders are provided by Bookmark Organizer."),
    'weight' => 10,
  );

  $tables['bookmarkorganizer_folder']['table']['join'] = array(
    'bookmarkorganizer_item' => array(
      'left_field' => 'content_id',
      'field' => 'id',
      'extra' => "{bookmarkorganizer_item}.type = 'folder'",
    ),
  );
  $tables['bookmarkorganizer_folder']['collapsed'] = array(
    'title' => t('Folder collapsed'),
    'help' => t('Whether or not the folder is collapsed.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
  );
  $tables['bookmarkorganizer_folder']['public'] = array(
    'title' => t('Folder published'),
    'help' => t('Whether or not the folder is public or private.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
  );

  $tables['bookmarkorganizer_item']['table']['group'] = t('Bookmark Organizer');

  $tables['bookmarkorganizer_item']['table']['join'] = array(
    'bookmarkorganizer_item' => array(
      'left_field' => 'id',
      'field' => 'parent',
    ),
    'bookmarkorganizer_folder' => array(
      'left_field' => 'id',
      'field' => 'content_id',
      'extra' => "{bookmarkorganizer_item}.type = 'folder'",
    ),
    'node' => array(
      'left_field' => 'nid',
      'field' => 'content_id',
      'extra' => "{bookmarkorganizer_item}.type = 'node'",
    ),
  );

  // Relationship to parent item
  $tables['bookmarkorganizer_item']['folder_relation'] = array(
    'title' => t('Folder'),
    'help' => t('Infomation about the folder related to a bookmark.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'bookmarkorganizer_item',
      'base field' => 'id',
      'relationship field' => 'parent',
      'label' => t('Folder'),
    ),
  );

  // Relationship to parent item
  $tables['bookmarkorganizer_item']['folder_content'] = array(
    'title' => t('Folder content'),
    'help' => t('Infomation about the folder related to a bookmark.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'bookmarkorganizer_folder',
      'base field' => 'id',
      'relationship field' => 'content_id',
      'label' => t('Folder information'),
    ),
  );

  $tables['bookmarkorganizer_item']['folder'] = array(
    'title' => t('Folder'),
    'help' => t('Get contents of a folder.'),
    'real_field' => 'content_id',
    'argument' => array(
      'handler' => 'bookmarkorganizer_handler_argument_fid',
    ),
  );

  $tables['bookmarkorganizer_item']['content_id'] = array(
    'title' => t('Content ID'),
    'help' => t('Usually node ID or folder ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $tables['bookmarkorganizer_item']['id'] = array(
    'title' => t('Bookmark ID'),
    'help' => t('The ID of a bookmark.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $tables['bookmarkorganizer_item']['uid'] = array(
    'title' => t('User ID'),
    'help' => t('The owner of a bookmark.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $tables['bookmarkorganizer_item']['parent'] = array(
    'title' => t('Parent ID'),
    'help' => t('Bookmarks parent item (folder) ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $tables['bookmarkorganizer_item']['label'] = array(
    'title' => t('Label'),
    'help' => t('Label of a bookmark'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );

  $tables['bookmarkorganizer_item']['type'] = array(
    'title' => t('Type'),
    'help' => t('Type of a bookmark'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );

  $tables['bookmarkorganizer_item']['weight'] = array(
    'title' => t('Weight'),
    'help' => t('Weight of a bookmark.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $tables['bookmarkorganizer_item']['created'] = array(
    'title' => t('Created'),
    'help' => t('Creation time of a bookmark.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $tables['bookmarkorganizer_item']['updated'] = array(
    'title' => t('Updated'),
    'help' => t('The last update time of a bookmark.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  return $tables;
}

/**
 * Implements hook_views_plugins().
 */
function bookmarkorganizer_views_plugins() {
  return array(
    'access' => array(
      'parent' => array(
        'no ui' => TRUE,
        'handler' => 'views_plugin_access',
        'parent' => '',
      ),
      'folder' => array(
        'title' => t('Folder Access'),
        'help' => t('Access will be granted to users with permissions based on Bookmark Organizer folder access function.'),
        'handler' => 'bookmarkorganizer_plugin_access_folder',
        'uses options' => FALSE,  //This needs to be TRUE if you want to add any options
        'path' => drupal_get_path('module', 'bookmarkorganizer'),
      ),
    ),
  );
}
