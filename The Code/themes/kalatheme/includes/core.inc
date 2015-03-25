<?php
/**
 * @file
 * Theme and hook functions for core things.
 */


/**
 * Implements theme_table().
 */
function kalatheme_table($variables) {
  $variables['attributes']['class'] = (isset($variables['attributes']['class'])) ? $variables['attributes']['class'] : array();
  $variables['attributes']['class'] = (is_array($variables['attributes']['class'])) ? $variables['attributes']['class'] : array($variables['attributes']['class']);
  $variables['attributes']['class'] = array_merge($variables['attributes']['class'],
    array(
      'table',
      'table-striped',
      'table-bordered',
      'table-hover',
    ));

  $header = $variables['header'];
  $rows = $variables['rows'];
  $attributes = $variables['attributes'];
  $caption = $variables['caption'];
  $colgroups = $variables['colgroups'];
  $sticky = $variables['sticky'];
  $empty = $variables['empty'];

  $output = '<table' . drupal_attributes($attributes) . ">\n";

  if (isset($caption)) {
    $output .= '<caption>' . $caption . "</caption>\n";
  }

  if (count($colgroups)) {
    foreach ($colgroups as $number => $colgroup) {
      $attributes = array();

      if (isset($colgroup['data'])) {
        foreach ($colgroup as $key => $value) {
          if ($key == 'data') {
            $cols = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $cols = $colgroup;
      }

      // Build colgroup.
      if (is_array($cols) && count($cols)) {
        $output .= ' <colgroup' . drupal_attributes($attributes) . '>';
        $i = 0;
        foreach ($cols as $col) {
          $output .= ' <col' . drupal_attributes($col) . ' />';
        }
        $output .= " </colgroup>\n";
      }
      else {
        $output .= ' <colgroup' . drupal_attributes($attributes) . " />\n";
      }
    }
  }

  // Add the 'empty' row message if available.
  if (!count($rows) && $empty) {
    $header_count = 0;
    foreach ($header as $header_cell) {
      if (is_array($header_cell)) {
        $header_count += isset($header_cell['colspan']) ? $header_cell['colspan'] : 1;
      }
      else {
        $header_count++;
      }
    }
    $rows[] = array(array(
        'data' => $empty,
        'colspan' => $header_count,
        'class' => array('empty', 'message'),
      ));
  }

  // Format the table header:
  if (count($header)) {
    $ts = tablesort_init($header);
    // HTML requires that the thead tag has tr tags in it followed by tbody
    // tags. Using ternary operator to check and see if we have any rows.
    $output .= (count($rows) ? ' <thead><tr>' : ' <tr>');
    foreach ($header as $cell) {
      $cell = tablesort_header($cell, $header, $ts);
      $output .= _theme_table_cell($cell, TRUE);
    }
    // Using ternary operator to close the tags based on whether,
    // or not there are rows.
    $output .= (count($rows) ? " </tr></thead>\n" : "</tr>\n");
  }
  else {
    $ts = array();
  }

  // Format the table rows.
  if (count($rows)) {
    $output .= "<tbody>\n";
    foreach ($rows as $number => $row) {
      $attributes = array();

      // Check if we're dealing with a simple or complex row.
      if (isset($row['data'])) {
        foreach ($row as $key => $value) {
          if ($key == 'data') {
            $cells = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $cells = $row;
      }
      if (count($cells)) {

        // Build row.
        $output .= ' <tr' . drupal_attributes($attributes) . '>';
        $i = 0;
        foreach ($cells as $cell) {
          $cell = tablesort_cell($cell, $header, $ts, $i++);
          $output .= _theme_table_cell($cell);
        }
        $output .= " </tr>\n";
      }
    }
    $output .= "</tbody>\n";
  }

  $output .= "</table>\n";
  return $output;
}

/**
 * Implements theme_status_report().
 */
function kalatheme_status_report($variables) {
  $requirements = $variables['requirements'];
  $severities = array(
    REQUIREMENT_INFO => array(
      'title' => t('Info'),
      'class' => 'alert alert-info',
    ),
    REQUIREMENT_OK => array(
      'title' => t('OK'),
      'class' => 'alert alert-success',
    ),
    REQUIREMENT_WARNING => array(
      'title' => t('Warning'),
      'class' => 'alert alert-warning',
    ),
    REQUIREMENT_ERROR => array(
      'title' => t('Error'),
      'class' => 'alert alert-danger',
    ),
  );
  $output = '<table class="system-status-report">';

  foreach ($requirements as $requirement) {
    if (empty($requirement['#type'])) {
      $severity = $severities[isset($requirement['severity']) ? (int) $requirement['severity'] : 0];
      $severity['icon'] = '<div title="' . $severity['title'] . '"><span class="element-invisible">' . $severity['title'] . '</span></div>';

      // Output table row(s)
      if (!empty($requirement['description'])) {
        $output .= '<tr class="' . $severity['class'] . ' merge-down"><td class="status-icon">' . $severity['icon'] . '</td><td class="status-title">' . $requirement['title'] . '</td><td class="status-value">' . $requirement['value'] . '</td></tr>';
        $output .= '<tr class="' . $severity['class'] . ' merge-up"><td colspan="3" class="status-description">' . $requirement['description'] . '</td></tr>';
      }
      else {
        $output .= '<tr class="' . $severity['class'] . '"><td class="status-icon">' . $severity['icon'] . '</td><td class="status-title">' . $requirement['title'] . '</td><td class="status-value">' . $requirement['value'] . '</td></tr>';
      }
    }
  }

  $output .= '</table>';
  return $output;
}

/**
 * Returns HTML for status and/or error messages, grouped by type.
 */
function kalatheme_status_messages($variables) {
  $display = $variables['display'];
  $output = '';

  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
  );

  // Map Drupal message types to their corresponding Bootstrap classes.
  // @see http://twitter.github.com/bootstrap/components.html#alerts
  $status_class = array(
    'status' => 'success',
    'error' => 'danger',
    'warning' => 'info',
  );

  foreach (drupal_get_messages($display) as $type => $messages) {
    $class = (isset($status_class[$type])) ? ' alert-' . $status_class[$type] : '';
    $output .= "<div role=\"alert\" class=\"alert alert-block $class\">\n";
    $output .= '<button type="button" class="close" data-dismiss="alert"><span aria-hidden="true">&times;</span><span class="sr-only">Close</span></button>';

    if (!empty($status_heading[$type])) {
      $output .= '<strong>' . $status_heading[$type] . "</strong>\n";
    }

    if (count($messages) > 1) {
      $output .= " <ul>\n";
      foreach ($messages as $message) {
        $output .= '  <li>' . $message . "</li>\n";
      }
      $output .= " </ul>\n";
    }
    else {
      $output .= $messages[0];
    }

    $output .= "</div>\n";
  }
  return $output;
}

/**
 * Returns HTML for the project status report.
 *
 * @param array $variables
 *   An associative array containing:
 *   - data: An array of data about each project's status.
 *
 * @ingroup themeable
 */
function kalatheme_update_report($variables) {
  $data = $variables['data'];

  $last = variable_get('update_last_check', 0);
  $output = theme('update_last_check', array('last' => $last));

  if (!is_array($data)) {
    $output .= '<p>' . $data . '</p>';
    return $output;
  }

  $header = array();
  $rows = array();

  $notification_level = variable_get('update_notification_threshold', 'all');

  // Create an array of status values keyed by module or theme name, since
  // we'll need this while generating the report if we have to cross reference
  // anything (e.g. subthemes which have base themes missing an update).
  foreach ($data as $project) {
    foreach ($project['includes'] as $key => $name) {
      $status[$key] = $project['status'];
    }
  }

  foreach ($data as $project) {
    switch ($project['status']) {
      case UPDATE_CURRENT:
        $class = 'alert alert-success';
        $icon = theme('image', array(
          'path' => 'misc/watchdog-ok.png',
          'width' => 18,
          'height' => 18,
          'alt' => t('ok'),
          'title' => t('ok'),
        ));
        break;

      case UPDATE_UNKNOWN:
        break;

      case UPDATE_FETCH_PENDING:
        break;

      case UPDATE_NOT_FETCHED:
        $class = 'alert alert-info';
        $icon = theme('image', array(
          'path' => 'misc/watchdog-warning.png',
          'width' => 18,
          'height' => 18,
          'alt' => t('warning'),
          'title' => t('warning'),
        ));
        break;

      case UPDATE_NOT_SECURE:
        break;

      case UPDATE_REVOKED:
        break;

      case UPDATE_NOT_SUPPORTED:
        $class = 'alert alert-danger';
        $icon = theme('image', array(
          'path' => 'misc/watchdog-error.png',
          'width' => 18, 'height' => 18,
          'alt' => t('error'),
          'title' => t('error'),
        ));
        break;

      case UPDATE_NOT_CHECKED:
        break;

      case UPDATE_NOT_CURRENT:
        break;

      default:
        $class = 'alert alert-warning';
        $icon = theme('image', array(
          'path' => 'misc/watchdog-warning.png',
          'width' => 18,
          'height' => 18,
          'alt' => t('warning'),
          'title' => t('warning'),
        ));
        break;
    }

    $row = '<div class="version-status">';
    $status_label = theme('update_status_label', array('status' => $project['status']));
    $row .= !empty($status_label) ? $status_label : check_plain($project['reason']);
    $row .= '<span class="icon">' . $icon . '</span>';
    $row .= "</div>\n";

    $row .= '<div class="project">';
    if (isset($project['title'])) {
      if (isset($project['link'])) {
        $row .= l($project['title'], $project['link']);
      }
      else {
        $row .= check_plain($project['title']);
      }
    }
    else {
      $row .= check_plain($project['name']);
    }
    $row .= ' ' . check_plain($project['existing_version']);
    if ($project['install_type'] == 'dev' && !empty($project['datestamp'])) {
      $row .= ' <span class="version-date">(' . format_date($project['datestamp'], 'custom', 'Y-M-d') . ')</span>';
    }
    $row .= "</div>\n";

    $versions_inner = '';
    $security_class = array();
    $version_class = array();
    if (isset($project['recommended'])) {
      if ($project['status'] != UPDATE_CURRENT || $project['existing_version'] !== $project['recommended']) {

        // First, figure out what to recommend.
        // If there's only 1 security update and it has the same version we're
        // recommending, give it the same CSS class as if it was recommended,
        // but don't print out a separate "Recommended" line for this project.
        if (!empty($project['security updates']) && count($project['security updates']) == 1 && $project['security updates'][0]['version'] === $project['recommended']) {
          $security_class[] = 'version-recommended';
          $security_class[] = 'version-recommended-strong';
        }
        else {
          $version_class[] = 'version-recommended';
          // Apply an extra class if we're displaying both a recommended
          // version and anything else for an extra visual hint.
          if ($project['recommended'] !== $project['latest_version']
              || !empty($project['also'])
              || ($project['install_type'] == 'dev'
                && isset($project['dev_version'])
                && $project['latest_version'] !== $project['dev_version']
                && $project['recommended'] !== $project['dev_version'])
              || (isset($project['security updates'][0])
                && $project['recommended'] !== $project['security updates'][0])
              ) {
            $version_class[] = 'version-recommended-strong';
          }
          $versions_inner .= theme('update_version', array(
            'version' => $project['releases'][$project['recommended']],
            'tag' => t('Recommended version:'),
            'class' => $version_class,
          ));
        }

        // Now, print any security updates.
        if (!empty($project['security updates'])) {
          $security_class[] = 'version-security';
          foreach ($project['security updates'] as $security_update) {
            $versions_inner .= theme('update_version', array(
              'version' => $security_update,
              'tag' => t('Security update:'),
              'class' => $security_class,
            ));
          }
        }
      }

      if ($project['recommended'] !== $project['latest_version']) {
        $versions_inner .= theme('update_version', array(
          'version' => $project['releases'][$project['latest_version']],
          'tag' => t('Latest version:'),
          'class' => array('version-latest'),
        ));
      }
      if ($project['install_type'] == 'dev'
          && $project['status'] != UPDATE_CURRENT
          && isset($project['dev_version'])
          && $project['recommended'] !== $project['dev_version']) {
        $versions_inner .= theme('update_version', array(
          'version' => $project['releases'][$project['dev_version']],
          'tag' => t('Development version:'),
          'class' => array('version-latest'),
        ));
      }
    }

    if (isset($project['also'])) {
      foreach ($project['also'] as $also) {
        $versions_inner .= theme('update_version', array(
          'version' => $project['releases'][$also],
          'tag' => t('Also available:'),
          'class' => array('version-also-available'),
        ));
      }
    }

    if (!empty($versions_inner)) {
      $row .= "<div class=\"versions\">\n" . $versions_inner . "</div>\n";
    }
    $row .= "<div class=\"info\">\n";
    if (!empty($project['extra'])) {
      $row .= '<div class="extra">' . "\n";
      foreach ($project['extra'] as $key => $value) {
        $row .= '<div class="' . implode(' ', $value['class']) . '">';
        $row .= check_plain($value['label']) . ': ';
        $row .= drupal_placeholder($value['data']);
        $row .= "</div>\n";
      }
      // Extra div.
      $row .= "</div>\n";
    }

    $row .= '<div class="includes">';
    sort($project['includes']);
    if (!empty($project['disabled'])) {
      sort($project['disabled']);
      // Make sure we start with a clean slate for each project in the report.
      $includes_items = array();
      $row .= t('Includes:');
      $includes_items[] = t('Enabled: %includes', array('%includes' => implode(', ', $project['includes'])));
      $includes_items[] = t('Disabled: %disabled', array('%disabled' => implode(', ', $project['disabled'])));
      $row .= theme('item_list', array('items' => $includes_items));
    }
    else {
      $row .= t('Includes: %includes', array('%includes' => implode(', ', $project['includes'])));
    }
    $row .= "</div>\n";

    if (!empty($project['base_themes'])) {
      $row .= '<div class="basethemes">';
      asort($project['base_themes']);
      $base_themes = array();
      foreach ($project['base_themes'] as $base_key => $base_theme) {
        switch ($status[$base_key]) {
          case UPDATE_NOT_SECURE:
          case UPDATE_REVOKED:
          case UPDATE_NOT_SUPPORTED:
            $base_themes[] = t('%base_theme (!base_label)', array(
              '%base_theme' => $base_theme,
              '!base_label' => theme('update_status_label', array(
                'status' => $status[$base_key])),
            ));
            break;

          default:
            $base_themes[] = drupal_placeholder($base_theme);
        }
      }
      $row .= t('Depends on: !basethemes', array('!basethemes' => implode(', ', $base_themes)));
      $row .= "</div>\n";
    }

    if (!empty($project['sub_themes'])) {
      $row .= '<div class="subthemes">';
      sort($project['sub_themes']);
      $row .= t('Required by: %subthemes', array('%subthemes' => implode(', ', $project['sub_themes'])));
      $row .= "</div>\n";
    }
    // Info div.
    $row .= "</div>\n";

    if (!isset($rows[$project['project_type']])) {
      $rows[$project['project_type']] = array();
    }
    $row_key = isset($project['title']) ? drupal_strtolower($project['title']) : drupal_strtolower($project['name']);
    $rows[$project['project_type']][$row_key] = array(
      'class' => array($class),
      'data' => array($row),
    );
  }

  $project_types = array(
    'core' => t('Drupal core'),
    'module' => t('Modules'),
    'theme' => t('Themes'),
    'module-disabled' => t('Disabled modules'),
    'theme-disabled' => t('Disabled themes'),
  );
  foreach ($project_types as $type_name => $type_label) {
    if (!empty($rows[$type_name])) {
      ksort($rows[$type_name]);
      $output .= "\n<h3>" . $type_label . "</h3>\n";
      $output .= theme('table', array(
        'header' => $header,
        'rows' => $rows[$type_name],
        'attributes' => array('class' => array('update')),
      ));
    }
  }
  drupal_add_css(drupal_get_path('module', 'update') . '/update.css');
  return $output;
}

/**
 * Implements theme_user_admin_permissions().
 *
 * Basically it just removes the 'checkbox' classes to avoid
 * conflicts with Bootstrap.
 */
function kalatheme_user_admin_permissions($variables) {
  $form = $variables['form'];

  $roles = user_roles();
  foreach (element_children($form['permission']) as $key) {
    $row = array();
    // Module name.
    if (is_numeric($key)) {
      $row[] = array(
        'data' => drupal_render($form['permission'][$key]),
        'class' => array('module'),
        'id' => 'module-' . $form['permission'][$key]['#id'],
        'colspan' => count($form['role_names']['#value']) + 1,
      );
    }
    else {
      // Permission row.
      $row[] = array(
        'data' => drupal_render($form['permission'][$key]),
        'class' => array('permission'),
      );
      foreach (element_children($form['checkboxes']) as $rid) {
        $form['checkboxes'][$rid][$key]['#title'] = $roles[$rid] . ': ' . $form['permission'][$key]['#markup'];
        // We are using a special title here for a couple reasons.
        // If use invisible the checkboxes get put inside the label as per
        // the bootstrap markup. However, this breaks the permissions UI
        // and does not allow us to unset permissions. If we use none then
        // we have no markup for screen readers.
        $form['checkboxes'][$rid][$key]['#title_display'] = 'permissions';
        $row[] = array('data' => drupal_render($form['checkboxes'][$rid][$key]));
      }
    }
    $rows[] = $row;
  }
  $header[] = (t('Permission'));
  foreach (element_children($form['role_names']) as $rid) {
    $header[] = array('data' => drupal_render($form['role_names'][$rid]));
  }
  $output = theme('system_compact_link');
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'permissions'),
  ));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Implements template_preprocess_comment().
 */
function kalatheme_preprocess_comment(&$variables) {
  $comment = $variables['comment'];

  // Add proper classes.
  $variables['title_attributes_array']['class'][] = 'media-heading';

  // Use icon for permalink.
  $uri = entity_uri('comment', $comment);
  $uri['options'] += array(
    'attributes' => array(
      'class' => 'permalink muted pull-right',
      'rel' => 'bookmark',
    ),
    'html' => TRUE,
  );
  $icon_html = '<i class="icon-comment"></i>';
  $variables['permalink'] = l($icon_html, $uri['path'], $uri['options']);

  // Use icon for new.
  $variables['new'] = !empty($comment->new) ? '<i class="icon-comment"></i>' : '';
}


/**
 * Implements theme_pager().
 */
function kalatheme_pager($variables) {
  $tags = $variables['tags'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $quantity = $variables['quantity'];
  global $pager_page_array, $pager_total;

  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // Current is the page we are currently paged to.
  $pager_current = $pager_page_array[$element] + 1;
  // First is the first page listed by this pager piece (re quantity).
  $pager_first = $pager_current - $pager_middle + 1;
  // Last is the last page listed by this pager piece (re quantity).
  $pager_last = $pager_current + $quantity - $pager_middle;
  // Max is the maximum page number.
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  // Prepare for generation loop.
  $i = $pager_first;
  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }
  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }
  // End of generation loop preparation.

  $li_first = theme('pager_first', array(
    'text' => (isset($tags[0]) ? $tags[0] : t('« first')),
    'element' => $element,
    'parameters' => $parameters,
  ));

  $li_previous = theme('pager_previous', array(
    'text' => (isset($tags[1]) ? $tags[1] : t('‹ previous')),
    'element' => $element,
    'interval' => 1,
    'parameters' => $parameters,
  ));

  $li_next = theme('pager_next', array(
    'text' => (isset($tags[3]) ? $tags[3] : t('next ›')),
    'element' => $element,
    'interval' => 1,
    'parameters' => $parameters,
  ));

  $li_last = theme('pager_last', array(
    'text' => (isset($tags[4]) ? $tags[4] : t('last »')),
    'element' => $element,
    'parameters' => $parameters,
  ));

  if ($pager_total[$element] > 1) {
    if ($li_first) {
      $items[] = array(
        'class' => array('pager-first'),
        'data' => $li_first,
      );
    }
    if ($li_previous) {
      $items[] = array(
        'class' => array('pager-previous'),
        'data' => $li_previous,
      );
    }

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {
      if ($i > 1) {
        $items[] = array(
          'class' => array('pager-ellipsis', 'disabled'),
          'data' => '<span aria-hidden="true">…<span>',
        );
      }
      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_previous', array(
              'text' => $i,
              'element' => $element,
              'interval' => ($pager_current - $i),
              'parameters' => $parameters,
            )),
          );
        }
        if ($i == $pager_current) {
          $items[] = array(
            'class' => array('pager-current', 'active'),
            'data' => '<span>' . $i . ' <span class="sr-only">(current)</span></span>',
          );
        }
        if ($i > $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_next', array(
              'text' => $i,
              'element' => $element,
              'interval' => ($i - $pager_current),
              'parameters' => $parameters,
            )),
          );
        }
      }
      if ($i < $pager_max) {
        $items[] = array(
          'class' => array('pager-ellipsis','disabled'),
          'data' => '<span aria-hidden="true">&#x2026;</span>',
        );
      }
    }
    // End generation.
    if ($li_next) {
      $items[] = array(
        'class' => array('pager-next'),
        'data' => $li_next,
      );
    }
    if ($li_last) {
      $items[] = array(
        'class' => array('pager-last'),
        'data' => $li_last,
      );
    }
    return '<nav role="navigation" class="text-center pagination-wrapper">' . theme('item_list', array(
      'plain' => TRUE,
      'items' => $items,
      'attributes' => array('class' => array('pagination')),
    )) . '</nav>';
  }
}


