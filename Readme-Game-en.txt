//------------------------------------------------------------------
// Daídalos Alpha Demo
//------------------------------------------------------------------
// Type: 3D FPS
// Language: C++
// Libraries: OpenGL, SDL, SDL_Mixer, TinyXML
// In progress since: Jully, 2008
// Latest version: December 16th, 2009
// Web site: http://www.stephane-baudoux.com
//------------------------------------------------------------------
//
//+Version 0.85
// New fonctionnalities:
// -Added a new trap "Trapped ground".
// -Added a new trap "Ark"
//
// Features modification:
// -Modification of the jump. It's now possible to slightly modify its trajectory during a jump.
// -Reduction of the activation radius of the "Pikes" and "Ventilation" traps.
// -Added a minimal distance for the traps to activate.
// -The locked doors now stay opened when they are unlocked.
// -Added a timer in the menus in order to avoid using to much ressources for nothing.
//
// Corrections:
// -Correction of the animations of the weapons on very fast computers.
// -Correction of the way projectiles are handled on very fast computers.
// -Correction of a bug where the jump wouldn't activate.
// -Correction of the way impacts behave on traps that are stuck on walls.
// -Correction of textures adjustment of the trap "Axes Pillar".
// -Reactivation of the mimaps. They were disabled for some times now, and their reactivation will provoke a performance gain.
//
// Please take notes that from now one the sources of the project are now available on my website. The licence is CC BY NC SA 2.0 for France and 3.0 for the Other countries.
// I hope my experience will benefits for someone even if the project is probably not the most interesting project.
//
//+Version 0.8
// New fonctionnalities:
//  -Added the objects Yellow, Red, Blue and Green Locked Door.
// -Added the objects Yellow, Red, Blue and Green Magnetic Card.
// -Added the 3D models of the handgun and the flashlight which are now also usable objects.
// -Added the object "Low Ceiling" which need the player to crouch in order cross it.
// -Added for traps: "Pikes", "Ventilation", "Arrow Tower" and "Axes Pillar".
// -Added wound magagement for the player (the wounds provokes bleedings).
// -Added message management when the player interact with the environment.
// -Added a debug mod (create a log file on the root directory).
// -Added an introduction picture.
// -Added some new elements in the interface.
// -Added the level editor which is now given together with the game.
//
// Features modification:
// -Huge update to the mod management:
// 		The configuration file is now an xml file, with new options available.
// 		Possibility to enable/disable custom textures.
// 		Possibility to choose which weapons are given by default and the amount of ammo given.
// 		Possibility to choose the default health and armor value.
// 		Possibility to choose if the player has the flashlight at the start of the mod.
// 		Possibility to disable loading texts.
// 		Possibility to adjust the ambient light value.
// 		Possibility to disable the environment separators.
// 		Possibility to change the coordinates and the size of each elements in the interface.
// 		Possibility to choose the default given magnetic cards
// -Adjustment of the sound volume calculation formula.
// -The configuration file of the game is now an xml file.
// -Suppression of the blinkings of some objects caused by the flashlight. 
// -It's now not possible to change direction during a jump.
// -Update of the Frustum Viewing Culling.
// -Suppression of a bug where a sound were not played.
// -The 1920*1200 resolution now works as intended.
// -Integration of the particles and the impacts in the Frustum Viewing Culling.
// -Suppression of a bug where it was not possible to reactivate the sound after putting it to 0.
// -Correction of the way impacts are shown when they are enlightened.
// -Menu textures have now power 2 size.
// -Modification of the way lang files are read in order to simplify futur additions.
// -Correction of an old memory leak.
//
//+Version 0.7
// -The project gets its final name: Daídalos
// -Correction: Big walls and small walls textures are now dependant of their coordinates which mean that aligned big walls / small walls will have correct textures alignment.
// -Correction: Errors on small walls texture.
// -Correction: Starting point of the player.
// -Correction: Impacts are now activated on the pillars
// -Added a maximum amount of carriable ammunitions for each weapon
// 		Handgun: 120
// 		Shotgun: 64
// -Added the object doors that can be of different size.
// -Added the possibility to jump.
// -Added the possibility to finish the levels (at last xD)
// -Added mod management, in order to permit custom levels by the community. For more information, please read the readme file in the mods folder.
// -Added: Working loading screen.
// -Added: 2D particles engine, with:
// 		Smoke on weapon cannon
// 		Smoke on non-metalical objects
// 		Sparks on non-wood objects
// 		Light particles on gates
// -Optimization: Portal space-division algorithm implementation.
// -Optimization: Frustum Viewing Culling algorithm implementation.
// -Optimization: Occlusion Culling algorithm implementation. Some work is still needed.
//
//+Version 0.416
// -Correction: The menu keys are not dependant on the choosen keys anymore, but are fixed to the directional buttons.
// -Correction: The validation button works on the notebooks with a keypad.
// -Correction: Resolution error: 1900x1080 -> 1920x1080.
// -Correction: Deletion of duplicates textures discharges.
// -Correction: Shooting non-stop with the shotgun could lead to a sound bug.
// -Added filetered resolutions in the display menu where only the supported resolutions are shown.
// -Added a dynamic display options changes. It's not needed anymore to restart the game for those options to take effect.
// -Added a sound when the user want to map an already used key.
// -If the resolution choosen is not supported, the game switch to 640*480 windowed. 
// -Added impacts management
// -Added shortcuts to weapons
// -Added a setting for the impacts number.
// -Modification of the shotgun: Each shot causes six impacts instead of one. So to maximise the damages, it's necessary that all impacts hits the target.
//
//+Version 0.345
// -Added key mouse management.
// -Added a scrollbar in the control menu.
// -Added three new keys in the control menu: Previous weapon, Next weapon, Reload.
// -Added shooting management:
// 		Each weapon posses 6 characteristics: Power, Speed, Reach, Loader size, Reload speed
// 		Precision is dependant of the distance between the target and the player, the state of the player (crouch, stand ...) and the precision modifier of the weapon.
// -Added different sounds on the bullets impacts depending of the material of the target.
// -Added two weapons: the handgun and the shotgun.
// -Added ammunitions for the shotgun.
// -Added the shotgun's ammunitions in the list of usable objects.
// -Correction of a bug where the in-game menu vanished if the user pressed enter before calling the menu.
// -Correction of a bug where some textures were not freed when coming back to the main menu.
// -Correction of a bug where it was necessary to restart the game in order for the texts quality to take effect.
// -Correction of the MD2 loader in order to be compatible with the backface culling.
// -Optimizations: Backface Culling activation
// -Optimizations: Level Of Detail implementation
// -The previous optimizations should lead to a much better fps
//
//+Version 0.178
// -Deletion of the "very hight" quality light option since it's not very much better than the "hight" option. 
// -Modification of the quality of some display options
// -Modification of the way the starting coordinates are stored in the levels files.
// -Modification of some textures.
// -Correction of a bug in the way gamma was saved
// -Correction of a bug were users had to restart the game for the mouse sensibility to take effect.
// -Correction of a texture name.
// -Correction of a missing texture.
// -Added an option to launch the levels from the editor.
// -Added the possibility to crouch.
// -Added multi-environment management for the levels
// -Added two new environments
// -Added footstep management 
// -Textures quality new also affect text quality
// -Deletion of a great amount of memory leak.
//
//+Version 0.102
// -Add a setting for smoothing the mouse movement
// -Modification of the impulsion of the player
//
//+Version 0.1
// Fonctionnalities:
// -Levels management
// -Keyboard management
// -Mouse management
// -Languages management
// -Sounds and musics management
// -Menus management
// -Interface management
// -Collisions management
// -Objects management
// -Possibility to configure the display
// -Possibility to configure the keyboard
// -French and english available
//
//-------------------------------------------------------------------
// Thanks
//-------------------------------------------------------------------
//+TGA and MD2 loading classes
// David Henry (http://tfc.duke.free.fr/)
//
//+XML Parser
// Lee Thomason, Yves Berquin et Andrew Ellerton pour tinyXML
// (http://www.grinninglizard.com/tinyxml/)
//
//+Sounds / Musics:
// Flick3r
// dynamedion (http://www.dynamedion.com)
// roocks (http://www.rookiecop.co.uk)
// Airborne Sound (http://www.airbornesound.com)
// SFX Bible (http://www.soundeffectsbible.com)
// BlastwaveFX (http://www.blastwavefx.com/)
// Shriek Productions (http://www.shriek-music.com/)
// Justine Angus
// Big Room Sound (http://www.bigroomsound.com/Home.html)
//
//+Textures
// http://www.cgtextures.com/
//-------------------------------------------------------------------
