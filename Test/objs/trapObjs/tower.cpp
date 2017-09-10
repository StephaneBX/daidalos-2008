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
#include "../../includes/global_var.h"
#include "../../includes/utility.h"
#include "../../manager/optionManager.h"
#include "../../objs/trapObjs/tower.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param player1 Pointer to the Player
* @param ambientLight Ambient light value
*/
Tower::Tower(int x, int z, Player * player1, int ambientLight) : Obj(x,z,player1)
{
	this->matType = OBJECTMAT_ROCK;
	this->arrowDelay = 0;
	this->projectileM = ProjectileManager::getInstance();

	std::ostringstream stringstream;
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/projectiles/arrow_impact.tga";
	this->impactId = CTextureManager::getInstance()->LoadTexture(stringstream.str());

	stringstream.str("");
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/traps/tower/tower.tga";
	this->lowModelEntity = ModelManager::getInstance()->loadModel("objs/traps/tower/tower_low.md2", stringstream.str());
	this->lowModelEntity->SetScale(1.2f);

	stringstream.str("");
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/traps/tower/tower.tga";
	this->highModelEntity = ModelManager::getInstance()->loadModel("objs/traps/tower/tower_high.md2", stringstream.str());
	this->highModelEntity->SetScale(1.2f);

	this->matSpec[0] = 0.1f;
	this->matSpec[1] = 0.1f;
	this->matSpec[2] = 0.1f;
	this->matSpec[3] = 1.0f;

	this->matDif[0] = 0.4f;
	this->matDif[1] = 0.4f;
	this->matDif[2] = 0.4f;
	this->matDif[3] = 1.0f;

	this->matAmb[0] = 0.4f * ambientLight / 100.0f;
	this->matAmb[1] = 0.4f * ambientLight / 100.0f;
	this->matAmb[2] = 0.4f * ambientLight / 100.0f;
	this->matAmb[3] = 1.0f;

	this->sound = this->soundM->loadSound("sounds/environment/rock_impact.wav");
}

/**
* Destructor
*/
Tower::~Tower(){}

/**
* Refresh the Tower
* @param time Elapsed time
*/
void Tower::refresh(long time)
{
	float dist = sqrt((this->x - player1->getCoor()[0]) * (this->x - player1->getCoor()[0]) + (this->z - player1->getCoor()[2]) * (this->z - player1->getCoor()[2]));
	this->arrowDelay += time;
	if(this->arrowDelay >= TOWERARROWDELAY && dist <= TRAPSPROJECTILEACTIVATIONRADIUS)
	{
		this->projectileM->addProjectile(Vector3d(this->x - 1.0f, 2.4f, this->z + 2.01f),PROJECTILE_TYPE_ARROW, PROJECTILE_SOURCE_ENEMY, this->impactId, IMPACT_REPLACEMENT_POLICY_REPLACE, 0.12f, Utility::randomRange(30,50) / 20.0f, 100.0f, 0.0f, 90.0f, 0.0f, 15);
		this->projectileM->addProjectile(Vector3d(this->x + 0.01f, 2.4f, this->z + 1.0f),PROJECTILE_TYPE_ARROW, PROJECTILE_SOURCE_ENEMY, this->impactId, IMPACT_REPLACEMENT_POLICY_REPLACE, 0.12f, Utility::randomRange(30,50) / 20.0f, 100.0f, 90.0f, 90.0f, 0.0f, 15);
		this->projectileM->addProjectile(Vector3d(this->x - 1.0f, 2.4f, this->z - 0.01f),PROJECTILE_TYPE_ARROW, PROJECTILE_SOURCE_ENEMY, this->impactId, IMPACT_REPLACEMENT_POLICY_REPLACE, 0.12f, Utility::randomRange(30,50) / 20.0f, 100.0f, 180.0f, 90.0f, 0.0f, 15);
		this->projectileM->addProjectile(Vector3d(this->x - 2.01f, 2.4f, this->z + 1.0f),PROJECTILE_TYPE_ARROW, PROJECTILE_SOURCE_ENEMY, this->impactId, IMPACT_REPLACEMENT_POLICY_REPLACE, 0.12f, Utility::randomRange(30,50) / 20.0f, 100.0f, 270.0f, 90.0f, 0.0f, 15);
		this->arrowDelay = 0;
	}
}

/**
* Draw the Tower
* @param lightPos Light position
*/
void Tower::draw( float * lightPos)
{
	vec3_t light;

	light[0] = lightPos[0];
	light[1] = lightPos[1];
	light[2] = lightPos[2];

	float dist = sqrt((this->x - player1->getCoor()[0]) * (this->x - player1->getCoor()[0]) + (this->z - player1->getCoor()[2]) * (this->z - player1->getCoor()[2]));

	glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
	glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
	glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
	glMaterialf (GL_FRONT,GL_SHININESS,50.0f);

	glPushMatrix();
		glTranslatef(this->x - 1, 0.0f ,this->z + 1);
		if(dist > VIEWQUALITYMIN1)
			this->lowModelEntity->DrawEntity( 1, MD2E_DRAWNORMAL | MD2E_TEXTURED, light );
		else
			this->highModelEntity->DrawEntity( 1, MD2E_DRAWNORMAL | MD2E_TEXTURED, light );
	glPopMatrix();
}

/**
* Return the object type
* @return Object type
*/
int Tower::getType()
{
	return OBJECTTYPE_IMMOVABLE_TRAP;
}

/**
* Detect collisions
* @param x X coordinate to test
* @param z Z coordinate to test
* @param y Y coordinate to test
* @return Result to the collsion test
*/
bool Tower::isCollision(float x, float z, float y)
{
	return y <= 2.6f;
}

/**
* Return if the object has to replace the player
* @return If need replacement
*/
bool Tower::needReplacement()
{
	return true;
}
