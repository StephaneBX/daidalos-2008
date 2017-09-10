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
#include "../manager/modManager.h"
#include "../manager/messageManager.h"
#include "../menu/modMenu.h"

/**
* Constructor
*/
ModMenu::ModMenu() : Menu()
{
	this->actionSize = ModManager::getInstance()->getNumberOfMod();
	this->topListIndex = 0;
	this->arrowUp = new Logo("imgs/menus/arrowUp.tga",0.9f,0.72f,0.03f,0.03f);
	this->arrowDown = new Logo("imgs/menus/arrowDown.tga",0.9f,0.15f,0.03f,0.03f);

	this->title = TEXT_MODLIST;
	this->previousMenuCode = ID_MENUMAIN;
	this->menuCode = ID_MENUMOD;
	this->choosenMenuCode = this->menuCode;
}

/**
* Destructor
*/
ModMenu::~ModMenu()
{
	delete this->arrowUp;
	this->arrowUp = NULL;
	delete this->arrowDown;
	this->arrowDown = NULL;
}

/**
* Refresh the Mod menu
* @param time Elapsed time
*/
void ModMenu::refresh(long time)
{
	if(keyM->isKeyPressed(KEYBOARD_MENU_UP_KEY))
	{
		keyM->setKey(KEYBOARD_MENU_UP_KEY,0);
		if(--this->actionChoosen < 0)
			this->actionChoosen = 0;

		if(this->actionChoosen < this->topListIndex)
			this->topListIndex--;
	}

	if(keyM->isKeyPressed(KEYBOARD_MENU_DOWN_KEY))
	{
		keyM->setKey(KEYBOARD_MENU_DOWN_KEY,0);
		if(++this->actionChoosen >= this->actionSize)
			this->actionChoosen = this->actionSize - 1;

		if(this->actionChoosen > (this->topListIndex + MAXMODDRAWN - 1))
			this->topListIndex++;
	}

	if(keyM->isKeyPressed(KEYBOARD_MENU_CANCEL_KEY))
			this->choosenMenuCode = this->previousMenuCode;

	if(keyM->isKeyPressed(KEYBOARD_MENU_VALID_KEY))
	{
		keyM->setKey(KEYBOARD_MENU_VALID_KEY,0);
		if(this->actionChoosen != ModManager::getInstance()->getModChoosenNumber())
		{
			ModManager::getInstance()->setModChoosen(this->actionChoosen);
			this->choosenMenuCode = this->previousMenuCode;
			MessageManager::getInstance()->reloadList();
		}
	}
}

/**
* Display the Mod menu
*/
void ModMenu::draw()
{
	glBindTexture  ( GL_TEXTURE_2D, this->image );
	glBegin(GL_QUADS);
		glTexCoord2d(0.0,0.0);			glVertex2d(0.0,0.0);
		glTexCoord2d(1.0,0.0);			glVertex2d(1.0,0.0);
		glTexCoord2d(1.0,this->yRatio);	glVertex2d(1.0,1.0); 	
		glTexCoord2d(0.0,this->yRatio);	glVertex2d(0.0,1.0);
	glEnd();

	float tmp = 1.0f - ((1.0f - (TEXTSPACE3 * this->actionSize)) / 2.0f) - (CHARACTERSCREENSIZE / 2.0f * this->actionSize);
	for(int i = this->topListIndex ; i < (this->topListIndex + MAXMODDRAWN) && i < ModManager::getInstance()->getNumberOfMod() ; i++)
	{
		if(i == actionChoosen)
		{
			if(i == ModManager::getInstance()->getModChoosenNumber())
			{
				std::ostringstream stringstream;
				stringstream << ModManager::getInstance()->getModName(i) << " -> " << langM->getText(TEXT_ACTIVATED);
				textM->drawText(stringstream.str(),0.5f,tmp,1.0f,TEXTMANAGER_ALIGN_CENTER,1.0f,0.5f,0.5f);
			}
			else
			{
				textM->drawText(ModManager::getInstance()->getModName(i),0.5f,tmp,1.0f,TEXTMANAGER_ALIGN_CENTER,1.0f,0.5f,0.5f);
			}
		}
		else
		{
			if(i == ModManager::getInstance()->getModChoosenNumber())
			{
				std::ostringstream stringstream;
				stringstream << ModManager::getInstance()->getModName(i) << " -> " << langM->getText(TEXT_ACTIVATED);
				textM->drawText(stringstream.str(),0.5f,tmp,1.0f,TEXTMANAGER_ALIGN_CENTER,0.5f,0.5f,1.0f);
			}
			else
			{
				textM->drawText(ModManager::getInstance()->getModName(i),0.5f,tmp,1.0f,TEXTMANAGER_ALIGN_CENTER,1.0f,1.0f,1.0f);
			}
		}
		tmp -= TEXTSPACE3;
	}

	if(this->topListIndex > 0)
		this->arrowUp->draw();

	if(this->topListIndex + MAXMODDRAWN < this->actionSize)
		this->arrowDown->draw();

	textM->drawText(langM->getText(TEXT_MODLIST),0.5f,0.8f,1.0f,TEXTMANAGER_ALIGN_CENTER,1.0f,1.0f,1.0f);
}
