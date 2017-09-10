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

#include <sstream>
#include "../../includes/global_var.h"
#include "../../manager/modelManager.h"
#include "../../enemies/immobileEnemies/turret.h"

#define TURRET_VALID_ANGLE_INTERVAL 1.0f
#define TURRET_VALID_Y_INTERVAL 1.0f
#define TURRET_WAIT_TIME 1800

/**
* Constructor
* @param coodinate coordinate
* @param playerPt Pointer to the player
* @param enemyId Unique id for the enemy
*/
Turret::Turret(Vector3d coordinate, Player * playerPt, int enemyId, int ambient_light) : EnemyEntity(coordinate, playerPt, enemyId, ambient_light)
{
	this->projectileM = ProjectileManager::getInstance();
	this->logM = LogManager::getInstance();

	this->takeDamageSound = this->soundM->loadSound("sounds/enemies/turrets/impact1.wav");
	this->takeDamageSound2 = this->soundM->loadSound("sounds/enemies/turrets/impact2.wav");
	this->takeDamageSound3 = this->soundM->loadSound("sounds/enemies/turrets/impact3.wav");
	this->destroyedSound = this->soundM->loadSound("sounds/enemies/turrets/powerDown.wav");
}

/**
* Destructor
*/
Turret::~Turret()
{
	if(this->smoke != NULL)
	{
		delete this->smoke;
		this->smoke = NULL;
	}
	if(this->weaponFlash != NULL)
	{
		delete this->weaponFlash;
		this->weaponFlash = NULL;
	}
}

/**
* Refresh the Turret
* @param time Elapsed time
*/
void Turret::refresh(long time)
{
	EnemyEntity::refresh(time);

	if(this->generalState == ENEMY_STATE_ALIVE)
	{
		this->actionStateTimer += time;
		this->reactionTimer += time;
		if(this->reactionTimer >= this->reactionTimeInterval)
		{
			this->reactionTimer = 0;
			makeDecision();
		}

		//Action depending of the state
		switch(this->actionState)
		{
		case IMMOBILE_ENEMY_ACTION_STATE_ATTACK:
			this->attackTimer += time;
			if(this->attackTimer > FLASHTIMEDRAWN2)
			{
				this->drawFlash = false;
				this->drawBulletLine = false;
			}

			if(this->lookAtPlayer[1] < this->player1->getYCenter() - TURRET_VALID_Y_INTERVAL)
				this->rotationY -=  this->rotationSpeed * time / 100.0f;
			else
				if(this->lookAtPlayer[1] > this->player1->getYCenter() + TURRET_VALID_Y_INTERVAL)
					this->rotationY +=  this->rotationSpeed * time / 100.0f;

			if(this->playerTurretAngleXZ > TURRET_VALID_ANGLE_INTERVAL)
				this->rotationXZ += this->rotationSpeed * time / 100.0f;
			else
				if(this->playerTurretAngleXZ < -TURRET_VALID_ANGLE_INTERVAL)
					this->rotationXZ -= this->rotationSpeed * time / 100.0f;

			if(this->attackTimer >= this->attackDelay && canAttack())
			{
				this->attackTimer = 0;
				this->projectileM->addProjectile(this->coor, this->projectileType, PROJECTILE_SOURCE_ENEMY, this->impactId, IMPACT_REPLACEMENT_POLICY_CREATE, 0.1f, this->projectileSpeed, this->projectileMaxDistance,this->rotationXZ, 90 - this->rotationY, 1, 10);
				this->soundM->playSound(this->fireSound, SOUNDDISTANCEADDITION + (int)(sqrt((this->player1->getCoor()[0] - this->coor[0]) * (this->player1->getCoor()[0] - this->coor[0]) + (this->player1->getCoor()[2] - this->coor[2]) * (this->player1->getCoor()[2] - this->coor[2]))) / SOUNDDISTANCEMODIFIER);

				this->drawFlash = true;
				this->weaponFlash->changeAngle(true);

				this->drawBulletLine = true;
				this->bulletLineAngleXZ = this->rotationXZ;
				this->bulletLineAngleY = this->rotationY;

				if(this->optM->getParticleModifier() > 0)
					this->smoke->resetSystemLife();
			}
			break;

		case IMMOBILE_ENEMY_ACTION_STATE_SEARCH:
		case IMMOBILE_ENEMY_ACTION_STATE_STANDBY_FROM_SEARCH:
			this->rotationXZ += this->rotationSpeed * time / 100.0f;
			break;

		case IMMOBILE_ENEMY_ACTION_STATE_TRACK_DOWN:
			if(this->playerTurretAngleXZ > TURRET_VALID_ANGLE_INTERVAL)
				this->rotationXZ += this->rotationSpeed * time / 100.0f;
			else
				if(this->playerTurretAngleXZ < -TURRET_VALID_ANGLE_INTERVAL)
					this->rotationXZ -= this->rotationSpeed * time / 100.0f;
			break;

		case IMMOBILE_ENEMY_ACTION_STATE_STANDBY_FROM_ATTACK:
			if(this->lookAtPlayer[1] < this->player1->getYCenter() - TURRET_VALID_Y_INTERVAL)
				this->rotationY -=  this->rotationSpeed * time / 100.0f;
			else
				if(this->lookAtPlayer[1] > this->player1->getYCenter() + TURRET_VALID_Y_INTERVAL)
					this->rotationY +=  this->rotationSpeed * time / 100.0f;
			break;
		}
	}

	if(this->optM->getParticleModifier() > 0)
	{
		this->smokeStart[0] = this->coor[0] + sin(this->rotationXZ / 180.0f * PI) * this->smokeDistance;
		this->smokeStart[1] = this->coor[1] + sin((this->rotationY - 180) / 180.0f * PI) * this->smokeDistance;
		this->smokeStart[2] = this->coor[2] + cos(this->rotationXZ / 180.0f * PI) * this->smokeDistance;
		this->smoke->refresh(time, this->smokeStart, this->player1->getAngleXZ());
	}
}

