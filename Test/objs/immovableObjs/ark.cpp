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

#include <sstream>
#include <math.h>
#include <GL/gl.h>
#include "../../includes/global_var.h"
#include "../../manager/optionManager.h"
#include "../../objs/immovableObjs/ark.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param player1 Pointer to the Player
* @param wallTextureNumber Texture number
* @param objectNumber GList number
* @param ambientLight Ambient light value
*/
Ark::Ark(int x, int z, Player * player1, int wallTextureNumber, int objectNumber, int ambientLight) : ImmovableObject(x,z,player1)
{
	this->texturesManager = CTextureManager::getInstance();
	this->lightQuality = OptionManager::getInstance()->getLightQuality();

	this->arkType = arkType;
	this->objectNumber = objectNumber;

	this->sound = this->soundM->loadSound("sounds/environment/rock_impact.wav");
	this->matType = OBJECTMAT_ROCK;
	this->impactEnabled = true;

	std::ostringstream stringstream;
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/wall" << wallTextureNumber << ".tga";
	this->wallTextureId = this->texturesManager->LoadTexture(stringstream.str());

	this->matSpec[0] = 0.1f;
	this->matSpec[1] = 0.1f;
	this->matSpec[2] = 0.1f;
	this->matSpec[3] = 1.0f;

	this->matDif[0] = 0.4f;
	this->matDif[1] = 0.4f;
	this->matDif[2] = 0.4f;
	this->matDif[3] = 1.0f;

	this->matAmb[0] = 0.4f * ambientLight / 100.0f;
	this->matAmb[1] = 0.4f * ambientLight / 100.0f;
	this->matAmb[2] = 0.4f * ambientLight / 100.0f;
	this->matAmb[3] = 1.0f;

	calculate();
}

/**
* Destructor
*/
Ark::~Ark(){}

