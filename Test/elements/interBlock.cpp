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
#include "../includes/global_var.h"
#include "../manager/optionManager.h"
#include "../elements/interBlock.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param interBlockType Type of the InterBlock
* @param interBlockTexture InterBlock texture
* @param objectNumber GList number
* @param ambientLight Default ambient light value
*/
InterBlock::InterBlock(float x, float z, int interBlockType, int interBlockTexture, int objectNumber, int ambientLight)
{
	this->x = -x;
	this->z = z;
	this->interBlockType = interBlockType;
	this->objectNumber = objectNumber;
	this->interBlocQuality = (float)OptionManager::getInstance()->getPillarQuality();

	this->matSpec[0] = 0.1f;
	this->matSpec[1] = 0.1f;
	this->matSpec[2] = 0.1f;
	this->matSpec[3] = 1.0f;

	this->matDif[0] = 0.4f;
	this->matDif[1] = 0.4f;
	this->matDif[2] = 0.4f;
	this->matDif[3] = 1.0f;

	this->matAmb[0] = 0.4f * ambientLight / 100.0f;
	this->matAmb[1] = 0.4f * ambientLight / 100.0f;
	this->matAmb[2] = 0.4f * ambientLight / 100.0f;
	this->matAmb[3] = 1.0f;

	texturesManager = CTextureManager::getInstance();
	
	std::ostringstream stringstream;
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/pillar" << interBlockTexture << ".tga";
	this->interBlocTextureId = texturesManager->LoadTexture(stringstream.str());

	calculate();
}

/**
* Destructor
*/
InterBlock::~InterBlock(){}