/**
* Draw the Turret
*/
void Turret::draw()
{
	vec3_t light;

	light[0] = 0;
	light[1] = 0;
	light[2] = 0;

	glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
	glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
	glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
	glMaterialf (GL_FRONT,GL_SHININESS,150.0f);

	glPushMatrix();
		glTranslatef(this->coor[0], BLOCDEFAULTHEIGHT ,this->coor[2]);
		glRotatef(this->rotationXZ, 0, 1, 0);
		this->upperEntity->DrawEntity( 1, MD2E_DRAWNORMAL | MD2E_TEXTURED, light );
	glPopMatrix();

	glPushMatrix();
		glTranslatef(this->lowerPosition[0], this->lowerPosition[1] ,this->lowerPosition[2]);
		glRotatef(this->rotationXZ, 0, 1, 0);
		glRotatef(this->rotationY, 1, 0, 0);
		this->modelEntity->DrawEntity( 1, MD2E_DRAWNORMAL | MD2E_TEXTURED, light );
	glPopMatrix();

	if(this->drawFlash)
	{
		glPushMatrix();
			glTranslatef(this->smokeStart[0], this->smokeStart[1] ,this->smokeStart[2]);
			glRotatef(this->rotationXZ, 0, 1, 0);
			glRotatef(this->rotationY, 1, 0, 0);
			glTranslatef(0, 0 ,1.0f);
			glRotatef(180.0f, 0, 1, 0);
			this->weaponFlash->draw();
		glPopMatrix();
	}

	if(this->drawBulletLine)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture  ( GL_TEXTURE_2D, this->bulletLineId );
		glDisable(GL_LIGHTING);
		glBegin(GL_QUADS);
			glVertex3f(this->coor[0] + sin(this->bulletLineAngleXZ / 180.0f * PI) * this->smokeDistance + sin((this->bulletLineAngleXZ + 90.0f) / 180.0f * PI) * 0.02f, this->coor[1] + sin((this->bulletLineAngleY - 180) / 180.0f * PI) * this->smokeDistance, this->coor[2] + cos(this->bulletLineAngleXZ / 180.0f * PI) * this->smokeDistance + cos((this->bulletLineAngleXZ + 90.0f) / 180.0f * PI) * 0.02f);
			glVertex3f(this->coor[0] + sin(this->bulletLineAngleXZ / 180.0f * PI) * this->smokeDistance + sin((this->bulletLineAngleXZ - 90.0f) / 180.0f * PI) * 0.02f, this->coor[1] + sin((this->bulletLineAngleY - 180) / 180.0f * PI) * this->smokeDistance, this->coor[2] + cos(this->bulletLineAngleXZ / 180.0f * PI) * this->smokeDistance + cos((this->bulletLineAngleXZ - 90.0f) / 180.0f * PI) * 0.02f);
			glVertex3f(this->coor[0] + sin(this->bulletLineAngleXZ / 180.0f * PI) * this->smokeDistance + sin((this->bulletLineAngleXZ - 90.0f) / 180.0f * PI) * 0.02f + sin(this->bulletLineAngleXZ / 180.0f * PI) * this->fireRange, this->coor[1] + sin((this->bulletLineAngleY - 180) / 180.0f * PI) * this->smokeDistance + sin((this->bulletLineAngleY - 180) / 180.0f * PI) * this->fireRange, this->coor[2] + cos(this->bulletLineAngleXZ / 180.0f * PI) * this->smokeDistance + cos((this->bulletLineAngleXZ - 90.0f) / 180.0f * PI) * 0.02f + cos(this->bulletLineAngleXZ / 180.0f * PI) * this->fireRange);
			glVertex3f(this->coor[0] + sin(this->bulletLineAngleXZ / 180.0f * PI) * this->smokeDistance + sin((this->bulletLineAngleXZ + 90.0f) / 180.0f * PI) * 0.02f + sin(this->bulletLineAngleXZ / 180.0f * PI) * this->fireRange, this->coor[1] + sin((this->bulletLineAngleY - 180) / 180.0f * PI) * this->smokeDistance + sin((this->bulletLineAngleY - 180) / 180.0f * PI) * this->fireRange, this->coor[2] + cos(this->bulletLineAngleXZ / 180.0f * PI) * this->smokeDistance + cos((this->bulletLineAngleXZ + 90.0f) / 180.0f * PI) * 0.02f + cos(this->bulletLineAngleXZ / 180.0f * PI) * this->fireRange);
		glEnd();
		glEnable(GL_LIGHTING);

		glDisable(GL_BLEND);
	}

	if(this->optM->getParticleModifier() > 0)
		this->smoke->draw();
}

