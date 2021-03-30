<?php
/**
 * Logic to header and footer menu
 */

/**
 * Return menu of region
 * @param string (region)
 * @return array
 */
function mainMenu_get_content($region){
  global $base_url;
  $menu = [];
  $menu['redes'] = mainMenu_get_redes();

  $qry = db_select('menu_links', 'mn', array('fetch' => PDO::FETCH_ASSOC));
  $qry->condition('mn.menu_name', $region, '=');
  $qry->fields('mn', ['mlid', 'link_title','link_path', 'options']);
  $qry->orderBy('mn.weight', 'ASC');
  $results = $qry->execute()->fetchAll();

  if (isset($results) && !empty($results)) {
    foreach ($results as $result) {
    	$options = unserialize($result['options']);

      $menu['menu'][$result['mlid']] = [
        'title' => $result['link_title'],
        'link'  => '/'.$result['link_path'],
        'desc'  => (!empty($options)) ? $options['attributes']['title'] : '',
      ];
    }//end foreach
  }//end if

	if ($region == 'main-menu') {
		$menu['form']  = drupal_get_form("mainMenu_search_form");
		return theme('mainMenu_header', array('menu' => $menu));
	}
	elseif ($region == 'menu-footermenu') {
		return theme('mainMenu_footer', array('menu' => $menu));
	}
}

/*
 * Implements hook_form()
 * Return custom form
 *
 */
function mainMenu_search_form($form, &$form_state){
  $form['custom_search'] = array(
    '#type'           => 'textfield',
    '#default_value'  => '',
    '#attributes'     => array( 'placeholder' => 'Buscar...'),
    '#theme_wrappers' => array(),
  );
  $form['button'] = array(
		'#type' => 'button',
		'#value' => '',
    '#executes_submit_callback' => 'mainMenu_search_form_submit',
  );
  $form['actions']['button']['#prefix'] = '<button type="submit"><span class="icon-menu-buscador-01">&nbsp;</span>';
  $form['actions']['button']['#suffix'] = '</button>';

  $form["#submit"][]       = 'mainMenu_search_form_submit';
  $form['#theme']          = 'form_search_custom';
  $form['#theme_wrappers'] = array('form');
  return $form;
}

/*
 * Implements hook_form()
 * Return custom form
 */
function mainMenu_search_form_submit($form, &$form_state){
  global $base_url;
  $search = $form_state['values']['custom_search'];
  drupal_goto($base_url.'/search/node/'.$search);
}

/**
 * Devuelve las redes cargadas.
 * See admin/structure/taxonomy/redes
 * @return array
 */
function mainMenu_get_redes() {
  $redes_content = [];
  $vocab = taxonomy_vocabulary_machine_name_load('redes');
  $vid   = $vocab->vid;

  $query = db_select('taxonomy_term_data', 'tx')
  ->condition('tx.vid', $vid, '=');
  $query->leftJoin('field_data_field_icon_class', 'fdfic', 'fdfic.entity_id = tx.tid');
  $query->leftJoin('field_data_field_link', 'fl', 'fl.entity_id = tx.tid');
  $query->fields('tx', ['tid', 'name']);
  $query->fields('fdfic', ['field_icon_class_value']);
  $query->fields('fl', ['field_link_value']);
  $query->orderBy('weight', 'ASC');

  $results = $query->execute()->fetchAll();

  if (!empty($results)) {
    foreach ($results as $result) {
      $redes_content[$result->tid] = [
        'name'  => $result->name,
        'class' => $result->field_icon_class_value,
        'link'  => $result->field_link_value,
      ];
    }//end foreach
  }//end if

  return $redes_content;
}