/**
* Calculate the InterBlock
*/
void InterBlock::calculate()
{
	glNewList(this->objectNumber,GL_COMPILE); 
	glBindTexture  ( GL_TEXTURE_2D, this->interBlocTextureId );
	if(this->interBlockType == 1)
	{
		glBegin(GL_QUADS);
		for(int i = 0 ; i < this->interBlocQuality ; i++)
		{
			glNormal3f( 0.0f, 1.0f, 0.0f);
			glTexCoord2f(i/this->interBlocQuality		,INTERBLOCDEFAULTTICKNESS);		glVertex3f(x - INTERBLOCDEFAULTTICKNESS + (i/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f))			,	INTERBLOCHEIGHT, z + INTERBLOCDEFAULTTICKNESS);
			glTexCoord2f((i + 1)/this->interBlocQuality	,INTERBLOCDEFAULTTICKNESS);		glVertex3f(x - INTERBLOCDEFAULTTICKNESS + ((i + 1)/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f))	,	INTERBLOCHEIGHT, z + INTERBLOCDEFAULTTICKNESS);
			glTexCoord2f((i + 1)/this->interBlocQuality	,0.0f);							glVertex3f(x - INTERBLOCDEFAULTTICKNESS + ((i + 1)/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f))	,	INTERBLOCHEIGHT, z - INTERBLOCDEFAULTTICKNESS); 	
			glTexCoord2f(i/this->interBlocQuality		,0.0f);							glVertex3f(x - INTERBLOCDEFAULTTICKNESS + (i/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f))			,	INTERBLOCHEIGHT, z - INTERBLOCDEFAULTTICKNESS);

			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(i/this->interBlocQuality		,0.0f);						glVertex3f(x - INTERBLOCDEFAULTTICKNESS + (i/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f))		,	BLOCDEFAULTHEIGHT - INTERBLOCHEIGHT, z - INTERBLOCDEFAULTTICKNESS);
			glTexCoord2f((i + 1)/this->interBlocQuality	,0.0f);						glVertex3f(x - INTERBLOCDEFAULTTICKNESS + ((i + 1)/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f)),	BLOCDEFAULTHEIGHT - INTERBLOCHEIGHT, z - INTERBLOCDEFAULTTICKNESS);
			glTexCoord2f((i + 1)/this->interBlocQuality	,INTERBLOCDEFAULTTICKNESS);	glVertex3f(x - INTERBLOCDEFAULTTICKNESS + ((i + 1)/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f)),	BLOCDEFAULTHEIGHT - INTERBLOCHEIGHT, z + INTERBLOCDEFAULTTICKNESS); 	
			glTexCoord2f(i/this->interBlocQuality		,INTERBLOCDEFAULTTICKNESS);	glVertex3f(x - INTERBLOCDEFAULTTICKNESS + (i/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f))		,	BLOCDEFAULTHEIGHT - INTERBLOCHEIGHT, z + INTERBLOCDEFAULTTICKNESS);
		}
		glEnd();
	}
	else
	{
		if(this->interBlockType == 2)
		{
			glBegin(GL_QUADS);
			for(int i = 0 ; i < this->interBlocQuality ; i++)
			{
				glNormal3f( 0.0f, 1.0f, 0.0f);
				glTexCoord2f(0.0f						,i/this->interBlocQuality);			glVertex3f(x - INTERBLOCDEFAULTTICKNESS,	INTERBLOCHEIGHT2, z - INTERBLOCDEFAULTTICKNESS + (i/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f)));
				glTexCoord2f(0.0f						,(i + 1)/this->interBlocQuality);	glVertex3f(x - INTERBLOCDEFAULTTICKNESS,	INTERBLOCHEIGHT2, z - INTERBLOCDEFAULTTICKNESS + ((i + 1)/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f)));
				glTexCoord2f(INTERBLOCDEFAULTTICKNESS	,(i + 1)/this->interBlocQuality);	glVertex3f(x + INTERBLOCDEFAULTTICKNESS,	INTERBLOCHEIGHT2, z - INTERBLOCDEFAULTTICKNESS + ((i + 1)/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f))); 	
				glTexCoord2f(INTERBLOCDEFAULTTICKNESS	,i/this->interBlocQuality);			glVertex3f(x + INTERBLOCDEFAULTTICKNESS,	INTERBLOCHEIGHT2, z - INTERBLOCDEFAULTTICKNESS + (i/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f)));

				glNormal3f( 0.0f, -1.0f, 0.0f);
				glTexCoord2f(0.0f						,i/this->interBlocQuality);			glVertex3f(x - INTERBLOCDEFAULTTICKNESS,	BLOCDEFAULTHEIGHT - INTERBLOCHEIGHT2, z - INTERBLOCDEFAULTTICKNESS + (i/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f)));
				glTexCoord2f(INTERBLOCDEFAULTTICKNESS	,i/this->interBlocQuality);			glVertex3f(x + INTERBLOCDEFAULTTICKNESS,	BLOCDEFAULTHEIGHT - INTERBLOCHEIGHT2, z - INTERBLOCDEFAULTTICKNESS + (i/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f)));
				glTexCoord2f(INTERBLOCDEFAULTTICKNESS	,(i + 1)/this->interBlocQuality);	glVertex3f(x + INTERBLOCDEFAULTTICKNESS,	BLOCDEFAULTHEIGHT - INTERBLOCHEIGHT2, z - INTERBLOCDEFAULTTICKNESS + ((i + 1)/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f))); 	
				glTexCoord2f(0.0f						,(i + 1)/this->interBlocQuality);	glVertex3f(x - INTERBLOCDEFAULTTICKNESS,	BLOCDEFAULTHEIGHT - INTERBLOCHEIGHT2, z - INTERBLOCDEFAULTTICKNESS + ((i + 1)/this->interBlocQuality * (BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS * 2.0f)));
			}
			glEnd();
		}
	}
	glEndList();
}

/**
* Draw the InterBlock
*/
void InterBlock::draw()
{
	glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
	glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
	glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
	glMaterialf (GL_FRONT,GL_SHININESS,50.0f);
	glCallList(this->objectNumber);
}

/**
* Return the X coordinate
* @return X coordinate
*/
float InterBlock::getX()
{
	return this->x;
}

/**
* Return the Z coordinate
* @return Z coordinate
*/
float InterBlock::getZ()
{
	return this->z;
}

/**
* Return the GList number
* @return GList number
*/
int InterBlock::getObjectNumber()
{
	return this->objectNumber;
}