/**
* Hurt the Turret
* @param damage Damage
*/
void Turret::takeDamage(int damage)
{
	//Random impact sound
	switch(rand()%3)
	{
		case 0:
		this->soundM->playSound(this->takeDamageSound, SOUNDDISTANCEADDITION + (int)(sqrt((this->player1->getCoor()[0] - this->coor[0]) * (this->player1->getCoor()[0] - this->coor[0]) + (this->player1->getCoor()[2] - this->coor[2]) * (this->player1->getCoor()[2] - this->coor[2]))) / SOUNDDISTANCEMODIFIER2);
		break;

		case 1:
		this->soundM->playSound(this->takeDamageSound2, SOUNDDISTANCEADDITION + (int)(sqrt((this->player1->getCoor()[0] - this->coor[0]) * (this->player1->getCoor()[0] - this->coor[0]) + (this->player1->getCoor()[2] - this->coor[2]) * (this->player1->getCoor()[2] - this->coor[2]))) / SOUNDDISTANCEMODIFIER2);
		break;

		case 2:
		this->soundM->playSound(this->takeDamageSound3, SOUNDDISTANCEADDITION + (int)(sqrt((this->player1->getCoor()[0] - this->coor[0]) * (this->player1->getCoor()[0] - this->coor[0]) + (this->player1->getCoor()[2] - this->coor[2]) * (this->player1->getCoor()[2] - this->coor[2]))) / SOUNDDISTANCEMODIFIER2);
		break;
	}

	if(this->generalState == ENEMY_STATE_ALIVE)
	{
		this->health -= damage;
		if(this->health <= 0)
		{
			this->health = 0;
			this->soundM->playSound(this->destroyedSound, SOUNDDISTANCEADDITION + (int)(sqrt((this->player1->getCoor()[0] - this->coor[0]) * (this->player1->getCoor()[0] - this->coor[0]) + (this->player1->getCoor()[2] - this->coor[2]) * (this->player1->getCoor()[2] - this->coor[2]))) / SOUNDDISTANCEMODIFIER);
			this->generalState = ENEMY_STATE_FADDING;
			this->drawFlash = false;
			this->drawBulletLine = false;
		}
		else
		{
			if(this->actionState != IMMOBILE_ENEMY_ACTION_STATE_ATTACK)
				this->actionState = IMMOBILE_ENEMY_ACTION_STATE_TRACK_DOWN;
		}
	}
}

