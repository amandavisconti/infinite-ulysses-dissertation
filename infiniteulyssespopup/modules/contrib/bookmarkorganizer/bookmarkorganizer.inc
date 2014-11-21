<?php

/**
 * @file
 * Contains administrative pages for Bookmark Organizers.
 */

/**
 * Bookmark Organizer user pages.
 */
function bookmarkorganizer_main_view($user, $item_id = NULL) {

  $elements = array();

  // Bookmarks' index tree
  $elements['index'] = array(
    '#list' => drupal_get_form('bookmarkorganizer_user_bookmarks_form', $user, 0, $item_id),
    '#theme' => array('bookmarkorganizer_index'),
  );
  // Bookmarks' index actions
  if (variable_get('bookmarkorganizer_enable_folders')) {
    $elements['index']['#actions'] = array(
      'addfolder' => array(
        '#type' => 'link',
        '#title' => t('Add folder'),
        '#href' => 'bookmarkorganizer/add/folder/' . $user->uid,
        '#attributes' => array('class' => 'action action-addfolder'),
        '#theme' => 'bookmarkorganizer_addfolder',
      ),
      '#theme' => 'bookmarkorganizer_actions',
      '#uid' => $user->uid,
      '#active_item_id' => $item_id,
    );
  }

  // Preview pane
  if (variable_get('bookmarkorganizer_enable_preview_pane')) {
    $item = ($item_id) ? bookmarkorganizer_item_load($item_id) : NULL;
    $elements['preview'] = _bookmarkorganizer_activelist_view($user->uid, $item);
  }

  // Dialog
  $elements['dialog'] = array(
    '#type' => 'markup',
    '#markup' => t('<div id="dialog"></div>'),
  );

  // Css classes for container
  $classes = array();
  $classes[] = 'bookmarkorganizer-container';
  $classes[] = (variable_get('bookmarkorganizer_enable_preview_pane')) ? 'preview-enabled' : 'preview-disabled';
  if (variable_get('bookmarkorganizer_remove_sidebars')) {
    $classes[] = 'sidebars-removed';
  }
  $elements['#classes'] = implode(' ', $classes);

  $elements['#theme'] = 'bookmarkorganizer_container';

  if (!empty($_REQUEST['js'])) {
    // Render elements
    $output = drupal_render($elements);
    // Send to javascript
    $json = array('html' => $output);
    drupal_json_output($json);
    exit();
  }
  else {
    return $elements;
  }
}

function _bookmarkorganizer_activelist_view($uid, $item = NULL) {
  $elements = array();
  // Set index to zero (root level of the tree) if item is null
  $index = ($item) ? $item->id : 0;
  // Store the original item into elements
  if ($item) {
    $elements['#item'] = $item;
  }
  // Use item label (folder name) as a title of the list if item exists.
  $elements['header']['title'] = array(
    '#type' => 'markup',
    '#markup' => ($item) ? $item->label : t('My Bookmarks'),
    '#prefix' => '<h2>',
    '#suffix' => '</h2>',
  );
  // Set actions list.
  $elements['header']['actions'] = array(
    '#items' => array(),
    '#theme' => array('bookmarkorganizer_action_list'),
  );
  // Set publish / unpublish link for folders.
  if (variable_get('bookmarkorganizer_enable_folder_publication', FALSE)) {
    if ($item && $item->type == 'folder') {
      $method = $item->public ? 'unpublish' : 'publish';
      $current_method = $item->public ? 'publish' : 'unpublish';
      $title = $item->public ? t('Unpublish') : t('Publish');
      $alt = $item->public ? t('Unpublish this folder') : t('Publish this folder');
      $elements['header']['actions']['#items']['publication'] = array(
        '#type' => 'link',
        '#title' => $title,
        '#href' => "bookmarkorganizer/folder/$method/$item->id",
        '#attributes' => array('class' => "action action-folder action-folder-$current_method", 'alt' => $alt, 'title' => $alt),
      );
    }
  }
  // If we have node item
  if ($item && $item->type == 'node') {
    $node = node_load($item->content_id);
    $elements['#node'] = $node;
    $elements['content'] = node_view($node, variable_get('bookmarkorganizer_preview_mode'));
  }
  // If we dont have node item we have a folder and if views is enabled
  elseif (module_exists('views')) {
    // Get view name we are going to use
    $preview_view = variable_get('bookmarkorganizer_default_preview_list', 'bookmarkorganizer_previews');
    // Get view to display contents of a folder
    $view = views_get_view($preview_view);
    $view->set_display('default');
    $view->args = array($uid, $index);
    $view->pre_execute();
    $view->execute();
    // If folder is empty use theme for empty preview otherwise display the view
    if (empty($view->result)) {
      $elements['content']['#empty'] = TRUE;
      if ($index == 0) {
        $preview_output = t("You have no bookmarks. Go and browse contents and bookmark your favorites. They will appear here.");
      }
      else {
        $preview_output = t("You have no bookmarks in this folder. Go and browse contents and bookmark your favorites. They will appear here.");
      }
    }
    else {
      $preview_output = $view->preview();
    }
    $elements['content']['#type'] = 'markup';
    $elements['content']['#markup'] = $preview_output;
  }

  // Set theme for preview pane
  $elements['#theme'] = array('bookmarkorganizer_preview');

  // Invoke alter functions
  drupal_alter('bookmarkorganizer_preview', $elements);

  // Return preview as a render array
  return $elements;
}

