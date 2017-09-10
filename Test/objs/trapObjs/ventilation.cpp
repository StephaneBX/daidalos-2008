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
#include "../../manager/particleManager.h"
#include "../../manager/optionManager.h"
#include "../../objs/trapObjs/ventilation.h"

/**
* Constructor
* @param x X coordinate
* @param z Z coordinate
* @param player1 Pointer to the Player
* @param objectNumber GList number
* @param ambientLight Ambient light value
*/
Ventilation::Ventilation(int x, int z, Player * player1, int objectNumber, int ambientLight) : Obj(x,z,player1)
{
	this->objectNumber = objectNumber;
	this->objectTopQuality = (float) OptionManager::getInstance()->getCrateQuality();
	this->lightQuality = OptionManager::getInstance()->getLightQuality();
	this->objectRotation = 250;
	this->rotation = 0;
	ParticleManager::getInstance()->addParticle(PARTICLE_TRAP,Vector3d(this->x - 1,0.3f,this->z + 1),80);

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

	std::ostringstream stringstream;
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/traps/ventilation/ventilation.tga";
	this->textureId = CTextureManager::getInstance()->LoadTexture(stringstream.str());

	stringstream.str("");
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/traps/ventilation/ventilation2.tga";
	this->textureId2 = CTextureManager::getInstance()->LoadTexture(stringstream.str());

	stringstream.str("");
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/traps/ventilation/helix.tga";
	std::string texture = stringstream.str();

	this->helixEntity = ModelManager::getInstance()->loadModel("objs/traps/helix.md2", texture);

	this->sound = this->soundM->loadSound("sounds/environment/metal_impact.wav");
	this->matType = OBJECTMAT_STEEL;

	calculate();
}

/**
* Destructor
*/
Ventilation::~Ventilation(){}

/**
* Refresh the Ventilation
* @param time Elapsed time
*/
void Ventilation::refresh(long time)
{
	this->rotation += time * this->objectRotation / 1000.0f;
	if(this->rotation > 360)
		this->rotation -= 360;
}

