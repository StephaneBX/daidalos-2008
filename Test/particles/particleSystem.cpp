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

#include <GL/gl.h>
#include "../includes/global_var.h"
#include "../includes/utility.h"
#include "../manager/texture.h"
#include "../particles/particleSystem.h"

/**
* Constructor
* @param systemType Syetem type
* @param position Position
* @param maxParticles Max particles
* @param player Pointer to the Player
*/
ParticleSystem::ParticleSystem(int systemType, Vector3d position, int maxParticles, Player * player)
{
	this->matSpec[0] = 0.1f;
	this->matSpec[1] = 0.1f;
	this->matSpec[2] = 0.1f;
	this->matSpec[3] = 1.0f;

	this->matDif[0] = 0.1f;
	this->matDif[1] = 0.1f;
	this->matDif[2] = 0.1f;
	this->matDif[3] = 1.0f;

	this->matAmb[0] = 0.1f;
	this->matAmb[1] = 0.1f;
	this->matAmb[2] = 0.1f;
	this->matAmb[3] = 1.0f;

	this->systemType = systemType;
	this->position = position;
	this->maxParticles = maxParticles;
	this->systemLife = 0;

	this->player1 = player;
	this->index = NULL;
	this->distance = NULL;

	this->particles = new Particle*[this->maxParticles];
	for(int i = 0 ; i < this->maxParticles ; i++)
		this->particles[i] = NULL;

	switch(systemType)
	{
		case PARTICLE_IMPACT_SMOKE_REVERSE:
		case PARTICLE_IMPACT_SMOKE:
			this->particleTexture = CTextureManager::getInstance()->LoadTexture("imgs/particles/smokeParticle.tga");
			this->averageSpeed = 1.5f;
			this->maxSystemLife = 500;
			this->particlesSize = 0.30f;
			this->needSort = false;
		break;

		case PARTICLE_SMOKE:
			this->particleTexture = CTextureManager::getInstance()->LoadTexture("imgs/particles/smokeParticle.tga");
			this->averageSpeed = 0.5f;
			this->maxSystemLife = 15000;
			this->particlesSize = 0.10f;
			this->needSort = false;
		break;

		case PARTICLE_IMPACT:
			this->particleTexture = CTextureManager::getInstance()->LoadTexture("imgs/particles/impactParticle.tga");
			this->averageSpeed = 5.0f;
			this->maxSystemLife = 200;
			this->particlesSize = 0.10f;

			this->matSpec[0] = 1.0f;
			this->matSpec[1] = 1.0f;
			this->matSpec[2] = 1.0f;
			this->matSpec[3] = 1.0f;

			this->matDif[0] = 1.0f;
			this->matDif[1] = 1.0f;
			this->matDif[2] = 1.0f;
			this->matDif[3] = 1.0f;

			this->matAmb[0] = 1.0f;
			this->matAmb[1] = 1.0f;
			this->matAmb[2] = 1.0f;
			this->matAmb[3] = 1.0f;

			this->needSort = false;
		break;

		case PARTICLE_RISE:
		case PARTICLE_RISE_REVERSE:
			this->particleTexture = CTextureManager::getInstance()->LoadTexture("imgs/particles/teleportParticle.tga");
			this->averageSpeed = 2.0f;
			this->maxSystemLife = -1;
			this->particlesSize = 0.1f;

			this->matSpec[0] = 1.0f;
			this->matSpec[1] = 1.0f;
			this->matSpec[2] = 1.0f;
			this->matSpec[3] = 1.0f;

			this->matDif[0] = 1.0f;
			this->matDif[1] = 1.0f;
			this->matDif[2] = 1.0f;
			this->matDif[3] = 1.0f;

			this->matAmb[0] = 1.0f;
			this->matAmb[1] = 1.0f;
			this->matAmb[2] = 1.0f;
			this->matAmb[3] = 1.0f;

			this->needSort = true;
			this->index = new int[this->maxParticles];
			this->distance = new float[this->maxParticles];
		break;

		case PARTICLE_RISE2:
		case PARTICLE_RISE2_REVERSE:
			this->particleTexture = CTextureManager::getInstance()->LoadTexture("imgs/particles/teleport2Particle.tga");
			this->averageSpeed = 2.0f;
			this->maxSystemLife = -1;
			this->particlesSize = 0.1f;

			this->matSpec[0] = 1.0f;
			this->matSpec[1] = 1.0f;
			this->matSpec[2] = 1.0f;
			this->matSpec[3] = 1.0f;

			this->matDif[0] = 1.0f;
			this->matDif[1] = 1.0f;
			this->matDif[2] = 1.0f;
			this->matDif[3] = 1.0f;

			this->matAmb[0] = 1.0f;
			this->matAmb[1] = 1.0f;
			this->matAmb[2] = 1.0f;
			this->matAmb[3] = 1.0f;

			this->needSort = true;
			this->index = new int[this->maxParticles];
			this->distance = new float[this->maxParticles];
		break;

		case PARTICLE_TRAP:
			this->particleTexture = CTextureManager::getInstance()->LoadTexture("imgs/particles/smokeParticle3.tga");
			this->averageSpeed = 3.5f;
			this->maxSystemLife = -1;
			this->particlesSize = 0.65f;

			this->matSpec[0] = 0.0f;
			this->matSpec[1] = 0.0f;
			this->matSpec[2] = 0.0f;
			this->matSpec[3] = 1.0f;

			this->matDif[0] = 0.0f;
			this->matDif[1] = 0.0f;
			this->matDif[2] = 0.0f;
			this->matDif[3] = 1.0f;

			this->needSort = false;
		break;
	}
	initSystem();
}