function bookmarkorganizer_activelist_view_root($user) {
  $preview = _bookmarkorganizer_activelist_view($user->uid);
  if (!empty($_POST['js'])) {
    // Render form
    $output = drupal_render($preview);

    $target_element = "bookmarkorganizer-preview";

    // Send to javascript
    $json = array('html' => $output, 'target_element' => $target_element);
    drupal_json_output($json);
    exit();
  }
  else {
    // Send data for page load
    return $preview;
  }
}

function bookmarkorganizer_activelist_view($item) {

  $preview = _bookmarkorganizer_activelist_view($item->uid, $item);

  if (!empty($_POST['js'])) {
    // Render form
    $output = drupal_render($preview);

    $target_element = "bookmarkorganizer-preview";

    // Send to javascript
    $json = array('html' => $output, 'target_element' => $target_element);
    drupal_json_output($json);
    exit();
  }
  else {
    // Send data for page load
    return $preview;
  }
}

function bookmarkorganizer_user_bookmarks_form($form, $form_state, $user, $index = 0, $active_item = NULL) {

  $data = bookmarkorganizer_get_items($user->uid, $index, !variable_get('bookmarkorganizer_enable_folders'));

  $form = array();

  $form['action_status'] = array(
    '#type' => 'markup',
    '#markup' => t('<div id="action-status"></div>'),
    '#weight' => -50,
  );

  $form['bookmarks'] = array();
  $form['bookmarks']['#tree'] = TRUE;

  $folders = bookmarkorganizer_get_possible_parents($user->uid);

  foreach ($data as $row) {

    $label = _bookmarkorganizer_get_label($row);

    $options = array(0 => t('-- root --'));
    $options += $folders;

    if ($row->type == 'folder' && isset($options[$row->id])) {
      unset($options[$row->id]);
    }

    // Set actions
    $actions = array();

    $alt = t('Remove article');
    $actions[] = array(
      '#type' => 'link',
      '#title' => t('Remove'),
      '#href' => 'bookmarkorganizer/delete/' . $row->id,
      '#attributes' => array('class' => 'action action-remove', 'alt' => $alt, 'title' => $alt),
      '#theme' => array('link__bookmarkorganizer_action_delete', 'link__bookmarkorganizer_action'),
    );

    $alt = t('Rename article');
    $actions[] = array(
      '#type' => 'link',
      '#title' => t('Rename'),
      '#href' => 'bookmarkorganizer/rename/' . $row->id,
      '#attributes' => array('class' => 'action action-rename', 'alt' => $alt, 'title' => $alt),
      '#theme' => array('link__bookmarkorganizer_action_rename', 'link__bookmarkorganizer_action'),
    );

    $form['bookmarks'][$row->id] = array(
      'type' => array(
        '#type' => 'hidden',
        '#value' => check_plain($row->type),
      ),
      'label' => array(
        '#type' => 'markup',
        '#markup' => $label,
      ),
      'weight' => array(
        '#type' => 'weight',
        '#delta' => count($data),
        '#default_value' => $row->weight,
      ),
      'parent' => array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => $row->parent,
      ),
      'actions' => array(
        '#items' => $actions,
        '#theme' => array('bookmarkorganizer_action_list'),
      ),
      'id' => array(
        '#type' => 'hidden',
        '#value' => check_plain($row->id),
      ),
      'content_id' => array(
        '#type' => 'hidden',
        '#value' => check_plain($row->content_id),
      ),
      '#depth' => $row->depth,
    );

    // In case the type is folder add info whether or not the folder is collapsed
    if ($row->type == 'folder') {
      $form['bookmarks'][$row->id]['collapsed'] = array(
        '#type' => 'hidden',
        '#value' => check_plain($row->collapsed),
      );
    }

    if ($active_item && $active_item == $row->id) {
      $form['bookmarks'][$row->id]['#attributes'] = array(
        'class' => array('active-item'),
      );
    }

  }

  $form['uid'] = array(
    '#type' => 'hidden',
    '#value' => check_plain($user->uid),
  );

  if (!isset($_REQUEST['js'])) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#attributes' => array('id' => 'edit-submit-bookmarkorganizer-index'),
    );
  }

  $form['#action'] = '#';

  return $form;
}

