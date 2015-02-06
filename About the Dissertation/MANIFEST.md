*I'm currently updating this file (circa February 6, 2015). More info soon!*

An ongoing manifest of the work my dissertation comprises, with explanations of the what and why behind formats unusual in literary studies. (Happy to take feedback if something isn't explained clearly enough, via [@Literature_Geek](http://www.twitter.com/Literature_Geek) or [my contact form](http://www.infiniteulysses.com/contact).)

# Thoughts:
+ Creating design wireframes, writing, code, and other web development activities are **both similar and different** to chapter-writing in terms of quantifying output:

  + Like chapter-writing drafts, with code **there's a lot of work that doesn't make it into the final product**. In my case, the final product doesn't include things like example Drupal modules I built to learn things like how to make the Annotator tool communicate with the Drupal database. 

  + **Better code is quite often more concise code** (i.e. less lines of code). This may or may not be the case with chapter-writing; in cases where you're shooting for a chapter length in some word count range (e.g. 20 pages), improving your writing is more likely to not impact the overall word count.

# Categories of work:
* [Design/appearance](#designappearance)
  
* [Code](#code)
  
* [Research writing](#research-writing)

* [Scholarly editing](#scholarly-editing)
  
* [Public humanities](#public-speaking)
  
* [Miscellaneous](#miscellaneous)

# The item manifest

------
------
##Design/appearance
Each Infinite Ulysses design and coding decison involved thinking about
* **scope**: what could I reasonably accomplish while leaving time for the other parts of my dissertational project?
* **speculative experiment**: thinking of the site as hosting thousands or millions of users and annotations, how could I build something that would structurally support that weight? how could I parse the noise of a million annotations so that each unique reader was offered just those annotations that suited her background, needs, and desires?
* **speculative experiment + scope**: how could I mediate between these two desires to support my research but also create something practically useful to a large public humanities audience?
* **theory**: what did my reading in areas such as visual design rhetoric, scholarly editing/textual scholarship, and reading behavior suggest? what did precedents in Joycean scholarship, print and digital editions, and digital humanities projects suggest?
* **online communities**: what did historical successes, working systems, organic uses of official features, and other observed phenomena at existing online communities with heavy user-created textual content (e.g. Reddit) suggest?

------
**Item:** Custom Drupal theme built off the Drupal Bootstrap theme framework (using [Kalatheme](https://www.drupal.org/project/kalatheme))

**What it does (technical, research):** A "theme" is a set of code (CSS3, HTML5, Javascript/jQuery, and PHP for page templates) that creates the look of a website. Some considerations that went into the design of the Infinite Ulysses digial edition theme:
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

**Where to find it:** Visible on [the Infinite Ulysses digital edition](http://www.infiniteulysses.com), and also stored in [this repository's "The Code" folder](https://github.com/amandavisconti/infinite-ulysses-public/tree/master/The%20Code)

------
------
##Code

**Item:** [Folder of example modules and other codework that didn't make it into the final website](https://github.com/amandavisconti/infinite-ulysses-public/tree/master/The%20Code/Old%2C%20defunct%20code%20in%20all%20states)
**What it does (technical, research):** Code I wrote that eventually developed into current code on the website—or didn't, but helped me learn how to create a module, send information from the client- to the server-side, etc.

------

**Item:** Highlighted Anno(tation)s Drupal module

**What it does (technical, research):** "highlighted_annos" is a custom module I created to further modify the Annotator.js Drupalization for Infinite Ulysses' needs; the module updates what annotations are displayed in a sidebar when you click on a particular instance of highlighted text. 

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

------

**Item:** My Modifications of Annotator.js (CSS and Javascript code)
**What it does (technical, research):** Annotator.js is the foundation for the site's highlight-and-annotate functionality; it lets you highlight text and add an annotation to it. See (this section of this repo's README)[https://github.com/amandavisconti/infinite-ulysses-public/blob/master/README.md#functionality] for more on what Annotator.js does and who was responsible for what parts of its functionality.

I committed the original Annotator.js library files, then made my next commit hold all my changes (up to 2/6/2015); this will allow you to see a nice comparison of modifications I made to the Annotator.js library by visiting [this commit page](https://github.com/amandavisconti/infinite-ulysses-public/commit/58ec43f29b4b27c41c9131ba56bd954fe3fc4064). Note that I may have made changes to these files subsequent to that commit (e.g. tidying things up a bit when time allows), but the most important code changes are visible in that commit.

1. **Annotator.min.css**
Use of the Mac FileMerge file comparison tool shows I made **21 changes** to this CSS file, including changing the mouse cursor to a hand when hovering over a highlight, browser prefixes (required to make certain browsers use a CSS rule; e.g. -moz, -webkit), removing a lot of unneeded elements (e.g. gradients on the annotation filter buttons), and most importantly removing display of the Annotator.js viewer (a pop-up that I replaced with my annotation sidebar).

2. **Annotator-full.min.js**
Use of the Mac FileMerge file comparison tool shows I made **25 changes** to this Javascript file. Many of the key modifications are marked with a comment (// or /* */) and the word "Amanda", usually followed by a short explanation. The main reasons for modifying this file were small tweaks such as removing use of the Annotator.js Viewer (the pop-up for viewing existing annotations) in favor of my sidebar, and changing behavior from on mouse hover to on mouse click. 

These were mostly one- or two-line changes; the biggest change is from lines 1847-1855, which I've commented [on this page to make them easy to look at](https://github.com/amandavisconti/infinite-ulysses-public/commit/58ec43f29b4b27c41c9131ba56bd954fe3fc4064#commitcomment-9632201). This code reacts to a user clicking on a highlighted piece of text by *adding the ID numbers (NIDs) 
*for all annotations referencing that highlight (i.e. the one or more annotations that might be associated with a highlighted piece of text) 
*into a list ("array"), 
*entering them into an invisible form field, and 
*submitting that form.
This makes the list of annotations associated with a highlight available to the website's server through my Highlighted Anno(tation)s module, described above.

**Where to find it:** 
* [The latest versions are in this repository's "The Code" folder](https://github.com/amandavisconti/infinite-ulysses-public/tree/master/The%20Code)
* [Here's a link to a comparison betwen the original Annotator.js files and most of my additions/modifications (2/5/2015); more work might have been done since then](https://github.com/amandavisconti/infinite-ulysses-public/commit/58ec43f29b4b27c41c9131ba56bd954fe3fc4064#commitcomment-9632201)
* You can see the largest chunk of code written by me [below the comment on this page](https://github.com/amandavisconti/infinite-ulysses-public/commit/58ec43f29b4b27c41c9131ba56bd954fe3fc4064#commitcomment-9632201)

------

**Item:** Server and Drupal configuration

**What it does (technical, research):** I installed and set up a server to run my website on; this included work like installing Ubuntu, MySQL (databases), and a mail server (to send beta-tester invitations), and modifications to make the server more secure. Drupal configuration consisted of using various forms on the Drupal administration pages; these are pages created by Drupal or by modules I'd installed that allow for easy, code-free customization of the site.

------
------
##Research writing
**Item:** LiteratureGeek.com blog posts

**What it does (technical, research):** A research blog with 31 posts (average length: 2,000-3,000 words) plus crosspostings to HASTAC.org plus 2 unique blog posts at mith.umd.edu. Posts are moderately polished; think somewhere between first and final draft level of writing: academic blogging is fast but careful about reaching its scholarly audience.

**Where to find it:** http://www.literaturegeek.com/category/dissertation/; http://www.hastac.org/blogs/amanda-visconti; http://mith.umd.edu/invitation-beta-test-infinite-ulysses-digital-edition/ and http://mith.umd.edu/infinite-ulysses-designing-public-humanities-conversation/

------
**Item:** Values statement

**What it does (technical, research):** Written statement of scholarly values as they apply to the dissertation.

**Where to find it:** http://www.literaturegeek.com/values/

------
**Item:** Book proposal on the digital dissertation process with University of Michigan Press [Practices in the Digital Humanities series](http://www.digitalculture.org/books/book-series/practices-in-the-digital-humanities)

**What it does (technical, research):** With co-author Dr. Kathie Gossett, book proposal, book outline, and example chapter thinking through the digital dissertation process from designing your format to fit your research questions through the defense.

**Where to find it:** More info once proposal accepted.

------
------
##Scholarly Editing
**Item:** Typographical formatting, proofing and error correction, and data formatting on the digital text of Ulysses

**What it does (technical, research):** This site builds off the digital plaintext transcription of the 1922 first printing of Ulysses, transcribed by Matthew Kochis and Patrick Belk with the Modernist Versions Project (MVP) and offered for public reuse by the MVP under a CC BY SA license. I've added corrections to errors I located in that digital transcription, used regular expressions to reintroduce some basic formatting (e.g. indent and em dash before lines of dialogue), and manually am adding HTML/CSS as I read through the text to reintroduce extremely important typographical choices used in the 1922 first printing of the novel (as verified against the Modernist Versions Project's digital images of the printing; decisions such as the all-caps headlines in the Aeolus episode).

**Where to find it:** The most up-to-date list of textual corrections can be found on the [Text page](http://www.infiniteulysses.com/content/text) on Infinite Ulysses. I've also created [a public repository](https://github.com/amandavisconti/ulysses) offering the text of Ulysses as HTML files (with my added typographical formatting and error correction) and files suitable for import into a Drual site (CSVs and .import/.export files).

------
------
##Talks and Presentations
**Item:** 4 invited talks about the dissertation:
* June 2013: Presentation to Dr. Hans Gabler's Digital Ulysses Master Class at the University of Victoria

* April 2014: [2nd Annual Nebraska Forum on Digital Humanities](http://cdrh.unl.edu/news-events/nebraskaforum)

* March 2015: [Not-yet-named panel on digital dissertations at CUNY Graduate Center](http://digitalfellows.commons.gc.cuny.edu/digital-dissertations/)

* April 2015: [MITH Digital Dialogue](http://mith.umd.edu/digitaldialogues))

**What it does (technical, research):** Written preparation for talks aimed at various audiences (Joycean, digital humanities scholars, digital dissertation students and advisors, digital humanities/public)

------
**Item:** 3 service panel talks supporting UMD English graduate students

**What it does (technical, research):** Synthesis of dissertational experience and writing of presentation notes for three panels: "What Does a 21st Century Dissertation Look Like?", "Social Media for Humanities Researchers", "Preparing for Your Ph.D. Exams"

------
**Item:** Introduction to the Infinite Ulysses Project video

**What it does (technical, research):** A three-minute overview of my research project aimed at a public audience. Condenses my research questions and translates their exigence for the public. Combines spoken text and still images.

**Where to find it:** http://vimeo.com/92430744

------
------
##Miscellaneous
**Item**: This manifest document

**What it does (technical, research):** This manifest took considerable time to create (roughly three work days); tasks included tracking down files I've altered since beginning the dissertation, figuring out how to quantify codework, and translating technical jargon to explanations free from technical vocabulary.

------
**Item:** 3 fellowship/scholarships in support of the project received (MITH Winnemore Digital Dissertation Fellowship 2014-2015, Editing Modernism in Canada Ph.D. Fellowship 2013-2014, UMD English Summer Fellowship 2014)

**What it does (technical, research):** I count scholarships/fellowships as produced items because of the significant effort in writing the proposals that led to them (although I didn't count non-successful applications).

------
**Item:** IRB Exemption

**What it does (technical, research):** Forms filled out, an example user-study survey submitted, and other hoops jumped to achieve a waiver of Institutional Review Board (IRB) oversight for my dissertational user studies.

**Where to find it:** [IRB Waiver](https://github.com/amandavisconti/infinite-ulysses-public/blob/master/Miscellaneous/IRBNetDocument.pdf)
