
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Features
 * Requirements
 * Installation
 * Similar Modules


INTRODUCTION
------------

The Book helper module improves Drupal's core book module's functionality.

Drupal's Book module, included in core, does a great job at building custom site
sections. The Book helper module enables additional features from Drupal's menu
system to improve the ordering of a book's pages. This module also allows
administrators to further customize a book's navigation block.


FEATURES
--------

- Enhances the book outline manager.
  - Allows a book's link title to be different then the node's title.
    This behavior is similar the menu system's default behavior.
  - Adds 'enable' checkbox (aka hidden) to the manage book outline page.

- Adds an 'Order' tab (node/%/order) to a book's main page.

- Adds 'administer own book outlines' permission for ordering books from a book's main page.

- Removes the 'Outline' tab from all nodes.

- Removes the book fieldset from node edit pages if the content type does not
  allowed book outlines.

- Removes 'add child page' and 'printer-friendly version'

- Removes book navigation

- Customizes visibility of a book's navigation elements.

- Automatically 'Create new book' for selected content types.

- Includes 'Book (inline) navigation' block that can be easily positioned after
  a node's content

- Disables new revisions from being created when a book's node title is updated.

- Displays a warning when deleting a book page (node/%/delete) that has child
  pages.

- Allows top-level pages to be removed from books.
  http://drupal.org/node/283045

- Keeps a book's main page's menu link active while navigating to lower-level
  book pages.


REQUIREMENTS
------------

- Book (core): Allows users to structure site pages in a hierarchy or outline.
  http://drupal.org/documentation/modules/book


INSTALLATION
------------

1. Copy/upload the book_helper.module to the sites/all/modules directory
   of your Drupal installation.

2. Enable the 'Book helper' module in 'Administer > Site building > Module'.
   (admin/build/modules)

3. Configure additional Book settings (admin/content/book/settings)


SIMILAR MODULES
---------------

- Book made simple: Automatic creation of a book and simple creation of child
  pages.
  http://drupal.org/project/BookMadeSimple

- Menu Editor enhances the menu editing form with inline text fields for title,
  path and description, and provides placeholders for new items.
  http://drupal.org/project/menu_editor


AUTHOR/MAINTAINER
-----------------

- Jacob Rockowitz
  http://drupal.org/user/371407
