<?php
/**
 * @file
 * Kalatheme's primary magic delivery system (MDS).
 */

// Use DIRNAME instead of drupal_get_path so we can use this
// during an install profile without nuking the world.
// Load some helper functions..
require_once dirname(__FILE__) . '/includes/utils.inc';

// Asset stuff.
define('KALATHEME_BOOTSTRAP_CSS', '//netdna.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css');
define('KALATHEME_BOOTSTRAP_JS', '//netdna.bootstrapcdn.com/bootstrap/3.2.0/js/bootstrap.min.js');
define('KALATHEME_FONTAWESOME_CSS', '//netdna.bootstrapcdn.com/font-awesome/4.1.0/css/font-awesome.min.css');

// Grid size constants.
define('KALATHEME_GRID_SIZE', kalatheme_get_grid_size());
define('KALATHEME_GRID_FULL', 1);
define('KALATHEME_GRID_HALF', 1 / 2);
define('KALATHEME_GRID_THIRD', 1 / 3);
define('KALATHEME_GRID_FOURTH', 1 / 4);
define('KALATHEME_GRID_FIFTH', 1 / 5);
define('KALATHEME_GRID_SIXTH', 1 / 6);
// Just because we can!
define('KALATHEME_GRID_SILLY', 1 / 42);

// Load Bootstrap overrides of Drupal theme things.
require_once dirname(__FILE__) . '/includes/core.inc';
require_once dirname(__FILE__) . '/includes/icons/icons.inc';
require_once dirname(__FILE__) . '/includes/fapi.inc';
require_once dirname(__FILE__) . '/includes/fields.inc';
require_once dirname(__FILE__) . '/includes/menu.inc';
require_once dirname(__FILE__) . '/includes/panels.inc';
require_once dirname(__FILE__) . '/includes/views.inc';

/**
 * Implements hook_theme().
 *
 * Theme specific compontents can be namespaced kt_<component>
 * to avoid conflicts with other projects.
 */
function kalatheme_theme($existing, $type, $theme, $path) {
  return array(
    'menu_local_actions' => array(
      'variables' => array('menu_actions' => NULL, 'attributes' => NULL),
      'file' => 'includes/menu.inc',
    ),
    'kt_site_header' => array(
      'template' => 'templates/sections/kt-site-header',
      'variables' => array(
        'front_page' => FALSE,
        'logo' => '',
        'site_name' => '',
        'hide_site_name' => FALSE,
        'site_slogan' => '',
        'hide_site_slogan' => FALSE,
      ),
    ),
    'kt_navbar' => array(
      'template' => 'templates/bootstrap/kt-navbar',
      'variables' => array(
        'main_menu' => NULL,
        'main_menu_expanded' => NULL,
        'secondary_menu' => NULL,
        'site_name' => NULL,
        'front_page' => NULL,
        'site_name' => NULL,
      ),
    ),
    'icon_html_tag' => array(
      'file' => 'includes/icons/icons.inc',
    ),
  );
}
/**
 * Utility function to remove conflicting scripts.
 */
function _kalatheme_remove_by_key(array $keys, array $target) {
  foreach ($keys as $key) {
    if (array_key_exists($key, $target)) {
      unset($target[$key]);
    }
  }
  return $target;
}

/**
 * Implements hook_js_alter().
 */
function kalatheme_js_alter(&$javascript) {
  $excludes = array('misc/progress.js');
  $javascript = _kalatheme_remove_by_key($excludes, $javascript);
}

/**
 * Helper function to return an array of CSS to remove on the page.
 */
function _kalatheme_css_excludes() {
  // Unset some core css & panopoly css.
  $excludes = &drupal_static(__FUNCTION__);
  if (!isset($excludes)) {
    if ($cache = cache_get('kalatheme_css_excludes')) {
      $excludes = $cache->data;
    }
    else {
      // Can add more expensive operations here.
      $panopoly_magic_path = drupal_get_path('module', 'panopoly_magic');
      $excludes = array(
        drupal_get_path('module', 'panopoly_admin') . '/panopoly-admin.css',
        $panopoly_magic_path . '/css/panopoly-modal.css',
        $panopoly_magic_path . '/css/panopoly-magic.css',
        'modules/system/system.menus.css',
        drupal_get_path('module', 'admin_views') . '/admin_views.css',
      );
      // Set the cache.
      cache_set('kalatheme_css_excludes', $excludes, 'cache');
    }
  }
  return $excludes;
}

/**
 * Remove conflicting CSS.
 *
 * Implements hook_css_alter().
 */
function kalatheme_css_alter(&$css) {
  $excludes = _kalatheme_css_excludes();
  $css = _kalatheme_remove_by_key($excludes, $css);
}


