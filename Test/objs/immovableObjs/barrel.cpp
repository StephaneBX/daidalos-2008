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
#include "../../objs/immovableObjs/barrel.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param player1 Pointer to the Player
* @param barrelType Type of barrel
* @param barrelTextureNumber Texture number
* @param ambientLight Ambient light value
*/
Barrel::Barrel(int x, int z, Player * player1, int barrelType, int barrelTextureNumber, int ambientLight) : ImmovableObject(x,z,player1)
{
	this->barrelTextureNumber = barrelTextureNumber;
	this->barrelType = barrelType;

	this->matSpec[0] = 0.5f;
	this->matSpec[1] = 0.5f;
	this->matSpec[2] = 0.5f;
	this->matSpec[3] = 1.0f;

	this->matDif[0] = 0.5f;
	this->matDif[1] = 0.5f;
	this->matDif[2] = 0.5f;
	this->matDif[3] = 1.0f;

	this->matAmb[0] = 0.4f * ambientLight / 100.0f;
	this->matAmb[1] = 0.4f * ambientLight / 100.0f;
	this->matAmb[2] = 0.4f * ambientLight / 100.0f;
	this->matAmb[3] = 1.0f;

	std::ostringstream stringstream;

	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/barrel/barrel" << this->barrelType << "_" << barrelTextureNumber << ".tga";
	std::string texture = stringstream.str();
	stringstream.str("");

	stringstream << "objs/barrel/high/barrel" << this->barrelType << ".md2";
	std::string filename = stringstream.str();
	stringstream.str("");

	stringstream << "objs/barrel/low/barrel" << this->barrelType << ".md2";
	std::string filenameLow = stringstream.str();

	this->barrelEntity = ModelManager::getInstance()->loadModel(filename, texture);
	this->barrelEntityLow = ModelManager::getInstance()->loadModel(filenameLow, texture);

	if(barrelType == 1)
	{
		this->sound = this->soundM->loadSound("sounds/environment/metal_impact.wav");
		this->matType = OBJECTMAT_STEEL;
	}
	else
	{
		this->sound = this->soundM->loadSound("sounds/environment/wood_impact.wav");
		this->matType = OBJECTMAT_WOOD;
	}
}

/**
* Destructor
*/
Barrel::~Barrel(){}

/**
* Draw the barrel
* @param lightPos Light position
*/
void Barrel::draw( float * lightPos)
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

	if(this->barrelType == 1)
	{
		glPushMatrix();
			glTranslatef(this->x - 1, 0.85f ,this->z + 1);
			glScalef(.8f,.8f,.8f);
			if(dist > VIEWQUALITYMIN2)
				this->barrelEntityLow->DrawEntity( 1, MD2E_DRAWNORMAL | MD2E_TEXTURED, light );
			else
				this->barrelEntity->DrawEntity( 1, MD2E_DRAWNORMAL | MD2E_TEXTURED, light );
		glPopMatrix();
	}
	else
	{
		if(this->barrelType == 2)
		{
			glPushMatrix();
				glTranslatef(this->x - 1, 0 ,this->z + 1);
				glScalef(.8f,.8f,.8f);
				if(dist > VIEWQUALITYMIN2)
					this->barrelEntityLow->DrawEntity( 1, MD2E_DRAWNORMAL | MD2E_TEXTURED, light );
				else
					this->barrelEntity->DrawEntity( 1, MD2E_DRAWNORMAL | MD2E_TEXTURED, light );
			glPopMatrix();
		}
	}
}

/**
* Detect collisions
* @param x X coordinate to test
* @param z Z coordinate to test
* @param y  coordinate to test
* @return Collision test result
*/
bool Barrel::isCollision(float x, float z, float y)
{
	bool ret = false;
	if(y >= 0 && y <= 2.15)
	{
		float xDiff,zDiff,distance;
		xDiff = this->x - 1 - x;
		zDiff = this->z + 1 - z;
		distance = sqrt(xDiff * xDiff + zDiff * zDiff);
		if(this->barrelType == 1 && distance <= 0.8 || this->barrelType == 2 && distance <= 0.95)
			ret = true;
	}
	return ret;
}
