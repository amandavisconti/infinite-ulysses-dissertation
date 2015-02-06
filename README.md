*Welcome to the infinite-ulysses-public repository!*

#Repository Contents: Top-Level Folders
---------------------
1. **About the Project** ([manifest of work units and rationale](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/About%20the%20Dissertation/MANIFEST.md), [description of formats in final submissions to the dissertation committee and registrar](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/About%20the%20Dissertation/UMBRELLA.md), [discussion of project scope](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/About%20the%20Dissertation/SCOPE.md))
2. **[The Code](https://github.com/amandavisconti/infinite-ulysses-public/tree/master/The%20Code)** (code allowing Infinite Ulysses' interface look and behavior)
3. **Technical Instructions** ([notes on syncing multiple repos](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/Technical%20Instructions/RepoSyncingNotes.md), [how to install and configure annotation functionality on Drupal](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/Technical%20Instructions/DrupalAnnotationSetUp.md))

#Table of Contents
---------------------
* [What's Infinite Ulysses?](#whats-infinite-ulysses)
  * [Milestones](#milestones)
* [Provenance of included code](#provenance-of-included-code)
  * [Functionality](#functionality)
  * [Appearance](#appearance)
  * [The Text](#the-text)
  * [Support credits](#support-credits)
* [I want to use this!](#i-want-to-use-this)
  * [Documentation](#documentation)
  * [Issues queue](#issues-queue)
  * [Caveat emptor: in-progress code](#caveat-emptor-in-progress-code)

#What's Infinite Ulysses?
This repo contains the code I created, modified, or otherwise built off to make [the Infinite Ulysses digital edition](http://www.infiniteulysses.com). Check out [this quick 3-minute video about the project](http://www.literaturegeek.com/digital-dissertation/) to learn more about the site and the resarch project behind it:

<a href="http:/player.vimeo.com/video/92430744" target="_blank"><img src="https://raw.githubusercontent.com/amandavisconti/infinite-ulysses-public/master/Miscellaneous/videoscreenshot.png" 
alt="3-minute video explaining the Infinite Ulysses project" width="500" height="375" border="0" /></a>

This repo will eventually offer Drupal modules, site theming, documentation, and an installation profile to quickly pop up your own participatory digital edition.

Infinite Ulysses is a dissertational project of designing, building, and user-testing a "participatory digital edition" of James Joyce's difficult but unbelievably rewarding novel Ulysses—a website where you can read the novel, annotate it (highlight words in the text and add comments on them) to ask questions, provide answers and interpretations and definitions, and otherwise collaboratively explore and enrich the novel. Infinite Ulysses builds off of a ton of great FOSS (free and open-source) coding work focusing around the "Annotator.js" tool (see the Provenance for more specific credits), with a focus on packaging these into a Drupal site where existing and custom social modules can act on the annotations (ranking them, tagging them, curating them, etc.). Check out my research blog [literaturegeek.com](http://www.literaturegeek.com) for regular blogging on the project's progress.

##Milestones
- [x] Alpha-testing: Open site to committee, Ulysses Seen, friends (November-December 2014)
- [x] Beta testing I (individual volunteers; January 2015)
- [x] Beta testing II (groups, classes, book clubs; Late January 2015 throughout Spring 2015)
- [ ] Article draft providing project narrative (text and website; available by May 2015)
- [ ] Site 1.0 release on Bloomsday 2015 (June 16, 2015)
- [ ] Installation profile for easy set-up of a site with all the above implemented (Summer 2015?)
- [ ] Code/set-up documentation and thoroughly comment code for understanding by others (Summer 2015?)

#Provenance of included code

##Functionality

###Annotator.js
**Annotator.js is the foundation for the site's highlight-and-annotate functionality.** From [the Annotator.js website](http://annotatorjs.org/): "Annotator is an open-source JavaScript library to easily add annotation functionality to any webpage. Annotations can have comments, tags, links, users, and more. Annotator is designed for easy extensibility so its a cinch to add a new feature or behaviour. Annotator also fosters an active developer community with contributors from four continents, building 3rd party plugins allowing the annotation of PDFs, EPUBs, videos, images, sound, and more... Annotator is developed by a wide range of [contributors](https://github.com/openannotation/annotator/graphs/contributors) and supported by [a number of organizations](http://annotatorjs.org/showcase.html). Previous financial support was provided by [the Shuttleworth Foundation](https://www.shuttleworthfoundation.org/) and [the Open Knowledge Foundation](https://okfn.org/)."

###Annotation Drupalization
The current Drupal modules allow Annotator.js to work on Drupal page elements and save to the Drupal database. 
* [Annotation](https://www.drupal.org/project/annotation]) module: [contributors](https://www.drupal.org/node/3220/committers?sort=desc&order=Commits)
* [Annotator](https://www.drupal.org/project/annotator) module: [contributors](https://www.drupal.org/node/1960282/committers)

Initial configuration on a Drupal site is currently a bit involved. Tags can be added to annotations, but not saved/stored (they're lost on page refresh). Admins can't interact with the Annotation entity via the Field UI (not fieldable), among other issues with how annotations are set up as entities.

###Better Annotation Drupalization
I'm currently using improved versions of these two modules, created by Michael Widner (@mwidner) of Stanford's Lacuna Stories project and graciously shared with me (yay digital humanities community!). That code is not yet shared publicly, as the Lacuna Stories project has not yet made their code public (but plans to do so). Until that code is made public, it won't appear in this repository. 

Where I've made changes to Michael's code, I'll include a note in the repository that records my work without revealing his code.

###Highlighted Anno(tation)s Module
"highlighted_annos" is a custom module I created to further modify the Annotator.js Drupalization for Infinite Ulysses' needs; the module updates what annotations are displayed in a sidebar when you click on a particular instance of highlighted text. 

By replacing the Annotator.js pop-up annotation display with a Views (Drupal SQL query module) block, I was able to alter both the design and functionality of the annotation feature (e.g. adding display of and interaction with a rating widget, commenting on annotations, tagging by users other than the annotation's author with separation of "my tags" from "all tags" on a given annotation for the current logged-in user). I was also able to improve the display of annotations when there were multiple annotations for a given highlight, adding an option to sort displayed annotations by highest- or lowest-rated, oldest or newest date, or most commented-on.

The module includes three files:
* highlighted_annos.info file: Contains basic module metadata (e.g. Drupal version, author)
* highlighted_annos.js: A 22-line jQuery (a Javascript library) file; handles the user's input (the "client side" code handling everything that happens in your browser before information is sent to my website's server)
* highlighted_annos.module file: A 67-line PHP file handling information once it's passed from the "client side" to my website's server. Accomplishes tasks including:
  * structuring the form that takes as input the ID number (NID) of the annotation(s) related to the highlight the user clicked
  * get that input and convert it into a format that works in a database query ("database, please find only those annotations matching these ID numbers and then return some specific information about each annotation")
  * prevent the webpage from redirecting when clicking on a highlight triggers the inputting of the related annotation IDs to the invisible form

Peter Schuelke's [post](http://www.metaltoad.com/blog/passing-multiple-values-through-exposed-filter) was especially helpful in creating this module. 

This module is dependent on changes I made to Annotator.js (see below).

##Appearance
* Custom child theme built off the Drupal Bootstrap theme framework (using [Kalatheme](https://www.drupal.org/project/kalatheme))
* Drupal layout modules: [Panoply](https://www.drupal.org/project/panopoly), [Radix](https://www.drupal.org/project/radix), [Panels](https://www.drupal.org/project/panels)
*[Flat UI kit](https://designmodo.github.io/Flat-UI/) by Designmodo
*The background photograph (on pages except the book pages) was taken by me, courtesy of the Special Collections and University Archives at the University of Victoria Libraries. I've released it as CC0, so you can do whatever you'd like with it (but please download it, don't hotlink!).  

##The Text
Typographical formatting, proofing and error correction, and data formatting on the digital text of Ulysses. This site builds off the digital plaintext transcription of the 1922 first printing of Ulysses, transcribed by Matthew Kochis and Patrick Belk with the Modernist Versions Project (MVP) and offered for public reuse by the MVP under a CC BY SA license. I've added corrections to errors I located in that digital transcription, used regular expressions to reintroduce some basic formatting (e.g. indent and em dash before lines of dialogue), and manually am adding HTML/CSS as I read through the text to reintroduce extremely important typographical choices used in the 1922 first printing of the novel (as verified against the Modernist Versions Project's digital images of the printing; decisions such as the all-caps headlines in the Aeolus episode).

The most up-to-date list of textual corrections can be found on the [Text page](http://www.infiniteulysses.com/content/text) on Infinite Ulysses. I've also created [a public repository](https://github.com/amandavisconti/ulysses) offering the text of Ulysses as HTML files (with my added typographical formatting and error correction) and files suitable for import into a Drual site (CSVs and .import/.export files).

##Support credits
* **The Maryland Institute for Technology in the Humanities (MITH) Winnemore Digital Dissertation Fellowship** (2014-2015)
* Editing Modernism in Canada (EMiC) Doctoral Fellowship (2013-2014)
* University of Maryland English Department Summer Fellowship (2014)
* University of Maryland University Fellowship (2010-2013)
* Funding as a graduate research assistant and course instructor from the University of Maryland English Department (Summer 2011 and Winter 2012), iSchool (2010-2011), Digital Cultures and Creativity Honors College (Fall 2012), and MITH (Summer 2011-Winter 2013, Fall 2013-Spring 2014)
    
#I want to use this!
You can! That is, you will be able to, easily, once the code is polished and documented (and a piece of the code provided by a colleague is made public). This is a dissertational project, and some of the work ater I've successfully defended will be packaging the code and creating user-friendly documentation so that others may make their own copy of the site (if that floats your boat) or otherwise mimic or build off this work. There will be many updates to the code over the next year (and beyond), as well as user-testing (both analysis and raw data to be made public). You may wish to check out [my page on Infinite Ulysses IP & Copyright](http://www.infiniteulysses.com/content/ip-and-copyright) to understand what you can reuse (and how).

##Documentation
This will eventually link you to resources to turn this repo into a participatory digital edition site of your own (probably over Summer 2015).

## Issues queue
You'll notice there's already a bunch of items in [this repo's issue queue](https://github.com/amandavisconti/infinite-ulysses-public/issues); I'm currently keeping most of my tasks private in Basecamp, but I'll be moving any remaining tasks here after the public site release in June 2015. 

You can submit issues to tell me about bugs on the site or feature requests, or otherwise contact me about the [InfiniteUlysses.com](http://www.InfiniteUlysses.com) part of my dissertation. You might notice issues get tagged with various labels—this helps me do things like prioritize work on anything blocking use/accessibility/inclusion on the site, or keep track of good suggestions and requests that I won't have time to address during the scope of my dissertation. The "out of scope" label doesn't mean your report isn't useful—any feedback is useful! I'll just only be able to do so much while trying to finish my PhD in 2015, which means data clean-up, analysis, and drafting a scholarly article on the project as well as trying to make it a better experience for our reading community. You can check out [this page on the scope of my project](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/About%20the%20Dissertation/SCOPE.md) to understand what I'll be working on beforemy dissertation defense.

# Caveat emptor: in-progress code
This is the public repo for code related to the Infinite Ulysses participatory digital edition, the focus of Amanda Visconti's (@amandavisconti) digital humanities doctoral dissertation. Watch a quick 3-minute video or read more about Infinite Ulysses [here](http://www.literaturegeek.com/digital-dissertation/) or [sign up to beta-test the site](http://goo.gl/qtcy6).

**WARNING** This repo represents code that is likely to be in-progress, outdated, insecure, strange, silly, or otherwise unwieldy, in an attempt to share the entire building process with the public. I don't currently recommend using any of it, and I cannot currently provide assistance with using any of this code. Please wait for Summer 2015, when I'll provide friendly documentation on creating your own participatory digital edition and also abstract the code from Infinite Ulysses to be used with any text.

If you'd like to be notified when an easy-to-use code+documentation package is available, please [use the sign up form here](http://www.infiniteulysses.com/#signup) and indicate your interest in the comments.
