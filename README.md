# drush-bde-env

Generates environment variables for the Behat Drupal Extension.

The configuration of the Behat Drupal Extension contains some
environment-specific parameters:

- The base URL.
- The root path.
- The paths to scan for subcontexts.

These parameters shouldn't be committed to the ``behat.yml`` configuration file
since these are different on development / staging / production environments.
Instead they should be passed to Behat by setting the ``$BEHAT_PARAMS``
environment variable. Unfortunately Behat expects this to be a JSON encoded
string which is very difficult to type or memorise.

This drush command generates the proper JSON encoded string for you, and allows
to save it to a file for easy execution.

It will default to the environment variables that are found in your Drupal
installation, but these can be overridden if needed. For example, Drush might
not know about your base URL and might report it as being 'http://default'.

## Installation

To make this drush command globally available, just download the command in
the ``.drush`` folder in your home directory, and clear the drush cache:

    $ cd ~/.drush
    $ git clone https://github.com/pfrenssen/drush-bde-env.git
    $ drush cc drush

## Usage

    $ drush bde-env-gen --base-url=http://localhost --site-root=/var/www/myproject --subcontexts=sites/all/modules config.local.sh

The following options are available:
- ``--base-url``: a fully qualified URL to use as the base URL. If omitted the
  default base URL will be used.
- ``--site-root``: the absolute path to the Drupal installation. If omitted the
  default root path will be used.
- ``--subcontexts``: a comma-separated list of paths to scan for subcontext
  files. These are the '*.behat.inc' files that provide step definitions for
  contributed module. If omitted this will default to 'sites/all/modules'.
- ``--skip-path-check``: by default the command will verify if the passed in
  site root and subcontext paths exist. Specify this option to skip this check.

Optionally a filename can be provided as an argument. If this is supplied the
environment variables will be written to the file, so they can easily be sourced
without having to type the drush command again:

    $ drush beg behat.local.sh
    $ source behat.local.sh

If no filename is provided the environment variables will be output in the
terminal.
