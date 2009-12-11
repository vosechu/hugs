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
    'application_status'             => 'as_id',
    'appointment'                    => 'app_id',
    'area'                           => 'ars_id',
    'college'                        => 'clg_id',
    'college_position'               => 'cp_id',
    'committee'                      => 'co_comittee_id',
    'discipline'                     => 'dsc_id',
    'district'                       => 'dst_id',
    'ethnicity'                      => 'eth_id',
    'goal'                           => 'goal_id',
    'group_info'                     => 'grp_id',
    'group_interest'                 => 'gi_id',
    'group_type'                     => 'gt_id',
    'interest'                       => 'int_id',
    'member'                         => 'mmb_id',
    'member_application'             => 'ma_id',
    'member_discipline'              => 'md_id',
    'member_interest'                => 'mi_id',
    'member_nomination'              => 'mn_id',
    'objective'                      => 'obj_id',
    'office'                         => 'off_id',
    'organization'                   => 'org_id',
    'organization_position'          => 'op_id',
    'publication'                    => 'pub_id',
    'resolution'                     => 'res_id',
    'resolution_publication'         => 'resp_id',
    'resolution_secondary_committee' => 'rsc_id',
    'resolution_status'              => 'rs_status_id',
    'resolution_topic'               => 'rt_topic_id',
    'rostrum'                        => 'ros_id',
    'senate_title'                   => 'st_id',
    'session'                        => 'se_session_id',
    'term'                           => 'trm_id',
    'topic'                          => 'tp_topic_id',
    'year'                           => 'yr_year_id',
  );

  $metadata['belongs_to'] = array(
    'appointment'              => array(
      'member'                 => 'app_mmb_id',
      'group_info'             => 'app_grp_id',
      'term'                   => 'app_trm_id',
    ),
    'committee'                => array(
      'group_info'             => 'co_grp_id',
    ),
    'college'                  => array(
      'district'               => 'clg_dst_id',
    ),
    'college_position'         => array(
      'member'                 => 'cp_mmb_id',
      'senate_title'           => 'cp_st_id',
      'college'                => 'cp_clg_id',
    ),
    'district'                 => array(
      'area'                   => 'dst_ars_id'
    ),
    'group_info'               => array(
      'group_type'             => 'grp_gt_id'
    ),
    'group_interest'           => array(
      'group_info'             => 'gi_grp_id',
      'interest'               => 'gi_int_id',
    ),
    'member'                   => array(
      'senate_title'           => 'mmb_st_id',
    ),
    'member_application'       => array(
      'college'                => 'ma_clg_id',
      'ethnicity'              => 'ma_eth_id',
      'member'                 => 'ma_mmb_id',
      'application_status'     => 'ma_as_id',
    ),
    'member_discipline'        => array(
      'member'                 => 'md_mmb_id',
      'discipline'             => 'md_dsc_id',
    ),
    'member_interest'          => array(
      'member'                 => 'mi_mmb_id',
      'interest'               => 'mi_int_id',
      'term'                   => 'mi_trm_id',
      'member_application'     => 'mi_ma_id',
    ),
    'member_nomination'        => array(
      'member'                 => 'mn_mmb_id',
      'group_info'             => 'mn_grp_id',
      'term'                   => 'mn_trm_id',
    ),
    'publication'              => array(
      'committee'              => 'pub_co_comittee_id',
      'session'                => 'pub_se_session_id',
      'year'                   => 'pub_yr_year_id',
      'topic'                  => 'pub_tp_topic_id',
      'group_info'             => 'pub_grp_id',
    ),
    'resolution'               => array(
      'committee'              => 'res_co_comittee_id',
      'resolution_status'      => 'res_rs_status_id',
      'session'                => 'res_se_session_id',
      'year'                   => 'res_yr_year_id',
      'resolution_topic'       => 'res_rt_topic_id',
      'goal'                   => 'res_goal_id',
      'objective'              => 'res_obj_id',
      'group_info'             => 'res_grp_id',
    ),
    'resolution_topic'         => array(
      'committee'              => 'rt_committee_id'
    ),
    'resolution_publication'   => array(
      'resolution'             => 'resp_res_id',
      'publication'            => 'resp_pub_id',
    ),
    'resolution_secondary_committee' => array(
      'group_info'             => 'rsc_grp_id',
      'committee'              => 'rsc_co_comittee_id',
      'resolution'             => 'rsc_res_id',
    ),
    'rostrum'                  => array(
      'committee'              => 'ros_co_comittee_id',
      'session'                => 'ros_se_session_id',
      'year'                   => 'ros_yr_year_id',
      'group_info'             => 'ros_grp_id',
    ),
    'objective'                => array(
      'goal'                   => 'obj_goal_id'
    ),
    'office'                   => array(
      'college'                => 'off_clg_id'
    ),
    'organization_position'    => array(
      'member'                 => 'op_mmb_id',
      'organization'           => 'op_org_id',
      'senate_title'           => 'op_st_id',
    ),
  );

  $metadata['has_many'] = array(
    'application_status'         => array(
      'member_application'       => 'ma_as_id',
    ),
    'area'                       => array(
      'district'                 => 'dst_ars_id',
    ),
    'college'                    => array(
      'college_position'         => 'cp_clg_id',
      'member_application'       => 'ma_clg_id',
      'office'                   => 'off_clg_id',
    ),
    'committee'                  => array(
      'resolution_topic'         => 'rt_committee_id',
      'publication'              => 'pub_co_comittee_id',
      'resolution'               => 'res_co_comittee_id',
      'rostrum'                  => 'ros_co_comittee_id',
      'resolution_secondary_committee' => 'rsc_co_comittee_id',
    ),
    'discipline'                 => array(
      'member_discipline'        => 'md_dsc_id',
    ),
    'district'                   => array(
      'college'                  => 'clg_dst_id',
    ),
    'ethnicity'                  => array(
      'member_application'       => 'ma_eth_id',
    ),
    'goal'                       => array(
      'resolution'               => 'res_goal_id',
      'objective'                => 'obj_goal_id',
    ),
    'group_info'                 => array(
      'committee'                => 'co_grp_id',
      'publication'              => 'pub_grp_id',
      'resolution'               => 'res_grp_id',
      'member_nomination'        => 'mn_grp_id',
      'rostrum'                  => 'ros_grp_id',
      'resolution_secondary_committee' => 'rsc_grp_id',
      'group_interest'           => 'gi_grp_id',
      'appointment'              => 'app_grp_id',
    ),
    'group_type'                 => array(
      'group_info'               => 'grp_gt_id',
    ),
    'interest'                   => array(
      'group_interest'           => 'gi_int_id',
      'member_interest'          => 'mi_int_id',
    ),
    'member'                     => array(
      'appointment'              => 'app_mmb_id',
      'college_position'         => 'cp_mmb_id',
      'organization_position'    => 'op_mmb_id',
      'member_application'       => 'ma_mmb_id',
      'member_nomination'        => 'mn_mmb_id',
      'member_interest'          => 'mi_mmb_id',
      'member_discipline'        => 'md_mmb_id',
    ),
    'member_application'         => array(
      'member_interest'          => 'mi_ma_id',
    ),
    'objective'                  => array(
      'resolution'               => 'res_obj_id',
    ),
    'organization'               => array(
      'organization_position'    => 'op_org_id',
    ),
    'publication'                => array(
      'resolution_publication'   => 'resp_pub_id',
    ),
    'resolution'                 => array(
      'resolution_publication'   => 'resp_res_id',
    ),
    'resolution_status'          => array(
      'resolution'               => 'res_rs_status_id',
    ),
    'resolution_topic'           => array(
      'resolution'               => 'res_rt_topic_id',
    ),
    'senate_title'               => array(
      'organization_position'    => 'op_st_id',
      'college_position'         => 'cp_st_id',
      'member'                   => 'mmb_st_id',
    ),
    'session'                    => array(
      'publication'              => 'pub_se_session_id',
      'resolution'               => 'res_se_session_id',
      'rostrum'                  => 'ros_se_session_id',
    ),
    'term'                       => array(
      'member_nomination'        => 'mn_trm_id',
      'member_interest'          => 'mi_trm_id',
      'appointment'              => 'app_trm_id',
    ),
    'topic'                      => array(
      'publication'              => 'pub_tp_topic_id',
    ),
    'year'                       => array(
      'publication'              => 'pub_yr_year_id',
      'resolution'               => 'res_yr_year_id',
      'rostrum'                  => 'ros_yr_year_id',
    ),
  );

  // Mappings that should be available for viewing or alteration. These are
  // direct mappings so I can use them to write directly to the database. I'm
  // going to use these to pull additional attributes into the node with the ro
  // attributes below. You may need to override them in form_defs/view_defs.
  $metadata['rw_attribute_mapping'] = array(
    'application_status'                => array(
      'title'                           => 'as_name',
      'body'                            => array('name' => 'as_desc', 'type' => 'textarea'),
      'active'                          => array('name' => 'as_active_fl', 'type' => 'checkbox'),
    ),
    'appointment'                       => array(
      'chair'                           => array('name' => 'app_chair_fl', 'type' => 'checkbox'),
    ),
    'area'                              => array(
      'title'                           => 'ars_name',
      'active'                          => array('name' => 'ars_active_fl', 'type' => 'checkbox'),
    ),
    'college'                           => array(
      'title'                           => 'clg_name',
      'address'                         => 'clg_address',
      'city'                            => 'clg_city',
      'zip'                             => 'clg_zip',
      'office_phone_area'               => array('name' => 'clg_oAreaCode', 'options' => array('#maxlength' => '3')),
      'office_phone_pre'                => array('name' => 'clg_oPre',      'options' => array('#maxlength' => '3')),
      'office_phone_four'               => array('name' => 'clg_oFour',     'options' => array('#maxlength' => '4')),
      'fax_area'                        => array('name' => 'clg_fAreaCode', 'options' => array('#maxlength' => '3')),
      'fax_pre'                         => array('name' => 'clg_fPre',      'options' => array('#maxlength' => '3')),
      'fax_four'                        => array('name' => 'clg_fFour',     'options' => array('#maxlength' => '4')),
      'website'                         => 'clg_website',
    ),
    'college_position'                  => array(
      'primary_position'                => array('name' => 'cp_primary_fl', 'type' => 'checkbox'),
    ),
    'committee'                         => array(
      'title'                           => 'co_comittee_name',
      'body'                            => array('name' => 'co_description', 'type' => 'textarea'),
      'active'                          => array('name' => 'co_active_fl', 'type' => 'checkbox'),
    ),
    'discipline'                        => array(
      'title'                           => 'dsc_name',
      'active'                          => array('name' => 'dsc_active_fl', 'type' => 'checkbox'),
    ),
    'district'                          => array(
      'title'                           => 'dst_name',
      'address'                         => 'dst_address',
      'city'                            => 'dst_city',
      'zip'                             => 'dst_zip',
      'phone'                           => 'dst_phone',
      'fax'                             => 'dst_fax',
    ),
    'ethnicity'                         => array(
      'ethnicity'                       => 'eth_name',
      'active'                          => array('name' => 'eth_active_fl', 'type' => 'checkbox'),
    ),
    'goal'                              => array(
      'goal_num'                        => 'goal_num',
      'body'                            => array('name' => 'goal_description', 'type' => 'textarea'),
      'outcomes'                            => array('name' => 'goal_outcomes', 'type' => 'textarea'),
      'year'                            => 'goal_year',
    ),
    'group_info'                        => array(
      'code'                            => 'grp_code',
      'body'                            => array('name' => 'grp_description', 'type' => 'textarea'),
      'title'                           => 'grp_name',
      'assign_resolutions'              => array('name' => 'grp_assign_fl', 'type' => 'checkbox'),
      'suspended'                       => array('name' => 'grp_suspended_fl', 'type' => 'checkbox'),
      'notes'                           => array('name' => 'grp_notes', 'type' => 'textarea'),
    ),
    'group_type'                        => array(
      'title'                           => 'gt_name',
      'active'                          => array('name' => 'gt_active_fl', 'type' => 'checkbox'),
    ),
    'interest'                          => array(
      'title'                           => 'int_name',
      'active'                          => array('name' => 'int_active_fl', 'type' => 'checkbox'),
    ),
    'member'                            => array(
      'first_name'                      => 'mmb_fname',
      'last_name'                       => 'mmb_lname',
      'address'                         => 'mmb_address',
      'city'                            => 'mmb_city',
      'zip'                             => 'mmb_zip',
      'comments'                        => array('name' => 'mmb_comments',  'type' => 'textarea'),
      'home_phone_area'                 => array('name' => 'mmb_hAreaCode', 'options' => array('#maxlength' => '3')),
      'home_phone_pre'                  => array('name' => 'mmb_hPre',      'options' => array('#maxlength' => '3')),
      'home_phone_four'                 => array('name' => 'mmb_hFour',     'options' => array('#maxlength' => '4')),
      'cell_phone_area'                 => array('name' => 'mmb_cAreaCode', 'options' => array('#maxlength' => '3')),
      'cell_phone_pre'                  => array('name' => 'mmb_cPre',      'options' => array('#maxlength' => '3')),
      'cell_phone_four'                 => array('name' => 'mmb_cFour',     'options' => array('#maxlength' => '4')),
      'work_phone_area'                 => array('name' => 'mmb_wAreaCode', 'options' => array('#maxlength' => '3')),
      'work_phone_pre'                  => array('name' => 'mmb_wPre',      'options' => array('#maxlength' => '3')),
      'work_phone_four'                 => array('name' => 'mmb_wFour',     'options' => array('#maxlength' => '4')),
      'work_phone_ext'                  => array('name' => 'mmb_wExt',      'options' => array('#maxlength' => '4')),
      'begin_month'                     => 'mmb_begin_month',
      'begin_year'                      => 'mmb_begin_year',
      'end_month'                       => 'mmb_end_month',
      'end_year'                        => 'mmb_end_year',
      'email'                           => 'mmb_email',
      'website'                         => 'mmb_website',
      'public'                          => array('name' => 'mmb_publish_fl', 'type' => 'checkbox', 'options' => array('#description' => 'Make this profile visible to the public in the member directory')),
    ),
    'member_application'                => array(
      'qualifications'                  => array('name' => 'ma_qualifications', 'type' => 'textarea', 'options' => array('#description' => 'Briefly explain your qualifications')),
      'expertise'                       => array('name' => 'ma_expertise', 'type' => 'textarea'),
      'experience'                      => array('name' => 'ma_experience', 'type' => 'textarea'),
      'work_status'                     => array('name' => 'ma_status_fl',   'type' => 'radias', 'options' => array('#options' => array(t('Full Time'), t('Part Time'), t('Noncredit')))),
      'gender'                          => array('name' => 'ma_gender_fl',   'type' => 'radios', 'options' => array('#options' => array(t('Male'), t('Female')))),
      'disabled'                        => array('name' => 'ma_disabled_fl', 'type' => 'radios', 'options' => array('#options' => array(t('Yes'), t('No')))),
      'vietnam_veteran'                 => array('name' => 'ma_vet_fl',      'type' => 'radios', 'options' => array('#options' => array(t('Yes'), t('No')))),
    ),
    'objective'                         => array(
      'objective_number'                => 'obj_num',
      'body'                            => array('name' => 'obj_description', 'type' => 'textarea'),
    ),
    'office'                            => array(
      'staff_name'                      => 'off_staff',
      'office_phone_area'               => 'off_oareacode',
      'office_phone_pre'                => 'off_opre',
      'office_phone_four'               => 'off_ofour',
      'office_phone_ext'                => 'off_oext',
      'fax_area'                        => 'off_fareacode',
      'fax_pre'                         => 'off_fpre',
      'fax_four'                        => 'off_ffour',
      'website'                         => 'off_website',
    ),
    'organization'                      => array(
      'title'                           => 'org_name',
      'address'                         => 'org_address',
      'city'                            => 'org_city',
      'zip'                             => 'org_zip',
      'office_phone_area'               => 'org_oareacode',
      'office_phone_pre'                => 'org_opre',
      'office_phone_four'               => 'org_ofour',
      'fax_area'                        => 'org_fareacode',
      'fax_pre'                         => 'org_fpre',
      'fax_four'                        => 'org_ffour',
      'website'                         => 'org_website',
    ),
    'organization_position'             => array(
      'primary_position'                => array('name' => 'op_primary_fl', 'type' => 'checkbox'),
    ),
    'publication'                       => array(
      'title'                           => 'pub_title',
      'body'                            => array('name' => 'pub_abstract', 'type' => 'textarea'),
      'file_path'                       => array('name' => 'pub_document_path', 'type' => 'upload_element'),
      'comment'                         => array('name' => 'pub_comment', 'type' => 'textarea'),
      'recommendation'                  => array('name' => 'pub_recommendation', 'type' => 'textarea'),
      'cross_reference'                 => array('name' => 'pub_cross_reference', 'type' => 'textarea'),
      'keywords'                        => array('name' => 'pub_keywords', 'type' => 'textarea'),
      'url'                             => 'pub_link',
    ),
    'resolution'                        => array(
      'resolution_number'               => 'res_number',
      'title'                           => 'res_title',
      'author'                          => 'res_presenter',
      'body'                            => array('name' => 'res_content', 'type' => 'textarea'),
      'referred'                        => array('name' => 'res_referred', 'type' => 'checkbox'),
      'file_path'                       => array('name' => 'res_document_path', 'type' => 'upload_element'),
      'final'                           => 'res_final',
      'priority'                        => 'res_priority',
      'summary'                         => array('name' => 'res_summary', 'type' => 'textarea'),
      'recommendation'                  => array('name' => 'res_recommendation', 'type' => 'textarea'),
      'status'                          => array('name' => 'res_status', 'type' => 'textarea'),
    ),
    'resolution_status'                 => array(
      'title'                           => 'rs_status',
      'active'                          => array('name' => 'rs_active_fl', 'type' => 'checkbox'),
    ),
    'resolution_topic'                  => array(
      'title'                           => 'rt_topic',
      'active'                          => array('name' => 'rt_active_fl', 'type' => 'checkbox'),
    ),
    'rostrum'                           => array(
      'title'                           => 'ros_title',
      'author'                          => 'res_author',
      'body'                            => array('name' => 'ros_content', 'type' => 'textarea'),
      'file_path'                       => array('name' => 'ros_document_path', 'type' => 'upload_element'),
      'month'                           => 'ros_mo_month',
      'keywords'                        => array('name' => 'ros_topic_keywords', 'type' => 'textarea'),
    ),
    'senate_title'                      => array(
      'title'                           => 'st_name',
      'active'                          => array('name' => 'st_active_fl', 'type' => 'checkbox'),
      'sort'                            => 'st_sort',        
    ),
    'session'                           => array(
      'title'                           => 'se_session',
      'active'                          => array('name' => 'se_active_fl', 'type' => 'checkbox'),
    ),
    'term'                              => array(
      'title'                           => 'trm_name',
      'sort'                            => 'trm_sort',
      'active'                          => array('name' => 'trm_active_fl', 'type' => 'checkbox'),
    ),
    'topic'                             => array(
      'title'                           => 'tp_topic',
      'active'                          => array('name' => 'tp_active_fl', 'type' => 'checkbox'),
    ),
    'year'                              => array(
      'title'                           => 'yr_year',
      'active'                          => array('name' => 'yr_active_fl', 'type' => 'checkbox'),
    ),
  );

  // Mappings that should only be used for viewing. These can be convinience
  // methods, pretification, or whatever you like. If they are pre-process 
  // functions then they should override the view_defs below or in the case of
  // standard node attributes just override the attribute directly (eg. title).
  $metadata['ro_attribute_mapping'] = array(
    'member' => array(
      'title' => 'CONCAT(mmb_fname, " ", mmb_lname)',
      'home_phone' => 'CONCAT(mmb_hAreaCode, mmb_hPre, mmb_hFour)',
      'cell_phone' => 'CONCAT(mmb_cAreaCode, mmb_cPre, mmb_cFour)',
      'work_phone' => 'CONCAT(mmb_wAreaCode, mmb_wPre, mmb_wFour, mmb_wExt)',
    ),
    'college' => array(
      'office_phone' => 'CONCAT(clg_oAreaCode, clg_oPre, clg_oFour)',
      'fax' => 'CONCAT(clg_fAreaCode, clg_fPre, clg_fFour)',
    ),
    'office' => array(
      'office_phone' => 'CONCAT(off_oAreaCode, off_oPre, off_oFour)',
      'fax' => 'CONCAT(off_fAreaCode, off_fPre, off_fFour)',
    ),
    'organization' => array(
      'office_phone' => 'CONCAT(org_oAreaCode, org_oPre, org_oFour)',
      'fax' => 'CONCAT(org_fAreaCode, org_fPre, org_fFour)',
    ),
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
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Status'),
      'has_body'                     => TRUE,
      'body_label'                   => t('Description'),
    ),
    'appointment'                    => array(
      'name'                         => t('Appointment'),
      'description'                  => t('A member\'s appointment to a group over the course of some term.'),
      'module'                       => 'wrapper',
      'has_title'                    => FALSE,
      'has_body'                     => FALSE,
    ),
    'area'                           => array(
      'name'                         => t('Area'),
      'description'                  => t('An area node holds information linking a number of districts.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Area Name'),
      'has_body'                     => FALSE,
    ),
    'college'                        => array(
      'name'                         => t('College'),
      'description'                  => t('A College node holds contact information about a college as well as links to the offices, titles, and members that belong to it.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Name'),
      'has_body'                     => TRUE,
      'body_label'                   => t('Description'),
    ),
    'college_position'               => array(
      'name'                         => t('College Position'),
      'description'                  => t('A mapping between members and colleges.'),
      'module'                       => 'wrapper',
      'has_title'                    => FALSE,
      'has_body'                     => FALSE,
    ),
    'committee'                      => array(
      'name'                         => t('Committee'),
      'description'                  => t('A committee with links to a particular group.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Name'),
      'has_body'                     => TRUE,
      'body_label'                   => t('Committee Description'),
    ),
    'discipline'                     => array(
      'name'                         => t('Discipline'),
      'description'                  => t('A member discipline.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Discipline'),
      'has_body'                     => FALSE,
    ),
    'district'                       => array(
      'name'                         => t('District'),
      'description'                  => t('A district of schools and its main contact information.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('District Name'),
      'has_body'                     => FALSE,
    ),
    'ethnicity'                      => array(
      'name'                         => t('Ethnicity'),
      'description'                  => t('A list of ethnicities which can be chosen for members.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Ethnicity'),
      'has_body'                     => FALSE,
    ),
    'goal'                           => array(
      'name'                         => t('Goal'),
      'description'                  => t('Goal nodes are stated objectives and outcomes used by Objectives and Resolutions.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Goal'),
      'has_body'                     => TRUE,
      'body_label'                   => t('Goal Description'),
    ),
    'group_info'                     => array(
      'name'                         => t('Group'),
      'description'                  => t('Group nodes hold information about the various groups in California, who belongs to them, their objectives, and whether they\'re active or not.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Name'),
      'has_body'                     => TRUE,
      'body_label'                   => t('Description'),
    ),
    'group_interest'                 => array(
      'name'                         => t('Group Interest'),
      'description'                  => t('A mapping between groups and interests.'),
      'module'                       => 'wrapper',
      'has_title'                    => FALSE,
      'has_body'                     => FALSE,
    ),
    'group_type'                     => array(
      'name'                         => t('Group Type'),
      'description'                  => t('A list of group types which can be chosen for groups.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Type'),
      'has_body'                     => FALSE,
    ),
    'interest'                       => array(
      'name'                         => t('Interest'),
      'description'                  => t('An list of interests which can be chosen for groups and members.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Interest'),
      'has_body'                     => FALSE,
    ),
    'member'                         => array(
      'name'                         => t('Member'),
      'description'                  => t('Information about a member in the ASCCC database.'),
      'module'                       => 'wrapper',
      'has_title'                    => FALSE,
      'has_body'                     => FALSE,
    ),
    'member_application'             => array(
      'name'                         => t('Member Application'),
      'description'                  => t('An application to serve which can be reviewed by certain people.'),
      'module'                       => 'wrapper',
      'has_title'                    => FALSE,
      'has_body'                     => FALSE,
    ),
    'member_discipline'              => array(
      'name'                         => t('Member Discipline'),
      'description'                  => t('A mapping between members and disciplines.'),
      'module'                       => 'wrapper',
      'has_title'                    => FALSE,
      'has_body'                     => FALSE,
    ),
    'member_interest'                => array(
      'name'                         => t('Member Interest'),
      'description'                  => t('A mapping between members and interests'),
      'module'                       => 'wrapper',
      'has_title'                    => FALSE,
      'has_body'                     => FALSE,
    ),
    'member_nomination'              => array(
      'name'                         => t('Member Nomination'),
      'description'                  => t('A nomination for a member to serve in a group.'),
      'module'                       => 'wrapper',
      'has_title'                    => FALSE,
      'has_body'                     => FALSE,
    ),
    'objective'                      => array(
      'name'                         => t('Objective'),
      'description'                  => t('Objective nodes hold information about goals and their intended results.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Number'),
      'has_body'                     => TRUE,
      'body_label'                   => t('Description'),
    ),
    'office'                         => array(
      'name'                         => t('Office'),
      'description'                  => t('Information about the senate offices at a college'),
      'module'                       => 'wrapper',
      'has_title'                    => FALSE,
      'has_body'                     => FALSE,
    ),
    'organization'                   => array(
      'name'                         => t('Organization'),
      'description'                  => t('An organization node holds contact information for some organization serving the California college system.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Name'),
      'has_body'                     => FALSE,
    ),
    'organization_position'          => array(
      'name'                         => t('Organization Position'),
      'description'                  => t('A mapping between organizations and members.'),
      'module'                       => 'wrapper',
      'has_title'                    => FALSE,
      'has_body'                     => FALSE,
    ),
    'publication'                    => array(
      'name'                         => t('Publication'),
      'description'                  => t('A publication node holds information about a publication such as its abstract, any comments and recommendations, as well as the group that worked on it.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Title'),
      'has_body'                     => TRUE,
      'body_label'                   => t('Abstract'),
    ),
    'resolution'                     => array(
      'name'                         => t('Resolution'),
      'description'                  => t('A resolution node holds information about a resolution such as the year it was floated, the content, any recommendations, and the goals and objectives that it is trying most closely paired with.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Title'),
      'has_body'                     => TRUE,
      'body_label'                   => t('Content'),
    ),
    'resolution_publication'         => array(
      'name'                         => t('Resolution Publication'),
      'description'                  => t('A mapping between resolutions and publications.'),
      'module'                       => 'wrapper',
      'has_title'                    => FALSE,
      'has_body'                     => FALSE,
    ),
    'resolution_secondary_committee' => array(
      'name'                         => t('Resolution Secondary Committee'),
      'description'                  => t('A mapping between resolutions and committees.'),
      'module'                       => 'wrapper',
      'has_title'                    => FALSE,
      'has_body'                     => FALSE,
    ),
    'resolution_status'              => array(
      'name'                         => t('Resolution Status'),
      'description'                  => t('A list of statuses which a resolution can hold.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Status'),
      'has_body'                     => FALSE,
    ),
    'resolution_topic'               => array(
      'name'                         => t('Resolution Topic'),
      'description'                  => t('A list of topics which a resolution can have.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Topic'),
      'has_body'                     => FALSE,
    ),
    'rostrum'                        => array(
      'name'                         => t('Rostrum'),
      'description'                  => t('A rostrum node holds information about a public speech, the author, the committee paired with the speech and some keywords.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Title'),
      'has_body'                     => TRUE,
      'body_label'                   => t('Content'),
    ),
    'senate_title'                   => array(
      'name'                         => t('Senate Title'),
      'description'                  => t('Titles for positions in organizations, groups, and colleges.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Title'),
      'has_body'                     => FALSE,
    ),
    'session'                        => array(
      'name'                         => t('Session'),
      'description'                  => t('A list of sessions which can be associated with publications, rostrae, and resolutions.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Session'),
      'has_body'                     => FALSE,
    ),
    'term'                           => array(
      'name'                         => t('Term'),
      'description'                  => t('A list of terms which can be associated with publications, rostrae, and resolutions.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Term'),
      'has_body'                     => FALSE,
    ),
    'topic'                          => array(
      'name'                         => t('Topic'),
      'description'                  => t('A list of potential topics which can be associated with publications, rostrae, and resolutions.'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Topic'),
      'has_body'                     => FALSE,
    ),
    'year'                           => array(
      'name'                         => t('Year'),
      'description'                  => t('A Year'),
      'module'                       => 'wrapper',
      'has_title'                    => TRUE,
      'title_label'                  => t('Year'),
      'has_body'                     => FALSE,
    ),
  );
}

