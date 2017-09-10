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

#include <GL/gl.h>
#include "../../manager/optionManager.h"
#include "../../elements/2D/cursor.h"

/**
* Constructor
*/
Cursor::Cursor()
{
	OptionManager::getInstance()->getResolution(this->height, this->width);
	this->ratio = (float)this->height / (float)this->width;
	this->decal = 0.010f;
}

/**
* Destructor
*/
Cursor::~Cursor(){}

/**
* Refresh the cursor
* @param pl A pointer to the Player
* @param weaponM A pointer to the WeaponManager
*/
void Cursor::refresh(Player * pl, WeaponManager * weaponM)
{
	if(pl->getMovementStatut() == PLAYER_MOVEMENT_CROUCH)
	{
		if(pl->getMoved())
			this->decal = 0.010f;
		else
			this->decal = 0.005f;
	}
	else
	{
		if(pl->getMovementStatut() == PLAYER_MOVEMENT_STAND)
		{
			if(!pl->getMoved())
				this->decal = 0.010f;
			else
				this->decal = 0.020f;
		}
		else
		{
			if(pl->getMovementStatut() == PLAYER_MOVEMENT_JUMP)
				this->decal = 0.035f;
			else
				this->decal = 0.020f;
		}
	}

	this->decal = this->decal + 0.03f * (2.0f / (float)weaponM->getSelectedWeapon()->getAccuracy());
}

/**
* Draw the Cursor
*/
void Cursor::draw()
{
	glDisable( GL_TEXTURE_2D );

	glBegin(GL_LINES);
		glColor3d(1.0,0.2,0.2);

		glVertex2d(0.5,0.51 + this->decal);
		glVertex2d(0.5,0.51 + this->decal + 0.015);

		glVertex2d(0.5,0.51 - this->decal);
		glVertex2d(0.5,0.51 - this->decal - 0.015);

		glVertex2d(0.50 - this->decal / this->ratio - (0.015 / this->ratio),0.51);
		glVertex2d(0.50 - this->decal / this->ratio,0.51);

		glVertex2d(0.50 + this->decal / this->ratio,0.51);
		glVertex2d(0.50 + this->decal / this->ratio + (0.015 / this->ratio),0.51);
	glEnd();

	glEnable( GL_TEXTURE_2D );
}
