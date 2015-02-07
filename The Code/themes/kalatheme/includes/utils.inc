<?php

/**
 * @file
 * Kalatheme's helper functions.
 */

// Constants.
define('KALATHEME_BOOTSWATCH_API', 'http://api.bootswatch.com/3/');

// Use Libraries intregration if applicable.
if (kalatheme_use_libraries()) {
  require_once dirname(__FILE__) . '/../includes/libraries.inc';
  // We want to use the global here to respect alterations from modules
  // like ThemeKey.
  global $theme_key;
  if (!defined('KALATHEME_BOOTSTRAP_LIBRARY')) {
    define('KALATHEME_BOOTSTRAP_LIBRARY', $theme_key . '_bootstrap');
  }
}

/**
 * Implements hook_updater_info().
 */
function kalatheme_updater_info_alter(&$updaters) {
  $updaters['library'] = array(
    'class' => 'KalathemeUpdater',
    'name' => t('Update Library'),
    'weight' => -10,
  );
}

/**
 * Loads all dependencies necessary for Kalatheme to operate.
 *
 * @return string
 *   Name of the function for kalatheme_status_messages to call
 */
function _kalatheme_load_dependencies() {
  // If we've already loaded dependencies, just return the correct function to
  // call for status messages.
  $dependencies_loaded = &drupal_static(__FUNCTION__);
  if (isset($dependencies_loaded)) {
    return $dependencies_loaded;
  }

  // Check some minimal module, variable and bootstrap reqs.
  // See if we are running a 3.x version of panels.
  $panels = FALSE;
  if (module_exists('panels')) {
    $info = system_get_info('module', 'panels');
    $regex = '/^7\.x-3\.[0-9]+.*$/';
    // Check major version.
    if (preg_match($regex, $info['version'])) {
      $minor = substr($info['version'], 6);
      if (strpos($info['version'], 'dev') !== FALSE) {
        // Keep commit add for potential future use.
        $dev = substr(substr($minor, strpos($minor, '+') + 1), 0, -4);
        $minor = substr($minor, 0, strpos($minor, '+'));
      }
      if ($minor >= 4) {
        $panels = TRUE;
      }
    }
    elseif (!$info['version']) {
      $panels = TRUE;
    }
  }
  if (!$panels) {
    drupal_set_message(t('Kalatheme requires at least Panels 3.4.'), 'error');
  }

  // See if we are running at least a 2.x branch of jquery_update.
  // Check to see if we are using jquery 1.7+.
  $jq = FALSE;
  if (module_exists('jquery_update')) {
    $info = system_get_info('module', 'jquery_update');
    $regex = '/^7\.x-2\.[0-9]+.*$/';
    $version = explode('.', variable_get('jquery_update_jquery_version'));
    if ($version[0] == 1 && $version >= 7) {
      $jq = TRUE;
    }
  }
  if (!$jq) {
    drupal_set_message(t('Kalatheme requires a 2.x version of JQuery Update that uses at least JQuery 1.7'), 'error');
  }

  // Check to see if we need libraries integration.
  if (kalatheme_use_libraries()) {
    // Check to see if we are running at least 2.1 of libraries.
    $libs = FALSE;
    if (module_exists('libraries')) {
      $info = system_get_info('module', 'libraries');
      $regex = '/^7\.x-2\.[1-9][0-9]*.*$/';
      if (preg_match($regex, $info['version']) || !$info['version']) {
        $libs = TRUE;
      }
    }
    if (!$libs) {
      drupal_set_message(t('Kalatheme requires at least Libraries 2.1.'), 'error');
    }
    // Check to see if bootstrap libraries exist.
    $boot = FALSE;
    if (module_exists('libraries')) {
      $bootstrap = libraries_detect(KALATHEME_BOOTSTRAP_LIBRARY);
      if ($bootstrap && !empty($bootstrap['installed'])) {
        $bootstrap_path = DRUPAL_ROOT . '/' . $bootstrap['library path'];
        $variant = NULL;
        $has_minified_css = file_exists($bootstrap_path . '/css/bootstrap.min.css');
        $has_minified_js = file_exists($bootstrap_path . '/js/bootstrap.min.js');
        if ($has_minified_css && $has_minified_js) {
          $variant = 'minified';
        }
        libraries_load(KALATHEME_BOOTSTRAP_LIBRARY, $variant);
        $boot = TRUE;
      }
    }
    if (!$boot) {
      drupal_set_message(t('You do not have a Bootstrap library installed but that is ok! To get equipped either check out our <a href="@setup_guide">Start Up Guide</a> or run our <a href="@setup_url">Setup Wizard.</a>', array(
        '@setup_guide' => 'https://drupal.org/node/2167149',
        '@setup_url' => url('admin/appearance/kalasetup'),
      )), 'error');
    }
  }
  else {
    $boot = TRUE;
  }

  // Return the correct status message function depending on whether
  // we were able to load all of our Bootstrap dependencies.
  if (!$boot || !$jq || !$panels) {
    return FALSE;
  }
  else {
    return TRUE;
  }
}

