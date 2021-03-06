<?php
/**
 * @file
 *
 * Define the constants to be used in voipextension module.
 * Some constants are for denoting permissions and some are for standard CCK
 * Audio Field names to be used in VoIP directory case.
 */

/**
 * Define a permission, that the current user is having the permission of
 * personal VoIP extension or not.
 */
define('VOIPEXTENSION_HAVE_PERSONAL_EXTENSION', 'have a personal voip extension');
/**
 * The current user can access the VoIP information or not for the nodes and
 * for the user profiles.
 */
define('VOIPEXTENSION_ACCESS_EXTENSION_INFO', 'access voip extension information');
/**
 * Admin of the website is having rights to manage the VoIP extension module or not.
 */
define('VOIPEXTENSION_ADMINISTER_EXTENSIONS', 'administer voip extensions');
/**
 * Anonymous users can access the VoIP information or not from the node and
 * user detail pages.
 */
define('VOIPEXTENSION_ALLOW_ANONYMOUS_ACCESS', 'allow anonymous access to voip extension');

/**
 * Standard fieldset for holding voipextension fields.
 */
define('VOIPEXTENSION_GROUP_FIELD_NAME', 'group_voipextension_settings');

/**
 * Standard fieldset for holding voipdirectory fields.
 */
define('VOIPEXTENSION_VOIPDIRECTORY_GROUP_FIELD_NAME', 'group_voipdirectory_settings');

/**
 * VoIP Extension Field name in CCK Fields
 */
define('VOIPEXTENSION_EXTENSION_MAIN_FIELD', 'field_voipextension_main_field');

/**
 * Is voipextension field listed in VoIP directory or not.
 */
define('VOIPEXTENSION_EXTENSION_LISTED', _voipextension_directory_field_names('VOIPEXTENSION_EXTENSION_LISTED'));
/**
 * Audio name cck field.
 */
define('VOIPEXTENSION_AUDIO_NAME', _voipextension_directory_field_names('VOIPEXTENSION_AUDIO_NAME'));
/**
 * Audio description cck field
 */
define('VOIPEXTENSION_AUDIO_DESCRIPTION', _voipextension_directory_field_names('VOIPEXTENSION_AUDIO_DESCRIPTION'));
/**
 * Audio greeting cck field.
 */
define('VOIPEXTENSION_AUDIO_GREETING', _voipextension_directory_field_names('VOIPEXTENSION_AUDIO_GREETING'));
