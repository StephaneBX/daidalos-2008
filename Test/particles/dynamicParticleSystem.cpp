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

#include <math.h>
#include <GL/gl.h>
#include "../includes/global_var.h"
#include "../includes/utility.h"
#include "../manager/texture.h"
#include "../particles/dynamicParticleSystem.h"

/**
* Constructor
* @param systemType Type of DynamicParticleSystem
* @param maxParticles Max particles
*/
DynamicParticleSystem::DynamicParticleSystem(int systemType, int maxParticles)
{
	this->matSpec[0] = 0.02f;
	this->matSpec[1] = 0.02f;
	this->matSpec[2] = 0.02f;
	this->matSpec[3] = 1.0f;

	this->matDif[0] = 0.02f;
	this->matDif[1] = 0.02f;
	this->matDif[2] = 0.02f;
	this->matDif[3] = 1.0f;

	this->matAmb[0] = 0.02f;
	this->matAmb[1] = 0.02f;
	this->matAmb[2] = 0.02f;
	this->matAmb[3] = 1.0f;

	this->systemType = systemType;
	this->maxParticles = maxParticles;
	this->systemLife = 0;

	this->positionStart = Vector3d(0.0f,0.0f,0.0f);

	this->particles = new Particle*[this->maxParticles];
	for(int i = 0 ; i < this->maxParticles ; i++)
		this->particles[i] = NULL;

	this->paused = false;

	switch(systemType)
	{
		case PARTICLE_SMOKE:
		this->particleTexture = CTextureManager::getInstance()->LoadTexture("imgs/particles/smokeParticle2.tga");
		this->averageSpeed = 0.5f;
		this->maxSystemLife = 2000;
		this->systemLife = this->maxSystemLife;
		this->particlesSize = 0.05f;
		break;
	}
	initSystem();
}

/**
* Destructor
*/
DynamicParticleSystem::~DynamicParticleSystem()
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
}

/**
* Initialize the particle system
*/
void DynamicParticleSystem::initSystem()
{
	for(int i = 0 ; i < this->maxParticles ; i++)
	{
		initParticle(i);
		this->particles[i]->setAlive(false);
	}
}

/**
* Reset de System Life to 0
*/
void DynamicParticleSystem::resetSystemLife()
{
	this->systemLife = 0;
	this->paused = false;
}

/**
* Set the system life to max
*/
void DynamicParticleSystem::fillSystemLife()
{
	this->systemLife = this->maxSystemLife;
}

/**
* Initialize a particle
* @param i Particle number
*/
void DynamicParticleSystem::initParticle(int i)
{
	if(this->particles[i] != NULL)
	{
		delete this->particles[i];
		this->particles[i] = NULL;
	}

	switch(this->systemType)
	{
		case PARTICLE_SMOKE:

			this->particles[i] = new Particle(Vector3d(this->positionStart[0],this->positionStart[1], this->positionStart[2]),
				Vector3d(RANDOM_FLOAT / 10.0f,RANDOM_ABS_FLOAT,0.0f),
				0.0f,
				Utility::randomRange(0,this->maxSystemLife),
				this->maxSystemLife / 2,
				this->averageSpeed,
				Vector3d(0.1f,0.1f,0.1f),
				this->particlesSize,
				0.02f);
		break;
	}
}

/**
* Refresh the DynamicParticleSystem
* @param time Elapsed time
* @param x Player x coordinate
* @param y Player y coordinate
* @param z Player z coordinate
* @param angleXZ XZ angle
* @param angleY Y angle
* @param angleMini Minimum Y angle allowed
* @param length Length to the origin
*/
void DynamicParticleSystem::refresh(long time, Vector3d systemStart, float playerAngle)
{
	if(!this->paused)
	{

		this->risenParticles = 0;

		// Update position start of the particles
		this->playerAngle = playerAngle;
		this->positionStart = systemStart;

		// Refresh each particles
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
				}
			}
		}
	}
}

