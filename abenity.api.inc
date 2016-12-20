<?php

/**
 * Abenity api call wrapper
 */

/**
 * @returns abenity credentials along with app status
 */
function abenity_get_abenity_credentials() {
  $abenity_credentials = array(
    'abenity_api_enabled' => variable_get('abenity_api_enabled', NULL),
    'api_username' => variable_get('abenity_api_username', NULL),
    'api_password' => variable_get('abenity_api_password', NULL),
    'api_key' => variable_get('abenity_api_key', NULL),
    'sso_privatekey' => variable_get('abenity_sso_privatekey', NULL),
    'api_version' => variable_get('abenity_api_version', 2),
  );
  return $abenity_credentials;
}

/**
 * @returns user abenity credentials, false if one does not exists
 */
function abenity_get_user_credentials($user_data) {
  $abenity_username = $user_data->field_abenity_username[LANGUAGE_NONE][0]['value'];
  $abenity_password = $user_data->field_abenity_password[LANGUAGE_NONE][0]['value'];

  // Format the zip code
  $zip_code = $user_data->field_profile_address_address[LANGUAGE_NONE][0]['postal_code'];
  $zip_code = preg_replace('/\s+/', '', $zip_code);
  $country_code = $user_data->field_profile_address_address[LANGUAGE_NONE][0]['country'];
  if($country_code == 'US') {
    $zip_code = substr($zip_code, 0, 5);
  } else if ($country_code == 'CA') {
    $zip_code = substr($zip_code, 0, 6);
  }

  if(!empty($abenity_username) && !empty($abenity_password)) {
    $user_credentials = array(
      'username' => $abenity_username,
      'password' =>  $abenity_password,
      'client_user_id' => $user_data->name,
      'firstname' => $user_data->field_profile_firstname[LANGUAGE_NONE][0]['value'],
      'lastname' => $user_data->field_profile_lastname[LANGUAGE_NONE][0]['value'],
      'email' => $user_data->mail,
      'zip' => $zip_code,
      'country' => $country_code,
    );
    return $user_credentials;
  }
  return FALSE;
}

/**
 * @returns Register a new member and generate the login url and redirects user
 * to abenity if $redirect is true, or else returns the repsonse after user registration.
 */
