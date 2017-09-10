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
#include "../../objs/usableObjs/shotgunObject.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param player1 Pointer to the Player
* @param ambientLight Ambient light value
*/
ShotgunObject::ShotgunObject(int x, int z, Player * player1, int ambientLight) : UsableObject(x,z,player1)
{
	std::ostringstream stringstream;
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/shotgun/shotgun.tga";
	std::string texture = stringstream.str();

	this->modelEntity = ModelManager::getInstance()->loadModel("objs/weapons/shotgun_object.md2", texture);
	this->modelEntity->SetScale(0.16f);
	this->sound = soundM->loadSound("sounds/objects/get_weapon.wav");
	this->objectRotation = 150;

	this->matSpec[0] = 0.4f;
	this->matSpec[1] = 0.4f;
	this->matSpec[2] = 0.4f;
	this->matSpec[3] = 1.0f;

	this->matDif[0] = 0.4f;
	this->matDif[1] = 0.4f;
	this->matDif[2] = 0.4f;
	this->matDif[3] = 1.0f;

	this->matAmb[0] = 0.6f * ambientLight / 100.0f;
	this->matAmb[1] = 0.6f * ambientLight / 100.0f;
	this->matAmb[2] = 0.6f * ambientLight / 100.0f;
	this->matAmb[3] = 1.0f;

	this->weaponM = WeaponManager::getInstance();
}

/**
* Destructor
*/
ShotgunObject::~ShotgunObject(){}

/**
* Draw the ShotgunObject
* @param lightPos Light position
*/
void ShotgunObject::draw( float * lightPos)
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
int ShotgunObject::getType()
{
	return OBJECTTYPE_WEAPON;
}

/**
* Return the second object type if it exists
* @return Second object type
*/
int ShotgunObject::getSecondType()
{
	return WEAPON_SHOTGUN_ID;
}

/**
* Activate the ShotgunObject function
* Give the Shotgun if the Player doesn't has it
* @return If the function is activated
*/
bool ShotgunObject::useObject()
{
	bool ret = false;
	if(!this->weaponM->isWeaponAvailible(getSecondType()))
	{
		this->weaponM->addWeapon(WEAPON_SHOTGUN_ID);
		this->weaponM->selectWeapon(WEAPON_SHOTGUN_ID);
		playSound();
		ret = true;
		this->messageM->addMessage(this->langM->getText(TEXT_INFORMATION_SHOTGUN_ACQUIRED),TEXTMANAGER_ALIGN_LEFT,1.0f,1.0f,1.0f);
	}
	else
		this->messageM->addMessage(this->langM->getText(TEXT_INFORMATION_SHOTGUN_HAVE),TEXTMANAGER_ALIGN_LEFT,1.0f,1.0f,1.0f);
	return ret;
}
