<?php

/**
 * @file
 *   Provision hooks for the verify command
 **/

function drush_solr_post_provision_verify() {
  if (d()->type == 'site') {
    // TODO: Should check whether a core already exists
    drush_log('hi1', 'warning');
    d()->service('solr')->create_site_solr_core();
    drush_log('hi2', 'warning');
  }
}

