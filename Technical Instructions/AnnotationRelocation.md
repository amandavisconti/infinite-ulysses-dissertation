#Fixing Annotator.js highlight locations after changing page structure

I want to make some major changes to the page structure on InfiniteUlysses.com, and changing the page structure (often) means needing to change how the path to an annotation's location is stored (you'll know this has happened if your annotation content still lives on your site, but the highlights are no longer showing up over the appropriate text). Below is an example process, followed by notes on adapting this to your own page changes.

##EXAMPLE PROCESS
Infinite Ulysses Beta (lemonsoap theme) to local Infinite Ulysses 1.0 (currently on seven theme as test case for annotation relocation)
1. Node export of annotations from live site,
2. Replace uri if it's changing (i.e. moving from local/dev/live to another).
3. Find+replace parts of the location ranges in the node export:

For moving from lemonsoap, search for 
'[{"start":"\/div[1]\/div[3]\/div[1]\/article[1]\/div[1]\/div[1]\/div[1]\/div[1]\/div[1]\/div[1]

and replace with (for local seven theme)
'[{"start":"\/div[3]\/div[2]\/div[2]\/div[1]\/div[1]\/div[1]\/div[1]\/div[1]\/div[3]\/div[1]\/div[1]\/div[1]\/div[2]\/div[1]\/div[1]

ADDITIONALLY

For moving from lemonsoap, search for
"end":"\/div[1]\/div[3]\/div[1]\/article[1]\/div[1]\/div[1]\/div[1]\/div[1]\/div[1]\/div[1]\/p[1]","endOffset"

and replace with (for local seven theme)
"end":"\/div[3]\/div[2]\/div[2]\/div[1]\/div[1]\/div[1]\/div[1]\/div[1]\/div[3]\/div[1]\/div[1]\/div[1]\/div[2]\/div[1]\/div[1]\/p[1]","endOffset"

##NOTES ON USE
These find/replace paths will obviously depend on what page structure you started and ended with. To figure out what to use for find/replace:
1. Export a single annotation from your old site (i.e. the site with the old page structure still intact) using Drupal's node export.
2. You'll now need access to your new site (i.e. the same page of text but with the new page structure; to make this easiest, I created a local site from which all existing annotations had been deleted). Highlight the same text as you did in the previous step to create a duplicate annotation. Export this single annotation from the new site using Drupal's node export.
3. If you're moving to a different URL (e.g. I was bringing annotations from my live site to one with a local URL), search for uri in the node export and alter it to fit the new site's URL.
4. Now we're going to compare the two annotation exports to figure out what needs to be changed. You'll be looking at the ranges array (begins "'ranges' => array(") to compare the text stored as "value" (just ignore the safe value that's below it). To make comparison easy, I inserted a line break in front of each \/ and then pasted these into a spreadsheet:

![Screenshot of spreadsheet used to compare annotation location paths](https://raw.githubusercontent.com/amandavisconti/infinite-ulysses-public/master/Technical%20Instructions/RangeSpreadsheetExample.png)

Using this information, you can now go to your old website and export all your annotations, use these finds/replaces on the file holding all the annotations, and import it to the new site. Your highlights should show up correctly.