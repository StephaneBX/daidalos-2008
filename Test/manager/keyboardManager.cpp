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

#include <fstream>
#include "../includes/global_var.h"
#include "../manager/soundManager.h"
#include "../manager/keyboardManager.h"

/**
* Constructor
*/
KeyboardManager::KeyboardManager()
{
	// Standard input mode
	this->inputMode = KEYBOARD_INPUT_MODE_NORMAL;

	this->keys = new int*[KEYBOARD_ARRAY_SIZE];
	for(int i = 0 ; i < KEYBOARD_ARRAY_SIZE ; i++)
	{
		if(i != KEYBOARD_MENU_VALID_KEY)
			this->keys[i] = new int[2];
		else
			this->keys[i] = new int[3];
	}

	// Initialize the keys
	initializeKeys();

	// Menu keys
	this->keys[KEYBOARD_MENU_UP_KEY][1]		= SDLK_UP;
	this->keys[KEYBOARD_MENU_DOWN_KEY][1]	= SDLK_DOWN;
	this->keys[KEYBOARD_MENU_LEFT_KEY][1]	= SDLK_LEFT;
	this->keys[KEYBOARD_MENU_RIGHT_KEY][1]	= SDLK_RIGHT;
	this->keys[KEYBOARD_MENU_CANCEL_KEY][1] = SDLK_ESCAPE;
	this->keys[KEYBOARD_MENU_VALID_KEY][1]	= SDLK_RETURN;
	this->keys[KEYBOARD_MENU_VALID_KEY][2]	= SDLK_KP_ENTER;

	// Game Keys
	this->keys[KEYBOARD_GAME_UP_KEY][1]					= SDLK_UP;
	this->keys[KEYBOARD_GAME_DOWN_KEY][1]				= SDLK_DOWN;
	this->keys[KEYBOARD_GAME_STRAFE_LEFT_KEY][1]		= SDLK_LEFT;
	this->keys[KEYBOARD_GAME_STRAFE_RIGHT_KEY][1]		= SDLK_RIGHT;
	this->keys[KEYBOARD_GAME_LEFT_KEY][1]				= SDLK_a;
	this->keys[KEYBOARD_GAME_RIGHT_KEY][1]				= SDLK_d;
	this->keys[KEYBOARD_GAME_LOOK_UP_KEY][1]			= SDLK_w;
	this->keys[KEYBOARD_GAME_LOOK_DOWN_KEY][1]			= SDLK_s;
	this->keys[KEYBOARD_GAME_FIRE_KEY][1]				= SDLK_RCTRL;
	this->keys[KEYBOARD_GAME_CENTER_VIEW_KEY][1]		= SDLK_q;
	this->keys[KEYBOARD_GAME_LIGHT_KEY][1]				= SDLK_l;
	this->keys[KEYBOARD_GAME_CROUCH_KEY][1]				= SDLK_c;
	this->keys[KEYBOARD_GAME_JUMP_KEY][1]				= SDLK_SPACE;
	this->keys[KEYBOARD_GAME_PREVIOUS_WEAPON_KEY][1]	= SDLK_PAGEDOWN;
	this->keys[KEYBOARD_GAME_NEXT_WEAPON_KEY][1]		= SDLK_PAGEUP;
	this->keys[KEYBOARD_GAME_RELOAD_KEY][1]				= SDLK_r;
	this->keys[KEYBOARD_GAME_WEAPON0_KEY][1]			= SDLK_F1;
	this->keys[KEYBOARD_GAME_WEAPON1_KEY][1]			= SDLK_F2;

	this->beepSound = SoundManager::getInstance()->loadSound("sounds/misc/beep.wav");

	// Open keyboard config file
	std::ifstream file;
	file.open("conf/keyboard.dat", std::ios::in);

	int tmp;

	// Read file
	if(file)
    {
		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_UP_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_DOWN_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_STRAFE_LEFT_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_STRAFE_RIGHT_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_LEFT_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_RIGHT_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_LOOK_UP_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_LOOK_DOWN_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_FIRE_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_LIGHT_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_CROUCH_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_PREVIOUS_WEAPON_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_NEXT_WEAPON_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_RELOAD_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_WEAPON0_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_WEAPON1_KEY][1] = tmp;

		if( file >> tmp > 0 )
			this->keys[KEYBOARD_GAME_JUMP_KEY][1] = tmp;

		file.close();
	}
}

