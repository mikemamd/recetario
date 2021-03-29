<?php 
/**
 * get adviceaunt items
 * @return array
 */
function redes_get($id) {
  $result = array();
  if( isset($id) && !empty($id) && is_int($id) ) {
    $advice = db_select('adviceaunt', 'aa')
    ->fields('aa')
    ->condition('id', $id)
    ->execute()
    ->fetchAssoc();
    if( isset($advice) && !empty($advice) ){
      $result = (array) $advice;
    }//end if
  }//end if

  return $result;
}

/**
 * get language items to Select Option of admin form
 * @return array
 */
function redes_get_languages() {
  $result    = [];
  $languages = db_select('languages', 'lan')
  ->fields('lan', ['language', 'name'])
  ->execute()
  ->fetchAll();
  if(isset($languages) && !empty($languages)){
    foreach ($languages as $lang) {
      $result[$lang->language] = $lang->name;
    }
  }//end if
  return $result;
}


/**
 * Function to delete contet of adviceaunt table
 * @return (bool)
 */
function redes_delete($id) {
  $result = FALSE;
  if(isset($id) && !empty($id) && is_int($id)) {
    $delete = db_delete('adviceaunt')
    ->condition('id', $id)
    ->execute();
    $result = !empty($delete) ? TRUE : FALSE;
  }//end if
  return $result;
}
