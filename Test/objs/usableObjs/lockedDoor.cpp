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
#include "../../objs/usableObjs/lockedDoor.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param player1 Pointer to the Player
* @param doorType Door type
* @param keyType Key type
* @param doorTexture Door texture number
* @param doorUpperTexture Upper door texture number
* @param objectNumber GList number
* @param ambientLight Ambient light value
*/
LockedDoor::LockedDoor(int x, int z, Player * player1, int doorType, int keyType, int doorTexture, int doorUpperTexture, int objectNumber, int ambientLight) : UsableObject(x,z,player1)
{
	this->texturesManager = CTextureManager::getInstance();
	this->optM = OptionManager::getInstance();
	this->lightQuality = OptionManager::getInstance()->getLightQuality();

	this->doorType = doorType;
	this->keyNeeded = keyType;
	this->objectNumber = objectNumber;
	this->doorState = DOOR_CLOSED;
	this->doorTranslation = 0.0f;
	this->timeOpened = 0;
	this->sound = this->soundM->loadSound("sounds/environment/rock_impact.wav");
	this->soundStopped = this->soundM->loadSound("sounds/objects/door_stopped.wav");
	this->soundMoved = this->soundM->loadSound("sounds/objects/door_moved.wav");
	this->soundUnlocked = this->soundM->loadSound("sounds/objects/door_unlocked.wav");
	this->matType = OBJECTMAT_STEEL;
	this->impactEnabled = true;
	this->locked = true;

	std::ostringstream stringstream;
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/door/door_" << doorTexture << ".tga";
	this->doorTextureId = this->texturesManager->LoadTexture(stringstream.str());

	stringstream.str("");
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/wall" << doorUpperTexture << ".tga";
	this->doorUpperTextureId = this->texturesManager->LoadTexture(stringstream.str());

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

	switch(this->keyNeeded)
	{
		case OBJECTTYPE_KEY_BLUE:
		stringstream.str("");
		stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/door/door_" << doorTexture << "_blue.tga";
		this->doorTextureLockedId = this->texturesManager->LoadTexture(stringstream.str());
		break;

		case OBJECTTYPE_KEY_GREEN:
		stringstream.str("");
		stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/door/door_" << doorTexture << "_green.tga";
		this->doorTextureLockedId = this->texturesManager->LoadTexture(stringstream.str());
		break;

		case OBJECTTYPE_KEY_RED:
		stringstream.str("");
		stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/door/door_" << doorTexture << "_red.tga";
		this->doorTextureLockedId = this->texturesManager->LoadTexture(stringstream.str());
		break;

		case OBJECTTYPE_KEY_YELLOW:
		stringstream.str("");
		stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/door/door_" << doorTexture << "_yellow.tga";
		this->doorTextureLockedId = this->texturesManager->LoadTexture(stringstream.str());
		break;
	}

	this->doorTextureUsedId = this->doorTextureLockedId;

	calculate();
}

/**
* Destructor
*/
LockedDoor::~LockedDoor(){}


/**
* Refresh the LockedDoor
* @param time Elapsed time
*/
void LockedDoor::refresh(long time)
{
	this->playerX = this->player1->getCoor()[0];
	this->playerZ = this->player1->getCoor()[2];

	if(this->doorState == DOOR_OPENING)
	{
		this->doorTranslation += 3.0f * time / 1000.0f;
		if(this->doorTranslation >= (DOORDEFAULTHEIGHT - 0.1f))
		{
			this->doorTranslation = (DOORDEFAULTHEIGHT - 0.1f);
			this->doorState = DOOR_OPENED;
			this->soundM->playSound(this->soundStopped,1 + (int)(sqrt((playerX - this->x) * (playerX - this->x) + (playerZ - this->z) * (playerZ - this->z))) / 15);
		}
	}
}