/**
* Destructor
*/
KeyboardManager::~KeyboardManager()
{
	for(int i = 0 ; i < KEYBOARD_ARRAY_SIZE ; i++)
	{
		delete this->keys[i];
		this->keys[i] = NULL;
	}
	delete this->keys;
	this->keys = NULL;
}

/**
* Initialize the keys
*/
void KeyboardManager::initializeKeys()
{
	// Menu keys
	this->keys[KEYBOARD_MENU_UP_KEY][0]		= 0;
	this->keys[KEYBOARD_MENU_DOWN_KEY][0]	= 0;
	this->keys[KEYBOARD_MENU_LEFT_KEY][0]	= 0;
	this->keys[KEYBOARD_MENU_RIGHT_KEY][0]	= 0;
	this->keys[KEYBOARD_MENU_CANCEL_KEY][0] = 0;
	this->keys[KEYBOARD_MENU_VALID_KEY][0]	= 0;

	// Game Keys
	this->keys[KEYBOARD_GAME_CENTER_VIEW_KEY][0]		= 0;
	this->keys[KEYBOARD_GAME_CROUCH_KEY][0]				= 0;
	this->keys[KEYBOARD_GAME_DOWN_KEY][0]				= 0;
	this->keys[KEYBOARD_GAME_FIRE_KEY][0]				= 0;
	this->keys[KEYBOARD_GAME_JUMP_KEY][0]				= 0;
	this->keys[KEYBOARD_GAME_LEFT_KEY][0]				= 0;
	this->keys[KEYBOARD_GAME_LIGHT_KEY][0]				= 0;
	this->keys[KEYBOARD_GAME_LOOK_DOWN_KEY][0]			= 0;
	this->keys[KEYBOARD_GAME_LOOK_UP_KEY][0]			= 0;
	this->keys[KEYBOARD_GAME_NEXT_WEAPON_KEY][0]		= 0;
	this->keys[KEYBOARD_GAME_PREVIOUS_WEAPON_KEY][0]	= 0;
	this->keys[KEYBOARD_GAME_RELOAD_KEY][0]				= 0;
	this->keys[KEYBOARD_GAME_RIGHT_KEY][0]				= 0;
	this->keys[KEYBOARD_GAME_STRAFE_LEFT_KEY][0]		= 0;
	this->keys[KEYBOARD_GAME_STRAFE_RIGHT_KEY][0]		= 0;
	this->keys[KEYBOARD_GAME_UP_KEY][0]					= 0;
	this->keys[KEYBOARD_GAME_WEAPON0_KEY][0]			= 0;
	this->keys[KEYBOARD_GAME_WEAPON1_KEY][0]			= 0;
}

