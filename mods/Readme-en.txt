//------------------------------------------------------------------
// Daídalos Alpha Demo
// - Mods System
//------------------------------------------------------------------
// Daídalos allow you to add mods to your Daídalos game.
// The mod system allow you to:
// - Replace all the textures.
// - Replace all the sounds.
// - Define a levels list.
// - Choose the music for each level.
// - Modify entirely the interface.
// - Modify the ambient light value of each level.
// - Modify the initial objects given to the player.
// - Modify the starting health and armor of the player.
//
// The mod system will surely be improved in the futur if needed.
//
// All the mods must be put in the "mods" directory present at the
// root directory of the game. This directory contains at least the
// "main" directory which is essential for the game in order to run 
// correctly.
//
//------------------------------------------------------------------
// The content of a mod:
//------------------------------------------------------------------
// The arborescence of your mods must be as follow:
// (The elements between [ ] are optional)
//
//
// - Directory_of_the_mod (Directory)
//		-> levels (Directory)
//		-> conf.xml
//		-> [ imgs ] (Directory)
//		-> [ sounds ] (Directory)
//	
//
//
//------------------------------------------------------------------
// Directory_of_the_mod:
//------------------------------------------------------------------
// The main directory of your mod.
//
//------------------------------------------------------------------
// The levels directory:
//------------------------------------------------------------------
// This directory must contain all the levels written in the conf.xml 
// file
//
//------------------------------------------------------------------
// The conf.xml file:
//------------------------------------------------------------------
// This file contain the different settings of the mod (name, levels,
// sounds etc...)
// For a preview of the available settings your can consult the file
// mods/Main/conf.xml, which contains all the settings with their
// default value. A description of those settings is also available
// at the end of this document.
//
//------------------------------------------------------------------
// The imgs directory:
//------------------------------------------------------------------
// This directory permit to replace the textures in the img root
// directory in order to use your own textures.
// In order to replace a texture, you just have to respect the same
// arborescence. The game will then check on it's your directory
// in order to load your texture instead of the default one.
// The only texture format compatible is .tga. If you use and another
// format, it will surely make the game crash.
// It's not necessary to replace all the textures, you just have to
// replace only the textures you want to modify.
//
// The directories 0 | 1 | 2 | 3 in the imgs directory regroup the
// same textures but with different quality (0: very low quality - 
// 3: high quality).
// If you want to modify a texture, you have to provide all the
// quality versions
// The size of the textures is in general as follow:
// Directory 0: 2*2 size
// Directory 1: 128*128 size
// Directory 2: 256*256 size
// Directory 3: 512*512 size
// In general, a texture is 2 time smaller than its upper equivalent.
// The only exception is the 0 directory which contains 2*2 textures.
//	
//------------------------------------------------------------------
// The sounds directory:
//------------------------------------------------------------------
// The directory is the equivalant of the imgs directory, but for 
// the sounds. 
// You can override a sound or a music by creating you own 
// arborescence and by giving your sound files.
// All the sounds must be .wav files.
//
//------------------------------------------------------------------
// Mods's configuration information:
//------------------------------------------------------------------
// General:
// - name: The mod's name as it will appear in the mod menu.
// - number: The number of levels that will contains the mod.
//
// Relate to the levels:
// - position: Level's number in the list.
//   The minimum value is 0 and the maximum value is the number of
//   levels minus 1.
// - filename: The level filename. Do not include the extension.
// - music: The music filename. Do not include the extension.
// - ambient_light: The ambient light value of the level in percentage.
//   The default value is 100.
// - intersection_block: Define if the environment separators are
//   activated or not. The allowed values are enabled and disabled.
// - name: The name of the level as it will appear at the begining
//   of the level.
//
// Relate to the interface:
// - text_x: X coordinate of the text on the screen. The value must
//   be between 0 (left of the screen) and 1 (right of the screen).
// - text_y: Y coordinate of the text on the screen. The value must
//   be between 0 (bottom of the screen) and 1 (top of the screen).
// - logo_x: X coordinate of the icon. The value must
//   be between 0 (left of the screen) and 1 (right of the screen).
// - logo_y: Y coordinate of the icon. The value must
//   be between 0 (bottom of the screen) and 1 (top of the screen).
// - logo_size_x: X logo size.
// - logo_size_y: Y logo size.
//
// Relate to the player:
// - health_start: Starting health value. Must be between 1 and 100.
// - armor_start: Starting armor value. Must be between 1 and 100.
// - flashlight_start: Define if the player has the flashlight at the
//   begining of the mod. The allowed values are yes and no.
// - start: Define if a weapon is given to the player at the begining
//   of the mod. The allowed values are yes and no.
// - ammo: Define the number of ammo given to the player for a weapon
//   that has its start attribute to yes.
//
// Miscellaneous:
// - loading_text: Allow enabling text during a loading.
//   The allowed values are enabled and disabled.
// - custom_textures: Allow custom textures search in the
//   mod directory that will replace the default textures.
//   The allowed values are enabled and disabled.
// - custom_sounds: Allow custom sounds search in the
//   mod directory that will replace the default sounds.
//   The allowed values are enabled and disabled.
//------------------------------------------------------------------
