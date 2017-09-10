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
#include "../elements/block.h"

/**
* Constructor
* @param z Z coordinate
* @param x X coordinate
* @param type Block type
* @param envNumber Environment number
* @param objectNumber GList number
* @param ambient_light The default ambient light value
*/
Block::Block(int z, int x, int type, GLuint envNumber, int objectNumber, int ambient_light)
{
	// Copy variables
	this->type = type;
	this->x = -x;
	this->z = z;
	this->objectNumber = objectNumber;
	this->envNumber = envNumber;

	// Pointer initialization
	this->leftBox = NULL;
	this->rightBox = NULL;
	this->upBox = NULL;
	this->downBox = NULL;

	this->groundQuality = (float) OptionManager::getInstance()->getGroundQuality();
	this->lightQuality = OptionManager::getInstance()->getLightQuality();

	// Material specifications
	this->matSpec[0] = 0.1f;
	this->matSpec[1] = 0.1f;
	this->matSpec[2] = 0.1f;
	this->matSpec[3] = 1.0f;

	this->matDif[0] = 0.4f;
	this->matDif[1] = 0.4f;
	this->matDif[2] = 0.4f;
	this->matDif[3] = 1.0f;

	this->matAmb[0] = 0.4f * ambient_light / 100.0f;
	this->matAmb[1] = 0.4f * ambient_light / 100.0f;
	this->matAmb[2] = 0.4f * ambient_light / 100.0f;
	this->matAmb[3] = 1.0f;

	// Manager
	texturesManager = CTextureManager::getInstance();
	std::ostringstream stringstream;

	// Get texture path depending on the quality choosen
	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/ceiling" << this->envNumber << ".tga";
	this->ceilingTexture = texturesManager->LoadTexture(stringstream.str());
	stringstream.str("");

	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/ground" << this->envNumber << ".tga";
	this->groundTexture	= texturesManager->LoadTexture(stringstream.str());
	stringstream.str("");

	stringstream << "imgs/" << OptionManager::getInstance()->getTexturesQuality() << "/wall" << this->envNumber << ".tga";
	this->wallTexture = texturesManager->LoadTexture(stringstream.str());

	// Get sound impact
	this->soundM = SoundManager::getInstance();
	this->impactSound = this->soundM->loadSound("sounds/environment/rock_impact.wav");

	calculate();
}

/**
* Destructor
*/
Block::~Block()
{
	if(this->leftBox != NULL)
	{
		delete leftBox;
		this->leftBox = NULL;
	}

	if(this->rightBox != NULL)
	{
		delete rightBox;
		rightBox = NULL;
	}

	if(this->upBox != NULL)
	{
		delete upBox;
		upBox = NULL;
	}

	if(this->downBox != NULL)
	{
		delete downBox;
		downBox = NULL;
	}
}