/**
 * Load Kalatheme dependencies.
 *
 * Implements template_preprocess_html().
 */
function kalatheme_preprocess_html(&$variables) {
  // Load all dependencies.
  _kalatheme_load_dependencies();
  // Add variables for path to theme.
  $variables['base_path'] = base_path();
  $variables['path_to_kalatheme'] = dirname(__FILE__);
  // Add meta for Bootstrap Responsive.
  // <meta name="viewport" content="width=device-width, initial-scale=1.0">
  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1.0',
    ),
  );
  drupal_add_html_head($element, 'bootstrap_responsive');
}

/**
 * Override or insert variables into the page template.
 *
 * Implements template_process_page().
 */
function kalatheme_process_page(&$variables) {
  // Add Bootstrap JS and stock CSS.
  global $base_url;
  $base = parse_url($base_url);
  // Use the CDN if not using libraries.
  if (!kalatheme_use_libraries()) {
    $library = theme_get_setting('bootstrap_library');
    if ($library !== 'none' && !empty($library)) {
      // Add the JS.
      drupal_add_js($base['scheme'] . ":" . KALATHEME_BOOTSTRAP_JS, 'external');
      // Add the CSS.
      $css = ($library === 'default') ? KALATHEME_BOOTSTRAP_CSS : kalatheme_get_bootswatch_theme($library)->cssCdn;
      drupal_add_css($base['scheme'] . ":" . $css, 'external');
    }
  }
  $font_awesome_active = FALSE;
  // Use Font Awesome.
  if (theme_get_setting('font_awesome_cdn')) {
    $font_awesome_active = TRUE;
    drupal_add_css($base['scheme'] . ":" . KALATHEME_FONTAWESOME_CSS, 'external');
  }
  // Let JS know that we have this enabled.
  drupal_add_js(array('kalatheme' => array('fontawesome' => $font_awesome_active)), 'setting');
  // Define variables to theme local actions as a dropdown.
  $dropdown_attributes = array(
    'container' => array(
      'class' => array('dropdown', 'actions', 'pull-right'),
    ),
    'toggle' => array(
      'class' => array('dropdown-toggle', 'enabled'),
      'data-toggle' => array('dropdown'),
      'href' => array('#'),
    ),
    'content' => array(
      'class' => array('dropdown-menu'),
    ),
  );

  // Add local actions as the last item in the local tasks.
  if (!empty($variables['action_links'])) {
    $variables['tabs']['#primary'][]['#markup'] = theme('menu_local_actions', array('menu_actions' => $variables['action_links'], 'attributes' => $dropdown_attributes));
    $variables['action_links'] = FALSE;
  }

  // Get the entire main menu tree.
  $main_menu_tree = array();
  $main_menu_tree = menu_tree_all_data('main-menu', NULL, 2);
  // Add the rendered output to the $main_menu_expanded variable.
  $variables['main_menu_expanded'] = menu_tree_output($main_menu_tree);

  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.
  $variables['hide_site_name']   = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, the site_slogan will be empty,
    // so we rebuild it.
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }
  // Since the title and the shortcut link are both block level elements,
  // positioning them next to each other is much simpler with a wrapper div.
  if (!empty($variables['title_suffix']['add_or_remove_shortcut']) && $variables['title']) {
    // Add a wrapper div using title_prefix and title_suffix render elements.
    $variables['title_prefix']['shortcut_wrapper'] = array(
      '#markup' => '<div class="shortcut-wrapper clearfix">',
      '#weight' => 100,
    );
    $variables['title_suffix']['shortcut_wrapper'] = array(
      '#markup' => '</div>',
      '#weight' => -99,
    );
    // Make sure the shortcut link is the first item in title_suffix.
    $variables['title_suffix']['add_or_remove_shortcut']['#weight'] = -100;
  }

  // If panels arent being used at all.
  $variables['no_panels'] = !(module_exists('page_manager') && page_manager_get_current_page());

  // Check if we're to always print the page title, even on panelized pages.
  $variables['always_show_page_title'] = theme_get_setting('always_show_page_title') ? TRUE : FALSE;

}

/**
 * Override or insert variables into the maintenance page template.
 */
function kalatheme_process_maintenance_page(&$variables) {
  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.
  $variables['hide_site_name']   = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, rebuild the empty site slogan.
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }
}

/**
 * Override or insert variables into the node template.
 *
 * Implements template_preprocess_node().
 */
function kalatheme_preprocess_node(&$variables) {
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }
}

/**
 * Override or insert variables into the block template.
 *
 * Implements template_preprocess_block().
 */
function kalatheme_preprocess_block(&$variables) {
  // In the header region visually hide block titles.
  if ($variables['block']->region == 'header') {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
}