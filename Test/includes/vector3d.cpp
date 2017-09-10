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

#include "../includes/vector3d.h"

/**
* Constructor
*/
Vector3d::Vector3d()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

/**
* Constructor
* @param x X value
* @param y Y value
* @param z Z value
*/
Vector3d::Vector3d(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

/**
* Constructor
* @param vec Vector3d to copy
*/
Vector3d::Vector3d(const Vector3d & vec)
{
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
}

/**
* Overloads + operator
*/
Vector3d operator+(const Vector3d & vec1, const Vector3d & vec2)
{
	Vector3d result = vec1;
	return result += vec2;
}

/**
* Overloads + operator
*/
Vector3d operator+(const Vector3d & vec1, const float v)
{
	Vector3d result = vec1;
	return result += v;
}

/**
* Overloads - operator
*/
Vector3d operator-(const Vector3d & vec1, const Vector3d & vec2)
{
	Vector3d result = vec1;
	return result -= vec2;
}

/**
* Overloads - operator
*/
Vector3d operator-(const Vector3d & vec1, const float v)
{
	Vector3d result = vec1;
	return result -= v;
}

/**
* Overloads * operator
*/
Vector3d operator*(const Vector3d & vec1, const Vector3d & vec2)
{
	Vector3d result = vec1;
	return result *= vec2;
}

/**
* Overloads * operator
*/
Vector3d operator*(const Vector3d & vec1, const float v)
{
	Vector3d result = vec1;
	return result *= v;
}

/**
* Overloads / operator
*/
Vector3d operator/(const Vector3d & vec1, const Vector3d & vec2)
{
	Vector3d result = vec1;
	return result /= vec2;
}

/**
* Overloads / operator
*/
Vector3d operator/(const Vector3d & vec1, const float v)
{
	Vector3d result = vec1;
	return result /= v;
}

/**
* Overloads = operator
*/
Vector3d & Vector3d::operator=(const Vector3d & vec)
{
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
	return *this;
}

/**
* Overloads = operator
*/
Vector3d & Vector3d::operator+=(const Vector3d & vec)
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;
	return *this;
}

/**
* Overloads += operator
*/
Vector3d & Vector3d::operator+=(const float v)
{
	this->x += v;
	this->y += v;
	this->z += v;
	return *this;
}

/**
* Overloads -= operator
*/
Vector3d & Vector3d::operator-=(const Vector3d & vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	this->z -= vec.z;
	return *this;
}

/**
* Overloads -= operator
*/
Vector3d & Vector3d::operator-=(const float v)
{
	this->x -= v;
	this->y -= v;
	this->z -= v;
	return *this;
}

/**
* Overloads *= operator
*/
Vector3d & Vector3d::operator*=(const Vector3d & vec)
{
	this->x *= vec.x;
	this->y *= vec.y;
	this->z *= vec.z;
	return *this;
}

/**
* Overloads *= operator
*/
Vector3d & Vector3d::operator*=(const float v)
{
	this->x *= v;
	this->y *= v;
	this->z *= v;
	return *this;
}

/**
* Overloads /= operator
*/
Vector3d & Vector3d::operator/=(const Vector3d & vec)
{
	this->x /= vec.x;
	this->y /= vec.y;
	this->z /= vec.z;
	return *this;
}

/**
* Overloads /= operator
*/
Vector3d & Vector3d::operator/=(const float v)
{
	this->x /= v;
	this->y /= v;
	this->z /= v;
	return *this;
}

/**
* Overloads - operator
*/
Vector3d & Vector3d::operator-(const Vector3d & vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	this->z -= vec.z;
	return *this;
}

/**
* Overloads - operator
*/
Vector3d & Vector3d::operator-()
{
	this->x = -this->x;
	this->y = -this->y;
	this->z = -this->z;
	return *this;
}

/**
* Overloads [] operator
*/
float & Vector3d::operator [](const int i)
{
	switch(i)
	{
		case 0:
		return this->x;
		break;

		case 1:
		return this->y;
		break;

		case 2:
		return this->z;
		break;

		default:
		return this->x;
		break;
	}
}

/**
* Overloads [] operator
*/
const float & Vector3d::operator [](const int i) const
{
	switch(i)
	{
		case 0:
		return this->x;
		break;

		case 1:
		return this->y;
		break;

		case 2:
		return this->z;
		break;
	}
	return this->x;
}

/**
* Perform a dot product
* @param vec1 First Vector3d
* @param vec2 Second Vector3d
* @return The dot product
*/
float Vector3d::dotProduct(const Vector3d &vec1, const Vector3d &vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

/**
* Perform a dot product
* @param vec1 First Vector3d
* @param vec2 Second Vector3d
* @return Vector3d with the cross product
*/
Vector3d Vector3d::crossProduct(const Vector3d &vec1, const Vector3d &vec2)
{
	return Vector3d(vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x);
}

/**
* Return the X value
* @return x
*/
float Vector3d::getX()
{
	return this->x;
}

/**
* Return the Y value
* @return y
*/
float Vector3d::getY()
{
	return this->y;
}

/**
* Return the Z value
* @return z
*/
float Vector3d::getZ()
{
	return this->z;
}

/**
* Return the Vector3d length
* @return The length
*/
float Vector3d::length()
{
	return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

/**
* Set the X value
* @param f The new X value
*/
void Vector3d::setX(float f)
{
	this->x = f;
}

/**
* Set the Y value
* @param f The new Y value
*/
void Vector3d::setY(float f)
{
	this->y = f;
}

/**
* Set the Z value
* @param f The new Z value
*/
void Vector3d::setZ(float f)
{
	this->z = f;
}
