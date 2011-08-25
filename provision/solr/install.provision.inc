<?php
// $Id$

function drush_solr_provision_install_validate() {
  d()->service('solr')->connect();
}

function drush_solr_pre_provision_install() {
  d()->service('solr')->create_site_core();
}

function drush_solr_provision_install() {
  if (d()->type == 'site') {
    d()->service('solr')->create_config('site');
  }
  d()->service('solr')->create_site_core();
}

function drush_solr_post_provision_install() {
  d()->service('solr')->parse_configs();
}

function drush_solr_provision_install_rollback() {
  d()->service('solr')->destroy_site_core();
  d()->service('solr')->delete_config('site'); 
}