function bookmarkorganizer_item_action($action, $item) {

  if ($action == 'delete') {
    $form_id = 'bookmarkorganizer_delete_item_form';
    $target_element = 'action-status';
  }
  elseif ($action == 'rename') {
    $form_id = 'bookmarkorganizer_rename_item_form';
    $target_element = 'label-' . $item->id;
  }

  if (!isset($form_id)) {
    return;
  }

  // Get form
  $form = drupal_get_form($form_id, $item);

  if (!empty($_POST['js'])) {
    // Render form
    $output = drupal_render($form);

    $ajax_action_submit = url("bookmarkorganizer/$action/ajax/" . $item->id);

    // Send to javascript
    $json = array('html' => $output, 'data' => $form, 'target_element' => $target_element, 'path' => $ajax_action_submit, 'item' => $item);
    drupal_json_output($json);
    exit();
  }
  else {
    // Send data for page load
    return $form;
  }
}

function bookmarkorganizer_delete_item_form($form, $form_state, $item) {

  $form = array();

  $form['bookmarkorganizer_item'] = array('#type' => 'value', '#value' => $item);

  // Check if the form is requested by javascript
  if (!empty($_POST['js'])) {
    // Button form elements are created for javascript use only
    $form['button'] = array(
      '#type' => 'button',
      '#value' => t('OK'),
      '#attributes' => array('id' => 'edit-submit-delete-' . $item->id),
    );
    $form['cancel'] = array(
      '#type' => 'markup',
      '#markup' => l(t('Cancel'), 'user/' . $item->uid . '/bookmarks', array('attributes' => array('id' => 'cancel-delete-' . $item->id))),
      '#attributes' => array('id' => 'cancel-delete-' . $item->id),
    );

    // Unset submit button created by confirm_form when using javascript
    unset($form['submit']);
  }
  else {
    // When javascript is disabled use traditional submit element and callback
    $form = confirm_form(
      $form,
      t('Bookmark Organizer'),
      'user/' . $item->uid . '/bookmarks',
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
    $form['#theme'] = 'bookmarkorganizer_delete_item_form';
  }

  return $form;
}

/**
 * Custom delete submit for javascript use only.
 */
function bookmarkorganizer_delete_item_ajax_submit($item) {
  // Get form values from http request
  $values = $_REQUEST;

  // Validate form token manually because normal Form API validation has not been processed
  if (!drupal_valid_token($values['form_token'], 'bookmarkorganizer_delete_item_form')) {
    // If validation fails output an existing label and error message
    $json = array('error' => t('Form validation error'));
    drupal_json_output($json);
    exit();
  }

  // Execute database delete query
  $msg = _bookmarkorganizer_delete_item($item);

  $user = user_load($item->uid);
  $bookmarks_form = drupal_get_form('bookmarkorganizer_user_bookmarks_form', $user);

  // Send json to browser
  $json = array('message' => $msg, 'bookmarks' => drupal_render($bookmarks_form));
  drupal_json_output($json);
  exit();
}

function bookmarkorganizer_delete_item_form_submit($form, &$form_state) {

  if ($form_state['values']['confirm']) {

    // Set item
    $item = $form_state['values']['bookmarkorganizer_item'];

    // Delete item
    $msg = _bookmarkorganizer_delete_item($item);

    // Redirect user to bookmarks page
    $url = 'user/' . $item->uid . '/bookmarks';

  }
  else {
    $msg = t('Can not execute command.');
    $url = 'user';
  }

  drupal_set_message(t('%bookmarkorganizer_delete_item_message', array('%bookmarkorganizer_delete_item_message' => $msg)));
  $form_state['redirect'] = $url;

}

function _bookmarkorganizer_delete_item($item) {

  // Delete item and children
  $num_deleted = bookmarkorganizer_delete_items_recursively($item->id);

  // Set message
  if ($item->type == 'folder') {
    // For folder type
    if ($num_deleted) {
      $msg = t('Folder %label has been deleted.', array('%label' => $item->label));
    }
    else {
      $msg = t('Folder %label can not be found.', array('%label' => $item->label));
    }

  }
  else {
    // If content type is some sort of flag
    if ($num_deleted) {
      $msg = t('Bookmark %label has been deleted.', array('%label' => $item->label));
    }
    else {
      $msg = t('Bookmark %label can not be found.', array('%label' => $item->label));
    }

  }

  return $msg;

}

function bookmarkorganizer_delete_items_recursively($id) {

  // Delete single item (and folder from bookmarkorganizer table if needed)
  $num_deleted = bookmarkorganizer_delete_item($id);

  // Get item children
  $query = db_query("SELECT id, type FROM {bookmarkorganizer_item} WHERE parent = :pid", array(':pid' => $id));

  while ($row = $query->fetch()) {
    // Recursively call children
    bookmarkorganizer_delete_items_recursively($row->id, $row->type);
  }

  return $num_deleted;
}

function bookmarkorganizer_delete_item($id) {

  $item = db_query("SELECT id, type, content_id FROM {bookmarkorganizer_item} WHERE id = :id", array(':id' => $id))->fetch();

  // Delete item
  $num_deleted = db_delete('bookmarkorganizer_item')->condition('id', $id)->execute();

  if ($item->type == 'folder') {
    // Delete folder from folder table
    db_delete('bookmarkorganizer_folder')->condition('id', $item->content_id)->execute();
  }
  else {
    // Delete flag as well
    $flag = flag_get_flag(variable_get('bookmarkorganizer_flag_name', 'bookmarks'));
    $flag->flag('unflag', $item->content_id); //, user_load($item->uid));
  }

  return $num_deleted;
}

function bookmarkorganizer_get_items($uid, $index = 0, $exclude_subfolders = FALSE) {
  $query = db_select('bookmarkorganizer_item', 'boi');
  $query->leftJoin('bookmarkorganizer_folder', 'bof', 'boi.content_id = bof.id');
  $query->fields('boi');
  $query->addField('bof', 'collapsed');
  $query->addField('bof', 'public');
  $query->condition('boi.uid', $uid);
  if ($exclude_subfolders) {
    $query->condition('boi.type', 'folder', '!=');
  }
  $query->orderBy('boi.weight', 'ASC');
  $result = $query->execute();

  $items = array();
  while ($row = $result->fetch()) {
    $items[$row->parent][] = $row;
  }

  $items = bookmarkorganizer_recursive_tree_flatten($items, $index);

  return $items;
}

function bookmarkorganizer_get_item($id) {
  $id = intval($id);
  $query = db_query("SELECT boi.*,
                            bof.collapsed,
                            bof.public
                      FROM {bookmarkorganizer_item} boi
                      LEFT JOIN {bookmarkorganizer_folder} bof
                      ON boi.content_id = bof.id
                      WHERE boi.id = :id
                      ORDER BY boi.weight ASC",
                      array(':id' => $id));
  return $query->fetch();
}

function bookmarkorganizer_get_item_by_content($content_id, $uid, $type) {
  $query = db_query("SELECT *
                      FROM {bookmarkorganizer_item}
                      WHERE content_id = :content_id AND uid = :uid AND type = :type", array(':content_id' => $content_id, ':uid' => $uid, ':type' => $type));
  return $query->fetch();
}

// Arrange flatten folder tree recursively
function bookmarkorganizer_recursive_tree_flatten($tree, $index = 0, &$items = array(), $depth = 0) {
  if (isset($tree[$index])) {
    foreach ($tree[$index] as $item) {
      $item->depth = $depth;
      $items[] = $item;
      if ($item->collapsed != 1) {
        bookmarkorganizer_recursive_tree_flatten($tree, $item->id, $items, $depth + 1);
      }
    }
  }
  return $items;
}

function bookmarkorganizer_get_possible_parents($uid, $mode = NULL) {
  if ($mode == 'all') {
    $query = db_query("SELECT id, label FROM {bookmarkorganizer_item} WHERE uid = :uid ORDER BY label ASC", array(':uid' => $uid));
  }
  else {
    $query = db_query("SELECT id, label FROM {bookmarkorganizer_item} WHERE type = 'folder' AND uid = :uid ORDER BY label ASC", array(':uid' => $uid));
  }

  $items = array();

  while ($row = $query->fetch()) {
    $items[$row->id] = $row->label;
  }

  return $items;
}

function bookmarkorganizer_user_bookmarks_form_ajax_submit($user) {
  // Get form values from http request
  $values = $_REQUEST;

  // Validate form token manually because normal Form API validation has not been processed
  if (!drupal_valid_token($values['form_token'], 'bookmarkorganizer_user_bookmarks_form')) {
    // If validation fails output an existing label and error message
    $json = array('error' => t('Form validation error'));
    drupal_json_output($json);
    exit();
  }

  // Execute save bookmarks query
  $msg = _bookmarkorganizer_save_bookmarks($user, $values);

  $bookmarks_form = drupal_get_form('bookmarkorganizer_user_bookmarks_form', $user);

  // Send json to browser
  $json = array('message' => $msg, 'bookmarks' => drupal_render($bookmarks_form));
  drupal_json_output($json);
  exit();
}

function bookmarkorganizer_user_bookmarks_form_submit($form, &$form_state) {
  $user = $form_state['build_info']['args'][0];
  // Execute save bookmarks query
  $msg = _bookmarkorganizer_save_bookmarks($user, $form_state['values']);
}

function _bookmarkorganizer_save_bookmarks($user, $values) {
  // Update items weights and folders
  if (isset($values['bookmarks'])) {
    foreach ($values['bookmarks'] as $item) {
      // @TODO: Field 'updated' should be updated only if weight of the item or its parent has been changed
      $fields = array('weight' => $item['weight'], 'parent' => $item['parent'], 'updated' => time('YYYY-MM-DD HH:MM:SS'));
      $query = db_update('bookmarkorganizer_item');
      $query->fields($fields);
      $query->condition('id', $item['id']);
      $query->execute();
    }
  }

  return t('Bookmarks saved.');
}

function bookmarkorganizer_add_folder_view($user) {
  // Get form
  $form = drupal_get_form('bookmarkorganizer_add_folder_form', $user);

  if (!empty($_POST['js'])) {
    // Render form
    $output = drupal_render($form);

    $ajax_action_submit = url("bookmarkorganizer/add/folder/ajax/" . $user->uid);

    // Send to javascript
    $json = array('html' => $output, 'data' => $form, 'path' => $ajax_action_submit);
    drupal_json_output($json);
    exit();
  }
  else {
    // Send data for page load
    return $form;
  }

}

function bookmarkorganizer_add_folder_form($form, $form_state, $user) {
  $form['name'] = array(
    '#title' => t('Add new folder'),
    '#type' => 'textfield',
    '#length' => 255,
  );

  $form['uid'] = array(
    '#type' => 'hidden',
    '#value' => $user->uid,
  );

  // Check if the form is requested by javascript
  if (!empty($_POST['js'])) {

    $form['button'] = array(
      '#type' => 'button',
      '#value' => t('OK'),
      '#attributes' => array('id' => 'edit-submit-addfolder'),
    );

    $form['cancel'] = array(
      '#type' => 'markup',
      '#markup' => l(t('Cancel'), 'user/' . $user->uid . '/bookmarks', array('attributes' => array('id' => 'cancel-addfolder'))),
      '#attributes' => array('id' => 'cancel-addfolder'),
    );

  }
  else {

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
        '#attributes' => array('id' => 'edit-submit-add-folder'),
    );

  }

  $form['#attributes'] = array('class' => 'bookmarkorganizer-add-folder-form');

  return $form;
}

function bookmarkorganizer_add_folder_form_submit($form, &$form_state) {

  // Execute database insert query
  $msg = _bookmarkorganizer_add_folder($form_state['values']['name'], $form_state['values']['uid']);

  // Output a message
  drupal_set_message(t('%bookmarkorganizer_add_folder_message', array('%bookmarkorganizer_add_folder_message' => $msg)));

  // Redirect to the user bookmarks page
  drupal_goto('user/' . $form_state['values']['uid'] . '/bookmarks');
}

function bookmarkorganizer_add_folder_ajax_submit($user) {
  // Get form values from http request
  $values = $_REQUEST;

  // Validate form token manually because normal Form API validation has not been processed
  if (!drupal_valid_token($values['form_token'], 'bookmarkorganizer_add_folder_form')) {
    // If validation fails output an existing label and error message
    $json = array('error' => t('Form validation error'));
    drupal_json_output($json);
    exit();
  }

  // Execute database insert query
  $msg = _bookmarkorganizer_add_folder($values['name'], $user->uid);

  $bookmarks_form = drupal_get_form('bookmarkorganizer_user_bookmarks_form', $user);

  // Send json to browser
  $json = array('message' => $msg, 'bookmarks' => drupal_render($bookmarks_form));
  drupal_json_output($json);
  exit();
}

function bookmarkorganizer_rename_item_form($form, $form_state, $item) {

  // Common rename form elements
  $form['id'] = array(
      '#type' => 'hidden',
      '#value' => $item->id,
  );
  $form['prev_label'] = array(
      '#type' => 'hidden',
      '#value' => $item->label,
  );
  $form['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Rename "%old_name"', array('%old_name' => $item->label)),
      '#default_value' => $item->label,
  );

  $form['#attributes'] = array('id' => 'bookmarkorganizer-rename-item-form-' . $item->id, 'class' => array('bookmarkorganizer-rename-item-form'));

  // Check if the form is requested by javascript
  if (!empty($_POST['js'])) {
    // Button form element is created for javascript use only
    $form['button'] = array(
      '#type' => 'button',
      '#value' => t('OK'),
      '#attributes' => array('id' => 'edit-submit-rename-' . $item->id, 'class' => array('edit-submit-rename')),
    );
    // We do not need label title when using javascript
    unset($form['label']['#title']);
  }
  else {
    // When javascript is disabled use traditional submit element and callback
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
    );
  }

  return $form;
}