/**
* Draw the LockedDoor
* @param lightPos Light position
*/
void LockedDoor::draw( float * lightPos)
{
	glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
	glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
	glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
	glMaterialf (GL_FRONT,GL_SHININESS,50.0f);

	// Upper wall
	float dist = sqrt((this->x - player1->getCoor()[0]) * (this->x - player1->getCoor()[0]) + (this->z - player1->getCoor()[2]) * (this->z - player1->getCoor()[2]));
	if(dist > VIEWQUALITYMIN0 || this->lightQuality == 0)
	{
		glCallList(this->objectNumber);
		this->doorQuality = (float)this->optM->getDoorQuality(0);
	}
	else 
	{
		if(dist > VIEWQUALITYMIN1 || this->lightQuality == 1)
		{
			glCallList(this->objectNumber + 1);
			this->doorQuality = (float)this->optM->getDoorQuality(1);
		}
		else
		{
			if(dist > VIEWQUALITYMIN2 || this->lightQuality == 2)
			{
				glCallList(this->objectNumber + 2);
				this->doorQuality = (float)this->optM->getDoorQuality(2);
			}
			else
			{
				glCallList(this->objectNumber + 3);
				this->doorQuality = (float)this->optM->getDoorQuality(3);
			}
		}
	}
	

	// Doors
	glBindTexture(GL_TEXTURE_2D, this->doorTextureUsedId);
	if(this->doorType == 1)
	{
		glBegin(GL_QUADS);
		for( int i = 0 ; i < this->doorQuality ; i++)
		{
			for( int j = 0 ; j < this->doorQuality ; j++)
			{
				// Front
				glNormal3f(0.0f, 0.0f, -1.0f);
				glTexCoord2f(i/this->doorQuality			,j/this->doorQuality);			glVertex3f(x - (2 * i/this->doorQuality)		, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z + 1 - DOORDEFAULTTIKNESS);
				glTexCoord2f((i + 1)/this->doorQuality		,j/this->doorQuality);			glVertex3f(x - (2 * (i + 1)/this->doorQuality)	, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z + 1 - DOORDEFAULTTIKNESS);
				glTexCoord2f((i + 1)/this->doorQuality		,(j+1)/this->doorQuality);		glVertex3f(x - (2 * (i + 1)/this->doorQuality)	, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z + 1 - DOORDEFAULTTIKNESS); 	
				glTexCoord2f(i/this->doorQuality			,(j+1)/this->doorQuality);		glVertex3f(x - (2 * i/this->doorQuality)		, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z + 1 - DOORDEFAULTTIKNESS);

				// Behind
				glNormal3f(0.0f, 0.0f, 1.0f);
				glTexCoord2f((i + 1)/this->doorQuality		,j/this->doorQuality);			glVertex3f(x - (2 * (i + 1)/this->doorQuality)	, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z + 1 + DOORDEFAULTTIKNESS);
				glTexCoord2f(i/this->doorQuality			,j/this->doorQuality);			glVertex3f(x - (2 * i/this->doorQuality)		, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z + 1 + DOORDEFAULTTIKNESS);
				glTexCoord2f(i/this->doorQuality			,(j + 1)/this->doorQuality);	glVertex3f(x - (2 * i/this->doorQuality)		, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z + 1 + DOORDEFAULTTIKNESS); 	
				glTexCoord2f((i + 1)/this->doorQuality		,(j + 1)/this->doorQuality);	glVertex3f(x - (2 * (i + 1)/this->doorQuality)	, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z + 1 + DOORDEFAULTTIKNESS);

				// Left
				glNormal3f(1.0f, 0.0f, 0.0f);
				glTexCoord2f((i + 1)/this->doorQuality		,j/this->doorQuality);			glVertex3f(x									, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality));
				glTexCoord2f(i/this->doorQuality			,j/this->doorQuality);			glVertex3f(x									, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * i/this->doorQuality));
				glTexCoord2f(i/this->doorQuality			,(j + 1)/this->doorQuality);	glVertex3f(x									, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * i/this->doorQuality)); 	
				glTexCoord2f((i + 1)/this->doorQuality		,(j + 1)/this->doorQuality);	glVertex3f(x									, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality));

				// Right
				glNormal3f(-1.0f, 0.0f, 0.0f);
				glTexCoord2f(i/this->doorQuality			,j/this->doorQuality);			glVertex3f(x - 2								, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * i/this->doorQuality));
				glTexCoord2f((i + 1)/this->doorQuality		,j/this->doorQuality);			glVertex3f(x - 2								, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality));
				glTexCoord2f((i + 1)/this->doorQuality		,(j + 1)/this->doorQuality);	glVertex3f(x - 2								, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality)); 	
				glTexCoord2f(i/this->doorQuality			,(j + 1)/this->doorQuality);	glVertex3f(x - 2								, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * i/this->doorQuality));

				// Top
				glNormal3f(0.0f, 1.0f, 0.0f);
				glTexCoord2f(i/this->doorQuality			,j/this->doorQuality);			glVertex3f(x - (2 * i/this->doorQuality)		, -this->doorTranslation + DOORDEFAULTHEIGHT								, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * j/this->doorQuality));
				glTexCoord2f((i + 1)/this->doorQuality		,j/this->doorQuality);			glVertex3f(x - (2 * (i + 1)/this->doorQuality)	, -this->doorTranslation + DOORDEFAULTHEIGHT								, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * j/this->doorQuality));
				glTexCoord2f((i + 1)/this->doorQuality		,(j + 1)/this->doorQuality);	glVertex3f(x - (2 * (i + 1)/this->doorQuality)	, -this->doorTranslation + DOORDEFAULTHEIGHT								, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * (j + 1)/this->doorQuality)); 	
				glTexCoord2f(i/this->doorQuality			,(j + 1)/this->doorQuality);	glVertex3f(x - (2 * i/this->doorQuality)		, -this->doorTranslation + DOORDEFAULTHEIGHT								, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * (j + 1)/this->doorQuality));

				// Front
				glNormal3f(0.0f, 0.0f, -1.0f);
				glTexCoord2f(i/this->doorQuality			,1.0f - j/this->doorQuality);		glVertex3f(x - (2 * i/this->doorQuality)		, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z + 1 - DOORDEFAULTTIKNESS);
				glTexCoord2f((i + 1)/this->doorQuality		,1.0f - j/this->doorQuality);		glVertex3f(x - (2 * (i + 1)/this->doorQuality)	, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z + 1 - DOORDEFAULTTIKNESS);
				glTexCoord2f((i + 1)/this->doorQuality		,1.0f - (j+1)/this->doorQuality);	glVertex3f(x - (2 * (i + 1)/this->doorQuality)	, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z + 1 - DOORDEFAULTTIKNESS); 	
				glTexCoord2f(i/this->doorQuality			,1.0f - (j+1)/this->doorQuality);	glVertex3f(x - (2 * i/this->doorQuality)		, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z + 1 - DOORDEFAULTTIKNESS);

				// Behind
				glNormal3f(0.0f, 0.0f, 1.0f);
				glTexCoord2f((i + 1)/this->doorQuality		,1.0f - j/this->doorQuality);		glVertex3f(x - (2 * (i + 1)/this->doorQuality)	, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z + 1 + DOORDEFAULTTIKNESS);
				glTexCoord2f(i/this->doorQuality			,1.0f - j/this->doorQuality);		glVertex3f(x - (2 * i/this->doorQuality)		, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z + 1 + DOORDEFAULTTIKNESS);
				glTexCoord2f(i/this->doorQuality			,1.0f - (j + 1)/this->doorQuality);	glVertex3f(x - (2 * i/this->doorQuality)		, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z + 1 + DOORDEFAULTTIKNESS); 	
				glTexCoord2f((i + 1)/this->doorQuality		,1.0f - (j + 1)/this->doorQuality);	glVertex3f(x - (2 * (i + 1)/this->doorQuality)	, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z + 1 + DOORDEFAULTTIKNESS);

				// Left
				glNormal3f(1.0f, 0.0f, 0.0f);
				glTexCoord2f((i + 1)/this->doorQuality		,j/this->doorQuality);			glVertex3f(x									, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality));
				glTexCoord2f(i/this->doorQuality			,j/this->doorQuality);			glVertex3f(x									, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * i/this->doorQuality));
				glTexCoord2f(i/this->doorQuality			,(j + 1)/this->doorQuality);	glVertex3f(x									, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * i/this->doorQuality)); 	
				glTexCoord2f((i + 1)/this->doorQuality		,(j + 1)/this->doorQuality);	glVertex3f(x									, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality));

				// Right
				glNormal3f(-1.0f, 0.0f, 0.0f);
				glTexCoord2f(i/this->doorQuality			,j/this->doorQuality);			glVertex3f(x - 2								, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * i/this->doorQuality));
				glTexCoord2f((i + 1)/this->doorQuality		,j/this->doorQuality);			glVertex3f(x - 2								, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality));
				glTexCoord2f((i + 1)/this->doorQuality		,(j + 1)/this->doorQuality);	glVertex3f(x - 2								, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality)); 	
				glTexCoord2f(i/this->doorQuality			,(j + 1)/this->doorQuality);	glVertex3f(x - 2								, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * i/this->doorQuality));

				// Bottom
				glNormal3f(0.0f, -1.0f, 0.0f);
				glTexCoord2f(i/this->doorQuality			,j/this->doorQuality);			glVertex3f(x - (2 * i/this->doorQuality)		, this->doorTranslation + DOORDEFAULTHEIGHT														, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * j/this->doorQuality));
				glTexCoord2f(i/this->doorQuality			,(j + 1)/this->doorQuality);	glVertex3f(x - (2 * i/this->doorQuality)		, this->doorTranslation + DOORDEFAULTHEIGHT														, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * (j + 1)/this->doorQuality));
				glTexCoord2f((i + 1)/this->doorQuality		,(j + 1)/this->doorQuality);	glVertex3f(x - (2 * (i + 1)/this->doorQuality)	, this->doorTranslation + DOORDEFAULTHEIGHT														, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * (j + 1)/this->doorQuality)); 	
				glTexCoord2f((i + 1)/this->doorQuality		,j/this->doorQuality);			glVertex3f(x - (2 * (i + 1)/this->doorQuality)	, this->doorTranslation + DOORDEFAULTHEIGHT														, z + 1 - DOORDEFAULTTIKNESS + (2 * DOORDEFAULTTIKNESS * j/this->doorQuality));
			}
		}
		glEnd();
	}
	else
	{
		if(this->doorType == 2)
		{
			glBegin(GL_QUADS);
			for( int i = 0 ; i < this->doorQuality ; i++)
			{
				for( int j = 0 ; j < this->doorQuality ; j++)
				{
					// Front
					glNormal3f(0.0f, 0.0f, -1.0f);
					glTexCoord2f(i/this->doorQuality			,j/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * i/this->doorQuality)			, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z);
					glTexCoord2f((i + 1)/this->doorQuality		,j/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality)	, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z);
					glTexCoord2f((i + 1)/this->doorQuality		,(j+1)/this->doorQuality);		glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality)	, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z); 	
					glTexCoord2f(i/this->doorQuality			,(j+1)/this->doorQuality);		glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * i/this->doorQuality)			, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z);

					// Behind
					glNormal3f(0.0f, 0.0f, 1.0f);
					glTexCoord2f((i + 1)/this->doorQuality		,j/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality)	, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z + 2);
					glTexCoord2f(i/this->doorQuality			,j/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * i/this->doorQuality)			, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z + 2);
					glTexCoord2f(i/this->doorQuality			,(j + 1)/this->doorQuality);	glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * i/this->doorQuality)			, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z + 2); 	
					glTexCoord2f((i + 1)/this->doorQuality		,(j + 1)/this->doorQuality);	glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality)	, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z + 2);

					// Left
					glNormal3f(1.0f, 0.0f, 0.0f);
					glTexCoord2f((i + 1)/this->doorQuality		,j/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS															, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z + (2 * (i + 1)/this->doorQuality));
					glTexCoord2f(i/this->doorQuality			,j/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS															, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z + (2 * i/this->doorQuality));
					glTexCoord2f(i/this->doorQuality			,(j + 1)/this->doorQuality);	glVertex3f(x - 1 + DOORDEFAULTTIKNESS															, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z + (2 * i/this->doorQuality)); 	
					glTexCoord2f((i + 1)/this->doorQuality		,(j + 1)/this->doorQuality);	glVertex3f(x - 1 + DOORDEFAULTTIKNESS															, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z + (2 * (i + 1)/this->doorQuality));

					// Right
					glNormal3f(-1.0f, 0.0f, 0.0f);
					glTexCoord2f(i/this->doorQuality			,j/this->doorQuality);			glVertex3f(x - 1 - DOORDEFAULTTIKNESS															, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z + (2 * i/this->doorQuality));
					glTexCoord2f((i + 1)/this->doorQuality		,j/this->doorQuality);			glVertex3f(x - 1 - DOORDEFAULTTIKNESS															, j/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation			, z + (2 * (i + 1)/this->doorQuality));
					glTexCoord2f((i + 1)/this->doorQuality		,(j + 1)/this->doorQuality);	glVertex3f(x - 1 - DOORDEFAULTTIKNESS															, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z + (2 * (i + 1)/this->doorQuality)); 	
					glTexCoord2f(i/this->doorQuality			,(j + 1)/this->doorQuality);	glVertex3f(x - 1 - DOORDEFAULTTIKNESS															, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT - this->doorTranslation		, z + (2 * i/this->doorQuality));

					// Top
					glNormal3f(0.0f, 1.0f, 0.0f);
					glTexCoord2f(j/this->doorQuality			,i/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * i/this->doorQuality)			, -this->doorTranslation + DOORDEFAULTHEIGHT								, z + (2 * j/this->doorQuality));
					glTexCoord2f((j + 1)/this->doorQuality		,i/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality)	, -this->doorTranslation + DOORDEFAULTHEIGHT								, z + (2 * j/this->doorQuality));
					glTexCoord2f((j + 1)/this->doorQuality		,(i + 1)/this->doorQuality);	glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality)	, -this->doorTranslation + DOORDEFAULTHEIGHT								, z + (2 * (j + 1)/this->doorQuality)); 	
					glTexCoord2f(j/this->doorQuality			,(i + 1)/this->doorQuality);	glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * i/this->doorQuality)			, -this->doorTranslation + DOORDEFAULTHEIGHT								, z + (2 * (j + 1)/this->doorQuality));


					// Front
					glNormal3f(0.0f, 0.0f, -1.0f);
					glTexCoord2f(i/this->doorQuality			,j/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * i/this->doorQuality)			, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z);
					glTexCoord2f((i + 1)/this->doorQuality		,j/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality)	, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z);
					glTexCoord2f((i + 1)/this->doorQuality		,(j+1)/this->doorQuality);		glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality)	, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z); 	
					glTexCoord2f(i/this->doorQuality			,(j+1)/this->doorQuality);		glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * i/this->doorQuality)			, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z);

					// Behind
					glNormal3f(0.0f, 0.0f, 1.0f);
					glTexCoord2f((i + 1)/this->doorQuality		,j/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality)	, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z + 2);
					glTexCoord2f(i/this->doorQuality			,j/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * i/this->doorQuality)			, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z + 2);
					glTexCoord2f(i/this->doorQuality			,(j + 1)/this->doorQuality);	glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * i/this->doorQuality)			, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z + 2); 	
					glTexCoord2f((i + 1)/this->doorQuality		,(j + 1)/this->doorQuality);	glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality)	, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z + 2);

					// Left
					glNormal3f(1.0f, 0.0f, 0.0f);
					glTexCoord2f((i + 1)/this->doorQuality		,1.0f - j/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS															, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z + (2 * (i + 1)/this->doorQuality));
					glTexCoord2f(i/this->doorQuality			,1.0f - j/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS															, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z + (2 * i/this->doorQuality));
					glTexCoord2f(i/this->doorQuality			,1.0f - (j + 1)/this->doorQuality);	glVertex3f(x - 1 + DOORDEFAULTTIKNESS															, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z + (2 * i/this->doorQuality)); 	
					glTexCoord2f((i + 1)/this->doorQuality		,1.0f - (j + 1)/this->doorQuality);	glVertex3f(x - 1 + DOORDEFAULTTIKNESS															, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z + (2 * (i + 1)/this->doorQuality));

					// Right
					glNormal3f(-1.0f, 0.0f, 0.0f);
					glTexCoord2f(i/this->doorQuality			,1.0f - j/this->doorQuality);			glVertex3f(x - 1 - DOORDEFAULTTIKNESS															, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z + (2 * i/this->doorQuality));
					glTexCoord2f((i + 1)/this->doorQuality		,1.0f - j/this->doorQuality);			glVertex3f(x - 1 - DOORDEFAULTTIKNESS															, j/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT			, z + (2 * (i + 1)/this->doorQuality));
					glTexCoord2f((i + 1)/this->doorQuality		,1.0f - (j + 1)/this->doorQuality);	glVertex3f(x - 1 - DOORDEFAULTTIKNESS															, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z + (2 * (i + 1)/this->doorQuality)); 	
					glTexCoord2f(i/this->doorQuality			,1.0f - (j + 1)/this->doorQuality);	glVertex3f(x - 1 - DOORDEFAULTTIKNESS															, (j + 1)/this->doorQuality * DOORDEFAULTHEIGHT + this->doorTranslation + DOORDEFAULTHEIGHT		, z + (2 * i/this->doorQuality));

					// Bottom
					glNormal3f(0.0f, 1.0f, 0.0f);
					glTexCoord2f(j/this->doorQuality			,i/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * i/this->doorQuality)			, this->doorTranslation + DOORDEFAULTHEIGHT														, z + (2 * j/this->doorQuality));
					glTexCoord2f(j/this->doorQuality			,(i + 1)/this->doorQuality);	glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * i/this->doorQuality)			, this->doorTranslation + DOORDEFAULTHEIGHT														, z + (2 * (j + 1)/this->doorQuality));
					glTexCoord2f((j + 1)/this->doorQuality		,(i + 1)/this->doorQuality);	glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality)	, this->doorTranslation + DOORDEFAULTHEIGHT														, z + (2 * (j + 1)/this->doorQuality)); 		
					glTexCoord2f((j + 1)/this->doorQuality		,i/this->doorQuality);			glVertex3f(x - 1 + DOORDEFAULTTIKNESS - (2 * DOORDEFAULTTIKNESS * (i + 1)/this->doorQuality)	, this->doorTranslation + DOORDEFAULTHEIGHT														, z + (2 * j/this->doorQuality));
					
				}
			}
			glEnd();
		}
	}
}

