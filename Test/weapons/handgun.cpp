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
#include "../weapons/handgun.h"

/**
* Constructor
*/
Handgun::Handgun() : Weapon()
{
	std::ostringstream stringstream;
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/handgun/handgun.tga";

	this->lastFrame = 9;
	this->entity = ModelManager::getInstance()->loadModel("objs/weapons/handgun.md2", stringstream.str());
	this->sound = this->soundM->loadSound("sounds/objects/handgun_fire.wav");
	this->reloadSound = this->soundM->loadSound("sounds/objects/handgun_reload.wav");
	this->impactSize = 0.12f;
	this->fireDelay = 400;
	this->power = 2;
	this->range = 6;
	this->accuracy = 5;
	this->loaderCapacity = 12;
	this->carryingCapacity = 120;
	this->loaderAmount = this->loaderCapacity;
	this->reloadTime = 1300;
	this->impactPerShot = 1;
	this->angleMini = -20.0f;
	this->length = 0.9f;

	stringstream.str("");
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/handgun/flash.tga";
	this->weaponFlash = new Flash(stringstream.str(),0.1f);

	stringstream.str("");
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/handgun/impact.tga";
	this->impactId = CTextureManager::getInstance()->LoadTexture(stringstream.str());
}

/**
* Destructor
*/
Handgun::~Handgun(){}

/**
* Draw the handgun
* @param playerX Player x coordinate
* @param playerY Player y coordinate
* @param playerZ Player z coordinate
* @param playerAngleXZ Player XZ angle
* @param playerAngleY Player Y angle
* @param lightPos Light position
* @param angleOffset Angle Offset
*/
void Handgun::draw( float playerX, float playerY, float playerZ, float playerAngleXZ, float playerAngleY, float * lightPos, float angleOffset )
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
		glTranslatef(0,0,2.2f);
		glScalef(.15f,.15f,.15f);

		glDisable( GL_LIGHTING );
		glClear(GL_DEPTH_BUFFER_BIT);
		this->entity->DrawEntity( 1, MD2E_DRAWNORMAL | MD2E_TEXTURED | MD2E_ANIMATED | MD2E_CULLBACKFACE, light );
		glEnable( GL_LIGHTING );

	glPopMatrix();
}
