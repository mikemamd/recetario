<?php
/**
 * Get all links for categoria
 *
 * @return string
 */
function categorias_getdetail() {
  global $base_url;

  drupal_add_library('system', 'drupal.ajax');
  $current_path = current_path();

  $nametid = str_replace('category/', '', $current_path);
  $tid     = key(taxonomy_get_term_by_name($nametid));

  if (empty($tid)) {
    return $content;
  } // end if

  $qryrelated = db_select('field_data_field_category', 'fdfc')
                ->condition('fdfc.	field_category_tid', $tid, '=');
  $qryrelated->fields('fdfc', ['entity_id']);
  $qryrelated->orderBy('fdfc.entity_id', 'DESC');
  $resultsrelated = $qryrelated->execute()->fetchAll();

  if (count($resultsrelated)<=0) {
    return array();
  } // end if

  if (isset($resultsrelated) && !empty($resultsrelated)) {
    foreach ($resultsrelated as $resultrelated) {
      $query_node = db_select('node', 'n')
      ->condition('n.nid', $resultrelated->entity_id, '=');
      $query_node->leftJoin('field_data_field_img_min', 'fdfm', 'fdfm.entity_id = n.nid');
      $query_node->fields('n', ['nid', 'title']);
      $query_node->fields('fdfm', ['field_img_min_fid']);
      $results = $query_node->execute()->fetchAll();

      foreach ($results as $result) {
        $image = common_file_path($result->field_img_min_fid);
        $content['data'][] = array(
          'image_path' => $image,
          'name' => $result->title,
          'link' => '/'.common_get_alias($result->nid),
          'categoria' => $nametid,
        );
      } // end foreach
    }//end foreach

    $content['categoria'] = common_get_categorias($tid);
  }//end if
  return theme('categoria_theme_tpl', array('content' => $content));
}
