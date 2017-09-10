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

#ifndef __ENNEMYENTITY_H
#define __ENNEMYENTITY_H

#include "../includes/vector3d.h"
#include "../includes/vector2d.h"
#include "../elements/player.h"

enum
{
	ENEMY_STATE_ALIVE,
	ENEMY_STATE_FADDING,
	ENEMY_STATE_DEAD
};

enum
{
	IMMOBILE_ENEMY_ACTION_STATE_WAIT,
	IMMOBILE_ENEMY_ACTION_STATE_SEARCH,
	IMMOBILE_ENEMY_ACTION_STATE_ATTACK,
	IMMOBILE_ENEMY_ACTION_STATE_TRACK_DOWN,
	IMMOBILE_ENEMY_ACTION_STATE_STANDBY_FROM_SEARCH,
	IMMOBILE_ENEMY_ACTION_STATE_STANDBY_FROM_ATTACK,
};

/**
* EnemyEntity Class
* @author Stephane Baudoux
*/
class EnemyEntity
{
protected:
	int id;
	int health;
	int reactionTimeInterval;
	int actionState;
	int generalState;
	long actionStateTimer;
	long reactionTimer;
	long attackTimer;
	long attackDelay;
	float playerYAngle;
	float playerXZAngle;
	float playerTurretAngleXZ;
	float fireRange;
	float viewRange;
	float viewAngle;
	float rotationSpeed;
	float rotationXZ;
	float rotationY;
	float accuracy;

	float matSpec[4];
	float matDif[4];
	float matAmb[4];

	Vector3d coor;
	Vector3d lookAt;
	Vector3d lookAtPlayer;
	Vector2d viewSector[3];
	Player * player1;
	OptionManager * optM;
	SoundManager * soundM;
	Mix_Chunk * fireSound;
	Mix_Chunk * takeDamageSound;
	Mix_Chunk * attackModeSound;
	Mix_Chunk * destroyedSound;
	CEntity	* modelEntity;

public:
	EnemyEntity(Vector3d coordinate, Player * playerPt, int enemyId, int ambient_light);
	virtual ~EnemyEntity();
	virtual void refresh(long time);
	virtual void draw();
	virtual void takeDamage(int damage);
	virtual void playerDetection(bool value);
	virtual int getHealth();
	virtual int getGeneralState();
	virtual bool isCollision(float x, float z, float y);

protected:
	virtual void makeDecision();
	virtual bool canAttack();
};

#endif