/**
* Refresh the keyboard events
* @param event SDL event
* @param gameMode Current game mode
*/
void KeyboardManager :: keyManagment(SDL_Event event, int gameMode)
{
	this->event = event;
	if(event.type == SDL_KEYDOWN)
	{
		if(this->inputMode == KEYBOARD_INPUT_MODE_NORMAL)
		{
			if(gameMode == MODE_INGAME)
			{
				if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_UP_KEY][1])
				{
					this->keys[KEYBOARD_GAME_UP_KEY][0] = 1;
					if(this->keys[KEYBOARD_GAME_DOWN_KEY][0] == 1)
						this->keys[KEYBOARD_GAME_DOWN_KEY][0] = 2;
				}
				else
					if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_DOWN_KEY][1])
					{
						this->keys[KEYBOARD_GAME_DOWN_KEY][0] = 1;
						if(this->keys[KEYBOARD_GAME_UP_KEY][0] == 1)
							this->keys[KEYBOARD_GAME_UP_KEY][0] = 2;
					}
				else
					if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_STRAFE_LEFT_KEY][1])
					{
						this->keys[KEYBOARD_GAME_STRAFE_LEFT_KEY][0] = 1;
						if(this->keys[KEYBOARD_GAME_STRAFE_RIGHT_KEY][0] == 1)
							this->keys[KEYBOARD_GAME_STRAFE_RIGHT_KEY][0] = 2;
					}
				else
					if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_STRAFE_RIGHT_KEY][1])
					{
						this->keys[KEYBOARD_GAME_STRAFE_RIGHT_KEY][0] = 1;
						if(this->keys[KEYBOARD_GAME_STRAFE_LEFT_KEY][0] == 1)
							this->keys[KEYBOARD_GAME_STRAFE_LEFT_KEY][0] = 2;
					}
				else
					if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_LEFT_KEY][1])
					{
						this->keys[KEYBOARD_GAME_LEFT_KEY][0] = 1;
						if(this->keys[KEYBOARD_GAME_RIGHT_KEY][0] == 1)
							this->keys[KEYBOARD_GAME_RIGHT_KEY][0] = 2;
					}
				else
					if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_RIGHT_KEY][1])
					{
						this->keys[KEYBOARD_GAME_RIGHT_KEY][0] = 1;
						if(this->keys[KEYBOARD_GAME_LEFT_KEY][0] == 1)
							this->keys[KEYBOARD_GAME_LEFT_KEY][0] = 2;
					}
				else
					if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_LOOK_UP_KEY][1])
					{
						this->keys[KEYBOARD_GAME_LOOK_UP_KEY][0] = 1;
						if(this->keys[KEYBOARD_GAME_LOOK_DOWN_KEY][0] == 1)
							this->keys[KEYBOARD_GAME_LOOK_DOWN_KEY][0] = 2;
					}
				else
					if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_LOOK_DOWN_KEY][1])
					{
						this->keys[KEYBOARD_GAME_LOOK_DOWN_KEY][0] = 1;
						if(this->keys[KEYBOARD_GAME_LOOK_UP_KEY][0] == 1)
							this->keys[KEYBOARD_GAME_LOOK_UP_KEY][0] = 2;
					}
				else if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_CROUCH_KEY][1])
						this->keys[KEYBOARD_GAME_CROUCH_KEY][0] = 1;
				else if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_JUMP_KEY][1])
						this->keys[KEYBOARD_GAME_JUMP_KEY][0] = 1;
				else if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_FIRE_KEY][1])
						this->keys[KEYBOARD_GAME_FIRE_KEY][0] = 1;
				else if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_CENTER_VIEW_KEY][1])
						this->keys[KEYBOARD_GAME_CENTER_VIEW_KEY][0] = 1;
				else if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_LIGHT_KEY][1])
						this->keys[KEYBOARD_GAME_LIGHT_KEY][0] = 1;
				else if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_PREVIOUS_WEAPON_KEY][1])
						this->keys[KEYBOARD_GAME_PREVIOUS_WEAPON_KEY][0] = 1;
				else if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_NEXT_WEAPON_KEY][1])
						this->keys[KEYBOARD_GAME_NEXT_WEAPON_KEY][0] = 1;
				else if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_RELOAD_KEY][1])
						this->keys[KEYBOARD_GAME_RELOAD_KEY][0] = 1;
				else if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_WEAPON0_KEY][1])
						this->keys[KEYBOARD_GAME_WEAPON0_KEY][0] = 1;
				else if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_WEAPON1_KEY][1])
						this->keys[KEYBOARD_GAME_WEAPON1_KEY][0] = 1;
			}
			else
			{
				if(event.key.keysym.sym == this->keys[KEYBOARD_MENU_VALID_KEY][1] || event.key.keysym.sym == this->keys[KEYBOARD_MENU_VALID_KEY][2])
					this->keys[KEYBOARD_MENU_VALID_KEY][0] = 1;
				else if(event.key.keysym.sym == this->keys[KEYBOARD_MENU_UP_KEY][1])
					this->keys[KEYBOARD_MENU_UP_KEY][0] = 1;
				else if(event.key.keysym.sym == this->keys[KEYBOARD_MENU_DOWN_KEY][1])
					this->keys[KEYBOARD_MENU_DOWN_KEY][0] = 1;
				else if(event.key.keysym.sym == this->keys[KEYBOARD_MENU_LEFT_KEY][1])
					this->keys[KEYBOARD_MENU_LEFT_KEY][0] = 1;
				else if(event.key.keysym.sym == this->keys[KEYBOARD_MENU_RIGHT_KEY][1])
					this->keys[KEYBOARD_MENU_RIGHT_KEY][0] = 1;
			}

			if(event.key.keysym.sym == this->keys[KEYBOARD_MENU_CANCEL_KEY][1])
				this->keys[KEYBOARD_MENU_CANCEL_KEY][0] = 1;
		}
		else
		{
			if(this->inputMode == KEYBOARD_INPUT_MODE_WAITING_FOR_INPUT)
			{
				if(event.key.keysym.sym != SDLK_ESCAPE && event.key.keysym.sym != SDLK_RETURN)
				{
					if(!isKeyAlreadyUsed(event.key.keysym.sym))
					{
						this->inputKey = event.key.keysym.sym;
						this->inputMode = KEYBOARD_INPUT_MODE_NEED_UPDATE;
					}
					else
						SoundManager::getInstance()->playSound(this->beepSound);
				}
				else
					this->inputMode = KEYBOARD_INPUT_MODE_NORMAL;
			}
		}
	}
	else
	{
		if(event.type == SDL_KEYUP)
		{
			if(gameMode == MODE_INGAME)
			{
				if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_UP_KEY][1])
				{
					this->keys[KEYBOARD_GAME_UP_KEY][0] = 0;
					if(this->keys[KEYBOARD_GAME_DOWN_KEY][0] == 2)
						this->keys[KEYBOARD_GAME_DOWN_KEY][0] = 1;
				}
				else
					if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_DOWN_KEY][1])
					{
						this->keys[KEYBOARD_GAME_DOWN_KEY][0] = 0;
						if(this->keys[KEYBOARD_GAME_UP_KEY][0] == 2)
							this->keys[KEYBOARD_GAME_UP_KEY][0] = 1;
					}
				else
					if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_STRAFE_LEFT_KEY][1])
					{
						this->keys[KEYBOARD_GAME_STRAFE_LEFT_KEY][0] = 0;
						if(this->keys[KEYBOARD_GAME_STRAFE_RIGHT_KEY][0] == 2)
							this->keys[KEYBOARD_GAME_STRAFE_RIGHT_KEY][0] = 1;
					}
				else
					if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_STRAFE_RIGHT_KEY][1])
					{
						this->keys[KEYBOARD_GAME_STRAFE_RIGHT_KEY][0] = 0;
						if(this->keys[KEYBOARD_GAME_STRAFE_LEFT_KEY][0] == 2)
							this->keys[KEYBOARD_GAME_STRAFE_LEFT_KEY][0] = 1;
					}
				else
					if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_LEFT_KEY][1])
					{
						this->keys[KEYBOARD_GAME_LEFT_KEY][0] = 0;
						if(this->keys[KEYBOARD_GAME_RIGHT_KEY][0] == 2)
							this->keys[KEYBOARD_GAME_RIGHT_KEY][0] = 1;
					}
				else
					if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_RIGHT_KEY][1])
					{
						this->keys[KEYBOARD_GAME_RIGHT_KEY][0] = 0;
						if(this->keys[KEYBOARD_GAME_LEFT_KEY][0] == 2)
							this->keys[KEYBOARD_GAME_LEFT_KEY][0] = 1;
					}
				else
					if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_LOOK_UP_KEY][1])
					{
						this->keys[KEYBOARD_GAME_LOOK_UP_KEY][0] = 0;
						if(this->keys[KEYBOARD_GAME_LOOK_DOWN_KEY][0] == 2)
							this->keys[KEYBOARD_GAME_LOOK_DOWN_KEY][0] = 1;
					}
				else
					if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_LOOK_DOWN_KEY][1])
					{
						this->keys[KEYBOARD_GAME_LOOK_DOWN_KEY][0] = 0;
						if(this->keys[KEYBOARD_GAME_LOOK_UP_KEY][0] == 2)
							this->keys[KEYBOARD_GAME_LOOK_UP_KEY][0] = 1;
					}
				else if(event.key.keysym.sym == this->keys[KEYBOARD_GAME_FIRE_KEY][1])
					this->keys[KEYBOARD_GAME_FIRE_KEY][0] = 0;
			}
		}
	}
}

