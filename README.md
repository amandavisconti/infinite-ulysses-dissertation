infinite-ulysses-public
=======================

Table of Contents
---------------------
 * Caveats and provisions 
 * What's this repo about?
 * What's Infinite Ulysses?
 * Provenance of included code
 * Milestones
 * Issues queue
 * I want to use this!
 * Documentation (not yet complete)
 * Support credits

# Caveats and provisions
This is the public repo for code related to the Infinite Ulysses participatory digital edition, the focus of Amanda Visconti's (@amandavisconti) digital humanities doctoral dissertation. Watch a quick 3-minute video or read more about Infinite Ulysses [here](http://www.literaturegeek.com/digital-dissertation/) or [sign up to beta-test the site](http://goo.gl/qtcy6).

*WARNING* This repo represents code that is likely to be in-progress, outdated, insecure, strange, silly, or otherwise unwieldy, in an attempt to share the entire building process with the public. I don't currently recommend using any of it, and I cannot currently provide assistance with using any of this code. Please wait for late Spring 2015, when I'll provide clean, concise code and friendly documentation on creating your own participatory digital edition. 

If you'd like to be notified when an easy-to-use code+documentation package is available, please [use the beta-testing sign up form here](http://goo.gl/qtcy6) and indicate your interest in the comments.

