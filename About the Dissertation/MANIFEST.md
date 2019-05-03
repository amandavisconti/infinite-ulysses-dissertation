*Some of these links have been broken by reorganzing the repo; those files should still exist, but you may need to hunt around [the repo](https://github.com/amandavisconti/infinite-ulysses-dissertation) to find them.*

A manifest of the work my dissertation comprises, with explanations of the what and why behind formats unusual in literary studies. A link to the final dissertation website has been added here post-defense: [Dr.AmandaVisconti.com](https://Dr.AmandaVisconti.com). This site links out to all the pieces of the submitted dissertation in an exploration-friendly manner.

Note: creating design wireframes, writing, code, and other web development activities are **both similar and different** to chapter-writing in terms of quantifying output:
  + Like chapter-writing drafts, with code **there's a lot of work that doesn't make it into the final product**. In my case, the final product doesn't include things like [example Drupal modules I built](https://github.com/amandavisconti/infinite-ulysses-dissertation/tree/master/The%20Code/Old%2C%20defunct%20code%20in%20all%20states) to learn things like how to make the Annotator tool communicate with the Drupal database. 

  + **Better code is quite often more concise code** (i.e. fewer lines of code). This may or may not be the case with chapter-writing; in cases where you're shooting for a chapter length in some word count range (e.g. 20 pages), improving your writing is more likely to not impact the overall word count.

# Dissertation efforts:
* [Design/appearance](#designappearance)
  
* [Code](#code)
  
* [Research writing](#research-writing)

* [Scholarly editing](#scholarly-editing)
  
* [Talks and presentations](#talks-and-presentations)
  
* [Miscellaneous](#miscellaneous)

# The item manifest

## Design/appearance
Each Infinite Ulysses design and coding decison involved thinking about
* **scope**: what could I reasonably accomplish while leaving time for the other parts of my dissertational project?
* **speculative experiment**: thinking of the site as hosting thousands or millions of users and annotations, how could I build something that would structurally support that weight? how could I parse the noise of a million annotations so that each unique reader was offered just those annotations that suited her background, needs, and desires?
* **speculative experiment + scope**: how could I mediate between these two desires to support my research but also create something practically useful to a large public humanities audience?
* **theory**: what did my reading in areas such as visual design rhetoric, scholarly editing/textual scholarship, and reading behavior suggest? what did precedents in Joycean scholarship, print and digital editions, and digital humanities projects suggest?
* **online communities**: what did historical successes, working systems, organic uses of official features, and other observed phenomena at existing online communities with heavy user-created textual content (e.g. Reddit) suggest?

### Custom Drupal theme built off the Drupal Bootstrap theme framework (using [Kalatheme](https://www.drupal.org/project/kalatheme))
A "theme" is a set of code (CSS3, HTML5, Javascript/jQuery, and PHP for page templates) that creates the look of a website. Some considerations that went into the design of the Infinite Ulysses digial edition theme:
* accessibility
* use-testing feedback
* research on reading practices (e.g. visual chunking)
* the type of reading experience I wanted my audience to have
* my expected audience
* behaviors/emotions I wanted to scaffold
The largest amount of research time (thinking, sketching, reading, translating drawings into code) went into questions around laying out the page. For example, I decided to divide the text into pages that corresponded with the print pages of the Ulysses edition I'm using for several reasons: 
* bookmarking
* page analytics
* loading time /speculative experiment

My theme contains 1,398 lines of CSS I authored (as of February 6, 2015). 

My theme also contains [4 template files (PHP)](https://github.com/amandavisconti/infinite-ulysses-public/tree/master/The%20Code/themes/throwaway/templates/core) that I have modified with a few lines of PHP each.

There is also a [template.php file](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/The%20Code/themes/throwaway/template.php) I authored (53 lines of PHP) that changes the display of user account information and allows the [4 other template files](https://github.com/amandavisconti/infinite-ulysses-public/tree/master/The%20Code/themes/throwaway/templates/core) to be read.

**Where to find it:** Visible on [the Infinite Ulysses digital edition](http://www.infiniteulysses.com), and also stored in [this repository's "The Code" folder](https://github.com/amandavisconti/infinite-ulysses-public/tree/master/The%20Code). 

See also [various dated screenshot files here](https://github.com/amandavisconti/infinite-ulysses-public/tree/master/Screenshots), especially comparing the current book page design with its [first](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/Screenshots/4-13-2013_1stBookPageDesigninAI.png) and [second](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/Screenshots/4-13-2014_2ndWireframeBookPageDesigninAI.jpg) wireframe iterations back in Spring 2013.

## Code

### [Folder of example modules and other codework that didn't make it into the final website](https://github.com/amandavisconti/infinite-ulysses-public/tree/master/The%20Code/Old%2C%20defunct%20code%20in%20all%20states)
Code I wrote that eventually developed into current code on the website—or didn't, but helped me learn how to create a module, send information from the client- to the server-side, etc.

### Highlighted Anno(tation)s Drupal module
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

**Where to find it:** [This repository's "The Code" folder](https://github.com/amandavisconti/infinite-ulysses-public/tree/master/The%20Code)

### My Modifications of Annotator.js (CSS and Javascript code)
Annotator.js is the foundation for the site's highlight-and-annotate functionality; it lets you highlight text and add an annotation to it. See [this section of this repo's README](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/README.md#functionality) for more on what Annotator.js does and who was responsible for what parts of its functionality.

I committed the original Annotator.js library files, then made my next commit hold all my changes (up to 2/6/2015); this will allow you to see a nice comparison of modifications I made to the Annotator.js library by visiting [this commit page](https://github.com/amandavisconti/infinite-ulysses-public/commit/58ec43f29b4b27c41c9131ba56bd954fe3fc4064). Note that I may have made changes to these files subsequent to that commit (e.g. tidying things up a bit when time allows), but the most important code changes are visible in that commit.

#### Annotator.min.css
Use of the Mac FileMerge file comparison tool shows I made **21 changes** to this CSS file, including changing the mouse cursor to a hand when hovering over a highlight, browser prefixes (required to make certain browsers use a CSS rule; e.g. -moz, -webkit), removing a lot of unneeded elements (e.g. gradients on the annotation filter buttons), and most importantly removing display of the Annotator.js viewer (a pop-up that I replaced with my annotation sidebar).

#### Annotator-full.min.js
Use of the Mac FileMerge file comparison tool shows I made **25 changes** to this Javascript file. Many of the key modifications are marked with a comment (`// or /* */`) and the word "Amanda", usually followed by a short explanation. The main reasons for modifying this file were small tweaks such as removing use of the Annotator.js Viewer (the pop-up for viewing existing annotations) in favor of my sidebar, and changing behavior from on mouse hover to on mouse click. 

These were mostly one- or two-line changes; the biggest change is from lines 1847-1855, which I've commented [on this page to make them easy to look at](https://github.com/amandavisconti/infinite-ulysses-public/commit/58ec43f29b4b27c41c9131ba56bd954fe3fc4064#commitcomment-9632201). This code reacts to a user clicking on a highlighted piece of text by  
* adding the ID numbers (NIDs)  
* for all annotations referencing that highlight (i.e. the one or more annotations that might be associated with a highlighted piece of text)  
* into a list ("array"), 
* entering them into an invisible form field, and  
* submitting that form.  
This makes the list of annotations associated with a highlight available to the website's server through my Highlighted Anno(tation)s module, described above.

**Where to find it:** 
* [The latest versions are in this repository's "The Code" folder](https://github.com/amandavisconti/infinite-ulysses-public/tree/master/The%20Code)
* [Here's a link to a comparison betwen the original Annotator.js files and most of my additions/modifications (2/5/2015); more work might have been done since then](https://github.com/amandavisconti/infinite-ulysses-public/commit/58ec43f29b4b27c41c9131ba56bd954fe3fc4064#commitcomment-9632201)
* You can see the largest chunk of code written by me [below the comment on this page](https://github.com/amandavisconti/infinite-ulysses-public/commit/58ec43f29b4b27c41c9131ba56bd954fe3fc4064#commitcomment-9632201)

### My changes to Michael Widner's Lacuna Stories code
See [this section](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/README.md#better-annotation-drupalization) of this repository's README for an explanation of Widner's code. Short version: he's vastly improved the Annotation and Annotator modules available on Drupal.org. He hasn't done a public release of the code yet, so I'm not posting it into my repo—but I will post a page of notes on the lines in his code that I further modified. These were mostly small changes to text wording, although I did fix one site-breaking issue in the annotation.install file that Widner incorporated back into his code.

**Where to find it:** https://github.com/amandavisconti/infinite-ulysses-public/blob/master/The%20Code/Changes%20to%20Lacuna%20Stories%20Private%20Code

### Drupal View exports
"Views" is an extremely useful Drupal module that gives you an user interface for creating displays of database queries. I've used it to learn how write SQL queries. 

Why would you want to query the Drupal database? All the site information is in there, so you can do things like ask the database to send annotations that were authored in the month of January by users who accessed the site more than once and have at least one piece upvoted annotation. It's the back end of the filtering options you see on the book pages' sidebar, which lets me make fine-grained requests to filter what data is shown.

**Where to find it:** [My Views exports](https://github.com/amandavisconti/infinite-ulysses-public/tree/master/The%20Code/Views%20(DB%20Query%20Results%20Display))

### Server and Drupal configuration
I installed and set up multiple servers to run my website on; this included work like installing Ubuntu, MySQL (databases), and a mail server (to send beta-tester invitations), and modifications to make the server more secure. At the time of writing (2/6/2015), I have three servers up and running: one for my live InfiniteUlysses.com site, another online server for development use, and a local MAMP server for quick testing.

Drupal configuration consisted of using various forms on the Drupal administration pages; these are pages created by Drupal or by modules I'd installed that allow for easy, code-free customization of the site.

I also spent considerable time doing web development and systems administration work with Islandora (a combination of Drupal and Fedora), but didn't ultimately build my site on that installation.

### Modules used on Infinite Ulysses
These will be linked to their information pages on Drupal.org; you can read about the latest code contributors to each there.

#### Modules I've made tiny customizations to: (e.g. applying a patch that hasn't been rolled into the current release yet; modules/custom)**
* [book_helper](https://www.drupal.org/project/book_helper)

* [community_tags](https://www.drupal.org/project/community_tags)

* [node_export](https://www.drupal.org/project/node_export)

* [rate](https://www.drupal.org/project/rate)

* [views](https://www.drupal.org/project/views)

#### Modules I'm using as packaged (modules/contrib)
Installed and configured around 50 modules over the course of working on the current Drupal site; kept about 20 on the current site.

### Accepted commits to others' code projects
I made two fixes to other people's code that they then incorporated back into their own copy of the code:  
* accepted commit to another DH GitHub repo (currently in [a private repo](https://github.com/LacunaStories/Drupal-Master/commit/a2b3d1c317625e7427de11b549a8b6e653442af3))  
* 1 accepted patch to a Drupal module (Annotation; [patch issue here](https://www.drupal.org/node/2329271))  

## Research writing
### LiteratureGeek.com blog posts
A research blog with 31 posts (average length: 2,000-3,000 words) plus crosspostings to HASTAC.org plus 2 unique blog posts at mith.umd.edu. Posts are moderately polished; think somewhere between first and final draft level of writing: academic blogging is fast but careful about reaching its scholarly audience. 11 of my LiteratureGeek.com posts have been highlighted on [Digital Humanities Now](http://digitalhumanitiesnow.org/), the main aggregator of quality written content for the digital humanites.

**Where to find it:** http://www.literaturegeek.com/category/dissertation/; http://www.hastac.org/blogs/amanda-visconti; http://mith.umd.edu/invitation-beta-test-infinite-ulysses-digital-edition/ and http://mith.umd.edu/infinite-ulysses-designing-public-humanities-conversation/

### Values statement
Written statement of scholarly values as they apply to the dissertation.

**Where to find it:** http://www.literaturegeek.com/values/

### Book proposal on the digital dissertation process with University of Michigan Press [Practices in the Digital Humanities series](http://www.digitalculture.org/books/book-series/practices-in-the-digital-humanities)  
With co-author Dr. Kathie Gossett, book proposal, book outline, and example chapter thinking through the digital dissertation process from designing your format to fit your research questions through the defense.

**Where to find it:** More info once proposal accepted.

### Dissertation prospectus
I submitted an 18-page dissertation prospectus PDF at the beginning of my dissertation, containing plans for the project, a literature review, and a bibliography.

**Where to find it:** https://github.com/amandavisconti/infinite-ulysses-public/tree/master/Miscellaneous/Prospectus

### Dissertation whitepaper
I created an analytic discussion of the dissertation project's process and product in the form of a whitepaper. This format was chosen to help me analyze my scholarly building work and provide a narrative of the project for others to read, while not doubling my labor by requiring the traditional 4-5 chapters of written work in addition to the other efforts described on this page.

**Where to find it:** http://dr.amandavisconti.com/#whitepaper

## Scholarly Editing
### Typographical formatting, proofing and error correction, and data formatting on the digital text of Ulysses
This site builds off the digital plaintext transcription of the 1922 first printing of Ulysses, transcribed by Matthew Kochis and Patrick Belk with the Modernist Versions Project (MVP) and offered for public reuse by the MVP under a CC BY SA license. I have added corrections to errors I located in that digital transcription (20 as of 2/6/2015), used regular expressions to reintroduce some basic formatting (e.g. indent and em dash before lines of dialogue), and manually am adding HTML/CSS as I read through the text to reintroduce extremely important typographical choices used in the 1922 first printing of the novel (as verified against the Modernist Versions Project's digital images of the printing; decisions such as the all-caps headlines in the Aeolus episode).

**Where to find it:** The most up-to-date list of textual corrections can be found on the [Text page](http://www.infiniteulysses.com/content/text) on Infinite Ulysses. I've also created [a public repository](https://github.com/amandavisconti/ulysses) offering the text of Ulysses as HTML files (with my added typographical formatting and error correction) and files suitable for import into a Drual site (CSVs and .import/.export files).

## Talks and Presentations
### 4 invited talks about the dissertation:
* June 2013: Presentation to Dr. Hans Gabler's Digital Ulysses Master Class at the University of Victoria

* April 2014: [2nd Annual Nebraska Forum on Digital Humanities](http://cdrh.unl.edu/news-events/nebraskaforum)

* March 2015: [Not-yet-named panel on digital dissertations at CUNY Graduate Center](http://digitalfellows.commons.gc.cuny.edu/digital-dissertations/)

* April 2015: [MITH Digital Dialogue](http://mith.umd.edu/digitaldialogues))
Written preparation for talks aimed at various audiences (Joycean, digital humanities scholars, digital dissertation students and advisors, digital humanities/public)

### 3 service panel talks supporting UMD English graduate students
Synthesis of dissertational experience and writing of presentation notes for three panels: "What Does a 21st Century Dissertation Look Like?", "Social Media for Humanities Researchers", "Preparing for Your Ph.D. Exams"

### Introduction to the Infinite Ulysses Project video
A three-minute overview of my research project aimed at a public audience. Condenses my research questions and translates their exigence for the public. Combines spoken text and still images.

**Where to find it:** http://vimeo.com/92430744

## Miscellaneous
### This manifest document and this repository
This manifest took considerable time to create (roughly two work days); tasks included tracking down files I've altered since beginning the dissertation, figuring out how to quantify codework, and translating technical jargon to explanations free from technical vocabulary. I've also worked throughout the repository on clearly documenting code provenance and usage; the [README file](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/README.md) at the front of the repository is especially thorough in explaining my project and how items in this repository tie into the project. Other notable pages are [the explanation of my project scope](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/About%20the%20Dissertation/SCOPE.md), an [overview of the deliverables](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/About%20the%20Dissertation/UMBRELLA.md) I'll submit to my committee and the registrar at my defense, and [instructions on installing and configuring annotation on Drupal sites](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/Technical%20Instructions/DrupalAnnotationSetUp.md).

### 3 fellowship/scholarships in support of the project received (MITH Winnemore Digital Dissertation Fellowship 2014-2015, Editing Modernism in Canada Ph.D. Fellowship 2013-2014, UMD English Summer Fellowship 2014)
I count scholarships/fellowships as produced items because of the significant effort in writing the proposals that led to them (although I didn't count non-successful applications).

### IRB Exemption
Forms filled out, an example user-study survey submitted, and other hoops jumped to achieve a waiver of Institutional Review Board (IRB) oversight for my dissertational user studies.

**Where to find it:** [IRB Waiver](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/Miscellaneous/IRBNetDocument.pdf)

### Affinity Wall
An affinity wall is a mind-mapping technique I learned during my iSchool degree. I used it to draft an outline for my scholarly article. See also [this blog post on how I created the affinity wall](http://www.literaturegeek.com/2014/04/28/affinitywallarticle/).

**Where to find it:** https://github.com/amandavisconti/infinite-ulysses-public/tree/master/Miscellaneous/Affinity%20Wall%20Photos%20(Article%20Outlining)

### 221 annotations to James Joyce's Ulysses authored on InfiniteUlysses.com as of 2/6/2015.
These are annotations aimed at first-time readers covering the low fruit of expected reader questions for the first two episodes of the novel. These annotations were built on annotations I'd authored on my old UlyssesUlysses.com project, but they were edited and often expanded for the new project.

### 14 site pages authored, including an overview of my research, information about IP and copyright on the site, and information on the text used for the book pages. 

**Where to find it:** All pages are available from the drop-down menu in the InfiniteUlysses.com site header.

### RSS/XML feed of annotations updating as they are added to the site for research and artistic use.
**Where to find it:** http://www.infiniteulysses.com/all-annotations-feed

### Twitter for public outreach
**Where to find it:** Not easily quantifiable, but I've used Twitter to share my progress, research blog posts, and other news related to the dissertation with others in my field to good effect. I've had about 200 people sign up to beta-test my digital edition (as of 2/6/2015), and that's probably mostly through Twitter publicity. I currently have 1,731 followers (a number that increased steadily starting when I began tweeting my dissertation three years ago). A [a dissertation tweet I made yesterday](https://twitter.com/Literature_Geek/status/563457579243016192) had 888 impressions ("number of times users saw the tweet on Twitter") on Twitter (according to Twitter Analytics), and a [January tweet](https://twitter.com/Literature_Geek/status/554713283471880192) reminding people about the 3-minute video explaining my dissertation was seen by 2,070 people.