/**
* Save the keyboard configuration
*/
void KeyboardManager::save()
{
	// Open keyboard config file
	std::ofstream file;
	file.open("conf/keyboard.dat", std::ios::out);

	// Save configuration
	if(file)
    {
		file << this->keys[KEYBOARD_GAME_UP_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_DOWN_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_STRAFE_LEFT_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_STRAFE_RIGHT_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_LEFT_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_RIGHT_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_LOOK_UP_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_LOOK_DOWN_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_FIRE_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_LIGHT_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_CROUCH_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_PREVIOUS_WEAPON_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_NEXT_WEAPON_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_RELOAD_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_WEAPON0_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_WEAPON1_KEY][1];
		file << '\n';
		file << this->keys[KEYBOARD_GAME_JUMP_KEY][1];
		file << '\n';
		file.close();
	}
}

/**
* Generic method to key a key
* @param v Key number
*/
int KeyboardManager::getKey(int v)
{
	return this->keys[v][1];
}

/**
* Return a key number
* Used in the control menu
* @param v Key number
*/
int KeyboardManager::getKeyNumber(int v)
{
	int ret = 0;
	switch(v)
	{
	case 0:
		ret = this->keys[KEYBOARD_GAME_UP_KEY][1];
		break;

	case 1:
		ret = this->keys[KEYBOARD_GAME_DOWN_KEY][1];
		break;

	case 2:
		ret = this->keys[KEYBOARD_GAME_STRAFE_LEFT_KEY][1];
		break;

	case 3:
		ret = this->keys[KEYBOARD_GAME_STRAFE_RIGHT_KEY][1];
		break;

	case 4:
		ret = this->keys[KEYBOARD_GAME_LEFT_KEY][1];
		break;

	case 5:
		ret = this->keys[KEYBOARD_GAME_RIGHT_KEY][1];
		break;

	case 6:
		ret = this->keys[KEYBOARD_GAME_CROUCH_KEY][1];
		break;

	case 7:
		ret = this->keys[KEYBOARD_GAME_JUMP_KEY][1];
		break;

	case 8:
		ret = this->keys[KEYBOARD_GAME_LOOK_UP_KEY][1];
		break;

	case 9:
		ret = this->keys[KEYBOARD_GAME_LOOK_DOWN_KEY][1];
		break;

	case 10:
		ret = this->keys[KEYBOARD_GAME_CENTER_VIEW_KEY][1];
		break;

	case 11:
		ret = this->keys[KEYBOARD_GAME_LIGHT_KEY][1];
		break;

	case 12:
		ret = this->keys[KEYBOARD_GAME_FIRE_KEY][1];
		break;

	case 13:
		ret = this->keys[KEYBOARD_GAME_PREVIOUS_WEAPON_KEY][1];
		break;

	case 14:
		ret = this->keys[KEYBOARD_GAME_NEXT_WEAPON_KEY][1];
		break;

	case 15:
		ret = this->keys[KEYBOARD_GAME_RELOAD_KEY][1];
		break;

	case 16:
		ret = this->keys[KEYBOARD_GAME_WEAPON0_KEY][1];
		break;

	case 17:
		ret = this->keys[KEYBOARD_GAME_WEAPON1_KEY][1];
		break;
	}
	return ret;
}

