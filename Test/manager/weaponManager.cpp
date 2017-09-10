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
#include "../includes/global_var.h"
#include "../includes/vector3d.h"
#include "../weapons/handgun.h"
#include "../weapons/shotgun.h"
#include "../manager/weaponManager.h"

/**
* Constructor
*/
WeaponManager::WeaponManager()
{
	this->soundM = SoundManager::getInstance();
	this->modM = ModManager::getInstance();
	this->optM = OptionManager::getInstance();
	this->weaponDatabase = new Weapon*[NUMBEROFWEAPONS];
	for(int i = 0 ; i < NUMBEROFWEAPONS ; i++)
		this->weaponDatabase[i] = NULL;

	if(this->modM->getStartConfiguration(MOD_CONFIGURATION_START_ELEMENTS_HANDGUN))
	{
		this->weaponDatabase[WEAPON_HANDGUN_ID] = new Handgun();
		this->weaponDatabase[WEAPON_HANDGUN_ID]->inscreaseAmmo(this->modM->getStartConfigurationValue(MOD_CONFIGURATION_START_VALUE_HANDGUN));
	}

	if(this->modM->getStartConfiguration(MOD_CONFIGURATION_START_ELEMENTS_SHOTGUN))
	{
		this->weaponDatabase[WEAPON_SHOTGUN_ID] = new Shotgun();
		this->weaponDatabase[WEAPON_SHOTGUN_ID]->inscreaseAmmo(this->modM->getStartConfigurationValue(MOD_CONFIGURATION_START_VALUE_SHOTGUN));
	}
	
	if(this->modM->getStartConfiguration(MOD_CONFIGURATION_START_ELEMENTS_HANDGUN))
	{
		this->hasWeapon = true;
		this->weaponSelected = WEAPON_HANDGUN_ID;
	}
	else
	{
		if(this->modM->getStartConfiguration(MOD_CONFIGURATION_START_ELEMENTS_SHOTGUN))
		{
			this->hasWeapon = true;
			this->weaponSelected = WEAPON_SHOTGUN_ID;
		}
		else
		{
			this->hasWeapon = false;
			this->weaponSelected = -1;
		}
	}

	this->mode = WEAPON_MODE_NORMAL;
	this->angleOffset = 0;
	this->currentDelay = 0;
	this->currentFrame = 0;
	this->changeWeaponSound = this->soundM->loadSound("sounds/misc/changeWeapon.wav");

	if(this->optM->getParticleModifier() > 0)
		this->smoke = new DynamicParticleSystem(PARTICLE_SMOKE,80 / this->optM->getParticleModifier());
	else
		this->smoke = NULL;
}

/**
* Destructor
*/
WeaponManager::~WeaponManager()
{
	for(int i = 0 ; i < NUMBEROFWEAPONS ; i++)
		delete this->weaponDatabase[i];
	delete[] this->weaponDatabase;
	this->weaponDatabase = NULL;

	if(this->smoke != NULL)
	{
		delete this->smoke;
		this->smoke = NULL;
	}
}

/**
* Add a weapon
* @param number Weapon number
*/
void WeaponManager::addWeapon(int number)
{
	switch(number)
	{
		case WEAPON_HANDGUN_ID:
		this->weaponDatabase[WEAPON_HANDGUN_ID] = new Handgun();
		break;

		case WEAPON_SHOTGUN_ID:
		this->weaponDatabase[WEAPON_SHOTGUN_ID] = new Shotgun();
		break;
	}
}

