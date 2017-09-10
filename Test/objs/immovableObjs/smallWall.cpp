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
#include <math.h>
#include <GL/gl.h>
#include "../../includes/global_var.h"
#include "../../manager/optionManager.h"
#include "../../objs/immovableObjs/smallWall.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param player1 Pointer to the Player
* @param wallType Type of wall
* @param wallTextureNumber Texture number
* @param objectNumber GList number
* @param ambientLight Ambient light value
*/
SmallWall::SmallWall(int x, int z, Player * player1, int wallType, int wallTextureNumber, int objectNumber, int ambientLight) : ImmovableObject(x,z,player1)
{
	this->wallType = wallType;
	this->objectNumber = objectNumber;
	this->smallWallQuality = (float)OptionManager::getInstance()->getSmallWallQuality();
	if(smallWallQuality > 1)
		this->smallWallQualitySmall = smallWallQuality / 2;
	else
		this->smallWallQualitySmall = 1;
	this->texturesManager = CTextureManager::getInstance();
	this->impactEnabled = true;
	this->matType = OBJECTMAT_ROCK;

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

	std::ostringstream stringstream;
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/wall" << wallTextureNumber << ".tga";
	this->wallTextureId	= texturesManager->LoadTexture(stringstream.str());

	this->sound = this->soundM->loadSound("sounds/environment/rock_impact.wav");

	calculate();
}

/**
* Destructor
*/
SmallWall::~SmallWall(){}

