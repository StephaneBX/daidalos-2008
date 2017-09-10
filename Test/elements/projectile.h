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

#ifndef __PROJECTILE_H
#define __PROJECTILE_H

#include "../includes/vector3d.h"
#include "../manager/modelManager.h"

#define PROJECTILE_ROTATION_SPEED 550
#define PROJECTILE_FULL_SPEED_DISTANCE 10
#define PROJECTILE_SLOW_TYPE 10.0f

enum
{
	PROJECTILE_TYPE_INVISIBLE,
	PROJECTILE_TYPE_ARROW,
	PROJECTILE_TYPE_AXE,
	PROJECTILE_TYPE_ATTACK_TEST,
	PROJECTILE_SOURCE_PLAYER,
	PROJECTILE_SOURCE_ENEMY
};

/**
* Projectile Class
* Handle the projectiles in the game
* @author Stephane Baudoux
*/
class Projectile
{
private:
	Vector3d startCoordinates;
	Vector3d previousCoordinates;
	Vector3d currentCoordinates;
	float speed;
	float distance;
	float maxDistance;
	float angleXZ;
	float angleY;
	float power;
	float impactRadius;
	float rotation;
	float matSpec[4];
	float matDif[4];
	float matAmb[4];
	int impactId;
	int impactPolicy;
	int source;
	int type;

	CEntity	* modelEntity;

public:
	Projectile(Vector3d startCoordinates, int projectileType, int source, int impactId, int impactPolicy, float impactRadius, float speed, float maxDistance, float angleXZ, float angleY, float precisionMidifier, float power);
	~Projectile();
	void refresh(long time);
	void draw();
	float getPower();
	float getSpeed();
	float getImpactRadius();
	bool isAlive();
	int getImpactId();
	int getImpactPolicy();
	int getSource();
	int getType();
	Vector3d getPreviousCoordinates();
	Vector3d getCurrentCoordinates();
	Vector3d getStartCoordinates();
};

#endif
