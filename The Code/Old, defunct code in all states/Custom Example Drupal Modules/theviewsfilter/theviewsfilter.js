(function ($) {
	Drupal.behaviors.annotatorSidebar = {
		attach: function (context, settings) {
          $("a").hover(function() { //works on hover highlight then hover not-anno link?
            currentValue = 50;
            alert(grabNID);
            $('#filter-nid').val(currentValue +  ","); // does this need AJAX post?
          });
		}
	};
})(jQuery);