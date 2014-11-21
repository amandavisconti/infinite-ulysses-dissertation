<?php

/**
 * @file
 * The Bookmark Organizer theme.inc.
 */

function theme_bookmarkorganizer_container($variables) {
  $elements = $variables['elements'];
  $output = '<div class="' . $elements['#classes'] . '">';
  $output .= render($elements['index']);
  if (variable_get('bookmarkorganizer_enable_preview_pane')) {
    $output .= render($elements['preview']);
  }
  $output .= render($elements['dialog']);
  $output .= '</div>';
  return $output;
}

function theme_bookmarkorganizer_action_list($variables) {
  $links = array();
  foreach ($variables['links']['#items'] as $link) {
    $links[] = render($link);
  }
  return theme('item_list', array('items' => $links, 'attributes' => array('class' => array('links', 'inline'))));
}
/**
 * Implements hook_link__bookmarkorganizer_action().
 *
 * Available variants:
 * hook_link__bookmarkorganizer_action
 * hook_link__bookmarkorganizer_action_delete
 * hook_link__bookmarkorganizer_action_rename.
 *
 * @param $variables
 *
 */
function theme_link__bookmarkorganizer_action($variables) {
  return $variables['element']['#markup'];
}

function theme_bookmarkorganizer_addfolder($variables) {
  return $variables['element']['#markup'];
}


function theme_bookmarkorganizer_user_bookmarks_form($variables) {
  $form = $variables['form'];

  drupal_add_library('system', 'ui.dialog');

  drupal_add_css(drupal_get_path('module', 'bookmarkorganizer') . '/theme/bookmarkorganizer.css');

  $enable_preview_pane = variable_get('bookmarkorganizer_enable_preview_pane');

  // Settings for bookmarkorganizer.js
  $js_settings = array(
    'uid' => $form['uid']['#value'],
    'savepath' => url('bookmarkorganizer/save/'),
    'togglefolderpath' => url('bookmarkorganizer/toggle/ajax/'),
    'activelistpath' => url('bookmarkorganizer/activelist/'),
    'previewrootpath' => url('bookmarkorganizer/' . $form['uid']['#value'] . '/activelist/'),
    'enable_preview_pane' => $enable_preview_pane,
  );

  // Add settings
  drupal_add_js(array('bookmarkorganizer' => $js_settings), 'setting');
  // Add bookmarkorganizer custom javascript functionality
  drupal_add_js(drupal_get_path('module', 'bookmarkorganizer') . '/js/bookmarkorganizer.js');

  drupal_add_tabledrag('bookmarkorganizer-index-table', 'order', 'sibling', 'item-weight');
  drupal_add_tabledrag('bookmarkorganizer-index-table', 'match', 'parent', 'item-parent', 'item-parent', 'item-id', TRUE);

  $header = array(array('data' => t('My Bookmarks'), 'class' => array('bookmarkorganizer-index-table-heading')), 'Weight', 'Folder');

  $rows = array();

  foreach (element_children($form['bookmarks']) as $key) {

    $element = &$form['bookmarks'][$key];

    $element['weight']['#attributes']['class'] = array('item-weight');
    $element['parent']['#attributes']['class'] = array('item-parent');
    $element['id']['#attributes']['class'] = array('item-id');

    $label = render($element['label']);

    if ($element['type']['#value'] == 'folder') {
      $folder_status_class = ($element['collapsed']['#value'] == 1) ? 'collapsed' : 'open';
      $label = '<div class="folder-link"></div>' . $label;
    }
    else {
      $folder_status_class = '';
    }

    $row = array();
    $row[] = array('data' => theme('indentation', array('size' => $element['#depth'])) . $label . render($element['actions']) . render($element['id']), 'class' => array('row-label', 'indentation-' . $element['#depth']));
    $row[] = render($element['weight']);
    $row[] = render($element['parent']);
    // Add a draggable class to every table row (<tr>)
    $classes = array('draggable', 'row', 'item-' . $element['id']['#value'], $element['type']['#value'], $folder_status_class);
    if ($element['#attributes'] && $element['#attributes']['class']) {
      $classes = array_merge($classes, $element['#attributes']['class']);
    }
    $rows[] = array('data' => $row, 'class' => $classes);
  }

  // Assign classes
  $preview_mode = ($enable_preview_pane) ? 'preview-enabled' : 'preview-disabled';
  $classes = array('bookmarkorganizer-index-table', $preview_mode);
  if (count($rows) == 0) {
    $classes[] = 'empty';
  }

  // Output
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'bookmarkorganizer-index-table', 'class' => $classes)));
  $output .= drupal_render_children($form);
  return $output;
}

function theme_bookmarkorganizer_add_folder_form($variables) {
  $form = $variables['form'];

  $titlearea = '<div class="form-title"><h2>' . $form['name']['#title'] . '</h2></div>';
  unset($form['name']['#title']);
  $output = $titlearea . '<div class="form-content">' . drupal_render_children($form) . '</div>';
  return $output;
}

function theme_bookmarkorganizer_delete_item_form($variables) {
  $form = $variables['form'];
  $item = $form['bookmarkorganizer_item']['#value'];

  if ($item->type == 'folder') {
    $msg = t('Do you really want to delete folder %label?', array('%label' => $item->label));
  }
  else {
    $msg = t('Do you really want to delete bookmark %label?', array('%label' => $item->label));
  }
  $msg = "<h2>$msg</h2>";

  $output = '<div class="form-title">' . $msg . '</div>' . '<div class="form-content">' . drupal_render_children($form) . '</div>';
  return $output;
}

/**
 * Wrapper theme for bookmarks' index pane.
 */
function theme_bookmarkorganizer_index($variables) {
  $output = '<div id="bookmarkorganizer-index" class="bookmarkorganizer-index">';
  $output .= render($variables['list']);
  $output .= render($variables['actions']);
  $output .= '</div>';
  return $output;
}

/**
 * Wrapper theme for bookmarks' index pane actions.
 */
function theme_bookmarkorganizer_actions($variables) {
  $elements = $variables['elements'];
  $output = '<div class="bookmarkorganizer-actions">';
  $links = array();
  foreach ($elements as $key => $element) {
    if ($key[0] != '#') {
      $links[] = render($element);
    }
  }
  if (!empty($links)) {
    $output .= theme('item_list', array('items' => $links, 'attributes' => array('class' => array('links', 'inline'))));
  }
  $output .= '</div>';
  return $output;
}

/**
 * Wrapper theme for the preview pane.
 */
function theme_bookmarkorganizer_preview($variables) {
  $elements = $variables['elements'];
  $output = '<div id="bookmarkorganizer-preview" class="bookmarkorganizer-preview">';
  if (isset($elements['#item']) && $elements['#item']->type == 'folder') {
    $output .= '<div class="bookmarkorganizer-preview-headerbar">';
    $output .= render($elements['header']);
    $output .= '</div>';
    $output .= render($elements['content']);
  }
  else {
    $output .= render($elements['content']);
  }
  $output .= '</div>';
  return $output;
}


