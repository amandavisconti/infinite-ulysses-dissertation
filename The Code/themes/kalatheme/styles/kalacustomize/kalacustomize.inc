<?php

/**
 * @file
 * Definition of the 'kalacustomize' panels style plugin.
 */

$plugin = array(
  'title' => t('Kalacustomize'),
  'description' => t('Customize elements, visbility and classes with Twitter Bootstrap'),
  'render region' => 'kalatheme_kalacustomize_render_region',
  'render pane' => 'kalatheme_kalacustomize_render_pane',
  'settings form' => 'kalatheme_kalacustomize_pane_settings_form',
  'pane settings form' => 'kalatheme_kalacustomize_pane_settings_form',
  'weight' => 100,
);

/**
 * Theme fuction for the region style.
 */
function theme_kalatheme_kalacustomize_render_region($vars) {
  $content = $vars['panes'];
  $settings = $vars['settings'];

  $settings['attributes'] = '';
  $settings['attributes'] .= isset($settings['devices']) ? _kalatheme_get_styles($settings['devices']) : '';
  $settings['attributes'] .= isset($settings['pane_style']) ? _kalatheme_get_styles($settings['pane_style']) : '';

  // Theme.
  if (!empty($settings['theme']) && $settings['theme']) {
    $output = theme('panels_default_style_render_region', $vars);
  }
  else {
    $output = implode(PHP_EOL, $content);
  }

  // Wrap.
  if (!empty($output)) {
    $content_wrapper = _kalatheme_wrapper_wrap($settings);
    $output = $content_wrapper['prefix'] . $output . $content_wrapper['suffix'];
  }

  return $output;
}

/**
 * Theme function for the pane style.
 */
function theme_kalatheme_kalacustomize_render_pane($vars) {
  $content = &$vars['content'];
  $settings = $vars['settings'];

  // This is needed to prevent throwing an error in php 5.5+
  if (!isset($content->css_class)) {
    $content->css_class = '';
  }

  if (isset($settings['devices'])) {
    $content->css_class .= _kalatheme_get_styles($settings['devices']);
  }
  if (isset($settings['pane_style'])) {
    $content->css_class .= _kalatheme_get_styles($settings['pane_style']);
  }
  $output = theme('panels_pane', $vars);

  return $output;
}

/**
 * Options for the Panels style plugin to help style panes.
 */
function kalatheme_kalacustomize_pane_settings_form($style_settings) {

  $settings_exposed = 0;
  $default_theme = variable_get('theme_default', NULL);

  if (theme_get_setting('responsive_toggle', $default_theme)) {
    $mobile_options = array(
      'hidden-xs' => 'Phone',
      'hidden-sm' => 'Tablet',
      'hidden-md' => 'Desktop',
      'hidden-lg' => 'Large Desktop',
    );
    $form['devices'] = array(
      '#prefix' => '<div class="well">',
      '#suffix' => '</div>',
      '#type' => 'checkboxes',
      '#options' => $mobile_options,
      '#title' => t('Hide this pane on the following devices.'),
      '#default_value' => $style_settings['devices'],
    );
    $settings_exposed++;
  }

  if (theme_get_setting('pane_styles_toggle', $default_theme)) {
    $pane_style_default_value = isset($style_settings['pane_style']) ? $style_settings['pane_style'] : '';
    $pane_options = array(
      'pull-left' => 'Float Left',
      'pull-right' => 'Float Right',
      'clearfix' => 'Clearfix',
      'jumbotron' => 'Jumbotron',
      'well' => 'Well',
    );
    if (theme_get_setting('pane_classes', $default_theme)) {
      $pane_options += theme_get_setting('pane_classes', $default_theme);
    }

    $form['pane_style'] = array(
      '#title' => t('Styles'),
      '#prefix' => '<div class="well">',
      '#suffix' => '</div>',
      '#type' => 'checkboxes',
      '#options' => $pane_options,
      '#default_value' => $pane_style_default_value,
    );
    $settings_exposed++;
  }

  // If we don't have any enabled options, let the user know.
  if ($settings_exposed == 0) {
    _kalatheme_warn_no_style_options();
  }

  return $form;
}

/**
 * Get pane/region styles depending on chosen classes.
 *
 * @return array
 *   Array with prefix and suffix keys.
 */
function _kalatheme_get_styles($styles = array()) {
  if (isset($styles)) {
    foreach ($styles as $key => $class) {
      if ($class === 0) {
        unset($styles[$key]);
      }
    }
    return $styles ? ' ' . implode(' ', $styles) : '';
  }
}

/**
 * Create a region wrapper from provided attributes.
 *
 * @return array
 *   Array with prefix and suffix keys.
 */
function _kalatheme_wrapper_wrap($item = array()) {
  $wrap['prefix'] = NULL;
  $wrap['suffix'] = NULL;
  $wrap['attributes'] = NULL;

  if (!empty($item['attributes'])) {
    $value = $item['attributes'];
    $wrap['attributes'] .= " class=\"$value\"";
  }

  $wrap['prefix'] = '<div ' . $wrap['attributes'] . '>';
  $wrap['suffix'] = '</div>';

  if (!empty($item['prefix'])) {
    $wrap['prefix'] = $item['prefix'];
  }
  if (!empty($item['suffix'])) {
    $wrap['suffix'] = $item['suffix'];

  }

  $wrap['prefix'] = PHP_EOL . $wrap['prefix'];
  $wrap['suffix'] = $wrap['suffix'] . PHP_EOL;

  return $wrap;
}

/**
 * Adds a warning status message.
 *
 * Letting the user know that no customization
 * options have been enabled from the theme settings page.
 */
function _kalatheme_warn_no_style_options() {
  $settings_link_title = variable_get('theme_default', 'kalatheme') . ' settings page';
  $settings_link = l($settings_link_title, 'admin/appearance/settings/' . variable_get('theme_default', 'kalatheme'));
  drupal_set_message(t("You haven't enabled any customizations! Please check
    out the @settings_link to see what's available.", array('@settings_link' => $settings_link)), 'warning');
}
