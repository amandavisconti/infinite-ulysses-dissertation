<?php
/**
 * @file
 * Theme and hook functions for views.
 */

// Load plugins
// We need to do this so that our views plugin class will be loaded correctly
// since we cant use the files[] directive in a theme .info file.
require_once dirname(__FILE__) . '/../views/plugins/views_plugin_style_grid_bootstrap.inc';

/**
 * Implements template_preprocess().
 */
function kalatheme_preprocess_views_view(&$variables) {
  $view = $variables['view'];

  // Add a .row class to wrap .views-row.
  $variables['view_content_class'] = 'view-content';
  $plugin = $view->style_plugin->plugin_name;
  if ($plugin == 'grid') {
    $variables['view_content_class'] .= ' row';
  }
}

/**
 * Adds responsive Bootstrap options to views grid displays.
 *
 * Implement hook_views_plugins_alter().
 */
function kalatheme_views_plugins_alter(&$plugins) {
  if (isset($plugins['style']['grid'])) {
    $plugins['style']['grid']['handler'] = 'views_plugin_style_grid_bootstrap';
    $plugins['style']['grid']['path'] = drupal_get_path('theme', 'kalatheme') . '/views/plugins';
    $plugins['style']['grid']['file'] = 'views_plugin_style_grid_bootstrap.inc';
  }
  $plugins = $plugins;
}

/**
 * Implements hook_preprocess_views_view_grid().
 */
function kalatheme_preprocess_views_view_grid(&$variables) {
  $variables['gridsize'] = kalatheme_get_grid_size();
  $responsive_tiers = array('xs', 'sm', 'lg');
  foreach ($responsive_tiers as $tier) {
    if (!$variables['options']['responsive_toggle']) {
      $variables['options']['columns_' . $tier] = 1;
    }
    if ($variables['gridsize'] % $variables['options']['columns_' . $tier] === 0) {
      $variables[$tier] = $variables['gridsize'] / $variables['options']['columns_' . $tier];
    }
  }
  // This is set using the default grid columns value not columns_tier.
  if ($variables['gridsize'] % $variables['options']['columns'] === 0) {
    $variables['md'] = $variables['gridsize'] / $variables['options']['columns'];
    // We need to do ths too.
    if (!$variables['options']['responsive_toggle']) {
      $variables['lg'] = $variables['gridsize'] / $variables['options']['columns'];
    }
  }
}

/**
 * Implements hook_preprocess_views_view_table().
 */
function kalatheme_preprocess_views_view_table(&$variables) {
  $rows = array();
  foreach ($variables['row_classes'] as $row) {
    // This assume the first element of any row will be the odd/even class,
    // which we no longer need.
    array_shift($row);
    $rows[] = $row;
  }
  $variables['row_classes'] = $rows;

  // Add in bootstrap classes.
  $variables['classes_array'] = array(
    'table',
    'table-striped',
    'table-bordered',
    'table-hover',
  );

  // Remove the active class from table cells,
  // as Bootstrap 3 gives them a funky background.
  $handler = $variables['view']->style_plugin;
  $active = !empty($handler->active) ? $handler->active : FALSE;
  if ($active && array_key_exists($active, $variables['field_classes'])) {
    foreach ($variables['field_classes'][$active] as &$cell) {
      $cell_classes = explode(' ', $cell);
      $active_class_index = array_search('active', $cell_classes);
      if ($active_class_index !== FALSE) {
        unset($cell_classes[$active_class_index]);
        $cell = implode(' ', $cell_classes);
      }
    }
  }
}

/**
 * Implements template_preprocess_views_ui_display_tab_setting().
 */
function kalatheme_preprocess_views_ui_display_tab_setting(&$variables) {
  $variables['settings_links'] = str_replace('class="label"', 'class="views-admin-label"', $variables['settings_links']);
}


/**
 * Implements theme_views_mini_pager().
 */
function kalatheme_views_mini_pager($vars) {
  global $pager_page_array, $pager_total;

  $tags = $vars['tags'];
  $element = $vars['element'];
  $parameters = $vars['parameters'];

  // Current is the page we are currently paged to.
  $pager_current = $pager_page_array[$element] + 1;
  // Max is the maximum page number.
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  if ($pager_total[$element] > 1) {

    $li_previous = theme('pager_previous', array(
      'text' => (isset($tags[1]) ? $tags[1] : t('‹‹')),
      'element' => $element,
      'interval' => 1,
      'parameters' => $parameters,
    ));
    if (empty($li_previous)) {
      $li_previous = "&nbsp;";
    }

    $li_next = theme('pager_next', array(
      'text' => (isset($tags[3]) ? $tags[3] : t('››')),
      'element' => $element,
      'interval' => 1,
      'parameters' => $parameters,
    ));

    if (empty($li_next)) {
      $li_next = "&nbsp;";
    }

    $items[] = array(
      'class' => array('pager-previous'),
      'data' => $li_previous,
    );

    $items[] = array(
      'class' => array('pager-current'),
      'data' => '<span>' . t('@current of @max', array('@current' => $pager_current, '@max' => $pager_max)) . '</span>',
    );

    $items[] = array(
      'class' => array('pager-next'),
      'data' => $li_next,
    );
    return theme('item_list', array(
      'items' => $items,
      'title' => NULL,
      'type' => 'ul',
      'attributes' => array('class' => array('pagination')),
    ));
  }
}