/**
* Calculate the Locked Door
*/
void LockedDoor::calculate()
{
	// Upper wall
	for(int j = 0; j <= this->lightQuality ; j++)
	{
		this->doorUpperQuality = (float)OptionManager::getInstance()->getDoorUpperQuality(j);
		glNewList(this->objectNumber + j,GL_COMPILE); 
		glBindTexture(GL_TEXTURE_2D, this->doorUpperTextureId);

		glBegin(GL_QUADS);
		for( int i = 0 ; i < this->doorUpperQuality ; i++)
		{
			for( int j = 0 ; j < this->doorUpperQuality ; j++)
			{
				// Bottom
				glNormal3f(0.0f, 0.0f, -1.0f);
				glTexCoord2f(i/this->doorUpperQuality * 0.2f		+ 0.2f * ((int)(-this->x/2)%5)		,j/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);			glVertex3f(x - (2 * i/this->doorUpperQuality)		, DOORDEFAULTHEIGHT * 2.0f + j/this->doorUpperQuality * 2			, z);
				glTexCoord2f((i + 1)/this->doorUpperQuality * 0.2f	+ 0.2f * ((int)(-this->x/2)%5)		,j/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);			glVertex3f(x - (2 * (i + 1)/this->doorUpperQuality)	, DOORDEFAULTHEIGHT * 2.0f + j/this->doorUpperQuality * 2			, z);
				glTexCoord2f((i + 1)/this->doorUpperQuality * 0.2f	+ 0.2f * ((int)(-this->x/2)%5)		,(j+1)/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);		glVertex3f(x - (2 * (i + 1)/this->doorUpperQuality)	, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->doorUpperQuality * 2		, z); 	
				glTexCoord2f(i/this->doorUpperQuality * 0.2f		+ 0.2f * ((int)(-this->x/2)%5)		,(j+1)/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);		glVertex3f(x - (2 * i/this->doorUpperQuality)		, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->doorUpperQuality * 2		, z);

				// Top
				glNormal3f(0.0f, 0.0f, 1.0f);
				glTexCoord2f((i + 1)/this->doorUpperQuality * 0.2f	+ 0.2f * ((int)(-this->x/2)%5)		,j/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);			glVertex3f(x - (2 * (i + 1)/this->doorUpperQuality)	, DOORDEFAULTHEIGHT * 2.0f + j/this->doorUpperQuality * 2			, z + 2);
				glTexCoord2f(i/this->doorUpperQuality * 0.2f		+ 0.2f * ((int)(-this->x/2)%5)		,j/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);			glVertex3f(x - (2 * i/this->doorUpperQuality)		, DOORDEFAULTHEIGHT * 2.0f + j/this->doorUpperQuality * 2			, z + 2);
				glTexCoord2f(i/this->doorUpperQuality * 0.2f		+ 0.2f * ((int)(-this->x/2)%5)		,(j + 1)/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);	glVertex3f(x - (2 * i/this->doorUpperQuality)		, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->doorUpperQuality * 2		, z + 2); 	
				glTexCoord2f((i + 1)/this->doorUpperQuality * 0.2f	+ 0.2f * ((int)(-this->x/2)%5)		,(j + 1)/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);	glVertex3f(x - (2 * (i + 1)/this->doorUpperQuality)	, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->doorUpperQuality * 2		, z + 2);

				// Left
				glNormal3f(1.0f, 0.0f, 0.0f);
				glTexCoord2f((i + 1)/this->doorUpperQuality * 0.2f	+ 0.2f * ((int)(-this->z/2)%5)		,j/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);			glVertex3f(x, DOORDEFAULTHEIGHT * 2.0f + j/this->doorUpperQuality * 2			, z + (2 * (i + 1)/this->doorUpperQuality));
				glTexCoord2f(i/this->doorUpperQuality * 0.2f		+ 0.2f * ((int)(-this->z/2)%5)		,j/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);			glVertex3f(x, DOORDEFAULTHEIGHT * 2.0f + j/this->doorUpperQuality * 2			, z + (2 * i/this->doorUpperQuality));
				glTexCoord2f(i/this->doorUpperQuality * 0.2f		+ 0.2f * ((int)(-this->z/2)%5)		,(j + 1)/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);	glVertex3f(x, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->doorUpperQuality * 2		, z + (2 * i/this->doorUpperQuality)); 	
				glTexCoord2f((i + 1)/this->doorUpperQuality * 0.2f	+ 0.2f * ((int)(-this->z/2)%5)		,(j + 1)/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);	glVertex3f(x, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->doorUpperQuality * 2		, z + (2 * (i + 1)/this->doorUpperQuality));

				// Right
				glNormal3f(-1.0f, 0.0f, 0.0f);
				glTexCoord2f(i/this->doorUpperQuality * 0.2f		+ 0.2f * ((int)(this->z/2)%5)		,j/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT)+ (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);			glVertex3f(x - 2, DOORDEFAULTHEIGHT * 2.0f + j/this->doorUpperQuality * 2			, z + (2 * i/this->doorUpperQuality));
				glTexCoord2f((i + 1)/this->doorUpperQuality * 0.2f	+ 0.2f * ((int)(this->z/2)%5)		,j/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);			glVertex3f(x - 2, DOORDEFAULTHEIGHT * 2.0f+ j/this->doorUpperQuality * 2			, z + (2 * (i + 1)/this->doorUpperQuality));
				glTexCoord2f((i + 1)/this->doorUpperQuality * 0.2f	+ 0.2f * ((int)(this->z/2)%5)		,(j + 1)/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);	glVertex3f(x - 2, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->doorUpperQuality * 2		, z + (2 * (i + 1)/this->doorUpperQuality)); 	
				glTexCoord2f(i/this->doorUpperQuality * 0.2f		+ 0.2f * ((int)(this->z/2)%5)		,(j + 1)/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);	glVertex3f(x - 2, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->doorUpperQuality * 2		, z + (2 * i/this->doorUpperQuality));

				// Down
				glNormal3f(0.0f, -1.0f, 0.0f);
				glTexCoord2f(i/this->doorUpperQuality * 0.2f		+ 0.2f * ((int)(-this->x/2)%5)		,j/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);			glVertex3f(x - (2 * i/this->doorUpperQuality)		, DOORDEFAULTHEIGHT * 2.0f	, z + (2 * j/this->doorUpperQuality));
				glTexCoord2f(i/this->doorUpperQuality * 0.2f		+ 0.2f * ((int)(-this->x/2)%5)		,(j + 1)/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);	glVertex3f(x - (2 * i/this->doorUpperQuality)		, DOORDEFAULTHEIGHT * 2.0f	, z + (2 * (j + 1)/this->doorUpperQuality));
				glTexCoord2f((i + 1)/this->doorUpperQuality * 0.2f	+ 0.2f * ((int)(-this->x/2)%5)		,(j + 1)/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);	glVertex3f(x - (2 * (i + 1)/this->doorUpperQuality)	, DOORDEFAULTHEIGHT * 2.0f	, z + (2 * (j + 1)/this->doorUpperQuality)); 	
				glTexCoord2f((i + 1)/this->doorUpperQuality * 0.2f	+ 0.2f * ((int)(-this->x/2)%5)		,j/this->doorUpperQuality * ((BLOCDEFAULTHEIGHT - (DOORDEFAULTHEIGHT * 2)) / BLOCDEFAULTHEIGHT) + (DOORDEFAULTHEIGHT * 2) / BLOCDEFAULTHEIGHT);			glVertex3f(x - (2 * (i + 1)/this->doorUpperQuality)	, DOORDEFAULTHEIGHT * 2.0f	, z + (2 * j/this->doorUpperQuality));
			}
		}
		glEnd();
		glEndList();
	}
}