/**
* Method called by the LevelManager when a test projectile gives a result
* Used to know if the player is hidden behind a wall or an object
* @param value True if the player is detected, False if the player is hidden
*/
void Turret::playerDetection(bool value)
{
	//this->logM->logIt("Receiving","Player Detection");
	if(value && (this->actionState == IMMOBILE_ENEMY_ACTION_STATE_STANDBY_FROM_ATTACK || this->actionState == IMMOBILE_ENEMY_ACTION_STATE_STANDBY_FROM_SEARCH))
	{
		//this->logM->logIt("Receiving Test =>OK","Attack mode");
		this->actionState = IMMOBILE_ENEMY_ACTION_STATE_ATTACK;
		this->soundM->playSound(this->attackModeSound, SOUNDDISTANCEADDITION + (int)(sqrt((this->player1->getCoor()[0] - this->coor[0]) * (this->player1->getCoor()[0] - this->coor[0]) + (this->player1->getCoor()[2] - this->coor[2]) * (this->player1->getCoor()[2] - this->coor[2]))) / SOUNDDISTANCEMODIFIER);
		this->actionStateTimer = 0;
		this->attackTimer = 0;
	}
	else
	{
		if(!value && this->actionState == IMMOBILE_ENEMY_ACTION_STATE_STANDBY_FROM_ATTACK)
		{
			//this->logM->logIt("Receiving Test =>FALSE","Wait mode");
			this->actionState = IMMOBILE_ENEMY_ACTION_STATE_WAIT;
			this->actionStateTimer = 0;
			this->attackTimer = 0;
		}
		else
		{
			if(!value && this->actionState == IMMOBILE_ENEMY_ACTION_STATE_STANDBY_FROM_SEARCH)
			{
				//this->logM->logIt("Receiving Test =>False","Search mode");
				this->actionState = IMMOBILE_ENEMY_ACTION_STATE_SEARCH;
				this->actionStateTimer = 0;
				this->attackTimer = 0;
			}
			else
			{
				if(value && this->actionState == IMMOBILE_ENEMY_ACTION_STATE_WAIT)
				{
					this->actionState = IMMOBILE_ENEMY_ACTION_STATE_ATTACK;
					this->soundM->playSound(this->attackModeSound, SOUNDDISTANCEADDITION + (int)(sqrt((this->player1->getCoor()[0] - this->coor[0]) * (this->player1->getCoor()[0] - this->coor[0]) + (this->player1->getCoor()[2] - this->coor[2]) * (this->player1->getCoor()[2] - this->coor[2]))) / SOUNDDISTANCEMODIFIER);
					this->actionStateTimer = 0;
					this->attackTimer = 0;
				}
				else
				{
					if(!value && this->actionState == IMMOBILE_ENEMY_ACTION_STATE_ATTACK)
					{
						this->actionState = IMMOBILE_ENEMY_ACTION_STATE_WAIT;
						this->actionStateTimer = 0;
						this->attackTimer = 0;
						this->drawFlash = false;
						this->drawBulletLine = false;
					}
				}
			}
		}
	}
}

