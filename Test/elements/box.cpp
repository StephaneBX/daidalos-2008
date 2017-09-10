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

#include <math.h>
#include <GL/gl.h>
#include "../manager/optionManager.h"
#include "../includes/global_var.h"
#include "../elements/box.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param boxType Type of box
* @param wallTextureId Texture id (already loaded in OpenGL)
* @param objectNumber GList number
*/
Box::Box(int x, int z, int boxType, GLuint wallTextureId, int objectNumber)
{
	// Copy variables
	this->boxType = boxType;
	this->wallTextureId = wallTextureId;
	this->texturesManager = CTextureManager::getInstance();
	this->objectNumber = objectNumber;
	this->blocQuality = (float) OptionManager::getInstance()->getBlocQuality();
	this->lightQuality = OptionManager::getInstance()->getLightQuality();

	// Affect variables depending on the box type
	switch(this->boxType)
	{
		case BOX_TYPE_LEFT:
		this->xMin = (float)x - BLOCDEFAULTTICKNESS;
		this->xMax = (float)x;
		this->zMin = (float)z;
		this->zMax = (float)z + BLOCDEFAULTSIZE;
		break;

		case BOX_TYPE_UP:
		this->xMin = (float)x - BLOCDEFAULTSIZE;
		this->xMax = (float)x;
		this->zMin = (float)z - BLOCDEFAULTTICKNESS;
		this->zMax = (float)z;
		break;

		case BOX_TYPE_RIGHT:
		this->xMin = (float)x;
		this->xMax = (float)x + BLOCDEFAULTTICKNESS;
		this->zMin = (float)z;
		this->zMax = (float)z + BLOCDEFAULTSIZE;
		break;

		case BOX_TYPE_DOWN:
		this->xMin = (float)x - BLOCDEFAULTSIZE;
		this->xMax = (float)x;
		this->zMin = (float)z;
		this->zMax = (float)z + BLOCDEFAULTTICKNESS;
		break;
	}

	calculate();
}

/**
* Destructor
*/
Box::~Box(){}