/**
* Return the type of the object
* @return Object type
*/
int LockedDoor::getType()
{
	int ret;
	if(this->doorType == 1)
		ret = OBJECTTYPE_LOCKEDDOOR_HORIZONTAL;
	else
		ret = OBJECTTYPE_LOCKEDDOOR_VERTICAL;
	return ret;
}

/**
* Return the second object type if it exists
* @return Second object type
*/
int LockedDoor::getSecondType()
{
	return this->doorState;
}

/**
* Return the key needed to unlock the door
* @return Key needed
*/
int LockedDoor::getKeyNeeded()
{
	return this->keyNeeded;
}

/**
* Activate the LockedDoor function
* @return If the function is activated
*/
bool LockedDoor::useObject()
{
	if(this->locked == true)
	{
		this->locked = false;
		this->doorState = DOOR_OPENING;
		this->soundM->playSound(this->soundUnlocked);
	}

	/*if(this->doorState == DOOR_CLOSING || this->doorState == DOOR_CLOSED)
	{
		this->doorState = DOOR_OPENING;
		this->timeOpened = 0;
		this->soundM->playSound(this->soundMoved,1 + (int)(sqrt((playerX - this->x) * (playerX - this->x) + (playerZ - this->z) * (playerZ - this->z))) / 15);
	}*/
	return true;
}

