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

#ifndef __BLOCK_H
#define __BLOCK_H

#include "../elements/box.h"
#include "../manager/optionManager.h"
#include "../manager/soundManager.h"
#include "../includes/vector2d.h"

/**
* Block Class
* Handle Blocks creation in the game, wich are the basic of the game environment
* @author Stephane Baudoux
*/
class Block
{
private:
	int type;
	int x;
	int z;
	int objectNumber;
	int envNumber;
	int lightQuality;
	int wallTexture;
	int groundTexture;
	int ceilingTexture;

	float groundQuality;
	float matSpec[4];
	float matDif[4];
	float matAmb[4];

	// Box pointers
	Box * leftBox;
	Box * rightBox;
	Box * upBox;
	Box * downBox;

	// Managers pointers
	CTextureManager * texturesManager;
	SoundManager * soundM;
	Mix_Chunk * impactSound;

public:
	Block(int z, int x, int type, GLuint envNumber, int objectNumber, int ambient_light);
	~Block();
	void draw(float playerX, float playerZ);
	void playSound(int volume);
	bool isLeftBoxExists();
	bool isRightBoxExists();
	bool isUpBoxExists();
	bool isDownBoxExists();
	bool isLeftPortalExists();
	bool isRightPortalExists();
	bool isUpPortalExists();
	bool isDownPortalExists();
	bool checkIntersection(const Vector2d & s1, const Vector2d & s2);
	Box * getLeftBox();
	Box * getRightBox();
	Box * getUpBox();
	Box * getDownBox();
	int getPositionX();
	int getPositionZ();
	int getType();
	int getEnvironment();

private:
	void calculate();
};

#endif
