/*
Annotator view panel Plugin v1.0 (https://https://github.com/albertjuhe/annotator_view/)
Copyright (C) 2014 Albert Juhé Brugué
License: https://github.com/albertjuhe/annotator_view/License.rst

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

(function() {
  var __bind = function(fn, me){ return function(){ return fn.apply(me, arguments); }; },
    __hasProp = {}.hasOwnProperty,
    __extends = function(child, parent) { for (var key in parent) { if (__hasProp.call(parent, key)) child[key] = parent[key]; } function ctor() { this.constructor = child; } ctor.prototype = parent.prototype; child.prototype = new ctor(); child.__super__ = parent.prototype; return child; };

  Annotator.Plugin.AnnotatorViewer = (function(_super) {
    __extends(AnnotatorViewer, _super);

    AnnotatorViewer.prototype.events = {
      'annotationsLoaded': 'onAnnotationsLoaded',
      'annotationCreated': 'onAnnotationCreated',
      'annotationDeleted': 'onAnnotationDeleted',
      'annotationUpdated': 'onAnnotationUpdated',
      ".annotator-viewer-delete click": "onDeleteClick",
      ".annotator-viewer-delete mouseover": "onDeleteMouseover",
      ".annotator-viewer-delete mouseout": "onDeleteMouseout",
    };


    AnnotatorViewer.prototype.field = null;

    AnnotatorViewer.prototype.input = null;

    AnnotatorViewer.prototype.options = {
      AnnotatorViewer: {}
    };


    function AnnotatorViewer(element, options) {
      this.onAnnotationCreated = __bind(this.onAnnotationCreated, this);
      this.onAnnotationUpdated = __bind(this.onAnnotationUpdated, this);
      this.onDeleteClick = __bind(this.onDeleteClick, this);
      this.onDeleteMouseover = __bind(this.onDeleteMouseover, this);
      this.onDeleteMouseout = __bind(this.onDeleteMouseout, this);
      AnnotatorViewer.__super__.constructor.apply(this, arguments);

      $( "body" ).append( this.createAnnotationPanel() );

      $(".container-anotacions").toggle();
      $("#annotations-panel").click(function(event) {
        $(".container-anotacions").toggle("slide");       
      });
     

    };

    AnnotatorViewer.prototype.pluginInit = function() {
      if (!Annotator.supported()) {
        return;
      }
      
      $('#type_share').click(this.onFilter);
      $('#type_own').click(this.onFilter);
     
    };

    /*
    Check the checkboxes filter to search the annotations to show.
    Shared annotations have the class shared
    My annotations have the me class
    */
    AnnotatorViewer.prototype.onFilter = function(event) {
      var annotations_panel = $(".container-anotacions").find('.annotator-marginviewer-element');
      $(annotations_panel).hide();  

      var class_view = "";
     
      var checkbox_selected = $('li.filter-panel').find('input:checked'); 
      if (checkbox_selected.length > 0) {
          $('li.filter-panel').find('input:checked').each(function () {
            class_view += $(this).attr('rel') + '.';
          });     
          $('.container-anotacions > li.' + class_view.substring(0,class_view.length-1)).show();
      } else {
        $(annotations_panel).show();   
      }
      

    };

    AnnotatorViewer.prototype.onDeleteClick = function(event) {
      return this.onButtonClick(event, 'delete');
    };

    AnnotatorViewer.prototype.onButtonClick = function(event, type) {
      var item;
      item = $(event.target).parents('.annotator-marginviewer-element');
      return this.annotator.deleteAnnotation(item.data('annotation'));
    };

     AnnotatorViewer.prototype.onDeleteMouseover = function(event) {
       $(event.target).attr('src', '../src/img/papelera_over.png');
    };

     AnnotatorViewer.prototype.onDeleteMouseout = function(event) {
      $(event.target).attr('src', '../src/img/icono_eliminar.png');      
    };

    AnnotatorViewer.prototype.onAnnotationCreated = function(annotation) { 

      this.createReferenceAnnotation(annotation);
      $('#count-anotations').text($(".container-anotacions").find('.annotator-marginviewer-element').length );
    };

    AnnotatorViewer.prototype.onAnnotationUpdated = function(annotation) {

      $( "#annotation-"+annotation.id ).html( this.mascaraAnnotation(annotation) );       
    };

    AnnotatorViewer.prototype.onAnnotationsLoaded = function(annotations) {
      var annotation;
      $('#count-anotations').text( $(".container-anotacions").find('.annotator-marginviewer-element').length );
      if (annotations.length > 0) {
        for(i=0, len = annotations.length; i < len; i++) {
          annotation = annotations[i];
          this.createReferenceAnnotation(annotation);   
        }
        
      }
      
    };

    
    AnnotatorViewer.prototype.onAnnotationDeleted = function(annotation) {
      
      $( "li" ).remove( "#annotation-"+annotation.id );      
      $('#count-anotations').text( $(".container-anotacions").find('.annotator-marginviewer-element').length );
      
    };

    AnnotatorViewer.prototype.mascaraAnnotation = function(annotation) {     
      if (!annotation.data_creacio) annotation.data_creacio = $.now();

      var anotacio_compartida = "";      
      var class_label = "label";
      var borrar = "<img src=\"../src/img/icono_eliminar.png\" class=\"annotator-viewer-delete\" title=\""+ i18n_dict.Delete +"\" style=\" float:right;margin-top:3px;\"/>";
      
      if (annotation.estat==1 || annotation.permissions.read.length===0 ) {
        anotacio_compartida = "<img src=\"../src/img/Compartido.png\" title=\""+ i18n_dict.share +"\" style=\"margin-left:5px\"/>"
      }

      if (annotation.propietary==0) {
        class_label = "label-compartit";
        borrar="";
        }

        //If you have instal.led a plug-in for categorize anotations, panel viewer can get this information with the category atribute
      if (annotation.category != null) {
        anotation_color = annotation.category;
      } else {
        anotation_color = "hightlight";
      }
      var textAnnotation = this.removeTags('iframe',annotation.text);
      var annotation_layer =  '<div class="annotator-marginviewer-text"><div class="'+anotation_color+' anotator_color_box"></div><div class="anotador_text">'+  textAnnotation  + '</div></div><div class="annotator-marginviewer-text">'+ $.format.date(annotation.data_creacio, "dd/MM/yyyy HH:mm:ss") + '</div><div class="annotator-marginviewer-quote">'+ annotation.quote + '</div><div class="annotator-marginviewer-footer"><span class="'+class_label+'">' + annotation.user + '</span>'+anotacio_compartida+borrar+'</div>';
      return annotation_layer;
    };

    AnnotatorViewer.prototype.createAnnotationPanel = function(annotation) {     
      var checboxes = '<label class="checkbox-inline"><input type="checkbox" id="type_own" rel="me"/>My annotations</label><label class="checkbox-inline">  <input type="checkbox" id="type_share" rel="shared"/>Shared</label>';
      var annotation_layer =  '<div  class="annotations-list-uoc" style="background-color:#ddd;"><div id="annotations-panel"><span class="rotate" title="'+ i18n_dict.view_annotations +' '+ i18n_dict.pdf_resum +'" style="padding:5px;background-color:#ddd;position: absolute; top:10em;left: -50px; width: 155px; height: 110px;cursor:pointer">'+ i18n_dict.view_annotations +'<span class="label-counter" style="padding:0.2em 0.3em;float:right" id="count-anotations">0</span></span></div><div id="anotacions-uoc-panel" style="height:80%"><ul class="container-anotacions"><li class="filter-panel">'+checboxes+'</li></ul></div></div>';
      return annotation_layer;
    };

   
    AnnotatorViewer.prototype.createReferenceAnnotation = function(annotation) {     
     var anotation_reference = null;
     var data_owner = "me";
     var data_type = "";

      if (annotation.id != null) {
        anotation_reference = "annotation-"+annotation.id;
      } else {
        annotation.id = this.uniqId();
        //We need to add this id to the text anotation
        $element = $('span.annotator-hl:not([id])');
        if ($element) {
          $element.prop('id',annotation.id);
        }
        anotation_reference = "annotation-"+annotation.id;
      }

      if (annotation.estat==1 || annotation.permissions.read.length===0 ) data_type = "shared";
      if (annotation.propietary==0) data_owner = "";

      var annotation_layer =  '<li class="annotator-marginviewer-element '+data_type+' '+data_owner+'" id="'+anotation_reference +'">'+this.mascaraAnnotation(annotation)+'</li>';
      var malert = i18n_dict.anotacio_lost

      anotacioObject = $(annotation_layer).appendTo('.container-anotacions').click(function(event) {
          var viewPanelHeight = jQuery(window).height();
          var annotation_reference = annotation.id;

          $element= jQuery("#"+annotation.id); 
          if (!$element.length) {
            $element= jQuery("#"+annotation.order);   
            annotation_reference = annotation.order; //If exists a sorted annotations we put it in the right order, using order attribute
          }

          if ($element.length) {
            elOffset = $element.offset();
            $(this).children(".annotator-marginviewer-quote").toggle();
            $('html, body').animate({
              scrollTop: $("#"+annotation_reference).offset().top - (viewPanelHeight/2)
            }, 2000);
          } 
      })
      .mouseover(function() {
        $element = jQuery("span[id="+annotation.id+"]");
        if ($element.length) {
          $element.css({
            "border-color":"#000000",
            "border-width":"1px",
            "border-style":"solid"});
        }
      })
      .mouseout(function() {
        $element = jQuery("span[id="+annotation.id+"]");
        if ($element.length) {
          $element.css({
            "border-width":"0px"});
        }
      });

      
      //Adding annotation to data element for delete and link
      $('#'+anotation_reference).data('annotation', annotation);
      $(anotacioObject).fadeIn('fast');
    };

    AnnotatorViewer.prototype.uniqId = function() {
      return Math.round(new Date().getTime() + (Math.random() * 100));
    } 

     //Strip content tags
    AnnotatorViewer.prototype.removeTags = function(striptags, html) { 
      striptags = (((striptags || '') + '').toLowerCase().match(/<[a-z][a-z0-9]*>/g) || []).join(''); 
      var tags = /<\/?([a-z][a-z0-9]*)\b[^>]*>/gi,commentsAndPhpTags = /<!--[\s\S]*?-->|<\?(?:php)?[\s\S]*?\?>/gi;
      
      return html.replace(commentsAndPhpTags, '').replace(tags, function($0, $1) {
        return html.indexOf('<' + $1.toLowerCase() + '>') > -1 ? $0 : '';
      });
    };  

  

    return AnnotatorViewer;

  })(Annotator.Plugin);

}).call(this);