/**
* Calculate the Box
*/
void Box::calculate()
{
	// Boxes creation
	for(int j = 0; j <= this->lightQuality ; j++)
	{
		// Create the differents quality
		this->blocQuality = (float)OptionManager::getInstance()->getBlocQuality(j);
		glNewList(objectNumber + j,GL_COMPILE);
		switch(this->boxType)
		{
			case BOX_TYPE_LEFT:
				glBindTexture  ( GL_TEXTURE_2D, wallTextureId );
				glBegin(GL_QUADS);
				glNormal3f( -1.0f, 0.0f, 0.0f);
				for( int i = 0 ; i < this->blocQuality ; i++)
				{
					for( int j = 0 ; j < this->blocQuality ; j++)
					{
						glTexCoord2f(     j/this->blocQuality	, i/this->blocQuality);		glVertex3f(this->xMin, i/this->blocQuality * BLOCDEFAULTHEIGHT		, j/this->blocQuality * BLOCDEFAULTSIZE + this->zMin);
						glTexCoord2f( (j+1)/this->blocQuality	, i/this->blocQuality);		glVertex3f(this->xMin, i/this->blocQuality * BLOCDEFAULTHEIGHT		, (j+1)/this->blocQuality * BLOCDEFAULTSIZE + this->zMin);
						glTexCoord2f( (j+1)/this->blocQuality	,(i+1)/this->blocQuality);	glVertex3f(this->xMin, (i+1)/this->blocQuality * BLOCDEFAULTHEIGHT	, (j+1)/this->blocQuality * BLOCDEFAULTSIZE + this->zMin);
						glTexCoord2f(    j/this->blocQuality	,(i+1)/this->blocQuality);	glVertex3f(this->xMin, (i+1)/this->blocQuality * BLOCDEFAULTHEIGHT	, j/this->blocQuality * BLOCDEFAULTSIZE + this->zMin);
					}
				}
				glEnd();
			break;

			case BOX_TYPE_UP:
				glBindTexture  ( GL_TEXTURE_2D, wallTextureId );
				glBegin(GL_QUADS);
				glNormal3f( 0.0f, 0.0f, -1.0f);
				for( int i = 0 ; i < this->blocQuality ; i++)
				{
					for( int j = 0 ; j < this->blocQuality ; j++)
					{
						glTexCoord2f( (j + 1)/this->blocQuality	, i/this->blocQuality);			glVertex3f((j + 1)/this->blocQuality * BLOCDEFAULTSIZE + this->xMin		, i/this->blocQuality * BLOCDEFAULTHEIGHT		, this->zMin);
						glTexCoord2f( j/this->blocQuality		, i/this->blocQuality);			glVertex3f(j/this->blocQuality * BLOCDEFAULTSIZE + this->xMin			, i/this->blocQuality * BLOCDEFAULTHEIGHT		, this->zMin);
						glTexCoord2f( j/this->blocQuality		,(i+1)/this->blocQuality);		glVertex3f(j/this->blocQuality * BLOCDEFAULTSIZE + this->xMin			, (i+1)/this->blocQuality * BLOCDEFAULTHEIGHT	, this->zMin);
						glTexCoord2f( (j + 1)/this->blocQuality	,(i+1)/this->blocQuality);		glVertex3f((j + 1)/this->blocQuality * BLOCDEFAULTSIZE + this->xMin		, (i+1)/this->blocQuality * BLOCDEFAULTHEIGHT	, this->zMin);
					}
				}
				glEnd();
			break;

			case BOX_TYPE_RIGHT:
				glBindTexture  ( GL_TEXTURE_2D, wallTextureId );
				glBegin(GL_QUADS);
				glNormal3f( 1.0f, 0.0f, 0.0f);
				for( int i = 0 ; i < this->blocQuality ; i++)
				{
					for( int j = 0 ; j < this->blocQuality ; j++)
					{
						glTexCoord2f((j + 1)/this->blocQuality	, i/this->blocQuality);			glVertex3f(this->xMax, i/this->blocQuality * BLOCDEFAULTHEIGHT		, (j + 1)/this->blocQuality * BLOCDEFAULTSIZE + this->zMin);
						glTexCoord2f(		j/this->blocQuality	, i/this->blocQuality);			glVertex3f(this->xMax, i/this->blocQuality * BLOCDEFAULTHEIGHT		, j/this->blocQuality * BLOCDEFAULTSIZE + this->zMin);
						glTexCoord2f(		j/this->blocQuality	,(i+1)/this->blocQuality);		glVertex3f(this->xMax, (i+1)/this->blocQuality * BLOCDEFAULTHEIGHT	, j/this->blocQuality * BLOCDEFAULTSIZE + this->zMin);
						glTexCoord2f((j + 1)/this->blocQuality	,(i+1)/this->blocQuality);		glVertex3f(this->xMax, (i+1)/this->blocQuality * BLOCDEFAULTHEIGHT	, (j + 1)/this->blocQuality * BLOCDEFAULTSIZE + this->zMin);
					}
				}
				glEnd();
			break;

			case BOX_TYPE_DOWN:
				glBindTexture  ( GL_TEXTURE_2D, wallTextureId );
				glBegin(GL_QUADS);
				glNormal3f( 0.0f, 0.0f, 1.0f);
				for( int i = 0 ; i < this->blocQuality ; i++)
				{
					for( int j = 0 ; j < this->blocQuality ; j++)
					{
						glTexCoord2f(     j/this->blocQuality	, i/this->blocQuality);			glVertex3f( j/this->blocQuality * BLOCDEFAULTSIZE + this->xMin		, i/this->blocQuality * BLOCDEFAULTHEIGHT		, this->zMax);
						glTexCoord2f( (j+1)/this->blocQuality	, i/this->blocQuality);			glVertex3f((j+1)/this->blocQuality * BLOCDEFAULTSIZE + this->xMin	, i/this->blocQuality * BLOCDEFAULTHEIGHT		, this->zMax);
						glTexCoord2f( (j+1)/this->blocQuality	,(i+1)/this->blocQuality);		glVertex3f((j+1)/this->blocQuality * BLOCDEFAULTSIZE + this->xMin	, (i+1)/this->blocQuality * BLOCDEFAULTHEIGHT	, this->zMax);
						glTexCoord2f(     j/this->blocQuality	,(i+1)/this->blocQuality);		glVertex3f( j/this->blocQuality * BLOCDEFAULTSIZE + this->xMin		, (i+1)/this->blocQuality * BLOCDEFAULTHEIGHT	, this->zMax);
					}
				}
				glEnd();
			break;
		}
		glEndList();
	}
}

