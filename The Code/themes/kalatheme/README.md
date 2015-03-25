# kalatheme

[![Build Status](https://travis-ci.org/drupalprojects/kalatheme.svg?branch=master)](https://travis-ci.org/drupalprojects/kalatheme)

> Kalatheme is designed for people who want to get to work quickly so they can make things happen.


## Contents

 * Installing Kalatheme
 * Automatic Setup and Subthemes
 * Using Bootstrap
 * Creating a Subtheme
 * Key Features

### Installing Kalatheme

Unlike other Drupal themes Kalatheme requires some other modules to work
properly. Please verify you have the following before proceeding with
installation. If you fail to do as Kalatheme has requested it will bug
you about it until the ending of the world.

 - [Libraries 2.1+](https://www.drupal.org/project/libraries)
 - [Panels 3.3+](https://www.drupal.org/project/panels)
 - PHP 5.3+
 - [JQuery Update 2.x](https://www.drupal.org/project/jquery_update)
   - with JQuery version set to 1.7+
 - Optional
  - [Custom Boostrap Library](http://getbootstrap.com/)
  - [Icon API](https://www.drupal.org/project/icon)
  - [Panopoly Theme](https://www.drupal.org/project/panopoly_theme)
  - [Magic](https://www.drupal.org/project/magic)

The easiest way to satisfy all of these requirements is to just start with
[Panopoly on Pantheon.](https://drupal.org/node/2175703)

Then [install Kalatheme like any other theme.](http://drupal.org/documentation/install/modules-themes)


Some people having troubles using the Kalacustomize plugin were helped by
[Patch #5](https://drupal.org/node/2024441)
which was rolled against Panels 3.3+41-dev.

### Automatic Setup and Subthemes

Kalatheme is meant to be the base theme that is used to build more powerful
subthemes. Subthemes inherit almost all of the propoerties of their base theme
so you can reduce theme clutter and start on the 10th floor. 

Luckily, Kalatheme features a pretty neat subtheme generation tool that will
automatically set everything up for you and allow you to customize your
subtheme. You don't even need to install a Bootstrap library, Kalatheme will pull one from [Bootswatch](http://bootswatch.com/) for you!

Check out more documentation on the [Kalatheme subtheme generator GUI](https://github.com/drupalprojects/kalatheme/wiki/Setup-and-Installation)


### Using Bootstrap

Kalatheme no longer requires an installed Bootstrap library to work. Our subtheme generator does that for you. However, if you are looking to create a custom bootstrap library, here are some options:

 - To get the standard Bootstrap library, or to customize that library:
 http://getbootstrap.com/

 - If you are looking for a free and pre-made custom version of Bootstrap:
 http://bootswatch.com/

 - If you are looking to roll with a custom version of Bootstrap try out
 http://getbootstrap.com/customize/
 https://drupal.org/node/2167149

 - If you don't mind paying for a little extra:
 http://wrapbootstrap.com/

 - You can also Google for other sources if you are feeling adventerous.
Kalatheme uses the Libraries API so in order to get Bootstrap working you need
to put your Bootstrap files in sites/all/libraries/CURRENT-THEME_bootstrap. For
example, if you have a Kalatheme subtheme enabled called mytheme, you'd put
Bootstrap's files in sites/all/libraries/mytheme_bootstrap. If you have
Kalatheme set as your default theme, you'd use
sites/all/libraries/kalatheme_bootstrap.
This is so you can have differently customized installations of Bootstrap for
different themes.

Custom Bootstrap libraries can use a non-standard files scheme so you need to
make sure that your bootstrap directory looks like the following folders and
files.

```
  /CURRENT-THEME_bootstrap
  /CURRENT-THEME_bootstrap/css
  /CURRENT-THEME_bootstrap/css/bootstrap.css
  /CURRENT-THEME_bootstrap/css/bootstrap.min.css
  /CURRENT-THEME_bootstrap/fonts/
  /CURRENT-THEME_bootstrap/js/
  /CURRENT-THEME_bootstrap/js/bootstrap.js
  /CURRENT-THEME_bootstrap/js/bootstrap.min.js
```

### Upgrading Bootstrap to 3.2
If you have a Custom Bootstrap library that is older than 3.2 you may not have the URL to rebuild the library.  With 3.2, this is now added into the json file.  However, it can be done with older bootstrap json files by following [this StackOverflow post](http://stackoverflow.com/questions/20384330/reload-bootstrap-customization).

#### Important

The only actual requirement here is that either css/bootstrap.css or
css/bootstrap.min.css exist and that they both have some sort of version
information at the top like this:

```js
  /*!
   * Bootstrap v3.0.0
   *
   * Copyright 2013 Twitter, Inc
   * Licensed under the Apache License v2.0
   * http://www.apache.org/licenses/LICENSE-2.0
   */
```

Most themes have this by default and you can use the above as a basis. It is
also worth noting that while you only need boostrap.(min).css for this to "work"
you will likely be disappointed if you don't have the JS and font files as well.

If you have more files than what is listed above we recommend putting these
files in a KalaSUBtheme.

You also do not need to have the minified files to get this to work but they are
highly recommended for better performance.

### KEY FEATURES

 - Settings
   - On the settings page for Kalatheme you can [configure how you want the style
 plugin to work.](https://github.com/drupalprojects/kalatheme/wiki/Kalatheme-settings-and-config)
 - [Style Plugin](https://github.com/drupalprojects/kalatheme/wiki/Styles-Plugin)
   - When you choose to "Customize this page" using the Panels In-Place Editor you
 gain access to a bunch of customization tools provided by Kalatheme. Select the
 paintbrush on the panels pane or region you want to edit, choose "Kalacustomize" and hit next.
 - [Mobile first and responsive](https://github.com/drupalprojects/kalatheme/wiki/Responsive-Toggling)
 - [One Region Theme Nirvana](https://github.com/drupalprojects/kalatheme/wiki/Theming-with-One-Region)
 - [Panels Layouts](https://github.com/drupalprojects/kalatheme/wiki/Panels-Layouts)
 -  Accessibility
   - Kalatheme strives to provide and improve accessibiltiy for all users. [Please
 report issues](https://github.com/drupalprojects/kalatheme/issues) with the tag `accessibility`.