/**
* Calculate the Ark
*/
void Ark::calculate()
{
	for(int j = 0; j <= this->lightQuality ; j++)
	{
		this->wallQuality = (float)OptionManager::getInstance()->getDoorUpperQuality(j);
		glNewList(this->objectNumber + j,GL_COMPILE); 
		glBindTexture  ( GL_TEXTURE_2D, this->wallTextureId);
		glBegin(GL_QUADS);
		for( int i = 0 ; i < this->wallQuality ; i++)
		{
			for( int j = 0 ; j < this->wallQuality ; j++)
			{
				// Bottom
				glNormal3f(0.0f, 0.0f, -1.0f);
				glTexCoord2f(i/this->wallQuality * 0.2f		+ 0.2f * ((int)(-this->x/2)%5)		,j/this->wallQuality * 0.25f + 0.75f);			glVertex3f(x - (2 * i/this->wallQuality)		, DOORDEFAULTHEIGHT * 2.0f + j/this->wallQuality * 2			, z);
				glTexCoord2f((i + 1)/this->wallQuality * 0.2f	+ 0.2f * ((int)(-this->x/2)%5)	,j/this->wallQuality * 0.25f + 0.75f);			glVertex3f(x - (2 * (i + 1)/this->wallQuality)	, DOORDEFAULTHEIGHT * 2.0f + j/this->wallQuality * 2			, z);
				glTexCoord2f((i + 1)/this->wallQuality * 0.2f	+ 0.2f * ((int)(-this->x/2)%5)	,(j+1)/this->wallQuality * 0.25f + 0.75f);		glVertex3f(x - (2 * (i + 1)/this->wallQuality)	, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->wallQuality * 2		, z); 	
				glTexCoord2f(i/this->wallQuality * 0.2f		+ 0.2f * ((int)(-this->x/2)%5)		,(j+1)/this->wallQuality * 0.25f + 0.75f);		glVertex3f(x - (2 * i/this->wallQuality)		, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->wallQuality * 2		, z);

				// Top
				glNormal3f(0.0f, 0.0f, 1.0f);
				glTexCoord2f((i + 1)/this->wallQuality * 0.2f	+ 0.2f * ((int)(-this->x/2)%5)	,j/this->wallQuality * 0.25f + 0.75f);			glVertex3f(x - (2 * (i + 1)/this->wallQuality)	, DOORDEFAULTHEIGHT * 2.0f + j/this->wallQuality * 2			, z + 2);
				glTexCoord2f(i/this->wallQuality * 0.2f		+ 0.2f * ((int)(-this->x/2)%5)		,j/this->wallQuality * 0.25f + 0.75f);			glVertex3f(x - (2 * i/this->wallQuality)		, DOORDEFAULTHEIGHT * 2.0f + j/this->wallQuality * 2			, z + 2);
				glTexCoord2f(i/this->wallQuality * 0.2f		+ 0.2f * ((int)(-this->x/2)%5)		,(j + 1)/this->wallQuality * 0.25f + 0.75f);	glVertex3f(x - (2 * i/this->wallQuality)		, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->wallQuality * 2		, z + 2); 	
				glTexCoord2f((i + 1)/this->wallQuality * 0.2f	+ 0.2f * ((int)(-this->x/2)%5)	,(j + 1)/this->wallQuality * 0.25f + 0.75f);	glVertex3f(x - (2 * (i + 1)/this->wallQuality)	, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->wallQuality * 2		, z + 2);

				// Left
				glNormal3f(1.0f, 0.0f, 0.0f);
				glTexCoord2f((i + 1)/this->wallQuality * 0.2f	+ 0.2f * ((int)(this->z/2)%5)	,j/this->wallQuality * 0.25f + 0.75f);			glVertex3f(x, DOORDEFAULTHEIGHT * 2.0f + j/this->wallQuality * 2			, z + (2 * (i + 1)/this->wallQuality));
				glTexCoord2f(i/this->wallQuality * 0.2f		+ 0.2f * ((int)(this->z/2)%5)		,j/this->wallQuality * 0.25f + 0.75f);			glVertex3f(x, DOORDEFAULTHEIGHT * 2.0f + j/this->wallQuality * 2			, z + (2 * i/this->wallQuality));
				glTexCoord2f(i/this->wallQuality * 0.2f		+ 0.2f * ((int)(this->z/2)%5)		,(j + 1)/this->wallQuality * 0.25f + 0.75f);	glVertex3f(x, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->wallQuality * 2		, z + (2 * i/this->wallQuality)); 	
				glTexCoord2f((i + 1)/this->wallQuality * 0.2f	+ 0.2f * ((int)(this->z/2)%5)	,(j + 1)/this->wallQuality * 0.25f + 0.75f);	glVertex3f(x, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->wallQuality * 2		, z + (2 * (i + 1)/this->wallQuality));

				// Right
				glNormal3f(-1.0f, 0.0f, 0.0f);
				glTexCoord2f(i/this->wallQuality * 0.2f		+ 0.2f * ((int)(this->z/2)%5)		,j/this->wallQuality * 0.25f + 0.75f);			glVertex3f(x - 2, DOORDEFAULTHEIGHT * 2.0f + j/this->wallQuality * 2		, z + (2 * i/this->wallQuality));
				glTexCoord2f((i + 1)/this->wallQuality * 0.2f	+ 0.2f * ((int)(this->z/2)%5)	,j/this->wallQuality * 0.25f + 0.75f);			glVertex3f(x - 2, DOORDEFAULTHEIGHT * 2.0f+ j/this->wallQuality * 2			, z + (2 * (i + 1)/this->wallQuality));
				glTexCoord2f((i + 1)/this->wallQuality * 0.2f	+ 0.2f * ((int)(this->z/2)%5)	,(j + 1)/this->wallQuality * 0.25f + 0.75f);	glVertex3f(x - 2, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->wallQuality * 2	, z + (2 * (i + 1)/this->wallQuality)); 	
				glTexCoord2f(i/this->wallQuality * 0.2f		+ 0.2f * ((int)(this->z/2)%5)		,(j + 1)/this->wallQuality * 0.25f + 0.75f);	glVertex3f(x - 2, DOORDEFAULTHEIGHT * 2.0f + (j + 1)/this->wallQuality * 2	, z + (2 * i/this->wallQuality));

				// Down
				glNormal3f(0.0f, -1.0f, 0.0f);
				glTexCoord2f(i/this->wallQuality * 0.2f		+ 0.2f * ((int)(-this->x/2)%5)		,j/this->wallQuality * 0.25f + 0.75f);			glVertex3f(x - (2 * i/this->wallQuality)		, DOORDEFAULTHEIGHT * 2.0f	, z + (2 * j/this->wallQuality));
				glTexCoord2f(i/this->wallQuality * 0.2f		+ 0.2f * ((int)(-this->x/2)%5)		,(j + 1)/this->wallQuality * 0.25f + 0.75f);	glVertex3f(x - (2 * i/this->wallQuality)		, DOORDEFAULTHEIGHT * 2.0f	, z + (2 * (j + 1)/this->wallQuality));
				glTexCoord2f((i + 1)/this->wallQuality * 0.2f	+ 0.2f * ((int)(-this->x/2)%5)	,(j + 1)/this->wallQuality * 0.25f + 0.75f);	glVertex3f(x - (2 * (i + 1)/this->wallQuality)	, DOORDEFAULTHEIGHT * 2.0f	, z + (2 * (j + 1)/this->wallQuality)); 	
				glTexCoord2f((i + 1)/this->wallQuality * 0.2f	+ 0.2f * ((int)(-this->x/2)%5)	,j/this->wallQuality * 0.25f + 0.75f);			glVertex3f(x - (2 * (i + 1)/this->wallQuality)	, DOORDEFAULTHEIGHT * 2.0f	, z + (2 * j/this->wallQuality));
			}
		}
		glEnd();
		glEndList();
	}
}

