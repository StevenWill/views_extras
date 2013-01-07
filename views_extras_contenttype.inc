<?php
/**
 * @file
 * Include file for views_extras module.
 *
 */

/**
 * Extends views_handler_filter.
 * We add our form elements to the options form and exposed filter 
 * before altering the query to include our values. 
 *
 */
class views_extras_contenttype extends views_handler_filter {

  /**
   * Append our form elements to the options form.
   *
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $result = db_query("SELECT type, name FROM node_type");
    $content_type = array();
    while ($value = db_fetch_object($result)) {
     $content_type[$value->type] = $value->name;
    }

    $default_value = isset($this->options['contenttype']) ? $this->options['contenttype'] : array(page);
    //drupal_set_message($default_value);
    $form['contenttype'] = array(
      '#type' => 'checkboxes',
      '#title'  => t('Include Content Types'),
      '#options' => $content_type,
      '#default_value'  => $default_value,
    );

  }
  
  /** 
   * Pass the value as a hidden form to the exposed filter.
   * Needed for the exposed form.
   */
  function value_form(&$form, &$form_state) {
    $form['views_extras_contenttype']  = array(
      '#type' => 'hidden',
    );
  }
  
  /**
   * Add our elements to the Exposed form.
   */
  function exposed_form(&$form, &$form_state) {
	// Perform query to select content types.
    $result = db_query("SELECT type, name FROM node_type");
    $content_type = array();
    // Iterate through results and pair content type name and value.
    while ($value = db_fetch_object($result)) {
	  $content_type[$value->type] = $value->name;
    } 

    $ct = array_filter($this->options['contenttype']);
    $ct_options = array();
    // Iterate through result and selected form value to pair machine name
    // and human name selected.
    foreach($content_type as $key => $ctvalue) {
      foreach($ct as $value) {
        if ($value == $key) {
          $ct_options[$value] = $ctvalue;
        }
      }
    }  

    $form['views_extras_contenttype'] = array(
      '#type' => 'checkboxes',
      '#title'  => t('Include Content Types'),
      '#options' => $ct_options,
    );  
  }

  // Alter the query to add our values.
  function query() {
    
    // Check for exposed filters.
    if (!empty($this->value)) {
      $value = $this->value['page'];
      //drupal_set_message('<pre>' . print_r($this->value, TRUE) . '</pre>');
	  foreach ($this->value as $value) {
	    $this->query->add_where($this->options['group'], "node.type = '%s'", $value);
	    $this->query->set_where_group('OR', $this->options['group']);
	  } 
    }
    else {
	  //Check that the unexposed form was defined.
      if (isset($this->options['contenttype'])) {
	    // Filter out unselected items.
        $content_type = array_filter($this->options['contenttype']);
        // Iterate through the content types.
        foreach ($content_type as $key => $value) {
          // Add the WHERE clause and to filter on node type. 
          $this->query->add_where($this->options['group'], "node.type = '%s'", $key);
          // Add the OR between the WHRE clauses.
          $this->query->set_where_group('OR', $this->options['group']);
        }
      }
    }
  }
}