/**
* Update the key choosen
* Used in the control menu
* @param v Key number
*/
void KeyboardManager::updateKey(int v)
{
	switch(v)
	{
	case 0:
		this->keys[KEYBOARD_GAME_UP_KEY][1] = this->inputKey;
		break;

	case 1:
		this->keys[KEYBOARD_GAME_DOWN_KEY][1] = this->inputKey;
		break;

	case 2:
		this->keys[KEYBOARD_GAME_STRAFE_LEFT_KEY][1] = this->inputKey;
		break;

	case 3:
		this->keys[KEYBOARD_GAME_STRAFE_RIGHT_KEY][1] = this->inputKey;
		break;

	case 4:
		this->keys[KEYBOARD_GAME_LEFT_KEY][1] = this->inputKey;
		break;

	case 5:
		this->keys[KEYBOARD_GAME_RIGHT_KEY][1] = this->inputKey;
		break;

	case 6:
		this->keys[KEYBOARD_GAME_CROUCH_KEY][1] = this->inputKey;
		break;

	case 7:
		this->keys[KEYBOARD_GAME_JUMP_KEY][1] = this->inputKey;
		break;

	case 8:
		this->keys[KEYBOARD_GAME_LOOK_UP_KEY][1] = this->inputKey;
		break;

	case 9:
		this->keys[KEYBOARD_GAME_LOOK_DOWN_KEY][1] = this->inputKey;
		break;

	case 10:
		this->keys[KEYBOARD_GAME_CENTER_VIEW_KEY][1] = this->inputKey;
		break;

	case 11:
		this->keys[KEYBOARD_GAME_LIGHT_KEY][1] = this->inputKey;
		break;

	case 12:
		this->keys[KEYBOARD_GAME_FIRE_KEY][1] = this->inputKey;
		break;

	case 13:
		this->keys[KEYBOARD_GAME_PREVIOUS_WEAPON_KEY][1] = this->inputKey;
		break;

	case 14:
		this->keys[KEYBOARD_GAME_NEXT_WEAPON_KEY][1] = this->inputKey;
		break;

	case 15:
		this->keys[KEYBOARD_GAME_RELOAD_KEY][1] = this->inputKey;
		break;

	case 16:
		this->keys[KEYBOARD_GAME_WEAPON0_KEY][1] = this->inputKey;
		break;

	case 17:
		this->keys[KEYBOARD_GAME_WEAPON1_KEY][1] = this->inputKey;
		break;
	}
}

