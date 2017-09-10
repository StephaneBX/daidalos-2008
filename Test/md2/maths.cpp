//
//	maths.cpp - source file
//
//	David Henry - tfc_duke@club-internet.fr
//


#include	<string.h>
#include	"maths.h"



// ----------------------------------------------
// vec_RotateX() - rotation d'un vecteur sur
// l'axe des X (angle en radians).
// ----------------------------------------------

void vec_RotateX( vec3_t *vec, float fAngle )
{
	vec3_t v;
	float fSin = (float)sin( fAngle );
	float fCos = (float)cos( fAngle );

	memcpy( v, vec, sizeof( vec3_t ) );

	(*vec)[0] = v[0];
	(*vec)[1] = (v[1] * fCos) - (v[2] * fSin);
	(*vec)[2] = (v[1] * fSin) + (v[2] * fCos);
}



// ----------------------------------------------
// vec_RotateY() - rotation d'un vecteur sur
// l'axe des Y (angle en radians).
// ----------------------------------------------

void vec_RotateY( vec3_t *vec, float fAngle )
{
	vec3_t v;
	float fSin = (float)sin( fAngle );
	float fCos = (float)cos( fAngle );

	memcpy( v, vec, sizeof( vec3_t ) );

	(*vec)[0] = (v[0] * fCos) + (v[2] * fSin);
	(*vec)[1] = v[1];
	(*vec)[2] = (v[2] * fCos) - (v[0] * fSin);
}



// ----------------------------------------------
// vec_RotateZ() - rotation d'un vecteur sur
// l'axe des Z (angle en radians).
// ----------------------------------------------

void vec_RotateZ( vec3_t *vec, float fAngle )
{
	vec3_t v;
	float fSin = (float)sin( fAngle );
	float fCos = (float)cos( fAngle );

	memcpy( v, vec, sizeof( vec3_t ) );

	(*vec)[0] = (v[0] * fCos) - (v[1] * fSin);
	(*vec)[1] = (v[0] * fSin) + (v[1] * fCos);
	(*vec)[2] = v[2];
}



// ----------------------------------------------
// vec_Normalize() - normalise un vecteur.
// ----------------------------------------------

void vec_Normalize( vec3_t *vec )
{
	float length = (float)sqrt( ((*vec)[0] * (*vec)[0])
				 + ((*vec)[1] * (*vec)[1])
				 + ((*vec)[2] * (*vec)[2]) );

	(*vec)[0] /= length;
	(*vec)[1] /= length;
	(*vec)[2] /= length;
}



// ----------------------------------------------
// vec_DotProduct() - produit scalaire entre deux
// vecteurs.
// ----------------------------------------------

float vec_DotProduct( vec3_t &vec1, vec3_t &vec2 )
{
	return ((vec1[0] * vec2[0]) + (vec1[1] * vec2[1]) + (vec1[2] * vec2[2]));
}
