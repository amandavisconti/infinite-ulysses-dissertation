<?php

/**
 * Preprocessor for theme('admin_toolbar').
 */
function template_preprocess_admin_toolbar(&$vars) {
  // Set the active block.
  $active = admin_get_settings('active_tab') ? admin_get_settings('active_tab') : key($vars['blocks']);

  if (!empty($vars['blocks'])) {
    $blocks = $vars['blocks'];
    $vars['tabs'] = array();
    $vars['blocks'] = array();
    foreach ($blocks as $block) {
      $bid = $block->bid;
      $vars['tabs'][$bid] = array(
        'title' => $block->subject,
        'attributes' => array('id' => 'admin-tab-' . $bid),
      );
      $vars['blocks'][$bid] = $block;

      // Mark active tab, block.
      if ($bid == $active) {
        $vars['blocks'][$bid]->class = 'admin-active';
        $vars['tabs'][$bid]['active'] = TRUE;
      }
    }
  }
}

/**
 * Theme function for form API element '#type' => 'admin_panes'.
 */
function template_preprocess_admin_panes(&$vars) {
  drupal_add_js(drupal_get_path('module', 'admin') . '/includes/admin.toolbar.js');
  $vars['layout'] = admin_get_settings('layout');
  $vars['labels'] = $vars['panels'] = $vars['others'] = array();
  $first = TRUE;
  foreach (element_children($vars['element']) as $key) {
    if (!isset($vars['element'][$key]['#access']) || $vars['element'][$key]['#access']) {
      if (!empty($vars['element'][$key]['#title'])) {
        $vars['labels'][$key] = l($vars['element'][$key]['#title'], $_GET['q'], array(
          'fragment' => 'admin-pane-' . $key,
          'attributes' => array(
            'class' => array($first ? 'admin-pane-active' : '')
          )
        ));
        $vars['panels'][$key] = $vars['element'][$key];
        $first = FALSE;
      }
      else {
        $vars['others'][] = $vars['element'][$key];
      }
    }
  }
}

/**
 * Generate markup for an admin tab.
 */
function theme_admin_tab($variables) {
  $tab = $variables['tab'];
  $class = $variables['class'];
  $attr = isset($tab['attributes']) ? drupal_attributes($tab['attributes']) : '';
  $class .= !empty($tab['active']) ? ' admin-tab-active' : '';
  $title = filter_xss_admin($tab['title']);
  return "<div class='admin-tab {$class}'><span {$attr}>{$title}</span></div>";
}

/**
 * Alternative to menu_tree_output() which uses admin implementations of the
 * core menu theme functions.
 */
function theme_admin_drilldown_menu_tree_output($variables) {
  $tree = $variables['menu'];
  $output = '';
  $items = array();

  // Pull out just the menu items we are going to render so that we
  // get an accurate count for the first/last classes.
  foreach ($tree as $data) {
    if (!$data['link']['hidden']) {
      $items[] = $data;
    }
  }

  $num_items = count($items);
  foreach ($items as $i => $data) {
    $extra_class = NULL;
    if ($i == 0) {
      $extra_class = 'first';
    }
    if ($i == $num_items - 1) {
      $extra_class = 'last';
    }
    $link = theme('admin_drilldown_menu_item_link', array('item' => $data['link']));
    $in_active_trail = isset($data['link']['in_active_trail']) ? $data['link']['in_active_trail'] : FALSE;
    if ($data['below']) {
      $output .= theme('admin_drilldown_menu_item', array('link' => $link, 'has_children' => $data['link']['has_children'], 'menu' => theme('admin_drilldown_menu_tree_output', array('menu' => $data['below'])), 'in_active_trail' => $in_active_trail, 'extra_class' => $extra_class));
    }
    else {
      $output .= theme('admin_drilldown_menu_item', array('link' => $link, 'has_children' => $data['link']['has_children'], 'menu' => '', 'in_active_trail' => $in_active_trail, 'extra_class' => $extra_class));
    }
  }
  return $output ? theme('admin_drilldown_menu_tree', array('menu' => $output)) : '';
}

/**
 * Alternative to theme_menu_tree().
 * Ensures custom theme-level overrides of theme_menu_tree() do not break
 * markup expected by admin.
 */
function theme_admin_drilldown_menu_tree($variables) {
  $tree = $variables['menu'];
  return theme_menu_tree($tree);
}

/**
 * Alternative to theme_menu_item().
 * Ensures custom theme-level overrides of theme_menu_item() do not break
 * markup expected by admin.
 */
function theme_admin_drilldown_menu_item($variables) {
  $link = $variables['link'];
  $has_children = $variables['has_children'];
  $menu = $variables['menu'];
  $in_active_trail = $variables['in_active_trail'];
  $extra_class = $variables['extra_class'];
  return theme_menu_item($link, $has_children, $menu, $in_active_trail, $extra_class);
}

/**
 * Alternative to theme_menu_item_link().
 */
function theme_admin_drilldown_menu_item_link($variables) {
  $link = $variables['item'];
  $link['localized_options'] = empty($link['localized_options']) ? array() : $link['localized_options'];
  $link['localized_options']['html'] = TRUE;
  $link['localized_options']['purl'] = array('disabled' => TRUE);
  $link['description'] = check_plain(truncate_utf8(strip_tags($link['description']), 150, TRUE, TRUE));
  $link['title'] .= !empty($link['description']) ? "<span class='menu-description'>{$link['description']}</span>" : '';
  $link['title'] = filter_xss_admin($link['title']);
  return l($link['title'], $link['href'], $link['localized_options']);
}
