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
#include "../../manager/optionManager.h"
#include "../../includes/global_var.h"
#include "../../objs/immovableObjs/crate.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param player1 Pointer to the Player
* @param crateType Type of crate
* @param crateTextureNumber Texture number
* @param objectNumber GList number
* @param ambientLight Ambient light value
*/
Crate::Crate(int x, int z, Player * player1, int crateType, int crateTextureNumber, int objectNumber, int ambientLight) : ImmovableObject(x,z,player1)
{
	this->crateTextureNumber = crateTextureNumber;
	this->objectNumber = objectNumber;
	this->crateType = crateType;
	this->crateQuality = (float) OptionManager::getInstance()->getCrateQuality();
	this->impactEnabled = true;

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

	texturesManager = CTextureManager::getInstance();
	
	std::ostringstream stringstream;
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/crate/crate_" << crateTextureNumber << ".tga";
	this->crateTextureId= texturesManager->LoadTexture(stringstream.str());

	if(crateTextureNumber == 1)
	{
		this->sound = this->soundM->loadSound("sounds/environment/wood_impact.wav");
		this->matType = OBJECTMAT_WOOD;
	}
	else
	{
		this->sound = this->soundM->loadSound("sounds/environment/metal_impact.wav");
		this->matType = OBJECTMAT_STEEL;
	}

	calculate();
}

/**
* Destructor
*/
Crate::~Crate(){}

/**
* Calculate the Crate
*/
void Crate::calculate()
{
	for(int j = 0; j <= this->lightQuality ; j++)
	{
		this->crateQuality = (float)OptionManager::getInstance()->getCrateQuality(j);
		glNewList(this->objectNumber + j,GL_COMPILE); 
		glBindTexture  ( GL_TEXTURE_2D, crateTextureId );
		glBegin(GL_QUADS);
		for( int i = 0 ; i < this->crateQuality ; i++)
		{
			for( int j = 0 ; j < this->crateQuality * this->crateType ; j++)
			{
				// Front
				glNormal3f(0.0f, 0.0f, -1.0f);
				glTexCoord2f(i/this->crateQuality			,j/this->crateQuality);			glVertex3f(x - (2 * i/this->crateQuality)		, j/this->crateQuality * 2			, z);
				glTexCoord2f((i + 1)/this->crateQuality		,j/this->crateQuality);			glVertex3f(x - (2 * (i + 1)/this->crateQuality)	, j/this->crateQuality * 2			, z);
				glTexCoord2f((i + 1)/this->crateQuality		,(j+1)/this->crateQuality);		glVertex3f(x - (2 * (i + 1)/this->crateQuality)	, (j + 1)/this->crateQuality * 2	, z); 	
				glTexCoord2f(i/this->crateQuality			,(j+1)/this->crateQuality);		glVertex3f(x - (2 * i/this->crateQuality)		, (j + 1)/this->crateQuality * 2	, z);

				// Behind
				glNormal3f(0.0f, 0.0f, 1.0f);
				glTexCoord2f((i + 1)/this->crateQuality		,j/this->crateQuality);			glVertex3f(x - (2 * (i + 1)/this->crateQuality)	, j/this->crateQuality * 2			, z + 2);
				glTexCoord2f(i/this->crateQuality			,j/this->crateQuality);			glVertex3f(x - (2 * i/this->crateQuality)		, j/this->crateQuality * 2			, z + 2);
				glTexCoord2f(i/this->crateQuality			,(j + 1)/this->crateQuality);	glVertex3f(x - (2 * i/this->crateQuality)		, (j + 1)/this->crateQuality * 2	, z + 2); 	
				glTexCoord2f((i + 1)/this->crateQuality		,(j + 1)/this->crateQuality);	glVertex3f(x - (2 * (i + 1)/this->crateQuality)	, (j + 1)/this->crateQuality * 2	, z + 2);

				// Left
				glNormal3f(1.0f, 0.0f, 0.0f);
				glTexCoord2f((i + 1)/this->crateQuality		,j/this->crateQuality);			glVertex3f(x, j/this->crateQuality * 2			, z + (2 * (i + 1)/this->crateQuality));
				glTexCoord2f(i/this->crateQuality			,j/this->crateQuality);			glVertex3f(x, j/this->crateQuality * 2			, z + (2 * i/this->crateQuality));
				glTexCoord2f(i/this->crateQuality			,(j + 1)/this->crateQuality);	glVertex3f(x, (j + 1)/this->crateQuality * 2	, z + (2 * i/this->crateQuality)); 	
				glTexCoord2f((i + 1)/this->crateQuality		,(j + 1)/this->crateQuality);	glVertex3f(x, (j + 1)/this->crateQuality * 2	, z + (2 * (i + 1)/this->crateQuality));

				// Right
				glNormal3f(-1.0f, 0.0f, 0.0f);
				glTexCoord2f(i/this->crateQuality			,j/this->crateQuality);			glVertex3f(x - 2, j/this->crateQuality * 2			, z + (2 * i/this->crateQuality));
				glTexCoord2f((i + 1)/this->crateQuality		,j/this->crateQuality);			glVertex3f(x - 2, j/this->crateQuality * 2			, z + (2 * (i + 1)/this->crateQuality));
				glTexCoord2f((i + 1)/this->crateQuality		,(j + 1)/this->crateQuality);	glVertex3f(x - 2, (j + 1)/this->crateQuality * 2	, z + (2 * (i + 1)/this->crateQuality)); 	
				glTexCoord2f(i/this->crateQuality			,(j + 1)/this->crateQuality);	glVertex3f(x - 2, (j + 1)/this->crateQuality * 2	, z + (2 * i/this->crateQuality));

				// Top
				glNormal3f(0.0f, 1.0f, 0.0f);
				glTexCoord2f(i/this->crateQuality			,j/this->crateQuality);			glVertex3f(x - (2 * i/this->crateQuality)		, 2.0f * this->crateType	, z + (2 * j/(this->crateQuality * this->crateType)));
				glTexCoord2f((i + 1)/this->crateQuality		,j/this->crateQuality);			glVertex3f(x - (2 * (i + 1)/this->crateQuality)	, 2.0f * this->crateType	, z + (2 * j/(this->crateQuality * this->crateType)));
				glTexCoord2f((i + 1)/this->crateQuality		,(j + 1)/this->crateQuality);	glVertex3f(x - (2 * (i + 1)/this->crateQuality)	, 2.0f * this->crateType	, z + (2 * (j + 1)/(this->crateQuality * this->crateType))); 	
				glTexCoord2f(i/this->crateQuality			,(j + 1)/this->crateQuality);	glVertex3f(x - (2 * i/this->crateQuality)		, 2.0f * this->crateType	, z + (2 * (j + 1)/(this->crateQuality * this->crateType)));
				
			}
		}
		glEnd();
		glEndList();
	}
}

