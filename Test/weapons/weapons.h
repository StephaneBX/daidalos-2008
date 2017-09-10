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

#ifndef __WEAPON_H
#define __WEAPON_H

#include "../manager/soundManager.h"
#include "../manager/modelManager.h"
#include "../elements/flash.h"

#define WEAPONTREMBLE 1.1f

/**
* Weapon Class
* Handle the weapons
* @author Stephane Baudoux
*/
class Weapon
{
protected:
	int ammo;
	int fireDelay;
	int power;
	int range;
	int accuracy;
	int loaderCapacity;
	int carryingCapacity;
	int loaderAmount;
	int impactPerShot;
	int reloadTime;
	int lastFrame;
	int impactId;
	float angle;
	float impactSize;
	float angleMini;
	float length;
	bool drawFlash;

	CEntity * entity;
	Mix_Chunk * sound;
	Mix_Chunk * reloadSound;
	Flash * weaponFlash;
	SoundManager * soundM;

public:
	Weapon();
	~Weapon();
	int getAmmo();
	int getRange();
	int getAccuracy();
	float getAngle();
	void setFlash(bool v);
	CEntity * getEntity();
	virtual int getFireDelay();
	virtual int getLoaderCapacity();
	virtual int getCarryingCapacity();
	virtual int getLoaderAmount();
	virtual int getReloadTime();
	virtual int getLastFrame();
	virtual int getImpactPerShot();
	virtual int getImpactId();
	virtual int getPower();
	virtual float getImpactSize();
	virtual float getAngleMini();
	virtual float getLength();
	virtual void draw( float playerX, float playerY, float playerZ, float playerAngleXZ, float playerAngleY, float * lightPos, float angleOffset );
	virtual void drawWeaponFlash(float playerX, float playerY, float playerZ, float playerAngleXZ, float playerAngleY, float * lightPos, float angleOffset);
	virtual void inscreaseAmmo(int val);
	virtual void fire();
	virtual void reload();
};

#endif
