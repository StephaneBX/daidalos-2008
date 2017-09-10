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

#ifndef __VECTOR3D_H
#define __VECTOR3D_H

#include <math.h>

/**
* Vector3d Class
* Handle Vector3d operations
* @author Stephane Baudoux
*/
class Vector3d
{
private:
	float x;
	float y;
	float z;

	friend Vector3d operator+(const Vector3d & vec1, const Vector3d & vec2);
	friend Vector3d operator+(const Vector3d & vec1, const float v);
    friend Vector3d operator-(const Vector3d & vec1, const Vector3d & vec2);
	friend Vector3d operator-(const Vector3d & vec1, const float v);
    friend Vector3d operator*(const Vector3d & vec1, const Vector3d & vec2);
	friend Vector3d operator*(const Vector3d & vec1, const float v);
    friend Vector3d operator/(const Vector3d & vec1, const Vector3d & vec2);
	friend Vector3d operator/(const Vector3d & vec1, const float v);
	
public:
	Vector3d();
	Vector3d(float x, float y, float z);
	Vector3d(const Vector3d & vec);
	Vector3d & operator=(const Vector3d & vec);
	Vector3d & operator+=(const Vector3d & vec);
	Vector3d & operator+=(const float v);
    Vector3d & operator-=(const Vector3d & vec);
	Vector3d & operator-=(const float v);
    Vector3d & operator*=(const Vector3d & vec);
	Vector3d & operator*=(const float v);
    Vector3d & operator/=(const Vector3d & vec);
	Vector3d & operator/=(const float v);
	Vector3d & operator-(const Vector3d & vec);
	Vector3d & operator-();
	const float & operator[](const int i) const;
	float & operator[](const int i);
	float getX();
	float getY();
	float getZ();
	float length();
	inline void normalize();
	void setX(float f);
	void setY(float f);
	void setZ(float f);

	static float dotProduct(const Vector3d & vec1, const Vector3d & vec2);
	static Vector3d crossProduct(const Vector3d & vec1, const Vector3d & vec2);
};

/**
* Normalize the Vector3d
*/
void Vector3d::normalize()
{
	float l = sqrtf((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
	this->x *= l;
	this->y *= l;
	this->z *= l;
}

#endif
