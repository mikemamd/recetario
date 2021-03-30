<?php
/**
 * @file
 * Enable Recipes Module
 */

/**
 * Return content to display on header banner.
 * @return array
 */
function recipes_get_header() {
  $content = [];

  $content = [
    'title' => variable_get('recipes_title',''),
    'desc'  => variable_get('recipes_desc',''),
    'img'   => trCommon_file_path(variable_get('recipes_banner_img', '')),
    'video' => variable_get('recipes_banner_yt',''),
    'logo'  => theme_get_setting('logo', 'tiarosa'),
  ];

  return theme('header_recipes', array('content' => $content));
}

/**
 * Return related recipes by nodo.
 * @return array
 */
function recipes_get_related() {
  global $base_url;
  $content = $tags = $products = [];

  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nid = arg(1);
    $related_fields = recipes_get_current_related($nid);
  }//endif

  $qry = db_select('node', 'n');
  $qry->innerJoin('field_data_field_img_min', 'fim', 'fim.entity_id = n.nid');
  $qry->innerJoin('field_data_field_moment', 'fm', 'fm.entity_id = n.nid');
  $qry->innerJoin('field_data_field_description_ec', 'fde', 'fde.entity_id = n.nid');
  $qry->innerJoin('taxonomy_term_data', 'ttd', 'ttd.tid = fm.field_moment_tid');
  $qry->innerJoin('field_data_field_icon_class', 'fic', 'fic.entity_id = ttd.tid');
  $qry->leftJoin('field_data_field_receipe_tag', 'frt', 'frt.entity_id = n.nid');
  $qry->leftJoin('field_data_field_productos_relacionados', 'fpr', 'fpr.entity_id = n.nid');
  $qry->condition('type', 'receipes', '=')
      ->condition('status', '1', '=')
      ->condition('n.nid', $nid, '!=')
      ->condition(db_or()
        ->condition('frt.field_receipe_tag_tid', $related_fields['tags'], 'IN')
        ->condition('fpr.field_productos_relacionados_target_id', $related_fields['prods'], 'IN'));
  $qry->fields('n', ['nid', 'title'])
      ->fields('fim', ['field_img_min_fid'])
      ->fields('fde', ['field_description_ec_value'])
      ->fields('ttd', ['name'])
      ->fields('fic', ['field_icon_class_value']);
  $qry->orderBy('n.changed', 'ASC');
  $qry->range(0,5);
  $results = $qry->execute()->fetchAll();

  if (isset($results) && !empty($results)) {
    foreach ($results as $result) {
      $content[$result->nid] = [
        'img'    => common_file_path($result->field_img_min_fid),
        'title'  => $result->title,
        'desc'   => $result->field_description_ec_value,
        'alias'  => $base_url.'/'.recipes_get_alias($result->nid),
        'moment' => [
          'class' => $result->field_icon_class_value,
          'time'  => $result->name,
        ],
      ];
    }//end foreach
  }//end if
  return theme('related_recipes', array('content' => $content));
}

/**
 * Return fields related by current node.
 * @return array
 */
function recipes_get_current_related($nid) {
  $related = [];

  $qry = db_select('node', 'n');
  $qry->leftJoin('field_data_field_receipe_tag', 'frt', 'frt.entity_id = n.nid');
  $qry->leftJoin('field_data_field_productos_relacionados', 'fpr', 'fpr.entity_id = n.nid');
  $qry->condition('type', 'receipes', '=')
      ->condition('status', '1', '=')
      ->condition('n.nid', $nid, '=');
  $qry->fields('frt', ['field_receipe_tag_tid'])
      ->fields('fpr', ['field_productos_relacionados_target_id']);
  $results = $qry->execute()->fetchAll();

  foreach ($results as $relateds) {
    $related['tags'][$relateds->field_receipe_tag_tid] = $relateds->field_receipe_tag_tid;
    $related['prods'][$relateds->field_productos_relacionados_target_id] = $relateds->field_productos_relacionados_target_id;
  }//endforeach

  return $related;
}

/**
 * Return the finally url of node.
 * @return string
 */
function recipes_get_alias($nid) {
  $source = 'node/'.$nid;
  $qry = db_select('url_alias', 'ua');
  $qry->condition('ua.source', $source, '=');
  $qry->fields('ua', ['alias']);
  $results = $qry->execute()->fetchAll();
  if (isset($results) && !empty($results)) {
    foreach ($results as $result) {
      $alias = $result->alias;
    }
    return $alias;
  }
}

/**
 * Return template by blokc.
 * @param string (vocabulary machine name)
 */
function recipes_filter_moment($filter) {
  switch ($filter) {
    case 'moments':
      $content = recipes_get_filters('moments_of_the_day');
      return theme('filter_moments', array('content' => $content));
      break;
    case 'times':
      $content['temporada'] = recipes_get_filters('season');
      $content['tiempo']    = recipes_get_filters('menu_time');
      return theme('filter_times', array('content' => $content));
      break;
    case 'results':
      $content = recipes_serviceFilter_content()->data['list'];
      return theme('filter_results', array('content' => $content));
      break;
  }
}

/**
 * Return category filter.
 * @param string (vocabulary machine name)
 * @return array
 */
function recipes_get_filters($vocabulary) {
  $vid    = taxonomy_vocabulary_machine_name_load($vocabulary)->vid;
  $filter = [];

  $qry = db_select('taxonomy_term_data', 'ttd');
  $qry->condition('ttd.vid', $vid, '=');
  if ($vocabulary == 'moments_of_the_day') {
    $qry->innerJoin('field_data_field_icon_class', 'fic', 'fic.entity_id = ttd.tid');
    $qry->fields('fic', ['field_icon_class_value']);
  }
  $qry->fields('ttd', ['tid', 'name']);
  $qry->orderBy('ttd.weight', 'ASC');
  $results = $qry->execute()->fetchAll();

  if (!empty($results)) {
    foreach ($results as $term) {
      $filter[$term->tid] = [ 'name' => $term->name, ];

      if ( isset($term->field_icon_class_value) && !empty($term->field_icon_class_value)  ) {
        $filter[$term->tid]['class'] = $term->field_icon_class_value;
      }//endif
    }//endforeach
  }//endif

  return $filter;
}