/**
 * Modifies the config form.
 *
 * @return array
 *   The modified form
 */
function kalatheme_prepare_config_form(&$form) {
  // Provide needed ajax wrappers.
  $form['actions']['submit']['#prefix'] = "<div id='kala-submit-change-title'>";
  $form['actions']['submit']['#suffix'] = "</div>";
  // Add custom submit and validate functions to handle custom bootstrap
  // libraries.
  if (isset($form['#submit']) && is_array($form['#submit']) && !in_array('kalatheme_custom_bootstrap_library_submit', $form['#submit'])) {
    // Add our submit function first.
    array_unshift($form['#submit'], 'kalatheme_custom_bootstrap_library_submit');
    // Remove system_theme_settings_submit here to prevent
    // form_execute_handlers from adding the submit function
    // to the control batch.
    // We will add it back in later if needed.
    if (($key = array_search('system_theme_settings_submit', $form['#submit'])) !== FALSE) {
      unset($form['#submit'][$key]);
    }
  }
  if (isset($form['#validate']) && is_array($form['#validate']) && !in_array('kalatheme_custom_bootstrap_library_validate', $form['#validate'])) {
    $form['#validate'][] = 'kalatheme_custom_bootstrap_library_validate';
  }

  return $form;
}

/**
 * Gets the total column count from the Bootstrap grid in use.
 *
 * @return int
 *   The total number of columns.
 */
function kalatheme_get_grid_size() {
  // Check for cached result.
  $cached_size = cache_get('kalatheme:grid_size');
  if ($cached_size !== FALSE) {
    return $cached_size->data;
  }
  $size = _kalatheme_load_grid_size();
  cache_set('kalatheme:grid_size', $size, 'cache', CACHE_TEMPORARY);
  return $size;
}

/**
 * Parses the main Bootstrap CSS file to determine its grid size.
 * @todo : fix this so it can probe external css as well
 */
function _kalatheme_load_grid_size() {
  $default_column_count = 12;
  // Load in the Bootstrap css file if using a custom library.
  if (kalatheme_use_libraries()) {
    $bootstrap_directory = libraries_get_path(KALATHEME_BOOTSTRAP_LIBRARY);
    if (!$bootstrap_directory) {
      return $default_column_count;
    }
    $bootstrap_file = $bootstrap_directory . '/css/bootstrap.min.css';
    if (!file_exists($bootstrap_file)) {
      $bootstrap_file = $bootstrap_directory . '/css/bootstrap.css';
      if (!file_exists($bootstrap_file)) {
        return $default_column_count;
      }
    }
    $bootstrap_contents = file_get_contents($bootstrap_file);
    // Find the largest column size.
    $column_sizes = array();
    preg_match_all('/col-md-(\d{1,2})/', $bootstrap_contents, $column_sizes);
    if (empty($column_sizes)) {
      return $default_column_count;
    }
    $column_sizes = array_pop($column_sizes);
    $column_sizes = array_unique($column_sizes);
    foreach ($column_sizes as &$column_size) {
      $column_size = (int) $column_size;
    }
    sort($column_sizes);
    return array_pop($column_sizes);
  }
  else {
    return $default_column_count;
  }
  // @todo what do we do if we aren't using CDN or libraries?
}

