<?php
/**
 * @file
 * Contains kalatheme updater.
 */

class KalathemeUpdater extends Updater implements DrupalUpdaterInterface {
  /**
   * Returns install directory.
   */
  public function getInstallDirectory() {
    return DRUPAL_ROOT . "/sites/all/libraries";
  }

  /**
   * Returns if the library is installed.
   */
  public function isInstalled() {
    return is_dir(DRUPAL_ROOT . "/sites/all/libraries/{$this->name}");
  }

  /**
   * Returns if it can update directory.
   */
  public static function canUpdateDirectory($directory) {
    if (!file_scan_directory($directory, '/.*\.info$/')) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Returns if it can update.
   */
  public static function canUpdate($project_name) {
    return is_dir(DRUPAL_ROOT . "/sites/all/libraries/$project_name");
  }

  /**
   * Constructor function.
   */
  public function __construct($source) {
    $this->source = $source;
    $this->name = self::getProjectName($source);
    $this->title = self::getProjectName($source);
  }

}
