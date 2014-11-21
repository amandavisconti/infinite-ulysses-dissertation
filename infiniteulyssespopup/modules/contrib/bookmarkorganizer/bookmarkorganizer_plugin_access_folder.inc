<?php

/**
 * @file
 * Contains the folder access plugin.
 */

/**
 * A plugin to handle access control based on custom access callback.
 */
class bookmarkorganizer_plugin_access_folder extends views_plugin_access {

  /**
   * Initialize the plugin.
   *
   * @param $view
   *   The view object.
   * @param $display
   *   The display handler.
   */
  function init(&$view, &$display) {
    $this->view = &$view;    $this->display = &$display;    $this->options = array();
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {   }

  /**
   * Provide the default form form for validating options
   */
  function options_validate(&$form, &$form_state) {  }

  /**
   * Provide the default form form for submitting options
   */
  function options_submit(&$form, &$form_state) {  }

  /**
   * Return a string to display as the clickable title for the
   * access control.
   */
  function summary_title() {
    return t('Bookmark Organizer Folder Access');
  }

  /**
   * Determine if the current user has access or not.
   */
  function access($account) {
    return bookmarkorganizer_folder_access(array($this->display), $account);
  }

  /**
   * Determine the access callback and arguments.                                                                     *
   * This information will be embedded in the menu in order to reduce
   * performance hits during menu item access testing, which happens
   * a lot.
   *
   * @return an array; the first item should be the function to call,
   *   and the second item should be an array of arguments. The first
   *   item may also be TRUE (bool only) which will indicate no
   *   access control.)
   */
  function get_access_callback() {
    return array('bookmarkorganizer_folder_access', array($this->display));
  }

}