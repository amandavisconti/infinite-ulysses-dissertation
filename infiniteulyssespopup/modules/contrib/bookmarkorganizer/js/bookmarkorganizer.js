(function ($) {

/**
 * Override tabledrag onDrop event and save table automatically on drop
 */
Drupal.tableDrag.prototype.onDrop = function() {
  // Store active item id
  var activeItemID = $('.bookmarkorganizer-index-table.preview-enabled tr.active-item .item-id').attr('value');

  $.ajax({
    type: "POST",
    url: Drupal.settings.bookmarkorganizer.savepath + Drupal.settings.bookmarkorganizer.uid,  
    data:  $('#bookmarkorganizer-user-bookmarks-form').serialize() + "&js=1",
    success: function(data) {
      $('#bookmarkorganizer-user-bookmarks-form').replaceWith(data.bookmarks);
      Drupal.attachBehaviors($('#bookmarkorganizer-user-bookmarks-form'));
      // Refresh preview pane according to the active item
      if(!activeItemID) {
        Drupal.bookmarkorganizer.getPreviewRoot();
      }
      else {
        $('.bookmarkorganizer-index-table.preview-enabled tr.item-'+activeItemID+' .label').click();
      }

    },
    dataType: "json"
  });
  return false;
};

/**
 * Get rid of tabledrag messages about needing to save
 */ 
Drupal.theme.tableDragChangedWarning = function () { 
  return '<div></div>';
};

/*
 * Item add folder form on a popup dialog
 */
Drupal.bookmarkorganizer = {};
Drupal.bookmarkorganizer.build_addfolder_popup = function(response) {
  // Print form on dialog popup
  $("#dialog").html(response.html);
  
   // Open dialog
  $('#dialog').dialog('open');
  
  // Form elements
  var form_button = $('#'+response.data.button['#attributes'].id);
  var form_cancel_button = $('#'+response.data.cancel['#attributes'].id);
  var target_form = form_button.parents("form:first");
  
  // When button click event is invoked send form to add folder menu callback
  form_button.click(function() {
    $.ajax({  
      type: "GET",  
      url: response.path,
      data:  target_form.serialize() + "&js=1",  
      success: function(data) {
        $('#bookmarkorganizer-user-bookmarks-form').replaceWith(data.bookmarks);
        // After addition close dialog popup
        $('#dialog').dialog('close');
        // Rebind DOM elements
        Drupal.attachBehaviors($('#bookmarkorganizer-user-bookmarks-form'));
      },
      dataType: "json"
    });
    return false;
  });
  
  // Cancel button click event handling
  form_cancel_button.click(function() {
    $('#dialog').dialog('close');
    return false;
  });
  
}

/*
 * Item remove form on a popup dialog
 */
Drupal.bookmarkorganizer.build_remove_popup = function(response) {
  // Print form on dialog popup
  $("#dialog").html(response.html);
  
   // Open dialog
  $('#dialog').dialog('open');
  
  // Form elements
  var form_button = $('#'+response.data.button['#attributes'].id);
  var form_cancel_button = $('#'+response.data.cancel['#attributes'].id);
  var target_form = form_button.parents("form:first");
  
  // Store active item id
  var activeItemID = $('.bookmarkorganizer-index-table.preview-enabled tr.active-item .item-id').attr('value');
  // Store the item id to be removed
  var removedItemID = response.data.bookmarkorganizer_item['#value'].id;
  //alert("activeItemID:"+activeItemID+", removedItemID:"+removedItemID);
  // When button click event is invoked send form to delete menu callback
  form_button.click(function() {
    $.ajax({  
      type: "GET",  
      url: response.path,
      data:  target_form.serialize() + "&js=1",  
      success: function(data) {
        // After deletion close dialog popup
        $('#dialog').dialog('close');
        $('#bookmarkorganizer-user-bookmarks-form').replaceWith(data.bookmarks);
        Drupal.attachBehaviors($('#bookmarkorganizer-user-bookmarks-form'));
        // Display root folder on preview pane in case we removed an active preview item
        if(activeItemID == removedItemID) {
          Drupal.bookmarkorganizer.getPreviewRoot();
        }
        else {
          $('.bookmarkorganizer-index-table.preview-enabled tr.item-'+activeItemID+' .label').click();
        }
      },
      dataType: "json"
    });
    return false;
  });
  
  // Cancel button click event handling
  form_cancel_button.click(function() {
    $('#dialog').dialog('close');
    return false;
  });    
 
}

/*
 * Item rename form
 */
Drupal.bookmarkorganizer.build_rename_form = function(response) {
  // Title field
  var target_element = $('#'+response.target_element);
  
  // Toggle back to non editable
  if($('#'+response.data.button['#attributes'].id).length) {
    var form_button = $('#'+response.data.button['#attributes'].id);
    var target_form = form_button.parents("form:first");
    target_form.remove();
    target_element.show();
    target_element.parents("td:first").removeClass("editable");
    return false;
  } else {
    target_element.parents("td:first").addClass("editable");
  }
  
  // Hide element and add form to dom
  target_element.hide();
  target_element.after(response.html);
  
  // Form elements
  var form_button = $('#'+response.data.button['#attributes'].id);
  var target_form = form_button.parents("form:first");
  
  $(document).keyup(function(e) {
    // esc
    if (e.keyCode == 27) { 
      target_form.remove();
      target_element.show();
      target_element.parents("td:first").removeClass("editable");
    }
  });
  
  // When button click event is invoked send form to rename menu callback
  form_button.click(function() {
    $.ajax({  
      type: "GET",  
      url: response.path,
      data:  target_form.serialize() + "&js=1",  
      success: function(data) {
        // Remove rename form and replace the old label with the new one. Dom is not changing so we don't have to attachBehaviors.
        target_form.remove();
        target_element.html(data.label);
        target_element.show();
        target_element.parents("td:first").removeClass("editable");
      },
      dataType: "json"
    });
    return false;
  });

}

/**
 * Update root folder to preview pane
 */
Drupal.bookmarkorganizer.getPreviewRoot = function(context) {
  var bookmarks_form = $(this).parents("form:first");
  var token = bookmarks_form.find("input[name=form_token]").val();
  $(".bookmarkorganizer-index-table tr.active-item").removeClass("active-item");
  $.ajax({
    type: "POST",
    url: Drupal.settings.bookmarkorganizer.previewrootpath,
    data:  {js:1, form_token:token},
    success: function(data) {
      $('#'+data.target_element).replaceWith(data.html);
      // Attach possible javascript behaviors on bookmarkorganizer preview dom.
      Drupal.attachBehaviors($('#bookmarkorganizer-preview'));
    },
    dataType: "json"
  });
}

/**
 * Bookmark Organizer behaviors
 */
Drupal.behaviors.bookmarkorganizer = {
  attach: function(context) {

    $('.bookmarkorganizer-index-table .row', context).hover(
      function() {
        $(this).addClass("hover");
      },
      function() {
        $(this).removeClass("hover");
      }
    );

    $('.bookmarkorganizer-index-table-heading', context).click(function() {
      Drupal.bookmarkorganizer.getPreviewRoot(context);
    });

    $('a.action-addfolder', context).click(function() {
      $.ajax({
        type: "POST",
        url: $(this).attr("href"), 
        data: {js:1},
        success: function(data) {
          Drupal.bookmarkorganizer.build_addfolder_popup(data)
        },
        dataType: "json"
      });
      return false;
    });

    /*
     * Preview click
     */
    $('.bookmarkorganizer-index-table.preview-enabled tr .label', context).click(function() {
      var bookmarks_form = $(this).parents("form:first");
      var token = bookmarks_form.find("input[name=form_token]").val();
      var item_id = $(this).parents("tr:first").find("input.item-id:first").val();
      
      $(this).parents("table:first").find("tr").removeClass("active-item");
	    $(this).parents("tr:first").addClass("active-item");

	    // Update activelist table
	    $.ajax({
        type: "POST",
        url: Drupal.settings.bookmarkorganizer.activelistpath + item_id,  
        data:  {js:1, form_token:token},
        success: function(data) {
          $('#'+data.target_element).replaceWith(data.html);
          // Attach possible javascript behaviors on bookmarkorganizer preview dom 
          Drupal.attachBehaviors($('#bookmarkorganizer-preview'));
		    },
		    dataType: "json"
	    });
	    
	    return false;
    });

    /*
     * Toggle bookmarkorganizer item open or closed on click
     */
    $('.bookmarkorganizer-index-table .folder-link', context).click(function() {
      var bookmarks_form = $(this).parents("form:first");
      var token = bookmarks_form.find("input[name=form_token]").val();
      var item_id = $(this).parents("tr:first").find("input.item-id:first").val();    
      // Toggle folder
      $.ajax({
        type: "GET",
        url: Drupal.settings.bookmarkorganizer.togglefolderpath + item_id,  
        data:  {js:1, form_token:token},
        success: function(data) {
          $('#bookmarkorganizer-user-bookmarks-form').replaceWith(data.bookmarks);
          Drupal.attachBehaviors($('#bookmarkorganizer-user-bookmarks-form'));
		    },
		    dataType: "json"
	    });
	    return false;
    });
    
    /*
     * Popup dialog initialization
     */
    $('#dialog', context).dialog({
      autoOpen: false,
      width: 400,
      modal: true,
      resizable: false
    });

    /*
     * Item click event handlers on remove and rename
     */
    $(".action-remove", context).click(function() {
      $.ajax({  
        type: "POST",  
        url: $(this).attr("href"),  
        data: {js:1},  
        success: function(response) {
          Drupal.bookmarkorganizer.build_remove_popup(response);
		},
		dataType: "json"
	  });
      return false;
    });

    $(".action-rename", context).click(function() {
      $.ajax({  
        type: "POST",  
        url: $(this).attr("href"),  
        data: {js:1},  
        success: function(response) {
          Drupal.bookmarkorganizer.build_rename_form(response);
		},
		dataType: "json"
	  });
      return false;
    });

    // Publish / unpublish folder
    $('.action-folder', context).click(function() {
      var bookmarks_form = $('form#bookmarkorganizer-user-bookmarks-form');
      var token = bookmarks_form.find("input[name=form_token]").val();
      var link = $(this);
      $.ajax({
        type: "GET",
        url: $(this).attr('href'),
        data:  {js:1, form_token:token},
        success: function(data) {
          if(data.url && data.label) {
            if(data.action == 'publish') {
              link.removeClass('action-folder-unpublish').addClass('action-folder-publish');
            }
            else if(data.action == 'unpublish') {
              link.removeClass('action-folder-publish').addClass('action-folder-unpublish');
            }
            link.attr('alt', data.alt);
            link.attr('title', data.alt);
            link.attr('href', data.url);
            link.html(data.label);
          }
        },
        dataType: "json"
      });
      return false;
    });

  }
};

})(jQuery);