/**
* Calculate the Small Wall
*/
void SmallWall::calculate()
{
	for(int j = 0; j <= this->lightQuality ; j++)
	{
		this->smallWallQuality = (float)OptionManager::getInstance()->getSmallWallQuality(j);

		if(smallWallQuality > 1)
			this->smallWallQualitySmall = smallWallQuality / 2;
		else
			this->smallWallQualitySmall = 1;

		glNewList(this->objectNumber + j,GL_COMPILE); 
		glBindTexture  ( GL_TEXTURE_2D, wallTextureId );
		if(wallType == 1)
		{
			glBegin(GL_QUADS);
			for( int i = 0 ; i < this->smallWallQuality ; i++)
			{
				for( int j = 0 ; j < this->smallWallQuality ; j++)
				{
					// bottom
					glNormal3f( 0.0f, 0.0f, -1.0f);
					glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)			, j/this->smallWallQuality * 0.25f);		glVertex3f(this->x - i/this->smallWallQuality * 2		, j/this->smallWallQuality * OBJECTWALLHEIGHT		, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f);
					glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)	, j/this->smallWallQuality * 0.25f);		glVertex3f(this->x - (i+1)/this->smallWallQuality * 2	, j/this->smallWallQuality * OBJECTWALLHEIGHT		, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f);
					glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)	,(j+1)/this->smallWallQuality * 0.25f);		glVertex3f(this->x - (i+1)/this->smallWallQuality * 2	, (j+1)/this->smallWallQuality * OBJECTWALLHEIGHT	, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f); 	
					glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)			,(j+1)/this->smallWallQuality * 0.25f);		glVertex3f(this->x - i/this->smallWallQuality * 2		, (j+1)/this->smallWallQuality * OBJECTWALLHEIGHT	, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f);

					// top
					glNormal3f( 0.0f, 0.0f, 1.0f);
					glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)	, j/this->smallWallQuality * 0.25f);		glVertex3f(this->x - (i + 1)/this->smallWallQuality * 2		, j/this->smallWallQuality * OBJECTWALLHEIGHT		, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS);
					glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)			, j/this->smallWallQuality * 0.25f);		glVertex3f(this->x - i/this->smallWallQuality * 2			, j/this->smallWallQuality * OBJECTWALLHEIGHT		, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS);
					glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)			,(j+1)/this->smallWallQuality * 0.25f);		glVertex3f(this->x - i/this->smallWallQuality * 2			, (j+1)/this->smallWallQuality * OBJECTWALLHEIGHT	, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS); 	
					glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)	,(j+1)/this->smallWallQuality * 0.25f);		glVertex3f(this->x - (i + 1)/this->smallWallQuality * 2		, (j+1)/this->smallWallQuality * OBJECTWALLHEIGHT	, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS);

					// bottom up
					glNormal3f( 0.0f, 0.0f, -1.0f);
					glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)			, j/this->smallWallQuality * 0.05f);		glVertex3f(this->x - i/this->smallWallQuality * 2		, j/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT		, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f - 0.25f);
					glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)	, j/this->smallWallQuality * 0.05f);		glVertex3f(this->x - (i+1)/this->smallWallQuality * 2	, j/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT		, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f - 0.25f);
					glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)	,(j+1)/this->smallWallQuality * 0.05f);		glVertex3f(this->x - (i+1)/this->smallWallQuality * 2	, (j+1)/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT	, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f - 0.25f); 	
					glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)			,(j+1)/this->smallWallQuality * 0.05f);		glVertex3f(this->x - i/this->smallWallQuality * 2		, (j+1)/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT	, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f - 0.25f);

					// Top up
					glNormal3f( 0.0f, 0.0f, 1.0f);
					glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)	, j/this->smallWallQuality * 0.05f);		glVertex3f(this->x - (i + 1)/this->smallWallQuality * 2	, j/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT		, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS + 0.25f);
					glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)			, j/this->smallWallQuality * 0.05f);		glVertex3f(this->x - i/this->smallWallQuality * 2		, j/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT		, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS + 0.25f);
					glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)			,(j+1)/this->smallWallQuality * 0.05f);		glVertex3f(this->x - i/this->smallWallQuality * 2		, (j+1)/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT	, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS + 0.25f); 	
					glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)	,(j+1)/this->smallWallQuality * 0.05f);		glVertex3f(this->x - (i + 1)/this->smallWallQuality * 2	, (j+1)/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT	, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS + 0.25f);

					// Top
					glNormal3f(0.0f, 1.0f, 0.0f);
					glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)			,j/this->smallWallQuality * 0.25f);			glVertex3f(this->x - i/this->smallWallQuality * 2		, 2.25f, this->z - 0.25f + (2 - OBJECTWALLTICKNESS) / 2.0f + (1.5f * j/this->smallWallQuality));
					glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)	,j/this->smallWallQuality * 0.25f);			glVertex3f(this->x - (i + 1)/this->smallWallQuality * 2	, 2.25f, this->z - 0.25f + (2 - OBJECTWALLTICKNESS) / 2.0f + (1.5f * j/this->smallWallQuality));
					glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)	,(j + 1)/this->smallWallQuality * 0.25f);	glVertex3f(this->x - (i + 1)/this->smallWallQuality * 2	, 2.25f, this->z - 0.25f + (2 - OBJECTWALLTICKNESS) / 2.0f + (1.5f * (j + 1)/this->smallWallQuality)); 	
					glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)			,(j + 1)/this->smallWallQuality * 0.25f);	glVertex3f(this->x - i/this->smallWallQuality * 2		, 2.25f, this->z - 0.25f + (2 - OBJECTWALLTICKNESS) / 2.0f + (1.5f * (j + 1)/this->smallWallQuality));

					// Bottom
					glNormal3f(0.0f, -1.0f, 0.0f);
					glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)			,j/this->smallWallQuality * 0.25f);			glVertex3f(this->x - i/this->smallWallQuality * 2		, 2		, this->z - 0.25f + (2 - OBJECTWALLTICKNESS) / 2.0f + (1.5f * j/this->smallWallQuality));
					glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)			,(j + 1)/this->smallWallQuality * 0.25f);	glVertex3f(this->x - i/this->smallWallQuality * 2		, 2		, this->z - 0.25f + (2 - OBJECTWALLTICKNESS) / 2.0f + (1.5f * (j + 1)/this->smallWallQuality));
					glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)	,(j + 1)/this->smallWallQuality * 0.25f);	glVertex3f(this->x - (i + 1)/this->smallWallQuality * 2	, 2		, this->z - 0.25f + (2 - OBJECTWALLTICKNESS) / 2.0f + (1.5f * (j + 1)/this->smallWallQuality));
					glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(-this->x/2)%5)	,j/this->smallWallQuality * 0.25f);			glVertex3f(this->x - (i + 1)/this->smallWallQuality * 2	, 2		, this->z - 0.25f + (2 - OBJECTWALLTICKNESS) / 2.0f + (1.5f * j/this->smallWallQuality));
				}
			}

			for(int i = 0 ; i < this->smallWallQualitySmall ; i++)
			{
				for(int j = 0 ; j < this->smallWallQualitySmall ; j++)
				{
					// Left
					glNormal3f(1.0f, 0.0f, 0.0f);
					glTexCoord2f((i + 1)/this->smallWallQualitySmall * 0.2f	, j/this->smallWallQualitySmall * 0.4f);			glVertex3f(this->x	, 2 * j/this->smallWallQualitySmall			, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS * (i + 1)/this->smallWallQualitySmall);
					glTexCoord2f(i/this->smallWallQualitySmall * 0.2f		, j/this->smallWallQualitySmall * 0.4f);			glVertex3f(this->x	, 2 * j/this->smallWallQualitySmall			, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS * i/this->smallWallQualitySmall);
					glTexCoord2f(i/this->smallWallQualitySmall * 0.2f		, (j + 1)/this->smallWallQualitySmall * 0.4f);		glVertex3f(this->x	, 2 * (j + 1)/this->smallWallQualitySmall	, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS * i/this->smallWallQualitySmall);
					glTexCoord2f((i + 1)/this->smallWallQualitySmall * 0.2f	, (j + 1)/this->smallWallQualitySmall * 0.4f);		glVertex3f(this->x	, 2 * (j + 1)/this->smallWallQualitySmall	, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS * (i + 1)/this->smallWallQualitySmall);

					// Right
					glNormal3f(-1.0f, 0.0f, 0.0f);
					glTexCoord2f(i/this->smallWallQualitySmall * 0.2f		, j/this->smallWallQualitySmall * 0.4f);			glVertex3f(this->x - 2	, 2 * j/this->smallWallQualitySmall			, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS * i/this->smallWallQualitySmall);
					glTexCoord2f((i + 1)/this->smallWallQualitySmall * 0.2f	, j/this->smallWallQualitySmall * 0.4f);			glVertex3f(this->x - 2	, 2 * j/this->smallWallQualitySmall			, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS * (i + 1)/this->smallWallQualitySmall);
					glTexCoord2f((i + 1)/this->smallWallQualitySmall * 0.2f	, (j + 1)/this->smallWallQualitySmall * 0.4f);		glVertex3f(this->x - 2	, 2 * (j + 1)/this->smallWallQualitySmall	, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS * (i + 1)/this->smallWallQualitySmall);
					glTexCoord2f(i/this->smallWallQualitySmall * 0.2f		, (j + 1)/this->smallWallQualitySmall * 0.4f);		glVertex3f(this->x - 2	, 2 * (j + 1)/this->smallWallQualitySmall	, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS * i/this->smallWallQualitySmall);
				}
			}

			// Left up
			glNormal3f(1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);	glVertex3f(this->x	, 2, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS + 0.25f);
			glTexCoord2f(0.2f, 0.0f);	glVertex3f(this->x	, 2, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f - 0.25f);
			glTexCoord2f(0.2f, 0.1f);	glVertex3f(this->x	, 2.25f, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f - 0.25f);
			glTexCoord2f(0.0f, 0.1f);	glVertex3f(this->x	, 2.25f, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS + 0.25f);

			// Right up
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);	glVertex3f(this->x - 2	, 2		, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f - 0.25f);
			glTexCoord2f(0.2f, 0.0f);	glVertex3f(this->x - 2	, 2		, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS + 0.25f);
			glTexCoord2f(0.2f, 0.1f);	glVertex3f(this->x - 2	, 2.25f	, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS + 0.25f);
			glTexCoord2f(0.0f, 0.1f);	glVertex3f(this->x - 2	, 2.25f	, this->z + (2 - OBJECTWALLTICKNESS) / 2.0f - 0.25f);

			glEnd();
		}
		else
		{
			if(this->wallType == 2)
			{
				glBegin(GL_QUADS);
				for( int i = 0 ; i < this->smallWallQuality ; i++)
				{
					for( int j = 0 ; j < this->smallWallQuality ; j++)
					{
						// Left
						glNormal3f(1.0f, 0.0f, 0.0f);
						glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)	, j/this->smallWallQuality * 0.4f);			glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	, j/this->smallWallQuality * OBJECTWALLHEIGHT		, this->z + 2 * (i + 1)/this->smallWallQuality);
						glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)			, j/this->smallWallQuality * 0.4f);			glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	, j/this->smallWallQuality * OBJECTWALLHEIGHT		, this->z + 2 * i/this->smallWallQuality);
						glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)			, (j + 1)/this->smallWallQuality * 0.4f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	, (j + 1)/this->smallWallQuality * OBJECTWALLHEIGHT	, this->z + 2 * i/this->smallWallQuality);
						glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)	, (j + 1)/this->smallWallQuality * 0.4f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	, (j + 1)/this->smallWallQuality * OBJECTWALLHEIGHT	, this->z + 2 * (i + 1)/this->smallWallQuality);

						// Right
						glNormal3f(-1.0f, 0.0f, 0.0f);
						glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)			, j/this->smallWallQuality * 0.4f);			glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	- OBJECTWALLTICKNESS, j/this->smallWallQuality * OBJECTWALLHEIGHT		, this->z + 2 * i/this->smallWallQuality);
						glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)	, j/this->smallWallQuality * 0.4f);			glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	- OBJECTWALLTICKNESS, j/this->smallWallQuality * OBJECTWALLHEIGHT		, this->z + 2 * (i + 1)/this->smallWallQuality);
						glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)	, (j + 1)/this->smallWallQuality * 0.4f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	- OBJECTWALLTICKNESS, (j + 1)/this->smallWallQuality * OBJECTWALLHEIGHT	, this->z + 2 * (i + 1)/this->smallWallQuality);
						glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)			, (j + 1)/this->smallWallQuality * 0.4f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	- OBJECTWALLTICKNESS, (j + 1)/this->smallWallQuality * OBJECTWALLHEIGHT	, this->z + 2 * i/this->smallWallQuality);

						// Left top
						glNormal3f(1.0f, 0.0f, 0.0f);
						glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)	, j/this->smallWallQuality * 0.05f);		glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	+ 0.25f, j/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT			, this->z + 2 * (i + 1)/this->smallWallQuality);
						glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)			, j/this->smallWallQuality * 0.05f);		glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	+ 0.25f, j/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT			, this->z + 2 * i/this->smallWallQuality);
						glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)			, (j + 1)/this->smallWallQuality * 0.05f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	+ 0.25f, (j + 1)/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT		, this->z + 2 * i/this->smallWallQuality);
						glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)	, (j + 1)/this->smallWallQuality * 0.05f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	+ 0.25f, (j + 1)/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT		, this->z + 2 * (i + 1)/this->smallWallQuality);

						// Right top
						glNormal3f(-1.0f, 0.0f, 0.0f);
						glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)			, j/this->smallWallQuality * 0.05f);		glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	- OBJECTWALLTICKNESS - 0.25f, j/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT			, this->z + 2 * i/this->smallWallQuality);
						glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)	, j/this->smallWallQuality * 0.05f);		glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	- OBJECTWALLTICKNESS - 0.25f, j/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT			, this->z + 2 * (i + 1)/this->smallWallQuality);
						glTexCoord2f((i + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)	, (j + 1)/this->smallWallQuality * 0.05f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	- OBJECTWALLTICKNESS - 0.25f, (j + 1)/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT	, this->z + 2 * (i + 1)/this->smallWallQuality);
						glTexCoord2f(i/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)			, (j + 1)/this->smallWallQuality * 0.05f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f	- OBJECTWALLTICKNESS - 0.25f, (j + 1)/this->smallWallQuality * 0.25f + OBJECTWALLHEIGHT	, this->z + 2 * i/this->smallWallQuality);

						// Top
						glNormal3f(0.0f, 1.0f, 0.0f);
						glTexCoord2f(j/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)			,i/this->smallWallQuality * 0.25f);			glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f + 0.25f - i/this->smallWallQuality * 1.5f			, 2.25f, this->z + (2 * j/this->smallWallQuality));
						glTexCoord2f(j/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)			,(i + 1)/this->smallWallQuality * 0.25f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f + 0.25f - (i + 1)/this->smallWallQuality * 1.5f	, 2.25f, this->z + (2 * j/this->smallWallQuality));
						glTexCoord2f((j + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)	,(i + 1)/this->smallWallQuality * 0.25f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f + 0.25f - (i + 1)/this->smallWallQuality * 1.5f	, 2.25f, this->z + (2 * (j + 1)/this->smallWallQuality)); 	
						glTexCoord2f((j + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)	,i/this->smallWallQuality * 0.25f);			glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f + 0.25f - i/this->smallWallQuality * 1.5f			, 2.25f, this->z + (2 * (j + 1)/this->smallWallQuality));

						// Bottom
						glNormal3f(0.0f, -1.0f, 0.0f);
						glTexCoord2f(j/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)			,i/this->smallWallQuality * 0.25f);			glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f + 0.25f - i/this->smallWallQuality * 1.5f			, 2		, this->z + (2 * j/this->smallWallQuality));
						glTexCoord2f((j + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)	,i/this->smallWallQuality * 0.25f);			glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f + 0.25f - i/this->smallWallQuality * 1.5f			, 2		, this->z + (2 * (j + 1)/this->smallWallQuality));
						glTexCoord2f((j + 1)/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)	,(i + 1)/this->smallWallQuality * 0.25f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f + 0.25f - (i + 1)/this->smallWallQuality * 1.5f	, 2		, this->z + (2 * (j + 1)/this->smallWallQuality));
						glTexCoord2f(j/this->smallWallQuality * 0.2f + 0.2f * ((int)(this->z/2)%5)			,(i + 1)/this->smallWallQuality * 0.25f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f + 0.25f - (i + 1)/this->smallWallQuality * 1.5f	, 2		, this->z + (2 * j/this->smallWallQuality));
					}
				}

				for(int i = 0 ; i < this->smallWallQualitySmall ; i++)
				{
					for(int j = 0 ; j < this->smallWallQualitySmall ; j++)
					{
						// Bottom
						glNormal3f( 0.0f, 0.0f, -1.0f);
						glTexCoord2f(i/this->smallWallQualitySmall * 0.2f			, j/this->smallWallQualitySmall * 0.4f);		glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - i/this->smallWallQualitySmall		, j/this->smallWallQualitySmall * OBJECTWALLHEIGHT		, this->z);
						glTexCoord2f((i + 1)/this->smallWallQualitySmall * 0.2f		, j/this->smallWallQualitySmall * 0.4f);		glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - (i+1)/this->smallWallQualitySmall	, j/this->smallWallQualitySmall * OBJECTWALLHEIGHT		, this->z);
						glTexCoord2f((i + 1)/this->smallWallQualitySmall * 0.2f		,(j+1)/this->smallWallQualitySmall * 0.4f);		glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - (i+1)/this->smallWallQualitySmall	, (j+1)/this->smallWallQualitySmall * OBJECTWALLHEIGHT	, this->z); 	
						glTexCoord2f(i/this->smallWallQualitySmall * 0.2f			,(j+1)/this->smallWallQualitySmall * 0.4f);		glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - i/this->smallWallQualitySmall		, (j+1)/this->smallWallQualitySmall * OBJECTWALLHEIGHT	, this->z);

						// Top
						glNormal3f( 0.0f, 0.0f, 1.0f);
						glTexCoord2f((i + 1)/this->smallWallQualitySmall * 0.2f		, j/this->smallWallQualitySmall * 0.4f);		glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - (i + 1)/this->smallWallQualitySmall	, j/this->smallWallQualitySmall * OBJECTWALLHEIGHT		, this->z + 2);
						glTexCoord2f(i/this->smallWallQualitySmall * 0.2f			, j/this->smallWallQualitySmall * 0.4f);		glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - i/this->smallWallQualitySmall		, j/this->smallWallQualitySmall * OBJECTWALLHEIGHT		, this->z + 2);
						glTexCoord2f(i/this->smallWallQualitySmall * 0.2f			,(j+1)/this->smallWallQualitySmall * 0.4f);		glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - i/this->smallWallQualitySmall		, (j+1)/this->smallWallQualitySmall * OBJECTWALLHEIGHT	, this->z + 2); 	
						glTexCoord2f((i + 1)/this->smallWallQualitySmall * 0.2f		,(j+1)/this->smallWallQualitySmall * 0.4f);		glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - (i + 1)/this->smallWallQualitySmall	, (j+1)/this->smallWallQualitySmall * OBJECTWALLHEIGHT	, this->z + 2);
					}
				}

				// Bottom up
				glNormal3f(0.0f, 0.0f, -1.0f);
				glTexCoord2f(0.0f, 0.0f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f + 0.25f, 2								, this->z);
				glTexCoord2f(0.2f, 0.0f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - OBJECTWALLTICKNESS - 0.25f, 2		, this->z);
				glTexCoord2f(0.2f, 0.1f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - OBJECTWALLTICKNESS - 0.25f, 2.25f	, this->z);
				glTexCoord2f(0.0f, 0.1f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f + 0.25f, 2.25f							, this->z);

				// Top up
				glNormal3f(0.0f, 0.0f, 1.0f);
				glTexCoord2f(0.2f, 0.0f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - OBJECTWALLTICKNESS - 0.25f	, 2		, this->z + 2);
				glTexCoord2f(0.0f, 0.0f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f + 0.25f						, 2		, this->z + 2);
				glTexCoord2f(0.0f, 0.1f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f + 0.25f						, 2.25f	, this->z + 2);
				glTexCoord2f(0.2f, 0.1f);	glVertex3f(this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - OBJECTWALLTICKNESS - 0.25f	, 2.25f	, this->z + 2);
				glEnd();
			}
		}
		glEndList();
	}
}

