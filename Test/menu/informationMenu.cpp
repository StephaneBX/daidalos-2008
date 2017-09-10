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
#include "../menu/informationMenu.h"

/**
* Constructor
*/
InformationMenu::InformationMenu() : Menu()
{
	this->menuCode = ID_MENUINFO;
	this->previousMenuCode = ID_MENUMAIN;
	this->choosenMenuCode = this->menuCode;
}

/**
* Destructor
*/
InformationMenu::~InformationMenu(){}

/**
* Refresh the Information menu
* @param time Elapsed time
*/
void InformationMenu::refresh(long time)
{
	if(keyM->isKeyPressed(KEYBOARD_MENU_CANCEL_KEY))
		this->choosenMenuCode = this->previousMenuCode;

	// Prevent from memorizing the validation key wich can cause glitches
	if(keyM->isKeyPressed(KEYBOARD_MENU_VALID_KEY))
		keyM->setKey(KEYBOARD_MENU_VALID_KEY,0);
}

/**
* Display the Information menu
*/
void InformationMenu::draw()
{
	glBindTexture  ( GL_TEXTURE_2D, image );
	glBegin(GL_QUADS);
		glTexCoord2d(0.0,0.0);			glVertex2d(0.0,0.0);
		glTexCoord2d(1.0,0.0);			glVertex2d(1.0,0.0);
		glTexCoord2d(1.0,this->yRatio);	glVertex2d(1.0,1.0); 	
		glTexCoord2d(0.0,this->yRatio);	glVertex2d(0.0,1.0);
	glEnd();

	textM->drawText(langM->getText(TEXT_DESCINFO1) ,0.06f,0.70f,0.9f,TEXTMANAGER_ALIGN_LEFT ,1.0f,1.0f,1.0f);
	textM->drawText(langM->getText(TEXT_DESCINFO2) ,0.06f,0.65f,0.9f,TEXTMANAGER_ALIGN_LEFT ,1.0f,1.0f,1.0f);
	textM->drawText(langM->getText(TEXT_DESCINFO3) ,0.06f,0.60f,0.9f,TEXTMANAGER_ALIGN_LEFT ,1.0f,1.0f,1.0f);
	textM->drawText(langM->getText(TEXT_DESCINFO4) ,0.06f,0.55f,0.9f,TEXTMANAGER_ALIGN_LEFT ,1.0f,1.0f,1.0f);
	textM->drawText(langM->getText(TEXT_DESCINFO5) ,0.06f,0.50f,0.9f,TEXTMANAGER_ALIGN_LEFT ,1.0f,1.0f,1.0f);
	textM->drawText(langM->getText(TEXT_DESCINFO6) ,0.06f,0.45f,0.9f,TEXTMANAGER_ALIGN_LEFT ,1.0f,1.0f,1.0f);
	textM->drawText(langM->getText(TEXT_DESCINFO7) ,0.06f,0.40f,0.9f,TEXTMANAGER_ALIGN_LEFT ,1.0f,1.0f,1.0f);
	textM->drawText(langM->getText(TEXT_DESCINFO8) ,0.06f,0.35f,0.9f,TEXTMANAGER_ALIGN_LEFT ,1.0f,1.0f,1.0f);
	textM->drawText(langM->getText(TEXT_DESCINFO9) ,0.06f,0.30f,0.9f,TEXTMANAGER_ALIGN_LEFT ,1.0f,1.0f,1.0f);
	textM->drawText(langM->getText(TEXT_DESCINFO10),0.06f,0.25f,0.9f,TEXTMANAGER_ALIGN_LEFT ,1.0f,1.0f,1.0f);
	textM->drawText(langM->getText(TEXT_DESCINFO11),0.95f,0.05f,0.9f,TEXTMANAGER_ALIGN_RIGHT,1.0f,1.0f,1.0f);
}
