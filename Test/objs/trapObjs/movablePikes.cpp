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
#include "../../objs/trapObjs/movablePikes.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param player1 Pointer to the Player
* @param objectNumber GList number
* @param ambientLight Ambient light value
*/
MovablePikes::MovablePikes(int x, int z, Player * player1, int objectNumber, int ambientLight) : Obj(x,z,player1)
{
	this->objectNumber = objectNumber;
	this->lightQuality = OptionManager::getInstance()->getLightQuality();
	this->trapMode = MOVABLEPIKES_READY;
	this->pikesY = 0.0f;
	this->trapTimer = 0;

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
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/traps/movablePikes/movablePikes2.tga";
	this->textureId = CTextureManager::getInstance()->LoadTexture(stringstream.str());

	stringstream.str("");
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/traps/movablePikes/movablePikes.tga";
	this->modelEntity = ModelManager::getInstance()->loadModel("objs/traps/movablePikes/movablePikes.md2", stringstream.str());

	this->sound = this->soundM->loadSound("sounds/environment/metal_impact.wav");
	this->activationSound = this->soundM->loadSound("sounds/objects/movablepikes_activate.wav");
	this->raiseSound = this->soundM->loadSound("sounds/objects/movablepikes_raise.wav");

	this->matType = OBJECTMAT_STEEL;

	calculate();
}

/**
* Destructor
*/
MovablePikes::~MovablePikes(){}

/**
* Refresh the MovablePikes
* @param time Elapsed time
*/
void MovablePikes::refresh(long time)
{
	// Timer before the raise of the pikes
	if(this->trapMode == MOVABLEPIKES_START_TIMER)
	{
		this->trapTimer += time;
		if(this->trapTimer >= MOVABLEPIKES_WAIT_TIME1)
		{
			this->soundM->playSound(this->raiseSound,1 + (int)(sqrt((this->x - 1 - this->player1->getCoor()[0]) * (this->x - 1 - this->player1->getCoor()[0]) + (this->z + 1 - this->player1->getCoor()[2]) * (this->z + 1 - this->player1->getCoor()[2]))) / 2);
			this->trapMode = MOVABLEPIKES_RAISE;
		}
	}
	else
	{
		// Raise movement
		if(this->trapMode == MOVABLEPIKES_RAISE)
		{
			this->pikesY += MOVABLEPIKES_SPEED * time / 1000;
			if(this->pikesY >= MOVABLEPIKES_MAX_HEIGHT)
			{
				this->pikesY = MOVABLEPIKES_MAX_HEIGHT;
				this->trapMode = MOVABLEPIKES_ACTIVATED;
				this->trapTimer = 0;
			}
		}
		else
		{
			// Timer before disactivating the pikes
			if(this->trapMode == MOVABLEPIKES_ACTIVATED)
			{
				this->trapTimer += time;
				if(this->trapTimer >= MOVABLEPIKES_WAIT_TIME2)
					this->trapMode = MOVABLEPIKES_DESACTIVATE;
			}
			else
			{
				// Pikes on the ground
				if(this->trapMode == MOVABLEPIKES_DESACTIVATE)
				{
					this->pikesY -= MOVABLEPIKES_SPEED * time / 1000;
					if(this->pikesY <= 0)
					{
						this->pikesY = 0;
						this->trapMode = MOVABLEPIKES_READY;
						this->trapTimer = 0;
					}
				}
			}
		}
	}
}

/**
* Draw the MovablePikes
* @param lightPos Light position
*/
void MovablePikes::draw( float * lightPos)
{
	vec3_t light;

	light[0] = lightPos[0];
	light[1] = lightPos[1];
	light[2] = lightPos[2];

	glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
	glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
	glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
	glMaterialf (GL_FRONT,GL_SHININESS,3.0f);

	float dist = sqrt((this->x - player1->getCoor()[0]) * (this->x - player1->getCoor()[0]) + (this->z - player1->getCoor()[2]) * (this->z - player1->getCoor()[2]));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	if(dist > VIEWQUALITYMIN0 || this->lightQuality == 0)
		glCallList(this->objectNumber);
	else if(dist > VIEWQUALITYMIN1 || this->lightQuality == 1)
			glCallList(this->objectNumber + 1);
	else if(dist > VIEWQUALITYMIN2 || this->lightQuality == 2)
			glCallList(this->objectNumber + 2);
	else
		glCallList(this->objectNumber + 3);

	glDisable(GL_BLEND);

	if(this->trapMode != MOVABLEPIKES_READY)
	{
		glPushMatrix();
			glTranslatef(this->x - 1, this->pikesY ,this->z + 1);
			this->modelEntity->DrawEntity( 1, MD2E_DRAWNORMAL | MD2E_TEXTURED, light );
		glPopMatrix();
	}
}

/**
* Return the object type
* @return Object type
*/
int MovablePikes::getType()
{
	return OBJECTTYPE_TRAP;
}

/**
* Activate the MovablePikes function
* @return If the function is activated
*/
bool MovablePikes::useObject()
{
	if(this->player1->getCoor()[0] < this->x && this->player1->getCoor()[0] > this->x - 2.f && this->player1->getCoor()[2] < this->z + 2.0f && this->player1->getCoor()[2] > this->z )
	{
		if(this->trapMode >= MOVABLEPIKES_RAISE && this->trapMode <= MOVABLEPIKES_DESACTIVATE)
			this->player1->takeDamage(35);
		else
		{
			if(this->trapMode == MOVABLEPIKES_READY && this->player1->getMovementStatut() != PLAYER_MOVEMENT_JUMP)
			{
				this->soundM->playSound(this->activationSound);
				this->trapMode = MOVABLEPIKES_START_TIMER;
			}
		}
	}
	return false;
}

/**
* Calculate the Pikes
*/
void MovablePikes::calculate()
{
	for(int j = 0; j <= this->lightQuality ; j++)
	{
		this->objectQuality = (float)OptionManager::getInstance()->getCrateQuality(j);
		glNewList(this->objectNumber + j,GL_COMPILE); 
		glBindTexture  ( GL_TEXTURE_2D, this->textureId );
		glBegin(GL_QUADS);
		for( int i = 0 ; i < this->objectQuality ; i++)
		{
			for( int j = 0 ; j < this->objectQuality ; j++)
			{
				glNormal3f(0.0f, 1.0f, 0.0f);
				glTexCoord2f(i/this->objectQuality			,j/this->objectQuality);			glVertex3f(x - (2 * i/this->objectQuality)			, 0.001f	, z + (2 * j/this->objectQuality));
				glTexCoord2f((i + 1)/this->objectQuality	,j/this->objectQuality);			glVertex3f(x - (2 * (i + 1)/this->objectQuality)	, 0.001f	, z + (2 * j/this->objectQuality));
				glTexCoord2f((i + 1)/this->objectQuality	,(j + 1)/this->objectQuality);		glVertex3f(x - (2 * (i + 1)/this->objectQuality)	, 0.001f	, z + (2 * (j + 1)/this->objectQuality)); 	
				glTexCoord2f(i/this->objectQuality			,(j + 1)/this->objectQuality);		glVertex3f(x - (2 * i/this->objectQuality)			, 0.001f	, z + (2 * (j + 1)/this->objectQuality));
				
			}
		}
		glEnd();
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
bool MovablePikes::isCollision(float x, float z, float y)
{
	return y <= this->pikesY;
}