/**
* Detect collisions
* @param x X coordinate to test
* @param z Z coordinate to test
* @param y Y coordinate to test
* @return Result to the collsion test
*/
bool LockedDoor::isCollision(float x, float z, float y)
{
	bool ret = false;
	if(!ret)
	{
		if(y >= DOORDEFAULTHEIGHT * 2.0f || (y <= (DOORDEFAULTHEIGHT - this->doorTranslation) || y <= (DOORDEFAULTHEIGHT * 2.0f) && y >= DOORDEFAULTHEIGHT + this->doorTranslation) && (this->doorType == 1 && z >= this->z + 1 - DOORDEFAULTTIKNESS && z <= this->z + 1 + DOORDEFAULTTIKNESS || this->doorType == 2 && x >= this->x - 1 - DOORDEFAULTTIKNESS && x <= this->x - 1 + DOORDEFAULTTIKNESS))
		{
			ret = true;
			if(!this->locked)
				useObject();
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
void LockedDoor::impactGetLocation(float x1, float y1, float z1, float x2, float y2, float z2, float &impactX, float &impactY, float &impactZ, float &impactAngleX, float &impactAngleY, float &impactAngleZ, int &modif, float impactRadius)
{
	if(y1 > 2 * DOORDEFAULTHEIGHT)
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
			if(impactX < this->x - 2 + impactRadius)
				impactX = this->x - 2 + impactRadius;
			if(impactX > this->x - impactRadius)
				impactX = this->x - impactRadius;
			if(impactY < 2 * DOORDEFAULTHEIGHT + impactRadius)
				impactY = 2 * DOORDEFAULTHEIGHT + impactRadius;
			if(impactY > BLOCDEFAULTHEIGHT)
				impactY = BLOCDEFAULTHEIGHT;
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
				if(impactX < this->x - 2 + impactRadius)
					impactX = this->x - 2 + impactRadius;
				if(impactX > this->x - impactRadius)
					impactX = this->x - impactRadius;
				if(impactY < 2 * DOORDEFAULTHEIGHT + impactRadius)
					impactY = 2 * DOORDEFAULTHEIGHT + impactRadius;
				if(impactY > BLOCDEFAULTHEIGHT)
					impactY = BLOCDEFAULTHEIGHT;
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
			if(impactZ < this->z + impactRadius)
				impactZ = this->z + impactRadius;
			if(impactZ > this->z + 2 - impactRadius)
				impactZ = this->z + 2 - impactRadius;
			if(impactY < 2 * DOORDEFAULTHEIGHT + impactRadius)
				impactY = 2 * DOORDEFAULTHEIGHT + impactRadius;
			if(impactY > BLOCDEFAULTHEIGHT)
				impactY = BLOCDEFAULTHEIGHT;
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
				if(impactZ < this->z + impactRadius)
					impactZ = this->z + impactRadius;
				if(impactZ > this->z + 2 - impactRadius)
					impactZ = this->z + 2 - impactRadius;
				if(impactY < 2 * DOORDEFAULTHEIGHT + impactRadius)
					impactY = 2 * DOORDEFAULTHEIGHT + impactRadius;
				if(impactY > BLOCDEFAULTHEIGHT)
					impactY = BLOCDEFAULTHEIGHT;
			}
		}
	}
	else
	{
		modif = 0;
	}
}

/**
* Play the sound at the appropriate volume
* @param volume Volume modifier
*/
void LockedDoor::playSound(int volume)
{
	this->soundM->playSound(this->sound, volume);
}
