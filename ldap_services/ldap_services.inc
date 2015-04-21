<?php
/**
 * @param $username
 * @param $password
 *
 * @return
 *  
 */
function _ldap_services_user_resource_login($username, $password) {
  watchdog("user","JH LDAP services called");
  global $user;
  if ($user->uid) {
    return services_error(t('Already logged in as @user.', array('@user' => $user->name)), 406);
  }
  if (module_load_include('inc', 'ldap_authentication')) {
    $form_state['values']['name'] = $username;
    $form_state['values']['pass'] = $password;
    $uid = _ldap_authentication_user_login_authenticate_validate($form_state)->uid;
  }
  if (!isset($uid)) {
    $uid = user_authenticate($username, $password);
  } 

  if ($uid) {
    $user = user_load($uid);
    if ($user->uid) {
      user_login_finalize();
      $return = new stdClass();
      $return->sessid = session_id();
      $return->session_name = session_name();
      services_remove_user_data($user);    
      $return->user = $user
      return $return;
    }
  }
  watchdog('user', 'Invalid login attempt for %username.', array('%username' => $username));
  return services_error(t('Wrong username or password. (ID:JH1)'), 401);
}