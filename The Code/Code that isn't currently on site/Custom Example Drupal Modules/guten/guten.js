(function($) { 
Drupal.behaviors.guten = function (context) {
	$('a.annotator-h1', context).click(function () {
    // This function will get executed after the ajax request is completed successfully
		var updateguten = function(data) { //The js function that will be called upon success request
			$grabnid = '50'; // Testing by sending static number as string
		}
		$.ajax({
			type: 'POST',
			url: this.href, //Amanda: Which url should be handle the ajax request. This is the url defined in the <a> html tag but maybe NOT for a.annotator-h1? Might be fine (see return false comment below; seems to stop from navigating to that URL). Might need to be / instead, or match URL at top of .module file
			success: updateguten, //The js function that will be called upon success request
			dataType: 'json', //define the type of data that is going to get back from the server, can use other type such as HTML if needed but then strip out json de/encoding
			data: 'js=1' //Amanda: Pass a key/value pair. Why? Appended to the url for GET-requests. If value is an Array, jQuery serializes multiple values with same key based on the value of the traditional setting. Is this appending the NIDs to the URL?
		});
		return false;  // return false so the navigation stops here and not continue to the page in the link
	})
}
})(jQuery);