<?php
/**
 * @file
 * Template for default Panopoly Hewston Flipped.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display hewston-flipped clearfix <?php !empty($class) ? print $class : ''; ?>" <?php !empty($css_id) ? print "id=\"$css_id\"" : ''; ?>>
  <section class="section">
    <div class="container">
      <div class="row">
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_THIRD); ?> hewston-flipped-slider-gutter-area">
          <?php print $content['slidergutter']; ?>
        </div>
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_THIRD * 2); ?> hewston-flipped-slider-area">
          <?php print $content['slider']; ?>
        </div>
      </div>
    </div>
  </section>
  <section class="section">
    <div class="container">
      <div class="row">
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_THIRD, 3); ?> hewston-flipped-middle-region-1">
          <?php print $content['column1']; ?>
        </div>
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_THIRD, 3); ?> hewston-flipped-middle-region-2">
          <?php print $content['column2']; ?>
        </div>
        <div class="col-md-<?php print kalatheme_grid_size(KALATHEME_GRID_THIRD, 3); ?> hewston-flipped-middle-region-3">
          <?php print $content['column3']; ?>
        </div>
      </div>
    </div>
  </section>
</div><!-- /.hewston-flipped -->
