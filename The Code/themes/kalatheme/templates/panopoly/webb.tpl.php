<?php
/**
 * @file
 * Template for Panopoly Webb.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display webb clearfix <?php !empty($class) ? print $class : ''; ?>" <?php !empty($css_id) ? print "id=\"$css_id\"" : ''; ?>>
  <section class="section">
    <div class="container">
      <div class="row">
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_FULL); ?> webb-header-area">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>
  <section class="section">
    <div class="container">
      <div class="row">
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_THIRD); ?> webb-sidebar-area">
          <?php print $content['sidebar']; ?>
        </div>
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_THIRD * 2); ?> webb-flipped-column-content-region-area">
          <div class="row">
            <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_FULL); ?> webb-flipped-column-header-region-area">
              <?php print $content['contentheader']; ?>
            </div>
          </div>
          <div class="row">
            <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_HALF); ?> webb-flipped-column-content-region-1">
              <?php print $content['contentcolumn1']; ?>
            </div>
            <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_HALF); ?> webb-flipped-column-content-region-2">
              <?php print $content['contentcolumn2']; ?>
            </div>
          </div>
        </div>
      </div>
    </div><!-- /.webb-column-content -->
  </section>
</div><!-- /.webb -->
