//
//	gl_ext.cpp - source file
//
//	David Henry - tfc_duke@club-internet.fr
//

#ifdef _WIN32
#define	WIN32_LEAN_AND_MEAN
#include	<windows.h>
#endif

#include	<GL/gl.h>
#include	<GL/glu.h>
#include	<iostream>
#include	<SDL/SDL.h>

#include	"../md2/gl_ext.h"

using namespace std;


#ifndef GL_VERSION_1_3
PFNGLACTIVETEXTUREARBPROC				glActiveTexture		= NULL;
PFNGLMULTITEXCOORD2FARBPROC				glMultiTexCoord2f	= NULL;
PFNGLMULTITEXCOORD2FVARBPROC			glMultiTexCoord2fv	= NULL;
PFNGLMULTITEXCOORD3FARBPROC				glMultiTexCoord3f	= NULL;
PFNGLMULTITEXCOORD3FVARBPROC			glMultiTexCoord3fv	= NULL;
#endif	// GL_VERSION_1_3


const GLubyte *glext = NULL;



// ----------------------------------------------
// ExtensionSupported() - test si une extension
// donnée est supportée.
// ----------------------------------------------

static bool ExtensionSupported( const char *szExt )
{
#if defined( GLU_VERSION_1_3 ) && !defined( _WIN32 )
	if( !gluCheckExtension( (GLubyte *)szExt, glext ) )
		return false;
#else
	if( !strstr( (char *)glext, szExt ) )
		return false;
#endif	// GLU_VERSION_1_3

	return true;
}



// ----------------------------------------------
// InitExtensions() - initialise les extensions
// OpenGL nécessaires.
// ----------------------------------------------

bool InitExtensions( void )
{
	glext = (GLubyte *)glGetString( GL_EXTENSIONS );

	if( ExtensionSupported( "GL_ARB_multitexture" ) )
	{
		#ifndef GL_VERSION_1_3
				glActiveTexture			= (PFNGLACTIVETEXTUREARBPROC)	SDL_GL_GetProcAddress( "glActiveTextureARB"		);
				glMultiTexCoord2f		= (PFNGLMULTITEXCOORD2FARBPROC)	SDL_GL_GetProcAddress( "glMultiTexCoord2fARB"	);
				glMultiTexCoord2fv		= (PFNGLMULTITEXCOORD2FVARBPROC)SDL_GL_GetProcAddress( "glMultiTexCoord2fvARB"	);
				glMultiTexCoord3f		= (PFNGLMULTITEXCOORD3FARBPROC)	SDL_GL_GetProcAddress( "glMultiTexCoord3fARB"	);
				glMultiTexCoord3fv		= (PFNGLMULTITEXCOORD3FVARBPROC)SDL_GL_GetProcAddress( "glMultiTexCoord3fvARB"	);
		#endif	// GL_VERSION_1_3
	}
	else
	{
		cout << "Multitexturing not supported! (GL_ARB_multitexture not found)" << endl;
		return false;
	}

	return true;
}
