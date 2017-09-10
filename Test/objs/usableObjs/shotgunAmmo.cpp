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
#include <string>
#include <GL/gl.h>
#include "../../includes/global_var.h"
#include "../../manager/optionManager.h"
#include "../../objs/usableObjs/shotgunAmmo.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param player1 Pointer to the Player
* @param ambientLight Ambient light value
*/
ShotgunAmmo::ShotgunAmmo(int x, int z, Player * player1, int ambientLight) : UsableObject(x,z,player1)
{
	std::ostringstream stringstream;
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/ammo/shotgun_ammo.tga";
	std::string texture = stringstream.str();

	this->modelEntity = ModelManager::getInstance()->loadModel("objs/ammo/shotgun_ammo.md2", texture);
	this->modelEntity->SetScale(0.4f);
	this->sound = soundM->loadSound("sounds/objects/shotgun_ammo.wav");
	this->objectRotation = 150;

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

	this->weaponM = WeaponManager::getInstance();
}

/**
* Destructor
*/
ShotgunAmmo::~ShotgunAmmo(){}

/**
* Draw the ShotgunAmmo
* @param lightPos Light position
*/
void ShotgunAmmo::draw(float * lightPos)
{
	vec3_t light;

	light[0] = 0;
	light[1] = 0;
	light[2] = 0;

	glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
	glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
	glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
	glMaterialf (GL_FRONT,GL_SHININESS,150.0f);

	glPushMatrix();
		glTranslatef(this->x - 1, 1.0f ,this->z + 1);
		glRotatef(this->rotation, 0, 1, 0);
		this->modelEntity->DrawEntity( 1, MD2E_DRAWNORMAL | MD2E_TEXTURED, light );
	glPopMatrix();
}

/**
* Return the type of the object
* @return Object type
*/
int ShotgunAmmo::getType()
{
	return OBJECTTYPE_AMMO;
}

/**
* Return the second object type if it exists
* @return Second object type
*/
int ShotgunAmmo::getSecondType()
{
	return WEAPON_SHOTGUN_ID;
}

/**
* Activate the ShotgunAmmo function
* Give 8 ammo if the Player has the Shotgun
* @return If the function is activated
*/
bool ShotgunAmmo::useObject()
{
	bool ret = false;
	if(this->weaponM->isWeaponAvailible(getSecondType()))
	{
		if(this->weaponM->getWeapon(WEAPON_SHOTGUN_ID)->getAmmo() < this->weaponM->getWeapon(WEAPON_SHOTGUN_ID)->getCarryingCapacity())
		{
			this->weaponM->addAmmo(WEAPON_SHOTGUN_ID,8);
			playSound();
			ret = true;
			this->messageM->addMessage(this->langM->getText(TEXT_INFORMATION_SHOTGUN_AMMO_ACQUIRED),TEXTMANAGER_ALIGN_LEFT,1.0f,1.0f,1.0f);
		}
		else
			this->messageM->addMessage(this->langM->getText(TEXT_INFORMATION_SHOTGUN_AMMO_HAVE),TEXTMANAGER_ALIGN_LEFT,1.0f,1.0f,1.0f);
	}
	else
		this->messageM->addMessage(this->langM->getText(TEXT_INFORMATION_SHOTGUN_AMMO_NEED_WEAPON),TEXTMANAGER_ALIGN_LEFT,1.0f,1.0f,1.0f);
	return ret;
}
