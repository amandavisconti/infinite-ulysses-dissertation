<div class='admin-panes clearfix '>
  <?php if ($layout == 'vertical'): ?>

    <?php foreach ($panels as $key => $panel): ?>
      <div class='admin-pane <?php if (!isset($first)) print 'admin-pane-active' ?> admin-pane-<?php print $key ?>'>
        <h2 class='admin-pane-title'><?php print $labels[$key] ?></h2>
        <div class='admin-pane-content clearfix'><?php print render($panel) ?></div>
      </div>
      <?php $first = TRUE ?>
    <?php endforeach; ?>

  <?php else: ?>

    <div class='admin-pane-tabs'>
      <?php foreach ($panels as $key => $panel): ?>
        <h2 class='admin-pane-title'><?php print $labels[$key] ?></h2>
      <?php endforeach; ?>
    </div>

    <?php foreach ($panels as $key => $panel): ?>
      <div class='admin-pane <?php if (!isset($first)) print 'admin-pane-active' ?> admin-pane-<?php print $key ?>'>
        <div class='admin-pane-content clearfix'><?php print render($panel) ?></div>
      </div>
      <?php $first = TRUE ?>
    <?php endforeach; ?>

  <?php endif; ?>

  <?php print render($others) ?>
</div>
