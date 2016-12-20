<?php

/**
 * Implements hook_variable_group_info().
 */
function abenity_variable_group_info() {
  $groups['abenity'] = array(
    'title' => t('Abenity API settings'),
    'description' => t('Abenity API Settings.'),
    'access' => 'administer abenity',
    'path' => array('admin/config/system/abenity'),
  );
  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function abenity_variable_info($options) {
  $variables = array();
  $variables['abenity_api_enabled'] = array(
    'title' => t('Enable abenity.', array(), $options),
    'description' => t('Enable abenity.', array(), $options),
    'type' => 'boolean',
    'group' => 'abenity',
    'default_value' => 1,
  );

  $variables['abenity_api_sandbox_mode'] = array(
    'title' => t('Enable sandbox mode?.', array(), $options),
    'description' => t('The sandbox operates on a copy of abenity main database that is updated nightly. So any changes made to the sandbox will be erased on a daily basis.', array(), $options),
    'type' => 'boolean',
    'group' => 'abenity',
    'default_value' => 0,
  );

  $variables['abenity_api_username'] = array(
    'title' => t('Abenity API username.', array(), $options),
    'description' => t('Abenity API username.', array(), $options),
    'type' => 'string',
    'group' => 'abenity',
    'required' => TRUE,
  );

  $variables['abenity_api_password'] = array(
    'title' => t('Abenity API password.', array(), $options),
    'description' => t('Abenity API password.', array(), $options),
    'type' => 'string',
    'group' => 'abenity',
    'required' => TRUE,
  );

  $variables['abenity_api_key'] = array(
    'title' => t('Abenity API key.', array(), $options),
    'description' => t('Abenity API key.', array(), $options),
    'type' => 'string',
    'group' => 'abenity',
    'required' => TRUE,
  );

  $variables['abenity_api_offer_radius'] = array(
    'title' => t('Offer radius ', array(), $options),
    'description' => t('The number of miles from zip to display offers. Value will be rounded to 10, 20 ,30 or 40, value to be set for users created.Enter 0 to skip.', array(), $options),
    'type' => 'string',
    'group' => 'abenity',
    'default_value' => 0,
  );

  $variables['abenity_api_welcome_email'] = array(
    'title' => t('Send welcome email on user creation via api ?', array(), $options),
    'description' => t('Check to send a welcome message to email upon successful registration. Uncheck if you do not want a message to be sent.', array(), $options),
    'type' => 'boolean',
    'group' => 'abenity',
  );

  $variables['abenity_api_spotlight'] = array(
    'title' => t('Subscribe member to monthly Spotlight email via api ?', array(), $options),
    'description' => t('Check to subscribe member to monthly Spotlight email messages. Uncheck to not subscribe.', array(), $options),
    'type' => 'boolean',
    'group' => 'abenity',
  );

  $variables['abenity_disabled_message'] = array(
    'title' => t('Abenity system disabled message', array(), $options),
    'description' => t('If Abenity system is disabled, what message should be shown to user on the page?.', array(), $options),
    'type' => 'text_format',
    'group' => 'abenity',
    'default' => "The perks systems is not available at the moment.",
  );

  $variables['abenity_failure_message'] = array(
    'title' => t('Abenity request failure message', array(), $options),
    'description' => t('If a abenity request fails for some reason, what message should be shown to user on the page?.', array(), $options),
    'type' => 'text_format',
    'group' => 'abenity',
    'default' => "The perks systems is not available at the moment.",
  );

  $variables['abenity_sso_privatekey'] = array(
    'title' => t('Abenity SSO private key', array(), $options),
    'description' => t('Private key to manage abenity sso authentication.', array(), $options),
    'type' => 'text',
    'group' => 'abenity',
    'default' => "",
  );

  $variables['abenity_failure_message_notify'] = array(
    'title' => t('Failure notification options', array(), $options),
    'description' => t('When abenity is down, and any api request fails, which requests should be notified via email ?.', array(), $options),
    'type' => 'options',
    'options' =>  array(
      'registerMember' => 'Register member',
      'ssoMember' => 'Login Url request',
      'deactivateMember' => 'Member deactivation',
      'reactivateMember' => 'Member Reactivation',
    ),
    'group' => 'abenity',
  );

  $variables['abenity_api_failure_email'] = array(
    'title' => t('Failure notification email', array(), $options),
    'description' => t('When abenity is down, and any api request fails, system will send email to this address about the incident.', array(), $options),
    'type' => 'string',
    'group' => 'abenity',
    'default_value' => variable_get('site_mail', NULL),
  );

  $variables['abenity_api_failure_email_message_[mail_part]'] = array(
    'title' => t('Failure notification email message'),
    'type' => 'mail_html',
    'description' => t('When abenity is down, and any api request fails, system will send this email message. Add token !abenity_request_details in email body to get the response data.', array(), $options),
    'children' => array(
      'abenity_api_failure_email_message_subject' => array(
        'default' => 'Abenity API request failure.',
      ),
      'abenity_api_failure_email_message_body' => array(
        'default' => "<p>Abenity API request failure Details : !abenity_request_details .</p>",
      ),
    ),
    'group' => 'abenity',
  );
  return $variables;
}
