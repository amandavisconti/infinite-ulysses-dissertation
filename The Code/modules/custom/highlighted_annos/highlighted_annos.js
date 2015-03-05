/* Amanda: patched via this workaround: https://www.drupal.org/node/1543752#comment-7393666 */
(function ($) {
  Drupal.behaviors.ViewsExposedFormFix = {
    attach: function() {
      if (Drupal.settings && Drupal.settings.views && Drupal.settings.views.ajaxViews) {
        $.each(Drupal.settings.views.ajaxViews, function(i, settings) {
          // This matches the logic in Drupal.views.ajaxView.prototype.attachExposedFormAjax.
          var exposed_form = $('form#views-exposed-form-'+ settings.view_name.replace(/_/g, '-') + '-' + settings.view_display_id.replace(/_/g, '-'));
          exposed_form.once('views-exposed-form-fix', function() {
            var button = $('input[type=submit], button[type=submit], input[type=image]', exposed_form);
            button = button[0];
            // This will catch browsers that don't activate the submit button when pressing enter in the form.
            exposed_form.submit(function (event) {
              button.click();
              event.preventDefault();
              return false;
            });
          })
        });
      }
    }
  };
})(jQuery);