/**
* Draw the Crate
*/
void Crate::draw( float * lightPos)
{
	glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
	glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
	glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
	glMaterialf (GL_FRONT,GL_SHININESS,3.0f);

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
* Detect collisions
* @param x X coordinate to test
* @param z Z coordinate to test
* @param y Y coordinate to test
* @return Result to the collision test
*/
bool Crate::isCollision(float x, float z, float y)
{
	return (y >= 0 && y <= 2 * this->crateType);
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
void Crate::impactGetLocation(float x1, float y1, float z1, float x2, float y2, float z2, float &impactX, float &impactY, float &impactZ, float &impactAngleX, float &impactAngleY, float &impactAngleZ, int &modif, float impactRadius)
{
	// The object to test is at the same height of this object
	if(y1 < 2 * this->crateType)
	{
		// The object to test is from the front of this object
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
			if(impactY < impactRadius)
				impactY = impactRadius;
			if(impactY > 2 * this->crateType - impactRadius)
				impactY = 2 * this->crateType - impactRadius;
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
				if(impactY < impactRadius)
					impactY = impactRadius;
				if(impactY > 2 * this->crateType - impactRadius)
					impactY = 2 * this->crateType - impactRadius;
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
			if(impactY < impactRadius)
				impactY = impactRadius;
			if(impactY > 2 * this->crateType - impactRadius)
				impactY = 2 * this->crateType - impactRadius;
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
				if(impactY < impactRadius)
					impactY = impactRadius;
				if(impactY > 2 * this->crateType - impactRadius)
					impactY = 2 * this->crateType - impactRadius;
			}
		}
	}
	else
	{
		// The object to test is from the top of this object
		impactX = x2;
		impactY = 2 * this->crateType + 0.001f;
		impactZ = z2;
		impactAngleX = 90;
		impactAngleY = 0;
		impactAngleZ = 0;
		modif = IMPACT_REPLACEMENT_Y;

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
