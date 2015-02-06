<div class="well"><div id="loginmessage">This site is in beta-testing and is currently accepting new users via a waitlist. If you haven't received login information yet, sign up using the <a href="http://www.infiniteulysses.com/#signup">sign-up form on the front page</a>. You'll be contacted when we reach your name on the waitlist or when the site opens to the public (by June 16, 2015).</div>
<?php 
$elements = drupal_get_form("user_login"); 
$form = drupal_render($elements);
echo $form;
?></div>