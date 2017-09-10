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
#include "../../manager/optionManager.h"
#include "../../includes/global_var.h"
#include "../../objs/trapObjs/pikes.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param player1 Pointer to the Player
* @param objectNumber GList number
* @param ambientLight Ambient light value
*/
Pikes::Pikes(int x, int z, Player * player1, int objectNumber, int ambientLight) : Obj(x,z,player1)
{
	this->objectNumber = objectNumber;
	this->lightQuality = OptionManager::getInstance()->getLightQuality();
	this->objectQuality = (float)OptionManager::getInstance()->getCrateQuality();

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
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/traps/pikes/pikes.tga";
	this->textureId = CTextureManager::getInstance()->LoadTexture(stringstream.str());

	stringstream.str("");
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/traps/pikes/pikes2.tga";
	this->textureId2 = CTextureManager::getInstance()->LoadTexture(stringstream.str());

	this->sound = this->soundM->loadSound("sounds/environment/metal_impact.wav");
	this->matType = OBJECTMAT_STEEL;

	calculate();
}

/**
* Destructor
*/
Pikes::~Pikes(){}

/**
* Draw the Pikes
* @param lightPos Light position
*/
void Pikes::draw( float * lightPos)
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
* Return the object type
* @return Object type
*/
int Pikes::getType()
{
	return OBJECTTYPE_TRAP;
}

/**
* Activate the Pikes function
* @return If the function is activated
*/
bool Pikes::useObject()
{
	if(this->player1->getMovementStatut() != PLAYER_MOVEMENT_JUMP)
	{
		if( player1->getCoor()[0] < this->x && player1->getCoor()[0] > this->x - 2.f && player1->getCoor()[2] < this->z + 2.0f && player1->getCoor()[2] > this->z )
			this->player1->takeDamage(35);
	}
	return false;
}

