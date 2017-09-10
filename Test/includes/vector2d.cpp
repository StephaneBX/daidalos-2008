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

#include <math.h>
#include "../includes/vector2d.h"

/**
* Constructor
*/
Vector2d::Vector2d()
{
	this->x = 0;
	this->y = 0;
}

/**
* Constructor
* @param x X value
* @param y Y value
*/
Vector2d::Vector2d(float x, float y)
{
	this->x = x;
	this->y = y;
}

/**
* Constructor
* @param vec Vector2d to copy
*/
Vector2d::Vector2d(const Vector2d & vec)
{
	this->x = vec.x;
	this->y = vec.y;
}

/**
* Overloads + operator
*/
Vector2d operator+(const Vector2d & vec1, const Vector2d & vec2)
{
	Vector2d result = vec1;
	return result += vec2;
}

/**
* Overloads - operator
*/
Vector2d operator-(const Vector2d & vec1, const Vector2d & vec2)
{
	Vector2d result = vec1;
	return result -= vec2;
}

/**
* Overloads * operator
*/
Vector2d operator*(const Vector2d & vec1, const Vector2d & vec2)
{
	Vector2d result = vec1;
	return result *= vec2;
}

/**
* Overloads * operator
*/
Vector2d operator*(const Vector2d & vec1, float v)
{
	Vector2d result = vec1;
	return result *= v;
}

/**
* Overloads / operator
*/
Vector2d operator/(const Vector2d & vec1, const Vector2d & vec2)
{
	Vector2d result = vec1;
	return result /= vec2;
}

/**
* Overloads = operator
*/
Vector2d & Vector2d::operator=(const Vector2d & vec)
{
	this->x = vec.x;
	this->y = vec.y;
	return *this;
}

/**
* Overloads += operator
*/
Vector2d & Vector2d::operator+=(const Vector2d & vec)
{
	this->x += vec.x;
	this->y += vec.y;
	return *this;
}

/**
* Overloads += operator
*/
Vector2d & Vector2d::operator+=(const float v)
{
	this->x += v;
	this->y += v;
	return *this;
}

/**
* Overloads -= operator
*/
Vector2d & Vector2d::operator-=(const Vector2d & vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	return *this;
}

/**
* Overloads -= operator
*/
Vector2d & Vector2d::operator-=(const float v)
{
	this->x -= v;
	this->y -= v;
	return *this;
}

/**
* Overloads *= operator
*/
Vector2d & Vector2d::operator*=(const Vector2d & vec)
{
	this->x *= vec.x;
	this->y *= vec.y;
	return *this;
}

/**
* Overloads *= operator
*/
Vector2d & Vector2d::operator*=(const float v)
{
	this->x *= v;
	this->y *= v;
	return *this;
}

/**
* Overloads /= operator
*/
Vector2d & Vector2d::operator/=(const Vector2d & vec)
{
	this->x /= vec.x;
	this->y /= vec.y;
	return *this;
}

/**
* Overloads /= operator
*/
Vector2d & Vector2d::operator/=(const float v)
{
	this->x /= v;
	this->y /= v;
	return *this;
}

/**
* Overloads - operator
*/
Vector2d & Vector2d::operator-(const Vector2d & vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	return *this;
}

/**
* Overloads - operator
*/
Vector2d & Vector2d::operator-()
{
	this->x = -this->x;
	this->y = -this->y;
	return *this;
}

/**
* Overloads [] operator
*/
float & Vector2d::operator [](const int i)
{
	switch(i)
	{
		case 0:
		return this->x;
		break;

		case 1:
		return this->y;
		break;

		default:
		return this->x;
		break;
	}
}

/**
* Overloads [] operator
*/
const float & Vector2d::operator [](const int i) const
{
	switch(i)
	{
		case 0:
		return this->x;
		break;

		case 1:
		return this->y;
		break;

		default:
		return this->x;
		break;
	}
}

/**
* Return the x value
* @return x
*/
float Vector2d::getX()
{
	return this->x;
}

/**
* Return the y value
* @return y
*/
float Vector2d::getY()
{
	return this->y;
}

/**
* Set the x value
* @param f New x value
*/
void Vector2d::setX(float f)
{
	this->x = f;
}

/**
* Return the y value
* @return y
*/
void Vector2d::setY(float f)
{
	this->y = f;
}

/**
* Return the Vector2d's normal
* @return The normal
*/
float Vector2d::normal()
{
	return sqrtf((this->x * this->x) + (this->y * this->y));
}

/**
* Normalize the Vector2d
*/
void Vector2d::normalize()
{
	float length = normal();
	this->x /= length;
	this->y /= length;
}

/**
* Return the sign of the Vector2d
* @return The sign
*/
float Vector2d::sign(const Vector2d & a, const Vector2d & b, const Vector2d & c)
{
	return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
}

