Annotator.js works on Drupal as two modules and a library (installed, as usual, under /sites/all):
    /modules/annotator
    /modules/annotation
    /libraries/annotator
The files in libraries/annotator are the latest from the Annotator.js repo (https://github.com/openannotation/annotator/releases). Although the modules Infinite Ulysses uses have been significantly changed from the modules you could download on Drupal.org, the Drupal.org Annotator (https://www.drupal.org/project/annotator) and Annotation (https://www.drupal.org/project/annotation) modules pages might still have useful info if you're troubleshooting. 

Instructions:
I'll refer to the two modules as "Annotator" and "Annotation" throughout, and use "libraries/annotator" if I'm referring to the JS library.

1. Rename /libraries/annotator folder as just "annotator" and place in your sites/all/libraries folder (create a libraries folder there if you don't already have one).
2. Install the two modules as usual (sites/all/modules) and enable both on the modules admin page.
3. Set permissions for Annotation (admin/people/permissions#module-annotation) and Annotator (http://www.infiniteulysses.com/admin/people/permissions#module-annotator). The Annotator permissions are where you can choose which roles can view, create, and edit the annotations.
4. Configure the Annotator module (admin/config/content/annotator). I recommend turning on the following plugins all at the same time:
    Store (lets you store annotations to the Drupal database)
    Unsupported (gives users a useful message if their browser doesn't support annotation—which probably means their JS is turned off)
    Filter (adds the filtering bar for navigating from one annotation to the next and filtering by tags, user, and annotation text—note that permissions and tags plugins must also be turned on at the same time for these to appear in the filter bar, so you're good if you turn on all five plugins I've bulleted here at the same time)
    Permissions (allows filtering by author of annotation and permission access)
    Tags (saves tags added to annotations to the Drupal database in a taxonomy, allows filtering by tags)
5. Under "Plugin Settings" and under "Annotator Element", write the element you want annotation to work on (i.e. if you highlight text in that element, you'll get the Annotator pop-up). I have mine set to .node-type-book .col-md-8 to only apply to one section on "book page" node types. You may also need to change ".node" in line 18 of annotator/annotator.module to your preferred element if entering it using the admin page isn't working.
6. Under "Plugin Settings", you might need to play around a bit with the path under "Prefix". /annotation/api has worked for me on my live servers, and /www.infiniteulysses.com/annotation/api is working for me on a MAMP site at localhost:8080/www.infiniteulysses.com. You should be able to enter the path after your site's URL (e.g. www.infiniteulysses.com/annotation/api) and see "{"name":"Annotator Store API","version":"1.2.9"}".
7. Under "Plugin Settings", the two checkboxes under Permissions let you allow an annotation's author to specify permissions for a particular annotation (I'm not using these).
8. If you start to create annotations, you should be able to see them listed as any other node gets listed on your content admin page and in your database. You can also play around with settings at structure>content types>annotation or admin/structure/taxonomy/tags to see your annotation tags adding into your taxonomy.
9. Remember that if your page structure changes above the element getting annotated, your annotations will get messed up (won't highlight the correct words anymore). You won't lose the annotations if this happens, so you could always go into the database and figure out how to adjust the range to work with the new page structure.