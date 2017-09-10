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
#include "../manager/optionManager.h"
#include "../manager/textManager.h"

/**
* Constructor
*/
TextManager::TextManager()
{
	this->texturesManager = CTextureManager::getInstance();
	OptionManager::getInstance()->getResolution(this->height, this->width);
	this->ratio = this->height / (float)this->width;
	loadText();
}

/**
* Destructor
*/
TextManager::~TextManager()
{
	this->extended.clear();
}

/**
* Load the text texture
*/
void TextManager::loadText()
{
	std::ostringstream stringstream;
	stringstream << "imgs/fonts/" << OptionManager::getInstance()->getTexturesQuality() << ".tga";
	this->fontId = texturesManager->LoadTexture(stringstream.str());
	OptionManager::getInstance()->getResolution(this->height, this->width);
	this->ratio = this->height / (float)this->width;
}

/**
* Draw the specified text at the specified place on the screen
* @param text Text
* @param x X coordinate
* @param y Y coordinate
* @param size Size
* @param type Horizontal alignment type
* @param r Red color
* @param g Green color
* @param b Blue color
* @param alpha Alpha value
*/
void TextManager::drawText(std::string text, float x, float y, float size, int type, float r, float g, float b, float alpha)
{
	// Center, right or left
	if(type == TEXTMANAGER_ALIGN_CENTER)
			x = x - (text.size() * CHARACTERSCREENSIZE / this->ratio * size) / 2.0f;
		else
			if(type == TEXTMANAGER_ALIGN_RIGHT)
				x = x - (text.size() * CHARACTERSCREENSIZE / this->ratio * size);

	// Draw
	glBindTexture  ( GL_TEXTURE_2D, fontId );
	for(unsigned int i = 0 ; i < text.size() ; i++)
	{
		float texX = ((unsigned char)text[i] - 32) % 16 * CHARACTERFONTSIZE;
		float texY = 1.0f - ((unsigned char)(((unsigned char)text[i] - 32) / 16) + 1) * CHARACTERFONTSIZE;
		if(texX < 0.0f || texX > 1.0f || texY < 0.0f || texY > 1.0f)
		{
			texX = 0.0;
			texY = 1.0f - CHARACTERFONTSIZE;
		}

		glColor4d(r,g,b,alpha);
		glBegin(GL_QUADS);
			glTexCoord2d(texX						,texY);							glVertex2d(x + i * (CHARACTERSCREENSIZE / this->ratio) * size		, y);
			glTexCoord2d(texX + CHARACTERFONTSIZE2	,texY);							glVertex2d(x + (i + 1) * (CHARACTERSCREENSIZE / this->ratio) * size	, y);
			glTexCoord2d(texX + CHARACTERFONTSIZE2	,texY + CHARACTERFONTSIZE2);	glVertex2d(x + (i + 1) * (CHARACTERSCREENSIZE / this->ratio) * size	, y + CHARACTERSCREENSIZE * size); 	
			glTexCoord2d(texX						,texY + CHARACTERFONTSIZE2);	glVertex2d(x + i * (CHARACTERSCREENSIZE / this->ratio) * size		, y + CHARACTERSCREENSIZE * size);
		glEnd();
		glColor3d(1.0,1.0,1.0);
	}
}
