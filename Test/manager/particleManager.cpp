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
#include "../manager/particleManager.h"

/**
* Constructor
*/
ParticleManager::ParticleManager()
{
	for(int i = 0 ; i < MAXPARTICLESYSTEM ; i++)
		this->particleSystemList[i] = NULL;
	this->optM = OptionManager::getInstance();
}

/**
* Destructor
*/
ParticleManager::~ParticleManager()
{
	freeParticles();
}

/**
* Set the player
* Needed before handling particles
* @param player1 Player pointer
*/
void ParticleManager::setPlayer(Player * player1)
{
	this->player1 = player1;
}

/**
* Add the particle system to the list
* @param systemType Type of the Particle System
* @param position Position of the Particle System
* @param maxParticles Max Particles alive
*/
void ParticleManager::addParticle(int systemType, Vector3d position, int maxParticles)
{
	if(this->optM->getParticleModifierValue() > 0)
	{
		bool added = false;
		for(int i = 0 ; i < MAXPARTICLESYSTEM && !added; i++)
		{
			if(this->particleSystemList[i] == NULL)
			{
				this->particleSystemList[i] = new ParticleSystem(systemType,position,maxParticles / this->optM->getParticleModifier(),this->player1);
				added = true;
			}
		}
	}
}

/**
* Refresh the Particle Systems
* @param time Elapsed time
*/
void ParticleManager::refresh(long time)
{
	if(this->optM->getParticleModifierValue() > 0)
	{
		Vector3d tmp;
		for(int i = 0 ; i < MAXPARTICLESYSTEM ; i++)
		{
			this->index[i] = i;
			if(this->particleSystemList[i] != NULL)
			{
				if(this->particleSystemList[i]->isAlive())
				{
					this->particleSystemList[i]->refresh(time);
					tmp = Vector3d(this->player1->getCoor()[0], 0 , this->player1->getCoor()[2]) - this->particleSystemList[i]->getPosition();
					this->distance[i] = tmp.length();
				}
				else
				{
					delete this->particleSystemList[i];
					this->particleSystemList[i] = NULL;
					this->distance[i] = -1;
				}
			}
			else
				this->distance[i] = -1;
		}
		bubbleSort();
	}
}

/**
* Draw the particle systems
* @param blocksNumber Blocks that are drawn
* @param levelWidth Width of the level
*/
void ParticleManager::draw(std::vector<int> blocksNumber, int levelWidth)
{
	if(this->optM->getParticleModifierValue() > 0)
	{
		bool done = false;
		for(int i = MAXPARTICLESYSTEM - 1 ; i >= 0 && !done ; i--)
		{
			if(this->distance[i] > -1)
			{
				bool particleSystemValid = false;
				int particleSystemBlocNumber = levelWidth * (int)(this->particleSystemList[index[i]]->getPosition()[2] / BLOCDEFAULTSIZE) + (int)(-this->particleSystemList[index[i]]->getPosition()[0] / BLOCDEFAULTSIZE);

				// Draw only particles systems in a bloc that has been drawn in this frame
				for(int j = 0 ; j < (int)blocksNumber.size() && !particleSystemValid ; j++)
					if(blocksNumber[j] == particleSystemBlocNumber)
						particleSystemValid = true;

				if(particleSystemValid)
					this->particleSystemList[index[i]]->draw();
			}
			else
				done = true;
		}
	}
}

/**
* Free the Particle System
*/
void ParticleManager::freeParticles()
{
	for(int i = 0 ; i < MAXPARTICLESYSTEM ; i++)
	{
		if(this->particleSystemList[i] != NULL)
		{
			delete this->particleSystemList[i];
			this->particleSystemList[i] = NULL;
		}
	}
}

/**
* Bubble Sort
*/
void ParticleManager::bubbleSort()
{
	bool to_do = true;
	float tmp;
	int tmp2;
	for(int i = 0 ; i < MAXPARTICLESYSTEM && to_do; i++)
	{
		to_do = false;
		for(int j = 0 ; j < MAXPARTICLESYSTEM - 1 - i ; j++)
		{
			if(this->distance[j] > this->distance[j + 1])
			{
				tmp = this->distance[j];
				this->distance[j] = this->distance[j + 1];
				this->distance[j + 1] = tmp;

				tmp2 = this->index[j];
				this->index[j] = this->index[j + 1];
				this->index[j + 1] = tmp2;

				to_do = true;
			}
		}
	}
}
