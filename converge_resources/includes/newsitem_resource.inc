<?php

/**
 * @file
 * Newsitem resource.
 */

/**
 * Returns the results of a node_load() for the specified newsitem id.
 *
 * @param $id
 *   ID of the newsitem we want to return.
 * @return
 *   Node object or FALSE if not found.
 */
function _newsitem_resource_retrieve($id) {
  module_load_include('inc', 'services', 'resources/node_resource');
  $nid = _newsitem_resource_get_entity_id($id);

  return _node_resource_retrieve($nid);
}

/**
 * Determine whether the current user can access a newsitem resource.
 *
 * @param $op
 *   One of view, update, create, delete per node_access().
 * @param $args
 *   Resource arguments passed through from the original request.
 * @return
 *   True if they have access.
 *
 * @see node_access()
 */
function _newsitem_resource_access($op = 'view', $args = array()) {
  module_load_include('inc', 'services', 'resources/node_resource');

  // Adds backwards compatability with regression fixed in #1083242
  if (isset($args[0])) {
    $args[0] = _services_access_value($args[0], 'newsitem');
  }

  // Make sure we have an object or this all fails, some servers can mess up
  // the types.
  if (is_array($args[0])) {
    $args[0] = (object) $args[0];
  }
  elseif (!is_array($args[0]) && !is_object($args[0])) {  // This is to determine if it is just a string happens on newsitem/%NID
    $args[0] = (object) array('id' => $args[0]);
  }

  $nid = _newsitem_resource_get_entity_id($args[0]->id);

  return _node_resource_access($op, array($nid));
}

/**
 * Returns the NID (node id) of a newsitem.
 *
 * @param $id
 *   ID of the newsitem we want to return.
 * @return
 *   NID for the newsitem.
 */
function _newsitem_resource_get_entity_id($id) {
  $field_name = variable_get('converge_resources_newsitem', '');
  $field = field_info_field($field_name);
  $field_current = $field['storage']['details']['sql'][FIELD_LOAD_CURRENT];
  $table_name = key($field_current);
  $table = array_shift($field_current);

  $node_select = db_select($table_name, 't')
          ->fields('t', array('entity_id'))
          ->condition($table['value'], $id, '=');

  return $node_select->execute()->fetch()->entity_id;
}