/**
* Generic method to know if a key is pressed
* @param v Key number
*/
bool KeyboardManager::isKeyPressed(int v)
{
	bool ret = false;
	if(this->keys[v][0] == 1)
	{
		ret = true;
		if(v == KEYBOARD_GAME_CENTER_VIEW_KEY || v == KEYBOARD_MENU_CANCEL_KEY)
			this->keys[v][0] = 0;
	}
	return ret;
}

/**
* Generic method to set a key
* @param key Key number
* @param v Value
*/
void KeyboardManager::setKey(int key, int v)
{
	this->keys[key][0] = v;
}

/**
* Return the input mode
* @return Input mode
*/
int KeyboardManager::getInputMode()
{
	return this->inputMode;
}

/**
* Change the input mode
* @param v New input mode
*/
void KeyboardManager::setInputMode(int v)
{
	this->inputMode = v;
}

/**
* Check if a key is already mapped
* @param key Key to test
* @return If the key is availible
*/
bool KeyboardManager::isKeyAlreadyUsed(int key)
{
	bool found = false;
	for(int i = KEYBOARD_GAME_UP_KEY ; i < KEYBOARD_ARRAY_SIZE && !found ; i++)
	{
		if(this->keys[i][1] == key)
			found = true;
	}
	return found;
}