/**
 * Custom rename submit for javascript use only.
 */
function bookmarkorganizer_rename_item_ajax_submit($item) {
  // Get form values from http request
  $values = $_REQUEST;

  // Validate form token manually because normal Form API validation has not been processed
  if (!drupal_valid_token($values['form_token'], 'bookmarkorganizer_rename_item_form')) {
    // If validation fails output an existing label and error message
    $json = array('label' => $values['prev_label'], 'error' => t('Form validation error'));
    drupal_json_output($json);
    exit();
  }

  // Execute database update query
  _bookmarkorganizer_rename_item($values);

  // Send json to browser
  $json = array('label' => $values['label']);
  drupal_json_output($json);
  exit();
}

/**
 * Rename form submit.
 */
function bookmarkorganizer_rename_item_form_submit($form, &$form_state) {
  // Get user object for later redirection
  global $user;

  // Execute database update query
  _bookmarkorganizer_rename_item($form_state['values']);

  // Output a message
  drupal_set_message(t('"%prev_label" updated to "%label".', array('%prev_label' => $form_state['values']['prev_label'], '%label' => $form_state['values']['label'])));

  // Redirect to the user bookmarks page
  drupal_goto('user/' . $user->uid . '/bookmarks');
}

/**
 * Toggle folder menu callback.
 */
