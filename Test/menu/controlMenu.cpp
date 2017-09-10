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

#include <sstream>
#include <GL/gl.h>
#include "../menu/controlMenu.h"

/**
* Constructor
*/
ControlMenu::ControlMenu() : Menu()
{
	this->actionSize = 18;
	this->topListIndex = 0;
	this->arrowUp = new Logo("imgs/menus/arrowUp.tga",0.9f,0.74f,0.03f,0.03f);
	this->arrowDown = new Logo("imgs/menus/arrowDown.tga",0.9f,0.23f,0.03f,0.03f);

	this->title = TEXT_CONTROL;
	this->previousMenuCode = ID_MENUOPTION;
	this->menuCode = ID_MENUOPTIONCONTROL;
	this->choosenMenuCode = this->menuCode;

	this->actionList = new int[this->actionSize];
	this->keyList = new int[this->actionSize];

	this->actionList[0]  = TEXT_CONTROLFORWARD;
	this->actionList[1]  = TEXT_CONTROLBACKWARD;
	this->actionList[2]  = TEXT_CONTROLSTRAFELEFT;
	this->actionList[3]  = TEXT_CONTROLSTRAFERIGHT;
	this->actionList[4]  = TEXT_CONTROLTURNLEFT;
	this->actionList[5]  = TEXT_CONTROLTURNRIGHT;
	this->actionList[6]  = TEXT_CONTROLCROUCH;
	this->actionList[7]  = TEXT_CONTROLJUMP;
	this->actionList[8]  = TEXT_CONTROLLOOKUP;
	this->actionList[9]  = TEXT_CONTROLLOOKDOWN;
	this->actionList[10] = TEXT_CONTROLCENTERVIEW;
	this->actionList[11] = TEXT_CONTROLFLASHLIGHT;
	this->actionList[12] = TEXT_CONTROLFIRE;
	this->actionList[13] = TEXT_CONTROLPREVIOUSWEAPON;
	this->actionList[14] = TEXT_CONTROLNEXTWEAPON;
	this->actionList[15] = TEXT_CONTROLRELOAD;
	this->actionList[16] = TEXT_CONTROLHANDGUN;
	this->actionList[17] = TEXT_CONTROLSHOTGUN;

	this->keyList[0]  = keyM->getKey(KEYBOARD_GAME_UP_KEY);
	this->keyList[1]  = keyM->getKey(KEYBOARD_GAME_DOWN_KEY);
	this->keyList[2]  = keyM->getKey(KEYBOARD_GAME_STRAFE_LEFT_KEY);
	this->keyList[3]  = keyM->getKey(KEYBOARD_GAME_STRAFE_RIGHT_KEY);
	this->keyList[4]  = keyM->getKey(KEYBOARD_GAME_LEFT_KEY);
	this->keyList[5]  = keyM->getKey(KEYBOARD_GAME_RIGHT_KEY);
	this->keyList[6]  = keyM->getKey(KEYBOARD_GAME_CROUCH_KEY);
	this->keyList[7]  = keyM->getKey(KEYBOARD_GAME_JUMP_KEY);
	this->keyList[8]  = keyM->getKey(KEYBOARD_GAME_LOOK_UP_KEY);
	this->keyList[9]  = keyM->getKey(KEYBOARD_GAME_LOOK_DOWN_KEY);
	this->keyList[10] = keyM->getKey(KEYBOARD_GAME_CENTER_VIEW_KEY);
	this->keyList[11] = keyM->getKey(KEYBOARD_GAME_LIGHT_KEY);
	this->keyList[12] = keyM->getKey(KEYBOARD_GAME_FIRE_KEY);
	this->keyList[13] = keyM->getKey(KEYBOARD_GAME_PREVIOUS_WEAPON_KEY);
	this->keyList[14] = keyM->getKey(KEYBOARD_GAME_NEXT_WEAPON_KEY);
	this->keyList[15] = keyM->getKey(KEYBOARD_GAME_RELOAD_KEY);
	this->keyList[16] = keyM->getKey(KEYBOARD_GAME_WEAPON0_KEY);
	this->keyList[17] = keyM->getKey(KEYBOARD_GAME_WEAPON1_KEY);
}

/**
* Destructor
*/
ControlMenu::~ControlMenu()
{
	delete[] this->keyList;
	this->keyList = NULL;
}

