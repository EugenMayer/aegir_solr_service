<?php

/**
 * @file
 *   Provision hooks for the verify command
 **/

function drush_solr_post_provision_verify() {
  if (d()->type == 'site') {
    drush_log('*** type: ' . d()->type, 'warning');
    //if (d()->type == 'site') {
    //  d()->service('solr')->create_site_solr_core();
    //}
    drush_log('hi1', 'warning');
    drush_log('*** http port: ' . print_r(d()->get_services(), TRUE), 'warning');
    drush_log('* post get_services() *', 'warning');
    drush_log('*** http port: ' . print_r(d()->service('http')->default_port(), TRUE), 'warning');
    drush_log('*** solr port: ' . print_r(d()->service('solr')->default_port(), TRUE), 'warning');
    d()->service('solr')->create_site_solr_core();
    drush_log('hi2', 'warning');
  }
}

