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
#include <math.h>
#include <GL/gl.h>
#include "../includes/global_var.h"
#include "../includes/utility.h"
#include "../manager/optionManager.h"
#include "../elements/player.h"

/**
* Constructor
* @param playerCoor Player start coordinate
* @param angle View direction
* @param startHealth Default Health
* @param startArmor Default Armor
* @param flashlight If the player has the flashlight
*/
Player::Player(int * playerCoor, int angle, int startHealth, int startArmor, bool flashlight)
{
	// Player starting coordinates - For the respawn on death
	this->startCoordinates[0] = (float)playerCoor[0];
	this->startCoordinates[1] = (float)playerCoor[1];
	this->startCoordinates[2] = (float)playerCoor[2] - 2.0f;
	this->startOrientation[0] = (float)playerCoor[3];
	this->startOrientation[1] = (float)playerCoor[4];
	this->startOrientation[2] = (float)playerCoor[5];
	this->startAngleXZ = (float)angle;
	this->startHealth = startHealth;
	this->startArmor = startArmor;
	
	// Light variables
	this->lightWeaponDif[0]  = 0.2f;
	this->lightWeaponDif[1]  = 0.2f;
	this->lightWeaponDif[2]  = 0.2f;
	this->lightWeaponDif[3]  = 1.0f;
	this->lightWeaponSpec[0] = 0.2f;
	this->lightWeaponSpec[1] = 0.2f;
	this->lightWeaponSpec[2] = 0.2f;
	this->lightWeaponSpec[3] = 1.0f;
	this->lightWeaponAmb[0]  = 0.2f;
	this->lightWeaponAmb[1]  = 0.2f;
	this->lightWeaponAmb[2]  = 0.2f;
	this->lightWeaponAmb[3]  = 1.0f;
	this->spotWeaponDir[0]   = 0.0f;
	this->spotWeaponDir[1]   = 0.0f;
	this->spotWeaponDir[2]   = -1.0f;
	this->lightWeaponDir[0]  = -1.0f;
	this->lightWeaponDir[1]  = 0.0f;
	this->lightWeaponDir[2]  = 0.0f;
	this->lightWeaponPos[0]  = 15.0f;
	this->lightWeaponPos[1]  = 9.0f;
	this->lightWeaponPos[2]  = 20.0f;
	this->lightWeaponPos[3]  = 1.0f;

	// View
	this->viewDistance = (float)OptionManager::getInstance()->getGlobalVisibility() * GLOBALVISIBILITYMODIFIER + 45;
	if(OptionManager::getInstance()->getResolutionNumber() > 4)
		this->viewAngle = PLAYERVIEWANGLE2;
	else
		this->viewAngle = PLAYERVIEWANGLE1;

	// Managers
	this->keyM = KeyboardManager::getInstance();
	this->mouseM = MouseManager::getInstance();
	this->weaponM = WeaponManager::getInstance();
	this->soundM = SoundManager::getInstance();
	this->impactM = ImpactManager::getInstance();
	this->messageM = MessageManager::getInstance();

	this->flashlightAvailible = flashlight;

	// Key
	for(int i = 0 ; i < PLAYER_KEY_SIZE ; i++)
		this->keys[i] = false;

	if(this->flashlightAvailible)
	{
		this->light = true;
		glEnable(GL_LIGHT0);
	}
	else
	{
		this->light = false;
		glDisable(GL_LIGHT0);
	}

	// Steps tab allocation
	this->steps = new Mix_Chunk ** [PLAYERNUMBERSTEPS];
	for(int i = 0 ; i < 3 ; i++)
	{
		this->steps[i] = new Mix_Chunk * [PLAYERNUMBERSTEPPERSOUNDS];
		for(int j = 0 ; j < 4 ; j++)
			this->steps[i][j] = NULL;
	}

	// We load the soundsteps
	for(int i = 0 ; i < PLAYERNUMBERSTEPS ; i++)
	{
		std::ostringstream stringstream;

		stringstream << "sounds/steps/" << (i + 1) << "/step1_1.wav";
		this->steps[i][0] = this->soundM->loadSound(stringstream.str());
		stringstream.str("");

		stringstream << "sounds/steps/" << (i + 1) << "/step1_2.wav";
		this->steps[i][1] = this->soundM->loadSound(stringstream.str());
		stringstream.str("");

		stringstream << "sounds/steps/" << (i + 1) << "/step2_1.wav";
		this->steps[i][2] = this->soundM->loadSound(stringstream.str());
		stringstream.str("");

		stringstream << "sounds/steps/" << (i + 1) << "/step2_2.wav";
		this->steps[i][3] = this->soundM->loadSound(stringstream.str());
	}

	std::ostringstream stringstream;
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/blood/blood.tga";

	this->bloodId = CTextureManager::getInstance()->LoadTexture(stringstream.str());
	this->flashLightSound = this->soundM->loadSound("sounds/objects/flashlight.wav");
	this->jumpSound = this->soundM->loadSound("sounds/misc/jump.wav");
	this->hurtSound = this->soundM->loadSound("sounds/misc/hurt.wav");
	this->stepCounter = 0;

	// Initialization of the light used when firing
	glLightfv(GL_LIGHT1, GL_DIFFUSE, this->lightWeaponDif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, this->lightWeaponSpec);
	glLightfv(GL_LIGHT1, GL_AMBIENT, this->lightWeaponAmb);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.0005f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.00005f);

	respawn();
}

/**
* Destructor
*/
Player::~Player()
{
	// We delete the steps tab (but not the sounds!)
	for(int i = 0 ; i < PLAYERNUMBERSTEPS ; i++)
	{
		delete[] this->steps[i];
		this->steps[i] = NULL;
	}
	delete[] this->steps;
	this->steps = NULL;
}

