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

#ifndef __TURRET_H
#define __TURRET_H

#include "../../elements/flash.h"
#include "../../enemies/enemyEntity.h"
#include "../../manager/soundManager.h"
#include "../../manager/projectileManager.h"
#include "../../manager/optionManager.h"
#include "../../manager/logManager.h"
#include "../../particles/dynamicParticleSystem.h"

/**
* Turret Class
* Turret generic class
* @author Stephane Baudoux
*/
class Turret : public EnemyEntity
{
protected:
	int impactId;
	int bulletLineId;
	int projectileType;
	float projectileSpeed;
	float projectileMaxDistance;
	float smokeDistance;
	float bulletLineAngleXZ;
	float bulletLineAngleY;
	bool drawFlash;
	bool drawBulletLine;

	Vector3d smokeStart;
	Vector3d lowerPosition;
	ProjectileManager * projectileM;
	LogManager * logM;
	DynamicParticleSystem * smoke;
	Flash * weaponFlash;
	CEntity	* upperEntity;
	Mix_Chunk * takeDamageSound2;
	Mix_Chunk * takeDamageSound3;

public:
	Turret(Vector3d coordinate, Player * playerPt, int enemyId, int ambient_light);
	virtual ~Turret();
	virtual void refresh(long time);
	virtual void draw();
	virtual void takeDamage(int damage);
	virtual void playerDetection(bool value);
	virtual void makeDecision();
	virtual bool isCollision(float x, float z, float y);
};

#endif