function bookmarkorganizer_toggle_folder($item) {
  // Validate form token manually because normal Form API validation has not been processed
  if (!drupal_valid_token($_GET['form_token'], 'bookmarkorganizer_user_bookmarks_form')) {
    // If validation fails output an existing label and error message
    $json = array('error' => t('Form validation error'));
    drupal_json_output($json);
    exit();
  }

  if (isset($item->collapsed) && $item->type == 'folder') {
    // Execute update collapsed query
    bookmarkorganizer_update_collapsed_value($item);
  }
  else {
    $json = array('error' => t('Folder not found'));
    drupal_json_output($json);
    exit();
  }

  // Note: Check here against the previous collapsed value, therefore vica versa
  $mode = ($item->collapsed == 1) ? 'opened' : 'closed';
  $msg = t('Folder %mode', array('%mode' => $mode));

  $user = user_load($item->uid);
  // @todo: find a better solution to update bookmarks form. There is now an unnecessary performance hit because of updating the whole tree.
  $bookmarks_form = drupal_get_form('bookmarkorganizer_user_bookmarks_form', $user);

  // Send json to browser
  $json = array('message' => $msg, 'bookmarks' => drupal_render($bookmarks_form));
  drupal_json_output($json);
  exit();
}

/**
 * Helper method to update bookmarkorganizer item in database.
 */
