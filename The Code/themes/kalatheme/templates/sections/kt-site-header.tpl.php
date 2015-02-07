<header role="banner">
 <div class="container">
  <div class="row">
    <div class="col-sm-6">
    <?php if ($logo): ?>
      <div class='brand pull-left'>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      </div>
    <?php endif; ?>




        <?php if ($site_name): ?>

          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
            <span class="h2 brand <?php if ($hide_site_name) { print 'element-invisible'; } ?>">
              <?php print $site_name; ?>
            </span>
          </a>

        <?php endif; ?>
    </div><!-- /.col-sm-6 -->

    <div class="col-sm-6">
      <?php if ($site_slogan): ?>
        <div id="site-slogan" <?php if ($hide_site_slogan) { print 'class="element-invisible"'; } ?>>
          <p class="lead text-right">
            <?php print $site_slogan; ?>
          </p>
        </div>
      <?php endif; ?>
    </div><!-- /.col-sm-6 -->

  </div><!--/.row-->

 </div><!--/.container-->

</header>