/**
* Draw the Ark
*/
void Ark::draw( float * lightPos)
{
	glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
	glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
	glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
	glMaterialf (GL_FRONT,GL_SHININESS,50.0f);

	float dist = sqrt((this->x - player1->getCoor()[0]) * (this->x - player1->getCoor()[0]) + (this->z - player1->getCoor()[2]) * (this->z - player1->getCoor()[2]));
	if(dist > VIEWQUALITYMIN0 || this->lightQuality == 0)
		glCallList(this->objectNumber);
	else if(dist > VIEWQUALITYMIN1 || this->lightQuality == 1)
			glCallList(this->objectNumber + 1);
	else if(dist > VIEWQUALITYMIN2 || this->lightQuality == 2)
			glCallList(this->objectNumber + 2);
	else
		glCallList(this->objectNumber + 3);
}

/**
* Return if the object has to replace the player
* @return If need replacement
*/
bool Ark::needReplacement()
{
	return false;
}

/**
* Detect collisions
* @param x X coordinate to test
* @param z Z coordinate to test
* @param y Y coordinate to test
* @return Result to the collsion test
*/
bool Ark::isCollision(float x, float z, float y)
{
	return y >= DOORDEFAULTHEIGHT * 2.0f;
}

/**
* Detect the Impact Location and return corrects values
* @param x1 Previous Projectile's X coordinate
* @param y1 Previous Projectile's Y coordinate
* @param z1 Previous Projectile's Z coordinate
* @param x2 Projectile's X coordinate
* @param y2 Projectile's Y coordinate
* @param z2 Projectile's Z coordinate
* @param impactX Reference to the Impact X coordinate
* @param impactY Reference to the Impact Y coordinate
* @param impactZ Reference to the Impact Z coordinate
* @param impactAngleX Reference to the Impact X Angle
* @param impactAngleY Reference to the Impact Y Angle
* @param impactAngleZ Reference to the Impact Z Angle
* @param modif Reference to the Impact replacement choosen
* @param impactRadius The Impact Radius
*/
void Ark::impactGetLocation(float x1, float y1, float z1, float x2, float y2, float z2, float &impactX, float &impactY, float &impactZ, float &impactAngleX, float &impactAngleY, float &impactAngleZ, int &modif, float impactRadius)
{
	// The object to test is from the front of this object
	if(y1 > DOORDEFAULTHEIGHT * 2.0f)
	{
		if(z1 <= this->z)
		{
			impactX = x2;
			impactY = y2;
			impactZ = this->z - 0.001f;
			impactAngleX = 0;
			impactAngleY = 0;
			impactAngleZ = 0;
			modif = IMPACT_REPLACEMENT_MINUS_Z;

			// Prevent impact from overflowing outside the object
			if(impactX < this->x - 2 + impactRadius)
				impactX = this->x - 2 + impactRadius;
			if(impactX > this->x - impactRadius)
				impactX = this->x - impactRadius;
			if(impactY < SMALLCEILINGHEIGHT + impactRadius)
				impactY = SMALLCEILINGHEIGHT + impactRadius;
		}
		else
		{
			// The object to test is from behind of this object
			if(z1 >= this->z + 2)
			{
				impactX = x2;
				impactY = y2;
				impactZ = this->z + 2.001f;
				impactAngleX = 0;
				impactAngleY = 180;
				impactAngleZ = 0;
				modif = IMPACT_REPLACEMENT_Z;

				// Prevent impact from overflowing outside the object
				if(impactX < this->x - 2 + impactRadius)
					impactX = this->x - 2 + impactRadius;
				if(impactX > this->x - impactRadius)
					impactX = this->x - impactRadius;
				if(impactY < SMALLCEILINGHEIGHT + impactRadius)
					impactY = SMALLCEILINGHEIGHT + impactRadius;
			}
		}

		// The object to test is from the right of this object
		if(x1 <= this->x - 2)
		{
			impactX = this->x - 2.001f;
			impactY = y2;
			impactZ = z2;
			impactAngleX = 0;
			impactAngleY = 90;
			impactAngleZ = 0;
			modif = IMPACT_REPLACEMENT_MINUS_X;

			// Prevent impact from overflowing outside the object
			if(impactZ < this->z + impactRadius)
				impactZ = this->z + impactRadius;
			if(impactZ > this->z + 2 - impactRadius)
				impactZ = this->z + 2 - impactRadius;
			if(impactY < SMALLCEILINGHEIGHT + impactRadius)
				impactY = SMALLCEILINGHEIGHT + impactRadius;
		}
		else
		{
			// The object to test is from the left of this object
			if(x1 >= this->x)
			{
				impactX = this->x + 0.001f;
				impactY = y2;
				impactZ = z2;
				impactAngleX = 0;
				impactAngleY = -90;
				impactAngleZ = 0;
				modif = IMPACT_REPLACEMENT_X;

				// Prevent impact from overflowing outside the object
				if(impactZ < this->z + impactRadius)
					impactZ = this->z + impactRadius;
				if(impactZ > this->z + 2 - impactRadius)
					impactZ = this->z + 2 - impactRadius;
				if(impactY < SMALLCEILINGHEIGHT + impactRadius)
					impactY = SMALLCEILINGHEIGHT + impactRadius;
			}
		}
	}
	else
	{
		// The object to test from below this object
		impactX = x2;
		impactY = DOORDEFAULTHEIGHT * 2.0f - 0.001f;
		impactZ = z2;
		impactAngleX = 270;
		impactAngleY = 0;
		impactAngleZ = 0;
		modif = IMPACT_REPLACEMENT_MINUS_Y;

		// Prevent impact from overflowing outside the object
		if(impactX < this->x - 2 + impactRadius)
			impactX = this->x - 2 + impactRadius;
		if(impactX > this->x - impactRadius)
			impactX = this->x - impactRadius;
		if(impactZ < this->z + impactRadius)
			impactZ = this->z + impactRadius;
		if(impactZ > this->z + 2 - impactRadius)
			impactZ = this->z + 2 - impactRadius;
	}
}
