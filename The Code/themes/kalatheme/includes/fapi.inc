<?php
/**
 * @file
 * Theme and hook functions for fapi things.
 */

/**
 * Implements theme_form().
 */
function kalatheme_form($variables) {
  $element = $variables['element'];
  if (isset($element['#action'])) {
    $element['#attributes']['action'] = drupal_strip_dangerous_protocols($element['#action']);
  }
  element_set_attributes($element, array('method', 'id'));
  if (empty($element['#attributes']['accept-charset'])) {
    $element['#attributes']['accept-charset'] = "UTF-8";
  }

  $element['#attributes']['role'] = "form";

  return '<form' . drupal_attributes($element['#attributes']) . '><div>' . $element['#children'] . '</div></form>';
}

/**
 * Implements hook_element_info_alter().
 */
function kalatheme_element_info_alter(&$elements) {
  foreach ($elements as &$element) {
    $element['#process'][] = 'kalatheme_process_element';
    if (!empty($element['#input'])) {
      $element['#process'][] = 'kalatheme_process_input';
    }
  }
}

/**
 * Implements template_process_element().
 */
function kalatheme_process_element(&$element, &$form_state) {
  if (!empty($element['#attributes']['class']) && is_array($element['#attributes']['class'])) {
    if (in_array('container-inline', $element['#attributes']['class'])) {
      $element['#attributes']['class'][] = 'form-inline';
    }
    if (in_array('form-wrapper', $element['#attributes']['class'])) {
      $element['#attributes']['class'][] = 'form-group';
    }
  }
  return $element;
}

/**
 * Implements template_process_input().
 */
function kalatheme_process_input(&$element, &$form_state) {
  if (isset($element['#required']) && $element['#required'] == TRUE) {
    if (!isset($element['#attributes'])) {
      $element['#attributes'] = array();
    }
    // Is this process going to use a WYSIWYG?
    $wysiwyg_process = FALSE;
    // Checks prerender function.
    if (isset($element['#pre_render'])) {
      foreach ($element['#pre_render'] as $delta => $callback) {
        if ($callback == 'wysiwyg_pre_render_text_format') {
          $wysiwyg_process = TRUE;
        }
      }
    }

    // Unless it is a wysiwyg field.
    if (!$wysiwyg_process) {
      // Doubling up on required
      // @link http://john.foliot.ca/required-inputs/
      $element['#attributes']['required'] = 'true';
      $element['#attributes']['aria-required'] = 'true';
    }
  }

  $types = array(
    'textfield',
    'textarea',
    'password',
    'password_confirm',
    'select',
    'machine_name',
    'webform_email',
    'emailfield',
    'numberfield',
    'rangefield',
    'searchfield',
    'telfield',
    'urlfield',
  );
  if (!empty($element['#type']) && (in_array($element['#type'], $types))) {
    $element['#attributes']['class'][] = 'form-control';
  }
  return $element;
}


/**
 * Implements theme_form_element().
 */
