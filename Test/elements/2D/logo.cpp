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
#include "../../manager/optionManager.h"
#include "../../manager/texture.h"
#include "../../elements/2D/logo.h"

/**
* Constructor
* @param logoPath Texture path used by the Logo
* @param x X coordinate on screen
* @param y Y coordinate on screen
* @param sizeX X size on screen
* @param sizeY Y size on screen
*/
Logo::Logo(std::string logoPath, float x, float y, float sizeX, float sizeY)
{
	this->texturesManager = CTextureManager::getInstance();
	this->logoId = texturesManager->LoadTexture(logoPath);
	OptionManager::getInstance()->getResolution(this->height, this->width);
	this->ratio = (float)this->height / (float)this->width;
	this->x = x;
	this->y = y;
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	if(OptionManager::getInstance()->getResolutionNumber() > 4)
		this->x += sizeX * 2.0f / 15.0f;
}

/**
* Destructor
*/
Logo::~Logo(){}

/**
* Draw the Logo
*/
void Logo::draw()
{
	// Draw
	glBindTexture  (GL_TEXTURE_2D, this->logoId);
	glBegin(GL_QUADS);
		glTexCoord2d(0.0,0.0);	glVertex2d(this->x , this->y);
		glTexCoord2d(1.0,0.0);	glVertex2d(this->x + this->sizeX / this->ratio, this->y);
		glTexCoord2d(1.0,1.0);	glVertex2d(this->x + this->sizeX / this->ratio, this->y + this->sizeY); 	
		glTexCoord2d(0.0,1.0);	glVertex2d(this->x , this->y + this->sizeY);
	glEnd();
}