/**
* Refresh the Control menu
* @param time Elapsed time
*/
void ControlMenu::refresh(long time)
{
	if(keyM->getInputMode() == KEYBOARD_INPUT_MODE_NEED_UPDATE)
	{
		keyM->updateKey(this->actionChoosen);
		keyM->setInputMode(KEYBOARD_INPUT_MODE_NORMAL);
	}

	if(keyM->isKeyPressed(KEYBOARD_MENU_UP_KEY))
	{
		keyM->setKey(KEYBOARD_MENU_UP_KEY,0);
		if(keyM->getInputMode() == KEYBOARD_INPUT_MODE_NORMAL)
		{
			if(--this->actionChoosen < 0)
				this->actionChoosen = 0;

			if(this->actionChoosen < this->topListIndex)
				this->topListIndex--;
		}
	}

	if(keyM->isKeyPressed(KEYBOARD_MENU_DOWN_KEY))
	{
		keyM->setKey(KEYBOARD_MENU_DOWN_KEY,0);
		if(keyM->getInputMode() == KEYBOARD_INPUT_MODE_NORMAL)
		{
			if(++this->actionChoosen >= this->actionSize)
				this->actionChoosen = this->actionSize - 1;

			if(this->actionChoosen > (this->topListIndex + MAXCONTROLPERLINE - 1))
				this->topListIndex++;
		}
	}

	if(keyM->isKeyPressed(KEYBOARD_MENU_CANCEL_KEY))
	{
		if(keyM->getInputMode() == KEYBOARD_INPUT_MODE_NORMAL)
			this->choosenMenuCode = this->previousMenuCode;
		else
			if(keyM->getInputMode() == KEYBOARD_INPUT_MODE_WAITING_FOR_INPUT)
				keyM->setInputMode(KEYBOARD_INPUT_MODE_NORMAL);	
	}

	if(keyM->isKeyPressed(KEYBOARD_MENU_VALID_KEY))
	{
		keyM->setKey(KEYBOARD_MENU_VALID_KEY,0);
		if(keyM->getInputMode() == KEYBOARD_INPUT_MODE_NORMAL)
		{
			keyM->setInputMode(KEYBOARD_INPUT_MODE_WAITING_FOR_INPUT);
		}
	}
}

/**
* Display the Control menu
*/
void ControlMenu::draw()
{
	glBindTexture  ( GL_TEXTURE_2D, this->image );
	glBegin(GL_QUADS);
		glTexCoord2d(0.0,0.0);			glVertex2d(0.0,0.0);
		glTexCoord2d(1.0,0.0);			glVertex2d(1.0,0.0);
		glTexCoord2d(1.0,this->yRatio);	glVertex2d(1.0,1.0); 	
		glTexCoord2d(0.0,this->yRatio);	glVertex2d(0.0,1.0);
	glEnd();

	float tmp = 1.0f - ((1.0f - (TEXTSPACE3 * this->actionSize)) / 2.0f) - (CHARACTERSCREENSIZE / 2.0f * this->actionSize);
	for(int i = this->topListIndex ; i < (this->topListIndex + MAXCONTROLPERLINE) ; i++)
	{
		std::ostringstream stringstream, stringstream2;
		stringstream << langM->getText(actionList[i]) << " :";
		stringstream2 << " < " << SDL_GetKeyName( (SDLKey) keyM->getKeyNumber(i) ) << " >";

		if(i == actionChoosen)
		{
			if(keyM->getInputMode() == KEYBOARD_INPUT_MODE_NORMAL)
			{
				textM->drawText(stringstream.str(),0.5f,tmp,1.0f,TEXTMANAGER_ALIGN_RIGHT,1.0f,0.5f,0.5f);
				textM->drawText(stringstream2.str(),0.5f,tmp,1.0f,TEXTMANAGER_ALIGN_LEFT,1.0f,0.5f,0.5f);
			}
			else
			{
				textM->drawText(stringstream.str(),0.5f,tmp,1.0f,TEXTMANAGER_ALIGN_RIGHT,1.0f,0.5f,0.5f);
				textM->drawText(langM->getText(TEXT_CONTROLCHANGE),0.5f,tmp,1.0f,TEXTMANAGER_ALIGN_LEFT,1.0f,0.5f,0.5f);
			}
		}
		else
		{
			textM->drawText(stringstream.str(),0.5f,tmp,1.0f,TEXTMANAGER_ALIGN_RIGHT,1.0f,1.0f,1.0f);
			textM->drawText(stringstream2.str(),0.5f,tmp,1.0f,TEXTMANAGER_ALIGN_LEFT,1.0f,1.0f,1.0f);
		}
		tmp -= TEXTSPACE3;
	}

	if(this->topListIndex > 0)
		this->arrowUp->draw();

	if(this->topListIndex + MAXCONTROLPERLINE < this->actionSize)
		this->arrowDown->draw();
}