/**
* Draw the Small Wall
*/
void SmallWall::draw( float * lightPos)
{
	glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
	glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
	glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
	glMaterialf (GL_FRONT,GL_SHININESS,50.0f);

	float dist = sqrt((this->x - player1->getCoor()[0]) * (this->x - player1->getCoor()[0]) + (this->z - player1->getCoor()[2]) * (this->z - player1->getCoor()[2]));
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
* Detect collisions
* @param x X coordinate to test
* @param z Z coordinate to test
* @param y Y coordinate to test
* @return Result to the collsion test
*/
bool SmallWall::isCollision(float x, float z, float y)
{
	bool ret = false;

	if(this->wallType == 1)
	{
		ret = (x >= this->x - 2 && x <= this->x && z >= this->z + (2 - OBJECTWALLTICKNESS) / 2.0 && z <= this->z + (2 - OBJECTWALLTICKNESS) / 2.0 + OBJECTWALLTICKNESS && y >= 0 && y <= OBJECTWALLHEIGHT) || (x >= this->x - 2 && x <= this->x && z >= this->z + (2 - OBJECTWALLTICKNESS) / 2.0 - 0.25 && z <= this->z + (2 - OBJECTWALLTICKNESS) / 2.0 + OBJECTWALLTICKNESS + 0.25 && y > OBJECTWALLHEIGHT && y <= OBJECTWALLHEIGHT + 0.25);
	}
	else
	{
		if(this->wallType == 2)
		{
			ret = (x >= this->x - (2 - OBJECTWALLTICKNESS) / 2.0 - OBJECTWALLTICKNESS && x <= this->x - (2 - OBJECTWALLTICKNESS) / 2.0 && z >= this->z && z <= this-> z + 2 && y >= 0 && y <= OBJECTWALLHEIGHT) || (x >= this->x - (2 - OBJECTWALLTICKNESS) / 2.0 - OBJECTWALLTICKNESS - 0.25 && x <= this->x - (2 - OBJECTWALLTICKNESS) / 2.0 + 0.25 && z >= this->z && z <= this->z + 2 && y > OBJECTWALLHEIGHT && y <= OBJECTWALLHEIGHT + 0.25);
		}
	}
	return ret;
}

/**
* Detect the Impact Location and return corrects values
* @param x1 Previous Projectile's X coordinate
* @param y1 Previous Projectile's Y coordinate
* @param z1 Previous Projectile's Z coordinate
* @param x2 Projectile's X coordinate
* @param y2 Projectile's Y coordinate
* @param z2 Projectile's Z coordinate
* @param impactX Reference to the Impact X coordinate
* @param impactY Reference to the Impact Y coordinate
* @param impactZ Reference to the Impact Z coordinate
* @param impactAngleX Reference to the Impact X Angle
* @param impactAngleY Reference to the Impact Y Angle
* @param impactAngleZ Reference to the Impact Z Angle
* @param modif Reference to the Impact replacement choosen
* @param impactRadius The Impact Radius
*/
void SmallWall::impactGetLocation(float x1, float y1, float z1, float x2, float y2, float z2, float &impactX, float &impactY, float &impactZ, float &impactAngleX, float &impactAngleY, float &impactAngleZ, int &modif, float impactRadius)
{
	if(this->wallType == 1)
	{
		if(y1 <= 2.25f)
		{
			// The object to test is from the front of this object
			if(z1 <= this->z + (2 - OBJECTWALLTICKNESS) / 2.0f)
			{
				impactX = x2;
				impactY = y2;
				if(y2 < 2)
					impactZ = this->z + (2 - OBJECTWALLTICKNESS) / 2.0f - 0.001f;
				else
					impactZ = this->z - 0.001f + 0.25f;
				impactAngleX = 0;
				impactAngleY = 0;
				impactAngleZ = 0;
				modif = -3;

				// Prevent impact from overflowing outside the object
				if(impactX < this->x - 2 + impactRadius)
					impactX = this->x - 2 + impactRadius;
				if(impactX > this->x - impactRadius)
					impactX = this->x - impactRadius;
				if(y2 < 2)
				{
					if(impactY < impactRadius)
						impactY = impactRadius;
				}
				else
				{
					if(impactY < 2 + impactRadius)
						impactY = 2 + impactRadius;
					if(impactY > 2.25f - impactRadius)
						impactY = 2.25f - impactRadius;
				}
			}
			else
			{
				// The object to test is from behind of this object
				if(z1 >= this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS)
				{
					impactX = x2;
					impactY = y2;
					if(y2 < 2)
						impactZ = this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS + 0.001f;
					else
						impactZ = this->z + 2.001f - 0.25f;
					impactAngleX = 0;
					impactAngleY = 180;
					impactAngleZ = 0;
					modif = 3;

					// Prevent impact from overflowing outside the object
					if(impactX < this->x - 2 + impactRadius)
						impactX = this->x - 2 + impactRadius;
					if(impactX > this->x - impactRadius)
						impactX = this->x - impactRadius;
					if(y2 < 2)
					{
						if(impactY < impactRadius)
							impactY = impactRadius;
					}
					else
					{
						if(impactY < 2 + impactRadius)
							impactY = 2 + impactRadius;
						if(impactY > 2.25f - impactRadius)
							impactY = 2.25f - impactRadius;
					}
				}
			}

			// The object to test is from the right of this object
			if(x1 <= this->x - 2)
			{
				impactX = this->x - 2.001f;
				impactY = y2;
				impactZ = z2;
				impactAngleX = 0;
				impactAngleY = 90;
				impactAngleZ = 0;
				modif = -1;

				// Prevent impact from overflowing outside the object
				if(impactY < impactRadius)
					impactY = impactRadius;
				if(impactY > 2.25f - impactRadius)
					impactY = 2.25f - impactRadius;
				if(y2 < 2)
				{
					if(impactZ < this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + impactRadius)
						impactZ = this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + impactRadius;
					if(impactZ > this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS - impactRadius)
						impactZ = this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS - impactRadius;
				}
				else
				{
					if(impactZ < this->z + 0.25f + impactRadius)
						impactZ = this->z + 0.25f + impactRadius;
					if(impactZ > this->z + 2 - 0.25f - impactRadius)
						impactZ = this->z + 2 - 0.25f - impactRadius;
					if(impactY < 2 + impactRadius)
						impactY = 2 + impactRadius;
					if(impactY > 2.25f - impactRadius)
						impactY = 2.25f - impactRadius;
				}
			}
			else
			{
				// The object to test is from the left of this object
				if(x1 >= this->x)
				{
					impactX = this->x + 0.001f;
					impactY = y2;
					impactZ = z2;
					impactAngleX = 0;
					impactAngleY = -90;
					impactAngleZ = 0;
					modif = 1;

					// Prevent impact from overflowing outside the object
					if(impactY < impactRadius)
						impactY = impactRadius;
					if(impactY > 2.25f - impactRadius)
						impactY = 2.25f - impactRadius;
					if(y2 < 2)
					{
						if(impactZ < this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + impactRadius)
							impactZ = this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + impactRadius;
						if(impactZ > this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS - impactRadius)
							impactZ = this->z + (2 - OBJECTWALLTICKNESS) / 2.0f + OBJECTWALLTICKNESS - impactRadius;
					}
					else
					{
						if(impactZ < this->z + 0.25f + impactRadius)
							impactZ = this->z + 0.25f + impactRadius;
						if(impactZ > this->z + 2 - 0.25f - impactRadius)
							impactZ = this->z + 2 - 0.25f - impactRadius;
						if(impactY < 2 + impactRadius)
							impactY = 2 + impactRadius;
						if(impactY > 2.25f - impactRadius)
							impactY = 2.25f - impactRadius;
					}
				}
			}
		}
		else
		{
			// The object to test is from the top of this object
			impactX = x2;
			impactY = 2.251f;
			impactZ = z2;
			impactAngleX = 90;
			impactAngleY = 0;
			impactAngleZ = 0;
			modif = 2;

			// Prevent impact from overflowing outside the object
			if(impactZ < this->z + 0.25f + impactRadius)
				impactZ = this->z + 0.25f + impactRadius;
			if(impactZ > this->z + 2 - 0.25f - impactRadius)
				impactZ = this->z + 2 - 0.25f - impactRadius;
			if(impactX < this->x - 2 + impactRadius)
				impactX = this->x - 2 + impactRadius;
			if(impactX > this->x - impactRadius)
				impactX = this->x - impactRadius;
		}
	}
	else
	{
		if(this->wallType == 2)
		{
			if(y1 <= 2.25)
			{
				// The object to test is from the front of this object
				if(z1 <= this->z)
				{
					impactX = x2;
					impactY = y2;
					impactZ = this->z - 0.001f;
					impactAngleX = 0;
					impactAngleY = 0;
					impactAngleZ = 0;
					modif = -3;

					// Prevent impact from overflowing outside the object
					if(impactY < impactRadius)
						impactY = impactRadius;
					if(impactY > 2.25f - impactRadius)
						impactY = 2.25f - impactRadius;
					if(y2 < 2)
					{
						if(impactX <  this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - OBJECTWALLTICKNESS + impactRadius)
							impactX = this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - OBJECTWALLTICKNESS + impactRadius;
						if(impactX >  this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - impactRadius)
							impactX = this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - impactRadius;
					}
					else
					{
						if(impactX <  this->x - 2 + 0.25f + impactRadius)
							impactX = this->x - 2 + 0.25f + impactRadius;
						if(impactX >  this->x - 0.25f - impactRadius)
							impactX = this->x - 0.25f - impactRadius;
						if(impactY < 2 + impactRadius)
							impactY = 2 + impactRadius;
						if(impactY > 2.25f - impactRadius)
							impactY = 2.25f - impactRadius;
					}
				}
				else
				{
					// The object to test is from behind of this object
					if(z1 >= this->z + 2)
					{
						impactX = x2;
						impactY = y2;
						impactZ = this->z + 2.001f;
						impactAngleX = 0;
						impactAngleY = 180;
						impactAngleZ = 0;
						modif = 3;

						// Prevent impact from overflowing outside the object
						if(impactY < impactRadius)
							impactY = impactRadius;
						if(impactY > 2.25f - impactRadius)
							impactY = 2.25f - impactRadius;
						if(y2 < 2)
						{
							if(impactX <  this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - OBJECTWALLTICKNESS + impactRadius)
								impactX = this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - OBJECTWALLTICKNESS + impactRadius;
							if(impactX >  this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - impactRadius)
								impactX = this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - impactRadius;
						}
						else
						{
							if(impactX <  this->x - 2 + 0.25f + impactRadius)
								impactX = this->x - 2 + 0.25f + impactRadius;
							if(impactX >  this->x - 0.25f - impactRadius)
								impactX = this->x - 0.25f - impactRadius;
							if(impactY < 2 + impactRadius)
								impactY = 2 + impactRadius;
							if(impactY > 2.25f - impactRadius)
								impactY = 2.25f - impactRadius;
						}
					}
				}

				// The object to test is from the right of this object
				if(x1 <= this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - OBJECTWALLTICKNESS)
				{
					if(y1 < 2)
						impactX = this->x - (2 - OBJECTWALLTICKNESS) / 2.0f - OBJECTWALLTICKNESS - 0.001f;
					else
						impactX = this->x - 2.001f + 0.25f;
					impactY = y2;
					impactZ = z2;
					impactAngleX = 0;
					impactAngleY = 90;
					impactAngleZ = 0;
					modif = -1;

					// Prevent impact from overflowing outside the object
					if(impactZ < this->z + impactRadius)
						impactZ = this->z + impactRadius;
					if(impactZ > this->z + 2 - impactRadius)
						impactZ = this->z + 2 - impactRadius;
					if(y2 < 2)
					{
						if(impactY < impactRadius)
							impactY = impactRadius;
					}
					else
					{
						if(impactY < 2 + impactRadius)
							impactY = 2 + impactRadius;
						if(impactY > 2.25f - impactRadius)
							impactY = 2.25f - impactRadius;
					}
				}
				else
				{
					// The object to test is from the left of this object
					if(x1 >= this->x - (2 - OBJECTWALLTICKNESS) / 2.0f)
					{
						if(y1 < 2)
							impactX = this->x - (2 - OBJECTWALLTICKNESS) / 2.0f + 0.001f;
						else
							impactX = this->x + 0.001f - 0.25f;
						impactY = y2;
						impactZ = z2;
						impactAngleX = 0;
						impactAngleY = -90;
						impactAngleZ = 0;
						modif = 1;

						// Prevent impact from overflowing outside the object
						if(impactZ < this->z + impactRadius)
							impactZ = this->z + impactRadius;
						if(impactZ > this->z + 2 - impactRadius)
							impactZ = this->z + 2 - impactRadius;
						if(y2 < 2)
						{
							if(impactY < impactRadius)
								impactY = impactRadius;
						}
						else
						{
							if(impactY < 2 + impactRadius)
								impactY = 2 + impactRadius;
							if(impactY > 2.25f - impactRadius)
								impactY = 2.25f - impactRadius;
						}
					}
				}
			}
			else
			{
				// The object to test is from the top of this object
				impactX = x2;
				impactY = 2.251f;
				impactZ = z2;
				impactAngleX = 90;
				impactAngleY = 0;
				impactAngleZ = 0;
				modif = 2;

				// Prevent impact from overflowing outside the object
				if(impactZ < this->z + impactRadius)
					impactZ = this->z + impactRadius;
				if(impactZ > this->z + 2 - impactRadius)
					impactZ = this->z + 2 - impactRadius;
				if(impactX < this->x - 2 + 0.25f + impactRadius)
					impactX = this->x - 2 + 0.25f + impactRadius;
				if(impactX > this->x - 0.25f - impactRadius)
					impactX = this->x - 0.25f - impactRadius;
			}
		}
	}
}
