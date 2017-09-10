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
#include "../manager/projectileManager.h"

/**
* Constructor
*/
ProjectileManager::ProjectileManager()
{
	createProjectilesList();
}

/**
* Destructor
*/
ProjectileManager::~ProjectileManager()
{
	// Free projectiles memory allocations
	freeProjectiles();

	if(this->projectileList != NULL)
	{
		delete[] this->projectileList;
		this->projectileList = NULL;
	}
}

/**
* Refresh the projectiles
* @param time Elapsed time
*/
void ProjectileManager::refresh(long time)
{
	for(int i = 0 ; i < PROJECTILESLISTSIZE ; i++)
	{
		if(this->projectileList[i] != NULL)
		{
			if(this->projectileList[i]->isAlive())
			{
				this->projectileList[i]->refresh(time);
			}
			else
			{
				delete this->projectileList[i];
				this->projectileList[i] = NULL;
			}
		}
	}
}

/**
* Draw the projectiles
* @param blocksNumber Blocks that are drawn
* @param levelWidth Width of the level
*/
void ProjectileManager::draw(std::vector<int> blocksNumber, int levelWidth)
{
	for(int i = 0 ; i < PROJECTILESLISTSIZE ; i++)
	{
		if(this->projectileList[i] != NULL)
		{
			bool projectileValid = false;
			int impactBlockNumber = levelWidth * (int)(this->projectileList[i]->getCurrentCoordinates()[2] / BLOCDEFAULTSIZE) + (int)(-this->projectileList[i]->getCurrentCoordinates()[0] / BLOCDEFAULTSIZE);

			// Draw only impacts in a bloc that has been drawn in this frame
			for(int j = 0 ; j < (int)blocksNumber.size() && !projectileValid ; j++)
				if(blocksNumber[j] == impactBlockNumber)
					projectileValid = true;

			if(projectileValid)
				this->projectileList[i]->draw();
		}
	}
}

/**
* Add a projectile
* @param startCoordinates Starting coordinates
* @param projectileType Type
* @param source Source of the Projectile
* @param impactId Id of the impact
* @param impactPolicy Replacement policy
* @param impactRadius Radius of the impact
* @param speed Speed of the Projectile
* @param maxDistance Max distance of the Projectile
* @param angleXZ XZ angle
* @param angleY Y angle
* @param precisionModifier Precision of the Projectile
* @param power Power of the Projectile
*/
void ProjectileManager::addProjectile(Vector3d startCoordinates, int projectileType, int source, int impactId, int impactPolicy, float impactRadius, float speed, float maxDistance, float angleXZ, float angleY, float precisionModifier, float power)
{
	bool found = false;
	for(int i = 0 ; i < PROJECTILESLISTSIZE && !found ; i++)
	{
		if(this->projectileList[i] == NULL)
		{
			this->projectileList[i] = new Projectile(startCoordinates, projectileType, source, impactId, impactPolicy, impactRadius, speed, maxDistance, angleXZ, angleY, precisionModifier, power);
			found = true;
		}
	}
}

/**
* Free the Projectiles
*/
void ProjectileManager::freeProjectiles()
{
	if(this->projectileList != NULL)
	{
		for(int i = 0 ; i < PROJECTILESLISTSIZE ; i++)
		{
			if(this->projectileList[i] != NULL)
			{
				delete this->projectileList[i];
				this->projectileList[i] = NULL;
			}
		}
	}
}

/**
* Allocate Projectiles memory
*/
void ProjectileManager::createProjectilesList()
{
	this->projectileList = new Projectile * [PROJECTILESLISTSIZE];

	for(int i = 0 ; i < PROJECTILESLISTSIZE ; i++)
		this->projectileList[i] = NULL;
}

/**
* Delete a Projectile
* @param v Projectile number
*/
void ProjectileManager::deleteProjectile(int v)
{
	if(v >= 0 && v < PROJECTILESLISTSIZE && this->projectileList[v] != NULL)
	{
		delete this->projectileList[v];
		this->projectileList[v] = NULL;
	}
}

/**
* Get a Projectile pointer
* @param v Projectile number
* @return The projectile or NULL if it doesn't exists
*/
Projectile * ProjectileManager::getProjectile(int v)
{
	Projectile * ret = NULL;
	if(v >= 0 && v < PROJECTILESLISTSIZE && this->projectileList[v] != NULL)
		ret = this->projectileList[v];
	return ret;
}
