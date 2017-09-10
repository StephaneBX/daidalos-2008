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
#include "../includes/utility.h"
#include "../includes/global_var.h"
#include "../manager/optionManager.h"
#include "../elements/projectile.h"

/**
* Constructor
* @param startCoordinates Projectile starting coordinate
* @param projectileType Projectile's type
* @param source Source type of the Projectile
* @param impactId Impact id that will be used if an Impact is created
* @param impactPolicy Impact replacement policy
* @param impactRadius Impact radius
* @param speed Speed
* @param maxDistance Max distance that the Projectile can attain
* @param angleXZ The XZ angle
* @param angleY The Y angle
* @param precisionModifier Precision of the Projectile
* @param power Quantity of damage that the Projectile will do
*/
Projectile::Projectile(Vector3d startCoordinates, int projectileType, int source, int impactId, int impactPolicy, float impactRadius, float speed, float maxDistance, float angleXZ, float angleY, float precisionModifier, float power)
{
	this->startCoordinates = startCoordinates;
	this->previousCoordinates = startCoordinates;
	this->currentCoordinates = startCoordinates;
	this->type = projectileType;
	this->source = source;
	this->impactId = impactId;
	this->impactPolicy = impactPolicy;
	this->impactRadius = impactRadius;
	this->speed = speed;
	this->maxDistance = maxDistance;
	this->angleXZ = angleXZ + RANDOM_FLOAT * precisionModifier;
	this->angleY = angleY + RANDOM_FLOAT * precisionModifier;
	this->power = power;
	this->distance = 0;
	this->rotation = 0;

	this->matSpec[0] = 0.4f;
	this->matSpec[1] = 0.4f;
	this->matSpec[2] = 0.4f;
	this->matSpec[3] = 1.0f;

	this->matDif[0] = 0.4f;
	this->matDif[1] = 0.4f;
	this->matDif[2] = 0.4f;
	this->matDif[3] = 1.0f;

	this->matAmb[0] = 0.4f;
	this->matAmb[1] = 0.4f;
	this->matAmb[2] = 0.4f;
	this->matAmb[3] = 1.0f;

	std::ostringstream stringstream;

	// The different kind of Projectiles
	switch(projectileType)
	{
		case PROJECTILE_TYPE_INVISIBLE:
		case PROJECTILE_TYPE_ATTACK_TEST:
		this->modelEntity = NULL;
		break;

		case PROJECTILE_TYPE_ARROW:
		stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/projectiles/arrow.tga";
		this->modelEntity = ModelManager::getInstance()->loadModel("objs/projectiles/arrow.md2", stringstream.str());
		this->modelEntity->SetScale(0.1f);
		break;

		case PROJECTILE_TYPE_AXE:
		stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/projectiles/axe.tga";
		this->modelEntity = ModelManager::getInstance()->loadModel("objs/projectiles/axe.md2", stringstream.str());
		this->modelEntity->SetScale(0.3f);
		break;
	}
}

/**
* Destructor
*/
Projectile::~Projectile(){}

/**
* Refresh the Projectile
* @param time Elapsed time
*/
void Projectile::refresh(long time)
{
	this->previousCoordinates = this->currentCoordinates;
	this->currentCoordinates[0] += (sin(this->angleXZ / 180.0f * PI)) * (this->speed * time / 100.0f);
	this->currentCoordinates[1] -= (cos(this->angleY / 180.0f * PI)) * (this->speed * time / 100.0f);
	this->currentCoordinates[2] += (cos(this->angleXZ / 180.0f * PI)) * (this->speed * time / 100.0f);

	if(this->distance < PROJECTILE_FULL_SPEED_DISTANCE)
		this->distance += (this->speed * time * (this->distance / (float)PROJECTILE_FULL_SPEED_DISTANCE)) / 100.0f;
	else
		this->distance += this->speed * time / 100.0f;

	// The axe projectile rotate on itself
	if(this->type == PROJECTILE_TYPE_AXE)
	{
		this->rotation += PROJECTILE_ROTATION_SPEED * time / 1000.0f;
		if(this->rotation > 360.0f)
			this->rotation -= 360.0f;
	}
}

/**
* Draw the Projectile
*/
void Projectile::draw()
{
	if(this->modelEntity != NULL)
	{
		vec3_t light;

		light[0] = 0;
		light[1] = 0;
		light[2] = 0;

		glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
		glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
		glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
		glMaterialf (GL_FRONT,GL_SHININESS,100.0f);

		glPushMatrix();
			glTranslatef(currentCoordinates[0], currentCoordinates[1] , currentCoordinates[2]);
			glRotatef(this->angleXZ, 0, 1, 0);
			glRotatef(this->rotation, 1, 0, 0);
			this->modelEntity->DrawEntity( 1, MD2E_DRAWNORMAL | MD2E_TEXTURED, light );
		glPopMatrix();
	}
}

/**
* Return the power of the Projectile
* @return Power
*/
float Projectile::getPower()
{
	return this->power;
}

/**
* Return the speed of the Projectile
* @return Speed
*/
float Projectile::getSpeed()
{
	return this->speed;
}

/**
* Return the radius of the Projectile's impact
* @return Impact radius
*/
float Projectile::getImpactRadius()
{
	return this->impactRadius;
}

/**
* Return if the Projectile is alive
* @return If the Projectile is alive
*/
bool Projectile::isAlive()
{
	return this->distance < this->maxDistance;
}

/**
* Return the Impact texture id
* @return Impact id
*/
int Projectile::getImpactId()
{
	return this->impactId;
}

/**
* Return the Impact policy
* @return Impact policy
*/
int Projectile::getImpactPolicy()
{
	return this->impactPolicy;
}

/**
* Return the source of the Projectile
* If the projectile is of the type PROJECTILE_TYPE_ATTACK_TEST, the source is the enemy unique id
* @return Source type
*/
int Projectile::getSource()
{
	return this->source;
}

/**
* Return the projectile type
* @return Projectile type
*/
int Projectile::getType()
{
	return this->type;
}

/**
* Return the previous coordinates
* @return Previous coordinates
*/
Vector3d Projectile::getPreviousCoordinates()
{
	return this->previousCoordinates;
}

/**
* Return the current coordinates
* @return Current coordinates
*/
Vector3d Projectile::getCurrentCoordinates()
{
	return this->currentCoordinates;
}

/**
* Return the start coordinates
* @return Start coordinates
*/
Vector3d Projectile::getStartCoordinates()
{
	return this->startCoordinates;
}
