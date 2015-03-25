<?php

/**
 * @file
 * Kalatheme theme implementation to display a node.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 *
 * @ingroup themeable
 */
?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

<!-- Amanda: add edit link for those with permissions to edit an annotation (admin role or author of annotation) on annotations -->
<div id="editannolink"><?php
if (node_access('update',$node)){
print l(t('edit'),'node/'.$node->nid.'/edit' );
}
?></div>

  <?php print render($title_prefix); ?>
  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <?php print $user_picture; ?>
  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>
     
<!-- Amanda: uncomment if wish to show user name, date metadata again -->
<!--<?php if ($display_submitted): ?> 
    <div class="submitted">
      <?php print $submitted; ?>
    </div>
  <?php endif; ?>
--><?php print render($content['links']); ?>
  <?php print render($content['comments']); ?>

</article>
