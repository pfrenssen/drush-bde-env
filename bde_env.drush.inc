<?php

/**
 * @file
 * Drush command to generate environment variables for Behat Drupal Extension.
 */

/**
 * Implements hook_drush_command().
 */
function bde_env_drush_command() {
  return array(
    'bde-env-set' => array(
      'description' => 'Generate environment variables for the Behat Drupal Extension.',
      'aliases' => array('bes'),
      'arguments' => array(
        'filename' => 'The name of the file where the environment variables will be saved. This can be either an absolute path, or a path relative to the current working directory. If omitted the environment variables will be printed on the command line.',
      ),
      'options' => array(
        'base-url' => array(
          'description' => 'The base URL of the website. If omitted the default base URL will be used.',
          'example-value' => 'http://localhost',
        ),
        'site-root' => array(
          'description' => 'The absolute path to the website root. If omitted the default root path will be used.',
          'example-value' => '/var/www/myproject',
        ),
        'subcontexts' => array(
          'description' => 'A comma-separated list of paths to scan for \'*.behat.inc\' Behat subcontext files. Paths that do not start with a slash will be considered to be relative to the site root. If omitted this will default to \'sites/all/modules\'.',
          'example-value' => 'sites/all/modules,profiles/myprofile/modules',
        ),
      ),
    ),
  );
}

/**
 * Drush command to generate environment variables.
 */
function drush_bde_env_set($filename = '') {
  // Default to the site root and base URL from the site context.
  $site_root = rtrim(drush_get_option('site-root', drush_get_context('DRUSH_DRUPAL_ROOT')), '/');
  $base_url = rtrim(drush_get_option('base-url', drush_get_context('DRUSH_URI')), '/');
  
  // Default to 'sites/all/modules' for the subcontexts.
  $subcontexts = explode(',', drush_get_option('subcontexts', 'sites/all/modules'));

  // Convert the subcontexts into absolute paths.
  foreach ($subcontexts as &$subcontext) {
    if (substr($subcontext, 0, 1) !== '/') {
      $subcontext = $site_root . '/' . $subcontext;
    }
    $subcontext = realpath($subcontext);
  }
  
  $parameters = array(
    'extensions' => array(
      'Behat\MinkExtension' => array(
        'base_url' => $base_url,
      ),
      'Drupal\DrupalExtension' => array(
        'drupal' => array(
          'drupal_root' => $site_root,
        ),
        'subcontexts' => array(
          'paths' => $subcontexts,
        ),
      ),
    ),
  );

  $output = 'export BEHAT_PARAMS=' . escapeshellarg(json_encode($parameters, JSON_UNESCAPED_SLASHES));

  // If a filename is provided, write the parameters to a file.
  if (!empty($filename)) {
    // If the filename is relative, use the current working directory as root.
    if (substr($filename, 0, 1) !== '/') {
      $filename = drush_cwd() . '/' . $filename;
    }
    file_put_contents($filename, $output);
  }
  // No filename was provided, output the parameters on the command line.
  else {
    drush_print('Use the following command to set the environment variables for Behat:');
    drush_print($output);
  }
}