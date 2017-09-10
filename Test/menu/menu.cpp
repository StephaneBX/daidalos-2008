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

#include <GL/gl.h>
#include "../menu/menu.h"

/**
* Constructor
*/
Menu::Menu()
{
	this->title = -1;
	this->actionChoosen = 0;
	this->texturesManager = CTextureManager::getInstance();
	this->textM = TextManager::getInstance();
	this->langM = LangManager::getInstance();
	this->keyM = KeyboardManager::getInstance();
	this->optM = OptionManager::getInstance();
	this->soundM = SoundManager::getInstance();
	this->actionList = NULL;
	loadTexture();
}

/**
* Destructor
*/
Menu::~Menu()
{
	if(this->actionList != NULL)
	{
		delete this->actionList;
		this->actionList = NULL;
	}
}

/**
* Return the type of the menu
* @return Menu code
*/
int Menu::getMenuCode()
{
	return this->menuCode;
}

/**
* Load the texture
*/
void Menu::loadTexture()
{
	if(this->optM->getResolutionNumber() > 4)
	{
		this->image = this->texturesManager->LoadTexture("imgs/menus/title_wide.tga");
		this->yRatio = 0.625f;
	}
	else
	{
		this->image = this->texturesManager->LoadTexture("imgs/menus/title.tga");
		this->yRatio = 0.75f;
	}
}

/**
* Return the menu choosen
* @return Choosen menu code
*/
int Menu::getChoosenMenuCode()
{
	return this->choosenMenuCode;
}

/**
* Refresh the menu
* @param time Elapsed time
*/
void Menu::refresh(long time)
{
	if(keyM->isKeyPressed(KEYBOARD_MENU_UP_KEY))
	{
		keyM->setKey(KEYBOARD_MENU_UP_KEY,0);
		if(--this->actionChoosen < 0)
			this->actionChoosen = this->actionSize - 1;
	}

	if(keyM->isKeyPressed(KEYBOARD_MENU_DOWN_KEY))
	{
		keyM->setKey(KEYBOARD_MENU_DOWN_KEY,0);
		if(++this->actionChoosen >= this->actionSize)
			this->actionChoosen = 0;
	}

	if(keyM->isKeyPressed(KEYBOARD_MENU_CANCEL_KEY))
	{
		this->choosenMenuCode = this->previousMenuCode;
		if(this->menuCode == ID_MENUOPTION)
		{
			optM->save();
			keyM->save();
		}
	}
}

/**
* Display the menu
*/
void Menu::draw()
{
	glBindTexture  ( GL_TEXTURE_2D, image );
	glBegin(GL_QUADS);
		glTexCoord2d(0.0,0.0);			glVertex2d(0.0,0.0);
		glTexCoord2d(1.0,0.0);			glVertex2d(1.0,0.0);
		glTexCoord2d(1.0,this->yRatio);	glVertex2d(1.0,1.0); 	
		glTexCoord2d(0.0,this->yRatio);	glVertex2d(0.0,1.0);
	glEnd();

	float tmp = 1.0f - ((1.0f - (TEXTSPACE * this->actionSize)) / 2.0f) - (CHARACTERSCREENSIZE * this->actionSize);
	for(int i = 0 ; i < this->actionSize ; i++)
	{
		if(i == actionChoosen)
			textM->drawText(langM->getText(actionList[i]),0.5f,tmp,1.5f,TEXTMANAGER_ALIGN_CENTER,1.0f,0.5f,0.5f);
		else
			textM->drawText(langM->getText(actionList[i]),0.5f,tmp,1.5f,TEXTMANAGER_ALIGN_CENTER,1.0f,1.0f,1.0f);
		tmp -= TEXTSPACE;
	}
}
