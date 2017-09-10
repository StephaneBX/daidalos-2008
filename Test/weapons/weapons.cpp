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

#include "../weapons/weapons.h"

/**
* Constructor
*/
Weapon::Weapon()
{
	this->soundM = SoundManager::getInstance();
	this->ammo = 0;
	this->loaderAmount = this->loaderCapacity;
	this->drawFlash = false;
}

/**
* Destructor
*/
Weapon::~Weapon()
{
	this->sound = NULL;
	delete this->weaponFlash;
	this->weaponFlash = NULL;
}

/**
* Return the number of ammo left
* @return Ammo left
*/
int Weapon::getAmmo()
{
	return this->ammo;
}

/**
* Return the range
* @return Range
*/
int Weapon::getRange()
{
	return this->range;
}

/**
* Return the accuracy
* @return Accuracy
*/
int Weapon::getAccuracy()
{
	return this->accuracy;
}

/**
* Return the fire delay
* @return Fire delay
*/
int Weapon::getFireDelay()
{
	return this->fireDelay;
}

/**
* Return the loader capacity
* @return Loader capacity
*/
int Weapon::getLoaderCapacity()
{
	return this->loaderCapacity;
}

/**
* Return the carrying capacity
* @return Carrying capacity
*/
int Weapon::getCarryingCapacity()
{
	return this->carryingCapacity;
}

/**
* Return the ammo left in the loader
* @return Ammo left in the loader
*/
int Weapon::getLoaderAmount()
{
	return this->loaderAmount;
}

/**
* Return the reload time
* @return Reload time
*/
int Weapon::getReloadTime()
{
	return this->reloadTime;
}

/**
* Return the number of Impact per shot
* @return Impact per shot
*/
int Weapon::getImpactPerShot()
{
	return this->impactPerShot;
}

/**
* Return the power of the shot
* @return weapon power
*/
int Weapon::getPower()
{
	return this->power;
}

/**
* Return the angle
* @return Angle
*/
float Weapon::getAngle()
{
	return this->angle;
}

/**
* Return the last frame number
* @return Last frame number
*/
int Weapon::getLastFrame()
{
	return this->lastFrame;
}

/**
* Return the Impact texture id
* @return Impact texture id
*/
int Weapon::getImpactId()
{
	return this->impactId;
}

/**
* Return the Impact size
* @return Impact size
*/
float Weapon::getImpactSize()
{
	return this->impactSize;
}

/**
* Return the minimal y angle allowed
* @return Minimal y angle allowed
*/
float Weapon::getAngleMini()
{
	return this->angleMini;
}

/**
* Return the weapon's length
* @return Weapon's length
*/
float Weapon::getLength()
{
	return this->length;
}

/**
* set the drawFlash variable
* @param v New draw flash state
*/
void Weapon::setFlash(bool v)
{
	this->drawFlash = v;
}

/**
* Draw the weapon
* @param playerX Player x coordinate
* @param playerY Player y coordinate
* @param playerZ Player z coordinate
* @param playerAngleXZ Player XZ angle
* @param playerAngleY Player Y angle
* @param lightPos Light position
* @param angleOffset Angle Offset
*/
void Weapon::draw( float playerX, float playerY, float playerZ, float playerAngleXZ, float playerAngleY, float * lightPos , float angleOffset ){}

/**
* Draw the flash
* @param playerX Player x coordinate
* @param playerY Player y coordinate
* @param playerZ Player z coordinate
* @param playerAngleXZ Player XZ angle
* @param playerAngleY Player Y angle
* @param lightPos Light position
* @param angleOffset Angle Offset
*/
void Weapon::drawWeaponFlash( float playerX, float playerY, float playerZ, float playerAngleXZ, float playerAngleY, float * lightPos , float angleOffset )
{
	this->angle = - playerAngleY + 50.0f - angleOffset;

	if( this->angle < this->angleMini)
		this->angle = this->angleMini;

	if(this->drawFlash)
	{
		this->weaponFlash->changeAngle(true);

		glPushMatrix();
			glTranslatef(playerX,playerY + 1,playerZ - 2);
			glRotatef(playerAngleXZ,0,1,0);
			glTranslatef(-0.25f,1.7f,0.0);
			glRotatef(this->angle,1,0,0);
			glTranslatef(0,0,this->length - 0.1f);
			glRotatef(playerAngleY,0,0,1);
			this->weaponFlash->draw();
		glPopMatrix();
	}
	else
	{
		this->weaponFlash->changeAngle(false);
	}
}

/**
* Increase the number of bullets
* @param val Increased number
*/
void Weapon::inscreaseAmmo(int val)
{
	this->ammo += val;
	if(this->ammo > this->carryingCapacity)
		this->ammo = this->carryingCapacity;
}

/**
* Return the entity
* @return Entity
*/
CEntity * Weapon::getEntity()
{
	return this->entity;
}

/**
* Fire
*/
void Weapon::fire()
{
	if(this->loaderAmount > 0)
	{
		this->soundM->playSound(this->sound);
		this->loaderAmount--;
	}
}

/**
* Reload the weapon
*/
void Weapon::reload()
{
	int bullet = this->loaderCapacity - this->loaderAmount;
	if(bullet <= this->ammo)
	{
		this->loaderAmount = loaderCapacity;
		this->ammo -= bullet;
	}
	else
	{
		this->loaderAmount += ammo;
		this->ammo = 0;
	}
	this->soundM->playSound(this->reloadSound);
}
