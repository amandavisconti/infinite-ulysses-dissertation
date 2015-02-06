//Amanda: links[0].href.replace( '/www.infiniteulysses.com/node/', ''); might need AnnoJS's implementation of LinkParser
(function ($) {
	Drupal.behaviors.annotatorSidebar = {
		attach: function (context, settings) {
          $("a").click(function() { //.annotator-h1
            currentValue = 50; //Could ref uncommented grabNID in AnnoJS
            alert(currentValue);
          });
		}
	};
})(jQuery);

// Query is now namespaced to avoid conflicts with other Javascript libraries such as Prototype. All your code that expects to use jQuery as $ should be wrapped in an outer context like so:

(function ($) {
  // ;
})(jQuery);

===========
===========

//Amanda: links[0].href.replace( '/www.infiniteulysses.com/node/', ''); might need AnnoJS's implementation of LinkParser
(function ($) {
	Drupal.behaviors.annotatorSidebar = {
		attach: function (context, settings) {
      // Next we’re going to write some jQuery to insert our values into the field. You can use any event trigger you want (.click or .change for example). What needs to be accomplished is to insert a delimited string of values. I’m grabbing the current value from the exposed form field...
          $(".annotator-h1").click(function() {
            currentValue = $(this).attr("href");
            currentValue = currentValue.replace( '/www.infiniteulysses.com/node/', '');
            currentValue = parseInt(currentValue);
            alert(currentValue);
          });
// ...And appending the new value. I’m using commas to delimit the list but you can use any character. Will this reset to current clicked highlight, or will NIDs accrue? //$('#filter-nid').val( $('#filter-nid').val() + currentValue +  ",");
		  $('#filter-nid').val(currentValue +  ",");
		}
	};
})(jQuery);