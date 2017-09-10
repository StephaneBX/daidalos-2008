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

#ifndef __WEAPONMANAGER_H
#define __WEAPONMANAGER_H

#include "../weapons/weapons.h"
#include "../particles/dynamicParticleSystem.h"
#include "../manager/modManager.h"
#include "../manager/soundManager.h"
#include "../manager/weaponManager.h"
#include "../manager/optionManager.h"

#define NUMBEROFWEAPONS 3
#define WEAPONREACH 100
#define WEAPONTIMEPERFRAME 20

enum
{
	WEAPON_HANDGUN_ID,
	WEAPON_SHOTGUN_ID
};

enum
{
	WEAPON_MODE_NORMAL,
	WEAPON_MODE_FIRE,
	WEAPON_MODE_DOWN,
	WEAPON_MODE_UP,
	WEAPON_MODE_RELOAD,
	WEAPON_MODE_DISABLED,
	WEAPON_MODE_ENABLED
};

/**
* WeaponManager class
* Handle the weapons
* @author Stephane Baudoux
*/
class WeaponManager : public CSingleton<WeaponManager>
{
	friend class CSingleton<WeaponManager>;

private:
	int mode;
	int weaponSelected;
	int nextWeapon;
	int currentFrame;
	float angleOffset;
	long currentDelay;
	bool fireLight;
	bool hasWeapon;

	Weapon ** weaponDatabase;
	SoundManager * soundM;
	ModManager * modM;
	OptionManager * optM;
	Mix_Chunk * changeWeaponSound;
	DynamicParticleSystem * smoke;

public:
	void addWeapon(int number);
	void refresh(long time, float playerX, float playerY, float playerZ, float playerAngleXZ, float playerAngleY);
	void drawWeapon(float playerX, float playerY, float playerZ, float playerAngleXZ, float playerAngleY, float * lightPos);
	void fireWeapon();
	void reloadWeapon();
	void selectNextWeapon();
	void selectPreviousWeapon();
	void selectWeapon(int number);
	void setEnabled(bool v);
	void addAmmo(int weapon, int number);
	int getMode();
	int getSelectedWeaponNumber();
	Weapon * getSelectedWeapon();
	Weapon * getWeapon(int v);
	bool isWeaponAvailible(int v);
	bool isEnabled();
	bool getHasWeapon();

private:
	WeaponManager();
	~WeaponManager();
};

#endif