/**
* Draw the Ventilation
* @param lightPos Light position
*/
void Ventilation::draw( float * lightPos)
{
	vec3_t light;

	light[0] = lightPos[0];
	light[1] = lightPos[1];
	light[2] = lightPos[2];

	glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
	glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
	glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
	glMaterialf (GL_FRONT,GL_SHININESS,50.0f);

	float dist = sqrt((this->x - player1->getCoor()[0]) * (this->x - player1->getCoor()[0]) + (this->z - player1->getCoor()[2]) * (this->z - player1->getCoor()[2]));

	if(dist < VIEWQUALITYMIN0)
	{
		glPushMatrix();
			glTranslatef(this->x - 1, 0.02f ,this->z + 1);
			glRotatef(this->rotation, 0, 1, 0);
			this->helixEntity->SetScale(0.2f);
			this->helixEntity->DrawEntity( 1, MD2E_DRAWNORMAL | MD2E_TEXTURED, light );
		glPopMatrix();
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	if(dist > VIEWQUALITYMIN0 || this->lightQuality == 0)
		glCallList(this->objectNumber);
	else if(dist > VIEWQUALITYMIN1 || this->lightQuality == 1)
			glCallList(this->objectNumber + 1);
	else if(dist > VIEWQUALITYMIN2 || this->lightQuality == 2)
			glCallList(this->objectNumber + 2);
	else
		glCallList(this->objectNumber + 3);

	glDisable(GL_BLEND);
}

/**
* Return the object type
* @return Object type
*/
int Ventilation::getType()
{
	return OBJECTTYPE_TRAP;
}

/**
* Activate the Ventilation function
* @return If the function is activated
*/
bool Ventilation::useObject()
{
	if( player1->getCoor()[0] < this->x && player1->getCoor()[0] > this->x - 2.f && player1->getCoor()[2] < this->z +2.f && player1->getCoor()[2] > this->z )
		this->player1->takeDamage(15);
	return false;
}

/**
* Calculate the Ventilation
*/
void Ventilation::calculate()
{
	for(int j = 0; j <= this->lightQuality ; j++)
	{
		this->objectTopQuality = (float)OptionManager::getInstance()->getCrateQuality(j);
		glNewList(this->objectNumber + j,GL_COMPILE); 
		glBindTexture(GL_TEXTURE_2D, this->textureId );

		// Bottom
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(x		, 0.0f					, z);
		glTexCoord2f(1.0f, 0.0f);	glVertex3f(x - 2.0f	, 0.0f					, z);
		glTexCoord2f(1.0f, 0.14f);	glVertex3f(x - 2.0f	, MORTALSMOKEBLOCHEIGHT	, z); 	
		glTexCoord2f(0.0f, 0.14f);	glVertex3f(x		, MORTALSMOKEBLOCHEIGHT	, z);

		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.0f ,0.0f);	glVertex3f(x - 2.0f	, 0.0f					, z + 0.01f);
		glTexCoord2f(0.0f ,0.0f);	glVertex3f(x		, 0.0f					, z + 0.01f);
		glTexCoord2f(0.0f ,0.14f);	glVertex3f(x		, MORTALSMOKEBLOCHEIGHT	, z + 0.01f); 	
		glTexCoord2f(1.0f ,0.14f);	glVertex3f(x - 2.0f	, MORTALSMOKEBLOCHEIGHT	, z + 0.01f);

		// Top
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.0f ,0.106f);	glVertex3f(x - 2.0f	, 0.0f					, z + 2);
		glTexCoord2f(0.0f ,0.106f);	glVertex3f(x		, 0.0f					, z + 2);
		glTexCoord2f(0.0f ,0.14f);	glVertex3f(x		, MORTALSMOKEBLOCHEIGHT	, z + 2); 	
		glTexCoord2f(1.0f ,0.14f);	glVertex3f(x - 2.0f	, MORTALSMOKEBLOCHEIGHT	, z + 2);

		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.106f);	glVertex3f(x		, 0.0f					, z + 1.99f);
		glTexCoord2f(1.0f, 0.106f);	glVertex3f(x - 2.0f	, 0.0f					, z + 1.99f);
		glTexCoord2f(1.0f, 0.14f);	glVertex3f(x - 2.0f	, MORTALSMOKEBLOCHEIGHT	, z + 1.99f); 	
		glTexCoord2f(0.0f, 0.14f);	glVertex3f(x		, MORTALSMOKEBLOCHEIGHT	, z + 1.99f);

		// Left
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.106f);	glVertex3f(x, 0.0f					, z + 2.0f);
		glTexCoord2f(0.0f, 0.106f);	glVertex3f(x, 0.0f					, z);
		glTexCoord2f(0.0f, 0.14f);	glVertex3f(x, MORTALSMOKEBLOCHEIGHT	, z); 	
		glTexCoord2f(1.0f, 0.14f);	glVertex3f(x, MORTALSMOKEBLOCHEIGHT	, z + 2.0f);

		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.106f);	glVertex3f(x - 0.001f, 0.0f					, z);
		glTexCoord2f(1.0f, 0.106f);	glVertex3f(x - 0.001f, 0.0f					, z + 2.0f);
		glTexCoord2f(1.0f, 0.14f);	glVertex3f(x - 0.001f, MORTALSMOKEBLOCHEIGHT	, z + 2.0f); 	
		glTexCoord2f(0.0f, 0.14f);	glVertex3f(x - 0.001f, MORTALSMOKEBLOCHEIGHT	, z);

		// Right
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.106f);	glVertex3f(x - 2, 0.0f					, z);
		glTexCoord2f(1.0f, 0.106f);	glVertex3f(x - 2, 0.0f					, z + 2.0f);
		glTexCoord2f(1.0f, 0.14f);	glVertex3f(x - 2, MORTALSMOKEBLOCHEIGHT	, z + 2.0f); 	
		glTexCoord2f(0.0f, 0.14f);	glVertex3f(x - 2, MORTALSMOKEBLOCHEIGHT	, z);

		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.106f);	glVertex3f(x - 1.99f, 0.0f					, z + 2.0f);
		glTexCoord2f(0.0f, 0.106f);	glVertex3f(x - 1.99f, 0.0f					, z);
		glTexCoord2f(0.0f, 0.14f);	glVertex3f(x - 1.99f, MORTALSMOKEBLOCHEIGHT	, z); 	
		glTexCoord2f(1.0f, 0.14f);	glVertex3f(x - 1.99f, MORTALSMOKEBLOCHEIGHT	, z + 2.0f);
		glEnd();

		// Black part
		glBindTexture(GL_TEXTURE_2D, this->textureId2);
		glBegin(GL_QUADS);
			glNormal3f(0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);	glVertex3f(x		, 0.01f	, z);
			glTexCoord2f(1.0f, 0.0f);	glVertex3f(x - 2.0f	, 0.01f	, z);
			glTexCoord2f(1.0f, 0.0f);	glVertex3f(x - 2.0f	, 0.01f	, z + 2.0f); 	
			glTexCoord2f(0.0f, 0.0f);	glVertex3f(x		, 0.01f	, z + 2.0f);
		glEnd();

		// Grid
		glBindTexture(GL_TEXTURE_2D, this->textureId );
		glBegin(GL_QUADS);
		for( int i = 0 ; i < this->objectTopQuality ; i++)
		{
			for( int j = 0 ; j < this->objectTopQuality ; j++)
			{
				glNormal3f(0.0f, 1.0f, 0.0f);
				glTexCoord2f(i/this->objectTopQuality			,j/this->objectTopQuality);			glVertex3f(x - (2 * i/this->objectTopQuality)		, MORTALSMOKEBLOCHEIGHT	, z + (2 * j/this->objectTopQuality));
				glTexCoord2f((i + 1)/this->objectTopQuality		,j/this->objectTopQuality);			glVertex3f(x - (2 * (i + 1)/this->objectTopQuality)	, MORTALSMOKEBLOCHEIGHT	, z + (2 * j/this->objectTopQuality));
				glTexCoord2f((i + 1)/this->objectTopQuality		,(j + 1)/this->objectTopQuality);	glVertex3f(x - (2 * (i + 1)/this->objectTopQuality)	, MORTALSMOKEBLOCHEIGHT	, z + (2 * (j + 1)/this->objectTopQuality)); 	
				glTexCoord2f(i/this->objectTopQuality			,(j + 1)/this->objectTopQuality);	glVertex3f(x - (2 * i/this->objectTopQuality)		, MORTALSMOKEBLOCHEIGHT	, z + (2 * (j + 1)/this->objectTopQuality));
				
			}
		}
		glEnd();

		glEndList();
	}
}

/**
* Detect collisions
* @param x X coordinate to test
* @param z Z coordinate to test
* @param y Y coordinate to test
* @return Result to the collsion test
*/
bool Ventilation::isCollision(float x, float z, float y)
{
	return y <= MORTALSMOKEBLOCHEIGHT;
}