/**
* Make decision in order to change strategy or not
* Called each time the minimum time reaction is reached by the refresh method
*/
void Turret::makeDecision()
{
	//this->logM->logIt("Making","Decision");
	switch(this->actionState)
		{
		case IMMOBILE_ENEMY_ACTION_STATE_STANDBY_FROM_ATTACK:
		case IMMOBILE_ENEMY_ACTION_STATE_STANDBY_FROM_SEARCH:
			//this->logM->logIt("Standby mode ->","Throw attack text");
			this->projectileM->addProjectile(this->coor, PROJECTILE_TYPE_ATTACK_TEST, this->id, 1, IMPACT_REPLACEMENT_POLICY_CREATE, 0.1f, 100.0f, 50.0f,this->playerXZAngle, 90 - this->playerYAngle, 1, 10);
			break;

		case IMMOBILE_ENEMY_ACTION_STATE_ATTACK:
			if(!canAttack())
			{
				//this->logM->logIt("Attack mode ->","Switching to Trackdown mode");
				this->actionState = IMMOBILE_ENEMY_ACTION_STATE_TRACK_DOWN;
				this->actionStateTimer = 0;
				this->drawFlash = false;
				this->drawBulletLine = false;
			}
			else
				this->projectileM->addProjectile(this->coor, PROJECTILE_TYPE_ATTACK_TEST, this->id, 1, IMPACT_REPLACEMENT_POLICY_CREATE, 0.1f, 100.0f, 50.0f,this->playerXZAngle, 90 - this->playerYAngle, 1, 10);
			break;

		case IMMOBILE_ENEMY_ACTION_STATE_SEARCH:
			if(canAttack())
			{
				//this->logM->logIt("Search mode ->","Switching to Standby mode");
				this->actionState = IMMOBILE_ENEMY_ACTION_STATE_STANDBY_FROM_SEARCH;
				this->actionStateTimer = 0;
			}
			break;

		case IMMOBILE_ENEMY_ACTION_STATE_TRACK_DOWN:
			if(canAttack())
			{
				//this->logM->logIt("Trackdown mode ->","Switching to Standby mode");
				this->actionState = IMMOBILE_ENEMY_ACTION_STATE_STANDBY_FROM_ATTACK;
				this->actionStateTimer = 0;
			}
			break;

		case IMMOBILE_ENEMY_ACTION_STATE_WAIT:
			if(this->actionStateTimer >= TURRET_WAIT_TIME)
			{
				//this->logM->logIt("Wait mode ->","Switching to Search mode");
				this->actionState = IMMOBILE_ENEMY_ACTION_STATE_SEARCH;
				this->actionStateTimer = 0;
			}
			else
			{
				if(canAttack())
					this->projectileM->addProjectile(this->coor, PROJECTILE_TYPE_ATTACK_TEST, this->id, 1, IMPACT_REPLACEMENT_POLICY_CREATE, 0.1f, 5.0f, 50.0f,this->playerXZAngle, 90 - this->playerYAngle, 1, 10);
			}
			break;
		}
}

/**
* Detect collisions
* @param x X coordinate to test
* @param z Z coordinate to test
* @param y Y coordinate to test
* @return Result to the collsion test
*/
bool Turret::isCollision(float x, float z, float y)
{
	Vector2d tmp;
	tmp[0] = this->coor[0] - x;
	tmp[1] = this->coor[2] - z;
	return tmp.normal() <= 0.8f && y >= 6.5f;
}
