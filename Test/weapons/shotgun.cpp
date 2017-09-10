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
#include <stdlib.h>
#include <GL/gl.h>
#include "../manager/optionManager.h"
#include "../weapons/shotgun.h"

/**
* Constructor
*/
Shotgun::Shotgun() : Weapon()
{
	std::ostringstream stringstream;
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/shotgun/shotgun.tga";

	this->lastFrame = 19;
	this->entity = ModelManager::getInstance()->loadModel("objs/weapons/shotgun.md2", stringstream.str());
	this->sound = this->soundM->loadSound("sounds/objects/shotgun_fire.wav");
	this->reloadSound = this->soundM->loadSound("sounds/objects/shotgun_reload.wav");
	this->impactSize = 0.1f;
	this->fireDelay = 1000;
	this->power = 6;
	this->range = 4;
	this->accuracy = 2;
	this->loaderCapacity = 8;
	this->carryingCapacity = 64;
	this->loaderAmount = this->loaderCapacity;
	this->reloadTime = 3000;
	this->impactPerShot = 6;
	this->angleMini = -30.0f;
	this->length = 1.2f;

	stringstream.str("");
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/shotgun/flash.tga";
	this->weaponFlash = new Flash(stringstream.str(),0.3f);

	stringstream.str("");
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/shotgun/impact.tga";
	this->impactId = CTextureManager::getInstance()->LoadTexture(stringstream.str());

	stringstream.str("");
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/shotgun/impact2.tga";
	this->impactId2 = CTextureManager::getInstance()->LoadTexture(stringstream.str());

	stringstream.str("");
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/shotgun/impact3.tga";
	this->impactId3 = CTextureManager::getInstance()->LoadTexture(stringstream.str());
}

/**
* Destructor
*/
Shotgun::~Shotgun(){}

/**
* Draw the shotgun
* @param playerX Player x coordinate
* @param playerY Player y coordinate
* @param playerZ Player z coordinate
* @param playerAngleXZ Player XZ angle
* @param playerAngleY Player Y angle
* @param lightPos Light position
* @param angleOffset Angle Offset
*/
void Shotgun::draw( float playerX, float playerY, float playerZ, float playerAngleXZ, float playerAngleY, float * lightPos, float angleOffset )
{
	vec3_t light;

	light[0] = lightPos[0];
	light[1] = lightPos[1];
	light[2] = lightPos[2];

	glPushMatrix();

		glTranslatef(playerX,playerY / WEAPONTREMBLE + 1,playerZ - 2);
		glRotatef(playerAngleXZ,0,1,0);
		glTranslatef(-0.8f,1.0f,0.0f);
		glRotatef( this->angle ,1,0,0);
		glTranslatef(0,0,1.3f);
		glScalef(.3f,.3f,.3f);

		glDisable( GL_LIGHTING );
		glClear(GL_DEPTH_BUFFER_BIT);
		this->entity->DrawEntity( 1, MD2E_DRAWNORMAL | MD2E_TEXTURED | MD2E_ANIMATED | MD2E_CULLBACKFACE, light );
		glEnable( GL_LIGHTING );

	glPopMatrix();
}

/**
* Return a random impact file name
* @return Impact filename
*/
int Shotgun::getImpactId()
{
	int ret = 0;
	switch((int)((float)rand() / ((float)RAND_MAX + 1) * 3))
	{
		case 0:
		ret = this->impactId;
		break;

		case 1:
		ret = this->impactId2;
		break;

		case 2:
		ret = this->impactId3;
		break;
	}
	return ret;
}
