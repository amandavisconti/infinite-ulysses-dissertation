<?php
/**
 * @file
 * Drushhhhhhhh
 */

// Pull in the setup magic.
require_once 'includes/config.inc';

/**
 * Implements hook_drush_command().
 */
function kalatheme_drush_command() {
  $items = array();

  $items['kalatheme'] = array(
    'description' => 'Create a Kalatheme subtheme.',
    'arguments' => array(
      'name'         => 'The name of your subtheme.',
    ),
    'options' => array(
      'destination' => 'The destination of your subtheme. Defaults to sites/all/themes.',
      'base-theme' => 'This will let you use a different basetheme than kalatheme.',
      'custom' => 'The name of a Bootstwatch theme or a URL containing a custom Bootstrap library archive. Can also use "none" if you hate bootstrap but are still using this theme for some reason.',
      'fonts' => 'Add FontAwesome 4 support.',
      'no-magic' => 'Stop Kalatheme from autoloading the additional assets from a custom third party Bootstrap Library into your subtheme',
    ),
    'examples' => array(
      'drush kalatheme "My Awesome Theme"' => 'Creates a kalatheme subtheme called "My Theme"',
      'drush kalatheme "My Awesome Theme" --fonts' => 'Creates a kalatheme subtheme called "My Theme" with Font Awesome installed and configured.',
      'drush kalatheme "My Awesome Theme" --custom=yeti' => 'Creates a kalatheme subtheme called "My Theme" with the Yeti Bootstwatch theme.',
      'drush kalatheme "My Awesome Theme" --base-theme=kalasubtheme' => 'Creates a kalatheme subtheme called "My Theme" but sets its basetheme to "kalsubtheme"',
      'drush kalatheme "My Awesome Theme" --custom=http://www.mywebsite.com/mycustombootstraparchive.zip' => 'Creates a kalatheme subtheme called "My Theme" but uses the custom bootlibrary',
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function kalatheme_drush_help($section) {
  switch ($section) {
    case 'drush:kalatheme':
      return dt('This command will create a Kalatheme subtheme.');
  }
}

/**
 * Implements drush_hook_command().
 */
function drush_kalatheme($name = NULL) {
  // Let's get it started.
  $theme_settings = drush_kalatheme_default_settings();
  // Get arguments and parse the machine name.
  if (!$name) {
    drush_print(dt('You need a theme name.'));
    return;
  }
  if (!empty($name)) {
    $theme_settings['subtheme_name'] = $name;
  }
  $machine_name = kalatheme_get_machine_name($name);

  // Get options.
  // Get paths to theme folder.
  $subtheme_path = 'sites/all/themes';
  if ($destination = drush_get_option('destination')) {
    $subtheme_path = drush_trim_path($destination);
  }
  // See if we are going to use a custom bootswatch theme or pull in a custom
  // bootstrap library from a URL archive.k
  $custom = FALSE;
  if ($custom = drush_get_option('custom')) {
    $bootswatch = array();
    foreach (kalatheme_get_bootswatch_themes() as $theme) {
      $bootswatch[] = strtolower($theme->name);
    }
    // @todo handle custom libaries
    if (filter_var($custom, FILTER_VALIDATE_URL)) {
      $theme_settings['bootstrap_library'] = 'custom';
    }
    elseif (in_array($custom, $bootswatch) && $custom != "custom") {
      $theme_settings['bootstrap_library'] = $custom;
    }
    elseif (!filter_var($custom, FILTER_VALIDATE_URL) && !in_array($custom, $bootswatch)) {
      drush_print(dt('Not a valid URL or Bootswatch theme!'));
      return;
    }
  }
  // Use a custom subtehme basetheme instead of kalatheme.
  if ($basetheme = drush_get_option('base-theme')) {
    $base_themes = array_keys(kalatheme_get_base_themes());
    if (in_array($basetheme, $base_themes)) {
      $theme_settings['subtheme_base'] = drush_get_option('base-theme');
    }
    else {
      drush_print(dt('This is not a valid Kalatheme subtheme!'));
      return;
    }
  }
  // Get FONT AWESOME.
  $fonts = FALSE;
  if (drush_get_option('fonts')) {
    $theme_settings['icon_font_library'] = 'font_awesome';
    $theme_settings['font_awesome_cdn'] = TRUE;
  }
  // Autoload assets?
  $no_magic = FALSE;
  if (drush_get_option('no-magic')) {
    $no_magic = drush_get_option('no-magic');
  }

  // Write the subtheme and then move it to the right spot.
  $dtemp = drush_tempdir();
  $sub_tmp_directory = drush_normalize_path($dtemp . '/' . $machine_name);
  if (kalatheme_write_stuff(kalatheme_custom_subtheme_skeleton($theme_settings), $sub_tmp_directory)) {
    kalatheme_custom_subtheme_vars($theme_settings);
    $subtheme_path = drush_normalize_path(drush_get_context('DRUSH_DRUPAL_ROOT') . '/' . $subtheme_path . '/' . $machine_name);
  }
  else {
    drush_print(dt('Could not write to temp directory!'));
    return;
  }

  // Write the custom bootstrap library if applicable.
  $current_dir = getcwd();
  chdir($dtemp);
  if (($theme_settings['bootstrap_library'] == 'custom')) {
    $filepath = drush_download_file($custom);
    if (empty($filepath)) {
      drush_print(dt('Could not download custom library.'));
      return;
    }
    $filename = basename($filepath);
    $dirname = basename($filepath, '.zip');
    $library_name = $machine_name . '_bootstrap';
    $library_path = drush_normalize_path(drush_get_context('DRUSH_DRUPAL_ROOT') . '/' . 'sites/all/libraries' . '/' . $library_name);
    $lib_tmp_directory = drush_normalize_path($dtemp . '/' . $library_name);

    // Decompress the zip archive.
    drush_tarball_extract($filename, $dirname, TRUE);
    // Import additional third party bootstrap library assets into
    // your subtheme.
    $files = array_keys(drush_scan_directory($dirname, '(.*)'));
    drush_op('kalatheme_custom_bootstrap_library_integrate', $files, $machine_name, $dtemp, $lib_tmp_directory, $sub_tmp_directory, !$no_magic);
    drush_move_dir($lib_tmp_directory, $library_path, FILE_EXISTS_OVERWRITE);
  }

  // Return.
  chdir($current_dir);
  // Move the subtheme into the right directory.
  drush_move_dir(drush_normalize_path($sub_tmp_directory), $subtheme_path,
    FILE_EXISTS_OVERWRITE);
  // Notify user of the new polution.
  $message = dt('kalatheme subtheme "!name" created in: !path.', array(
    '!name' => $name,
    '!path' => $subtheme_path,
  ));
  drush_print($message);
}

/**
 * Returns an array of default settings.
 */
function drush_kalatheme_default_settings() {
  return array(
    'var' => NULL,
    'bootstrap_library' => 'default',
    'fontawesome' => 1,
    'bootstrap_upload' => NULL,
    'build_subtheme' => 1,
    'subtheme_name' => NULL,
    'subtheme_base' => 'kalatheme',
    'default_logo' => 1,
    'default_favicon' => 1,
    'toggle_logo' => 0,
    'toggle_favicon' => 1,
    'responsive_toggle' => 1,
    'pane_styles_toggle' => 1,
  );
}