/**
* Draw the particle system
*/
void DynamicParticleSystem::draw()
{
	if(!this->paused)
	{
		glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
		glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
		glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
		glMaterialf (GL_FRONT,GL_SHININESS,0.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glBindTexture  ( GL_TEXTURE_2D, this->particleTexture );
		
		for(int i = this->maxParticles - 1 ; i >= 0 ; i--)
		{
			if(this->particles[i]->isAlive())
			{
				glPushMatrix();

					glTranslatef(this->particles[i]->getPosition()[0],this->particles[i]->getPosition()[1],this->particles[i]->getPosition()[2]);

					// Rotate the particle system
					glRotatef(this->playerAngle - 180,0, 1, 0);

					glBegin(GL_TRIANGLE_FAN);
						glTexCoord2f(0.5f,0.5f);	glVertex3f(0.0f,0.0f,-i * 0.002f);

						glTexCoord2f(0.75f,0.95f);	glVertex3f(this->particles[i]->getSize() / 4.0f,this->particles[i]->getSize() / 2.0f	,-i * 0.002f);
						glTexCoord2f(0.25f,0.95f);	glVertex3f(-this->particles[i]->getSize() / 4.0f,this->particles[i]->getSize() / 2.0f	,-i * 0.002f);

						glTexCoord2f(0.25f,0.95f);	glVertex3f(-this->particles[i]->getSize() / 4.0f,this->particles[i]->getSize() / 2.0f	,-i * 0.002f);
						glTexCoord2f(0.05f,0.75f);	glVertex3f(-this->particles[i]->getSize() / 2.0f,this->particles[i]->getSize() / 4.0f	,-i * 0.002f);

						glTexCoord2f(0.05f,0.75f);	glVertex3f(-this->particles[i]->getSize() / 2.0f,this->particles[i]->getSize() / 4.0f	,-i * 0.002f);
						glTexCoord2f(0.05f,0.25f);	glVertex3f(-this->particles[i]->getSize() / 2.0f,-this->particles[i]->getSize() / 4.0f	,-i * 0.002f);

						glTexCoord2f(0.05f,0.25f);	glVertex3f(-this->particles[i]->getSize() / 2.0f,-this->particles[i]->getSize() / 4.0f	,-i * 0.002f);
						glTexCoord2f(0.25f,0.05f);	glVertex3f(-this->particles[i]->getSize() / 4.0f,-this->particles[i]->getSize() / 2.0f	,-i * 0.002f);

						glTexCoord2f(0.25f,0.05f);	glVertex3f(-this->particles[i]->getSize() / 4.0f,-this->particles[i]->getSize() / 2.0f	,-i * 0.002f);
						glTexCoord2f(0.75f,0.05f);	glVertex3f(this->particles[i]->getSize() / 4.0f,-this->particles[i]->getSize() / 2.0f	,-i * 0.002f);

						glTexCoord2f(0.75f,0.05f);	glVertex3f(this->particles[i]->getSize() / 4.0f,-this->particles[i]->getSize() / 2.0f	,-i * 0.002f);
						glTexCoord2f(0.95f,0.25f);	glVertex3f(this->particles[i]->getSize() / 2.0f,-this->particles[i]->getSize() / 4.0f	,-i * 0.002f);

						glTexCoord2f(0.95f,0.25f);	glVertex3f(this->particles[i]->getSize() / 2.0f,-this->particles[i]->getSize() / 4.0f	,-i * 0.002f);
						glTexCoord2f(0.95f,0.75f);	glVertex3f(this->particles[i]->getSize() / 2.0f,this->particles[i]->getSize() / 4.0f	,-i * 0.002f);

						glTexCoord2f(0.95f,0.75f);	glVertex3f(this->particles[i]->getSize() / 2.0f,this->particles[i]->getSize() / 4.0f	,-i * 0.002f);
						glTexCoord2f(0.75f,0.95f);	glVertex3f(this->particles[i]->getSize() / 4.0f,this->particles[i]->getSize() / 2.0f	,-i * 0.002f);

					glEnd();

				glPopMatrix();
			}
		}
		glColor4f(1.0f, 1.0f, 1.0f,1.0f);
		glDisable(GL_BLEND);
	}
}



/**
* Check if the system is still alive
* @return DynamicParticleSystem state
*/
bool DynamicParticleSystem::isAlive()
{
	bool alive = false;
	for(int i = 0 ; i < this->maxParticles && !alive; i++)
		if(this->particles[i]->isAlive())
			alive = true;
	return alive;
}
