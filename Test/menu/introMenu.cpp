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
#include "../menu/introMenu.h"

/**
* Constructor
*/
IntroMenu::IntroMenu() : Menu()
{
	this->menuCode = ID_MENUINTRO;
	this->previousMenuCode = ID_MENUMAIN;
	this->choosenMenuCode = this->menuCode;
	this->time = 0;

	if(this->optM->getResolutionNumber() > 4)
		this->image2 = this->texturesManager->LoadTexture("imgs/menus/intro_wide.tga");
	else
		this->image2 = this->texturesManager->LoadTexture("imgs/menus/intro.tga");
}

/**
* Destructor
*/
IntroMenu::~IntroMenu(){}

/**
* Refresh the menu
* @param time Elapsed time
*/
void IntroMenu::refresh(long time)
{
	this->time += time;
	if(keyM->isKeyPressed(KEYBOARD_MENU_CANCEL_KEY) || keyM->isKeyPressed(KEYBOARD_MENU_VALID_KEY) || this->time >= INTROMENU_TIME + INTROMENU_WAIT_TIME * 2)
	{
		this->choosenMenuCode = this->previousMenuCode;
		keyM->setKey(KEYBOARD_MENU_VALID_KEY,0);
	}
}

/**
* Display the Introduction menu
*/
void IntroMenu::draw()
{
	float gammaValue;

	if(this->time <= INTROMENU_FADE_TIME)
	{
		gammaValue = (float)this->time / INTROMENU_FADE_TIME;
	}
	else
	{
		if(this->time  >= INTROMENU_TIME - INTROMENU_FADE_TIME)
			gammaValue = 1.0f - ((float)(this->time + INTROMENU_FADE_TIME - INTROMENU_TIME) / INTROMENU_FADE_TIME);
		else
			gammaValue = 1.0f;
	}

	glBindTexture  ( GL_TEXTURE_2D, this->image2 );
	glBegin(GL_QUADS);
		glColor4f(gammaValue,gammaValue,gammaValue,1.0f);
		glTexCoord2d(0.0,0.0);				glVertex2d(0.0,0.0);
		glTexCoord2d(1.0,0.0);				glVertex2d(1.0,0.0);
		glTexCoord2d(1.0,this->yRatio);		glVertex2d(1.0,1.0); 	
		glTexCoord2d(0.0,this->yRatio);		glVertex2d(0.0,1.0);
		glColor4f(1.0f,1.0f,1.0f,1.0f);
	glEnd();
}
