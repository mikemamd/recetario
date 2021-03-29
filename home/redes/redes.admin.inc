<?php

/**
 * Implements hook_form()
 * Show table of items created.
 */
function redes_admin_form_table() {
  $form['redes_table'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Table of Advices Aunt'),
    '#weight'      => 1,
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );
  $form['redes_table']['table'] = array(
    '#type'   => 'markup',
    '#prefix' => redes_admin_table(),
  );

  return $form;
}

/**
 * Implements theme_table()
 * Show items of Advice Aunt.
 */
function redes_admin_table() {
  $headers = array(
    array(
      'data'  => t('ID'),
      'field' => 'id',
    ),
    array(
      'data'  => t('Frame'),
      'field' => 'frame',
    ),
    array(
      'data'  => t('Language'),
      'field' => 'language',
    ),
    array(
      'data'  => t('Create'),
      'field' => 'fb_create',
    ),
    array(
      'data' => t('Edit'),
    ),
    array(
      'data' => t('Delete'),
    ),
  );
  $qry = db_select('adviceaunt', 'aa')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('aa')
    ->limit(5)
    ->orderByHeader($headers);
  $advices = $qry->execute();
  $rows = array();

  foreach ($advices as $row) {
    $rows[] = array(
      $row->id,
      $row->frame,
      $row->language,
      format_date($row->fb_create),
      l(t('Edit'), '/admin/config/recetario/adviceaunt/edit/'.$row->id),
      l(t('Delete'), '/admin/config/recetario/adviceaunt/delete/'.$row->id),
    );
  }
  $output = theme('table', array(
    'header' => $headers,
    'rows'   => $rows
    )
  );
  $output .= theme('pager');

  return $output;
}

/**
 * implements hook_form()
 */
function redes_admin_form($form, &$form_state, $id = null) {
  $form = array();
  module_load_include('inc', 'redes', 'redes');
  $advice = !empty($id)? redes_get( (int) $id ) : null ;
  $form['adviceaunt'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Content of FB Post'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#weight'      => 1,
  );
  if($advice !== NULL ) {
    $form['adviceaunt']['adviceaunt_id'] = array(
      '#type'  => 'hidden',
      '#value' => $advice['id'],
    );
  }//end if
  $form['adviceaunt']['adviceaunt_frame'] = array(
    '#title'         => t('Frame Facebook '),
    '#description'   => t('Content of the Facebook Frame'),
    '#type'          => 'textarea',
    '#rows'          => 5,
    '#required'      => TRUE,
    '#default_value' => $advice != null ? $advice['frame'] : '',
  );
  $form['adviceaunt']['adviceaunt_lang'] = array(
    '#title'         => t('Language'),
    '#description'   => t('Select language of content'),
    '#type'          => 'select',
    '#options'       => redes_get_languages(),
    '#required'      => TRUE,
    '#default_value' => $advice != null ? $advice['language'] : '',
   );
  $form['adviceaunt']['adviceaunt_submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Save'),
    '#submit' => array('redes_admin_form_submit'),
  );
  return $form;
}

/*
 * Implements hook_form_submit()
 * Función de envío de formulario para guardar en BD
 */
function redes_admin_form_submit($form, &$form_state){
  $values = [
    'frame'       => (string) $form_state['values']['adviceaunt_frame'],
    'language'    => (string) $form_state['values']['adviceaunt_lang'],
    'fb_create'   => time()
  ];
  if( isset($form_state['values']['adviceaunt_id']) && !empty($form_state['values']['adviceaunt_id']) ){
    $primaryKey   = 'id';
    $values['id'] = (int) $form_state['values']['adviceaunt_id'];
    $id           = (int) $form_state['values']['adviceaunt_id'];
  }else{
    $primaryKey = array();
    $id         = null;
  }//end if
  if( $write = drupal_write_record('adviceaunt', $values, $primaryKey) != FALSE ){
    block_flush_caches();
    $form_state['redirect'] = 'admin/config/recetario/adviceaunt/table';
    drupal_set_message(t('Registro guardado con éxito'));
  }else{
    form_set_error('adviceaunt_submit', t('Ocurrió un error al intentar guardar el elemento, intente más tarde'));
  }
}

/**
 * implements hook_form()
 */
function redes_admin_delete_form($form, &$form_state, $id = null ) {
  $form     = array();
  $advice   = !empty($id)? (int) $id : null ;

  $question = t('¿Está seguro que desea eliminar el contenido?');
  if($advice !== NULL ) {
    $form['adviceaunt_id_delete'] = array(
      '#type'  => 'hidden',
      '#value' => $id,
    );
  }//end if
  $form['#submit'][] = 'redes_admin_delete_form_submit';

  return confirm_form(
    $form,
    $question,
    'admin/config/recetario/adviceaunt/add',
    t('Esta accion es irreversible.'),
    t('Confirmar'),
    t('Cancelar')
  );
}

/**
 * Implements hook_form_submit()
 */
function redes_admin_delete_form_submit($form, &$form_state){
  if(isset($form_state['values']['adviceaunt_id_delete']) && !empty($form_state['values']['adviceaunt_id_delete'])){
    $id = (int) $form_state['values']['adviceaunt_id_delete'];
    module_load_include('inc', 'redes', 'redes');
    $delete = redes_delete($id);
    drupal_set_message(t('FB Post eliminado con éxito'));
  }else{
    form_set_error('adviceaunt_submit_delete', t('No es posible eliminar el contenido'));
  }//end if
  $form_state['redirect'] = 'admin/config/redes/adviceaunt/table';
}
