<?php

/**
 * Devel admin block.
 */
function admin_block_devel() {
  if (module_exists('devel') && (user_access('access devel information') || user_access('switch users') || user_access('execute php code'))) {
    drupal_add_css(drupal_get_path('module', 'admin') . '/includes/admin.devel.css');
    drupal_add_js(drupal_get_path('module', 'admin') . '/includes/admin.devel.js');
    return array('subject' => t('Devel'), 'content' => drupal_get_form('admin_block_devel_form'));
  }
}

/**
 * Devel admin block form.
 */
function admin_block_devel_form($form, $form_state) {
  $panes = array();
  if (user_access('access devel information')) {
    $panes['performance'] = admin_devel_performance_form();
  }
  if (user_access('switch users')) {
    $panes['switch_user'] = array(
      '#title' => t('Switch user'),
      'list' => array('#markup' => theme('links', array(
        'links' => devel_switch_user_list(),
        'attributes' => array('class' => 'links clearfix'),
      ))),
      'user' => devel_switch_user_form(),
    );
    $panes['switch_user']['user']['#submit'] = array('devel_switch_user_form_submit');
  }
  if (user_access('execute php code')) {
    $panes['execute'] = devel_execute_form() + array('#title' => t('Execute PHP'));
    $panes['execute']['op']['#submit'] = array('devel_execute_form_submit');
  }
  if (user_access('access devel information')) {
    $panes['menu'] = menu_tree_output(menu_tree_all_data('devel'));
    $panes['menu']['#title'] = t('Other tools');
  }
  if (!empty($panes)) {
    $panes['#type'] = 'admin_panes';
    return array('panes' => $panes);
  }
  return array();
}

/**
 * Performance "form". Actually renders placeholders that are populated post-exit via javascript.
 */
function admin_devel_performance_form() {
  $items = array();
  if (variable_get('dev_timer', 0) && !variable_get('devel_query_display', FALSE)) {
    $items[] = array(
      'data' => "<div class='dev-info'></div>",
      'class' => array('dev-timer'),
    );
  }
  if (variable_get('dev_mem', FALSE) && function_exists('memory_get_usage')) {
    $items[] = array(
      'data' => "<div class='dev-info'></div>",
      'class' => array('dev-memory-usage'),
    );
  }
  if (variable_get('devel_query_display', FALSE)) {
    $show = "<input type='button' class='form-submit dev-querylog-show' value='" . t('Show querylog') . "'/>";
    $hide = "<input type='button' class='form-submit dev-querylog-hide' value='" . t('Hide querylog') . "'/>";
    $items[] = array(
      'data' => "<div class='dev-info'></div>{$show}{$hide}",
      'class' => array('dev-query'),
    );
  }
  if ($items) {
    return array(
      '#title' => t('Performance'),
      '#markup' => theme('item_list', array('items' => $items)),
    );
  }
}
