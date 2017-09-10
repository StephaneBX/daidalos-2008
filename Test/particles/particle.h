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

#ifndef __PARTICLE_H
#define __PARTICLE_H

#include "../includes/vector3d.h"

enum
{
	PARTICLE_SMOKE,
	PARTICLE_IMPACT,
	PARTICLE_IMPACT_SMOKE,
	PARTICLE_IMPACT_SMOKE_REVERSE,
	PARTICLE_RISE,
	PARTICLE_RISE_REVERSE,
	PARTICLE_RISE2,
	PARTICLE_RISE2_REVERSE,
	PARTICLE_TRAP
};


/**
* Particle Class
* Handle particles
* @author Stephane Baudoux
*/
class Particle
{
protected:
	Vector3d position;
	Vector3d direction;
	Vector3d rgb;
	float mass;
	float speed;
	float alpha;
	float size;
	float growSize;
	bool alive;
	long life;
	long maxLife;

public:
	Particle();
	Particle(Vector3d position, Vector3d direction, float mass, long startLife, long maxLife, float speed, Vector3d rgb, float size, float growSize);
	~Particle();
	Vector3d getPosition();
	Vector3d getDirection();
	Vector3d getRGB();
	float getMass();
	float getSpeed();
	float getAlpha();
	float getSize();
	bool isAlive();
	long getLife();
	long getMaxLife();
	void setPosition(float x, float y, float z);
	void setDirection(float x, float y, float z);
	void setMass(float m);
	void setSpeed(float s);
	void setLife(long l);
	void addLife(long l);
	void setAlive(bool a);
	void setAlpha(float a);
	virtual void refresh(long time);
};

#endif
