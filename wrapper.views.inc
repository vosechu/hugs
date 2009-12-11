<?php
// TODO: Documentation about how to create a view
// TODO: Add on all ro attributes of tables as displayable fields. Not sure if this is even possible or not. 
// TODO: This needs to supply handlers a little more intelligently. Currently it only supports text fields and numbers. Should be supporting booleans and select boxes at the very minimum. 
// TODO: If I choose to link a field to its node it should link to whatever node in belongs to.

/**
 * Implementation of Views' hook_views_data(). 
 *
 * Provides the data necessary for views to use the rw attributes from the metadata
 * as well as using the views relationships to include other tables. It's actually
 * surprisingly nice to work with once you figure out what's going on.
 * 
 * Documentation pending. My sincerest apologies. 
 */
function wrapper_views_data() {
  $data = array();
  
  $all_metadata = get_metadata();
  
  $tables = _get_table_mapping();
  foreach ($tables as $drupal_table => $legacy_table) {
    $data[$legacy_table]['table']['group'] = t(titleize($drupal_table));
    
    // Allow relationships from node to any wrapped content-type
    $data[$legacy_table]['table']['join']['node'] = array(
      'left_table' => $drupal_table,
      'left_field' => 'refid',
      'field' => $all_metadata['primary_key'][$drupal_table],
    );
    $data[$drupal_table]['table']['join']['node'] = array(
      'left_field' => 'nid',
      'field' => 'nid',
    );
    
    // Add in the belongs_to/has_many relationships so we can use other table attributes in the views_ui
    if ($all_metadata['belongs_to'][$drupal_table]) {
      foreach ($all_metadata['belongs_to'][$drupal_table] as $drupal_attr => $legacy_attr) {
        $data[$legacy_table][$legacy_attr]['title'] = t(titleize($drupal_attr));
        $data[$legacy_table][$legacy_attr]['relationship'] = array(
          'base' => $tables[$drupal_attr],
          'base field' => $all_metadata['primary_key'][$drupal_attr],
        );
      }
    }
    if ($all_metadata['has_many'][$drupal_table]) {
      foreach ($all_metadata['has_many'][$drupal_table] as $drupal_attr => $legacy_attr) {
        $data[$legacy_table][$legacy_attr]['title'] = t(titleize($drupal_attr));
        $data[$legacy_table][$legacy_attr]['relationship'] = array(
          'relationship table' => $legacy_table,
          'relationship field' => $all_metadata['primary_key'][$drupal_table],
          'base' => $tables[$drupal_attr],
          'base field' => $legacy_attr,
        );
      }
    }
    
    // Add on all rw attributes of tables as displayable fields
    if ($all_metadata['rw_attribute_mapping'][$drupal_table]) {
      foreach ($all_metadata['rw_attribute_mapping'][$drupal_table] as $drupal_attr => $legacy_attr) {
        $name = (is_array($legacy_attr)) ? $legacy_attr['name'] : $legacy_attr;
        $data[$legacy_table][$name] = add_handlers($legacy_table, $drupal_attr, $legacy_attr);
      }
    }
  }

  return $data;
}

/**
 * Set up the views handlers on rw/ro attributes. Only here to DRY up hook_views_data a bit. 
 */
function add_handlers($legacy_table, $drupal_attr, $legacy_attr) {
  $ret = array();
  
  $ret['title'] = t(titleize($drupal_attr));
  $ret['help'] = t("Displays the @node-attribute column for this node.", array('@node-attribute' => titleize($drupal_attr)));
  
  // Text types
  if (!is_array($legacy_attr) || in_array($legacy_attr['type'], array('textarea', 'textfield'))) {
    $ret['field'] = array(
      'handler' => 'wrapper_handler_field',
      'click sortable' => TRUE,
    );
    $ret['filter'] = array(
      'handler' => 'views_handler_filter_string',
    );
    $ret['argument'] = array(
       'handler' => 'views_handler_argument_string',
    );
  }

  // Numeric fields
  else {
    $ret['field'] = array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    );
    $ret['filter'] = array(
      'handler' => 'views_handler_filter_numeric',
    );
  }

  $ret['sort'] = array(
    'handler' => 'views_handler_sort',
  );
  
  return $ret;
}