function kalatheme_form_element($variables) {
  $element = &$variables['element'];
  $is_checkbox = FALSE;
  $is_radio = FALSE;

  $t = get_t();

  $element += array(
    '#title_display' => 'before',
  );

  $attributes['class'][] = 'form-item';

  if (isset($element['#markup']) && !empty($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  if (!empty($element['#type'])) {
    $attributes['class'][] = 'form-type-' . strtr($element['#type'], '_', '-');
  }
  if (!empty($element['#name'])) {
    $attributes['class'][] = 'form-item-' . strtr($element['#name'], array(
      ' ' => '-',
      '_' => '-',
      '[' => '-',
      ']' => '',
    ));
  }

  if (isset($element['#parents']) && form_get_error($element) !== NULL && !empty($element['#validated'])) {
    $attributes['class'][] = 'has-error';
  }

  if (isset($element['#type'])) {
    if ($element['#type'] == "radio") {
      $attributes['class'][] = 'radio';
      $is_radio = TRUE;
    }
    elseif ($element['#type'] == "checkbox") {
      $attributes['class'][] = 'checkbox';
      $is_checkbox = TRUE;
    }
    else {
      $attributes['class'][] = 'form-group';
    }
  }

  if (!empty($element['#attributes']['disabled'])) {
    $attributes['class'][] = 'form-disabled';
  }
  $output = '<div' . drupal_attributes($attributes) . '>' . "\n";

  if (!isset($element['#title'])) {
    $element['#title_display'] = 'none';
  }
  $prefix = isset($element['#field_prefix']) ? '<span class="field-prefix">' . $element['#field_prefix'] . '</span> ' : '';
  $suffix = isset($element['#field_suffix']) ? ' <span class="field-suffix">' . $element['#field_suffix'] . '</span>' : '';

  switch ($element['#title_display']) {
    case 'before':
    case 'permissions':
    case 'invisible':
      $output .= ' ' . theme('form_element_label', $variables);
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;

    case 'after':
      if ($is_radio || $is_checkbox) {
        $output .= ' ' . $prefix . theme('form_element_label', $variables) . $suffix;
      }
      else {
        $variables['#children'] = ' ' . $prefix . $element['#children'] . $suffix;
        $output .= ' ' . theme('form_element_label', $variables) . "\n";
      }
      break;

    case 'none':
    case 'attribute':
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;
  }

  if (!empty($element['#description'])) {
    $output .= '<p class="help-block">' . $element['#description'] . "</p>\n";
  }

  $output .= "</div>\n";

  return $output;
}


/**
 * Implements theme_form_element_label().
 */
function kalatheme_form_element_label($variables) {
  $element = $variables['element'];
  $t = get_t();

  $is_radio_or_checkbox = (isset($element['#type']) && ('checkbox' === $element['#type'] || 'radio' === $element['#type']));

  if ((!isset($element['#title']) || $element['#title'] === '') && empty($element['#required']) && !$is_radio_or_checkbox) {
    return '';
  }

  $required = !empty($element['#required']) ? theme('form_required_marker', array('element' => $element)) : '';

  $title = filter_xss_admin($element['#title']);

  $attributes = array();
  if ($element['#title_display'] == 'after' && !$is_radio_or_checkbox) {
    $attributes['class'] = 'option';
  }
  elseif ($element['#title_display'] == 'invisible' || $element['#title_display'] == 'permissions') {
    $attributes['class'] = 'element-invisible';
  }

  if (!empty($element['#id'])) {
    $attributes['for'] = $element['#id'];
  }

  $output = '';
  if ($is_radio_or_checkbox && $element['#title_display'] != 'permissions' && isset($element['#children'])) {
    $output .= $element['#children'];
  }

  $output .= $t('!title !required', array('!title' => $title, '!required' => $required));

  return ' <label' . drupal_attributes($attributes) . '>' . $output . "</label>\n";
}

/**
 * Implements theme_button().
 */
function kalatheme_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  $element['#attributes']['class'][] = 'btn';
  $element['#attributes']['class'][] = 'btn-default';
  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }

  if (isset($element['#parents']) && ($element['#parents'][0] == 'submit')) {
    $element['#attributes']['class'][] = 'btn-primary';
  }

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

/**
 * Implements theme_textarea().
 */
function kalatheme_textarea($variables) {
  $element = $variables['element'];
  // Set only if they are set.
  if (isset($element['#name'])) {
    $element['#attributes']['name'] = $element['#name'];
  }
  if (isset($element['#id'])) {
    $element['#attributes']['id'] = $element['#id'];
  }
  if (isset($element['#cols'])) {
    $element['#attributes']['cols'] = $element['#cols'];
  }
  if (isset($element['#rows'])) {
    $element['#attributes']['rows'] = $element['#rows'];
  }
  _form_set_class($element, array('form-textarea'));

  $wrapper_attributes = array(
    'class' => array('form-textarea-wrapper'),
  );

  if (!empty($element['#resizable'])) {
    $wrapper_attributes['class'][] = 'resizable';
  }

  $output = '<div' . drupal_attributes($wrapper_attributes) . '>';
  $output .= '<textarea' . drupal_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  $output .= '</div>';
  return $output;
}

/**
 * Implements theme_date().
 */
function kalatheme_date($variables) {
  $element = $variables['element'];

  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = (array) $element['#attributes']['class'];
  }
  $attributes['class'][] = 'form-inline';

  return '<div' . drupal_attributes($attributes) . '>' . drupal_render_children($element) . '</div>';
}

/**
 * Implements theme_textfield().
 */
function kalatheme_textfield($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'text';
  element_set_attributes($element, array(
    'id',
    'name',
    'value',
    'size',
    'maxlength',
  ));
  _form_set_class($element, array('form-text'));

  $output = '<input' . drupal_attributes($element['#attributes']) . ' />';

  $extra = '';
  if ($element['#autocomplete_path'] && drupal_valid_path($element['#autocomplete_path'])) {
    drupal_add_library('system', 'drupal.autocomplete');
    $element['#attributes']['class'][] = 'form-autocomplete';

    $attributes = array();
    $attributes['type'] = 'hidden';
    $attributes['id'] = $element['#attributes']['id'] . '-autocomplete';
    $attributes['value'] = url($element['#autocomplete_path'], array('absolute' => TRUE));
    $attributes['disabled'] = 'disabled';
    $attributes['class'][] = 'autocomplete';

    $icon = _kalatheme_font_icon('refresh');

    $output = '<div class="input-group">' . $output . '<span class="input-group-addon">' . $icon . '</span></div>';

    $extra = '<input' . drupal_attributes($attributes) . ' />';
  }

  return $output . $extra;
}


/**
 * Implements theme_fieldset().
 */
function kalatheme_fieldset($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper'));
  _form_set_class($element, array('panel', 'panel-default'));

  $output = '<fieldset' . drupal_attributes($element['#attributes']) . '>';
  if (!empty($element['#title'])) {
    $output .= '<legend class="panel-heading"><div class="panel-title fieldset-legend">' . $element['#title'] . '</div></legend>';
  }
  $output .= '<div class="panel-body fieldset-wrapper">';
  if (!empty($element['#description'])) {
    $output .= '<p class="help-block">' . $element['#description'] . '</p>';
  }
  $output .= $element['#children'];
  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }
  $output .= '</div>';
  $output .= "</fieldset>\n";
  return $output;
}
/**
 * Implements theme_form_required_marker().
 */
function kalatheme_form_required_marker($variables) {

  $t = get_t();
  $attributes = array(
    'class' => 'form-required',
    'title' => $t('This field is required.'),
    // Don't announce this since we are already giving the form aria-required
    // required attributes.
    'aria-hidden' => 'true',
  );
  return '<span' . drupal_attributes($attributes) . '>*</span>';
}