/**
* Change the Player i coordinate
* @param i Coordinate type
* @param v new i coordinate
*/
void Player::setCoor(int i,float v)
{
	this->coor[i] = v;
}

/**
* Change the Player previous i coordinate
* @param i Previous coordinate type
* @param v new previous i coordinate
*/
void Player::setPreviousCoor(int i, float v)
{
	this->previousCoor[i] = v;
}

/**
* Change the movement statut
* @param v new movement statut
*/
void Player::setMovementStatut(int v)
{
	this->movementStatut = v;
}

/**
* Change the fired state
* @param v new fired state
*/
void Player::setFired(bool v)
{
	this->fired = v;
}

/**
* Refresh the Player
* @param time Elapsed time
* @param envNumber Environment number where the player is
*/
void Player::refresh( long time, int envNumber )
{
	this->previousCoor[0] = this->coor[0];
	this->previousCoor[2] = this->coor[2];
	this->currentEnv = envNumber - 1;
	this->moved = false;

	this->damageTimer += time;
	if(this->damageTimer > PLAYERDAMAGETIMER)
		this->damageTimer = PLAYERDAMAGETIMER;

	// Prevent segmentation fault if an unknow environement is selected
	if(this->currentEnv >= PLAYERNUMBERSTEPS)
		this->currentEnv = 1;

	this->imp -= PLAYERGRAVITY * time / 1000.0f;

	if(this->health > 0)
	{
		if(this->movementStatut != PLAYER_MOVEMENT_JUMP && this->jumpTimer > 0)
		{
			this->jumpTimer -= time;
			if(this->jumpTimer < 0)
				this->jumpTimer = 0;
		}

		// If the player type the crouch key
		if(keyM->isKeyPressed(KEYBOARD_GAME_CROUCH_KEY))
		{
			// We are in stand mode, We reduce the speed and change to crouching mode
			if(this->movementStatut == PLAYER_MOVEMENT_STAND)
			{
				keyM->setKey(KEYBOARD_GAME_CROUCH_KEY,0);
				this->movementStatut = PLAYER_MOVEMENT_STAND_TO_CROUCH;
				this->playerSpeed = PLAYERSPEED / 2.0;
			}
			else
			{
				// We are in crouch mode, so change to standing mode
				if(this->movementStatut == PLAYER_MOVEMENT_CROUCH)
				{
					keyM->setKey(KEYBOARD_GAME_CROUCH_KEY,0);
					if(this->allowStand)
						this->movementStatut = PLAYER_MOVEMENT_CROUCH_TO_STAND;
				}
			}
		}

		// If the player type de jump key
		if(keyM->isKeyPressed(KEYBOARD_GAME_JUMP_KEY))
		{
			keyM->setKey(KEYBOARD_GAME_JUMP_KEY,0);
			if(this->movementStatut == PLAYER_MOVEMENT_STAND && this->jumpTimer == 0)
			{
				this->imp = PLAYERIMPSPEED * 3;
				this->movementStatut = PLAYER_MOVEMENT_JUMP;
				this->soundM->playSound(this->jumpSound);

				// Prevent cancelation of the jump if the player is under 0 in Y
				if(this->coor[1] <= 0)
					this->coor[1] = 0.01f;
			}
		}

		// Prevent angleXZ modification during a jump
		if(this->movementStatut != PLAYER_MOVEMENT_JUMP)
			this->lastAngleXZ = angleXZ;

		// If we are in mode PLAYER_MOVEMENT_CROUCH_TO_STAND, we increase playerGroundY
		if(this->movementStatut == PLAYER_MOVEMENT_CROUCH_TO_STAND)
			this->playerGroundY += 3.0f * time / 1000.0f;
		else
			// If we are in mode PLAYER_MOVEMENT_STAND_TO_CROUCH, we decrease playerGroundY
			if(this->movementStatut == PLAYER_MOVEMENT_STAND_TO_CROUCH)
				this->playerGroundY -= 3.0f * time / 1000.0f;
			else
				// If we are in mode PLAYER_MOVEMENT_JUMP, we increase imp
				if(this->movementStatut == PLAYER_MOVEMENT_JUMP)
					this->playerSpeed = PLAYERSPEED * 1.3f;

		// If we are at playerGroundY 0 (stand mode height), we change to that mode and set the speed
		if(this->playerGroundY >= 0 && this->movementStatut == PLAYER_MOVEMENT_CROUCH_TO_STAND)
		{
			this->playerGroundY = 0;
			this->movementStatut = PLAYER_MOVEMENT_STAND;
			this->playerSpeed = PLAYERSPEED;
		}
		else
		{
			// If we are at playerGroundY -1 (crouch mode height), we change to that mode
			if(this->playerGroundY <= -1.0 && this->movementStatut == PLAYER_MOVEMENT_STAND_TO_CROUCH)
			{
				this->playerGroundY = -1.0;
				this->movementStatut = PLAYER_MOVEMENT_CROUCH;
			}
		}

		// Strafes and movements keys
		if((keyM->isKeyPressed(KEYBOARD_GAME_STRAFE_RIGHT_KEY) && !keyM->isKeyPressed(KEYBOARD_GAME_UP_KEY) && keyM->isKeyPressed(KEYBOARD_GAME_DOWN_KEY)) || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_RIGHT_DOWN))
		{
			// Standard movement when the player isn't in jump mod or if the player is in jump mod and movement correspond to the jump direction
			if(this->movementStatut != PLAYER_MOVEMENT_JUMP || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_RIGHT_DOWN))
			{
				this->coor[0] += sin( ( this->lastAngleXZ - 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + sin( ( this->lastAngleXZ - 180 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;
				this->view[0] += sin( ( this->lastAngleXZ - 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + sin( ( this->lastAngleXZ - 180 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;

				this->coor[2] += cos( ( this->lastAngleXZ - 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + cos( ( this->lastAngleXZ - 180 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;
				this->view[2] += cos( ( this->lastAngleXZ - 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + cos( ( this->lastAngleXZ - 180 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;
			}

			// Used to know the last movement type
			if(this->movementStatut != PLAYER_MOVEMENT_JUMP)
				this->movementOrientation = PLAYER_ORIENTATION_RIGHT_DOWN;

			// Jump variation
			if(this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation != PLAYER_ORIENTATION_RIGHT_DOWN)
			{
				this->jumpVariation[0] = sin( ( this->angleXZ - 90 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f + sin( ( this->angleXZ - 180 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f;
				this->jumpVariation[1] = cos( ( this->angleXZ - 90 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f + cos( ( this->angleXZ - 180 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f;
			}

			this->moved = true;
		}

		if((keyM->isKeyPressed(KEYBOARD_GAME_STRAFE_LEFT_KEY) && !keyM->isKeyPressed(KEYBOARD_GAME_UP_KEY) && keyM->isKeyPressed(KEYBOARD_GAME_DOWN_KEY)) || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_LEFT_DOWN))
		{
			// Standard movement when the player isn't in jump mod or if the player is in jump mod and movement correspond to the jump direction
			if(this->movementStatut != PLAYER_MOVEMENT_JUMP || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_LEFT_DOWN))
			{
				this->coor[0] += sin( ( this->lastAngleXZ + 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + sin( ( this->lastAngleXZ - 180 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;
				this->view[0] += sin( ( this->lastAngleXZ + 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + sin( ( this->lastAngleXZ - 180 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;

				this->coor[2] += cos( ( this->lastAngleXZ + 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + cos( ( this->lastAngleXZ - 180 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;
				this->view[2] += cos( ( this->lastAngleXZ + 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + cos( ( this->lastAngleXZ - 180 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;
			}

			// Used to know the last movement type
			if(this->movementStatut != PLAYER_MOVEMENT_JUMP)
				this->movementOrientation = PLAYER_ORIENTATION_LEFT_DOWN;

			// Jump variation
			if(this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation != PLAYER_ORIENTATION_LEFT_DOWN)
			{
				this->jumpVariation[0] = sin( ( this->angleXZ + 90 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f + sin( ( this->angleXZ - 180 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f;
				this->jumpVariation[1] = cos( ( this->angleXZ + 90 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f + cos( ( this->angleXZ - 180 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f;
			}

			this->moved = true;
		}

		if((keyM->isKeyPressed(KEYBOARD_GAME_STRAFE_RIGHT_KEY) && keyM->isKeyPressed(KEYBOARD_GAME_UP_KEY) && !keyM->isKeyPressed(KEYBOARD_GAME_DOWN_KEY)) || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_RIGHT_UP))
		{
			// Standard movement when the player isn't in jump mod or if the player is in jump mod and movement correspond to the jump direction
			if(this->movementStatut != PLAYER_MOVEMENT_JUMP || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_RIGHT_UP))
			{
				this->coor[0] += sin( ( this->lastAngleXZ - 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + sin( this->lastAngleXZ / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;
				this->view[0] += sin( ( this->lastAngleXZ - 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + sin( this->lastAngleXZ / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;

				this->coor[2] += cos( ( this->lastAngleXZ - 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + cos( this->lastAngleXZ / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;
				this->view[2] += cos( ( this->lastAngleXZ - 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + cos( this->lastAngleXZ / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;
			}

			// Used to know the last movement type
			if(this->movementStatut != PLAYER_MOVEMENT_JUMP)
				this->movementOrientation = PLAYER_ORIENTATION_RIGHT_UP;

			// Jump variation
			if(this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation != PLAYER_ORIENTATION_RIGHT_UP)
			{
				this->jumpVariation[0] = sin( ( this->angleXZ - 90 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f + sin( this->angleXZ / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f;
				this->jumpVariation[1] = cos( ( this->angleXZ - 90 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f + cos( this->angleXZ / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f;
			}

			this->moved = true;
		}

		if((keyM->isKeyPressed(KEYBOARD_GAME_STRAFE_LEFT_KEY) && keyM->isKeyPressed(KEYBOARD_GAME_UP_KEY) && !keyM->isKeyPressed(KEYBOARD_GAME_DOWN_KEY)) || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_LEFT_UP))
		{
			// Standard movement when the player isn't in jump mod or if the player is in jump mod and movement correspond to the jump direction
			if(this->movementStatut != PLAYER_MOVEMENT_JUMP || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_LEFT_UP))
			{
				this->coor[0] += sin( ( this->lastAngleXZ + 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + sin( this->lastAngleXZ / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;
				this->view[0] += sin( ( this->lastAngleXZ + 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + sin( this->lastAngleXZ / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;

				this->coor[2] += cos( ( this->lastAngleXZ + 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + cos( this->lastAngleXZ / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;
				this->view[2] += cos( ( this->lastAngleXZ + 90 ) / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f + cos( this->lastAngleXZ / 180 * PI  ) * this->playerSpeed / 1.5f * time / 1000.0f;
			}

			// Used to know the last movement type
			if(this->movementStatut != PLAYER_MOVEMENT_JUMP)
				this->movementOrientation = PLAYER_ORIENTATION_LEFT_UP;

			// Jump variation
			if(this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation != PLAYER_ORIENTATION_LEFT_UP)
			{
				this->jumpVariation[0] = sin( ( this->angleXZ + 90 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f + sin( this->angleXZ / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f;
				this->jumpVariation[1] = cos( ( this->angleXZ + 90 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f + cos( this->angleXZ / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED / 1.5f * time / 1000.0f;
			}

			this->moved = true;
		}

		if(!moved || this->movementStatut == PLAYER_MOVEMENT_JUMP && (this->movementOrientation == PLAYER_ORIENTATION_UP_ONLY || this->movementOrientation == PLAYER_ORIENTATION_DOWN_ONLY || this->movementOrientation == PLAYER_ORIENTATION_LEFT_ONLY || this->movementOrientation == PLAYER_ORIENTATION_RIGHT_ONLY))
		{
			if((keyM->isKeyPressed(KEYBOARD_GAME_UP_KEY) && !keyM->isKeyPressed(KEYBOARD_GAME_STRAFE_LEFT_KEY) && !keyM->isKeyPressed(KEYBOARD_GAME_STRAFE_RIGHT_KEY)) || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_UP_ONLY))
			{
				// Standard movement when the player isn't in jump mod or if the player is in jump mod and movement correspond to the jump direction
				if(this->movementStatut != PLAYER_MOVEMENT_JUMP || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_UP_ONLY))
				{
					this->coor[2] += cos( this->lastAngleXZ / 180 * PI  ) * this->playerSpeed * time / 1000.0f;
					this->view[2] += cos( this->lastAngleXZ / 180 * PI  ) * this->playerSpeed * time / 1000.0f;

					this->coor[0] += sin( this->lastAngleXZ / 180 * PI  ) * this->playerSpeed * time / 1000.0f;
					this->view[0] += sin( this->lastAngleXZ / 180 * PI  ) * this->playerSpeed * time / 1000.0f;
				}

				// Used to know the last movement type
				if(this->movementStatut != PLAYER_MOVEMENT_JUMP)
					this->movementOrientation = PLAYER_ORIENTATION_UP_ONLY;

				// Jump variation
				if(this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation != PLAYER_ORIENTATION_UP_ONLY)
				{
					this->jumpVariation[0] = sin( this->angleXZ / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED * time / 1000.0f;
					this->jumpVariation[1] = cos( this->angleXZ / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED * time / 1000.0f;
				}

				this->moved = true;
			}

			if((keyM->isKeyPressed(KEYBOARD_GAME_DOWN_KEY) && !keyM->isKeyPressed(KEYBOARD_GAME_STRAFE_LEFT_KEY) && !keyM->isKeyPressed(KEYBOARD_GAME_STRAFE_RIGHT_KEY)) || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_DOWN_ONLY))
			{
				// Standard movement when the player isn't in jump mod or if the player is in jump mod and movement correspond to the jump direction
				if(this->movementStatut != PLAYER_MOVEMENT_JUMP || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_DOWN_ONLY))
				{
					this->coor[2] += cos( ( this->lastAngleXZ - 180 ) / 180 * PI  ) * this->playerSpeed * time / 1000.0f;
					this->view[2] += cos( ( this->lastAngleXZ - 180 ) / 180 * PI  ) * this->playerSpeed * time / 1000.0f;

					this->coor[0] += sin( ( this->lastAngleXZ - 180 ) / 180 * PI  ) * this->playerSpeed * time / 1000.0f;
					this->view[0] += sin( ( this->lastAngleXZ - 180 ) / 180 * PI  ) * this->playerSpeed * time / 1000.0f;
				}

				// Used to know the last movement type
				if(this->movementStatut != PLAYER_MOVEMENT_JUMP)
					this->movementOrientation = PLAYER_ORIENTATION_DOWN_ONLY;

				// Jump variation
				if(this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation != PLAYER_ORIENTATION_DOWN_ONLY)
				{
					this->jumpVariation[0] = sin( ( this->angleXZ - 180 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED * time / 1000.0f;
					this->jumpVariation[1] = cos( ( this->angleXZ - 180 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED * time / 1000.0f;
				}

				this->moved = true;
			}

			if((keyM->isKeyPressed(KEYBOARD_GAME_STRAFE_RIGHT_KEY) && !keyM->isKeyPressed(KEYBOARD_GAME_UP_KEY) && !keyM->isKeyPressed(KEYBOARD_GAME_DOWN_KEY)) || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_RIGHT_ONLY))
			{
				// Standard movement when the player isn't in jump mod or if the player is in jump mod and movement correspond to the jump direction
				if(this->movementStatut != PLAYER_MOVEMENT_JUMP || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_RIGHT_ONLY))
				{
					this->coor[0] += sin( ( this->lastAngleXZ - 90 ) / 180 * PI  ) * this->playerSpeed * time / 1000.0f;
					this->view[0] += sin( ( this->lastAngleXZ - 90 ) / 180 * PI  ) * this->playerSpeed * time / 1000.0f;

					this->coor[2] += cos( ( this->lastAngleXZ - 90 ) / 180 * PI  ) * this->playerSpeed * time / 1000.0f;
					this->view[2] += cos( ( this->lastAngleXZ - 90 ) / 180 * PI  ) * this->playerSpeed * time / 1000.0f;
				}

				// Used to know the last movement type
				if(this->movementStatut != PLAYER_MOVEMENT_JUMP)
					this->movementOrientation = PLAYER_ORIENTATION_RIGHT_ONLY;

				// Jump variation
				if(this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation != PLAYER_ORIENTATION_RIGHT_ONLY)
				{
					this->jumpVariation[0] = sin( ( this->angleXZ - 90 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED * time / 1000.0f;
					this->jumpVariation[1] = cos( ( this->angleXZ - 90 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED * time / 1000.0f;
				}
				
				this->moved = true;
			}

			if((keyM->isKeyPressed(KEYBOARD_GAME_STRAFE_LEFT_KEY) && !keyM->isKeyPressed(KEYBOARD_GAME_UP_KEY) && !keyM->isKeyPressed(KEYBOARD_GAME_DOWN_KEY)) || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_LEFT_ONLY))
			{
				// Standard movement when the player isn't in jump mod or if the player is in jump mod and movement correspond to the jump direction
				if(this->movementStatut != PLAYER_MOVEMENT_JUMP || (this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation == PLAYER_ORIENTATION_LEFT_ONLY))
				{
					this->coor[0] += sin( ( this->lastAngleXZ + 90 ) / 180 * PI  ) * this->playerSpeed * time / 1000.0f;
					this->view[0] += sin( ( this->lastAngleXZ + 90 ) / 180 * PI  ) * this->playerSpeed * time / 1000.0f;

					this->coor[2] += cos( ( this->lastAngleXZ + 90 ) / 180 * PI  ) * this->playerSpeed * time / 1000.0f;
					this->view[2] += cos( ( this->lastAngleXZ + 90 ) / 180 * PI  ) * this->playerSpeed * time / 1000.0f;
				}

				// Used to know the last movement type
				if(this->movementStatut != PLAYER_MOVEMENT_JUMP)
					this->movementOrientation = PLAYER_ORIENTATION_LEFT_ONLY;

				// Jump variation
				if(this->movementStatut == PLAYER_MOVEMENT_JUMP && this->movementOrientation != PLAYER_ORIENTATION_LEFT_ONLY)
				{
					this->jumpVariation[0] = sin( ( this->angleXZ + 90 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED * time / 1000.0f;
					this->jumpVariation[1] = cos( ( this->angleXZ + 90 ) / 180 * PI  ) * PLAYERJUMPVARIATIONSPEED * time / 1000.0f;
				}

				this->moved = true;
			}
		}
			
			

		// If the player hasn't moved during this frame, we change the movementOrientation value to prevent an error if the player decide to jump in the next one (he would jump in the last direction instead of jumping on the spot
		if(!this->moved)
			this->movementOrientation = PLAYER_ORIENTATION_NONE;

		// Apply jump variation
		if(this->movementStatut == PLAYER_MOVEMENT_JUMP)
		{
			this->coor[0] += this->jumpVariation[0];
			this->coor[2] += this->jumpVariation[1];
		}

		// Left / Right movements keys
		if(keyM->isKeyPressed(KEYBOARD_GAME_LEFT_KEY))
		{
				this->angleXZ += PLAYERLRROTATION * 10 * time / 1000.0f;
				this->moved = true;
		}
		else
		{
			if(keyM->isKeyPressed(KEYBOARD_GAME_RIGHT_KEY))
			{
					this->angleXZ -= PLAYERLRROTATION * 10 * time / 1000.0f;
					this->moved = true;
			}
		}

		// Look keys
		if(keyM->isKeyPressed(KEYBOARD_GAME_LOOK_UP_KEY))
		{
			this->angleY += PLAYERLRROTATION * 10 * time / 1000.0f;
		}
		else
		{
			if(keyM->isKeyPressed(KEYBOARD_GAME_LOOK_DOWN_KEY))
			{
				this->angleY -= PLAYERLRROTATION * 10 * time / 1000.0f;
			}
		}

		// Center key
		if(keyM->isKeyPressed(KEYBOARD_GAME_CENTER_VIEW_KEY))
		{
			this->angleY = 45;
		}

		// Impulsion
		if( (keyM->isKeyPressed(KEYBOARD_GAME_STRAFE_LEFT_KEY) || keyM->isKeyPressed(KEYBOARD_GAME_STRAFE_RIGHT_KEY) || keyM->isKeyPressed(KEYBOARD_GAME_UP_KEY) || keyM->isKeyPressed(KEYBOARD_GAME_DOWN_KEY)))
		{
			if(this->coor[1] <= this->playerGroundY)
			{
				this->coor[1] = this->playerGroundY;
				this->imp = PLAYERIMPSPEED;

				// No sound in standing up mode
				if(this->movementStatut != PLAYER_MOVEMENT_CROUCH_TO_STAND)
				{
					if(this->stepCounter %2 == 0)
					{
						if((int)((double)rand() / ((double)RAND_MAX + 1) * 2) == 0)
							soundM->playSound(this->steps[currentEnv][0]);
						else
							soundM->playSound(this->steps[currentEnv][1]);

					}
					else
					{
						if((int)((double)rand() / ((double)RAND_MAX + 1) * 2) == 0)
							soundM->playSound(this->steps[currentEnv][2]);
						else
							soundM->playSound(this->steps[currentEnv][3]);
					}

					this->stepCounter++;
				}
			}
		}

		// FlashLight
		if(keyM->isKeyPressed(KEYBOARD_GAME_LIGHT_KEY))
		{
			if(this->flashlightAvailible)
			{
				if(light)
				{
					glDisable(GL_LIGHT0);
					light = false;
				}
				else
				{
					light = true;
					glEnable(GL_LIGHT0);
				}
				this->soundM->playSound(this->flashLightSound);
			}
			this->keyM->setKey(KEYBOARD_GAME_LIGHT_KEY,0);
		}

		// Weapon Selection
		if((this->keyM->isKeyPressed(KEYBOARD_GAME_PREVIOUS_WEAPON_KEY) || this->mouseM->isMouseButtonPressed(MOUSE_WHEEL_DOWN_BUTTON)))
		{
			if(this->weaponM->getHasWeapon() && this->weaponM->getMode() == WEAPON_MODE_NORMAL)
				this->weaponM->selectPreviousWeapon();
			this->keyM->setKey(KEYBOARD_GAME_PREVIOUS_WEAPON_KEY,0);
			this->mouseM->setMouseButton(MOUSE_WHEEL_DOWN_BUTTON,0);
		}
		else
		{
			if(this->keyM->isKeyPressed(KEYBOARD_GAME_NEXT_WEAPON_KEY) || this->mouseM->isMouseButtonPressed(MOUSE_WHEEL_UP_BUTTON))
			{
				if(this->weaponM->getHasWeapon() && this->weaponM->getMode() == WEAPON_MODE_NORMAL)
					this->weaponM->selectNextWeapon();
				this->keyM->setKey(KEYBOARD_GAME_NEXT_WEAPON_KEY,0);
				this->mouseM->setMouseButton(MOUSE_WHEEL_UP_BUTTON,0);
			}
		}

		// Weapon 0 key
		if(this->keyM->isKeyPressed(KEYBOARD_GAME_WEAPON0_KEY))
		{
			if(this->weaponM->getHasWeapon() && this->weaponM->getMode() == WEAPON_MODE_NORMAL)
				this->weaponM->selectWeapon(0);
			this->keyM->setKey(KEYBOARD_GAME_WEAPON0_KEY,0);
		}

		// Weapon 1 key
		if(this->keyM->isKeyPressed(KEYBOARD_GAME_WEAPON1_KEY))
		{
			if(this->weaponM->getHasWeapon() && this->weaponM->getMode() == WEAPON_MODE_NORMAL)
				this->weaponM->selectWeapon(1);
			this->keyM->setKey(KEYBOARD_GAME_WEAPON1_KEY,0);
		}

		// Weapon Light
		this->lightWeaponDir[0] = (float)(this->view[0] - this->coor[0]);
		this->lightWeaponDir[1] = (float)(this->view[1] - this->coor[1]);
		this->lightWeaponDir[2] = (float)(this->view[2] - this->coor[2]);

		this->lightWeaponPos[0] = (float)this->coor[0];
		this->lightWeaponPos[1] = (float)this->coor[1] + 3.0f;
		this->lightWeaponPos[2] = (float)this->coor[2];

		glLightfv(GL_LIGHT1, GL_POSITION, this->lightWeaponPos);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, this->lightWeaponDir);

		// Reload
		if(this->keyM->isKeyPressed(KEYBOARD_GAME_RELOAD_KEY))
		{
			if(this->weaponM->getHasWeapon() && this->weaponM->getMode() == WEAPON_MODE_NORMAL && this->weaponM->getSelectedWeapon()->getLoaderAmount() < this->weaponM->getSelectedWeapon()->getLoaderCapacity() && this->weaponM->getSelectedWeapon()->getAmmo() > 0)
				this->weaponM->reloadWeapon();
			this->keyM->setKey(KEYBOARD_GAME_RELOAD_KEY,0);
		}

		// Fire
		if(this->keyM->isKeyPressed(KEYBOARD_GAME_FIRE_KEY) || this->mouseM->isMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			if(this->weaponM->getHasWeapon() && this->weaponM->getMode() == WEAPON_MODE_NORMAL && this->weaponM->getSelectedWeapon()->getLoaderAmount() > 0)
			{
				this->weaponM->fireWeapon();
				this->fired = true;
			}
			else
			{
				if(this->weaponM->getHasWeapon() && this->weaponM->getMode() == WEAPON_MODE_NORMAL && this->weaponM->getSelectedWeapon()->getLoaderAmount() == 0 && this->weaponM->getSelectedWeapon()->getAmmo() > 0)
					this->weaponM->reloadWeapon();
			}
		}

		this->coor[1] += this->imp * time / 1000.0f;

		// Replace the Player on the Ground if he has not moved
		if(!(keyM->isKeyPressed(KEYBOARD_GAME_STRAFE_LEFT_KEY) || keyM->isKeyPressed(KEYBOARD_GAME_STRAFE_RIGHT_KEY) || keyM->isKeyPressed(KEYBOARD_GAME_UP_KEY) || keyM->isKeyPressed(KEYBOARD_GAME_DOWN_KEY)) && this->coor[1] <= this->playerGroundY && this->movementStatut != PLAYER_MOVEMENT_JUMP)
			this->coor[1] = this->playerGroundY;

		// Reception of the jump
		if(this->coor[1] <= this->playerGroundY && this->movementStatut == PLAYER_MOVEMENT_JUMP)
		{
			this->movementStatut = PLAYER_MOVEMENT_STAND;
			this->coor[1] = this->playerGroundY;
			this->playerSpeed = PLAYERSPEED;
			soundM->playSound(this->steps[currentEnv][rand()%4]);
			this->jumpTimer = PLAYERJUMPDELAY;
			this->jumpVariation[0] = 0.0f;
			this->jumpVariation[1] = 0.0f;
		}

		// The player bleed if he has less than PLAYERBLEEDVALUE% health
		if(this->health < PLAYERBLEEDVALUE)
		{
			this->bleedTimer += time;
			if(this->bleedTimer >= PLAYERBLEEDTIMER)
			{
				this->bleedTimer = 0;

				for(int i = 0 ; i < 6 ; i++)
					this->impactM->addImpact(this->bloodId,(float)(Utility::randomRange(5,20) / 100.0f),Vector3d(this->coor[0] + RANDOM_FLOAT / 2.0f, 0.001f, this->coor[2] + RANDOM_FLOAT / 2.0f), Vector3d(90.0f,0.0f,0.0f),2,IMPACT_REPLACEMENT_POLICY_CREATE);
			}
		}
	}
	else
	{
		// Death animation (falling)
		if(this->coor[1] > this->playerGroundY)
		{
			this->coor[1] += this->imp * time / 1000.0f;
			if(this->coor[1] < this->playerGroundY)
				this->coor[1] = this->playerGroundY;
		}
		else
		{
			// Draw death message
			if(this->deathTimer < PLAYERDEATHTIMER / 2 && this->deathTimer + time >= PLAYERDEATHTIMER / 2)
				this->messageM->addMessage(TEXT_INFORMATION_DEAD,TEXTMANAGER_ALIGN_LEFT,1.0f,1.0f,1.0f);

			if(this->deathTimer < PLAYERDEATHTIMER)
				this->deathTimer += time;
			else
			{
				// Respawn if the player type the fire buttons
				if(this->keyM->isKeyPressed(KEYBOARD_GAME_FIRE_KEY) || this->mouseM->isMouseButtonPressed(MOUSE_LEFT_BUTTON))
				{
					this->keyM->initializeKeys();
					respawn();
				}
			}
		}
	}

	// Update view
	this->angleXZ += mouseM->getNewViewX() * 10.0f * time / 1000.0f;
	this->angleY  += mouseM->getNewViewY() * 10.0f * time / 1000.0f;

	if( angleXZ >= 360.0f )
		angleXZ -= 360.0f;

	if( angleXZ < 0 )
		angleXZ += 360.0f;

	if( angleY > 135.0f )
		angleY = 135.0f;

	if( angleY < 0 )
		angleY = 0;

	view[0] = coor[0] + sin( angleXZ / 180.0f * PI ) * 200.0f;
	view[2] = coor[2] + cos( angleXZ / 180.0f * PI ) * 200.0f;
	view[1] = coor[1] + sin( ( angleY - 45.0f ) / 180.0f * PI ) * 200.0f;

	// Update the view sector
	this->viewSector[0][0] = this->coor[0];
	this->viewSector[0][1] = this->coor[2];
	this->viewSector[1][0] = this->coor[0] + cos((-this->angleXZ - (this->viewAngle + abs(this->angleY - 45) / 4.0f) + 90.0f) * PI / 180.0f) * (this->viewDistance + abs(this->angleY - 45) / 2.0f);
	this->viewSector[1][1] = this->coor[2] + sin((-this->angleXZ - (this->viewAngle + abs(this->angleY - 45) / 4.0f) + 90.0f) * PI / 180.0f) * (this->viewDistance + abs(this->angleY - 45) / 2.0f);
	this->viewSector[2][0] = this->coor[0] + cos((-this->angleXZ + (this->viewAngle + abs(this->angleY - 45) / 4.0f) + 90.0f) * PI / 180.0f) * (this->viewDistance + abs(this->angleY - 45) / 2.0f);
	this->viewSector[2][1] = this->coor[2] + sin((-this->angleXZ + (this->viewAngle + abs(this->angleY - 45) / 4.0f) + 90.0f) * PI / 180.0f) * (this->viewDistance + abs(this->angleY - 45) / 2.0f);

	this->allowStand = true;
}

/**
* Use Health item
*/
void Player::useHealth()
{
	this->health += 25;
	if(this->health > 100)
		this->health = 100;
}

/**
* Use Armor item
*/
void Player::useArmor()
{
	this->armor += 40;
	if(this->armor > 100)
		this->armor = 100;
}

/**
* Get the flashight
*/
void Player::getFlashLight()
{
	this->flashlightAvailible = true;
}

/**
* Get a key
* @param key The key choosen
*/
void Player::getKey(int key)
{
	switch(key)
	{
		case OBJECTTYPE_KEY_YELLOW:
		this->keys[PLAYER_KEY_YELLOW] = true;
		break;

		case OBJECTTYPE_KEY_BLUE:
		this->keys[PLAYER_KEY_BLUE] = true;
		break;

		case OBJECTTYPE_KEY_GREEN:
		this->keys[PLAYER_KEY_GREEN] = true;
		break;

		case OBJECTTYPE_KEY_RED:
		this->keys[PLAYER_KEY_RED] = true;
		break;
	}
}

/**
* Respawn the Player
*/
void Player::respawn()
{
	// Player coordinates
	this->coor[0] = this->startCoordinates[0];
	this->coor[1] = this->startCoordinates[1];
	this->coor[2] = this->startCoordinates[2];
	this->previousCoor[0] = this->startCoordinates[0];
	this->previousCoor[1] = 0.0f;
	this->previousCoor[2] = this->startCoordinates[2];
	this->view[0] = this->startCoordinates[3];
	this->view[1] = this->startCoordinates[4];
	this->view[2] = this->startCoordinates[5];
	this->angleXZ = this->startAngleXZ;
	this->jumpVariation[0] = 0.0f;
	this->jumpVariation[1] = 0.0f;

	this->playerGroundY = PLAYERGROUNDY;
	this->playerSpeed = PLAYERSPEED;
	this->angleY  = 45;
	this->imp = 0;
	this->jumpTimer = PLAYERJUMPDELAY;
	this->deathTimer = 0;
	this->deathMessageTimer = 0;
	this->damageTimer = PLAYERDAMAGETIMER;
	this->bleedTimer = PLAYERBLEEDTIMER;

	this->movementStatut = PLAYER_MOVEMENT_STAND;
	this->fired = false;
	this->moved = false;
	this->dead = false;
	this->allowStand = true;

	this->health = this->startHealth;
	this->armor = this->startArmor;

	this->weaponM->setEnabled(true);
}

/**
* Change the Allow Stand State
* @param v new Allox Stand State
*/
void Player::setAllowStand(bool v)
{
	this->allowStand = v;
}

/**
* Return the i view
* @param i View type
* @return i view
*/
float Player::getView(int i)
{
	return this->view[i];
}

/**
* Return the XZ angle
* @return XZ angle
*/
float Player::getAngleXZ()
{
	return this->angleXZ;
}

/**
* Return the Y angle
* @return Y angle
*/
float Player::getAngleY()
{
	return this->angleY;
}

/**
* Return the player Y center
* @return Y center
*/
float Player::getYCenter()
{
	return this->coor[1] + PLAYERHEIGH / 2.0f;
}

/**
* Return the death timer
* @return Death timer
*/
long Player::getDeathTimer()
{
	return this->deathTimer;
}

/**
* Return the damage timer
* @return Damage timer
*/
long Player::getDamageTimer()
{
	return this->damageTimer;
}

/**
* Return if the player has fired during this frame
* @return fire state
*/
bool Player::getFired()
{
	return this->fired;
}

/**
* Return if the player has moved in this frame
* @return Moved state
*/
bool Player::getMoved()
{
	return this->moved;
}

/**
* Return if the player has a flashlight
* @return If the player has the flashlight
*/
bool Player::hasFlashlight()
{
	return this->flashlightAvailible;
}

/**
* Return if the player has a specified key
* @param key Key number
* @return If the Player possess this key
*/
bool Player::hasKey(int key)
{
	bool ret = false;
	switch(key)
	{
		case -1:
		ret = true;
		break;

		case OBJECTTYPE_KEY_YELLOW:
		ret = this->keys[PLAYER_KEY_YELLOW];
		break;

		case OBJECTTYPE_KEY_BLUE:
		ret = this->keys[PLAYER_KEY_BLUE];
		break;

		case OBJECTTYPE_KEY_GREEN:
		ret = this->keys[PLAYER_KEY_GREEN];
		break;

		case OBJECTTYPE_KEY_RED:
		ret = this->keys[PLAYER_KEY_RED];
		break;
	}
	return ret;
}

/**
* Return if the player is dead
* @return Return if the player is dead
*/
bool Player::isDead()
{
	return this->dead;
}

/**
* Damage the Player
* @param v Damage value
*/
bool Player::takeDamage(int v)
{
	bool ret = false;
	if(this->damageTimer >= PLAYERDAMAGETIMER)
	{
		if(this->health > 0)
		{
			this->soundM->playSound(this->hurtSound);

			// Blood impact depending on the damage taken
			for(int i = 0 ; i < v ; i++)
				this->impactM->addImpact(this->bloodId,(float)(Utility::randomRange(5,20) / 100.0f),Vector3d(this->coor[0] + RANDOM_FLOAT / 2.0f, MINIMUMIMPACTHEIGHT, this->coor[2] + RANDOM_FLOAT / 2.0f), Vector3d(90.0f,0.0f,0.0f),IMPACT_REPLACEMENT_Y,IMPACT_REPLACEMENT_POLICY_CREATE);

			// Damage calculation
			if(this->armor > 0)
			{
				this->armor -= v/2;
				this->health -= v/2;
			}
			else
				this->health -= v;

			// Death
			if(this->health <= 0)
			{
				this->health = 0;
				this->playerGroundY = -2.0f;
				this->imp = 0;
				this->movementStatut = PLAYER_MOVEMENT_STAND;
				this->weaponM->setEnabled(false);
				glDisable(GL_LIGHT1);
			}

			// Put the armor to 0 if the damage is superior to the armor value
			if(this->armor < 0)
				this->armor = 0;

			this->damageTimer = 0;

			ret = true;
		}
	}
	return ret;
}

/**
* Detect a collision between a 3D point a the Player
* @param x Object's X coordinate
* @param y Object's Y coordinate
* @param z Object's Z coordinate
* @return If a collision is detected
*/
bool Player::isCollision(float x, float z, float y)
{
	return (y >= this->coor[1] && y <= (this->coor[1] + PLAYERHEIGH) && x >= this->coor[0] - PLAYERCOLLISIONRADIUS && x <= this->coor[0] + PLAYERCOLLISIONRADIUS && z >= this->coor[2] - PLAYERCOLLISIONRADIUS && z <= this->coor[2] + PLAYERCOLLISIONRADIUS);
}

/**
* Return the X coordinate of the object where the player is
* @param x X modification
* @param decal Decal
* @return The player X coordinate
*/
int Player::getCurrentObjectCoordinateX(float x, float decal)
{
	return (int) ( - this->coor[0] + x + decal ) / 2;
}

/**
* Return the Z coordinate of the object where the player is
* @param z Z modification
* @param decal Decal
* @return The player Z coordinate
*/
int Player::getCurrentObjectCoordinateZ(float z, float decal)
{
	return (int)(this->coor[2] + z + decal) / 2;
}

/**
* Return the X coordinate of the object where the player was during the previous frame
* @param decal Decal
* @return The player X previous coordinate
*/
int Player::getPreviousObjectCoordinateX(float decal)
{
	return (int) (- this->previousCoor[0] + decal) / 2;
}

/**
* Return the Z coordinate of the object where the player was during the previous frame
* @param decal Decal
* @return The player Z previous coordinate
*/
int Player::getPreviousObjectCoordinateZ( float decal)
{
	return (int) ( this->previousCoor[2] + decal ) / 2;
}

/**
* Return the Health value
* @return Health
*/
int Player::getHealth()
{
	return this->health;
}

/**
* Return the Armor value
* @return Armor
*/
int Player::getArmor()
{
	return this->armor;
}

/**
* Return the movement statut
* @return The movement statut
*/
int Player::getMovementStatut()
{
	return this->movementStatut;
}

/**
* Return the View Sector
* @return The View Sector
*/
Vector2d * Player::getViewSector()
{
	return this->viewSector;
}

/**
* Return the Player coordinates
* @return Coordinate
*/
Vector3d Player::getCoor()
{
	return this->coor;
}

/**
* Return the Previous Player coordinates
* @return Previous coordinates
*/
Vector3d Player::getPreviousCoor()
{
	return this->previousCoor;
}
