<?php
/**
 * @file
 * Views include file for views hooks.
 * 
 */

/**
 * Implement hook_views_handlers().
 *
 */
function views_extras_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views_extras'),
    ),
    'handlers' => array(
      // Register custom filter, with the class/file name and parent class.
      'views_extras_contenttype' => array(
        'parent' => 'views_handler_filter',
      )
    ),
  );
}

/**
 * Implement hoob_views_data().
 *
 */
function views_extras_views_data() {
  $data = array();

  // Define our dats filter.
  $data['node']['views_extras_contenttype'] = array(
    'group' => t('Custom'),
    'real field'  => 'title',
    'title' => t('Custom filter by content type'),
    'help' => t('Filter based on selected content type'),
    'filter' => array(
      'handler' => 'views_extras_contenttype'
    ),
  );
  
  return $data;
}
