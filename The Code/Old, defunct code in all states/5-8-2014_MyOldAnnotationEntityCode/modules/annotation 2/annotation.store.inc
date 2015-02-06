<?php

/**
 * Annotator Store API
 * @see https://github.com/okfn/annotator/wiki/Storage
 */

/**
 * Annotation API main endpoint
 */
function annotation_api_endpoint($id = NULL) {
  $method = $_SERVER['REQUEST_METHOD'];

  switch ($method) {
    case 'GET':
      if (isset($id) && is_numeric($id)) {
        annotation_api_read($id);
      }
      else {
        annotation_api_index();
      }
      break;
    case 'POST':
      annotation_api_create();
      break;
    case 'PUT':
      annotation_api_update($id);
      break;
    case 'DELETE':
      annotation_api_destroy($id);
      break;
  }
}

/**
 * Annotation API root
 */
function annotation_api() {
  drupal_json_output(
    array(
      'name' => 'Annotator Store API',
      'version' => ANNOTATOR_VERSION,
    )
  );
}

/**
 * Annotation API index
 */
function annotation_api_index() {
  if (annotation_access('view')) {
    $records = entity_load_multiple_by_name('annotation', FALSE);
    foreach ($records as $rid => $record) {
      $records[$rid] = annotation_api_entity_prepare($record, 'read');
    }

    // Send response
    return drupal_json_output(array_values($records));
  }
  return FALSE;
}

/**
 * Annotation API create
 */
function annotation_api_create() {
  if (annotation_access('create')) {
    global $user;

    $entity = annotation_api_from_stdin();
    $entity->user = $user->uid;
    $entity->created = REQUEST_TIME;
    $entity->updated = REQUEST_TIME;

    $entity = annotation_api_entity_prepare($entity, 'create');
    entity_save('annotation', $entity);

    // Send response
    return drupal_json_output($entity);
  }
  return FALSE;
}

/**
 * Annotation API read
 */
function annotation_api_read($id) {
  if ($annotation = entity_object_load($id, 'annotation')) {
    if (annotation_access('view', $annotation)) {
      $record = entity_load_single('annotation', $id);
      $record = array_shift($record);
      $record = annotation_api_entity_prepare($record, 'read');
      // Send response
      return drupal_json_output($record);
    }
  }
  return FALSE;
}

/**
 * Annotation API update
 */
function annotation_api_update($id) {
  if ($annotation = entity_object_load($id, 'annotation')) {
    if (annotation_access('edit', $annotation)) {
      $entity = annotation_api_from_stdin();
      $entity->updated = REQUEST_TIME;
      $entity = annotation_api_entity_prepare($entity, 'update');
      entity_save('annotation', $entity);
      // Send response
      drupal_goto('annotation/api/annotations/' . $id, array(), 303);
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Annotation API destroy
 */
function annotation_api_destroy($id) {
  if ($annotation = entity_object_load($id, 'annotation')) {
    if (annotation_access('delete', $annotation)) {
      entity_delete('annotation', $id);
      // Send response
      drupal_add_http_header('Status', '204 NO CONTENT');
      return drupal_json_output(NULL);
    }
  }
  return FALSE;
}

/**
 * Annotation API search
 */
function annotation_api_search() {
  $total = 0;
  $records = array();

  if (user_access('view annotations') ) {
    if (isset($_GET['user'])) {
      $username = $_GET['user'];
      $account = user_load(array('name' => check_plain($username)));
      $uid = $account->uid;
    }
  }
  else {
    global $user;
    if (isset($user->name)) {
      $uid = $user->uid;
    }
    else {
      $output = array(
        'total' => 0,
        'rows' => array(),
      );
      return drupal_json_output($output);
    }
  }

  $limit   = isset($_GET['limit'])   ? $_GET['limit']      : 20;
  $offset  = isset($_GET['offset'])  ? $_GET['offset']     : 0;
  $id      = isset($_GET['id'])      ? $_GET['id']         : NULL;
  $created = isset($_GET['created']) ? $_GET['created']    : NULL;
  $updated = isset($_GET['updated']) ? $_GET['updated']    : NULL;
  $text    = isset($_GET['text'])    ? $_GET['text']       : NULL;
  $quote   = isset($_GET['quote'])   ? $_GET['quote']      : NULL;
  $uri     = isset($_GET['uri'])     ? $_GET['uri']        : NULL;
  $tags    = isset($_GET['tags'])    ? explode(',', $tags) : NULL;
  // TAGWORK what about GET tags?

  // Initiate query
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'annotation');

  // Add conditions to query
  if ($id) {
    $query->propertyCondition('uri', $id);
  }
  if ($uid) {
    // Use uid instead of user
    $query->propertyCondition('uid', $uid);
  }
  // TAGWORK; use tid instead of tags--added to code but later uncommented as not seeming to do anything
  /**
   if ($tags) {
    // Use tid instead of tag
    $query->propertyCondition('tags', $tid);
  }
  **/
  // End TAGWORK addition
  if ($created) {
    $query->propertyCondition('created', $created);
  }
  if ($updated) {
    $query->propertyCondition('updated', $updated);
  }
  if ($text) {
    $query->propertyCondition('text', $text, 'LIKE');
  }
  if ($quote) {
    $query->propertyCondition('quote', $quote, 'LIKE');
  }
  if ($uri) {
    $query->propertyCondition('uri', $uri);
  }
  // TAGWORK how does this work?
  if (is_array($tags)) {
    foreach($tags as $tag) {
      $query->propertyCondition('tags', '"' . $tag . '"', 'LIKE');
    }
  }

  // Get the total
  $count_query = clone $query;
  $total = $count_query->count()->execute();
  
  // Set query range
  if ($limit > 0) {
    $query->range($offset, $limit);
  }
  $result = $query->execute();

  if (isset($result['annotation'])) {
    $annotation_items_nids = array_keys($result['annotation']);
    $records = entity_load_multiple_by_name('annotation', $annotation_items_nids);
  }

  foreach ($records as $rid => $record) {
    $records[$rid] = annotation_api_entity_prepare($record, 'read');
  }

  $output = array(
    'total' => $total,
    'rows' => array_values($records),
  );
  return drupal_json_output($output);
}

/**
 * Get data from stdin
 * @see http://php.net/manual/en/features.file-upload.put-method.php
 */
function annotation_api_from_stdin() {
  $json = '';
  // PUT data comes in on the stdin stream
  $put = fopen('php://input', 'r');
  // Read the data 1 KB at a time and write to the file
  while ($chunk = fread($put, 1024)) {
    $json .= $chunk;
  }
  fclose($put);

  $entity = (object) drupal_json_decode($json);
  $entity = annotation_api_entity_prepare($entity);
  return $entity;
}

/**
 * Prepare entity for api usage
 */
function annotation_api_entity_prepare($entity, $op) {
  switch($op) {
    case 'read':
      $account = user_load($entity->uid);
      $entity->user = array(
        'uid' => $account->uid,
        'name' => format_username($account),
        'roles' => array_values($account->roles),
      );
      unset($entity->uid);

      $entity->links[] = array(
        'href' => url('annotation/' . $entity->id),
        'rel' => 'alternate',
        'type' => 'text/html',
      );
      break;
    case 'create':
      $entity->uid = $entity->user;
      unset($entity->user);
      break;
    case 'update':
      break;
  }
  return $entity;
}