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

#ifndef __VECTOR2D_H
#define __VECTOR2D_H

#define EPSILON 0.001f

/**
* Vector2d Class
* Handle Vector2d operations
* @author Stephane Baudoux
*/
class Vector2d
{
private:
	float x;
	float y;

	friend Vector2d operator+(const Vector2d & vec1, const Vector2d & vec2);
    friend Vector2d operator-(const Vector2d & vec1, const Vector2d & vec2);
    friend Vector2d operator*(const Vector2d & vec1, const Vector2d & vec2);
	friend Vector2d operator*(const Vector2d & vec1, const float v);
    friend Vector2d operator/(const Vector2d & vec1, const Vector2d & vec2);
	
public:
	Vector2d();
	Vector2d(float x, float y);
	Vector2d(const Vector2d & vec);
	Vector2d & operator=(const Vector2d & vec);
	Vector2d & operator+=(const Vector2d & vec);
	Vector2d & operator+=(const float v);
    Vector2d & operator-=(const Vector2d & vec);
	Vector2d & operator-=(const float v);
    Vector2d & operator*=(const Vector2d & vec);
	Vector2d & operator*=(const float v);
    Vector2d & operator/=(const Vector2d & vec);
	Vector2d & operator/=(const float v);
	Vector2d & operator-(const Vector2d & vec);
	Vector2d & operator-();
	const float & operator[](const int i) const;
	float & operator[](const int i);
	float getX();
	float getY();
	void setX(float f);
	void setY(float f);
	void normalize();
	float normal();
	static bool isZero(float f);
	static bool isLess(float f1, float f2 = 0.0f);
	static bool isGreater(float f1, float f2 = 0.0f);
	static float sign(const Vector2d & a, const Vector2d & b, const Vector2d & c);
	static float crossProductLength(const Vector2d & a, const Vector2d & b);
	static float dotProduct(const Vector2d & v1, const Vector2d & v2);
	static bool intersectionTrianglePoint(const Vector2d & a, const Vector2d & b, const Vector2d & c, const Vector2d & p);
	static bool intersectionTriangleSegment(const Vector2d & t1, const Vector2d & t2, const Vector2d & t3, const Vector2d & s1, const Vector2d & s2);
	static bool intersectionSegmentSegment(const Vector2d & a1, const Vector2d & a2, const Vector2d & b1, const Vector2d & b2);
	static bool linesIntersect(const Vector2d & a1, const Vector2d & a2, const Vector2d & b1, const Vector2d & b2, float & outDistLine1, float & outDistLine2);
	static bool pointInRectangle(const Vector2d & r1, const Vector2d & r2, const Vector2d & p);
};

#endif
