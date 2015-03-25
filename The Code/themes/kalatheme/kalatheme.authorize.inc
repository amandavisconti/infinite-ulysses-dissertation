<?php
/**
 * @file
 * Kalatheme's subtheme config form stuff
 */

/**
 * Batch callback: Copies project to its proper place when authorized to do so.
 *
 * @param string $project
 *   The canonical short name of the project being installed.
 * @param string $updater_name
 *   The name of the Updater class to use for installing this project.
 * @param string $local_url
 *   The URL to the locally installed temp directory where the project has
 *   already been downloaded and extracted into.
 * @param FileTransfer $filetransfer
 *   The FileTransfer object to use for performing this operation.
 * @param array $context
 *   Reference to an array used for Batch API storage.
 */
function kalatheme_authorize_batch_copy_project($project, $updater_name, $local_url, $subtheme, $filetransfer, &$context) {
  module_load_include('inc', 'update', 'update.authorize');
  // Initialize some variables in the Batch API $context array.
  if (!isset($context['results']['log'])) {
    $context['results']['log'] = array();
  }
  if (!isset($context['results']['log'][$project])) {
    $context['results']['log'][$project] = array();
  }
  if (!isset($context['results']['tasks'])) {
    $context['results']['tasks'] = array();
  }
  // Add some information about what to do when we finish.
  if ($subtheme) {
    $context['results']['tasks']['log']['subtheme'] = $project;
  }

  // The batch API uses a session, and since all the arguments are serialized
  // and unserialized between requests, although the FileTransfer object itself
  // will be reconstructed, the connection pointer itself will be lost. However,
  // the FileTransfer object will still have the connection variable, even
  // though the connection itself is now gone. So, although it's ugly, we have
  // to unset the connection variable at this point so that the FileTransfer
  // object will re-initiate the actual connection.
  unset($filetransfer->connection);

  if (!empty($context['results']['log'][$project]['#abort'])) {
    $context['finished'] = 1;
    return;
  }
  if ($updater_name == 'KalathemeUpdater') {
    // This is where we check to see if libraries dir exists or not.
    if (!is_dir(DRUPAL_ROOT . "/sites/all/libraries")) {
      $filetransfer->createDirectory(DRUPAL_ROOT . "/sites/all/libraries");
    }
    require_once dirname(__FILE__) . '/kalatheme.updater.inc';
  }
  $updater = new $updater_name($local_url);

  try {
    if ($updater->isInstalled()) {
      // This is an update.
      $tasks = $updater->update($filetransfer);
    }
    else {
      $tasks = $updater->install($filetransfer);
    }
  }
  catch (UpdaterException $e) {
    _update_batch_create_message($context['results']['log'][$project], t('Error installing / updating'), FALSE);
    _update_batch_create_message($context['results']['log'][$project], $e->getMessage(), FALSE);
    $context['results']['log'][$project]['#abort'] = TRUE;
    return;
  }

  _update_batch_create_message($context['results']['log'][$project], t('Installed %project_name successfully', array('%project_name' => $project)));
  if (!empty($tasks)) {
    $context['results']['tasks'] += $tasks;
  }

  // This particular operation is now complete, even though the batch might
  // have other operations to perform.
  $context['finished'] = 1;
}

/**
 * Batch callback: Performs actions when the authorized install batch is done.
 *
 * This processes the results and stashes them into SESSION such that
 * authorize.php will render a report. Also responsible for putting the site
 * back online after a successful install if necessary.
 *
 * @param bool $success
 *   TRUE if the batch operation was a success; FALSE if there were errors.
 * @param array $results
 *   An associative array of results from the batch operation.
 */
function kalatheme_authorize_install_batch_finished($success, $results) {
  foreach ($results['log'] as $project => $messages) {
    if (!empty($messages['#abort'])) {
      $success = FALSE;
    }
  }
  $offline = variable_get('maintenance_mode', FALSE);
  if ($success) {
    // Take the site out of maintenance mode if it was previously that way.
    if ($offline && isset($_SESSION['maintenance_mode']) && $_SESSION['maintenance_mode'] == FALSE) {
      variable_set('maintenance_mode', FALSE);
      $page_message = array(
        'message' => t('Installation was completed successfully. Your site has been taken out of maintenance mode.'),
        'type' => 'status',
      );
    }
    else {
      $page_message = array(
        'message' => t('Installation was completed successfully.'),
        'type' => 'status',
      );
    }
  }
  elseif (!$success && !$offline) {
    $page_message = array(
      'message' => t('Installation failed! See the log below for more information.'),
      'type' => 'error',
    );
  }
  else {
    $page_message = array(
      'message' => t('Installation failed! See the log below for more information. Your site is still in maintenance mode.'),
      'type' => 'error',
    );
  }

  // Unset the variable since it is no longer needed.
  unset($_SESSION['maintenance_mode']);

  // Set all these values into the SESSION so authorize.php can display them.
  $_SESSION['authorize_results']['success'] = $success;
  $_SESSION['authorize_results']['page_message'] = $page_message;
  $_SESSION['authorize_results']['messages'] = $results['log'];
  $_SESSION['authorize_results']['tasks'] = $results['tasks'];
  $_SESSION['authorize_operation']['page_title'] = t('Update manager');

  // Enable and redirect if required.
  if (isset($results['tasks']['log']['subtheme'])) {
    // Rebuild theme registry stuff.
    system_rebuild_theme_data();
    drupal_theme_rebuild();
    // Enable new theme.
    theme_enable(array($results['tasks']['log']['subtheme']));
    variable_set('theme_default', $results['tasks']['log']['subtheme']);
    // Refresh everythging and set a message.
    // @todo: make this abstract in the future, piped in from $context?
    drupal_set_message(t('<i class="fa fa-thumbs-up fa-2x">Your new subtheme is enabled! Looking good in the neighborhood!!</i>'), 'status', FALSE);

    // Redirect to new settings page but only if not an install profile.
    if (!drupal_installation_attempted()) {
      drupal_goto('admin/appearance/settings/' . $results['tasks']['log']['subtheme']);
    }
  }
  // We need to do the big dump.
  // @todo: should figure out the actual caches we need to clear
  // flush error messages so we dont get annoying bootstrap lib error.
  if ($success) {
    // drupal_get_message is not loaded so we directly remove errors.
    unset($_SESSION['messages']['error']);
  }
  drupal_flush_all_caches();
}
