(function ($) {

// Sync main title with secondary title.
Drupal.bookHelperSyncTitles  = function (syncSelector, mainTitleSelector, secondaryTitleSelector) {
  if (!$(syncSelector).length || $(syncSelector).hasClass('book-helper-sync-processed')) {
    return;
  }

  // Add event handler to sync checkbox.
  $(syncSelector).click( function () {
    if (this.checked) { // Node and book title are sync'ed
      $(secondaryTitleSelector).val( $(mainTitleSelector).val() )
    }
  }).addClass('book-helper-sync-processed');

  // Add event handlers to main title input.
  $(mainTitleSelector).bind('keyup mouseup blur', function () {
    if ($(syncSelector)[0].checked) {
      $(secondaryTitleSelector).val(this.value);
    }
  });

  // Add event handlers to secondary title input.
  $(secondaryTitleSelector).bind('keyup mouseup blur', function () {
    if ($(syncSelector)[0].checked) {
      $(mainTitleSelector).val(this.value);
    }
  });

  // Init the sync checkbox.
  $(syncSelector)[0].checked = ($(secondaryTitleSelector).val() == $(mainTitleSelector).val()) ? true : false;
}

// Hide/show book outline fields.
Drupal.bookHelperToggleSync = function() {
  var effect = ($('#edit-book-bid').val() == '0') ? 'hide' : 'show';
  $('.form-item-book-book-helper-link-title-custom, .form-item-book-book-helper-link-title-sync, .form-item-book-weight')[effect]();
}

// Book helper behaviors
Drupal.behaviors.bookHelper = {
  attach: function (context) {
    // Node edit form book outline.
    if ($('#edit-book-bid', context).length) {
      // Toggle sync fields
      $('#edit-book-bid').change(Drupal.bookHelperToggleSync);
      Drupal.bookHelperToggleSync();

      // Sync book titles on node edit form.
      Drupal.bookHelperSyncTitles(
        '#edit-book-book-helper-link-title-sync',
        '#edit-title',
        '#edit-book-book-helper-link-title-custom'
      );
    }

    // Sync book titles on book page order form.
    if ($('#book-outline', context).length) {
      $('#book-outline tbody td:nth-child(3) input').each(function(){
        // edit-table-book-admin-{nid}-sync
        var syncSelector = '#' + this.id;
        // edit-table-book-admin-{nid}-title
        var mainTitleSelector = syncSelector.replace('-sync', '-title');
        // edit-table-book-admin-{nid}-node-title
        var secondaryTitleSelector = syncSelector.replace('-sync', '-node-title');

        Drupal.bookHelperSyncTitles(syncSelector, mainTitleSelector, secondaryTitleSelector);
      });
    }
  }
};

})(jQuery);
