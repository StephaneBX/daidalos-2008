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

#ifndef __PARTICLESYSTEM_H
#define __PARTICLESYSTEM_H

#include "../includes/vector3d.h"
#include "../particles/particle.h"
#include "../elements/player.h"

/**
* Particle System Class
* Handle the Particle system
* @author Stephane Baudoux
*/
class ParticleSystem
{
private:
	float particlesSize;
	float averageSpeed;
	float matSpec[4];
	float matDif[4];
	float matAmb[4];
	float * distance;
	int systemType;
	int maxParticles;
	int risenParticles;
	int particleTexture;
	int * index;
	long maxSystemLife;
	long systemLife;
	bool needSort;

	Vector3d position;
	Particle ** particles;
	Player * player1;

public:
	ParticleSystem(int systemType, Vector3d position, int maxParticles, Player * player);
	~ParticleSystem();
	void initSystem();
	void resetSystemLife();
	void initParticle(int i);
	void refresh(long time);
	void draw();
	void setPosition(Vector3d position);
	bool isAlive();
	Vector3d getPosition();

private:
	void bubbleSort();
};

#endif
