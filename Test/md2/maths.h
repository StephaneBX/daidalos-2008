//
//	maths.h - header file
//
//	David Henry - tfc_duke@club-internet.fr
//


#ifndef __MATHS_H
#define __MATHS_H


#include	<math.h>


#ifndef vec3_t
typedef float vec3_t[3];
#endif

#ifndef PI
#	define PI	3.1415926535897932384626433832795
#endif


void vec_RotateX( vec3_t *vec, float fAngle );
void vec_RotateY( vec3_t *vec, float fAngle );
void vec_RotateZ( vec3_t *vec, float fAngle );

void vec_Normalize( vec3_t *vec );

float vec_DotProduct( vec3_t &vec1, vec3_t &vec2 );

#endif	// __MATHS_H

