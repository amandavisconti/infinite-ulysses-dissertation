/*! kalatheme - v4.0.3 - 2014-12-03
* https://drupal.org/project/kalatheme
* Copyright (c) 2014 ; Licensed  */(function e(t,n,r){function s(o,u){if(!n[o]){if(!t[o]){var a=typeof require=="function"&&require;if(!u&&a)return a(o,!0);if(i)return i(o,!0);throw new Error("Cannot find module '"+o+"'")}var f=n[o]={exports:{}};t[o][0].call(f.exports,function(e){var n=t[o][1][e];return s(n?n:e)},f,f.exports,e,t,n,r)}return n[o].exports}var i=typeof require=="function"&&require;for(var o=0;o<r.length;o++)s(r[o]);return s})({1:[function(require,module,exports){
(function (global){
module.exports = function() {
  var $;
  $ = (typeof window !== "undefined" ? window.jQuery : typeof global !== "undefined" ? global.jQuery : null);
  return $(function() {
    var toggle;
    toggle = $('.dropdown-toggle');
    toggle.attr({
      'aria-expanded': 'false'
    });
    return $('.dropdown').on('shown.bs.dropdown', function(e) {
      return $(this).find('.dropdown-toggle').attr({
        'aria-expanded': 'true'
      });
    }).on('hidden.bs.dropdown', function(e) {
      return $(this).find('.dropdown-toggle').attr({
        'aria-expanded': 'false'
      });
    });
  });
};


}).call(this,typeof self !== "undefined" ? self : typeof window !== "undefined" ? window : {})
},{}],2:[function(require,module,exports){
require('./kalathemeAjax.coffee');

require('./kalathemeAutocomplete.coffee');

require('./kalathemeModal.coffee');

require('./kalathemeProgress.coffee');

require('./dropdownExpand.coffee')();


},{"./dropdownExpand.coffee":1,"./kalathemeAjax.coffee":3,"./kalathemeAutocomplete.coffee":4,"./kalathemeModal.coffee":5,"./kalathemeProgress.coffee":6}],3:[function(require,module,exports){

/**
* @file
* Overrides for core AJAX functionality.
* See misc/ajax.js
*
* Thanks bootstrap theme for insperation
* @link https://drupal.org/project/bootstrap
 */
(function($) {
  var _base;
  if ((_base = window.Drupal).ajax == null) {
    _base.ajax = function() {};
  }
  return Drupal.ajax.prototype.beforeSend = function(xmlhttprequest, options) {
    var iconClasses, markup, progressBar, v;
    if (this.form) {
      options.extraData = options.extraData || {};
      options.extraData.ajax_iframe_upload = "1";
      v = $.fieldValue(this.element);
      if (v !== null) {
        options.extraData[this.element.name] = v;
      }
    }
    $(this.element).addClass("progress-disabled").attr("disabled", true);
    if (this.progress.type === "bar") {
      progressBar = new Drupal.progressBar("ajax-progress-" + this.element.id, eval_(this.progress.update_callback), this.progress.method, eval_(this.progress.error_callback));
      if (this.progress.message) {
        progressBar.setProgress(-1, this.progress.message);
      }
      if (this.progress.url) {
        progressBar.startMonitoring(this.progress.url, this.progress.interval || 1500);
      }
      this.progress.element = $(progressBar.element).addClass("ajax-progress ajax-progress-bar");
      this.progress.object = progressBar;
      return $(this.element).after(this.progress.element);
    } else if (this.progress.type === "throbber") {
      iconClasses = Drupal.settings.kalatheme.fontawesome === true ? "fa fa-refresh fa-spin" : "glyphicon glyphicon-refresh glyphicon-spin";
      markup = "<div class=\"ajax-progress ajax-progress-throbber\">";
      markup += "<span class=\"" + iconClasses + "\" aria-hidden=\"true\"></span><span class=\"sr-only\">Loading</span></div>";
      this.progress.element = $(markup);
      if ($(this.element).is("input")) {
        if (this.progress.message) {
          $(".throbber", this.progress.element).after("<div class=\"message\">" + this.progress.message + "</div>");
        }
        return $(this.element).after(this.progress.element);
      } else {
        if (this.progress.message) {
          $(".throbber", this.progress.element).append("<div class=\"message\">" + this.progress.message + "</div>");
        }
        return $(this.element).append(this.progress.element);
      }
    }
  };
})(jQuery);


},{}],4:[function(require,module,exports){

/**
* @file
* Overrides for core autocomplete themeing.
* See misc/autocomplete.js
*
* Thanks bootstrap theme for insperation
* @link https://drupal.org/project/bootstrap
 */
(function($) {
  var _base;
  if (window.Drupal == null) {
    window.Drupal = {};
  }

  /**
  *Attaches the autocomplete behavior to all required fields.
   */
  Drupal.behaviors.autocomplete = {
    attach: function(context, settings) {
      var acdb;
      acdb = [];
      return $("input.autocomplete", context).once("autocomplete", function() {
        var $input, ariaLive, uri;
        uri = this.value;
        if (!acdb[uri]) {
          acdb[uri] = new Drupal.ACDB(uri);
        }
        $input = $("#" + this.id.substr(0, this.id.length - 13)).attr("autocomplete", "OFF").attr("aria-autocomplete", "list");
        $($input[0].form).submit(Drupal.autocompleteSubmit);
        ariaLive = $("<span class=\"element-invisible\" aria-live=\"assertive\"/>").attr("id", "" + ($input.attr("id")) + "-autocomplete-aria-live");
        $input.after;
        $input.parent().parent().attr("role", "application");
        return new Drupal.jsAC($input, acdb[uri]);
      });
    }
  };

  /*
  Prevents the form from submitting if the suggestions popup is open
  and closes the suggestions popup when doing so.
   */
  Drupal.autocompleteSubmit = function() {
    return $(".form-autocomplete > .dropdown").each(function() {
      return this.owner.hidePopup();
    }).length === 0;
  };
  if ((_base = window.Drupal).jsAC == null) {
    _base.jsAC = function() {};
  }

  /*
  Highlights a suggestion.
   */
  Drupal.jsAC.prototype.highlight = function(node) {
    if (this.selected) {
      $(this.selected).removeClass("active");
    }
    $(node).addClass("active");
    this.selected = node;
    return $(this.ariaLive).html($(this.selected).html());
  };

  /*
  Unhighlights a suggestion.
   */
  Drupal.jsAC.prototype.unhighlight = function(node) {
    $(node).removeClass("active");
    this.selected = false;
    return $(this.ariaLive).empty();
  };

  /*
  Positions the suggestions popup and starts a search.
   */
  Drupal.jsAC.prototype.populatePopup = function() {
    var $input;
    $input = $(this.input);
    if (this.popup) {
      $(this.popup).remove();
    }
    this.selected = false;
    this.popup = $("<div class=\"dropdown\"></div>")[0];
    this.popup.owner = this;
    $input.parent().after(this.popup);
    this.db.owner = this;
    this.db.search(this.input.value);
  };

  /*
  Fills the suggestion popup with any matches received.
   */
  Drupal.jsAC.prototype.found = function(matches) {
    var ac, key, ul;
    if (!this.input.value.length) {
      return false;
    }
    ul = $("<ul class=\"dropdown-menu\" role=\"menu\"></ul>");
    ac = this;
    ul.css({
      display: "block",
      right: 0
    });
    for (key in matches) {
      $("<li role=\"presentation\"></li>").html($("<a href=\"#\" role=\"menuitem\"/>").html(matches[key]).click(function(e) {
        return e.preventDefault();
      })).mousedown(function() {
        return ac.select(this);
      }).mouseover(function() {
        return ac.highlight(this);
      }).mouseout(function() {
        return ac.unhighlight(this);
      }).data("autocompleteValue", key).appendTo(ul);
    }
    if (this.popup) {
      if (ul.children().length) {
        $(this.popup).empty().append(ul).show();
        return $(this.ariaLive).html(Drupal.t("Autocomplete popup"));
      } else {
        $(this.popup).css({
          visibility: "hidden"
        });
        return this.hidePopup();
      }
    }
  };
  return Drupal.jsAC.prototype.setStatus = function(status) {
    var $throbber, fontAwesome, iconSpin, throbbingClass;
    fontAwesome = Drupal.settings.kalatheme.fontawesome ? true : false;
    iconSpin = fontAwesome ? 'fa-spin' : 'glyphicon-spin';
    $throbber = $(".fa-refresh, .glyphicon-refresh, .autocomplete-throbber", $("#" + this.input.id).parent()).first();
    throbbingClass = ($throbber.is(".autocomplete-throbber") ? "throbbing" : iconSpin);
    switch (status) {
      case "begin":
        $throbber.addClass(throbbingClass);
        return $(this.ariaLive).html(Drupal.t("Searching for matches..."));
      case "cancel":
      case "error":
      case "found":
        return $throbber.removeClass(throbbingClass);
    }
  };
})(jQuery);


},{}],5:[function(require,module,exports){

/**
* @file
* Overrides for CTools modal.
* See ctools/js/modal.js
 */
(function($) {

  /*
  Override CTools modal show function so it can recognize
  the Bootstrap modal classes correctly
   */
  if (window.Drupal == null) {
    window.Drupal = {};
  }
  if (Drupal.CTools == null) {
    Drupal.CTools = {};
  }
  if (Drupal.CTools.Modal != null) {
    Drupal.CTools.Modal.show = function(choice) {
      var defaults, opts, resize, settings;
      opts = {};
      if (choice && typeof choice === "string" && Drupal.settings[choice]) {
        $.extend(true, opts, Drupal.settings[choice]);
      } else {
        if (choice) {
          $.extend(true, opts, choice);
        }
      }
      defaults = {
        modalTheme: "CToolsModalDialog",
        throbberTheme: "CToolsModalThrobber",
        animation: "show",
        animationSpeed: "fast",
        modalSize: {
          type: "scale",
          width: 0.8,
          height: 0.8,
          addWidth: 0,
          addHeight: 0,
          contentRight: 25,
          contentBottom: 45
        },
        modalOptions: {
          opacity: 0.55,
          background: "#fff"
        }
      };
      settings = {};
      $.extend(true, settings, defaults, Drupal.settings.CToolsModal, opts);
      if (Drupal.CTools.Modal.currentSettings && Drupal.CTools.Modal.currentSettings !== settings) {
        Drupal.CTools.Modal.modal.remove();
        Drupal.CTools.Modal.modal = null;
      }
      Drupal.CTools.Modal.currentSettings = settings;
      resize = function(e) {
        var context, height, width;
        context = (e ? document : Drupal.CTools.Modal.modal);
        if (Drupal.CTools.Modal.currentSettings.modalSize.type === "scale") {
          width = $(window).width() * Drupal.CTools.Modal.currentSettings.modalSize.width;
          height = $(window).height() * Drupal.CTools.Modal.currentSettings.modalSize.height;
        } else {
          width = Drupal.CTools.Modal.currentSettings.modalSize.width;
          height = Drupal.CTools.Modal.currentSettings.modalSize.height;
        }
        $("div.ctools-modal-dialog", context).css({
          width: width + Drupal.CTools.Modal.currentSettings.modalSize.addWidth + "px",
          height: height + Drupal.CTools.Modal.currentSettings.modalSize.addHeight + "px"
        });
        $("div.ctools-modal-dialog .modal-body", context).css({
          width: (width - Drupal.CTools.Modal.currentSettings.modalSize.contentRight) + "px",
          height: (height - Drupal.CTools.Modal.currentSettings.modalSize.contentBottom) + "px"
        });
      };
      if (!Drupal.CTools.Modal.modal) {
        Drupal.CTools.Modal.modal = $(Drupal.theme(settings.modalTheme));
        if (settings.modalSize.type === "scale") {
          $(window).bind("resize", resize);
        }
      }
      $("body").addClass("modal-open");
      resize();
      $(".modal-title", Drupal.CTools.Modal.modal).html(Drupal.CTools.Modal.currentSettings.loadingText);
      Drupal.CTools.Modal.modalContent(Drupal.CTools.Modal.modal, settings.modalOptions, settings.animation, settings.animationSpeed);
      $("#modalContent .modal-body").html(Drupal.theme(settings.throbberTheme));
    };
    Drupal.CTools.Modal.dismiss = function() {
      console.log("oi");
      if (Drupal.CTools.Modal.modal) {
        $("body").removeClass("modal-open");
        Drupal.CTools.Modal.unmodalContent(Drupal.CTools.Modal.modal);
      }
    };
    if (Drupal.theme == null) {
      Drupal.theme = function() {};
    }

    /*
    Provide the HTML for the Modal.
     */
    Drupal.theme.prototype.CToolsModalDialog = function() {
      var html;
      html = "";
      html += "  <div id=\"ctools-modal\">";
      html += "    <div class=\"ctools-modal-dialog modal-dialog\">";
      html += "      <div class=\"modal-content\">";
      html += "        <div class=\"modal-header\">";
      html += "          <button type=\"button\" class=\"close ctools-close-modal\">";
      html += "           <span aria-hidden=\"true\">&times;</span>";
      html += "           <span class=\"sr-only\">Close</span></button>";
      html += "          <h4 id=\"modal-title\" class=\"modal-title\">&nbsp;</h4>";
      html += "        </div>";
      html += "        <div id=\"modal-content\" class=\"modal-body\">";
      html += "        </div>";
      html += "      </div>";
      html += "    </div>";
      html += "  </div>";
      return html;
    };

    /*
    Provide the HTML for Modal Throbber.
     */
    Drupal.theme.prototype.CToolsModalThrobber = function() {
      var html;
      html = "";
      html += "  <div class=\"loading-spinner\" style=\"position: absolute; top: 45%; left: 50%\">";
      html += "    <i class=\"fa fa-cog fa-spin fa-3x\"></i>";
      html += "  </div>";
      return html;
    };
  }
})(jQuery);


},{}],6:[function(require,module,exports){

/**
* @file
* Overrides for Progressbar function
* See misc/batch.js
* Thanks to @link https://drupal.org/project/bootstrap
 */
(function($) {
  var ProgressBar;
  if (window.Drupal == null) {
    window.Drupal = {};
  }

  /*
  A progressbar object. Initialized with the given id. Must be inserted into
  the DOM afterwards through progressBar.element.
  
  method is the function which will perform the HTTP request to get the
  progress bar state. Either "GET" or "POST".
  
  e.g. pb = new progressBar('myProgressBar');
  some_element.appendChild(pb.element);
   */
  ProgressBar = (function() {
    var pb;

    pb = ProgressBar;

    function ProgressBar(id, updateCallback, method, errorCallback) {
      var el, modalHtml;
      this.id = id;
      this.updateCallback = updateCallback;
      this.method = method;
      this.errorCallback = errorCallback;
      el = $("<div class=\"progress-wrapper\" aria-live=\"polite\"></div>");
      modalHtml = "<div id =\"" + this.id + "\" class=\"progress progress-striped active\"";
      modalHtml += "aria-describedby=\"message" + this.id + "\">\n<div class=\"progress-bar\"";
      modalHtml += " role=\"progressbar\" aria-valuemin=\"0\" aria-valuemax=\"100\" ";
      modalHtml += "aria-valuenow=\"0\">\n<div class=\"percentage\">\n</div>\n</div>\n</div>";
      modalHtml += "</div>\n";
      modalHtml += "<p class=\"message\ help-block\" id=\"message" + this.id + "\"></p>";
      el.html(modalHtml);
      this.element = el;
    }


    /*
    Set the percentage and status message for the progressbar.
     */

    ProgressBar.prototype.setProgress = function(percentage, message) {
      if (percentage >= 0 && percentage <= 100) {
        $(".progress-bar", this.element).css("width", percentage + "%");
        $(".progress-bar", this.element).attr("aria-valuenow", percentage);
        $(".percentage", this.element).html(percentage + "%");
      }
      $(".message", this.element).html(message);
      if (this.updateCallback) {
        return this.updateCallback(percentage, message, this);
      }
    };


    /*
    Start monitoring progress via Ajax.
     */

    ProgressBar.prototype.startMonitoring = function(uri, delay) {
      this.delay = delay;
      this.uri = uri;
      return this.sendPing();
    };


    /*
    Stop monitoring progress via Ajax.
     */

    ProgressBar.prototype.stopMonitoring = function() {
      clearTimeout(this.timer);
      return this.uri = null;
    };


    /*
    Request progress data from server.
     */

    ProgressBar.prototype.sendPing = function() {
      if (this.timer) {
        clearTimeout(this.timer);
      }
      if (this.uri) {
        pb = this;
        return $.ajax({
          type: this.method,
          url: this.uri,
          data: "",
          dataType: "json",
          success: function(progress) {
            if (progress.status === 0) {
              pb.displayError(progress.data);
            }
            pb.setProgress(progress.percentage, progress.message);
            return pb.timer = setTimeout(function() {
              return pb.sendPing();
            }, pb.delay);
          },
          error: function(xmlhttp) {
            return pb.displayError(Drupal.ajaxError(xmlhttp, pb.uri));
          }
        });
      }
    };


    /*
    Display errors on the page.
     */

    ProgressBar.prototype.displayError = function(string) {
      var error, errorHtml;
      errorHtml = "<div class=\"alert alert-block alert-error\" role=\"alert\">";
      errorHtml += "<button type=\"button\" class=\"close\"";
      errorHtml += " data-dismiss=\"alert\">";
      errorHtml += "<span aria-hidden=\"true\">&times;</span>";
      errorHtml += "     <span class=\"sr-only\">Close</span></button>";
      errorHtml += "<h4>Error message</h4></div>";
      error = $(errorHtml).append(string);
      $(this.element).before(error).hide();
      if (this.errorCallback) {
        return this.errorCallback(this);
      }
    };

    return ProgressBar;

  })();
  return Drupal.progressBar = ProgressBar;
})(jQuery);


},{}]},{},[2]);