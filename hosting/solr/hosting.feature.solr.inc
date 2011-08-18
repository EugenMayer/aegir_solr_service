<?php

/**
 * @file
 *   Expose the solr feature to hostmaster.
 */

/**
 * Implementation of hook_hosting_feature().
 * 
 * Register the solr hosting feature with Aegir, initially this feature will
 * be disabled.
 */
function hosting_solr_hosting_feature() {
  $features['solr'] = array(
    // title to display in form
    'title' => t('Solr feature'),
    // description
    'description' => t('Solr feature documenting how to create your own extensions.'),
    // initial status ( HOSTING_FEATURE_DISABLED, HOSTING_FEATURE_ENABLED, HOSTING_FEATURE_REQUIRED )
    'status' => HOSTING_FEATURE_DISABLED,
    // module to enable/disable alongside feature
    'module' => 'hosting_solr',
    // associate with a specific node type.
    //  'node' => 'nodetype',
    // which group to display in ( null , experimental )
    'group' => 'experimental'
    );
  return $features;
}
