<?php
// $Id$

/**
 * @file
 *   An solr of the provision service API.
 *
 *  Declares a new service type and a basic implementation of it.
 *  It matches the same service definition in the hosting front end.
 */

include_once(dirname(__FILE__) . '/../provision.service.inc');

/**
 * Expose the service type this extension defines to provision.
 *
 * Implementations are then conditionally loaded when the option 
 * "--$service_service_type=$type" is passed to the provision-save
 * command of a server.
 *
 * Implementations are automatically loaded from :
 * provision/$service/$type/$type_service.inc.
 *
 * @return
 *   An array with the service type the key, and the default implementation the value.
 */
function solr_provision_services() {
  return array('solr' => NULL);
}

/**
 * The service type base class.
 *
 * All implementations of the service type will inherit this class.
 * This class should define the 'public API' to be used by the rest
 * of the system, which should not expose implementation details.
 */
class provisionService_solr extends provisionService {
  public $service = 'solr';

  static function subscribe_site($context) {
    $context->setProperty('solr', drush_get_option('solr',0));
    $context->setProperty('solr_core_name', drush_get_option('site_machine_name',NULL));
  }

  /**
   * When a site gets verified, be sure the core exists. In case the core-alias changed
   * we just create a new one
   * @return v
   */
  function verify_site_cmd() {
    // TODO: if the old alias has change we should cleanup the old.. but how to remember what the old was
    // guess this should be a trigger in hostmaster
    $this->create_site_solr_core();
  }
  /**
   * Find a viable solr_core name, based on the site's uri.
   */
  function suggest_solr_core_name() {
    $uri = $this->context->uri;

    $suggest_base = 'aegir-' . substr(str_replace(array('.', '-'), '' , preg_replace('/^www\./', '', $uri)), 0);

    if (!$this->solr_core_exists($suggest_base)) {
      return $suggest_base;
    }

    for ($i = 0; $i < 100; $i++) {
      $option = sprintf("%s_%d", substr($suggest_base, 0, 15 - strlen( (string) $i) ), $i);
      if (!$this->solr_core_exists($option)) {
        return $option;
      }
    }

    drush_set_error('PROVISION_CREATE_SOLR_CORE_FAILED', dt("Could not find a free solr_core names after 100 attempts"));
    return false;
  }

  /**
   * Generate a new solr_core and user account for the specified credentials
   * TODO: why we do have create_site_solr_core and create_solr_core? One of those should be at least protected
   */
  function create_site_solr_core($creds = array(), $force = FALSE) {
    if (!sizeof($creds)) {
      $creds = $this->fetch_site_credentials();
    }
    extract($creds);

    $status = TRUE;
    // create the solr core if its not yet here
    if(!$this->solr_core_exists($solr_core_name) || $force) {
      $status = $this->create_solr_core($solr_core_name);
    }
    if ($status) {
      drush_log(dt('Created @name solr_core', array("@name" => $solr_core_name)), 'success');
    }
    else {
      drush_set_error('PROVISION_CREATE_SOLR_CORE_FAILED', dt("Could not create @name solr_core", array("@name" => $solr_core_name)));
    }
    return $status;
  }

  /**
   * Remove the solr_core for the supplied credentials
   */
  function destroy_site_solr_core($creds = array()) {
    if (!sizeof($creds)) {
      $creds = $this->fetch_site_credentials();
    }
    extract($creds);

    if ( $this->solr_core_exists($solr_core_name) ) {
      drush_log(dt("Purging solr_core @solr_core_name", array('@solr_core_name' => $solr_core_name)));
      if (!$this->purge_solr_core($solr_core_name)) {
        drush_log(dt("Failed to purge solr_core @solr_core_name", array('@solr_core_name' => $solr_core_name)), 'warning');
      }
    }

    if ( $this->solr_core_exists($solr_core_name) ) {
     drush_set_error('PROVISION_PURGE_SOLR_CORE_FAILED');
     return FALSE;
    }
    return TRUE;
  }

  function generate_site_credentials() {
    $creds = array();
    // Todo: why should we need this at all?
    return array();
  }

  function fetch_site_credentials() {
    $creds = array();
    $creds['solr'] = drush_get_option('solr', '');
    $creds['solr_core_name'] = drush_get_option('solr_core_name', '');

    if($creds['solr_core_name'] == '') {
      $creds['solr_core_name'] = $this->suggest_solr_core_name();
    }
    return $creds;
  }
  /**
   * Checks if a core already exists
   * @param the alias of the core $name
   * @return bool return true if the core exists
   */
  function solr_core_exists($name) {
    return FALSE;
  }
  /**
   * Deletes the alias and removes the dat-folder
   * @param the alias of the core $name
   * TODO: why bool?
   * @return bool ?
   */
  function purge_solr_core($name) {
    return FALSE;
  }

  /**
   * The actually command to create a core-alias on the satelite.
   * @param the alias of the core $name
   * @return bool return true if core could be created
   */
  function create_solr_core($name) {
    return FALSE;
  }

  function connect($name) {
    return FALSE;
  }
}