/**
* Refresh weapons values
* @param time Elapsed time
* @param playerX Player x coordinate
* @param playerY Player y coordinate
* @param playerZ Player z coordinate
* @param playerAngleXZ Player XZ angle
* @param playerAngleY Player Y angle
*/
void WeaponManager::refresh(long time, float playerX, float playerY, float playerZ, float playerAngleXZ, float playerAngleY)
{
	if(getHasWeapon())
	{
		if(this->mode == WEAPON_MODE_FIRE)
		{
			this->currentDelay += time;

			// Weapon Light
			if(!this->fireLight && this->currentDelay >= 80)
			{
				this->fireLight = true;
				glDisable(GL_LIGHT1);
			}

			if(this->currentDelay >= FLASHTIMEDRAWN)
				this->weaponDatabase[this->weaponSelected]->setFlash(false);

			if((this->currentDelay + time) > this->weaponDatabase[this->weaponSelected]->getFireDelay())
			{
				this->mode = WEAPON_MODE_NORMAL;
				this->currentDelay = 0;
				this->currentFrame = 0;
				this->weaponDatabase[this->weaponSelected]->getEntity()->Animate(0,0,0);
			}
			else
			{
				this->weaponDatabase[this->weaponSelected]->getEntity()->Animate(this->currentFrame,this->currentFrame,100.0);
				this->currentFrame = (int)(currentDelay / WEAPONTIMEPERFRAME);
				if(this->currentFrame > this->weaponDatabase[this->weaponSelected]->getLastFrame())
					this->currentFrame = this->weaponDatabase[this->weaponSelected]->getLastFrame();
			}
		}
		else
		{
			if(this->mode == WEAPON_MODE_DOWN)
			{
				if(this->angleOffset <= -30.0f)
				{
					this->weaponSelected = this->nextWeapon;
					this->mode = WEAPON_MODE_UP;

					if(this->optM->getParticleModifier() > 0)
						this->smoke->fillSystemLife();
				}
				else
					this->angleOffset -= 90.0f * time / 1000.0f;
			}
			else
			{
				if(this->mode == WEAPON_MODE_UP)
				{
					if(this->angleOffset >= 0)
					{
						this->angleOffset = 0;
						this->mode = WEAPON_MODE_NORMAL;
					}
					else
						this->angleOffset += 90 * time / 1000.0f;
				}
				else
				{
					if(this->mode == WEAPON_MODE_RELOAD)
					{
						if(this->currentDelay == 0)
						{
							if(this->angleOffset > -30.0f)
							{
								this->angleOffset -= 90.0f * time / 1000.0f;
							}
							else
							{
								this->angleOffset = -30.0f;
								this->currentDelay += time;
								this->weaponDatabase[this->weaponSelected]->reload();
							}
						}
						else
						{
							if(this->currentDelay < this->weaponDatabase[this->weaponSelected]->getReloadTime())
							{
								this->currentDelay += time;
							}
							else
							{
								if(this->angleOffset >= 0)
								{
									this->angleOffset = 0;
									this->mode = WEAPON_MODE_NORMAL;
									this->currentDelay = 0;
								}
								else
									this->angleOffset += 90 * time / 1000.0f;
							}
						}
					}
					else
					{
						if(this->mode == WEAPON_MODE_DISABLED)
						{
							if(this->angleOffset > -30.0f)
								this->angleOffset -= 90.0f * time / 1000.0f;
						}
						else
						{
							if(this->mode == WEAPON_MODE_ENABLED)
							{
								if(this->angleOffset >= 0)
									this->mode = WEAPON_MODE_NORMAL;
								else
									this->angleOffset += 90 * time / 1000.0f;
							}
						}
					}
				}
			}
		}
		if(this->optM->getParticleModifier() > 0)
		{
			//Smoke data update
			float smokeAngle = - playerAngleY + 50.0f - angleOffset;
			if(smokeAngle < this->weaponDatabase[this->weaponSelected]->getAngleMini())
				smokeAngle = this->weaponDatabase[this->weaponSelected]->getAngleMini();

			Vector3d smokeStart = Vector3d(0.0f,0.0f,0.0f);
			smokeStart += Vector3d(playerX,playerY + 2.6f,playerZ - 2);
			smokeStart[0] += sin( (playerAngleXZ - 90.0f) / 180.0f * PI ) * 0.3f;
			smokeStart[2] += cos( (playerAngleXZ - 90.0f) / 180.0f * PI ) * 0.3f;
			smokeStart[1] += sin( -smokeAngle / 180.0f * PI ) * 1.2f;
			smokeStart[0] += sin( playerAngleXZ / 180.0f * PI ) * this->weaponDatabase[this->weaponSelected]->getLength();
			smokeStart[2] += cos( playerAngleXZ / 180.0f * PI ) * this->weaponDatabase[this->weaponSelected]->getLength();

			//Smoke refresh
			this->smoke->refresh(time, smokeStart, playerAngleXZ);
		}
	}
}

/**
* Draw the weapon
* @param playerX Player x coordinate
* @param playerY Player y coordinate
* @param playerZ Player z coordinate
* @param playerAngleXZ Player XZ angle
* @param playerAngleY Player Y angle
* @param lightPos Light position
*/
void WeaponManager::drawWeapon(float playerX, float playerY, float playerZ, float playerAngleXZ, float playerAngleY, float * lightPos)
{
	if(getHasWeapon())
	{
		this->weaponDatabase[this->weaponSelected]->drawWeaponFlash(playerX, playerY, playerZ, playerAngleXZ, playerAngleY, lightPos, this->angleOffset);

		if(this->optM->getParticleModifier() > 0)
			this->smoke->draw();

		this->weaponDatabase[this->weaponSelected]->draw(playerX, playerY, playerZ, playerAngleXZ, playerAngleY, lightPos, this->angleOffset);
	}
}

