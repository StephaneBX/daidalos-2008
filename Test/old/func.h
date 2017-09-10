#ifndef __FUNC_H
#define __FUNC_H

#include <SDL.h>
#include "../includes/vec3.h"

class func
{
private:
	static func * function;

public:
	static func * getInstance();
	bool sameSide(vec3 p1, vec3 p2, vec3 a, vec3 b);
	bool pointInTriangle(vec3 p, vec3 a, vec3 b, vec3 c);
	int findGreater(int a, int b, int c, int d);

private:
		func();
};

func * func::function = NULL;

func::func(){}

func * func::getInstance()
{
	if(function == NULL)
		function = new func();
	return function;
}

bool func::sameSide(vec3 p1, vec3 p2, vec3 a, vec3 b)
{
	bool ret = false;
	vec3 cp1 = crossprod((b - a),(p1 - a));
    vec3 cp2 = crossprod((b - a),(p2 - a));
    if (dotprod(cp1, cp2) >= 0)
		ret = true;
	return ret;
}

bool func::pointInTriangle(vec3 p, vec3 a, vec3 b, vec3 c)
{
	bool ret = false;
	if (sameSide(p,a, b,c) && sameSide(p,b, a,c) && sameSide(p,c, a,b))
		ret = true;
	return ret;
}

int func::findGreater(int a, int b, int c, int d)
{
	int ret;
	if(a > b)
	{	
		if(a > c)
		{
			if(a > d)
				ret = 1;
			else
				ret = 4;
		}
		else
		{
			if(c > d)
				ret = 3;
			else
				ret = 4;
		}
	}
	else
	{
		if(b > c)
		{
			if(b > d)
				ret = 2;
			else
				ret = 4;
		}
		else
		{
			if(c > d)
				ret = 3;
			else
				ret = 4;
		}
	}
	return ret;
}

#endif