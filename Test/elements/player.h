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

#ifndef __PLAYER_H
#define __PLAYER_H

//#include <ctime>

#include "../manager/keyboardManager.h"
#include "../manager/mouseManager.h"
#include "../manager/soundManager.h"
#include "../manager/weaponManager.h"
#include "../manager/impactManager.h"
#include "../manager/messageManager.h"
#include "../includes/vector2d.h"
#include "../includes/vector3d.h"

// Player data
#define PLAYERSPEED 9.0f
#define PLAYERJUMPVARIATIONSPEED 3.0f
#define PLAYERIMPSPEED 2
#define PLAYERGRAVITY 13
#define PLAYERLRROTATION 13
#define PLAYERGROUNDY 0
#define PLAYERNUMBERSTEPPERSOUNDS 4
#define PLAYERNUMBERSTEPS 3
#define PLAYERJUMPDELAY 300
#define PLAYERVIEWANGLE1 41.0f
#define PLAYERVIEWANGLE2 52.0f
#define PLAYERDEATHTIMER 4000
#define PLAYERDAMAGETIMER 800
#define PLAYERBLEEDTIMER 2000
#define PLAYERBLEEDVALUE 30
#define PLAYERCOLLISIONRADIUS 0.55f
#define PLAYERHEIGH 3.0f

// Player Movement Statut
enum
{
	PLAYER_MOVEMENT_STAND,
	PLAYER_MOVEMENT_CROUCH,
	PLAYER_MOVEMENT_CROUCH_TO_STAND,
	PLAYER_MOVEMENT_STAND_TO_CROUCH,
	PLAYER_MOVEMENT_JUMP
};

enum
{
	PLAYER_ORIENTATION_NONE,
	PLAYER_ORIENTATION_LEFT_ONLY,
	PLAYER_ORIENTATION_RIGHT_ONLY,
	PLAYER_ORIENTATION_UP_ONLY,
	PLAYER_ORIENTATION_DOWN_ONLY,
	PLAYER_ORIENTATION_LEFT_UP,
	PLAYER_ORIENTATION_LEFT_DOWN,
	PLAYER_ORIENTATION_RIGHT_UP,
	PLAYER_ORIENTATION_RIGHT_DOWN
};

enum
{
	PLAYER_KEY_BLUE,
	PLAYER_KEY_RED,
	PLAYER_KEY_GREEN,
	PLAYER_KEY_YELLOW,
	PLAYER_KEY_SIZE
};

/**
* Player Class
* Handle the Player
* @author Stephane Baudoux
*/
class Player
{
private:
	float startAngleXZ;
	float viewDistance;
	float viewAngle;
	float playerGroundY;
	float playerSpeed;
	float angleXZ;
	float angleY;
	float imp;
	float lastAngleXZ;
	
	// Weapons light variables
	float lightWeaponDif[4];
	float lightWeaponSpec[4];
	float lightWeaponAmb[4];
	float spotWeaponDir[3];
	float lightWeaponDir[3];
	float lightWeaponPos[4];

	int startHealth;
	int startArmor;

	int health;
	int armor;
	int stepCounter;
	int currentEnv;
	int movementStatut;
	int movementOrientation;

	int bloodId;

	bool allowStand;
	bool fired;
	bool moved;
	bool light;
	bool flashlightAvailible;
	bool keys[PLAYER_KEY_SIZE];
	bool dead;

	long jumpTimer;
	long damageTimer;
	long deathTimer;
	long deathMessageTimer;
	long bleedTimer;

	// Managers
	KeyboardManager * keyM;
	MouseManager * mouseM;
	WeaponManager * weaponM;
	SoundManager * soundM;
	ImpactManager * impactM;
	MessageManager * messageM;

	// Sounds
	Mix_Chunk *** steps;
	Mix_Chunk * flashLightSound;
	Mix_Chunk * jumpSound;
	Mix_Chunk * hurtSound;
	
	// Vector representing the view
	Vector2d viewSector[3];

	// Players variables
	Vector3d startCoordinates;
	Vector3d startOrientation;
	Vector3d coor;
	Vector3d view;
	Vector3d previousCoor;
	Vector3d jumpVariation;

public:
	Player(int * playerCoor, int angle, int startHealth, int startArmor, bool flashlight);
	~Player();
	void setCoor(int i, float v);
	void setPreviousCoor(int i, float v);
	void setMovementStatut(int v);
	void refresh( long time, int envNumber );
	void useHealth();
	void useArmor();
	void getFlashLight();
	void getKey(int key);
	void setFired(bool v);
	void respawn();
	void setAllowStand(bool v);
	float getView(int i);
	float getAngleXZ();
	float getAngleY();
	float getYCenter();
	long getDeathTimer();
	long getDamageTimer();
	bool getFired();
	bool getMoved();
	bool hasFlashlight();
	bool hasKey(int key);
	bool isDead();
	bool takeDamage(int v);
	bool isCollision(float x, float z, float y);
	int getCurrentObjectCoordinateX(float x, float decal = 0.0f);
	int getCurrentObjectCoordinateZ(float z, float decal);
	int getPreviousObjectCoordinateX(float decal = 0.0f);
	int getPreviousObjectCoordinateZ(float decal);
	int getHealth();
	int getArmor();
	int getMovementStatut();
	Vector2d * getViewSector();
	Vector3d getCoor();
	Vector3d getPreviousCoor();
};

#endif
