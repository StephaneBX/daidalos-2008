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

#include <stdlib.h>
#include <GL/gl.h>
#include "../elements/flash.h"

/**
* Constructor
* @param filePath Texture file path
* @param size Size
* @param depth Depth to the origin
*/
Flash::Flash(std::string filePath, float size)
{
	this->texturesManager = CTextureManager::getInstance();
	this->flashId = this->texturesManager->LoadTexture(filePath);
	this->angle = 0.0;
	this->size = size;
}

/**
* Destructor
*/
Flash::~Flash(){}

/**
* Draw the flash
*/
void Flash::draw()
{
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBindTexture  ( GL_TEXTURE_2D, this->flashId );
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f);	glVertex3f(this->size	, this->size	, 0);
		glTexCoord2f(0.0f,0.0f);	glVertex3f(this->size	, - this->size	, 0);
		glTexCoord2f(1.0f,0.0f);	glVertex3f(- this->size	, - this->size	, 0);
		glTexCoord2f(1.0f,1.0f);	glVertex3f(- this->size	, this->size	, 0);
	glEnd();
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
}

/**
* Change the flash angle
* @param v Change state
*/
void Flash::changeAngle(bool v)
{
	if(v && this->angle == 0.0f)
	{
		this->angle = (float)rand() / ((float)RAND_MAX + 1) * 359;
	}
	else
	{
		if(!v && this->angle != 0.0f)
			this->angle = 0.0f;
	}
}
