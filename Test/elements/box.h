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

#ifndef __BOX_H
#define __BOX_H

#include "../manager/texture.h"

enum
{
	BOX_TYPE_LEFT,
	BOX_TYPE_UP,
	BOX_TYPE_RIGHT,
	BOX_TYPE_DOWN
};

/**
* Box Class
* Handle the walls on the game
* @author Stephane Baudoux
*/
class Box
{
private:
	int boxType;
	int objectNumber;
	int lightQuality;
	int wallTextureId;
	float x;
	float z;
	float xMin;
	float xMax;
	float zMin;
	float zMax;
	float blocQuality;

	CTextureManager * texturesManager;

public:
	Box(int x, int z, int boxType, GLuint wallTextureId, int objectNumber);
	~Box();
	void draw(float playerX, float playerZ);
	int getType();
	float getXMin();
	float getXMax();
	float getZMin();
	float getZMax();
	bool isCollision(float x, float z);
	bool isCollision(float x, float z, float y);

private:
	void calculate();
};

#endif
