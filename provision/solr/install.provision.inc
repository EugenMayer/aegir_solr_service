<?php
// $Id$

function drush_solr_provision_install_validate() {
  //d()->service('solr')->connect();
}

function drush_solr_pre_provision_install() {
  if (d()->type == 'site') {
    //d()->service('solr')->create_solr_core();
  }
}

function drush_solr_provision_install() {
  if (d()->type == 'site') {
    d()->service('solr')->create_config('site');
    drush_log('hi1', 'warning');
    //drush_log('exists?: ' . function_exists(d()->service('solr')->create_site_solr_core()), 'warning');
    //d()->service('solr')->create_site_solr_core();
    drush_log('hi2', 'warning');
    //d()->service('apollo')->create_solr_core();
  }
}

function drush_solr_post_provision_install() {
  if (d()->type == 'site') {
    d()->service('solr')->parse_configs();
  }
}

function drush_solr_provision_install_rollback() {
  if (d()->type == 'site') {
    //d()->service('solr')->destroy_solr_core();
    //d()->service('solr')->delete_config('site');
  }
}

