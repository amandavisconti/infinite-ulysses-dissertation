<?php
/**
 * @file
 * Administration page for the 'Book helper' module.
 */


////////////////////////////////////////////////////////////////////////////////
// Book admin settings
////////////////////////////////////////////////////////////////////////////////

/**
 * Implements hook_form_alter().
 */
function _book_helper_form_book_admin_settings_alter(&$form, &$form_state) {
  // Create new
  $form['book_helper_create_new'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Always create new books'),
    '#default_value' => variable_get('book_helper_create_new', array()),
    '#options' => $form['book_allowed_types']['#options'],
    '#description' => t('Select content types which will automatically create new books when the main book page is initially created'),
  );

  // Navigation options
  $form['book_helper_navigation_options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select book navigation options'),
    '#default_value' => variable_get('book_helper_navigation_options', array('tree', 'prev', 'next', 'up')),
    '#options' => array(
      'tree' => t('Table of contents'),
      'prev' => t('Previous link'),
      'next' => t('Next link'),
      'up' => t('Up link (aka parent page)'),
    ),
  );

  // Links options
  $form['book_helper_links'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select book links options'),
    '#default_value' => variable_get('book_helper_links', array('book_add_child', 'book_printer')),
    '#options' => array(
      'book_add_child' => t('Add child page'),
      'book_printer' => t('Printer-friendly version'),
    ),
  );

  // Remove outline tab
  $form['book_helper_remove_outline'] = array(
    '#type' => 'radios',
    '#title' => t('Remove outline tab from all book pages'),
    '#description' => t("Recommended, since all this page's functionality is now included in the book 'Order' tab."),
    '#default_value' => variable_get('book_helper_remove_outline', 0),
    '#options' => array('0' => t('No'), '1' => t('Yes')),
  );

  // Remove book navigation
  $form['book_helper_remove_book_navigation'] = array(
    '#type' => 'radios',
    '#title' => t('Remove book navigation from all book pages'),
    '#description' => t("This removes the book.module's default navigation from the node content and assumes book navigation will being implemented using either the 'Book (inline) navigation' block or another book/menu-related module."),
    '#default_value' => variable_get('book_helper_remove_book_navigation', 0),
    '#options' => array('0' => t('No'), '1' => t('Yes')),
  );

  // Disable revisions
  $form['book_helper_order_disable_revisions'] = array(
    '#type' => 'radios',
    '#title' => t("Disable new revision when a book's page title is updated"),
    '#description' => t("By default, the book.module creates a new revision when a book pages's title is updated."),
    '#default_value' => variable_get('book_helper_order_disable_revisions', 0),
    '#options' => array('0' => t('No'), '1' => t('Yes')),
  );

  // Menus tree set path
  if (function_exists('menu_tree_set_path')) {
    $form['book_helper_menu_tree_set_path'] = array(
      '#type' => 'radios',
      '#title' => t("Keep a book's main page's menu link active while navigating to lower-level book pages."),
      '#description' => t('If set to \'Yes\', when a book\'s main page is added to a <a href="@">menu</a> it will remain active as users navigate to lower-level pages of the book.', array('@href' => url('admin/structure/menu'))),
      '#default_value' => variable_get('book_helper_menu_tree_set_path', 0),
      '#options' => array('0' => t('No'), '1' => t('Yes')),
    );
  }

  // Move buttons
  $form['buttons']['#weight'] = 100;

  // Rebuild menu
  $form['#submit'][] = 'book_helper_admin_settings_submit';
  return $form;
}

/**
 * Submit handler; For book admin settings.
 */
function book_helper_admin_settings_submit($form, &$form_state) {
  menu_rebuild();
}


////////////////////////////////////////////////////////////////////////////////
// Book order/edit page.
////////////////////////////////////////////////////////////////////////////////

/**
 * Build the form to administrate the hierarchy of a single book.
 *
 * Hijacks the book.module's 'edit order and titles' page. (admin/content/book/%node)
 * and adds 'enable' checkbox (aka hidden) to the table's form.
 *
 * Note:
 * - This module attempts to re-use code from the book.module's book.admin.inc while
 *   merging it with the 'enabled' checked code from the menu.admin.inc menu_overview_form().
 * @see book_admin_edit_submit()
 *
 * @ingroup forms.
 */
