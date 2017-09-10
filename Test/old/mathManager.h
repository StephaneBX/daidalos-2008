#ifndef __MATHMANAGER_H
#define __MATHMANAGER_H

class mathManager
{
private:
	static mathManager * mathM;
	SDL_Event event;

public:
	static mathManager * getInstance();
	double dotProduct(double * p1, double * p2);
	double crossProduct(double * p1, double * p2);
	int circleInSegment(double * s1, double * s2, double * c, double r);
	bool pointInRectangle(double * p, double * a, double * b, double * c, double * d);
	int getSide(double * p, double * l1, double * l2);
	bool pointInTriangle(double * p, double * a, double * b, double * c);
	double * createPoint(double x, double y);

private:
	mathManager();
};

mathManager * mathManager::mathM = NULL;

mathManager::mathManager(){}

mathManager * mathManager::getInstance()
{
	if(mathM == NULL)
		mathM = new mathManager();
	return mathM;
}

double mathManager::dotProduct(double *p1, double *p2)
{
	return (p1[0]*p2[0] + p1[1]*p2[1]);
}

double mathManager::crossProduct(double *p1, double *p2)
{
	return (p1[0] * p2[1] - p1[1] * p2[0]);
}

int mathManager::circleInSegment(double *s1, double *s2, double *c, double r)
{
	double Alpha, Beta, Gamma;
		    
	Alpha = (s2[0] - s1[0]) * (s2[0] - s1[0]) + (s2[1] - s1[1]) * (s2[1] - s1[1]);
	Beta  = 2 * ((s2[0] - s1[0]) * (s1[0] - c[0]) + (s2[1] - s1[1]) * (s1[1] - c[1]));
	Gamma = s1[0] * s1[0] + s1[1] * s1[1] + c[0] * c[0] + c[1] * c[1] - 2 * (s1[0] * c[0] + s1[1] * c[1]) - r * r;
    
	if((Beta * Beta - 4 * Alpha * Gamma) >= 0)
	{
		double u;
        
		u = ((c[0] - s1[0]) * (s2[0] - s1[0]) + (c[1] - s1[1]) * (s2[1] - s1[1])) / ((s2[0] - s1[0]) * (s2[0] - s1[0]) + (s2[1] - s1[1]) * (s2[1] - s1[1]));
		return 0.0f <= u && u <= 1.0f;
	} 
	else
		return 0;
}

int mathManager::getSide(double * p, double * l1, double * l2)
{
	int ret = false;
	double n = p[0] * (l1[1] - l2[1]) + l1[0] * (l2[1] - p[1]) + l2[0] * (p[1] - l1[1]);

	if ( n > 0 )
		ret = 1;
	else 
		if ( n < 0 ) 
			ret = -1;
		else 
			ret = 0;

	return ret;
}

bool mathManager::pointInTriangle(double *p, double *a, double *b, double *c)
{
	int pab = getSide( p, a, b );
	int pbc = getSide( p, b, c );
	int pca = getSide( p, c, a );

	if ( (0 < pab) && (0 < pbc) && (0 < pca) )
	return true;
	if ( (0 > pab) && (0 > pbc) && (0 > pca) )
	return true;

	if ( (0 <= pab) && (0 <= pbc) && (0 <= pca) )
	return false;
	if ( (0 >= pab) && (0 >= pbc) && (0 >= pca) )
	return false;

	return false;
}

bool mathManager::pointInRectangle(double *p, double *a, double *b, double *c, double *d)
{
	return (p[0] <= a[0] && p[0] >= b[0] && p[1] >= c[1] && p[1] <= a[1]);
}

double * mathManager::createPoint(double x, double z)
{
	double * a;
	a = new double[2];
	a[0] = x;
	a[1] = z;

	return a;
}

#endif