/**
* Calculate the Pikes
*/
void Pikes::calculate()
{
	for(int j = 0; j <= this->lightQuality ; j++)
	{
		this->objectQuality = (float)OptionManager::getInstance()->getCrateQuality(j);
		glNewList(this->objectNumber + j,GL_COMPILE); 

		// Pikes
		glBindTexture(GL_TEXTURE_2D, this->textureId );
		for( int i = 0 ; i < PIKESPERLINE ; i+=2)
		{
			for( int j = 0 ; j < PIKESPERLINE ; j+=2)
			{
				glBegin(GL_TRIANGLE_FAN);
				glTexCoord2f(0.5f,0.5f);	glVertex3f(this->x - (2.0f / (float)PIKESPERLINE) * i - (2.0f / (PIKESPERLINE * 2.0f)) - (2.0f / (PIKESPERLINE * 2.0f))	,PIKESHEIGHT		, this->z + (2.0f / (float)PIKESPERLINE) * j + (2.0f / (PIKESPERLINE * 2.0f)) + (2.0f / (PIKESPERLINE * 2.0f)));

				// Bottom
				glNormal3f(0.0f, 0.3f, -0.7f);
				glTexCoord2f(0.5f,0.0f);	glVertex3f(this->x - (2.0f / (float)PIKESPERLINE) * i - (2.0f / (PIKESPERLINE * 2.0f))									,0.0f				, this->z + (2.0f / (float)PIKESPERLINE) * j + (2.0f / (PIKESPERLINE * 2.0f)));
				glTexCoord2f(0.0f,0.0f);	glVertex3f(this->x - (2.0f / (float)PIKESPERLINE) * i - (2.0f / (float)PIKESPERLINE) - (2.0f / (PIKESPERLINE * 2.0f))	,0.0f				, this->z + (2.0f / (float)PIKESPERLINE) * j + (2.0f / (PIKESPERLINE * 2.0f)));

				// Right
				glNormal3f(-0.7f, 0.3f, 0.0f);
				glTexCoord2f(0.0f,0.0f);	glVertex3f(this->x - (2.0f / (float)PIKESPERLINE) * i - (2.0f / (float)PIKESPERLINE) - (2.0f / (PIKESPERLINE * 2.0f))	,0.0f				, this->z + (2.0f / (float)PIKESPERLINE) * j + (2.0f / (PIKESPERLINE * 2.0f)));
				glTexCoord2f(0.5f,0.0f);	glVertex3f(this->x - (2.0f / (float)PIKESPERLINE) * i - (2.0f / (float)PIKESPERLINE) - (2.0f / (PIKESPERLINE * 2.0f))	,0.0f				, this->z + (2.0f / (float)PIKESPERLINE) * j + (2.0f / (float)PIKESPERLINE) + (2.0f / (PIKESPERLINE * 2.0f)));

				// Top
				glNormal3f(0.0f, 0.3f, 0.7f);
				glTexCoord2f(0.5f,0.0f);	glVertex3f(this->x - (2.0f / (float)PIKESPERLINE) * i - (2.0f / (float)PIKESPERLINE) - (2.0f / (PIKESPERLINE * 2.0f))	,0.0f				, this->z + (2.0f / PIKESPERLINE) * j + (2.0f / (float)PIKESPERLINE) + (2.0f / (PIKESPERLINE * 2.0f)));
				glTexCoord2f(0.0f,0.0f);	glVertex3f(this->x - (2.0f / (float)PIKESPERLINE) * i - (2.0f / (PIKESPERLINE * 2.0f))									,0.0f				, this->z + (2.0f / (float)PIKESPERLINE) * j + (2.0f / (float)PIKESPERLINE) + (2.0f / (PIKESPERLINE * 2.0f)));

				// Left
				glNormal3f(0.7f, 0.3f, 0.0f);
				glTexCoord2f(0.0f,0.0f);	glVertex3f(this->x - (2.0f / (float)PIKESPERLINE) * i - (2.0f / (PIKESPERLINE * 2.0f))									,0.0f				, this->z + (2.0f / (float)PIKESPERLINE) * j + (2.0f / (float)PIKESPERLINE) + (2.0f / (PIKESPERLINE * 2.0f)));
				glTexCoord2f(0.5f,0.0f);	glVertex3f(this->x - (2.0f / (float)PIKESPERLINE) * i - (2.0f / (PIKESPERLINE * 2.0f))									,0.0f				, this->z + (2.0f / (float)PIKESPERLINE) * j + (2.0f / (PIKESPERLINE * 2.0f)));
				glEnd();
			}
		}

		// Ground
		glBindTexture(GL_TEXTURE_2D, this->textureId2 );
		for( int i = 0 ; i < this->objectQuality ; i++)
		{
			for( int j = 0 ; j < this->objectQuality ; j++)
			{
				glBegin(GL_QUADS);
					glNormal3f(0.0f, 1.0f, 0.0f);
					glTexCoord2f(i/this->objectQuality			,j/this->objectQuality);			glVertex3f(x - (2 * i/this->objectQuality)			, 0.01f	, z + (2 * j/this->objectQuality));
					glTexCoord2f((i + 1)/this->objectQuality	,j/this->objectQuality);			glVertex3f(x - (2 * (i + 1)/this->objectQuality)	, 0.01f	, z + (2 * j/this->objectQuality));
					glTexCoord2f((i + 1)/this->objectQuality	,(j + 1)/this->objectQuality);		glVertex3f(x - (2 * (i + 1)/this->objectQuality)	, 0.01f	, z + (2 * (j + 1)/this->objectQuality)); 	
					glTexCoord2f(i/this->objectQuality			,(j + 1)/this->objectQuality);		glVertex3f(x - (2 * i/this->objectQuality)			, 0.01f	, z + (2 * (j + 1)/this->objectQuality));
				glEnd();
			}
		}

		glEndList();
	}
}

/**
* Detect collisions
* @param x X coordinate to test
* @param z Z coordinate to test
* @param y Y coordinate to test
* @return Result to the collsion test
*/
bool Pikes::isCollision(float x, float z, float y)
{
	return y <= PIKESHEIGHT;
}
