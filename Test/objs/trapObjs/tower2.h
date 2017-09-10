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

#ifndef __TOWER2_H
#define __TOWER2_H

#define TOWERAXEDELAY 3000
#define TOWERAXEMINIMUMHEIGHT 1.0f
#define TOWERAXEINTERVAL 2.0f
#define TOWERAXEHOLEHEIGHT 0.6f
#define TOWERAXETEXTURERATIOX 1.0f
#define TOWERAXETEXTURERATIOY 0.333f

#include "../../objs/obj.h"
#include "../../manager/texture.h"
#include "../../manager/projectileManager.h"

/**
* Tower2 Class
* Handle Tower2 trap object
* @author Stephane Baudoux
*/
class Tower2 : public Obj
{
private:
	int objectNumber;
	int lightQuality;
	int impactId;
	int wallTextureId;
	int wallTextureId2;
	long axeDelay;
	float wallQuality;
	float wallQualityHeight;
	float wallQualityHeightPart1;
	float wallQualityHeightPart2;
	float wallQualityHeightPart3;
	float wallQualityHeightAxe;
	float axeHeight;

	CTextureManager * texturesManager;
	ProjectileManager * projectileM;

public:
	Tower2(int x, int z, Player * player1, int wallTextureNumber, int objectNumber, int ambientLight);
	~Tower2();
	virtual void refresh(long time);
	virtual void draw(float * lightPos);
	virtual void impactGetLocation(float x1, float y1, float z1, float x2, float y2, float z2, float &impactX, float &impactY, float &impactZ, float &impactAngleX, float &impactAngleY, float &impactAngleZ, int &modif, float impactRadius);
	virtual int getType();
	virtual bool isCollision(float x, float z, float y);
	virtual bool needReplacement();

private:
	void calculate();
};

#endif
