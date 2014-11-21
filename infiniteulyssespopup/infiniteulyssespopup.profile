<?php
/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Allows the profile to alter the site configuration form.
 */
if (!function_exists("system_form_install_configure_form_alter")) {
  function system_form_install_configure_form_alter(&$form, $form_state) {
      
    // Do not let the profile setup time out.
  drupal_set_time_limit(0);

  // Pre-populate site name and super-admin username.
 $form['site_information']['site_name']['#default_value'] = 'Infinite Ulysses Pop-Up';
  $form['site_information']['site_mail']['#default_value'] = 'ADDYOUREMAILADDRESS';
  $form['admin_account']['account']['name']['#default_value'] = 'ADDYOURUID1NAME';
  $form['admin_account']['account']['mail']['#default_value'] = 'ADDYOUREMAILADDRESS';
  }
}

/**
 * Implements hook_form_alter().
 *
 * Select the current install profile by default.
 */
if (!function_exists("system_form_install_select_profile_form_alter")) {
  function system_form_install_select_profile_form_alter(&$form, $form_state) {
    foreach ($form['profile'] as $key => $element) {
      $form['profile'][$key]['#value'] = 'infiniteulyssespopup';
    }
  }
}