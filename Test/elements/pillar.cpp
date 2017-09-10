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
#include "../includes/global_var.h"
#include "../manager/optionManager.h"
#include "../elements/pillar.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param pillarTexture The pillar texture
* @param objectNumber GList number
* @param ambientLight Default ambient light value
*/
Pillar::Pillar(float x, float z, int pillarTexture, int objectNumber, int ambientLight)
{
	this->x = -x;
	this->z = z;
	this->objectNumber = objectNumber;
	this->pillarQuality = (float)OptionManager::getInstance()->getPillarQuality();
	this->lightQuality = OptionManager::getInstance()->getLightQuality();

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
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/pillar" << pillarTexture << ".tga";
	this->pillarTextureId= texturesManager->LoadTexture(stringstream.str());

	// Get sound impact
	this->soundM = SoundManager::getInstance();
	this->impactSound = this->soundM->loadSound("sounds/environment/rock_impact.wav");

	calculate();
}

/**
* Destructor
*/
Pillar::~Pillar(){}

/**
* Calculate the Pillar
*/
void Pillar::calculate()
{
	// Calculate all the differents quality pillars
	for(int j = 0; j <= this->lightQuality ; j++)
	{
		this->pillarQuality = (float)OptionManager::getInstance()->getPillarQuality(j);
		glNewList(this->objectNumber + j,GL_COMPILE); 
		glBindTexture  ( GL_TEXTURE_2D, pillarTextureId );
		glBegin(GL_QUADS);
		for(int i = 0 ; i < this->pillarQuality ; i++)
		{
			for(int j = 0 ; j < this->pillarQuality ; j++)
			{
				// Top
				glNormal3f( 0.0f, 0.0f, -1.0f);
				glTexCoord2f(i/this->pillarQuality * 0.25f			,j/this->pillarQuality);		glVertex3f((x + PILLARDEFAULTTICKNESS) - (i/this->pillarQuality * PILLARDEFAULTTICKNESS * 2)		, 0.5f + j/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)		, z-PILLARDEFAULTTICKNESS);
				glTexCoord2f((i + 1)/this->pillarQuality * 0.25f	,j/this->pillarQuality);		glVertex3f((x + PILLARDEFAULTTICKNESS) - ((i + 1)/this->pillarQuality * PILLARDEFAULTTICKNESS * 2)	, 0.5f + j/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)		, z-PILLARDEFAULTTICKNESS);
				glTexCoord2f((i + 1)/this->pillarQuality * 0.25f	,(j + 1)/this->pillarQuality);	glVertex3f((x + PILLARDEFAULTTICKNESS) - ((i + 1)/this->pillarQuality * PILLARDEFAULTTICKNESS * 2)	, 0.5f + (j + 1)/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)	, z-PILLARDEFAULTTICKNESS); 	
				glTexCoord2f(i/this->pillarQuality * 0.25f			,(j + 1)/this->pillarQuality);	glVertex3f((x + PILLARDEFAULTTICKNESS) - (i/this->pillarQuality * PILLARDEFAULTTICKNESS * 2)		, 0.5f + (j + 1)/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)	, z-PILLARDEFAULTTICKNESS);
			
				// Bottom
				glNormal3f( 0.0f, 0.0f, 1.0f);
				glTexCoord2f((i + 1)/this->pillarQuality * 0.25f	,j/this->pillarQuality);		glVertex3f((x + PILLARDEFAULTTICKNESS) - ((i + 1)/this->pillarQuality * PILLARDEFAULTTICKNESS * 2)	, 0.5f + j/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)		, z+PILLARDEFAULTTICKNESS);
				glTexCoord2f(i/this->pillarQuality * 0.25f			,j/this->pillarQuality);		glVertex3f((x + PILLARDEFAULTTICKNESS) - (i/this->pillarQuality * PILLARDEFAULTTICKNESS * 2)		, 0.5f + j/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)		, z+PILLARDEFAULTTICKNESS);
				glTexCoord2f(i/this->pillarQuality * 0.25f			,(j + 1)/this->pillarQuality);	glVertex3f((x + PILLARDEFAULTTICKNESS) - (i/this->pillarQuality * PILLARDEFAULTTICKNESS * 2)		, 0.5f + (j + 1)/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)	, z+PILLARDEFAULTTICKNESS); 	
				glTexCoord2f((i + 1)/this->pillarQuality * 0.25f	,(j + 1)/this->pillarQuality);	glVertex3f((x + PILLARDEFAULTTICKNESS) - ((i + 1)/this->pillarQuality * PILLARDEFAULTTICKNESS * 2)	, 0.5f + (j + 1)/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)	, z+PILLARDEFAULTTICKNESS);

				// Left
				glNormal3f( -1.0f, 0.0f, 0.0f);
				glTexCoord2f(i/this->pillarQuality * 0.25f			,j/this->pillarQuality);		glVertex3f(x-PILLARDEFAULTTICKNESS	, 0.5f + j/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)		, (z - PILLARDEFAULTTICKNESS) + (i/this->pillarQuality * PILLARDEFAULTTICKNESS * 2));
				glTexCoord2f((i + 1)/this->pillarQuality * 0.25f	,j/this->pillarQuality);		glVertex3f(x-PILLARDEFAULTTICKNESS	, 0.5f + j/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)		, (z - PILLARDEFAULTTICKNESS) + ((i + 1)/this->pillarQuality * PILLARDEFAULTTICKNESS * 2));
				glTexCoord2f((i + 1)/this->pillarQuality * 0.25f	,(j + 1)/this->pillarQuality);	glVertex3f(x-PILLARDEFAULTTICKNESS	, 0.5f + (j + 1)/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)	, (z - PILLARDEFAULTTICKNESS) + ((i + 1)/this->pillarQuality * PILLARDEFAULTTICKNESS * 2)); 	
				glTexCoord2f(i/this->pillarQuality * 0.25f			,(j + 1)/this->pillarQuality);	glVertex3f(x-PILLARDEFAULTTICKNESS	, 0.5f + (j + 1)/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)	, (z - PILLARDEFAULTTICKNESS) + (i/this->pillarQuality * PILLARDEFAULTTICKNESS * 2));

				// Right
				glNormal3f( 1.0f, 0.0f, 0.0f);
				glTexCoord2f((i + 1)/this->pillarQuality * 0.25f	,j/this->pillarQuality);		glVertex3f(x+PILLARDEFAULTTICKNESS	, 0.5f + j/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)		, (z - PILLARDEFAULTTICKNESS) + ((i + 1)/this->pillarQuality * PILLARDEFAULTTICKNESS * 2));
				glTexCoord2f(i/this->pillarQuality * 0.25f			,j/this->pillarQuality);		glVertex3f(x+PILLARDEFAULTTICKNESS	, 0.5f + j/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)		, (z - PILLARDEFAULTTICKNESS) + (i/this->pillarQuality * PILLARDEFAULTTICKNESS * 2));
				glTexCoord2f(i/this->pillarQuality * 0.25f			,(j + 1)/this->pillarQuality);	glVertex3f(x+PILLARDEFAULTTICKNESS	, 0.5f + (j + 1)/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)	, (z - PILLARDEFAULTTICKNESS) + (i/this->pillarQuality * PILLARDEFAULTTICKNESS * 2)); 	
				glTexCoord2f((i + 1)/this->pillarQuality * 0.25f	,(j + 1)/this->pillarQuality);	glVertex3f(x+PILLARDEFAULTTICKNESS	, 0.5f + (j + 1)/this->pillarQuality * (PILLARDEFAULTHEIGHT - 0.5f)	, (z - PILLARDEFAULTTICKNESS) + ((i + 1)/this->pillarQuality * PILLARDEFAULTTICKNESS * 2));
			}
		}

		// Bottom down Center
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f,0.1f);	glVertex3f(x+PILLARDEFAULTTICKNESS	, 0		,z-PILLARDEFAULTTICKNESS - 0.3f);
		glTexCoord2f(0.25f,0.1f);	glVertex3f(x-PILLARDEFAULTTICKNESS	, 0		,z-PILLARDEFAULTTICKNESS - 0.3f);
		glTexCoord2f(0.25f,0.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS	, 0.5f	, z-PILLARDEFAULTTICKNESS); 	
		glTexCoord2f(0.0f,0.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS	, 0.5f	, z-PILLARDEFAULTTICKNESS);

		// Bottom Up Center
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f,0.9f);	glVertex3f(x+PILLARDEFAULTTICKNESS	, PILLARDEFAULTHEIGHT - 0.5f	, z-PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.25f,0.9f);	glVertex3f(x-PILLARDEFAULTTICKNESS	, PILLARDEFAULTHEIGHT - 0.5f	, z-PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.25f,1.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS	, PILLARDEFAULTHEIGHT			, z-PILLARDEFAULTTICKNESS - 0.3f); 	
		glTexCoord2f(0.0f,1.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS	, PILLARDEFAULTHEIGHT			, z-PILLARDEFAULTTICKNESS - 0.3f);

		// Right down center
		glNormal3f( -1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.25f,0.1f);	glVertex3f(x-PILLARDEFAULTTICKNESS - 0.3f	, 0			, z-PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.50f,0.1f);	glVertex3f(x-PILLARDEFAULTTICKNESS - 0.3f	, 0			, z+PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.50f,0.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS			, 0.5f		, z+PILLARDEFAULTTICKNESS); 	
		glTexCoord2f(0.25f,0.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS			, 0.5f		, z-PILLARDEFAULTTICKNESS);

		// Right up center
		glNormal3f( -1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.25f,0.9f);	glVertex3f(x-PILLARDEFAULTTICKNESS			, PILLARDEFAULTHEIGHT - 0.5f	, z-PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.50f,0.9f);	glVertex3f(x-PILLARDEFAULTTICKNESS			, PILLARDEFAULTHEIGHT - 0.5f	, z+PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.50f,1.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS - 0.3f	, PILLARDEFAULTHEIGHT			, z+PILLARDEFAULTTICKNESS); 	
		glTexCoord2f(0.25f,1.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS - 0.3f	, PILLARDEFAULTHEIGHT			, z-PILLARDEFAULTTICKNESS);

		// Top down center
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.50f,0.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS	, 0.5f	, z+PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.75f,0.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS	, 0.5f	, z+PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.75f,0.1f);	glVertex3f(x-PILLARDEFAULTTICKNESS	, 0		, z+PILLARDEFAULTTICKNESS + 0.3f); 	
		glTexCoord2f(0.50f,0.1f);	glVertex3f(x+PILLARDEFAULTTICKNESS	, 0		, z+PILLARDEFAULTTICKNESS + 0.3f);

		// Top up center
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.75f,0.9f);	glVertex3f(x-PILLARDEFAULTTICKNESS	, PILLARDEFAULTHEIGHT - 0.5f	, z+PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.50f,0.9f);	glVertex3f(x+PILLARDEFAULTTICKNESS	, PILLARDEFAULTHEIGHT - 0.5f	, z+PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.50f,1.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS	, PILLARDEFAULTHEIGHT			, z+PILLARDEFAULTTICKNESS + 0.3f); 	
		glTexCoord2f(0.75f,1.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS	, PILLARDEFAULTHEIGHT			, z+PILLARDEFAULTTICKNESS + 0.3f);

		// Left down center
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.75f,0.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS			, 0.5f	, z-PILLARDEFAULTTICKNESS);
		glTexCoord2f(1.0f,0.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS			, 0.5f	, z+PILLARDEFAULTTICKNESS);
		glTexCoord2f(1.0f,0.1f);	glVertex3f(x+PILLARDEFAULTTICKNESS + 0.3f	, 0		, z+PILLARDEFAULTTICKNESS); 	
		glTexCoord2f(0.75f,0.1f);	glVertex3f(x+PILLARDEFAULTTICKNESS + 0.3f	, 0		, z-PILLARDEFAULTTICKNESS);

		// Left up center
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f,0.9f);	glVertex3f(x+PILLARDEFAULTTICKNESS			, PILLARDEFAULTHEIGHT - 0.5f	, z+PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.75f,0.9f);	glVertex3f(x+PILLARDEFAULTTICKNESS			, PILLARDEFAULTHEIGHT - 0.5f	, z-PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.75f,1.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS + 0.3f	, PILLARDEFAULTHEIGHT			, z-PILLARDEFAULTTICKNESS); 	
		glTexCoord2f(1.0f,1.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS + 0.3f	, PILLARDEFAULTHEIGHT			, z+PILLARDEFAULTTICKNESS);
		glEnd();

		// Bottom down right triangle
		glBegin(GL_TRIANGLES);
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.25f,0.1f);	glVertex3f(x-PILLARDEFAULTTICKNESS			, 0.5f	, z-PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.20f,0.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS			, 0		, z-PILLARDEFAULTTICKNESS - 0.3f);
		glTexCoord2f(0.30f,0.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS - 0.3f	, 0		, z-PILLARDEFAULTTICKNESS);

		// Bottom up right triangle
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.25f,0.9f);	glVertex3f(x-PILLARDEFAULTTICKNESS			, PILLARDEFAULTHEIGHT - 0.5f	, z-PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.30f,1.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS - 0.3f	, PILLARDEFAULTHEIGHT			, z-PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.20f,1.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS			, PILLARDEFAULTHEIGHT			, z-PILLARDEFAULTTICKNESS - 0.3f);
	
		// Bottom down left triangle
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.9f,0.1f);	glVertex3f(x+PILLARDEFAULTTICKNESS			, 0.5f	, z-PILLARDEFAULTTICKNESS);
		glTexCoord2f(1.0f,0.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS + 0.3f	, 0		, z-PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.8f,0.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS			, 0		, z-PILLARDEFAULTTICKNESS - 0.3f);

		// Bottom up left triangle
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.9f,0.9f);	glVertex3f(x+PILLARDEFAULTTICKNESS			, PILLARDEFAULTHEIGHT - 0.5f	, z-PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.8f,1.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS			, PILLARDEFAULTHEIGHT			, z-PILLARDEFAULTTICKNESS - 0.3f);
		glTexCoord2f(1.0f,1.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS + 0.3f	, PILLARDEFAULTHEIGHT			, z-PILLARDEFAULTTICKNESS);

		// Top down right triangle
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.75f,0.1f);	glVertex3f(x+PILLARDEFAULTTICKNESS			, 0.5f	, z+PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.65f,0.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS			, 0		, z+PILLARDEFAULTTICKNESS + 0.3f);
		glTexCoord2f(0.85f,0.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS + 0.3f	, 0		, z+PILLARDEFAULTTICKNESS);

		// Top up left triangle
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.75f,0.9f);	glVertex3f(x+PILLARDEFAULTTICKNESS			, PILLARDEFAULTHEIGHT - 0.5f	, z+PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.85f,1.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS + 0.3f	, PILLARDEFAULTHEIGHT			, z+PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.95f,1.0f);	glVertex3f(x+PILLARDEFAULTTICKNESS 			, PILLARDEFAULTHEIGHT			, z+PILLARDEFAULTTICKNESS + 0.3f);

		// Top down left triangle
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.50f,0.1f);	glVertex3f(x-PILLARDEFAULTTICKNESS			, 0.5f	, z+PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.60f,0.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS - 0.3f	, 0		, z+PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.40f,0.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS			, 0		, z+PILLARDEFAULTTICKNESS + 0.3f);

		// Top up right triangle
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.50f,0.9f);	glVertex3f(x-PILLARDEFAULTTICKNESS			, PILLARDEFAULTHEIGHT - 0.5f	, z+PILLARDEFAULTTICKNESS);
		glTexCoord2f(0.40f,1.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS			, PILLARDEFAULTHEIGHT			, z+PILLARDEFAULTTICKNESS + 0.3f);
		glTexCoord2f(0.60f,1.0f);	glVertex3f(x-PILLARDEFAULTTICKNESS - 0.3f	, PILLARDEFAULTHEIGHT			, z+PILLARDEFAULTTICKNESS);
		glEnd();
		glEndList();
	}
}

