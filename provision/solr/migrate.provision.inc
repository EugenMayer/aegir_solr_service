<?php

function drush_provision_drupal_post_provision_migrate($platform, $new_uri = null) {
  // thats just example code which cannot be used with the core-migrate ootb.
  // Basically when you migrate to a new server, you need to remove the old core
  // when you stay on the server, you dont need to do anything on migrate
  /* if(!drush_get_option('solr',0) && !drush_get_option('same_server',FALSE)) {
    $core_name = drush_get_option('solr_core_name',FALSE);
    drush_log("Deleting solr core on ".$old_server->remote_host." named $core_name");
    $old_server = _migrate_ng_get_server(d()->platform);
    if(!$core_name) {
      drush_log("Could not delete solr-core on ".$old_server->remote_host." as not core-name could be determined",'warning');
    }
    else {
      $old_server->service('solr')->destroy_site_solr_core();
    }
  }
  else {
    drush_log("Not deleting old solr-core on".$old_server->remote_host." named $core_name, as the site stays on the same server");
  }
  }*/
}