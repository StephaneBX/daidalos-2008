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

#include "../particles/particle.h"

/**
* Constructor
*/
Particle::Particle()
{
	this->position[0] = 0;
	this->position[1] = 0;
	this->position[2] = 0;
	this->direction[0] = 0;
	this->direction[1] = 0;
	this->direction[2] = 0;
	this->mass = 0;
	this->speed = 0;
	this->alive = true;
	this->life = 0;
	this->maxLife = 10000;
	this->size = 0.10f;
	this->growSize = 0.0f;
}

/**
* Constructor
* @param position Start position
* @param direction Direction
* @param mass Mass
* @param startLife Initial life value
* @param maxLife Max life
* @param speed Speed
* @param rgb Color
* @param size Original size
* @param growSize Size growing factor
*/
Particle::Particle(Vector3d position, Vector3d direction, float mass, long startLife, long maxLife, float speed, Vector3d rgb, float size, float growSize)
{
	this->position[0] = position[0];
	this->position[1] = position[1];
	this->position[2] = position[2];
	this->direction[0] = direction[0];
	this->direction[1] = direction[1];
	this->direction[2] = direction[2];
	this->mass = mass;
	this->maxLife = maxLife;
	this->life = startLife;
	this->speed = speed;
	this->size = size;
	this->growSize = growSize;
	this->rgb = rgb;
	this->alpha = 1.0f;
	this->alive = true;
}

/**
* Destructor
*/
Particle::~Particle(){}


/**
* Return the particle position
* @return Position
*/
Vector3d Particle::getPosition()
{
	return this->position;
}

/**
* Return the particle direction
* @return Direction
*/
Vector3d Particle::getDirection()
{
	return this->direction;
}

/**
* Return the particle mass
* @return Mass
*/
float Particle::getMass()
{
	return this->mass;
}

/**
* Return the particle speed
* @return Speed
*/
float Particle::getSpeed()
{
	return this->speed;
}

/**
* Return the particle alpha
* @return Alpha
*/
float Particle::getAlpha()
{
	return this->alpha;
}

/**
* Return the particle size
* @return Size
*/
float Particle::getSize()
{
	return this->size;
}

/**
* Return the particle rgb
* @return RGB
*/
Vector3d Particle::getRGB()
{
	return this->rgb;
}

/**
* Return is the particle is alive
* @return Particle state
*/
bool Particle::isAlive()
{
	return this->alive;
}

/**
* Return the particle life
* @return Life
*/
long Particle::getLife()
{
	return this->life;
}

/**
* Return the particle maximum life
* @return Maximum life
*/
long Particle::getMaxLife()
{
	return this->maxLife;
}

/**
* Set the particle position
* @param x New x coordinate
* @param y New y coordinate
* @param z New z coordinate
*/
void Particle::setPosition(float x, float y, float z)
{
	this->position[0] = x;
	this->position[1] = y;
	this->position[2] = z;
}

/**
* Set the particle direction
* @param x New x value
* @param y New y value
* @param z New z value
*/
void Particle::setDirection(float x, float y, float z)
{
	this->direction[0] = x;
	this->direction[1] = y;
	this->direction[2] = z;
}

/**
* Set the particle mass
* @param m New mass
*/
void Particle::setMass(float m)
{
	this->mass = m;
}

/**
* Set the particle speed
* @param s new speed
*/
void Particle::setSpeed(float s)
{
	this->speed = s;
}

/**
* Set the particle life
* @param l New life
*/
void Particle::setLife(long l)
{
	this->life = l;
}

/**
* Add l to the particle's life
* @param l Value to add
*/
void Particle::addLife(long l)
{
	this->life += l;
}

/**
* Enable / Disable the particle
* @param a State to apply
*/
void Particle::setAlive(bool a)
{
	this->alive = a;
}

/**
* Set the particle alpha
* @param a New alpha
*/
void Particle::setAlpha(float a)
{
	this->alpha = a;
}

/**
* Refresh the particle
* @param time Elapsed time
*/
void Particle::refresh(long time)
{
	this->life += time;
	this->alpha = 1.0f - (float)(this->life / (double)this->maxLife);
	if(this->life >= this->maxLife)
	{
		this->alive = false;
	}
	else
	{
		this->position[0] += this->direction[0] * (this->speed * time / 1000.0f);
		this->position[1] += this->direction[1] * (this->speed * time / 1000.0f);
		this->position[2] += this->direction[2] * (this->speed * time / 1000.0f);
		this->size += (this->growSize * time) / 1000.0f;
		this->direction[1] -= (this->mass * time / 1000.0f);
	}
}