/**
* Perform a dot product
* @param v1 First Vector2d
* @param v2 Second Vector2d
* @return The result of the dot product
*/
float Vector2d::dotProduct(const Vector2d & v1, const Vector2d & v2)
{
	return (v1.x*v2.x + v1.y*v2.y);
}

/**
* Perform a cross product
* @param v1 First Vector2d
* @param v2 Second Vector2d
* @return The result of the cross product
*/
float Vector2d::crossProductLength(const Vector2d & v1, const Vector2d & v2)
{
	return (v1.x * v2.y - v1.y * v2.x);
}

/**
* Return if the value is close to zero
* @return Value close to zero
*/
bool Vector2d::isZero(float f) 
{
    return fabs(f) <= 1e-5; 
}

/**
* Return if f1 is lesser but not equal to f2
* @return Result
*/
bool Vector2d::isLess(float f1, float f2) 
{
    return (f1 < f2) && !isZero(f1 - f2); 
}

/**
* Return if f1 is greater but not equal to f2
* @return Result
*/
bool Vector2d::isGreater(float f1, float f2) 
{
    return (f1 > f2) && !isZero(f1 - f2); 
}


/**
* Test the intersection of the Vector2d with a triangle
* @param a First tiangle point
* @param b Second tiangle point
* @param c Third tiangle point
* @param p Point to test
* @return The Result
*/
bool Vector2d::intersectionTrianglePoint(const Vector2d & a, const Vector2d & b, const Vector2d & c, const Vector2d & p)
{
	bool b1, b2, b3;

	b1 = sign(p, a, b) < 0.0f;
	b2 = sign(p, b, c) < 0.0f;
	b3 = sign(p, c, a) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

/**
* Test the intersection of a segment with a triangle
* @param t1 First tiangle point
* @param t2 Second tiangle point
* @param t3 Third tiangle point
* @param s1 First segment point
* @param s2 Second segment point
* @return The Result
*/
bool Vector2d::intersectionTriangleSegment(const Vector2d & t1, const Vector2d & t2, const Vector2d & t3, const Vector2d & s1, const Vector2d & s2)
{
	bool ret = false;

	if(Vector2d::intersectionTrianglePoint(t1,t2,t3,s1) || Vector2d::intersectionTrianglePoint(t1,t2,t3,s2))
		ret = true;

	if(!ret)
		if(Vector2d::intersectionSegmentSegment(t1,t2,s1,s2) || Vector2d::intersectionSegmentSegment(t1,t3,s1,s2) || Vector2d::intersectionSegmentSegment(t2,t3,s1,s2))
			ret = true;

	return ret;
}

/**
* Test the intersection between two lines
* @param a1 First line point
* @param a2 First line point
* @param b1 Second line point
* @param b2 Second line point
* @param outDistLine1 outDistLine1
* @param outDistLine2 outDistLine2
* @return The result
*/
bool Vector2d::linesIntersect(const Vector2d & a1, const Vector2d & a2, const Vector2d & b1, const Vector2d & b2, float & outDistLine1, float & outDistLine2)
{
	outDistLine1 = 999999.0f;
	outDistLine2 = 999999.0f;

	float denominator = ((b2.y-b1.y) * (a2.x-a1.x)) - ((b2.x-b1.x) * (a2.y-a1.y));

    // cancel if parallel or collinear
    if(isZero(denominator))
        return false;

	float numerator1 = ((b2.x-b1.x) * (a1.y-b1.y)) - ((b2.y-b1.y) * (a1.x-b1.x));
	float numerator2 = ((a2.x-a1.x) * (a1.y-b1.y)) - ((a2.y-a1.y) * (a1.x-b1.x));

	outDistLine1 = numerator1 / denominator;
	outDistLine2 = numerator2 / denominator;

	return true;
}

/**
* Test the intersection between two segments
* @param a1 First segment point
* @param a2 First segment point
* @param b1 Second segment point
* @param b2 Second segment point
* @return The result
*/
bool Vector2d::intersectionSegmentSegment(const Vector2d & a1, const Vector2d & a2, const Vector2d & b1, const Vector2d & b2)
{
    float dist1, dist2;
	if(Vector2d::linesIntersect(a1, a2, b1, b2, dist1, dist2))
    {
        return Vector2d::isLess(dist1, 1.0f) && Vector2d::isLess(dist2, 1.0f) && Vector2d::isGreater(dist1, 0.0f) && Vector2d::isGreater(dist2, 0.0f);        
    }
    return false;
}

/**
* Test the intersection between a Vector2d and a rectangle
* @param r1 First rectangle point
* @param r2 First rectangle point
* @param p Vector2d
* @return The result
*/
bool Vector2d::pointInRectangle(const Vector2d & r1, const Vector2d & r2, const Vector2d & p)
{
	return (p.x <= r1.x && p.x >= r2.x && p.y >= r1.y && p.y <= r2.y);
}
