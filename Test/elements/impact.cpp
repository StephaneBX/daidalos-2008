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

#include <GL/gl.h>
#include "../elements/impact.h"

/**
* Constructor
* @param impactId Impact texture id loaded in OpenGL
* @param size Size of the Impact
* @param coor Coordinates
* @param angle Angles
*/
Impact::Impact(int impactId, float size, Vector3d coor, Vector3d angle)
{
	this->impactId = impactId;
	this->coor[0] = coor[0];
	this->coor[1] = coor[1];
	this->coor[2] = coor[2];
	this->angle[0] = angle[0];
	this->angle[1] = angle[1];
	this->angle[2] = angle[2];
	this->size = size;

	this->matSpec[0] = 0.0f;
	this->matSpec[1] = 0.0f;
	this->matSpec[2] = 0.0f;
	this->matSpec[3] = 1.0f;

	this->matDif[0] = 0.8f;
	this->matDif[1] = 0.8f;
	this->matDif[2] = 0.8f;
	this->matDif[3] = 1.0f;

	this->matAmb[0] = 0.1f;
	this->matAmb[1] = 0.1f;
	this->matAmb[2] = 0.1f;
	this->matAmb[3] = 1.00f;
}

/**
* Destructor
*/
Impact::~Impact(){}

/**
* Draw the Impact
*/
void Impact::draw()
{
	glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec); 
	glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
	glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
	glMaterialf (GL_FRONT,GL_SHININESS,100.0f);

	glPushMatrix();
		glTranslatef(this->coor[0],this->coor[1],this->coor[2]);
		glRotatef(this->angle[2],0,0,1);
		glRotatef(this->angle[1],0,1,0);
		glRotatef(this->angle[0],1,0,0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture  ( GL_TEXTURE_2D, this->impactId );
		glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0f,1.0f);	glVertex3f(this->size	, this->size	, 0);
			glTexCoord2f(0.0f,0.0f);	glVertex3f(this->size	, -this->size	, 0);
			glTexCoord2f(1.0f,0.0f);	glVertex3f(- this->size	, -this->size	, 0);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(- this->size	, this->size	, 0);
		glEnd();
		glDisable(GL_BLEND);
	glPopMatrix();
}

/**
* Set the i coordinate
* @param i Coordinate type
* @param v Value
*/
void Impact::setCoor(int i, float v)
{
	this->coor[i] = v;
}

/**
* Return the Impact size
* @return Size
*/
float Impact::getSize()
{
	return this->size;
}

/**
* Return the coordinates
* @return Coordinates
*/
Vector3d Impact::getCoor()
{
	return this->coor;
}
