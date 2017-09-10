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
#include "../menu/menuModification.h"

/**
* Constructor
*/
MenuModification::MenuModification() : Menu()
{
	this->modificationMoved = false;
}

/**
* Destructor
*/
MenuModification::~MenuModification()
{
	for(int i = 0 ; i < this->actionSize ; i++)
	{
		delete modificationList[i];
		modificationList[i] = NULL;
	} 
	delete modificationList;

	delete[] this->modificationSize;
	this->modificationSize = NULL;

	delete[] this->modificationChoosen;
	this->modificationChoosen = NULL;
}

/**
* Refresh the Menu Modification
* @param time Elapsed time
*/
void MenuModification::refresh(long time)
{
	Menu::refresh(time);
	if(keyM->isKeyPressed(KEYBOARD_MENU_LEFT_KEY))
	{
		this->keyM->setKey(KEYBOARD_MENU_LEFT_KEY,0);
		this->modificationMoved = true;
		if(--this->modificationChoosen[this->actionChoosen] < 0)
			this->modificationChoosen[this->actionChoosen] = this->modificationSize[this->actionChoosen] - 1;
	}

	if(keyM->isKeyPressed(KEYBOARD_MENU_RIGHT_KEY))
	{
		this->keyM->setKey(KEYBOARD_MENU_RIGHT_KEY,0);
		this->modificationMoved = true;
		if(++this->modificationChoosen[this->actionChoosen] >= this->modificationSize[this->actionChoosen])
			this->modificationChoosen[this->actionChoosen] = 0;
	}

	// Prevent from memorizing the validation key wich can cause glitches
	if(keyM->isKeyPressed(KEYBOARD_MENU_VALID_KEY))
		keyM->setKey(KEYBOARD_MENU_VALID_KEY,0);
}

/**
* Display the Menu Modification
*/
void MenuModification::draw()
{
	glBindTexture  ( GL_TEXTURE_2D, this->image );
	glBegin(GL_QUADS);
		glTexCoord2d(0.0,0.0);			glVertex2d(0.0,0.0);
		glTexCoord2d(1.0,0.0);			glVertex2d(1.0,0.0);
		glTexCoord2d(1.0,this->yRatio);	glVertex2d(1.0,1.0); 	
		glTexCoord2d(0.0,this->yRatio);	glVertex2d(0.0,1.0);
	glEnd();

	float tmp = 1.0f - ((1.0f - (TEXTSPACE2 * this->actionSize)) / 2.0f) - (CHARACTERSCREENSIZE / 2.0f * this->actionSize);
	for(int i = 0 ; i < this->actionSize ; i++)
	{
		std::ostringstream stringstream, stringstream2;
		stringstream << langM->getText(actionList[i]) << " :";
		stringstream2 << " < " << langM->getText(this->modificationList[i][this->modificationChoosen[i]]) << " > ";

		if(i == actionChoosen)
		{
			textM->drawText(stringstream.str(),0.5f,tmp,1.0f,TEXTMANAGER_ALIGN_RIGHT,1.0f,0.5f,0.5f);
			textM->drawText(stringstream2.str(),0.5f,tmp,1.0f,TEXTMANAGER_ALIGN_LEFT,1.0f,0.5f,0.5f);
		}
		else
		{
			textM->drawText(stringstream.str(),0.5f,tmp,1.0f,TEXTMANAGER_ALIGN_RIGHT,1.0f,1.0f,1.0f);
			textM->drawText(stringstream2.str(),0.5f,tmp,1.0f,TEXTMANAGER_ALIGN_LEFT,1.0f,1.0f,1.0f);
		}
		tmp -= TEXTSPACE2;
	}
}