function _get_table_mapping() {
  return array(
    'application_status'       => 'legacy_dir_applicationstatuses'
  ,   'appointment'              => 'legacy_dir_appointments'
  ,   'area'                     => 'legacy_dir_areas'
  ,   'college'                  => 'legacy_dir_colleges'
  ,   'college_position'         => 'legacy_dir_collegepositions'
  ,   'committee'                => 'legacy_committies'
  ,   'discipline'               => 'legacy_dir_disciplines'
  ,   'district'                 => 'legacy_dir_districts'
  ,   'ethnicity'                => 'legacy_dir_ethnicities'
  ,   'goal'                     => 'legacy_goals'
  ,   'group_info'               => 'legacy_dir_groups'
  ,   'group_interest'           => 'legacy_dir_groupinterests'
  ,   'group_type'               => 'legacy_dir_grouptypes'
  ,   'interest'                 => 'legacy_dir_interests'
  ,   'member'                   => 'legacy_dir_members'
  ,   'member_application'       => 'legacy_dir_memberapplications'
  ,   'member_discipline'        => 'legacy_dir_memberdisciplines'
  ,   'member_interest'          => 'legacy_dir_memberinterests'
  ,   'member_nomination'        => 'legacy_dir_membernominations'
  ,   'objective'                => 'legacy_objectives'
  ,   'office'                   => 'legacy_dir_offices'
  ,   'organization'             => 'legacy_dir_organizations'
  ,   'organization_position'    => 'legacy_dir_organizationpositions'
  ,   'publication'              => 'legacy_publications'
  ,   'resolution_publication'   => 'legacy_res_publication'
  ,   'resolution_status'        => 'legacy_res_statuses'
  ,   'resolution_topic'         => 'legacy_res_topics'
  ,   'resolution'               => 'legacy_resolutions'
  ,   'rostrum'                  => 'legacy_rostrums'
  ,   'resolution_secondary_committee' => 'legacy_res_secondary_comitties'
  ,   'senate_title'             => 'legacy_dir_senatetitles'
  ,   'session'                  => 'legacy_sessions'
  ,   'term'                     => 'legacy_dir_terms'
  ,   'topic'                    => 'legacy_topics'
  ,   'year'                     => 'legacy_years'
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