function abenity_api_register_member($user, $redirect = FALSE) {
  // Register a member
  // Generate username as user's member number and Unix epoch time&nbsp;(i.e., 220156789-1433508175)
  $date = new DateTime();
  $member_id = $user->field_member_id[LANGUAGE_NONE][0]['value'];
  if(empty($member_id)) {
    $user_name = preg_replace('/\s+/', '', $user->name);
    $member_id = $user_name;
  }
  $username = $member_id . '-' . $date->getTimestamp();
  // Generate password
  $password = user_password();

  // Format the zip code
  $zip_code = $user->field_profile_address_address[LANGUAGE_NONE][0]['postal_code'];
  $zip_code = preg_replace('/\s+/', '', $zip_code);
  $country_code = $user->field_profile_address_address[LANGUAGE_NONE][0]['country'];
  if($country_code == 'US') {
    $zip_code = substr($zip_code, 0, 5);
  } else if ($country_code == 'CA') {
    $zip_code = substr($zip_code, 0, 6);
  }

  $abenity_member_data_params = array(
    'client_user_id' => $user->name,
    'firstname' => $user->field_profile_firstname[LANGUAGE_NONE][0]['value'],
    'lastname' => $user->field_profile_lastname[LANGUAGE_NONE][0]['value'],
    'address' => $user->field_profile_address_address[LANGUAGE_NONE][0]['thoroughfare'] . ' ' .$user->field_profile_address_address[LANGUAGE_NONE][0]['permises'],
    'city' => $user->field_profile_address_address[LANGUAGE_NONE][0]['locality'],
    'state' => $user->field_profile_address_address[LANGUAGE_NONE][0]['administrative_area'],
    'zip' => $zip_code,
    'country' => $country_code,
    'email' => $user->mail,
    'username' => $username,
    'password' => $password,
    'spotlight' => variable_get('abenity_api_spotlight', 1),
    'send_welcome_email' => variable_get('abenity_api_welcome_email', 1),
  );

  // Pass the configured offer radius
  $abenity_api_offer_radius = variable_get('abenity_api_offer_radius', NULL);
  if(!empty($abenity_api_offer_radius)) {
    $abenity_member_data_params['offer_radius'] = $abenity_api_offer_radius;
  }

  // If phone passes the preg match, then only send, orelse skip.
  $user_phone = $user->field_phone_number[LANGUAGE_NONE][0]['value'];
  $phone_pattern = '^(\()?([2-9]{1}[0-9]{2})([) -.]{0,2})'.'([0-9]{3})([- .]?)([0-9]{4}[ ]?)'.'(ext|Ext|x|X)?(\s|\.|\. )?(\d*)?$^';
  $check_phone_pattern = preg_match($phone_pattern, $user_phone, $matches, PREG_OFFSET_CAPTURE, 3);
  if($check_phone_pattern) {
    $abenity_member_data_params['phone'] = $user_phone;
  }

  $register_member_success = abenity_api_request_handler('registerMember', $abenity_member_data_params);
  if($register_member_success) {
    $user->field_abenity_username[LANGUAGE_NONE][0]['value'] = $username;
    $user->field_abenity_password[LANGUAGE_NONE][0]['value'] = $password;
    user_save($user);
    if($redirect) {
      // Generate login url and redirect
      $user_credentials = array(
        'client_user_id' => $user->name,
        'email' => $user->mail,
        'firstname' => $abenity_member_data_params['firstname'],
        'lastname' => $abenity_member_data_params['lastname'],
        'zip' => $abenity_member_data_params['zip'],
        'country' => $abenity_member_data_params['country'],
      );
      abenity_sso_login($user_credentials, TRUE);
    }
  } else {
    $output = variable_get('abenity_failure_message', 'The perks systems is not available at the moment.');
    return $output['value'];
  }
}

/**
 * @returns Generates user login url and redirects user to abenity if $redirect
 * is true, or else returns the repsonse.
 */
function abenity_sso_login($user_credentials, $redirect = FALSE) {
  $abenity_member_data_params = array(
    'client_user_id' => $user_credentials['client_user_id'],
    'creation_time' => gmdate("Y-m-d\TH:i:s\+0000"),
    'email' => $user_credentials['email'],
    'firstname' => $user_credentials['firstname'],
    'lastname' => $user_credentials['lastname'],
    'zip' => $user_credentials['zip'],
    'country' => $user_credentials['country'],
    'salt' => rand(0,100000),
    'send_welcome_email' => 0,
  );
  $member_login = abenity_api_request_handler('ssoMember', $abenity_member_data_params);
  if($member_login && $redirect) {
    // Redirect user to the abenity platform
    if(!empty($member_login->data)) {
      drupal_goto($member_login->data->token_url);
    }
  } else {
    $output = variable_get('abenity_failure_message', 'The perks systems is not available at the moment.');
    return $output;
  }
}

/**
 * Deactivate a user account in abenity.
 */
function abenity_api_deactivate_member($user_credentials) {
  $abenity_member_data_params = array(
    'client_user_id' => $user_credentials['client_user_id'],
  );
  abenity_api_request_handler('deactivateMember', $abenity_member_data_params);
}

/**
 * Reactivate a user account in abenity.
 */
function abenity_api_reactivate_member($user_credentials) {
  $abenity_member_data_params = array(
    'client_user_id' => $user_credentials['client_user_id'],
  );
  abenity_api_request_handler('reactivateMmber', $abenity_member_data_params);
}

/**
 * Reactivate a user account in abenity.
 */