/**
* Calculate the Block
*/
void Block::calculate()
{
	// Left wall
	if(this->type == 1 ||  this->type == 3 || this->type == 5 || this->type == 8 || this->type == 12 || this->type == 13 || this->type == 14 || this->type == 15)
		leftBox = new Box(BLOCDEFAULTSIZE*this->x,BLOCDEFAULTSIZE*this->z,BOX_TYPE_LEFT,this->wallTexture,objectNumber);

	// Right wall
	if(this->type == 2 ||  this->type == 4 || this->type == 5 || this->type == 10 || this->type == 11 || this->type == 13 || this->type == 14 || this->type == 15)
		rightBox = new Box(BLOCDEFAULTSIZE*(this->x-1),BLOCDEFAULTSIZE*this->z,BOX_TYPE_RIGHT,this->wallTexture,objectNumber + 4);
		
	// Top wall
	if(this->type == 1 ||  this->type == 2 || this->type == 6 || this->type == 7 || this->type == 11 || this->type == 12 || this->type == 14 || this->type == 15)
		upBox = new Box(BLOCDEFAULTSIZE*this->x,BLOCDEFAULTSIZE*(this->z+1),BOX_TYPE_UP,this->wallTexture,objectNumber + 8);
	
	// Bottom wall
	if(this->type == 3 ||  this->type == 4 || this->type == 6 || this->type == 9 || this->type == 11 || this->type == 12 || this->type == 13 || this->type == 15)
		downBox = new Box(BLOCDEFAULTSIZE*this->x,BLOCDEFAULTSIZE*this->z,BOX_TYPE_DOWN,this->wallTexture, objectNumber + 12);

	for(int j = 0; j <= this->lightQuality ; j++)
	{
		// Ground
		this->groundQuality = (float)OptionManager::getInstance()->getGroundQuality(j);
		glNewList(objectNumber + 16 + j,GL_COMPILE); 
		glBindTexture  ( GL_TEXTURE_2D, this->groundTexture );
		glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		for( int i = 0 ; i < this->groundQuality ; i++)
		{
			for( int j = 0 ; j < this->groundQuality ; j++)
			{
				glTexCoord2f(j*2/this->groundQuality		,i*2/this->groundQuality);		glVertex3f(BLOCDEFAULTSIZE * this->x-j/this->groundQuality * BLOCDEFAULTSIZE		, 0	, i/this->groundQuality * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE * this->z);
				glTexCoord2f((j+1)*2/this->groundQuality	,i*2/this->groundQuality);		glVertex3f(BLOCDEFAULTSIZE * this->x-(j+1)/this->groundQuality * BLOCDEFAULTSIZE	, 0	, i/this->groundQuality * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE * this->z);
				glTexCoord2f((j+1)*2/this->groundQuality	,(i+1)*2/this->groundQuality);	glVertex3f(BLOCDEFAULTSIZE * this->x-(j+1)/this->groundQuality * BLOCDEFAULTSIZE	, 0	, (i+1)/this->groundQuality * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE * this->z); 	
				glTexCoord2f(j*2/this->groundQuality		,(i+1)*2/this->groundQuality);	glVertex3f(BLOCDEFAULTSIZE * this->x-j/this->groundQuality * BLOCDEFAULTSIZE		, 0	, (i+1)/this->groundQuality * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE * this->z);
			}
		}
		glEnd();

		// Ceiling
		glBindTexture  ( GL_TEXTURE_2D, this->ceilingTexture );
		glBegin(GL_QUADS);
		glNormal3f(0.0f,-1.0f,0.0f);
		for( int i = 0 ; i < this->groundQuality ; i++)
		{
			for( int j = 0 ; j < this->groundQuality ; j++)
			{
				glTexCoord2f(j*2/this->groundQuality		,(i + 1)*2/this->groundQuality);	glVertex3f(BLOCDEFAULTSIZE * this->x-j/this->groundQuality * BLOCDEFAULTSIZE		, BLOCDEFAULTHEIGHT	, (i + 1)/this->groundQuality * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE * this->z);
				glTexCoord2f((j+1)*2/this->groundQuality	,(i + 1)*2/this->groundQuality);	glVertex3f(BLOCDEFAULTSIZE * this->x-(j+1)/this->groundQuality * BLOCDEFAULTSIZE	, BLOCDEFAULTHEIGHT	, (i + 1)/this->groundQuality * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE * this->z);
				glTexCoord2f((j+1)*2/this->groundQuality	,i*2/this->groundQuality);			glVertex3f(BLOCDEFAULTSIZE * this->x-(j+1)/this->groundQuality * BLOCDEFAULTSIZE	, BLOCDEFAULTHEIGHT	, i/this->groundQuality * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE * this->z); 	
				glTexCoord2f(j*2/this->groundQuality		,i*2/this->groundQuality);			glVertex3f(BLOCDEFAULTSIZE * this->x-j/this->groundQuality * BLOCDEFAULTSIZE		, BLOCDEFAULTHEIGHT	, i/this->groundQuality * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE * this->z);
			}
		}
		glEnd();
		glEndList();
	}
}

/**
* Draw the Block
* The parameters are needed for the LOD
* @param playerX Player's X coordinate
* @param playerZ Player's Z coordinate
*/
void Block::draw(float playerX, float playerZ)
{
	glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
	glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
	glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
	glMaterialf (GL_FRONT,GL_SHININESS,50.0f);
	
	// Left wall
	if(this->type == 1 ||  this->type == 3 || this->type == 5 || this->type == 8 || this->type == 12 || this->type == 13 || this->type == 14 || this->type == 15)
		leftBox->draw(playerX,playerZ);

	// Right wall
	if(this->type == 2 ||  this->type == 4 || this->type == 5 || this->type == 10 || this->type == 11 || this->type == 13 || this->type == 14 || this->type == 15)	
		rightBox->draw(playerX,playerZ);
	
	// Bottom wall
	if(this->type == 1 ||  this->type == 2 || this->type == 6 || this->type == 7 || this->type == 11 || this->type == 12 || this->type == 14 || this->type == 15)
		upBox->draw(playerX,playerZ);
	
	// Top wall
	if(this->type == 3 ||  this->type == 4 || this->type == 6 || this->type == 9 || this->type == 11 || this->type == 12 || this->type == 13 || this->type == 15)
		downBox->draw(playerX,playerZ);

	// Ground + Ceiling
	float dist = sqrt(((BLOCDEFAULTSIZE * this->x - BLOCDEFAULTSIZE / 2.0f) - playerX) * ((BLOCDEFAULTSIZE * this->x - BLOCDEFAULTSIZE / 2.0f) - playerX) + ((BLOCDEFAULTSIZE * this->z + BLOCDEFAULTSIZE / 2.0f) - playerZ) * ((BLOCDEFAULTSIZE * this->z + BLOCDEFAULTSIZE / 2.0f) - playerZ));
	if(dist > VIEWQUALITYMIN0 || this->lightQuality == 0)
		glCallList(this->objectNumber + 16);
	else if(dist > VIEWQUALITYMIN1 || this->lightQuality == 1)
			glCallList(this->objectNumber + 17);
	else if(dist > VIEWQUALITYMIN2 || this->lightQuality == 2)
			glCallList(this->objectNumber + 18);
	else
		glCallList(this->objectNumber + 19);
}

