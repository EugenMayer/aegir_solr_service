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

  function default_port() {
    return 8989;
  }

  /**
   * Register the solr handler for platforms, based on the solr_server option.
   */
  //static function subscribe_platform($context) {
  //  $context->setProperty('solr_server', '@server_master');
  //  $context->is_oid('solr_server');
  //  $context->service_subscribe('http', $context->solr_server->name);
  //}

  /**
   * Register the solr handler for sites, based on the solr_server option.
   */
  static function subscribe_site($context) {
    $context->setProperty('solr_server', '@server_master');
    $context->is_oid('solr_server');
    $context->service_subscribe('solr', $context->solr_server->name);
  }

  /**
   * Initialize the service along with the server object.
   */
  function init() {
    // REMEMBER TO CALL THE PARENT!
    parent::init();

    /**
     * We do not need to use this in our solr.
     *
     * You would extend this if you needed to save values
     * for all possible implementations of this service type.
     */
  }

  /**
   * Called on provision-verify.
   *
   * We change what we will do based on what the 
   * type of object the command is being run against.
   */
  /*
  function verify() {
    $this->create_config(d()->type);
    $this->parse_configs();
  }
  //*/

  /*
  function verify_server_cmd() {
    $this->create_config($this->context->type);
    $this->parse_configs();
  }
  //*/

  function verify_site_cmd() {
    //$this->create_config(d()->type);
    $this->create_config($this->context->type);
    $this->parse_configs();
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
        drush_log('* oh hai 2', 'warning');
        return $option;
      }
    }

    drush_log('* oh noes 2', 'warning');
    drush_set_error('PROVISION_CREATE_SOLR_CORE_FAILED', dt("Could not find a free solr_core names after 100 attempts"));
    return false;
  }

  /**
   * Generate a new mysql solr_core and user account for the specified credentials
   */
  function create_site_solr_core($creds = array()) {
    drush_log('hi3', 'warning');
    if (!sizeof($creds)) {
      //$creds = $this->generate_site_credentials();
      $creds['solr_core_name'] = $this->suggest_solr_core_name();
    }
    extract($creds);

    /*
    if (!$this->can_create_solr_core()) {
      drush_set_error('PROVISION_CREATE_SOLR_CORE_FAILED');
      drush_log("solr_core could not be created.", 'error');
      return FALSE;
    }
    //*/

    $this->create_solr_core($solr_core_name);
    //$status = $this->solr_core_exists($solr_core_name);
    $status = TRUE;

    if ($status) {
      drush_log(dt('Created @name solr_core', array("@name" => $solr_core_name)), 'success');
    }
    else {
      drush_set_error('PROVISION_CREATE_SOLR_CORE_FAILED', dt("Could not create @name solr_core", array("@name" => $solr_core_name)));
    }
    return $status;
  }

  /**
   * Remove the solr_core and user account for the supplied credentials
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
  }

  function generate_site_credentials() {
    $creds = array();
    // replace with service type
    $solr_core_type = drush_get_option('solr_core_type', 'apollo');

    //TODO - this should not be here at all according to db.drush.inc
    $creds['solr_core_type'] = drush_set_option('solr_core_type', $solr_core_type, 'site');
    $creds['solr_core_host'] = drush_set_option('solr_core_host', $this->server->remote_host, 'site');
    $creds['solr_core_port'] = drush_set_option('solr_core_port', $this->server->solr_core_port, 'site');
    $creds['solr_core_name'] = drush_set_option('solr_core_name', $this->suggest_solr_core_name(), 'site');

    return $creds;
  }


  function fetch_site_credentials() {
    $creds = array();

    $keys = array('solr_core_type', 'solr_core_port', 'solr_core_name', 'solr_core_host');
    foreach ($keys as $key) {
      $creds[$key] = drush_get_option($key, '', 'site');
    }

    return $creds;
  }

  function solr_core_exists($name) {
    return FALSE;
  }

  function purge_solr_core($name) {
    return FALSE;
  }

  function create_solr_core($name) {
    return FALSE;
  }

  function connect($name) {
    return FALSE;
  }

  /**
   * PUBLIC API!
   *
   * These are just a basic solr, the thing to notice here is that these
   * methods are just stubs for later implementations to extend.
   *
   * If a specific implementation doesn't need to parse the configuration files
   * that are generated for instance, it can just not implement the stub.
   */

  /**
   * Commonly something like running the restart_cmd or sending SIGHUP to a process.
   */
  function parse_configs() {
    return TRUE;
  }

  /**
   * Generate a site specific configuration file
   */
  function create_site_config() {
    return TRUE;
  }

  /**
   * Remove an existing site configuration file.
   */
  function delete_site_config() {
    return TRUE;
  }

  /**
   * Add a new platform specific configuration file.
   */
  function create_platform_config() {
    return TRUE;
  }

  /**
   * Remove an existing platform configuration file.
   */
  function delete_platform_config() {
    return TRUE;
  }

  /**
   * Create a new server specific configuration file.
   */
  function create_server_config() {
    return TRUE;
  }

  /**
   * Remove an existing server specific configuration file
   */
  function delete_server_config() {
    return TRUE;
  }
}
