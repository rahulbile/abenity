<?php
/**
 * @file
 * abenity.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function abenity_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'user-user-field_abenity_password'.
  $field_instances['user-user-field_abenity_password'] = array(
    'bundle' => 'user',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 74,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'user',
    'exclude_cv' => 0,
    'field_name' => 'field_abenity_password',
    'label' => 'Abenity password',
    'required' => 0,
    'settings' => array(
      'enable_variable_configuration' => 0,
      'exclude_cv' => FALSE,
      'text_processing' => 0,
      'user_register_form' => 0,
      'variable_config_fieldset' => array(
        'variable_description' => '',
        'variable_group' => '',
        'variable_title' => '',
        'variable_type' => 0,
      ),
      'variable_config_group_fieldset' => array(
        'group_access' => 'administer site configuration',
        'group_description' => '',
        'group_path' => '',
        'group_title' => '',
      ),
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'maxlength_js' => 0,
        'maxlength_js_label' => 'Content limited to @limit characters, remaining: <strong>@remaining</strong>',
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 33,
    ),
  );

  // Exported field_instance: 'user-user-field_abenity_username'.
  $field_instances['user-user-field_abenity_username'] = array(
    'bundle' => 'user',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 73,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'user',
    'exclude_cv' => 0,
    'field_name' => 'field_abenity_username',
    'label' => 'Abenity username',
    'required' => 0,
    'settings' => array(
      'enable_variable_configuration' => 0,
      'exclude_cv' => FALSE,
      'text_processing' => 0,
      'user_register_form' => 0,
      'variable_config_fieldset' => array(
        'variable_description' => '',
        'variable_group' => '',
        'variable_title' => '',
        'variable_type' => 0,
      ),
      'variable_config_group_fieldset' => array(
        'group_access' => 'administer site configuration',
        'group_description' => '',
        'group_path' => '',
        'group_title' => '',
      ),
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'maxlength_js' => 0,
        'maxlength_js_label' => 'Content limited to @limit characters, remaining: <strong>@remaining</strong>',
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 32,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Abenity password');
  t('Abenity username');

  return $field_instances;
}