/**
* Destructor
*/
ParticleSystem::~ParticleSystem()
{
	for(int i = 0 ; i < this->maxParticles ; i++)
	{
		if(this->particles[i] != NULL)
		{
			delete this->particles[i];
			this->particles[i] = NULL;
		}
	}
	delete[] this->particles;
	this->particles = NULL;

	if(this->index != NULL)
	{
		delete this->index;
		this->index = NULL;
	}

	if(this->distance != NULL)
	{
		delete this->distance;
		this->distance = NULL;
	}
}

/**
* Initialize the particle system
*/
void ParticleSystem::initSystem()
{
	for(int i = 0 ; i < this->maxParticles ; i++)
		initParticle(i);

	if(this->needSort)
	{
		for(int i = 0 ; i < this->maxParticles ; i++)
		{
			this->index[i] = i;
			this->distance[i] = this->particles[i]->getPosition()[2];
		}
		bubbleSort();
	}
}

/**
* Reset de System Life to 0
*/
void ParticleSystem::resetSystemLife()
{
	this->systemLife = 0;
}

/**
* Initialize a particle
* @param i Particle number
*/
void ParticleSystem::initParticle(int i)
{
	if(this->particles[i] != NULL)
	{
		delete this->particles[i];
		this->particles[i] = NULL;
	}

	Vector3d direction;
	switch(this->systemType)
	{
		case PARTICLE_SMOKE:

			this->particles[i] = new Particle(Vector3d(0.0f,0.0f,0.0f),
				Vector3d(RANDOM_FLOAT / 10.0f,RANDOM_ABS_FLOAT,RANDOM_FLOAT / 10.0f),
				0.0f,
				Utility::randomRange(0,3500),
				4000,
				this->averageSpeed * RANDOM_ABS_FLOAT,
				Vector3d(0.3f,0.3f,0.3f),
				this->particlesSize,
				0.2f);
		break;

		case PARTICLE_IMPACT_SMOKE:

			this->particles[i] = new Particle(Vector3d(RANDOM_FLOAT / 10.0f,0.0f,RANDOM_FLOAT / 10.0f),
				Vector3d(RANDOM_FLOAT / 10.0f,RANDOM_ABS_FLOAT,RANDOM_FLOAT / 10.0f),
				0.0f,
				Utility::randomRange(0,this->maxSystemLife * 2),
				this->maxSystemLife * 2,
				this->averageSpeed * RANDOM_ABS_FLOAT,
				Vector3d(0.3f,0.3f,0.3f),
				this->particlesSize,
				0.2f);
		break;

		case PARTICLE_IMPACT_SMOKE_REVERSE:

			this->particles[i] = new Particle(Vector3d(RANDOM_FLOAT / 10.0f,0.0f,RANDOM_FLOAT / 10.0f),
				Vector3d(RANDOM_FLOAT / 10.0f,-RANDOM_ABS_FLOAT,RANDOM_FLOAT / 10.0f),
				0.0f,
				Utility::randomRange(0,this->maxSystemLife * 2),
				this->maxSystemLife * 2,
				this->averageSpeed * RANDOM_ABS_FLOAT,
				Vector3d(0.3f,0.3f,0.3f),
				this->particlesSize,
				0.2f);
		break;

		case PARTICLE_IMPACT:

			direction = Vector3d(RANDOM_FLOAT,RANDOM_ABS_FLOAT,RANDOM_FLOAT);
			direction.normalize();

			this->particles[i] = new Particle(
				Vector3d(RANDOM_FLOAT / 10.0f ,0.0f,RANDOM_FLOAT / 10.0f),
				direction,
				5.5f,
				Utility::randomRange(0,this->maxSystemLife * 2),
				this->maxSystemLife * 2,
				this->averageSpeed * RANDOM_ABS_FLOAT,
				Vector3d(0.3f,0.3f,0.3f),
				this->particlesSize,0.0f);
		break;

		case PARTICLE_RISE:
		case PARTICLE_RISE2:

			this->particles[i] = new Particle(
				Vector3d(RANDOM_FLOAT * 0.7f ,0.0f,RANDOM_FLOAT * 0.7f),
				Vector3d(0.0f,1.0f,0.0f),
				0.0f,
				Utility::randomRange(0,4000),
				7000,
				( this->averageSpeed * RANDOM_ABS_FLOAT )/3.0f * 2.0f + (this->averageSpeed /  3.0f),
				Vector3d(1.0f,1.0f,1.0f),
				this->particlesSize,
				0.0f);
		break;

		case PARTICLE_RISE_REVERSE:
		case PARTICLE_RISE2_REVERSE:

			this->particles[i] = new Particle(
				Vector3d(RANDOM_FLOAT * 0.7f ,0.0f,RANDOM_FLOAT * 0.7f),
				Vector3d(0.0f,-1.0f,0.0f),
				0.0f,
				Utility::randomRange(0,4000),
				7000,
				( this->averageSpeed * RANDOM_ABS_FLOAT )/3.0f * 2.0f + (this->averageSpeed / 3.0f),
				Vector3d(1.0f,1.0f,1.0f),
				this->particlesSize,
				0.0f);
		break;

		case PARTICLE_TRAP:
			
			this->particles[i] = new Particle(Vector3d(RANDOM_FLOAT,RANDOM_ABS_FLOAT,RANDOM_FLOAT),
				Vector3d(RANDOM_FLOAT / 10.0f,RANDOM_ABS_FLOAT,RANDOM_FLOAT / 10.0f),
				0.0f,
				Utility::randomRange(0,2500),
				3000,
				this->averageSpeed * RANDOM_ABS_FLOAT,
				Vector3d(1.0f,1.0f,1.0f),
				this->particlesSize,
				0.15f);
		break;
	}
}

