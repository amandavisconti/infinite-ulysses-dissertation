<?php

/**
 * @file
 * Contains the grid style plugin.
 */

/**
 * Style plugin to render each item in a grid cell.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_grid_bootstrap extends views_plugin_style {

  /**
   * Set default options.
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['columns'] = array('default' => '4');
    $options['alignment'] = array('default' => 'horizontal');
    $options['fill_single_line'] = array('default' => TRUE, 'bool' => TRUE);
    $options['summary'] = array('default' => '');
    $options['caption'] = array('default' => '');

    return $options;
  }

  /**
   * Render the given style.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['responsive_toggle'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use different number of columns on different devices'),
      '#default_value' => $this->options['responsive_toggle'],
      '#description' => t('Check here if you want to set the number of columns to use for each device.'),
    );
    $form['columns_lg'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of columns on large displays'),
      '#default_value' => $this->options['columns_lg'],
      '#required' => FALSE,
      '#element_validate' => array('views_element_validate_integer'),
      '#states' => array(
        'visible' => array(
          ':input[name="style_options[responsive_toggle]"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['columns'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of columns'),
      '#default_value' => $this->options['columns'],
      '#required' => TRUE,
      '#element_validate' => array('views_element_validate_integer'),
    );
    $form['columns_sm'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of columns on tablets'),
      '#default_value' => $this->options['columns_sm'],
      '#required' => FALSE,
      '#element_validate' => array('views_element_validate_integer'),
      '#states' => array(
        'visible' => array(
          ':input[name="style_options[responsive_toggle]"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['columns_xs'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of columns on phones'),
      '#default_value' => $this->options['columns_xs'],
      '#required' => FALSE,
      '#element_validate' => array('views_element_validate_integer'),
      '#states' => array(
        'visible' => array(
          ':input[name="style_options[responsive_toggle]"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['alignment'] = array(
      '#type' => 'radios',
      '#title' => t('Alignment'),
      '#options' => array('horizontal' => t('Horizontal'), 'vertical' => t('Vertical')),
      '#default_value' => $this->options['alignment'],
      '#description' => t('Horizontal alignment will place items starting in the upper left and moving right. Vertical alignment will place items starting in the upper left and moving down.'),
    );
    $form['fill_single_line'] = array(
      '#type' => 'checkbox',
      '#title' => t('Fill up single line'),
      '#description' => t('If you disable this option, a grid with only one row will have the same number of table cells (<TD>) as items. Disabling it can cause problems with your CSS.'),
      '#default_value' => !empty($this->options['fill_single_line']),
    );

    $form['caption'] = array(
      '#type' => 'textfield',
      '#title' => t('Short description of table'),
      '#description' => t('Include a caption for better accessibility of your table.'),
      '#default_value' => $this->options['caption'],
    );

    $form['summary'] = array(
      '#type' => 'textfield',
      '#title' => t('Table summary'),
      '#description' => t('This value will be displayed as table-summary attribute in the html. Use this to give a summary of complex tables.'),
      '#default_value' => $this->options['summary'],
    );
  }
}
