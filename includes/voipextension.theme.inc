<?php
/**
 * @file
 *
 * Theme hooks and formatters implemented by the voipextension module.
 */

/**
 * Theme function to fixup voipextension element.
 *
 * @ingroup themable
 */
function theme_voipextension_element($element) {
  // Prevent spurious "Array" from appearing.
  unset($element['#value']);
  return !empty($element['#children']) ? $element['#children'] : '';
}

/**
 * Return an address for an individual item.
 */
function theme_voipextension_formatter_default($element) {
  if (empty($element['#item'])) {
    return '';
  }

  $stuff = $element['#item'];
  if (user_access(VOIPEXTENSION_ADMINISTER_EXTENSIONS)) {
    $flds = array('extension_name', 'script_name', 'veid', 'extension_status');
  }
  else {
    $flds = array('extension_name', 'veid');
  }

  $flds_names = array(
    'extension_name' => t('Ext. Name'),
    'script_name' => t('Script Name'),
    'veid' => t('Ext. Number'),
    'extension_status' => t('Ext. Status')
  );

  $ret = '<div class="voip_extension_info">';
  $sep = '';
  foreach ($flds as $fld) {
    if ($fld == 'extension_status') {
      if ($stuff[$fld] == 1) {
        $status = t('Enabled');
      }
      else {
        $status = t('Disabled');
      }
      $ret .= $sep . '<span class="' . $fld . '">' . '<b>'.$flds_names[$fld] .':</b> '. $status . '</span>';
    }
    else {
      $ret .= $sep . '<span class="' . $fld . '">' . '<b>'.$flds_names[$fld] .':</b> '. $stuff[$fld] . '</span>';
    }
    $sep = "<br />\n";
  }
  $ret .= '</div>';
 
  return $ret;
}

/**
 * Theme the audio player to be shown in the custom generic views.
 * @param $file
 *   An array having all the information about the file to be played.
 * @return
 *   Themeable output
 *
 * @todo Either use audioplayer theme function or use own custom one. still a question?
 */
function theme_voipextension_audio_player($file) {
  $output = '<div class="voipextension-audio-player">';

  global $base_path;
  $player = variable_get('audiofield_audioplayer', 0);
  
  $element = array();
  $element['#item'] = array();
  $element['#item']['filepath'] = $file['filepath'];
  
//  $output .= theme('audiofield_play_mp3', $element);

  switch ($player) {
    case 'flowplayer':
      $output .= theme('flowplayer', $base_path . $element['#item']['filepath'], 'audiofield', array(
                  'style' => 'height: 24px',
      ));
      break;
    case '0':
      $output .= '<object id="audioplayer2" height="24" width="120" data="' . $base_path . 'sites/all/libraries/player/audio-player/player.swf" type="application/x-shockwave-flash">
                  <param value="' . $base_path . 'sites/all/libraries/player/audio-player/player.swf" name="movie"/>
                  <param value="playerID=2&amp;bg=0xCDDFF3&amp;leftbg=0x357DCE&amp;lefticon=0xF2F2F2&amp;rightbg=0xF06A51&amp;rightbghover=0xAF2910&amp;righticon=0xF2F2F2&amp;righticonhover=0xFFFFFF&amp;text=0x357DCE&amp;slider=0x357DCE&amp;track=0xFFFFFF&amp;border=0xFFFFFF&amp;loader=0xAF2910&amp;soundFile=' . $base_path . $element['#item']['filepath'] . '" name="FlashVars"/>
                  <param value="high" name="quality"/>
                  <param value="false" name="menu"/>
                  <param value="wmode" name="transparent"/></object>';
      break;
    case '1':
      $output .= '<object type="application/x-shockwave-flash"  width="175" height="14"
                data="' . $base_path . 'sites/all/libraries/player/xspf_player_slim.swf?song_url=' . $base_path . $element['#item']['filepath'] . '">
                <param name="movie" value="' . $base_path . 'sites/all/libraries/player/xspf_player_slim.swf?song_url=' . $base_path . $element['#item']['filepath'] . '" />
                </object>';
      break;
    case '2':
      $output .= '<object><param name="autoplay" value="true" />
                <param name="controller"value="true" />
                <embed src="' . $base_path . 'sites/all/libraries/player/playerSinglePackage/playerSingle.swf"  width="192" height="80" float="left" wmode="transparent"
                flashvars="soundPath=' . $base_path . $element['#item']['filepath'] . '" autostart="true" loop="false"  controller="true" bgcolor="#FF9900" pluginspage="http://www.macromedia.com/go/getflashplayer" >
                </embed></object>';
      break;
    case '3':
      $output .= '<object><param name="autoplay" value="true" />
                  <param name="controller"value="true" />
                  <embed src="' . $base_path . 'sites/all/libraries/player/OriginalThinMusicPlayer.swf"  width="220" height="21" float="left" wmode="transparent"
                  flashvars="mediaPath=' . $base_path . $element['#item']['filepath'] . '&defaultVolume=100" autostart="true" loop="false"  controller="true" bgcolor="#FF9900" pluginspage="http://www.macromedia.com/go/getflashplayer" >
                  </embed></object>';
      break;
    case '4':
      $output .= '<object><param name="autoplay" value="true" />
                  <param name="controller"value="true" />
                  <embed src="' . $base_path . 'sites/all/libraries/player/LWMusicPlayer.swf"  width="65" height="21" float="left" wmode="transparent"
                  flashvars="mediaPath=' . $base_path . $element['#item']['filepath'] . '&defaultVolume=100" autostart="true" loop="false"  controller="true" bgcolor="#FF9900" pluginspage="http://www.macromedia.com/go/getflashplayer" >
                  </embed></object>';
      break;
  }
  $output .= '</div>';
  return $output;
}