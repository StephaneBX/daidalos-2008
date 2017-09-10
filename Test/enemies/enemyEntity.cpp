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
#include "../enemies/enemyEntity.h"

/**
* Constructor
* @param coodinate coordinate
* @param playerPt Pointer to the player
* @param enemyId Unique id for the enemy
*/
EnemyEntity::EnemyEntity(Vector3d coordinate, Player * playerPt, int enemyId, int ambient_light)
{
	this->soundM = SoundManager::getInstance();
	this->optM = OptionManager::getInstance();

	this->actionStateTimer = 0;
	this->attackTimer = 0;
	this->reactionTimer = 0;

	this->id = enemyId;
	this->coor = coordinate;
	this->generalState = ENEMY_STATE_ALIVE;
	this->player1 = playerPt;
	this->viewSector[0][0] = this->coor[0];
	this->viewSector[0][1] = this->coor[2];
	this->health = 100;

	// Material specifications
	this->matSpec[0] = 0.1f;
	this->matSpec[1] = 0.1f;
	this->matSpec[2] = 0.1f;
	this->matSpec[3] = 1.0f;

	this->matDif[0] = 0.4f;
	this->matDif[1] = 0.4f;
	this->matDif[2] = 0.4f;
	this->matDif[3] = 1.0f;

	this->matAmb[0] = 0.4f * ambient_light / 100.0f;
	this->matAmb[1] = 0.4f * ambient_light / 100.0f;
	this->matAmb[2] = 0.4f * ambient_light / 100.0f;
	this->matAmb[3] = 1.0f;
}

/**
* Destructor
*/
EnemyEntity::~EnemyEntity(){}


/**
* Refresh the enemy
* @param time Elapsed time
*/
void EnemyEntity::refresh(long time)
{
	//Update the view and the variables that permit to know where is the player compared to the enemy position
	Vector3d tmpDist = this->player1->getCoor();
	tmpDist[1] = this->player1->getYCenter();
	float playerRange = (tmpDist - this->coor).length();

	this->viewSector[1][0] = this->coor[0] + cos((-this->rotationXZ - this->viewAngle / 2.0f + 90.0f) * PI / 180.0f) * this->viewRange;
	this->viewSector[1][1] = this->coor[2] + sin((-this->rotationXZ - this->viewAngle / 2.0f + 90.0f) * PI / 180.0f) * this->viewRange;
	this->viewSector[2][0] = this->coor[0] + cos((-this->rotationXZ + this->viewAngle / 2.0f + 90.0f) * PI / 180.0f) * this->viewRange;
	this->viewSector[2][1] = this->coor[2] + sin((-this->rotationXZ + this->viewAngle / 2.0f + 90.0f) * PI / 180.0f) * this->viewRange;

	this->lookAt[0] = this->coor[0] + sin(this->rotationXZ / 180.0f * PI) * this->viewRange;
	this->lookAt[1] = this->coor[1] + sin((this->rotationY - 180) / 180.0f * PI) * this->viewRange;
	this->lookAt[2] = this->coor[2] + cos(this->rotationXZ / 180.0f * PI) * this->viewRange;

	this->lookAtPlayer[0] = this->coor[0] + sin(this->rotationXZ / 180.0f * PI) * playerRange;
	this->lookAtPlayer[1] = this->coor[1] + sin((this->rotationY - 180) / 180.0f * PI) * playerRange;
	this->lookAtPlayer[2] = this->coor[2] + cos(this->rotationXZ / 180.0f * PI) * playerRange;

	Vector2d v1,v2;
	Vector3d v3;
	v1 = Vector2d(this->player1->getCoor()[0] - this->coor[0], this->player1->getCoor()[2] - this->coor[2]);
	v2 = Vector2d(this->lookAt[0] - this->coor[0], this->lookAt[2] - this->coor[2]);
	v1.normalize();
	v2.normalize();
	this->playerTurretAngleXZ = asin(Vector2d::crossProductLength(v1,v2)) * 180.0f / PI;

	v1 = Vector2d(this->player1->getCoor()[0] - this->coor[0], this->player1->getCoor()[2] - this->coor[2]);
	v3 = Vector3d(this->player1->getCoor()[0] - this->coor[0], this->player1->getYCenter() - this->coor[1], this->player1->getCoor()[2] - this->coor[2]);
	
	float v1Norm = v1.normal();
	float v3Norm = v3.length();
	float playerTurretAngleY = 90 - asin(v1Norm / v3Norm) * 180.0f / PI;

	this->playerYAngle = playerTurretAngleY;
	this->playerXZAngle = this->rotationXZ + this->playerTurretAngleXZ;
}

/**
* Draw the enemy
*/
void EnemyEntity::draw(){}

/**
* Hurt the enemy
* @param damage Damage
*/
void EnemyEntity::takeDamage(int damage){}

/**
* Method called by the LevelManager when a test projectile gives a result
* Used to know if the player is hidden behind a wall or an object
* @param value True if the player is detected, False if the player is hidden
*/
void EnemyEntity::playerDetection(bool value){}

/**
* Make decision in order to change strategy or not
* Called each time the minimum time reaction is reached by the refresh method
*/
void EnemyEntity::makeDecision(){}

/**
* Get the health value
* @return Health value
*/
int EnemyEntity::getHealth()
{
	return this->health;
}

/**
* Get the generalState value
* @return generalState value
*/
int EnemyEntity::getGeneralState()
{
	return this->generalState;
}

/**
* Detect collisions
* @param x X coordinate to test
* @param z Z coordinate to test
* @param y Y coordinate to test
* @return Result to the collsion test
*/
bool EnemyEntity::isCollision(float x, float z, float y)
{
	return false;
}

/**
* Return if the player is in fire range and in the view angle
* @return If the enemy can attack
*/
bool EnemyEntity::canAttack()
{
	Vector3d tmp = this->player1->getCoor() - this->coor;
	return (tmp.length() <= this->fireRange) && Vector2d::intersectionTrianglePoint(this->viewSector[0],this->viewSector[1],this->viewSector[2],Vector2d(this->player1->getCoor()[0],this->player1->getCoor()[2]));
}
