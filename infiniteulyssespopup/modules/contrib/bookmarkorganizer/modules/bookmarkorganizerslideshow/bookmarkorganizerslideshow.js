
(function($) {
Drupal.behaviors.bookmarkslideshow = {};
Drupal.bookmarkslideshow = {};
Drupal.bookmarkslideshow.pgIndex = 0;
Drupal.bookmarkslideshow.container = null;
Drupal.bookmarkslideshow.totalCount = 0;
Drupal.bookmarkslideshow.itemVisibleCallback = function(p1, p2, itemIndex, p4) {
  Drupal.bookmarkslideshow.pgIndex = itemIndex;
  title = $.trim($('.bookmarkorganizer-slideshow-title .view-content').hide().text());
  if (title.length > 28) {
    title = title.substr(0, 25) + '...';
  }
  $('.page-count', Drupal.bookmarkslideshow.container).html(Drupal.t('!title @pg/@total',
                                                  {'!title': title, '@total': Drupal.bookmarkslideshow.totalCount, '@pg':  Drupal.bookmarkslideshow.pgIndex}));
};

Drupal.behaviors.bookmarkslideshow.attach = function(context) {
  $container = $('.jcarousel-skin-bookmarkorganizer .jcarousel-container', context);
  if ($container.length > 0) {
    Drupal.bookmarkslideshow.container = $container;
    Drupal.bookmarkslideshow.totalCount = $('ul.jcarousel li.jcarousel-item', $container).length;
    
    closeTxt = Drupal.t('Close');
    $container.append('<button class="close" title="'+closeTxt+'">'+closeTxt+'</button>');
    $('button.close').click(function() {
      if (window.opener && !window.opener.closed) {
        window.close(); //Close this (popup) window since the opener exists.
      }
      else {
        history.go(-1); //This is not popup. Go to the previous page using current window.
      }
    });
    
    $('a.jcarousel-prev', $container).append('<span>&lt;</span>');
    $('a.jcarousel-prev', $container).after('<span class="page-count"></span>');
    $('a.jcarousel-next', $container).append('<span>&gt;</span>');
    
    Drupal.bookmarkslideshow.itemVisibleCallback(null, null, 1, null);
    
    $(document).keyup(function(event) {
      if (event.keyCode == 37) {
        //Left arrow
        $('a.jcarousel-prev', Drupal.bookmarkslideshow.container).trigger('click');
      }
      else if (event.keyCode == 39) {
        //Right arrow
        $('a.jcarousel-next', Drupal.bookmarkslideshow.container).trigger('click');
      }
    });
  }
  
};

})(jQuery);