function abenity_api_request_handler($request_method, $request_params) {

  $request_url  = variable_get('abenity_api_live_request_url', 'https://api.abenity.com/v2/client/');
  $sandbox_mode_enabled = variable_get('abenity_api_sandbox_mode', 0);
  if($sandbox_mode_enabled) {
    $request_url  = variable_get('abenity_api_sandbox_request_url', 'https://sandbox.abenity.com/v2/client/');
  }
  $abenity_credentials = abenity_get_abenity_credentials();
  module_load_include('php', 'abenity', 'abenity-php/vendor/phpseclib/phpseclib/phpseclib/Crypt/RSA');
  module_load_include('php', 'abenity', 'abenity-php/vendor/phpseclib/phpseclib/phpseclib/Math/BigInteger');
  module_load_include('php', 'abenity', 'abenity-php/lib/Abenity/ApiClient');
  $abenity = new \Abenity\ApiClient($abenity_credentials['api_username'], $abenity_credentials['api_password'], $abenity_credentials['api_key'], $abenity_credentials['api_version']);
  $private_key = $abenity_credentials['sso_privatekey'];
  if($request_method == 'ssoMember') {
    $abenity_response = $abenity->{$request_method}($request_params, $private_key);
  } else {
    $abenity_response = $abenity->{$request_method}($request_params);
  }

  if($abenity_response->status == 'ok') {
    // Successfull request
    switch ($request_method) {
      case 'registerMember':
        watchdog('abenity', 'Abenity member %abenity_username created.', array('%abenity_username' => $request_params['client_user_id']), WATCHDOG_DEBUG);
        break;
      case 'ssoMember':
        watchdog('abenity', 'Abenity member %abenity_username used one time login to access abenity platform.', array('%abenity_username' => $request_params['client_user_id']), WATCHDOG_DEBUG);
        return $abenity_response;
        break;
      case 'deactivateMember':
        watchdog('abenity', 'Abenity member %abenity_username deactivated.', array('%abenity_username' => $request_params['client_user_id']), WATCHDOG_DEBUG);
        break;
      case 'reactivateMember':
        watchdog('abenity', 'Abenity member %abenity_username reactivated.', array('%response_error' => $request_params['client_user_id']), WATCHDOG_DEBUG);
        break;
    }
    return TRUE;
  } else {
    if(!empty($abenity_response->error)) {
      $response_data['error'] = drupal_json_encode($abenity_response->error);
    }

    // If email notifcation is configured for this type of request, send email.
    $notification_request = variable_get('abenity_failure_message_notify', array());
    if(!empty($notification_request[$request_method])) {
      $default_email = variable_get('site_mail', NULL);
      $abenity_api_failure_email = variable_get('abenity_api_failure_email', $default_email);
      if(!empty($abenity_api_failure_email)) {
        $email_body = variable_get('abenity_api_failure_email_message_body', NULL);
        // Prepare the !abenity_request_details  variable
        $abenity_request_details = "Request " . $request_method . " failed with response error : " . $response_data['error'] . " for user with client_user_id : " . $request_params['client_user_id'];
        $email_body = str_replace('!abenity_request_details', $abenity_request_details,  $email_body['value']);
        $email_subject = variable_get('abenity_api_failure_email_message_subject', NULL);
        $message = 'New signup email address'; // Body of your email here.
        $params = array(
          'body' => $email_body,
          'subject' => $email_subject,
          'headers'=>'simple',
        );
        drupal_mail('abenity', 'api_notify', $abenity_api_failure_email, language_default(), $params, $default_email, TRUE);
      }
    }

    // Request failed
    watchdog('abenity', 'Request %request_method failed with response error %response_error . The request url was : %request_url', array('%request_method' => $request_method, '%response_error' => $response_data['error'], '%request_url' => $request_url), WATCHDOG_ERROR);
    watchdog('abenity', 'Failed request %request_method params. <pre>' . print_r($request_params, TRUE) . '</pre>. The request url was : %request_url', array('%request_method' => $request_method, '%request_url' => $request_url), WATCHDOG_ERROR);
    return FALSE;
  }
}