/**
* Refresh the particle system values
* @param time Elapsed time
*/
void ParticleSystem::refresh(long time)
{
	this->risenParticles = 0;
	bool newParticleAdded = false;
	this->systemLife += time;
	for(int i = 0 ; i < this->maxParticles ; i++)
	{
		if(this->particles[i]->isAlive())
			this->particles[i]->refresh(time);
		else
		{
			if(this->risenParticles < MAXRISENPARTICLES && (this->systemLife < this->maxSystemLife || this->maxSystemLife == -1))
			{
				initParticle(i);
				this->risenParticles++;
				newParticleAdded = true;
			}
		}
	}

	if(this->needSort && newParticleAdded)
	{
		for(int i = 0 ; i < this->maxParticles ; i++)
		{
			this->index[i] = i;
			this->distance[i] = this->particles[i]->getPosition()[2];
		}
		bubbleSort();
	}
}

/**
* Draw the particle system
*/
void ParticleSystem::draw()
{
	glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
	glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
	glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
	glMaterialf (GL_FRONT,GL_SHININESS,0.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glPushMatrix();

		glTranslatef(this->position[0],this->position[1],this->position[2]);

		// Rotate the particle system to face the player
		glRotatef(this->player1->getAngleXZ() - 180,0, 1, 0);

		Vector3d final[4];

		glBindTexture  ( GL_TEXTURE_2D, this->particleTexture );
		glBegin(GL_QUADS);

			/* We draw in order or not depending of the configuration choosen */
			if(this->needSort)
			{
				if(this->systemType == PARTICLE_RISE || this->systemType == PARTICLE_RISE2 || this->systemType == PARTICLE_RISE_REVERSE || this->systemType == PARTICLE_RISE2_REVERSE)
					glColor4f(1.0f, 1.0f, 1.0f,1.0f);

				for(int i = 0 ; i < this->maxParticles ; i++)
				{
					if(this->particles[this->index[i]]->isAlive())
					{
						final[0] = this->particles[this->index[i]]->getPosition();
						final[1] = this->particles[this->index[i]]->getPosition();
						final[2] = this->particles[this->index[i]]->getPosition();
						final[3] = this->particles[this->index[i]]->getPosition();
						final[0][0] += -this->particles[this->index[i]]->getSize() / 2.0f;
						final[0][1] +=  this->particles[this->index[i]]->getSize() / 2.0f;
						final[1][0] += -this->particles[this->index[i]]->getSize() / 2.0f;
						final[1][1] += -this->particles[this->index[i]]->getSize() / 2.0f;
						final[2][0] +=  this->particles[this->index[i]]->getSize() / 2.0f;
						final[2][1] += -this->particles[this->index[i]]->getSize() / 2.0f;
						final[3][0] +=  this->particles[this->index[i]]->getSize() / 2.0f;
						final[3][1] +=  this->particles[this->index[i]]->getSize() / 2.0f;

						if(this->systemType != PARTICLE_RISE && this->systemType != PARTICLE_RISE2 && this->systemType != PARTICLE_RISE_REVERSE && this->systemType != PARTICLE_RISE2_REVERSE)
							glColor4f(this->particles[this->index[i]]->getRGB()[0], this->particles[this->index[i]]->getRGB()[1], this->particles[this->index[i]]->getRGB()[2],this->particles[this->index[i]]->getAlpha());

						glTexCoord2f(0.2f,0.8f);	glVertex3f(final[0][0],final[0][1],final[0][2]);
						glTexCoord2f(0.2f,0.2f);	glVertex3f(final[1][0],final[1][1],final[1][2]);
						glTexCoord2f(0.8f,0.2f);	glVertex3f(final[2][0],final[2][1],final[2][2]);
						glTexCoord2f(0.8f,0.8f);	glVertex3f(final[3][0],final[3][1],final[3][2]);
					}
				}
			}
			else
			{
				for(int i = 0 ; i < this->maxParticles ; i++)
				{
					if(this->particles[i]->isAlive())
					{
						final[0] = this->particles[i]->getPosition();
						final[1] = this->particles[i]->getPosition();
						final[2] = this->particles[i]->getPosition();
						final[3] = this->particles[i]->getPosition();
						final[0][0] += -this->particles[i]->getSize() / 2.0f;
						final[0][1] +=  this->particles[i]->getSize() / 2.0f;
						final[1][0] += -this->particles[i]->getSize() / 2.0f;
						final[1][1] += -this->particles[i]->getSize() / 2.0f;
						final[2][0] +=  this->particles[i]->getSize() / 2.0f;
						final[2][1] += -this->particles[i]->getSize() / 2.0f;
						final[3][0] +=  this->particles[i]->getSize() / 2.0f;
						final[3][1] +=  this->particles[i]->getSize() / 2.0f;

						final[0][2] = i * 0.001f;
						final[1][2] = i * 0.001f;
						final[2][2] = i * 0.001f;
						final[3][2] = i * 0.001f;

						glColor4f(this->particles[i]->getRGB()[0], this->particles[i]->getRGB()[1], this->particles[i]->getRGB()[2],this->particles[i]->getAlpha());
						glTexCoord2f(0.0f,1.0f);	glVertex3f(final[0][0],final[0][1],final[0][2]);
						glTexCoord2f(0.0f,0.0f);	glVertex3f(final[1][0],final[1][1],final[1][2]);
						glTexCoord2f(1.0f,0.0f);	glVertex3f(final[2][0],final[2][1],final[2][2]);
						glTexCoord2f(1.0f,1.0f);	glVertex3f(final[3][0],final[3][1],final[3][2]);
					}
				}
			}

			glColor4f(1.0f, 1.0f, 1.0f,1.0f);
		glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
}

/**
* Set the system position
* @param position New position
*/
void ParticleSystem::setPosition(Vector3d position)
{
	this->position = position;
}

/**
* Check if the system is still alive
* @return Particle system state
*/
bool ParticleSystem::isAlive()
{
	bool alive = false;
	for(int i = 0 ; i < this->maxParticles && !alive; i++)
		if(this->particles[i]->isAlive())
			alive = true;
	return alive;
}

/**
* Return the system position
* @return System position
*/
Vector3d ParticleSystem::getPosition()
{
	return this->position;
}

/**
* Bubble Sort
*/
void ParticleSystem::bubbleSort()
{
	bool to_do = true;
	float tmp;
	int tmp2;
	for(int i = 0 ; i < this->maxParticles && to_do; i++)
	{
		to_do = false;
		for(int j = 0 ; j < this->maxParticles - 1 - i ; j++)
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
