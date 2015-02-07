(function($){
  // Shorthand for $( document ).ready()
  $(function() {
    // this will activate on the targets
    $('*[data-toggle="tooltip"]').tooltip();
    $('*[data-toggle="popover"]').popover();
  });
})(jQuery)