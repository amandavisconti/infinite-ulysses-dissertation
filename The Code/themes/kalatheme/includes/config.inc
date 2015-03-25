<?php
/**
 * @file
 * Kalatheme's subtheme config form stuff
 */


// Load utilities.
require_once dirname(__FILE__) . '/utils.inc';

/**
 * Form constructor for Bootstrap library selection form.
 *
 * @return array
 *   Library form for boostrap.
 */
function kalatheme_bootstrap_library_form() {
  $form = array();

  $form['bootstrap'] = array(
    '#type' => 'fieldset',
    '#title' => t('Kalatheme Settings'),
    '#weight' => -99,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['bootstrap']['bootstrap_library'] = array(
    '#type' => 'select',
    '#title' => t('Select a Bootstrap library.'),
    '#description' => t('Select a Bootstrap library to use with your subtheme. Visit <a href="@bootswatch">Bootswatch</a> for more info. Select "custom" to upload a third party Bootstrap library such as one from <a href="@wrapbootstrap">Wrapbootstrap</a> or built using Bootstrap\'s <a href="@bootcustom">Customization Utility</a>.', array(
      '@bootswatch' => 'http://bootswatch.com/',
      '@wrapbootstrap' => 'http://wrapbootstrap.com/',
      '@bootcustom' => 'http://getbootstrap.com/customize/',
    )),
    '#ajax' => array(
      'callback' => 'kalatheme_ajax_bootstrap_preview_submit',
      'wrapper' => 'kalapreview-image-load-image',
      'method' => 'replace',
      'effect' => 'fade',
      'progress' => array('type' => 'none'),
    ),
  );
  foreach (kalatheme_get_bootswatch_themes() as $theme) {
    $form['bootstrap']['bootstrap_library']['#options'][strtolower($theme->name)] = $theme->name;
  }
  $form['bootstrap']['bootstrap_preview'] = array(
    '#type' => 'markup',
    '#prefix' => "<div id='kalapreview-image-load-image'>",
    '#suffix' => "</div>",
  );
  $form['bootstrap']['icon_font_library'] = array(
    '#type' => 'select',
    '#title' => t('Icon Font Library'),
    '#default_value' => FALSE,
    '#description' => t("Select which icon font library you want to use with the theme. If you're using the whole Bootstrap library, glyphicons are already included. Font Awesome is a great choice with over 400 icons."),
    '#options' => array(
      'bootstrap_glyphicons' => t('Bootstrap Glyphicons'),
      'font_awesome' => t('Font Awesome'),
      'none' => t('None'),
    ),
  );
  $form['bootstrap']['font_awesome_cdn'] = array(
    '#type' => 'checkbox',
    '#title' => t('CDN Font Awesome CSS'),
    '#default_value' => FALSE,
    '#description' => t('Check here if you want to use font-awesome CSS CDN to load the icon-font library'),
    // This #states rule says that the "file upload" field should only
    // be shown if the "bootstrap_library" form element is set to "custom".
    '#states' => array(
      'visible' => array(
        ':input[name="icon_font_library"]' => array('value' => 'font_awesome'),
      ),
    ),
  );
  $form['bootstrap']['bootstrap_upload'] = array(
    '#type' => 'file',
    '#title' => t('Custom Bootstrap library'),
    '#description' => t('Upload any <a href="@valid_doc">valid bootstrap library</a> here to complete your install. For example: %filename from your local computer. <strong>DISCLAIMER:</strong> This works pretty good with some libraries but not so great with others. Use this to kick start your project not as a end-all-be-all tool.', array(
      '%filename' => 'name.tar.gz',
      '@valid_doc' => 'http://www.drupal.org/project/kalatheme',
    )),
    // This #states rule says that the "file upload" field should only
    // be shown if the "bootstrap_library" form element is set to "custom".
    '#states' => array(
      'visible' => array(
        ':input[name="bootstrap_library"]' => array('value' => 'custom'),
      ),
    ),
  );

  return $form;
}

/**
 * Form constructor for Bootstrap library selection form.
 *
 * @return array
 *   Built form for backend check.
 */
function kalatheme_backend_check_form() {
  $form = array();

  if (isset($_SERVER['PANTHEON_ENVIRONMENT'])) {
    $form['pantheon_check'] = array(
      '#weight' => -100,
      '#prefix' => '<div class="alert alert-info">',
      '#markup' => t("You are on Pantheon. <strong>You need to set the connection mode to SFTP</strong> to allow for custom Bootstrap libraries and subtheme generation!"),
      '#suffix' => '</div>',
    );
  }
  else {
    if (kalatheme_backend_check()) {
      $form['backend_check'] = array(
        '#weight' => -100,
        '#prefix' => '<div class="alert alert-success">',
        '#markup' => t("Your webserver is correctly configured to allow for custom Bootstrap libraries and subtheme generation!"),
        '#suffix' => '</div>',
      );
    }
    else {
      $form['backend_check'] = array(
        '#weight' => -100,
        '#prefix' => '<div class="alert alert-danger">',
        '#markup' => t("If you want Kalatheme to be able to use custom Bootstrap libraries or generate subthemes automatically please properly configure your webserver."),
          // @todo add link to docs here
        '#suffix' => '</div>',
      );
    }
  }

  return $form;
}

/**
 * Form constructor for subtheme selection form.
 *
 * @return array
 *   The subtheme form.
 */
function kalatheme_subtheme_form() {
  $form = array();

  if (kalatheme_backend_check()) {
    $form['subtheme'] = array(
      '#type' => 'fieldset',
      '#weight' => -98,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['subtheme']['build_subtheme'] = array(
      '#type' => 'checkbox',
      '#title' => t('Build and enable a custom subtheme'),
      '#default_value' => FALSE,
      '#description' => t('Check here to have kalatheme build you a subtheme with the settings on this form.'),
      '#ajax' => array(
        'callback' => 'kalatheme_ajax_submit_submit',
        'wrapper' => 'kala-submit-change-title',
        'progress' => array('type' => 'none'),
      ),
    );
    // Wrap submit in dix for ajaxification.
    $form['subtheme']['magic'] = array(
      '#type' => 'checkbox',
      '#title' => t('Autoload extra assets'),
      '#default_value' => TRUE,
      '#description' => t('Check here to have kalatheme add extra stuff from a custom third party Bootstrap library into your subtheme info file.'),
      '#states' => array(
        'visible' => array(
          ':input[name="build_subtheme"]' => array('checked' => TRUE),
          ':input[name="bootstrap_library"]' => array('value' => 'custom'),
        ),
      ),
    );
    $form['subtheme']['subtheme_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name your theme'),
      '#description' => t('No special characters. For example: My Theme, Stuff. If you input nothing it will use your sitename.'),
      '#states' => array(
        'visible' => array(
          ':input[name="build_subtheme"]' => array('checked' => TRUE),
        ),
      ),
    );
    $base_themes = kalatheme_get_base_themes();
    if (count($base_themes) > 1) {
      $form['subtheme']['subtheme_base'] = array(
        '#type' => 'select',
        '#title' => t('Subtheme base'),
        '#description' => t('The base theme to use for your theme.'),
        '#options' => $base_themes,
        '#default_value' => 'kalatheme',
        '#states' => array(
          'visible' => array(
            ':input[name="build_subtheme"]' => array('checked' => TRUE),
          ),
        ),
      );
    }
    else {
      $form['subtheme']['subtheme_base'] = array(
        '#type' => 'hidden',
        '#value' => 'kalatheme',
      );
    }
  }

  return $form;
}

/**
 * Handle custom Bootstrap library validation.
 */
function kalatheme_custom_bootstrap_library_validate(&$form, &$form_state) {
  // Validate subtehme name if exists.
  $regex = '/^[a-zA-Z][\da-zA-Z _\']*$/';
  if (!empty($form_state['values']['subtheme_name'])) {
    if (!preg_match($regex, $form_state['values']['subtheme_name'])) {
      form_set_error('form', t('Please enter a valid subtheme name.'));
    }
  }

  // Add the default system_form_submit back into the mix if we aren't building
  // a custom subtheme.
  if (isset($form_state['values']['build_subtheme']) && !$form_state['values']['build_subtheme']) {
    $form['#submit'][] = 'system_theme_settings_submit';
  }
}

/**
 * Handle custom Bootstrap library submission.
 */
function kalatheme_custom_bootstrap_library_submit(&$form, &$form_state) {
  // Summon the powerz of update manager.
  module_load_include("inc", "update", "update.manager");
  $projects = array();

  // Generate subtheme and build its settings var.
  if (isset($form_state['values']['build_subtheme']) && $form_state['values']['build_subtheme']) {
    // Trim things.
    form_state_values_clean($form_state);
    $projects[] = kalatheme_custom_subtheme_build($form_state['values']);
  }
  // Use a custom bootstrap library.
  // This can either be a file upload or a URL although the URL option really
  // only works with drush.
  if ($_FILES['files']['name']['bootstrap_upload'] || !empty($form_state['values']['bootstrap_upload'])) {
    $projects[] = kalatheme_custom_bootstrap_library_build($form_state['values']);
  }
  // Run the batch operation if we have things to move.
  if ($projects) {
    // If the owner of the directory we extracted is the same as the
    // owner of our configuration directory (e.g. sites/default) where we're
    // trying to install the code, there's no need to prompt for FTP/SSH
    // credentials. Instead, we instantiate a FileTransferLocal and invoke
    // update_authorize_run_install() directly.
    if (kalatheme_has_write_access()) {
      $filetransfer = new FileTransferLocal(DRUPAL_ROOT);
      call_user_func_array('kalatheme_run_install', array_merge(array($filetransfer), array($projects)));
    }
    // Otherwise, go through the regular workflow to prompt for FTP/SSH
    // credentials and invoke update_authorize_run_install() indirectly with
    // whatever FileTransfer object authorize.php creates for us.
    else {
      system_authorized_init('kalatheme_run_install', drupal_get_path('module', 'update') . '/update.authorize.inc', $projects, t('Update manager'));
      $form_state['redirect'] = system_authorized_get_url();
    }
  }
}

/**
 * Construct an array of subtheme things.
 *
 * @param array $theme_settings
 *   The current theme settings.
 */
function kalatheme_custom_subtheme_build($theme_settings) {
  $machine_name = kalatheme_get_machine_name($theme_settings['subtheme_name']);
  $directory = kalatheme_extract_directory('theme') . DIRECTORY_SEPARATOR . $machine_name;
  if (kalatheme_write_stuff(kalatheme_custom_subtheme_skeleton($theme_settings), $directory)) {
    // Make sure the Updater registry is loaded.
    drupal_get_updaters();
    try {
      $updater = Updater::factory($directory);
    }
    catch (Exception $e) {
      form_set_error($field, $e->getMessage());
      return;
    }
    $project_real_location = drupal_realpath($directory);
    $arguments = array(
      'project' => $machine_name,
      'updater_name' => get_class($updater),
      'local_url' => $project_real_location,
      'subtheme' => TRUE,
    );

    // Build some magic.
    kalatheme_custom_subtheme_vars($theme_settings);

    // Return the batch.
    return $arguments;
  }
  return NULL;
}

/**
 * Handle custom Bootstrap library submission.
 */
function kalatheme_custom_bootstrap_library_build($theme_settings) {
  // Save the file to tmp.
  $subtheme_name = NULL;
  if (!empty($theme_settings['subtheme_name'])) {
    $subtheme_name = $theme_settings['subtheme_name'];
  }
  $validators = array('file_validate_extensions' => array(archiver_get_extensions()));
  $field = 'bootstrap_upload';
  if (!($finfo = file_save_upload($field, $validators, NULL, FILE_EXISTS_REPLACE))) {
    // Failed to upload the file. file_save_upload() calls form_set_error() on
    // failure.
    return;
  }
  $local_cache = $finfo->uri;

  // Extract file to tmp.
  global $theme_key;
  $project = (!empty($subtheme_name)) ? kalatheme_get_machine_name($subtheme_name) : $theme_key;
  $subtheme_directory = (!empty($subtheme_name)) ? kalatheme_extract_directory('theme') . DIRECTORY_SEPARATOR . kalatheme_get_machine_name($subtheme_name) : NULL;
  $tmp_directory = kalatheme_extract_directory('library-tmp') . DIRECTORY_SEPARATOR . $project;
  try {
    $archive = update_manager_archive_extract($local_cache, $tmp_directory);
  }
  catch (Exception $e) {
    form_set_error($field, $e->getMessage());
    return;
  }

  $files = $archive->listContents();
  // Validate archive.
  if (!$files) {
    form_set_error($field, t('Provided archive contains no files.'));
    return;
  }
  foreach ($files as $file) {
    if (stripos($file, 'bootstrap.min.css') !== FALSE || stripos($file, 'bootstrap.css') !== FALSE) {
      $check_file = $file;
      break;
    }
  }
  if (!file_exists($tmp_directory . DIRECTORY_SEPARATOR . $check_file)) {
    form_set_error($field, t('Provided archive contains no files.'));
    return;
  }

  // Go through the library and do some magic.
  $lib_dir = kalatheme_extract_directory('library') . DIRECTORY_SEPARATOR . $project . '_bootstrap';
  kalatheme_custom_bootstrap_library_integrate($files, $project, $tmp_directory, $lib_dir, $subtheme_directory, $theme_settings['magic']);

  // Make sure the Updater registry is loaded.
  require_once dirname(__FILE__) . '/../kalatheme.updater.inc';
  drupal_get_updaters();
  try {
    $updater = Updater::factory($lib_dir);
  }
  catch (Exception $e) {
    form_set_error($field, $e->getMessage());
    return;
  }
  $project_real_location = drupal_realpath($lib_dir);
  $arguments = array(
    'project' => $project,
    'updater_name' => get_class($updater),
    'local_url' => $project_real_location,
    'subtheme' => FALSE,
  );

  return $arguments;
}

/**
 * Set the subtheme vars.
 */
function kalatheme_custom_subtheme_vars($theme_settings) {
  // Get the machine name.
  $machine_name = kalatheme_get_machine_name($theme_settings['subtheme_name']);
  // Remove unneded Vars.
  unset($theme_settings['var']);
  unset($theme_settings['build_subtheme']);
  unset($theme_settings['bootstrap_upload']);
  unset($theme_settings['subtheme_name']);
  unset($theme_settings['subtheme_base']);

  // If the user uploaded a new logo or favicon, save it to a permanent location
  // and use it in place of the default theme-provided file.
  if (isset($theme_settings['logo_upload']) && $file = $theme_settings['logo_upload']) {
    unset($theme_settings['logo_upload']);
    $filename = file_unmanaged_copy($file->uri);
    $theme_settings['default_logo'] = 0;
    $theme_settings['logo_path'] = $filename;
    $theme_settings['toggle_logo'] = 1;
  }
  if (isset($theme_settings['favicon_upload']) && $file = $theme_settings['favicon_upload']) {
    unset($theme_settings['favicon_upload']);
    $filename = file_unmanaged_copy($file->uri);
    $theme_settings['default_favicon'] = 0;
    $theme_settings['favicon_path'] = $filename;
    $theme_settings['toggle_favicon'] = 1;
  }

  // If the user entered a path relative to the system files directory for
  // a logo or favicon, store a public:// URI so the theme system can handle it.
  if (!empty($theme_settings['logo_path'])) {
    $theme_settings['logo_path'] = _system_theme_settings_validate_path($theme_settings['logo_path']);
  }
  if (!empty($theme_settings['favicon_path'])) {
    $theme_settings['favicon_path'] = _system_theme_settings_validate_path($theme_settings['favicon_path']);
  }

  if (empty($theme_settings['default_favicon']) && !empty($theme_settings['favicon_path'])) {
    $theme_settings['favicon_mimetype'] = file_get_mimetype($theme_settings['favicon_path']);
  }
  // Save the new stuff.
  $key = 'theme_' . $machine_name . '_settings';
  variable_set($key, $theme_settings);
}

/**
 * Parse custom bootstrap library.
 */
function kalatheme_custom_bootstrap_library_integrate($files, $project, $tmp_directory, $lib_dir, $theme_dir, $magic) {
  // Get rid of all the bullsh*t.
  foreach ($files as $key => $value) {
    if (kalatheme_strip_types($value) || kalatheme_strip_files($value)) {
      unset($files[$key]);
    }
  }
  // Pull from the archive.
  foreach ($files as $key => $value) {
    $filename = basename($value);
    $is_bootstrap = in_array($filename, kalatheme_get_bootstrap_files());
    $dir = kalatheme_get_asset_class($filename);
    $old_file = $tmp_directory . DIRECTORY_SEPARATOR . $value;
    $new_base_dir = ($is_bootstrap) ? $lib_dir : $theme_dir;
    $new_file = (!empty($new_base_dir)) ? $new_base_dir . DIRECTORY_SEPARATOR . $dir . DIRECTORY_SEPARATOR . $filename : NULL;
    // If something there do something.
    if (!empty($new_file) && (is_dir(dirname($new_file)) || mkdir(dirname($new_file), 0777, TRUE))) {
      // Move the file.
      copy($old_file, $new_file);
      // Add some none bootstrap stuff to the info file.
      if ($magic) {
        if (!$is_bootstrap && in_array($dir, array('css', 'js'))) {
          $info_file = $theme_dir . DIRECTORY_SEPARATOR . $project . '.info';
          $contents = file_get_contents($info_file);
          switch ($dir) {
            case 'css':
              $contents .= "stylesheets[all][]";
              break;

            case 'js':
              $contents .= "scripts[]";
              break;

          }
          $contents .= " = " . $dir . DIRECTORY_SEPARATOR . $filename . "\n";
          file_put_contents($info_file, $contents);
        }
      }
    }
  }
}

/**
 * The batch builder and processor for moving files to drupal.
 *
 * Taken from update_authorize_run_update.
 * Builds a batch and process it for installing modules from the templocation.
 */
function kalatheme_run_install($filetransfer, $projects) {
  $operations = array();
  foreach ($projects as $project_info) {
    $operations[] = array(
      'kalatheme_authorize_batch_copy_project',
      array(
        $project_info['project'],
        $project_info['updater_name'],
        $project_info['local_url'],
        $project_info['subtheme'],
        $filetransfer,
      ),
    );
  }

  // We have to do this because drupal_get_path doesnt really work
  // in an install profile when this happens. We need a relative path
  // but _batch_process prepends DRUPAL_ROOT so we need to strip that
  // out here as well.
  $file = dirname(__FILE__) . '/../kalatheme.authorize.inc';
  $file = str_replace(DRUPAL_ROOT . DIRECTORY_SEPARATOR, '', $file);

  // Set the batch up!
  $batch = array(
    'title' => t('Installing Kalatheme'),
    'init_message' => t('Preparing to Kalatheme'),
    'operations' => $operations,
    // @todo Use a different finished callback for different messages?
    'finished' => 'kalatheme_authorize_install_batch_finished',
    // @todo this doesnt seem to point to the correct location?
    'file' => $file,
  );
  batch_set($batch);
}

/**
 * Return basic subtheme skeleton to be written.
 */
function kalatheme_custom_subtheme_skeleton($theme_settings) {
  $subtheme = array();
  $machine_name = kalatheme_get_machine_name($theme_settings['subtheme_name']);
  // Basic Info File.
  $output  = "name = " . $theme_settings['subtheme_name'] . " \n";
  $output .= "description = A Kalatheme subtheme powered by Twitter Bootstrap & Science. \n";
  $output .= "base theme = " . $theme_settings['subtheme_base'] . " \n";
  $output .= "core = 7.x \n";
  $output .= "\n";
  $output .= "regions[content] = Content \n";
  $output .= "\n";
  $output .= "stylesheets[all][] = css/main.css \n";
  $output .= "\n";
  $subtheme[DIRECTORY_SEPARATOR . $machine_name . '.info'] = $output;
  // Basic CSS File.
  $output  = "/*! \n";
  $output .= " * Kalatheme (http://drupal.org/project/kalatheme) \n";
  $output .= " */ \n";
  $subtheme[DIRECTORY_SEPARATOR . 'css' . DIRECTORY_SEPARATOR . 'main.css'] = $output;
  // Basic Template File.
  $output  = "<?php \n";
  $output .= "/** \n";
  $output .= " * @file \n";
  $output .= " * Primary pre/preprocess functions and alterations.\n";
  $output .= " */ \n";
  $subtheme[DIRECTORY_SEPARATOR . 'template.php'] = $output;

  return $subtheme;
}

/**
 * Ajax callback that returns HTML to the setup form.
 *
 * Allows the user can see a preview of the bootstrap library they selected.
 *
 * @param array $form
 *   Form
 * @param array $form_state
 *   Form state
 *
 * @return array
 *   Updated form state to be merged back in
 */
function kalatheme_ajax_bootstrap_preview_submit($form, &$form_state) {
  $theme = $form_state['values']['bootstrap_library'];
  if ($theme == 'default' || $theme == 'custom' || $theme == 'none') {
    $form['bootstrap']['bootstrap_preview']['#markup'] = '';
  }
  else {
    $bootswatch = kalatheme_get_bootswatch_theme($theme);
    $form['bootstrap']['bootstrap_preview']['#markup'] = t('<h2>@text</h2><img src="@preview">', array('@preview' => $bootswatch->thumbnail, '@text' => $bootswatch->description));
  }
  return $form['bootstrap']['bootstrap_preview'];
}

/**
 * Ajax callback that returns HTML to the submit button.
 *
 * @param array $form
 *   Form.
 * @param array $form_state
 *   Form state.
 *
 * @return array
 *   Updated form state to be merged back in.
 */
function kalatheme_ajax_submit_submit($form, &$form_state) {
  $subtheme = $form_state['values']['build_subtheme'];
  $form['actions']['submit']['#value'] = ($subtheme) ? t('Build and enable subtheme') : t('Save configuration');
  return $form['actions']['submit'];
}

/**
 * Wrapper for _update_manager_extract_directory().
 *
 * Since libraries and themes can live in the same location, we need to
 * namespace the types so they don't collide like the colorbox module.
 */
function kalatheme_extract_directory($type = '') {
  $directory = _update_manager_extract_directory();
  if ($type) {
    $directory .= DIRECTORY_SEPARATOR . $type;
    if (!file_exists($directory)) {
      mkdir($directory);
    }
  }
  return $directory;
}

/**
 * Returns a list of Kalatheme base themes.
 */
function kalatheme_get_base_themes() {
  $themes = list_themes();

  $base_themes = array('kalatheme' => 'Kalatheme');
  if (isset($themes['kalatheme']->sub_themes)) {
    $base_themes += $themes['kalatheme']->sub_themes;
  }

  return $base_themes;
}

/**
 * Translate human readable to machine name.
 */
function kalatheme_get_machine_name($name) {
  $machine_name = strtolower($name);
  $machine_name = preg_replace('@[^a-z0-9_]+@', '_', $machine_name);

  return $machine_name;
}

/**
 * Translate a subtheme array into writable files.
 *
 * @param array $subtheme
 *   Array of stuff to write.
 * @param string $directory
 *   The directory to store the subtheme.
 *
 * @return bool
 *   TRUE is for success, FALSE is for fail.
 */
function kalatheme_write_stuff($subtheme, $directory) {
  foreach ($subtheme as $key => $value) {
    $filename = $directory . $key;
    $dir = dirname($filename);
    if (is_dir($dir) || mkdir($dir, 0777, TRUE)) {
      if (!file_put_contents($filename, $value)) {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Prune custom library, removing types we don't want.
 */
function kalatheme_strip_types($file) {
  $wanted_types = array(
    '.css',
    '.js',
    '.png',
    '.jpg',
    '.woff',
    '.ttf',
    '.svg',
    '.eot',
    '.json',
  );
  foreach ($wanted_types as $type) {
    if (stripos($file, $type) !== FALSE) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Prune custom library, removing files we don't want.
 */
function kalatheme_strip_files($file) {
  // @todo might want to update this over time?
  $unwanted_files = array('jquery.js', 'jquery.min.js', '._');
  foreach ($unwanted_files as $type) {
    if (stripos($file, $type) !== FALSE) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Return the correct asset class for a file.
 */
function kalatheme_get_asset_class($file) {
  $asset_classes = array(
    'img' => array('.png', '.jpg', '.gif'),
    'fonts' => array('.svg', '.woff', '.ttf', '.eot'),
    'js' => array('.js'),
    'css' => array('.css'),
  );
  // Return the correct asset class.
  foreach ($asset_classes as $class => $types) {
    foreach ($types as $type) {
      if (stripos($file, $type) !== FALSE) {
        return $class;
      }
    }
  }
}

/**
 * Return the files that make up a standard bootstrap library.
 */
function kalatheme_get_bootstrap_files() {
  return array(
    'bootstrap.css',
    'bootstrap.min.css',
    'bootstrap.js',
    'bootstrap.min.js',
    'glyphicons-halflings-regular.woff',
    'glyphicons-halflings-regular.ttf',
    'glyphicons-halflings-regular.svg',
    'glyphicons-halflings-regular.eot',
  );
}
