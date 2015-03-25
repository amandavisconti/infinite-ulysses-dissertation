<?php

/**
 * @file
 * Default simple view template to display a rows in a grid.
 *
 * - $rows contains a nested array of rows. Each row contains an array of
 *   columns.
 *
 * @ingroup views_templates
 */
?>
<?php
  // Build the bootstrap responsive classes
  $responsive_tiers = array('xs', 'sm', 'md', 'lg');
  $responsive = '';
  foreach ($responsive_tiers as $tier) {
    if (!empty(${$tier})) {
      $responsive .= 'col-' . $tier . '-' . ${$tier} . ' ';
    }
  }
  $item_count = 1;
?>

<?php if (!empty($title)) : ?>
<h3 class='grid-title'>
  <?php print $title; ?>
</h3>
<?php endif; ?>

<div class="views-view-grid row">
  <?php foreach ($rows as $row_number => $columns): ?>
    <?php foreach ($columns as $column_number => $item): ?>
      <?php if ($item): ?>
        <div class="gridcol gridborder <?php print 'col-' . ($column_number + 1); ?> <?php print 'col-item-' . ($item_count); ?>  <?php $responsive ? print $responsive : ''; ?>">
          <div class='grid-item'>
            <?php print $item; ?>
          </div>
        </div>
        <?php
        foreach ($responsive_tiers as $tier) {
          if (!empty(${$tier}) && (($item_count * ${$tier}) % $gridsize) === 0) { ?>
            <div class="clearfix visible-<?php print $tier; ?>"></div>
        <?php }
      } ?>
      <?php endif; ?>
      <?php $item_count++; ?>
    <?php endforeach; ?>
  <?php endforeach; ?>
</div>
