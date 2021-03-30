<?php

/**
 * @file
 * Admin page callbacks to recipes module.
 */



/**
 * Administer form to recipes header banner
 *
*/
function recipes_admin_form() {
  $form  = [];
  $title = variable_get('recipes_title','');
  $desc  = variable_get('recipes_desc','');
  $video = variable_get('recipes_banner_yt','');

  $form['recipes'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Configuracion general de contenido'),
    '#weight'      => 1,
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE
  );
  $form['recipes']['recipes_banner_img'] = array(
    '#type'              => 'managed_file',
    '#title'             => t( 'Imagen para banner general de recetas' ),
    '#description'       => t( 'Deje vacío para mostrar vídeo' ),
    '#required'          => FALSE,
    '#upload_location'   => file_default_scheme() . '://module/recipes',
    '#default_value'     => variable_get('recipes_banner_img', ''),
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
    ),
  );
  $form['recipes']['recipes_banner_yt'] = array(
    '#type'          => 'text_format',
    '#title'         => t( 'Vídeo de Banner' ),
    '#description'   => t( 'Deje vacío para mostrar imagen' ),
    '#default_value' => !empty($video) ? $video['value'] : '' ,
    '#rows'          => 1,
    '#format'        => 'plain_text',
  );
  $form['recipes']['recipes_title'] = array(
    '#type'          => 'text_format',
    '#title'         => t( 'Título de Banner' ),
    '#description'   => t( 'Tíulo que se mostrará en el banner general de recetas' ),
    '#default_value' => !empty($title) ? $title['value'] : '' ,
    '#rows'          => 1,
    '#format'        => 'plain_text',
  );
  $form['recipes']['recipes_desc'] = array(
    '#type'          => 'text_format',
    '#title'         => t( 'Descripción de Banner' ),
    '#description'   => t( 'Descripción que se mostrará en el banner general de recetas' ),
    '#default_value' => !empty($desc) ? $desc['value'] : '' ,
    '#rows'          => 3,
    '#format'        => 'plain_text',
  );

  $fid = variable_get('recipes_banner_img', '');
  if (isset($fid) && !empty($fid)) {
    $file = file_load($fid);
    if ($file->status == 0) {
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'recipes', 'recipes', 1);
      drupal_set_message(t('Archivo guardado con éxito'), 'status');
    }//end if
  }//end if

  return system_settings_form($form);
}