function book_helper_admin_edit($form, $form_state, $node)  {
  drupal_set_title($node->title);
  $form['#node'] = $node;
  _book_helper_admin_table($node, $form);

  // Add css
  drupal_add_css( drupal_get_path('module', 'book_helper') . '/book_helper.css');


  // Custom code: Allow top-level pages to be removed from books. http://drupal.org/node/283045
  if (isset($form['table']['#empty'])) {
    unset($form['table']); // Don't render the empty table with headers
    // Display the standard confirmation form.
    $t_args = array('%title' => $node->title);
    $child_type = variable_get('book_child_type', 'book');
    if ((user_access('add content to books') || user_access('administer book outlines')) && node_access('create', $child_type)) {
      $t_args['!add_child_page'] = l(t('add a child page'), 'node/add/' . str_replace('_', '-', $child_type), array('query' => array('parent' => $node->book['mlid'])));
    }
    else {
      $t_args['!add_child_page'] = t('add a child page');
    }
    drupal_set_message(t('%title contains no child book pages. To create a sort-able book heirarchy, please !add_child_page to this book to this book.', $t_args), 'warning');
    $description = t('%title may be added as a new book again using the Outline tab.', $t_args);
    return confirm_form($form, t('Do you want to revert %title from a book hierarchy?', $t_args), 'node/' . $node->nid, $description, t('Revert'));
  }

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save book pages'),
  );

  return $form;
}

/**
 * Check that the book has not been changed while using the form.
 *
 * @see book_admin_edit()
 */
function book_helper_admin_edit_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Save book pages') && $form_state['values']['tree_hash'] != $form_state['values']['tree_current_hash']) {
    form_set_error('', t('This book has been modified by another user, the changes could not be saved.'));
  }
}

/**
 * Handle submission of the book administrative page form.
 *
 * This function takes care to save parent menu items before their children.
 * Saving menu items in the incorrect order can break the menu tree.
 *
 * @see book_admin_edit()
 * @see menu_overview_form_submit()
 */
function book_helper_admin_edit_submit($form, &$form_state) {
  // Custom code: Allow top-level pages to be removed from books. http://drupal.org/node/283045
  if ($form_state['values']['op'] == t('Revert')) {
    $node = $form['#node'];
    menu_link_delete($node->book['mlid']);
    db_delete('book')
      ->condition('bid', $node->nid)
      ->execute();
    db_delete('menu_links')
      ->condition('menu_name', 'book-toc-' . $node->nid)
      ->execute();

    drupal_set_message(t('The post has been reverted from the book.'));
    $form_state['redirect'] = 'node/' . $node->nid;
    return;
  }

  // Save elements in the same order as defined in post rather than the form.
  // This ensures parents are updated before their children, preventing orphans.
  $order = array_flip(array_keys($form_state['input']['table']));
  $form['table'] = array_merge($order, $form['table']);

  foreach (element_children($form['table']) as $key) {
    if ($form['table'][$key]['#item']) {
      $row = $form['table'][$key];
      $values = $form_state['values']['table'][$key];

      /* Over-ridden code: Do not delete.
      // Update menu item if moved.
      if ($row['plid']['#default_value'] != $values['plid'] || $row['weight']['#default_value'] != $values['weight']) {
        $row['#item']['plid'] = $values['plid'];
        $row['#item']['weight'] = $values['weight'];
        menu_link_save($row['#item']);
      }
      */

      // Updated code: Add hidden and collapsed to update menu item if moved.
      if ( $row['plid']['#default_value'] != $values['plid'] ||
          $row['weight']['#default_value'] != $values['weight'] ||
          $row['hidden']['#default_value'] != $values['hidden']
        ) {
        $row['#item']['plid'] = $values['plid'];
        $row['#item']['weight'] = $values['weight'];
        $row['#item']['hidden'] = ($values['hidden']) ? 0 : 1; // Hidden is a special case, the value needs to be reversed.
        menu_link_save($row['#item']);
      }

      // Update the title if changed.
      if ($row['title']['#default_value'] != $values['title'] || $row['node_title']['#default_value'] != $values['node_title']) {
        $node = node_load($values['nid']);
        $langcode = LANGUAGE_NONE;
        /* Over-ridden code: Do not delete.
        $node->title = $values['title'];
        $node->book['link_title'] = $values['title'];
        $node->revision = 1;
        */

        // New code: This is where the custom book link title is set.
        $node->title = $values['node_title'];
        $node->book['link_title'] = $values['node_title'];
        $node->book['book_helper_link_title_custom'] = $values['title'];
        $node->book['book_helper_link_title_sync'] = $values['sync'];

        // Modified code: Allow admin to decide if new revisions should be disabled.
        $node->revision = (variable_get('book_helper_order_disable_revisions', 1) == 1) ? 0 : 1;

        $node->log = t('Title changed from %original to %current.', array('%original' => $node->title, '%current' => $values['title']));

        node_save($node);
        watchdog('content', 'book: updated %title.', array('%title' => $node->title), WATCHDOG_NOTICE, l(t('view'), 'node/' . $node->nid));
      }
    }
  }

  drupal_set_message(t('Updated book %title.', array('%title' => $form['#node']->title)));
}