/**
* Draw the Box
* The parameters are needed for the LOD
* @param playerX Player X coordinate
* @param playerZ Player Z coordinate
*/
void Box::draw(float playerX, float playerZ)
{
	// Calculate distance between the object dans the player
	float dist = 0.0f;
	switch(this->boxType)
	{
		case BOX_TYPE_UP:
		case BOX_TYPE_DOWN:
		dist = sqrt((this->xMin + BLOCDEFAULTSIZE / 2.0f - playerX) * (this->xMin + BLOCDEFAULTSIZE / 2.0f - playerX) + (this->zMin + BLOCDEFAULTTICKNESS / 2.0f - playerZ) * (this->zMin + BLOCDEFAULTTICKNESS / 2.0f - playerZ));
		break;


		case BOX_TYPE_LEFT:
		case BOX_TYPE_RIGHT:
		dist = sqrt((this->xMin + BLOCDEFAULTTICKNESS / 2.0f - playerX) * (this->xMin + BLOCDEFAULTTICKNESS / 2.0f - playerX) + (this->zMin + BLOCDEFAULTSIZE / 2.0f - playerZ) * (this->zMin + BLOCDEFAULTSIZE / 2.0f - playerZ));
		break;
	}

	// Draw the appropriate box
	if(dist > VIEWQUALITYMIN0 || this->lightQuality == 0)
		glCallList(this->objectNumber);
	else if(dist > VIEWQUALITYMIN1 || this->lightQuality == 1)
			glCallList(this->objectNumber + 1);
	else if(dist > VIEWQUALITYMIN2 || this->lightQuality == 2)
			glCallList(this->objectNumber + 2);
	else
		glCallList(this->objectNumber + 3);
}

/**
* Return the Box type
* @return type
*/
int Box::getType()
{
	return this->boxType;
}

/**
* Return the XMin value
* @return xMin
*/
float Box::getXMin()
{
	return this->xMin;
}

/**
* Return the XMax value
* @return xMax
*/
float Box::getXMax()
{
	return this->xMax;
}

/**
* Return the ZMin value
* @return zMin
*/
float Box::getZMin()
{
	return this->zMin;
}

/**
* Return the ZMax value
* @return xMax
*/
float Box::getZMax()
{
	return this->zMax;
}

/**
* Test if there is a collision between a 2D point and the Box
* @param x X coordinate
* @param z Z coordinate
* @return If there is collision
*/
bool Box::isCollision(float x, float z)
{
	bool ret = false;
	switch(this->boxType)
	{
		case BOX_TYPE_LEFT:
		ret = (x >= this->xMin && z <= this->zMax && z >= this->xMin);
		break;

		case BOX_TYPE_UP:
		ret = (x >= this->xMin && x <= this->xMax && z >= this->zMin);
		break;

		case BOX_TYPE_RIGHT:
		ret = (x <= this->xMax && z <= this->zMax && z >= this->xMin);
		break;

		case BOX_TYPE_DOWN:
		ret = (x >= this->xMin && x <= this->xMax && z <= this->zMax);
		break;
	}

	return ret;
}

/**
* Test if there is a collision between a 3D point and the Box
* @param x X coordinate
* @param z Z coordinate
* @param y Y coordinate
* @return If there is collision
*/
bool Box::isCollision(float x, float z, float y)
{
	bool ret = false;
	switch(this->boxType)
	{
		case BOX_TYPE_LEFT:
		ret = (x >= this->xMin && z <= this->zMax && z >= this->xMin) || y <= 0 || y >= BLOCDEFAULTHEIGHT;
		break;

		case BOX_TYPE_UP:
		ret = (x >= this->xMin && x <= this->xMax && z >= this->zMin) || y <= 0 || y >= BLOCDEFAULTHEIGHT;
		break;

		case BOX_TYPE_RIGHT:
		ret = (x <= this->xMax && z <= this->zMax && z >= this->xMin) || y <= 0 || y >= BLOCDEFAULTHEIGHT;
		break;

		case BOX_TYPE_DOWN:
		ret = (x >= this->xMin && x <= this->xMax && z <= this->zMax) || y <= 0 || y >= BLOCDEFAULTHEIGHT;
		break;
	}

	return ret;
}
