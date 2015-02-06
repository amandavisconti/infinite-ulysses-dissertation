(function ($) {
  Drupal.behaviors.ajax2example = { // Make sure the behavior name is unique.
    attach:function (context) { // In Drupal 7 the “attach: function” is used instead of the “= function” syntax. 
 
      // If the site name is present set it to the username.
      if ($('#site-name', context).length) { // This code simply checks if the site name is present.
        $.ajax({
          url: '/ajax/username',
          success: function(data) {
 
            // Change site name to current user name.
            $('#site-name a span').html(data + '.com'); // If so change the site name to the current users name. 
         }
        });
      }
    }
  }
})(jQuery);