////////////////////////////////////////////////////////////////////////////////
// Book order table code used by book_helper_admin_edit().
////////////////////////////////////////////////////////////////////////////////

/**
 * Build the table portion of the form for the book administration page.
 *
 * @see book_admin_edit()
 */
function _book_helper_admin_table($node, &$form) {
  $form['table'] = array(
    '#theme' => 'book_helper_admin_table',
    '#tree' => TRUE,
  );

  $tree = book_menu_subtree_data($node->book);
  $tree = array_shift($tree); // Do not include the book item itself.
  if ($tree['below']) {
    $hash = drupal_hash_base64(serialize($tree['below']));
    // Store the hash value as a hidden form element so that we can detect
    // if another user changed the book hierarchy.
    $form['tree_hash'] = array(
      '#type' => 'hidden',
      '#default_value' => $hash,
    );
    $form['tree_current_hash'] = array(
      '#type' => 'value',
      '#value' => $hash,
    );
    _book_helper_admin_table_tree($tree['below'], $form['table'], $node->book['bid']);
  }
  // Custom code: Allow top-level pages to be removed from books. http://drupal.org/node/283045
  else {
    // Track if book is empty.
    $form['table']['#empty'] = TRUE;
  }
}

/**
 * Recursive helper to build the main table in the book administration page form.
 *
 * @see book_admin_edit()
 */
function _book_helper_admin_table_tree($tree, &$form, $bid) {
  // Create a cache of all the book's node titles.
  static $node_titles;
  if (!isset($node_titles)) {
    $node_titles = array();
    $result = db_query('SELECT n.nid, n.title FROM {node} n INNER JOIN {book} b ON n.nid = b.nid WHERE bid = :bid', array(':bid' => $bid));
    foreach ($result as $record) {
      if (function_exists('node_parent_title_remove')) {
        $record['title'] = node_parent_title_remove($record->title);
      }
      $node_titles[ $record->nid ] = $record->title;
    }
  }

  // The delta must be big enough to give each node a distinct value.
  $count = count($tree);
  $delta = ($count < 30) ? 15 : intval($count / 2) + 1;

  foreach ($tree as $data) {
    $node_title = $node_titles[ $data['link']['nid'] ];
    $form['book-admin-' . $data['link']['nid']] = array(
      '#item' => $data['link'],
      'nid' => array('#type' => 'value', '#value' => $data['link']['nid']),
      'depth' => array('#type' => 'value', '#value' => $data['link']['depth']),
      'href' => array('#type' => 'value', '#value' => $data['link']['href']),
      'title' => array(
        '#type' => 'textfield',
        '#default_value' => $data['link']['link_title'],
        '#maxlength' => 255,
        '#size' => 15,
      ),

      // New code: Allows node title to be customized.
      'node_title' => array(
        '#type' => 'textfield',
        '#default_value' => $node_title,
        '#maxlength' => 255,
        '#size' => 15,
      ),
      'sync' => array(
        '#type' => 'checkbox',
        '#default_value' => ($node_title == $data['link']['link_title']) ? TRUE : FALSE,
      ),

      'weight' => array(
        '#type' => 'weight',
        '#default_value' => $data['link']['weight'],
        '#delta' => max($delta, abs($data['link']['weight'])),
        '#title' => t('Weight for @title', array('@title' => $data['link']['title'])),
        '#title_display' => 'invisible',
      ),
      'plid' => array(
        '#type' => 'hidden',
        '#default_value' => $data['link']['plid'],
      ),
      'mlid' => array(
        '#type' => 'hidden',
        '#default_value' => $data['link']['mlid'],
      ),

      // New code: Add checkbox element for hidden.
      'hidden' => array(
        '#type' => 'checkbox',
        '#default_value' => !$data['link']['hidden'], // Hidden is a special case, the value needs to be reversed.
      ),

    );
    if ($data['below']) {
      _book_helper_admin_table_tree($data['below'], $form, $bid);
    }
  }

  return $form;
}


