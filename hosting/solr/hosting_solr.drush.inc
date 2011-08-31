<?php


/**
 * Implementation of drush_hook_pre_hosting_task()
 */
function hosting_solr_hosting_site_context_options(&$task) {
  $task->context_options['solr_server'] = hosting_context_name($task->ref->solr_server);
  //$task->context_options['solr_server'] = $task->ref->solr_server;
}