/**
 * Implements theme_progress_bar().
 */
function kalatheme_progress_bar($variables) {
  $output = '<div id="progress" class="progress active progress-striped">';
  $output .= '<div class="progress-bar" role="progressbar" aria-describedby="progressMessage" ';
  $output .= 'aria-valuenow="' . $variables['percent'] . '" ';
  $output .= 'aria-valuemin="0" aria-valuemax="100" style="width: ' . $variables['percent'] . '%;">';
  $output .= $variables['percent'] . '% Complete</div></div>';
  $output .= '<p class="help-block" id="progressMessage">' . $variables['message'] . '</p>';
  $output = '<div class="progress-wrapper">' . $output . '</div>';
  return $output;
}


/**
 * Implements theme_item_list().
 */
function kalatheme_item_list($variables) {
  $items = $variables['items'];
  $title = $variables['title'];
  $type = $variables['type'];
  $attributes = $variables['attributes'];
  $output = '';
  // Only output the list container and title, if there are any list items.
  // Check to see whether the block title exists before adding a header.
  // Empty headers are not semantic and present accessibility challenges.
  if (isset($title) && $title !== '') {
    $output .= '<h3>' . $title . '</h3>';
  }

  if (!empty($items)) {
    $output .= "<$type" . drupal_attributes($attributes) . '>';
    $num_items = count($items);
    $i = 0;
    foreach ($items as $item) {
      $attributes = array();
      $children = array();
      $data = '';
      $i++;
      if (is_array($item)) {
        foreach ($item as $key => $value) {
          if ($key == 'data') {
            $data = $value;
          }
          elseif ($key == 'children') {
            $children = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $data = $item;
      }
      if (count($children) > 0) {
        // Render nested list.
        $data .= theme_item_list(array(
          'items' => $children,
          'title' => NULL,
          'type' => $type,
          'attributes' => $attributes,
        ));
      }
      if ($i == 1) {
        $attributes['class'][] = 'first';
      }
      if ($i == $num_items) {
        $attributes['class'][] = 'last';
      }
      $output .= '<li' . drupal_attributes($attributes) . '>' . $data . "</li>\n";
    }
    $output .= "</$type>";
  }
  if (!isset($variables['plain'])) {
    $output = '<div class="item-list">' . $output . '</div>';
  }
  return $output;
}
