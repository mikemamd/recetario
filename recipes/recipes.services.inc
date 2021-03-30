<?php
/**
 * @file
 * Enable Services to get Related Recipes
 */

/**
 * Implements of hook_services_resources().
 */
function recipes_services_resources(){
  $resource = array(
    'list' => array(
      'operations' => array(
        'index' => array(
          'help'             => 'Filtrado de resultados de recetas',
          'callback'         => 'recipes_serviceFilter_content',
          'access callback'  => 'recipes_permission',
          'access arguments' => array('view recipes'),
          'access arguments append' => TRUE,
          'args' => array(
            array(
              'name'        => 'momento',
              'type'        => 'int',
              'description' => '',
              'source'      => array('param' => 'momento'),
              'optional'    => TRUE,
              'default'     => 0,
            ),
            array(
              'name'        => 'temporada',
              'type'        => 'int',
              'description' => '',
              'source'      => array('param' => 'temporada'),
              'optional'    => TRUE,
              'default'     => 0,
            ),
            array(
              'name'        => 'tiempo',
              'type'        => 'int',
              'description' => '',
              'source'      => array('param' => 'tiempo'),
              'optional'    => TRUE,
              'default'     => 0,
            ),
            array(
              'name'        => 'page',
              'type'        => 'int',
              'description' => '',
              'source'      => array('param' => 'page'),
              'optional'    => TRUE,
              'default'     => 1,
            ),
          ),
        ), // end index
      ), // end operations
    ), // end latest
  );

  return $resource;
}

/**
 * Función para validación de parametros GET del servicio
 */
function recipes_serviceFilter_validate($momento = NULL, $temporada = NULL, $tiempo = NULL, $page = NULL) {
  $validate = [ $momento, $temporada, $tiempo, $page ];

  foreach ($validate as $value) {
    if ( $value == NULL || is_numeric($value) ) {
      $success = TRUE;
    }
    else{
     $success = FALSE;
     break;
    }
  }

  return $success;
}

/**
 * Callback para crear servicio de recetas
 * @return json
 */
function recipes_serviceFilter_content($momento = NULL, $temporada = NULL, $tiempo = NULL, $page = NULL) {
  module_load_include('inc', 'recipes', 'recipes');
  global $base_url;
  $list     = [];
  $response = new stdClass();

  //Se validam los parametros recibidos
  $validate = recipes_serviceFilter_validate($momento, $temporada, $tiempo, $page);

  if ( $validate == TRUE ) {
    $qry = db_select('node', 'n');
    $qry->innerJoin('field_data_field_img_min', 'fim', 'fim.entity_id = n.nid');
    $qry->innerJoin('field_data_field_description_ec', 'fde', 'fde.entity_id = n.nid');
    $qry->leftJoin('field_data_field_moment', 'fm', 'fm.entity_id = n.nid');
    $qry->leftJoin('field_data_field_temporalidad', 'ftemp', 'ftemp.entity_id = n.nid');
    $qry->leftJoin('field_data_field_time', 'ftime', 'ftime.entity_id = n.nid');
    $qry->innerJoin('taxonomy_term_data', 'ttd', 'ttd.tid = fm.field_moment_tid');
    $qry->innerJoin('field_data_field_icon_class', 'fic', 'fic.entity_id = ttd.tid');
    $qry->condition('type', 'receipes', '=')
        ->condition('status', '1', '=');
    if ($momento != NULL) {
      $qry->condition('fm.field_moment_tid', $momento, '=');
    }
    if ($temporada != NULL) {
      $qry->condition('ftemp.field_temporalidad_tid', $temporada, '=');
    }
    if ($tiempo != NULL) {
      $qry->condition('ftime.field_time_tid', $tiempo, '=');
    }
    $qry->fields('n', ['nid', 'title'])
        ->fields('fim', ['field_img_min_fid'])
        ->fields('fde', ['field_description_ec_value'])
        ->fields('ttd', ['name'])
        ->fields('fic', ['field_icon_class_value']);
    $qry->extend('PagerDefault')
        ->element('0')
        ->limit(10)
        ->orderBy('n.changed', 'ASC')
        ->execute();


    $results = $qry->execute()->fetchAll();

    if (isset($results) && !empty($results)) {
      $total = 0;
      foreach ($results as $result) {
        $list[] = [
          'nid'    => $result->nid,
          'name'   => $result->title,
          'image'  => trCommon_file_path($result->field_img_min_fid),
          'desc'   => $result->field_description_ec_value,
          'moment' => $result->name,
          'class'  => $result->field_icon_class_value,
          'url'    => $base_url.'/'.recipes_get_alias($result->nid),
        ];
        ++$total;
      }//end foreach
      $response->success      = TRUE;
      $response->message      = t("success");
      $response->totalResults = $total;
      $response->data         = ['list' => $list];
    }
    else{
      $response->success = TRUE;
      $response->message = t("Filtrado sin resultados");
      $response->data    = ['list' => $list];
    }//enif
  }
  else{
    $response->success = FALSE;
    $response->message = "Invalid params";
  }//enif

  return $response;
}
