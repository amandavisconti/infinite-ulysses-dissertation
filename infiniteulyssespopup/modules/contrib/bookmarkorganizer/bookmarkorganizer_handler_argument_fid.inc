<?php

/**
 * @file
 * Contains the folder ID argument handler.
 */

/**
 * Handler to accept an argument of the folder (parent ID) of a bookmark.
 *
 * @ingroup views
 */
class bookmarkorganizer_handler_argument_fid extends views_handler_argument_numeric {
  function query($group_by = FALSE) {

    $join = new views_join;
    $join->construct($this->table, 'node', 'nid', 'content_id');

    $alias = $this->query->add_relationship($this->table, $join, 'node');

    $this->query->add_where(0, "$this->table.type", "folder", "!=");

    $this->query->add_where(0, "$this->table.parent", $this->argument, "=");

  }
}