/**
* Fire with the weapon
*/
void WeaponManager::fireWeapon()
{
	this->mode = WEAPON_MODE_FIRE;
	this->weaponDatabase[this->weaponSelected]->fire();
	this->weaponDatabase[this->weaponSelected]->setFlash(true);
	this->fireLight = false;

	if(this->optM->getParticleModifier() > 0)
		this->smoke->resetSystemLife();

	glEnable(GL_LIGHT1);
}

/**
* Reload the weapon
*/
void WeaponManager::reloadWeapon()
{
	this->mode = WEAPON_MODE_RELOAD;
	this->currentDelay = 0;
}

/**
* Select the next weapon
*/
void WeaponManager::selectNextWeapon()
{
	this->nextWeapon = this->weaponSelected + 1;
	while(!isWeaponAvailible(this->nextWeapon % NUMBEROFWEAPONS))
		this->nextWeapon++;
	this->nextWeapon = this->nextWeapon % NUMBEROFWEAPONS;
	this->mode = WEAPON_MODE_DOWN;
	this->currentDelay = 0;
	this->soundM->playSound(this->changeWeaponSound);
}

/**
* Select the previous weapon
*/
void WeaponManager::selectPreviousWeapon()
{
	this->nextWeapon = this->weaponSelected - 1;
	while(!isWeaponAvailible(this->nextWeapon))
	{
		this->nextWeapon--;
		if(this->nextWeapon < 0)
			this->nextWeapon = NUMBEROFWEAPONS - 1;
	}
	this->nextWeapon = this->nextWeapon % NUMBEROFWEAPONS;
	this->mode = WEAPON_MODE_DOWN;
	this->currentDelay = 0;
	this->soundM->playSound(this->changeWeaponSound);
}

/**
* Select a weapon
* @param number Weapon number
*/
void WeaponManager::selectWeapon(int number)
{
	if(isWeaponAvailible(number) && this->weaponSelected != number)
	{
		this->nextWeapon = number;
		if(!getHasWeapon())
		{
			this->hasWeapon = true;
			this->angleOffset = -30.0f;
			this->weaponSelected = number;
			this->mode = WEAPON_MODE_UP;
		}
		else
			this->mode = WEAPON_MODE_DOWN;
		this->currentDelay = 0;
		this->soundM->playSound(this->changeWeaponSound);
	}
}

/**
* Enable / Disable the weapon
* @param v Enable/Disable weapon
*/
void WeaponManager::setEnabled(bool v)
{
	if(v)
	{
		this->mode = WEAPON_MODE_ENABLED;
	}
	else
	{
		this->weaponDatabase[this->weaponSelected]->setFlash(false);
		this->mode = WEAPON_MODE_DISABLED;
	}
}

/**
* Add ammo to a weapon
* @param weapon Weapon number
* @param number Number of ammo
*/
void WeaponManager::addAmmo(int weapon, int number)
{
	if(isWeaponAvailible(weapon))
		this->weaponDatabase[weapon]->inscreaseAmmo(number);
}

/**
* Get the mode of the weapon
* @return Weapon's mode
*/
int WeaponManager::getMode()
{
	return this->mode;
}

/**
* Get the selected weapon
* @return A pointer to the weapon or NULL
*/
Weapon * WeaponManager::getSelectedWeapon()
{
	if(this->weaponSelected >= 0)
		return this->weaponDatabase[this->weaponSelected];
	else
		return NULL;
}

/**
* Get an existing weapon
* @param v Weapon number
* @return A pointer to the weapon
*/
Weapon * WeaponManager::getWeapon(int v)
{
	return this->weaponDatabase[v];
}

/**
* Get the selected weapon number
* If no weapon is availible, return -1
* @return Weapon number
*/
int WeaponManager::getSelectedWeaponNumber()
{
	return this->weaponSelected;
}

/**
* Return if the player has the weapon
* @param v Weapon number
* @return If the Player has the weapon
*/
bool WeaponManager::isWeaponAvailible(int v)
{
	bool ret = false;
	if(v >= 0 && v < NUMBEROFWEAPONS)
	{
		if(this->weaponDatabase[v] != NULL)
			ret = true;
	}
	return ret;
}

/**
* Return if the weapon is enabled
* @return If the weapon is enabled
*/
bool WeaponManager::isEnabled()
{
	return this->mode != WEAPON_MODE_DISABLED;
}

/**
* Return if the player has at least one weapon
* @return If the Player has at least one weapon
*/
bool WeaponManager::getHasWeapon()
{
	return this->hasWeapon;
}
