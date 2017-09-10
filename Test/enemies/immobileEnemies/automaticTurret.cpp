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
#include "../../enemies/immobileEnemies/automaticTurret.h"

#define TURRET_VALID_ANGLE_INTERVAL 1.0f
#define TURRET_WAIT_TIME 1800
#define AUTOMATIC_TURRET_LOWER_Y_FIRE 1.97f
#define AUTOMATIC_TURRET_LOWER_Y_POSITION 1.25f

/**
* Constructor
* @param coodinate coordinate
* @param playerPt Pointer to the player
* @param enemyId Unique id for the enemy
*/
AutomaticTurret::AutomaticTurret(Vector3d coordinate, Player * playerPt, int enemyId, int ambient_light) : Turret(coordinate, playerPt, enemyId, ambient_light)
{
	std::ostringstream stringstream;
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/enemies/turrets/automaticTurret/lower.tga";
	std::string texture = stringstream.str();

	this->modelEntity = ModelManager::getInstance()->loadModel("enemies/turrets/automaticTurret/lower.md2", texture);
	this->modelEntity->SetScale(0.5f);

	stringstream.str("");
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/enemies/turrets/automaticTurret/upper.tga";
	texture = stringstream.str();

	this->upperEntity = ModelManager::getInstance()->loadModel("enemies/turrets/automaticTurret/upper.md2", texture);
	this->upperEntity->SetScale(0.5f);

	this->coor[1] = BLOCDEFAULTHEIGHT - AUTOMATIC_TURRET_LOWER_Y_FIRE;
	this->accuracy = 1;
	this->actionState = IMMOBILE_ENEMY_ACTION_STATE_SEARCH;

	this->attackDelay = 250;
	this->fireRange = 40;
	this->viewRange = 60;
	this->viewAngle = 45;
	this->reactionTimeInterval = 300;
	this->rotationSpeed = 5;
	this->rotationXZ = 0;
	this->rotationY = 25;
	this->smokeDistance = 1.15f;
	this->projectileType = PROJECTILE_TYPE_INVISIBLE;
	this->projectileSpeed = 50.0f;
	this->projectileMaxDistance = 50.0f;

	this->lowerPosition[0] = coor[0];
	this->lowerPosition[1] = BLOCDEFAULTHEIGHT - AUTOMATIC_TURRET_LOWER_Y_POSITION;
	this->lowerPosition[2] = coor[2];

	this->fireSound = this->soundM->loadSound("sounds/enemies/turrets/automaticTurret_fire.wav");
	this->attackModeSound = this->soundM->loadSound("sounds/enemies/turrets/automaticTurret_attack_mode.wav");

	stringstream.str("");
	stringstream << "imgs/" << this->optM->getTexturesQuality() << "/handgun/flash.tga";
	this->weaponFlash = new Flash(stringstream.str(),0.5f);
	this->drawFlash = false;

	stringstream.str("");
	stringstream << "imgs/" << this->optM->getTexturesQuality() << "/enemies/bulletLine.tga";
	this->bulletLineId = CTextureManager::getInstance()->LoadTexture(stringstream.str());
	this->drawBulletLine = false;

	stringstream.str("");
	stringstream << "imgs/" << this->optM->getTexturesQuality() << "/handgun/impact.tga";
	this->impactId = CTextureManager::getInstance()->LoadTexture(stringstream.str());

	if(this->optM->getParticleModifier() > 0)
		this->smoke = new DynamicParticleSystem(PARTICLE_SMOKE,110 / this->optM->getParticleModifier());
	else
		this->smoke = NULL;
}
