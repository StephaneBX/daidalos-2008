/*
==============================================================================================================================================================

Attribution-Noncommercial-Share Alike 3.0 Unported

You are free:
to Share - to copy, distribute and transmit the work
to Remix - to adapt the work

Under the following conditions:
Attribution - You must attribute the work in the manner specified by the author or licensor (but not in any way that suggests that they endorse you or your use of the work).
Noncommercial - You may not use this work for commercial purposes.
Share Alike - If you alter, transform, or build upon this work, you may distribute the resulting work only under the same or similar license to this one.

With the understanding that:
Waiver - Any of the above conditions can be waived if you get permission from the copyright holder.
Public Domain - Where the work or any of its elements is in the public domain under applicable law, that status is in no way affected by the license.
Other Rights - In no way are any of the following rights affected by the license:
Your fair dealing or fair use rights, or other applicable copyright exceptions and limitations;
The author's moral rights;
Rights other persons may have either in the work itself or in how the work is used, such as publicity or privacy rights.
Notice - For any reuse or distribution, you must make clear to others the license terms of this work. The best way to do this is with a link to this web page.

==============================================================================================================================================================
*/

#ifndef __KEYBOADMANAGER_H
#define __KEYBOADMANAGER_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "../includes/singleton.h"

enum
{
	KEYBOARD_MENU_UP_KEY,
	KEYBOARD_MENU_DOWN_KEY,
	KEYBOARD_MENU_LEFT_KEY,
	KEYBOARD_MENU_RIGHT_KEY,
	KEYBOARD_MENU_CANCEL_KEY,
	KEYBOARD_MENU_VALID_KEY,
	KEYBOARD_GAME_UP_KEY,
	KEYBOARD_GAME_DOWN_KEY,
	KEYBOARD_GAME_STRAFE_LEFT_KEY,
	KEYBOARD_GAME_STRAFE_RIGHT_KEY,
	KEYBOARD_GAME_LEFT_KEY,
	KEYBOARD_GAME_RIGHT_KEY,
	KEYBOARD_GAME_LOOK_UP_KEY,
	KEYBOARD_GAME_LOOK_DOWN_KEY,
	KEYBOARD_GAME_CENTER_VIEW_KEY,
	KEYBOARD_GAME_LIGHT_KEY,
	KEYBOARD_GAME_FIRE_KEY,
	KEYBOARD_GAME_CROUCH_KEY,
	KEYBOARD_GAME_JUMP_KEY,
	KEYBOARD_GAME_PREVIOUS_WEAPON_KEY,
	KEYBOARD_GAME_NEXT_WEAPON_KEY,
	KEYBOARD_GAME_RELOAD_KEY,
	KEYBOARD_GAME_WEAPON0_KEY,
	KEYBOARD_GAME_WEAPON1_KEY,
	KEYBOARD_ARRAY_SIZE
};

enum
{
	KEYBOARD_INPUT_MODE_NORMAL,
	KEYBOARD_INPUT_MODE_WAITING_FOR_INPUT,
	KEYBOARD_INPUT_MODE_NEED_UPDATE
};

/**
* KeyboardManager Class
* Handle the keyboard
* @author Stephane Baudoux
*/
class KeyboardManager : public CSingleton<KeyboardManager>
{
	friend class CSingleton<KeyboardManager>;

private:
	int inputMode;
	int inputKey;
	int ** keys;

	SDL_Event event;
	Mix_Chunk * beepSound;

public:
	void keyManagment(SDL_Event event, int gameMode);
	void save();
	void updateKey(int v);
	void initializeKeys();
	void setKey(int key, int v);
	void setInputMode(int v);

	int getKey(int v);
	int getKeyNumber(int v);
	int getInputMode();
	bool isKeyPressed(int v);

private:
	KeyboardManager();
	~KeyboardManager();
	bool isKeyAlreadyUsed(int key);
};

#endif
