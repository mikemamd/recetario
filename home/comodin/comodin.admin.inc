<?php
/**
 * @file
 * Admin page callbacks to Comodin module.
 */

/**
 * Administer form to recipes header banner
 *
*/
function comodin_admin_form() {
  $form  = [];
  $video = variable_get('comodin_banner_yt','');
  $seleccion = variable_get('comodin_seleccion_yt','');

      $options = [
        '0' => 'COMODIN',
        '1' => 'VIDEO',
      ];

  $form['comodin'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Configuracion general de Video Comodin'),
    '#weight'      => 1,
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE
  );
  $form['comodin']['comodin_banner_yt'] = array(
    '#type'          => 'text_format',
    '#title'         => t( 'Vídeo de Comodin' ),
    '#default_value' => !empty($video) ? $video['value'] : '' ,
    '#rows'          => 1,
    '#format'        => 'plain_text',
  );
  $form['comodin']['comodin_seleccion_yt'] = array(
    '#title'         => t( 'seleccionar activo' ),
    '#description'   => t('Show or Hide the content'),
    '#type'          => 'select',
    '#options'       => $options,
    '#required'      => TRUE,
    '#default_value' => $seleccion != null ? $seleccion : '',
  );

  return system_settings_form($form);
}
