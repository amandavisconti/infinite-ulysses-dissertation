<?php

/**
 * @file
 * Kalatheme's theme functions for icons
 */

require_once dirname(__FILE__) . '/glyphicons.inc';

/**
 * Theme function for icon fonts.
 *
 * @param string $name
 *   Common name to icon bundle.
 * @param string $bundle
 *   The bundle to use.
 * @param array $attr
 *   The attributes array to apply
 * @param bool $resolve_prefix
 *   Resolve the class prefix.
 *
 * @return string
 *   Themed HTML for the icon to use.
 */
function _kalatheme_font_icon($name, $bundle = NULL, $attr = array(), $resolve_prefix = TRUE) {
  if ($bundle === NULL) {
    $bundle = theme_get_setting('icon_font_library');
  }
  // Find the icon prefix.
  if ($resolve_prefix) {
    switch ($bundle) {
      case('font_awesome'):
        $name = 'fa-' . $name;
        break;

      case('bootstrap_glyphicons'):
        $name = 'glyphicon-' . $name;
        break;
    }
  }

  $output = NULL;
  if (module_exists('icon')) {
    $output = theme('icon', array(
      'bundle' => $bundle,
      'icon' => $name,
      'attributes' => $attr,
    ));
  }
  if ($output === NULL) {
    $attr = array();
    $attr += array('aria-hidden' => 'true');
    $attr['class'] = array();

    if ($bundle === 'font_awesome') {
      $attr['class'][] = 'fa';
      $attr['class'][] = 'fa-' . $name;
    }
    elseif ($bundle === 'bootstrap_glyphicons') {
      $attr['class'][] = 'glyphicon';
      $attr['class'][] = $name;
    }

    $output = '<span ' . drupal_attributes($attr) . '></span>';
  }
  return $output;
}


/**
 * Implements hook_icon_providers().
 */
function kalatheme_icon_providers() {
  $providers['font_awesome'] = array(
    'title' => t('Font Awesome'),
    'url' => 'http://fontawesome.io',
    'default bundle' => array(
      'render' => 'html_tag',
      'settings' => kalatheme_icon_default_settings(),
    ),
  );

  $providers['bootstrap_glyphicons'] = array(
    'title' => t('Bootstrap Glyphicons'),
    'url' => 'http://getbootstrap.com/components/#glyphicons',
    'default bundle' => array(
      'render' => 'html_tag',
      'settings' => kalatheme_icon_default_settings(),
    ),
  );
  return $providers;
}

/**
 * Provides the default settings for icon bundles.
 */
function kalatheme_icon_default_settings() {
  return array(
    'tag' => 'span',
  );
}


/**
 * Implements hook_icon_bundle_configure().
 */
function kalatheme_icon_bundle_configure(&$settings, &$form_state, &$complete_form) {
  $bundle = $form_state['bundle'];
  if ($bundle['provider'] === 'font_awesome' || 'bootstrap_glyphicons') {
    $settings['tag'] = array(
      '#type' => 'select',
      '#title' => t('HTML Markup'),
      '#description' => t('Choose the HTML markup tag that Font Awesome icons should be created with.
        Typically, this is an %span tag, however it can be changed to suite the theme requirements.', array('%span' => '<span/>')),
      '#options' => drupal_map_assoc(array('i', 'span', 'div')),
      '#default_value' => isset($bundle['settings']['tag']) ? $bundle['settings']['tag'] : 'span',
    );
  }
}


/**
 * Return HTML tag emement with fontawesome icon classed.
 *
 * @param array $variables
 *   Associative array of variables.
 *
 * @return string
 *   Themed HTML string.
 */
function kalatheme_icon_html_tag($variables) {
  $output = '';
  $bundle = $variables['bundle'];
  $icon = $variables['icon'];
  $output = theme('html_tag', array(
    'element' => array(
      '#tag' => isset($bundle['settings']['tag']) ? $bundle['settings']['tag'] : 'span',
      '#attributes' => $variables['attributes'],
      '#value' => '',
    ),
  ));
  return $output;
}

/**
 * Implements hook_preprocess_icon().
 */
function kalatheme_preprocess_icon(&$variables) {
  $icon = $variables['icon'];
  if (!empty($variables['bundle']['provider'])) {
    $classes = array();
    $classes[] = drupal_html_class($icon);
    switch ($variables['bundle']['provider']) {
      case 'font_awesome':
        $classes[] = 'fa';
        $classes[] = 'fa-' . $variables['icon'];
        break;

      case 'bootstrap_glyphicons':
        $classes[] = 'glyphicon';
        $classes[] = $variables['icon'];
        break;
    }
    if (isset($variables['attributes']['class'])) {
      $variables['attributes']['class'] += $classes;
    }
    else {
      $variables['attributes']['class'] = $classes;
    }
  }
}
/**
 * Implements hook_icon_bundles().
 */
function kalatheme_icon_bundles() {
  $bundles = array();
  $bundles['font_awesome'] = array(
    'render' => 'html_tag',
    'provider' => 'font_awesome',
    'title' => t('Font Awsome'),
    'icons' => _kalatheme_fontawesome_icons(),
  );

  $bundles['bootstrap_glyphicons'] = array(
    'render' => 'html_tag',
    'provider' => 'bootstrap_glyphicons',
    'title' => t('Bootstrap Glyphicons'),
    'icons' => _kalatheme_glyphicons_icons(),
  );
  return $bundles;
}


/**
 * Define render hook information.
 *
 * @see hook_icon_render_hooks()
 */
function kalatheme_icon_render_hooks() {
  $hooks['html_tag'] = array();
  return $hooks;
}

/**
 * Helper function to return icons for font awesome.
 */
function _kalatheme_fontawesome_icons() {
  $icons = array();
  // Load the font awesome json file.
  if (file_exists(__DIR__ . '/fontawesome.json')) {
    $file = file_get_contents(__DIR__ . '/fontawesome.json');
    $raw_icons = json_decode($file, TRUE);
    foreach ($raw_icons['icons'] as $delta => $icon) {
      $icons[$icon['id']] = $icon['name'];
    }
  }
  return $icons;
}
