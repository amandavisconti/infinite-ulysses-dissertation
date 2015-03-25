<?php
/**
 * @file
 * Template for Panopoly Rolph.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display rolph clearfix <?php !empty($class) ? print $class : ''; ?>" <?php !empty($css_id) ? print "id=\"$css_id\"" : ''; ?>>
  <section class="section alt" id="promo">
    <div class="container">
      <div class="row">
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_FULL); ?> rolph-header-area">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>
  <section class="section">
    <div class="container">
      <div class="row">
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_FOURTH, 4); ?> rolph-column-content-region1-">
          <?php print $content['quarter1']; ?>
        </div>
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_FOURTH, 4); ?> rolph-column-content-region-2">
          <?php print $content['quarter2']; ?>
        </div>
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_FOURTH, 4); ?> rolph-column-content-region-3">
          <?php print $content['quarter3']; ?>
        </div>
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_FOURTH, 4); ?> rolph-column-content-region-4">
          <?php print $content['quarter4']; ?>
        </div>
      </div>
    </div>
  </section>
   <footer class="section" id="footer">
    <div class="container">
      <div class="row">
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_FULL); ?> rolph-footer-area">
          <?php print $content['footer']; ?>
        </div>
      </div>
    </div>
  </footer>
</div><!-- /.rolph -->
