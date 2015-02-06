/**
 * Finding the related entities: Helper function to retrieve all entities of a given type that reference the node.
 * @param String $type
 * @param int $nid
 */
function mymodule_entity_query($type, $nid, $reference_field_id) {
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', $type)
     ->fieldCondition($reference_field_id, 'target_id', $nid, '=')
     ->propertyOrderBy('created', 'DESC');

  return $query->execute();
}

/**
 * Displaying the entities in a collapsable fieldset:
 * Implementation of hook_node_view().
 * You could use hook_entity_view here if not displaying a node.
 */
function mymodule_node_view($node, $view_mode, $langcode) {
  // if we are viewing a full example node.
  if ($node->type == 'example && $view_mode == 'full') {
     // get all embedded_entity entities.
     $result = mymodule_entity_query('embedded_entity',  $node->nid, 'field_myentity_reference');
     if (isset($result['embedded_entity'])) {
       $ids = array_keys($result['embedded_entity']);
       $items = entity_load('embedded_entity', $ids);

       // Generate an array for rendering.
       $entities = entity_view('embedded_entity', $items, 'full');
       
       // Remove the first item as we want it outside the fieldset. 
       $first_entity = array_shift($entities);       
       $node->content['entities']['first'] = $first_entity;
       
        // Group the rest in a fieldset.
        $node->content['entities']['fset'] = array(
             '#theme' => 'fieldset',
             '#title' => t('Some title: ') . ' ' . format_date($entity['#entity']->changed),
             '#children'  => render($entities),
             '#attributes' => array(
                'class' => array('collapsible', 'collapsed'),
             ),
              // JS needed to collapse fieldset.
             '#attached' => array(
               'js' => array(
                 'misc/form.js',
                 'misc/collapse.js',
               ),
             ),
           );
       }
   }
}

/**
* Put within hook_entity_view
**/
 module_load_include('inc', 'eck', 'eck.entity');
  $form = eck__entity__add('embedded_entity', 'embedded_entity');
   // Set the required reference field so when we save the entity it is linked to the parent.
  $form['field_myentity_reference'][LANGUAGE_NONE]['#value'] = $node->nid;
  
   $node->content['entity_form'] = array(
      '#theme' => 'fieldset',
      '#title' => 'Add form',
      '#children'  => render($form),
      '#attributes' => array(
        'class' => array('collapsible', 'collapsed'),
      ),
      // JS needed to collapse fieldset.
      '#attached' => array(
        'js' => array(
            'misc/form.js',
            'misc/collapse.js',
        ),
      ),
    );


/**
 * Implementation of hook_entity_info().
 */
function mymodule_entity_info_alter(&$entity_info) {
  // We want to redirect all changes to the embedded entity 
  // back to the parent node.
  $entity_info['embedded_entity']['uri callback'] = 'mymodule_entity_redirect';
}

function mymodule_entity_redirect(&$entity) {
  $flow_id = $entity->field_myentity_reference[LANGUAGE_NONE][0]['target_id'];
  
  return array(
    'path' => 'node/' . $flow_id, // path of the parent.
  );
}
/**
* Redirecting the entity back to the parent
* This embedded entity has no value on it's own, so we make sure it is displayed in the parent.
* Implementation of hook_form_alter().
**/
function mymodule_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == ''eck__entity__form_add_embedded_entity_embedded_entity') {
    // When the entity form is submitted we want to redirect back to the parent.
    // The embedded form could be submitted via ajax with a little work and this wouldn't be needed.
    $form['#submit'][] = 'mymodule_redirect_submit';
  }
}
/**
 * Redirects the embedded entity forms to the node they belong to.
**/
function mymodule_redirect_submit($form, &$form_state) {
  $values = $form_state['values'];
  if ( isset($values['field_myentity_reference'][LANGUAGE_NONE][0]['target_id']) {
    $form_state['redirect'] = 'node/' . $values['field_myentity_reference'][LANGUAGE_NONE][0]['target_id'];
  }
}