#What's this?
This repo contains the code you need to make your Drupal installation mimic or build off the behavior of the Infinite Ulysses site; see [especially this quick 3-minute video about the project](http://www.literaturegeek.com/digital-dissertation/) to read more about the eventual site. This repo will eventually offer Drupal modules, site theming, documentation, and customizations to Annotator.js:

* Custom code built by me off improvements by Michael Widner (@mwidner)/Stanford's Lacuna Stories project to the Drupal  [Annotation](https://drupal.org/project/annotation) module
* Custom code built by me off improvements by Michael Widner (@mwidner)/Stanford's Lacuna Stories project to the Drupal  [Annotator](https://www.drupal.org/project/annotator) module
* Custom code built by me off Annotator.js and CSS (to be stored in /sites/all/libraries): [latest code here](https://github.com/openannotation/annotator/releases), [main website here](http://annotatorjs.org)

Plus additional modules to add behavior to the site (e.g. social ranking on the annotations):
* An in-progress sidebar module for viewing and ranking annotations (included here as the partial highlighted_annos module; requires Views config rendered programmatically before use)

Plus the accessible Drupal theme I'll create to effect the interface of InfiniteUlysses.com.
* (Theme not yet released to public)

#What's Infinite Ulysses?
Infinite Ulysses is a dissertational project of designing, building, and user-testing a "participatory digital edition" of James Joyce's difficult but unbelievably rewarding novel Ulysses—a website where you can read the novel, annotate it (highlight words in the text and add comments on them) to ask questions, provide answers and interpretations and definitions, and otherwise collaboratively explore and enrich the novel. Infinite Ulysses builds off of a ton of great FOSS (free and open-source) coding work focusing around the "Annotator.js" tool (see below for more specific credits), with a focus on packaging these into a Drupal site where existing and custom social modules can act on the annotations (ranking them, tagging them, curating them, etc.). Check out my research blog [literaturegeek.com](http://www.literaturegeek.com) for regular blogging on the project's progress, or see [especially this quick 3-minute video about the project](http://www.literaturegeek.com/digital-dissertation).

#Provenance:
##Annotator.js
From [the Annotator.js website](http://annotatorjs.org/): "Annotator is an open-source JavaScript library to easily add annotation functionality to any webpage. Annotations can have comments, tags, links, users, and more. Annotator is designed for easy extensibility so its a cinch to add a new feature or behaviour. Annotator also fosters an active developer community with contributors from four continents, building 3rd party plugins allowing the annotation of PDFs, EPUBs, videos, images, sound, and more... Annotator is developed by a wide range of [contributors](https://github.com/openannotation/annotator/graphs/contributors) and supported by [a number of organizations](http://annotatorjs.org/showcase.html). Previous financial support was provided by [the Shuttleworth Foundation](https://www.shuttleworthfoundation.org/) and [the Open Knowledge Foundation](https://okfn.org/)."

##Annotation Drupalization
The current Drupal modules allow Annotator.js to work on Drupal nodes and save to the Drupal database. 
* [Annotation](https://www.drupal.org/project/annotation]) module: [contributors](https://www.drupal.org/node/3220/committers?sort=desc&order=Commits)
* [Annotator](https://www.drupal.org/project/annotator) module: [Contributors](https://www.drupal.org/node/1960282/committers)

Initial configuration on a Drupal site is currently a bit involved. Tags can be added to annotations, but not saved/stored (they're lost on page refresh). Admins can't interact with the Annotation entity via the Field UI (not fieldable), among other issues with how annotations are set up as entities.

I'm currently using improved versions of these two modules created by Michael Widner (@mwidner)/Stanford's Lacuna Stories project and graciously shared with me (yay digital humanities community!). That code is not yet shared here, as the Lacuna Stories project has not yet made their code public (but plans to do so).

More thorough crediting of FOSS licenses, previous contributors to customized modules, etc. will eventually be outlined here. When code is a bit more cleaned up, I'll try to label provenance within code as well, so that files representing multiple people's work clearly attribute each piece to its author. I've started to do that locally via comments beginning //Amanda:

# Milestones
- [ ] Alpha-testing: Open site to committee, Ulysses Seen, friends (November-December 2014)
- [ ] Beta testing I (individual volunteers; December 2014)
- [ ] Beta testing II (groups, classes, book clubs; January 2015 throughout Spring 2015)
- [ ] Installation profile for easy set-up of a site with all the above implemented (available by May 2015)
- [ ] Code/set-up documentation and thoroughly comment code for understanding by others (available by May 2015)
- [ ] Article draft providing project narrative (text and website; available by May 2015)
- [ ] Site 1.0 release on Bloomsday 2015

# Issues queue
You'll notice there's already a bunch of items in [this repo's issue queue](https://github.com/amandavisconti/infinite-ulysses-public/issues); I'm currently keeping most of my tasks private in Basecamp, but I'll be moving any remaining tasks here after I begin beta-testing with members of the public. You can submit issues to tell me about bugs on the site or feature requests, or otherwise contact me about the InfiniteUlysses.com site-part of my dissertation. You might notice issues get tagged with various labels—this helps me do things like prioritize work on anything blocking use/accessibility/inclusion on the site, or keep track of good suggestions and requests that I won't have time to address during the scope of my dissertation. The "out of scope" label doesn't mean your report isn't useful—any feedback is useful! I'll just only be able to do so much while trying to finish my PhD in 2015, which means data clean-up, analysis, and drafting a scholarly article on the project as well as trying to make it a better experience for our reading community.

#I want to use this!
You can! That is, you will be able to, easily, once the code is polished and documented (and a piece provided by a colleague is published). This is a dissertational project, and some of the work at the end of the tunnel (Spring 2015) will be packaging the code and creating user-friendly documentation so that others may make their own copy of the site (if that floats your boat) or otherwise mimic or build off this work. They'll be many updates to the code over the next year (and beyond), as well as user-testing (both analysis and raw data to be made public).

##Documentation (not yet complete)
This will eventually link you to resources to turn this repo into a participatory digital edition site of your own!

#Support credits
I'm being supported by the [Maryland Institute for Technology in the Humanities'](http://mith.umd.edu) Winnemore Digital Dissertation Fellowship this year as I complete my work on the site. My dissertation work has previously been supported by the UMD English Department (Summer 2014), MITH graduate research assistantships (Webmaster, BitCurator Usability/Documentation), and the Editing Modernism in Canada Ph.D. Fellowship.
