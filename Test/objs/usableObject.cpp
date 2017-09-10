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

#include "../objs/usableObject.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param player1 Pointer to the Player
*/
UsableObject::UsableObject(int x, int z, Player * player1) : Obj(x,z,player1)
{
	this->messageM = MessageManager::getInstance();
	this->langM = LangManager::getInstance();

	this->rotation = 0;
	this->matType = OBJECTMAT_NONE;

	this->matSpec[0] = 0.0f;
	this->matSpec[1] = 0.0f;
	this->matSpec[2] = 0.0f;
	this->matSpec[3] = 1.0f;

	this->matDif[0] = 0.0f;
	this->matDif[1] = 0.0f;
	this->matDif[2] = 0.0f;
	this->matDif[3] = 1.0f;

	this->matAmb[0] = 0.2f;
	this->matAmb[1] = 0.2f;
	this->matAmb[2] = 0.2f;
	this->matAmb[3] = 1.0f;
}

/**
* Destructor
*/
UsableObject::~UsableObject()
{
	this->sound = NULL;
}

/**
* Refresh the UsableObject
* @param time Elapsed time
*/
void UsableObject::refresh(long time)
{
	this->rotation += time * this->objectRotation / 1000.0f;
	if(this->rotation > 360)
		this->rotation -= 360;
}

/**
* Draw the UsableObject
* @param lightPos Light position
*/
void UsableObject::draw( float * lightPos){}

/**
* Return the type of the UsableObject
* @return UsableObject type
*/
int UsableObject::getType()
{
	return 2;
}

/**
* Play the object related sound
*/
void UsableObject::playSound()
{
	this->soundM->playSound(this->sound);
}
