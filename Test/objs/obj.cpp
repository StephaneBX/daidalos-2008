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

#include "../includes/global_var.h"
#include "../objs/obj.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param player1 Pointer to the Player
*/
Obj::Obj(int x, int z, Player * player1)
{
	this->sound = NULL;
	this->soundM = SoundManager::getInstance();
	this->x = (float)-x * 2;
	this->z = (float)z * 2;
	this->impactEnabled = false;
	this->player1 = player1;
}

/**
* Destructor
*/
Obj::~Obj(){}

/**
* Return the type of the object
* @return Object type
*/
int Obj::getType()
{
	return OBJECTTYPE_IMMOVABLE;
}

/**
* Return the second object type if it exists
* @return Second object type
*/
int Obj::getSecondType()
{
	return -1;
}

/**
* Return the object material type
* @return Object material
*/
int Obj::getMatType()
{
	return this->matType;
}

/**
* Return the key needed to unlock the door
* @return Key needed
*/
int Obj::getKeyNeeded()
{
	return -1;
}

/**
* Return the object x coordinate
* @return X coordinate
*/
float Obj::getX()
{
	return this->x;
}

/**
* Return the object y coordinate
* @return Y coordinate
*/
float Obj::getZ()
{
	return this->z;
}

/**
* Activate the function
* @return If the function is activated
*/
bool Obj::useObject(){return false;}

/**
* Detect collisions
* @param x X coordinate to test
* @param z Z coordinate to test
* @param y Y coordinate to test
* @return Result to the collsion test
*/
bool Obj::isCollision(float x, float z, float y)
{
	return false;
}

/**
* Return if the object can have bullet impacts
* @return Bullet impact state
*/
bool Obj::getImpactEnabled()
{
	return this->impactEnabled;
}

/**
* Return if the object has to replace the player
* @return If need replacement
*/
bool Obj::needReplacement()
{
	return false;
}

/**
* Refresh object
* @param time Elapsed time
*/
void Obj::refresh(long time){}

/**
* Draw the Obj
* @param lightPos Light position
*/
void Obj::draw(float * lightPos){}

/**
* Play the object related sound
*/
void Obj::playSound(){}

/**
* Play the sound at the appropriate volume
* @param volume Volume modifier
*/
void Obj::playSound(int volume)
{
	if(this->sound != NULL)
		this->soundM->playSound(this->sound, volume);
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
void Obj::impactGetLocation(float x1, float y1, float z1, float x2, float y2, float z2, float &impactX, float &impactY, float &impactZ, float &impactAngleX, float &impactAngleY, float &impactAngleZ, int &modif, float impactRadius){}