/**
* Play the Impact Sound
* @param volume the volume of the sound
*/
void Block::playSound(int volume)
{
	this->soundM->playSound(this->impactSound, volume);
}

/**
* Verify if the Block has a left Box
* @return If a left Box exists
*/
bool Block::isLeftBoxExists()
{
	return (this->leftBox != NULL);
}

/**
* Verify if the Block has a right Box
* @return If a right Box exists
*/
bool Block::isRightBoxExists()
{
	return (this->rightBox != NULL);
}

/**
* Verify if the Block has an upper Box
* @return If an upper Box exists
*/
bool Block::isUpBoxExists()
{
	return (this->upBox != NULL);
}

/**
* Verify if the Block has a down Box
* @return If a down Box exists
*/
bool Block::isDownBoxExists()
{
	return (this->downBox != NULL);
}

/**
* Verify if the Block has a left Portal
* @return If a left Portal exists
*/
bool Block::isLeftPortalExists()
{
	return !isLeftBoxExists();
}

/**
* Verify if the Block has a right Portal
* @return If a right Portal exists
*/
bool Block::isRightPortalExists()
{
	return !isRightBoxExists();
}

/**
* Verify if the Block has an upper Portal
* @return If an upper Portal exists
*/
bool Block::isUpPortalExists()
{
	return !isUpBoxExists();
}

/**
* Verify if the Block has a down Portal
* @return If a down Portal exists
*/
bool Block::isDownPortalExists()
{
	return !isDownBoxExists();
}

/**
* Check the intersection of a segment of the walls of the Block
* @param s1 Segment first point
* @param s2 Segment second point
* @return If an intersection is found
*/
bool Block::checkIntersection(const Vector2d & s1, const Vector2d & s2)
{
	return	(Vector2d::intersectionSegmentSegment(s1,s2,Vector2d((float)this->x * BLOCDEFAULTSIZE,(float)this->z * BLOCDEFAULTSIZE), Vector2d((float)this->x * BLOCDEFAULTSIZE - BLOCDEFAULTSIZE, (float)this->z * BLOCDEFAULTSIZE)) && isDownBoxExists()) || 
			(Vector2d::intersectionSegmentSegment(s1,s2,Vector2d((float)this->x * BLOCDEFAULTSIZE,(float)this->z * BLOCDEFAULTSIZE), Vector2d((float)this->x * BLOCDEFAULTSIZE, (float)this->z * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE)) && isLeftBoxExists()) || 
			(Vector2d::intersectionSegmentSegment(s1,s2,Vector2d((float)this->x * BLOCDEFAULTSIZE,(float)this->z * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE), Vector2d((float)this->x * BLOCDEFAULTSIZE - BLOCDEFAULTSIZE, (float)this->z * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE)) && isUpBoxExists()) || 
			(Vector2d::intersectionSegmentSegment(s1,s2,Vector2d((float)this->x * BLOCDEFAULTSIZE - BLOCDEFAULTSIZE,(float)this->z * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE), Vector2d((float)this->x * BLOCDEFAULTSIZE - BLOCDEFAULTSIZE, (float)this->z * BLOCDEFAULTSIZE)) && isRightBoxExists());
}

/**
* Return the left Box
* @return A left Box pointer
*/
Box * Block::getLeftBox()
{
	return this->leftBox;
}

/**
* Return the right Box
* @return A right Box pointer
*/
Box * Block::getRightBox()
{
	return this->rightBox;
}

/**
* Return the up Box
* @return A up Box pointer
*/
Box * Block::getUpBox()
{
	return this->upBox;
}

/**
* Return the down Box
* @return A down Box pointer
*/
Box * Block::getDownBox()
{
	return this->downBox;
}

/**
* Return the x coordonate of the Block
* @return X coordinate
*/
int Block::getPositionX()
{
	return this->x;
}

/**
* Return the z coordonate of the Block
* @return Z coordinate
*/
int Block::getPositionZ()
{
	return this->z;
}

/**
* Return the type of the Block
* @return Type
*/
int Block::getType()
{
	return this->type;
}

/**
* Return the environment number
* @return Environment number
*/
int Block::getEnvironment()
{
	return this->envNumber;
}