function _bookmarkorganizer_rename_item($values) {
  $id = $values['id'];
  $label = $values['label'];
  $prev_label = $values['prev_label'];

  $fields = array('label' => $label, 'updated' => time('YYYY-MM-DD HH:MM:SS'));
  $query = db_update('bookmarkorganizer_item');
  $query->fields($fields);
  $query->condition('id', $id);
  $query->execute();
}

/**
 * Helper method to update collapsed value on bookmarkorganizer table.
 */
function bookmarkorganizer_update_collapsed_value($item) {
  $new_value = ($item->collapsed == 1) ? 0 : 1;
  $fields = array('collapsed' => $new_value);
  $query = db_update('bookmarkorganizer_folder');
  $query->fields($fields);
  $query->condition('id', $item->content_id);
  $query->execute();
}

/**
 * Helper method to format item label in bookmarkorganizer list.
 */
function _bookmarkorganizer_get_label($item, $label = NULL) {
  if (!$label) {
    $label = $item->label;
  }
  $label_id = 'label-' . $item->id;
  if ($item->type == 'node') {
    $label = l($label, 'node/' . $item->content_id, array('attributes' => array('id' => $label_id, 'class' => array('label'))));
  }
  else {
    $label = t("<span id='@label_id' class='label'>@label</span>", array('@label_id' => $label_id, '@label' => $label));
  }
  return $label;
}