/**
 * Calculates a column number based on columns in the total and current grid.
 *
 * @param int $percentage
 *   Whole number version of the percentage to calculate.
 * @param int $total_columns
 *   (optional) The total actual columns in the current row.
 *   Defaults to 2.
 *
 * @return int
 *   The column count, rounded if necessary.
 */
function kalatheme_grid_size($percentage, $total_columns = 2) {
  $columns = $percentage * KALATHEME_GRID_SIZE;
  // Deal with uneven division.
  if (!is_int($columns)) {
    if ($columns > (KALATHEME_GRID_SIZE / $total_columns)) {
      $columns = floor($columns);
    }
    else {
      $columns = ceil($columns);
    }
  }

  return $columns;
}

/**
 * Returns the list of Bootwatch themes.
 */
function kalatheme_get_bootswatch_themes() {
  // Grab Bootstwatches from API or Drupal Cache.
  if (($bootswatches = cache_get('kalatheme:bootswatches', 'cache')) === FALSE) {
    $bootswatch_info = drupal_http_request(KALATHEME_BOOTSWATCH_API);
    if ($bootswatch_info && $bootswatch_info->code == 200) {
      $data = json_decode($bootswatch_info->data);
      if (isset($data->themes) && is_array($data->themes)) {
        cache_set('kalatheme:bootswatches', $data->themes, 'cache', CACHE_TEMPORARY);
        $bootswatches = $data->themes;
      }
    }
  }
  else {
    $bootswatches = $bootswatches->data;
  }
  // For the default option, add to the front.
  $default = new stdClass();
  $default->name = 'Default';
  array_unshift($bootswatches, $default);
  // For the null option.
  $none = new stdClass();
  $none->name = 'None';
  $bootswatches[] = $none;
  // If the user can use a custom theme.
  if (kalatheme_backend_check()) {
    $custom = new stdClass();
    $custom->name = 'Custom';
    $bootswatches[] = $custom;
  }
  return $bootswatches;
}

/**
 * Get specific information about a bootswatch theme.
 */
function kalatheme_get_bootswatch_theme($theme) {
  $bootswatches = kalatheme_get_bootswatch_themes();
  foreach ($bootswatches as $bootswatch) {
    if ($bootswatch->name == ucfirst($theme)) {
      return $bootswatch;
    }
  }
}

/**
 * Helper function to return if using libraries integration or not.
 */
function kalatheme_use_libraries() {
  if (function_exists('theme_get_setting')) {
    $library = theme_get_setting('bootstrap_library');
    return (isset($library) && $library == 'custom');
  }
  return FALSE;
}

/**
 * Checks Drupal stack.
 *
 * We want to check if the webserver can write our stuff to the
 * right places. If it can't, then we are going to tell the user
 * what's up.
 *
 * @return bool
 *   True if all good, message if no good
 */
function kalatheme_backend_check() {
  // Verify FTP support.
  $ftp_installed = extension_loaded('ftp');
  // Verify SSH support.
  $ssh_installed = extension_loaded('ssh2');
  // Verify web server write permissions.
  $install_permissions = kalatheme_has_write_access();
  // Verify update module is enabled.
  $updates_module = module_exists('update');

  return (($ftp_installed || $ssh_installed || $install_permissions) && $updates_module);
}

/**
 * Kalatheme has write access to libraries and modules directories.
 *
 * @return bool
 *   Return if kalatheme can write to that directory.
 */
function kalatheme_has_write_access() {
  return is_writable(drupal_realpath(kalatheme_lib_dir())) && is_writable(drupal_realpath('sites/all/modules'));
}

/**
 * Return the libraries dir to check.
 */
function kalatheme_lib_dir() {
  return is_dir('sites/all/libraries') ? 'sites/all/libraries' : 'sites/all';
}
