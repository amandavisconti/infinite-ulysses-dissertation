infinite-ulysses-public
=======================

Table of Contents
---------------------
 * Caveats and Provisions 
 * What's this repo about?
 * What's Infinite Ulysses?
 * Provenance of included code
 * I want to use this!
 * Documentation (not yet complete)

# Caveats and Provisions
This is the public repo for code related to the Infinite Ulysses participatory digital edition, the focus of Amanda Visconti's (@amandavisconti) digital humanities doctoral dissertation. Watch a quick 3-minute video or read more about Infinite Ulysses [here](http://www.literaturegeek.com/digital-dissertation/) or (sign up to beta-test the site)[http://goo.gl/qtcy6].

*WARNING* This repo represents code that is likely to be in-progress, outdated, insecure, strange, silly, or otherwise unwieldy, in an attempt to share the entire building process with the public. I don't currently recommend using any of it, and I cannot currently provide assistance with using any of this code. Please wait for late Spring 2015, when I'll provide clean, concise code and friendly documentation on creating your own participatory digital edition. 

If you'd like to be notified when an easy-to-use code+documentation package is available, please (use the beta-testing sign up form here)[http://goo.gl/qtcy6] and indicate your interest in the comments.

#What's this?
This repo contains the code you need to make your Drupal installation mimic or build off the behavior of the Infinite Ulysses site; see [http://www.literaturegeek.com/digital-dissertation/](especially this quick 3-minute video about the project) to read more about the eventual site. This repo will eventually offers 3+ Drupal modules and a Javascript library:

* Custom code built off [https://drupal.org/project/annotation](Annotation)
* Custom code built off [https://www.drupal.org/project/annotator](Annotator)
* Annotator.js and CSS (to be stored in /sites/all/libraries): [https://github.com/openannotation/annotator/releases](latest code here), [http://annotatorjs.org](main website here)

Plus additional modules to add behavior to the site (e.g. social ranking on the annotations):
* An in-progress sidebar module for viewing and ranking annotations

Plus the accessible Drupal theme I'll create to effect the interface of InfiniteUlysses.com.
* (Theme not yet released to public)

#What's Infinite Ulysses?
Infinite Ulysses is a dissertational project of designing, building, and user-testing a "participatory digital edition" of James Joyce's difficult but unbelievably rewarding novel Ulysses—a website where you can read the novel, annotate it (highlight words in the text and add comments on them) to ask questions, provide answers and interpretations and definitions, and otherwise collaboratively explore and enrich the novel. Infinite Ulysses builds off of a ton of great FOSS (free and open-source) coding work focusing around the "Annotator.js" tool (see below for more specific credits), with a focus on packaging these into a Drupal site where existing and custom social modules can act on the annotations (ranking them, tagging them, curating them, etc.). Check out my research blog [http://www.literaturegeek.com](literaturegeek.com) for regular blogging on the project's progress, or see [http://www.literaturegeek.com/digital-dissertation](especially this quick 3-minute video about the project).

#Provenance:
##Annotator.js
From [http://annotatorjs.org/](the Annotator.js website): "Annotator is an open-source JavaScript library to easily add annotation functionality to any webpage. Annotations can have comments, tags, links, users, and more. Annotator is designed for easy extensibility so its a cinch to add a new feature or behaviour. Annotator also fosters an active developer community with contributors from four continents, building 3rd party plugins allowing the annotation of PDFs, EPUBs, videos, images, sound, and more... Annotator is developed by a wide range of [https://github.com/openannotation/annotator/graphs/contributors](contributors) and supported by [http://annotatorjs.org/showcase.html](a number of organizations). Previous financial support was provided by [https://www.shuttleworthfoundation.org/](the Shuttleworth Foundation) and [https://okfn.org/](the Open Knowledge Foundation)."

##Annotation Drupalization
The current Drupal modules allow Annotator.js to work on Drupal nodes and save to the Drupal database. 
* [https://www.drupal.org/project/annotation](Annotation) module: [https://www.drupal.org/node/3220/committers?sort=desc&order=Commits](contributors)
* [https://www.drupal.org/project/annotator](Annotator) module: [https://www.drupal.org/node/1960282/committers](Contributors)
Initial configuration on a Drupal site is currently a bit involved. Tags can be added to annotations, but not saved/stored (they're lost on page refresh). Admins can't interact with the Annotation entity via the Field UI (not fieldable), among other issues with how annotations are set up as entities.

I'm currently using customizations of these two modules graciously provided by a digital humanist working on a related project to solve these issues; that code is not yet shared here, as the collaborator has not yet made their code public (but plans to do so with a FOSS license).

##Future stuff
- [ ] Alpha-testing: Open site to committee, Ulysses Seen, friends (November-December 2014)
- [ ] Beta testing I (individual volunteers; December 2014)
- [ ] Beta testing II (groups, classes, book clubs; January 2015 on)
- [ ] Installation profile for easy set-up of a site with all the above implemented (available by May 2015)
- [ ] Code/set-up documentation and thoroughly comment code for understanding by others (available by May 2015)
- [ ] Article draft providing project narrative (text and website; available by May 2015)
- [ ] Site 1.0 release on Bloomsday 2015

#I want to use this!
You can! Once the code is polished and documented (and a piece provided by a colleague is published). This is a dissertational project, and some of the work at the end of the tunnel (Spring 2015) will be packaging the code and creating user-friendly documentation so that others may make their own copy of the site (if that floats your boat) or otherwise mimic or build off this work. They'll be many updates to the code over the next year (and beyond), as well as user-testing (both analysis and raw data to be made public).

##Documentation (not yet complete)
This will eventually link you to resources to turn this repo into a participatory digital edition site of your own!