function _bookmarkorganizer_add_folder($name, $uid) {
  // Create new folder if addfolder field is not empty
  if (!empty($name)) {
    bookmarkorganizer_create_folder($uid, $name);
    $msg = t("Folder '@name' added.", array('@name' => $name));
  }
  else {
    $msg = t("Folder cannot be saved because the name field is empty.");
  }
  return $msg;
}

function _bookmarkorganizer_add_item($content_id, $uid, $label, $type, $parent_id = 0, $weight = 0) {
  $item = bookmarkorganizer_get_item_by_content($content_id, $uid, $type);
  if (!$item) {
    $fields = array(
      'content_id' => $content_id,
      'uid' => $uid,
      'parent' => $parent_id,
      'label' => $label,
      'type' => $type,
      'weight' => $weight,
      'created' => time('YYYY-MM-DD HH:MM:SS'),
      'updated' => time('YYYY-MM-DD HH:MM:SS')
    );
    db_insert('bookmarkorganizer_item')->fields($fields)->execute();
    return bookmarkorganizer_get_item_by_content($content_id, $uid, $type);
  }
  else {
    return $item;
  }
}

function bookmarkorganizer_folder_publication($action, $item) {
  // Get form values from http request
  $values = $_REQUEST;

  // Validate form token manually because normal Form API validation has not been processed
  if (!isset($values['form_token']) || !drupal_valid_token($values['form_token'], 'bookmarkorganizer_user_bookmarks_form')) {
    // If validation fails output an existing label and error message
    $json = array('error' => t('Form validation error'));
    drupal_json_output($json);
    exit();
  }

  // Publication can be done to folders only
  if ($item->type != 'folder') {
    return;
  }

  // Value
  $value =  ($action == 'publish') ? 1 : 0;
  $label =  ($action == 'publish') ? t('Publish') : t('Unpublish');
  $alt = ($action == 'publish') ? t('Unpublish this folder') : t('Publish this folder');

  // Update database
  $result = _bookmarkorganizer_folder_publication($item->content_id, $value);

  // Set message
  $success = ($result) ? TRUE : FALSE;
  $msg = ($action == 'publish') ? t('Folder unlocked') : t('Folder locked');
  $label = ($action == 'publish') ? t('Unpublish') : t('Publish');
  $next_action = ($action == 'publish') ? 'unpublish' : 'publish';

  if (!empty($_REQUEST['js'])) {
    // Send to javascript
    $json = array('success' => $success, 'message' => $msg, 'label' => $label, 'action' => $action, 'alt' => $alt, 'url' => url("bookmarkorganizer/folder/$next_action/$item->id"));
    drupal_json_output($json);
    exit();
  }
  else {
    return $msg;
  }

}

function _bookmarkorganizer_folder_publication($fid, $value) {
  // Update database
  $fields = array('public' => $value);
  $query = db_update('bookmarkorganizer_folder');
  $query->fields($fields);
  $query->condition('id', $fid);
  $result = $query->execute();
  // Cache must be cleared because of access permissions changes.
  $cache = url(NULL, array('absolute' => TRUE)) . "bookmarkorganizer/";
  cache_clear_all($cache, 'cache_page', TRUE);
  // Return result
  return $result;
}