/**
* Draw the Pillar
* The parameters are needed for the LOD
* @param playerX Player X coordinate
* @param playerZ Player Z coordinate
*/
void Pillar::draw(float playerX, float playerZ)
{
	glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
	glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
	glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
	glMaterialf (GL_FRONT,GL_SHININESS,50.0f);

	// Calculate the distance between the object dans the player and draw the appropriate pillar
	float dist = sqrt((this->x - playerX) * (this->x - playerX) + (this->z - playerZ) * (this->z - playerZ));
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
void Pillar::impactGetLocation(float x1, float y1, float z1, float x2, float y2, float z2, float &impactX, float &impactY, float &impactZ, float &impactAngleX, float &impactAngleY, float &impactAngleZ, int &modif, float impactRadius)
{
	// The object to test is from the front of the Pillar
	if(z1 <= this->z - PILLARDEFAULTTICKNESS)
	{
		impactX = x2;
		impactY = y2;
		impactZ = this->z - PILLARDEFAULTTICKNESS - 0.001f;
		impactAngleX = 0;
		impactAngleY = 0;
		impactAngleZ = 0;
		modif = IMPACT_REPLACEMENT_MINUS_Z;

		// Prevent impact from overflowing outside the object
		if(impactX < this->x - PILLARDEFAULTTICKNESS + impactRadius)
			impactX = this->x - PILLARDEFAULTTICKNESS + impactRadius;
		if(impactX > this->x + PILLARDEFAULTTICKNESS - impactRadius)
			impactX = this->x + PILLARDEFAULTTICKNESS - impactRadius;
		if(impactY < impactRadius + 0.5f)
			impactY = impactRadius + 0.5f;
		if(impactY > PILLARDEFAULTHEIGHT - 0.5f - impactRadius)
			impactY = PILLARDEFAULTHEIGHT - 0.5f - impactRadius;
	}
	else
	{
		// The object to test is from behind of the Pillar
		if(z1 >= this->z + PILLARDEFAULTTICKNESS)
		{
			impactX = x2;
			impactY = y2;
			impactZ = this->z + PILLARDEFAULTTICKNESS + 0.001f;
			impactAngleX = 0;
			impactAngleY = 180;
			impactAngleZ = 0;
			modif = IMPACT_REPLACEMENT_Z;

			// Prevent impact from overflowing outside the object
			if(impactX < this->x - PILLARDEFAULTTICKNESS + impactRadius)
				impactX = this->x - PILLARDEFAULTTICKNESS + impactRadius;
			if(impactX > this->x + PILLARDEFAULTTICKNESS - impactRadius)
				impactX = this->x + PILLARDEFAULTTICKNESS - impactRadius;
			if(impactY < impactRadius + 0.5f)
				impactY = impactRadius + 0.5f;
			if(impactY > PILLARDEFAULTHEIGHT - 0.5f - impactRadius)
				impactY = PILLARDEFAULTHEIGHT - 0.5f - impactRadius;
		}
	}

	// The object to test is from the right of the Pillar
	if(x1 <= this->x - PILLARDEFAULTTICKNESS)
	{
		impactX = this->x - PILLARDEFAULTTICKNESS - 0.001f;
		impactY = y2;
		impactZ = z2;
		impactAngleX = 0;
		impactAngleY = 90;
		impactAngleZ = 0;
		modif = IMPACT_REPLACEMENT_MINUS_X;

		// Prevent impact from overflowing outside the object
		if(impactZ < this->z - PILLARDEFAULTTICKNESS + impactRadius)
			impactZ = this->z - PILLARDEFAULTTICKNESS + impactRadius;
		if(impactZ > this->z + PILLARDEFAULTTICKNESS - impactRadius)
			impactZ = this->z + PILLARDEFAULTTICKNESS - impactRadius;
		if(impactY < impactRadius + 0.5f)
			impactY = impactRadius + 0.5f;
		if(impactY > PILLARDEFAULTHEIGHT - 0.5f - impactRadius)
			impactY = PILLARDEFAULTHEIGHT - 0.5f - impactRadius;
	}
	else
	{
		// The object to test is from the left of the Pillar
		if(x1 >= this->x + PILLARDEFAULTTICKNESS)
		{
			impactX = this->x + PILLARDEFAULTTICKNESS + 0.001f;
			impactY = y2;
			impactZ = z2;
			impactAngleX = 0;
			impactAngleY = -90;
			impactAngleZ = 0;
			modif = IMPACT_REPLACEMENT_X;

			// Prevent impact from overflowing outside the object
			if(impactZ < this->z - PILLARDEFAULTTICKNESS + impactRadius)
				impactZ = this->z - PILLARDEFAULTTICKNESS + impactRadius;
			if(impactZ > this->z + PILLARDEFAULTTICKNESS - impactRadius)
				impactZ = this->z + PILLARDEFAULTTICKNESS - impactRadius;
			if(impactY < impactRadius + 0.5f)
				impactY = impactRadius + 0.5f;
			if(impactY > PILLARDEFAULTHEIGHT - 0.5f - impactRadius)
				impactY = PILLARDEFAULTHEIGHT - 0.5f - impactRadius;
		}
	}
}

/**
* Play the Impact Sound
* @param volume Sound volume
*/
void Pillar::playSound(int volume)
{
	this->soundM->playSound(this->impactSound, volume);
}

/**
* Return the GList number
* @return GList number
*/
int Pillar::getObjectNumber()
{
	return this->objectNumber;
}

/**
* Return the Pillar X coordinate
* @return X coordinate
*/
float Pillar::getX()
{
	return this->x;
}

/**
* Return the Pillar Z coordinate
* @return Z coordinate
*/
float Pillar::getZ()
{
	return this->z;
}

/**
* Detect collision between a 3D point and the Pillar
* @param x X point coordinate
* @param y Y point coordinate
* @param z Z point coordinate
*/
bool Pillar::isCollision(float x, float z, float y)
{
	return (x >= this->x - PILLARDEFAULTTICKNESS && x <= this->x + PILLARDEFAULTTICKNESS && y >= 0.5f && y <= PILLARDEFAULTHEIGHT - 0.5f && z >= this->z - PILLARDEFAULTTICKNESS && z <= this->z + PILLARDEFAULTTICKNESS);
}