////////////////////////////////////////////////////////////////////////////////
// Theme function.
////////////////////////////////////////////////////////////////////////////////

/**
 * Returns HTML for a book administration form.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @see book_admin_table()
 * @ingroup themeable
 */
function theme_book_helper_admin_table($variables) {
  $form = $variables['form'];
  // New code: Adds js to sync link title and node title.
  drupal_add_js(drupal_get_path('module', 'book_helper') . '/book_helper.js');

  drupal_add_tabledrag('book-outline', 'match', 'parent', 'book-plid', 'book-plid', 'book-mlid', TRUE, MENU_MAX_DEPTH - 2);
  drupal_add_tabledrag('book-outline', 'order', 'sibling', 'book-weight');

  /* Over-ridden code: Do not delete.
  $header = array(t('Title'), t('Weight'), t('Parent'), array('data' => t('Operations'), 'colspan' => '3'));
  */

  // Updated code: Adds hidden to header.
  $header = array(
    t('Menu title'),
    t('Page title'),
    array('data' => t('Sync'), 'class' => 'checkbox'),
    t('Weight'),
    t('Parent'),
    array('data' => t('Enabled'), 'class' => 'checkbox'),
    array('data' => t('Operations'), 'colspan' => '4')
  );

  $rows = array();
  $destination = drupal_get_destination();
  $access = user_access('administer nodes');
  foreach (element_children($form) as $key) {
    $nid = $form[$key]['nid']['#value'];
    $href = $form[$key]['href']['#value'];

    // Add special classes to be used with tabledrag.js.
    $form[$key]['plid']['#attributes']['class'] = array('book-plid');
    $form[$key]['mlid']['#attributes']['class'] = array('book-mlid');
    $form[$key]['weight']['#attributes']['class'] = array('book-weight');

    $data = array(
      theme('indentation', array('size' => $form[$key]['depth']['#value'] - 2)) . drupal_render($form[$key]['title']),

      // New code: Add node title and sync checkbox
      drupal_render($form[$key]['node_title']),
      array('data' => drupal_render($form[$key]['sync']), 'class' => 'checkbox'),

      drupal_render($form[$key]['weight']),
      drupal_render($form[$key]['plid']) . drupal_render($form[$key]['mlid']),

      // New code: Add hidden checkbox to table
      array('data' => drupal_render($form[$key]['hidden']), 'class' => 'checkbox'),

      l(t('view'), $href),
      $access ? l(t('edit'), 'node/' . $nid . '/edit', array('query' => $destination)) : '&nbsp',
      $access ? l(t('remove from book'), 'node/' . $nid . '/outline/remove', array('query' => $destination) ) : '&nbsp',
      $access ? l(t('delete node'), 'node/' . $nid . '/delete', array('query' => $destination) ) : '&nbsp',
    );
    $row = array('data' => $data);
    if (isset($form[$key]['#attributes'])) {
      $row = array_merge($row, $form[$key]['#attributes']);
    }
    $row['class'][] = 'draggable';
    if (empty($form[$key]['hidden']['#value'])) {
      $row['class'][] = 'hidden';
    }
    $rows[] = $row;
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'book-outline')));
}