/**
 * API function: Publish folder.
 */
function bookmarkorganizer_folder_publish($fid) {
  // Permission check
  $item = bookmarkorganizer_get_item($fid);
  if (!bookmarkorganizer_access_bookmarks($item)) {
    return FALSE;
  }
  // Update database
  return _bookmarkorganizer_folder_publication($fid, 1);
}

/**
 * API function: Unpublish folder.
 */
function bookmarkorganizer_folder_unpublish($fid) {
  // Permission check
  $item = bookmarkorganizer_get_item($fid);
  if (!bookmarkorganizer_access_bookmarks($item)) {
    return FALSE;
  }
  // Update database
  return _bookmarkorganizer_folder_publication($fid, 0);
}

/**
 * API function: Get folder by uid and folder name
 *
 * Returns folder object or FALSE.
 */
function bookmarkorganizer_get_folder_by_name($uid, $name) {
  $type = 'folder';
  $query = db_query(
    "SELECT
      boi.*, bof.collapsed, bof.public
      FROM {bookmarkorganizer_item} boi
      LEFT JOIN {bookmarkorganizer_folder} bof
        ON boi.content_id = bof.id
      WHERE
        boi.type = :type AND boi.uid = :uid AND boi.label = :label",
    array(':type' => $type, ':uid' => $uid, ':label' => $name)
  );
  return $query->fetch();
}

/**
 * API function: Create folder
 *
 * @param uid integer.
 * @param label string.
 *
 * Returns folder object or FALSE.
 */
function bookmarkorganizer_create_folder($uid, $label, $collapsed = 0) {
  // Store a row into bookmarkorganizer_item table
  $fields = array(
    'name' => $label,
    'uid' => $uid,
    'collapsed' => $collapsed
  );
  $folder_id = db_insert('bookmarkorganizer_folder')->fields($fields)->execute();
  return _bookmarkorganizer_add_item($folder_id, $uid, $label, 'folder');
}

/**
 * API function: Create bookmark
 *
 * @param uid integer.
 * @param fid integer (bookmarkorganizer_item table parent).
 * @param content_id integer (node id or folder id).
 * @param label string (bookmarkorganizer_item table label).
 * @param type string (node or folder).
 * @param weight integer (bookmarkorganizer_item table weight).
 *
 * Returns bookmark object or FALSE.
 */
function bookmarkorganizer_add_bookmark($uid, $fid, $content_id, $label, $type = 'node', $weight = 0) {
  $item = _bookmarkorganizer_add_item($content_id, $uid, $label, $type, $fid, $weight);
  $flag = flag_get_flag(variable_get('bookmarkorganizer_flag_name', 'bookmarks'));
  $flag->flag('flag', $content_id, user_load($uid));
  return $item;
}

/**
 * API function: Get bookmark by id
 *
 * @param id integer (bookmarkorganizer_item table id).
 *
 * Returns bookmark object or FALSE.
 */
function bookmarkorganizer_get_bookmark($id) {
  return bookmarkorganizer_get_item($id);
}

/**
 * API function: Get bookmark by id
 *
 * @param content_id integer (node id or folder id).
 * @param uid integer.
 * @param type string (node or folder).
 *
 * Returns bookmark object or FALSE.
 */
function bookmarkorganizer_get_bookmark_by_content($content_id, $uid, $type = 'node') {
  return bookmarkorganizer_get_item_by_content($content_id, $uid, $type);
}

/**
 * API function: Generate bookmark items for all existing flag_content about selected flag
 */
function bookmarkorganizer_generate_bookmarks() {
  $flagtype = variable_get('bookmarkorganizer_flag_name', 'bookmarks');
  $query = db_query("SELECT fc.*, n.title
                       FROM {flag_content} fc, {flags} f, {node} n
                       WHERE f.fid = fc.fid
                         AND f.name = :name
                         AND n.nid = fc.content_id
                         AND fc.content_type = 'node'",
    array(':name' => $flagtype));
  $i = 0;
  while ($flag_item = $query->fetch()) {
    bookmarkorganizer_add_bookmark($flag_item->uid, 0, $flag_item->content_id, $flag_item->title, $flag_item->content_type);
    $i++;
  }
  drupal_set_message(t("Bookmark Organizer generated :num bookmarks.", array(':num' => $i)));
}