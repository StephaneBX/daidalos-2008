//
//	extensions.h - header file
//
//	David Henry - tfc_duke@club-internet.fr
//


#ifndef		__GL_EXT_H
#define		__GL_EXT_H

bool	InitExtensions( void );


#if defined(__WIN32__) || !defined( GL_VERSION_1_3 )
typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3FVARBPROC) (GLenum target, const GLfloat *v);

extern PFNGLACTIVETEXTUREARBPROC		glActiveTexture;
extern PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2f;
extern PFNGLMULTITEXCOORD2FVARBPROC		glMultiTexCoord2fv;
extern PFNGLMULTITEXCOORD3FARBPROC		glMultiTexCoord3f;
extern PFNGLMULTITEXCOORD3FVARBPROC		glMultiTexCoord3fv;

#define GL_TEXTURE0						0x84C0
#define GL_TEXTURE1						0x84C1
#endif	// GL_VERSION_1_3

#endif	// __GL_EXT_H
