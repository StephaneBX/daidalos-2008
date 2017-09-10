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
#include "../../elements/2D/screen.h"

/**
* Constructor
* @param file 4/3 texture filename
* @param fileWide 16/10 texture filename
*/
Screen::Screen(std::string file, std::string fileWide)
{
	this->optM = OptionManager::getInstance();
	this->textureM = CTextureManager::getInstance();

	if(this->optM->getResolutionNumber() > 4)
		this->image = this->textureM->LoadTexture(fileWide);
	else
		this->image = this->textureM->LoadTexture(file);

	this->alpha = 1.0f;
}

/**
* Constructor
* @param file Texture filename
*/
Screen::Screen(std::string file)
{
	this->optM = OptionManager::getInstance();
	this->textureM = CTextureManager::getInstance();
	this->image = this->textureM->LoadTexture(file);
	this->alpha = 1.0f;
}

/**
* Destructor
*/
Screen::~Screen(){}

/**
* Refresh the Screen
*/
void Screen::refresh(){}

/**
* Refresh the Screen
@param time Elapsed time
*/
void Screen::refresh(long time){}

/**
* Refresh the Screen
* @param alpha Alpha value to apply
*/
void Screen::refresh(float alpha){}

/**
* Draw the Message
*/
void Screen::draw()
{
	glBindTexture(GL_TEXTURE_2D, this->image );
	glColor4f(1.0f, 1.0f, 1.0f,this->alpha);
	glBegin(GL_QUADS);
		glTexCoord2d(0.0,0.0);	glVertex2d(0.0,0.0);
		glTexCoord2d(1.0,0.0);	glVertex2d(1.0,0.0);
		glTexCoord2d(1.0,1.0);	glVertex2d(1.0,1.0); 	
		glTexCoord2d(0.0,1.0);	glVertex2d(0.0,1.0);
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f,1.0f);
}
