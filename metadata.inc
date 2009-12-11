<?php

/**
 * Take a node or a pseudo-node and return metadata about what fields it should display or edit.
 *
 * Take a node or a pseudo-node and return metadata about what fields it should 
 * display or edit. 
 * 
 * @param $node Takes a full node or a pseudo-node with just the type attribute set. 
 * This allows get_metadata to filter and return only the data for the requested 
 * content-type.
 * @return mixed Returns an associative array with these indices: 
 * * table_mapping: A mapping of content-type name to old database table name.
 * * rw_attribute_mapping: An associative array of attributes where the key is 
 * the column in the old database and the value is the name of the attribute in
 * the node loaded by node_load(). 
 * * ro_attribute_mapping: An associative array of pseudo-attributes which should
 * be used only for display purposes and not written back to the database. 
 * * form_defs: A FAPI-style description of what drupal should display for the
 * add/edit forms. 
 * * view_defs: A Drupal API style description of what drupal should display when
 * someone asks to display the node. 
 */
function get_metadata($node = null) {
  global $all_metadata;
  
  if (!$all_metadata || empty($all_metadata)) {
    $all_metadata = _get_all_metadata();
  }
  
  // Requesting data for a node that isn't in the table_mapping gets you a big fat FALSE.
  if ($node && !$all_metadata['table_mapping'][$node->type]) {
    return false;
  }
  else {
    // Return all data when no node->type is available
    if (!$node) {
      return $all_metadata;
    }
    // Return just the data we need for this node->type
    
    else {
      $return = array();
      $return['table_mapping']          = $all_metadata['table_mapping'][$node->type];
      $return['primary_key']            = $all_metadata['primary_key'][$node->type];
      $return['has_many']               = $all_metadata['has_many'][$node->type];
      $return['belongs_to']             = $all_metadata['belongs_to'][$node->type];
      $return['ro_attribute_mapping']   = $all_metadata['ro_attribute_mapping'][$node->type];
      $return['rw_attribute_mapping']   = $all_metadata['rw_attribute_mapping'][$node->type];
      
      return $return;
    }
  }
}
 
function _get_all_metadata() {
  $metadata = array();
  
  // Master list of new->old table names. 
  // This is the list we compare when checking if a node is mapable.
  $metadata['table_mapping'] = _get_table_mapping();

  // Check that this is a valid datatype. Shortcircuit to save some CPU time
  $metadata['primary_key'] = array(
    // 'application_status'             => 'as_id',
  );

  $metadata['belongs_to'] = array(
    // 'appointment'              => array(
    //   'member'                 => 'app_mmb_id',
    //   'group_info'             => 'app_grp_id',
    //   'term'                   => 'app_trm_id',
    // ),
  );

  $metadata['has_many'] = array(
    // 'application_status'         => array(
    //   'member_application'       => 'ma_as_id',
    // ),
  );

  // Mappings that should be available for viewing or alteration. These are
  // direct mappings so I can use them to write directly to the database. I'm
  // going to use these to pull additional attributes into the node with the ro
  // attributes below. You may need to override them in form_defs/view_defs.
  $metadata['rw_attribute_mapping'] = array(
    // 'application_status'                => array(
    //   'title'                           => 'as_name',
    //   'body'                            => array('name' => 'as_desc', 'type' => 'textarea'),
    //   'active'                          => array('name' => 'as_active_fl', 'type' => 'checkbox'),
    // ),
  );

  // Mappings that should only be used for viewing. These can be convinience
  // methods, pretification, or whatever you like. If they are pre-process 
  // functions then they should override the view_defs below or in the case of
  // standard node attributes just override the attribute directly (eg. title).
  $metadata['ro_attribute_mapping'] = array(
    // 'member' => array(
    //   'title' => 'CONCAT(mmb_fname, " ", mmb_lname)',
    // ),
  );
  
  return $metadata;
}

function get_node_info() {
  global $node_info;
  
  if (!$node_info || empty($node_info)) {
    $node_info = _get_node_info();
  }
  
  return $node_info;
}

function _get_node_info() {
  return array(
    'application_status'             => array(
      'name'                         => t('Application Status'),
      'description'                  => t('Potential statuses for a member\'s application.'),
      'module'                       => 'hugs',
      'has_title'                    => TRUE,
      'title_label'                  => t('Status'),
      'has_body'                     => TRUE,
      'body_label'                   => t('Description'),
    ),
  );
}

function _get_table_mapping() {
  return array(
    'application_status'       => 'legacy_dir_applicationstatuses',
  );
}

function titleize($word) {
  static $node_info;
  static $titles;
  
  if (!isset($node_info) || empty($node_info)) {
    $node_info = _get_node_info();
  }
  
  if (!isset($titles[$word]) || empty($titles[$word])) {
    if (in_array($word, array_keys($node_info))) {
      $titles[$word] = $node_info[$word]['name'];
    }
    else {
      $titles[$word] = ucwords(preg_replace('/[ \-_]/', ' ', $word));
    }
  }

  return $titles[$word];
}

function untitleize($word) {
  return strtolower(preg_replace('/ /', '-', $word));
}