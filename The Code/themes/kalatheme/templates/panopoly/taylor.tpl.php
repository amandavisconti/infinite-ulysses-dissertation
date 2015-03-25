<?php
/**
 * @file
 * Template for Panopoly Taylor.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display taylor clearfix <?php !empty($class) ? print $class : ''; ?>" <?php !empty($css_id) ? print "id=\"$css_id\"" : ''; ?>>
  <section class="section">
    <div class="container">
      <div class="row">
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_FULL); ?> taylor-header-area">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>
  <section class="section">
    <div class="container">
      <div class="row">
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_HALF, 3); ?> taylor-half-area">
          <?php print $content['half']; ?>
        </div>
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_FOURTH, 3); ?> taylor-quarter-1-area">
          <?php print $content['quarter1']; ?>
        </div>
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_FOURTH, 3); ?> taylor-quarter-2-area">
          <?php print $content['quarter2']; ?>
        </div>
      </div>
    </div>
  </section>
  <footer class="section" id="footer">
    <div class="container">
      <div class="row">
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_FULL); ?> taylor-footer-area">
          <?php print $content['footer']; ?>
        </div>
      </div>
    </div>
  </footer>
</div><!-- /.taylor -->
