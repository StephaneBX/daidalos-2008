//
//	md2.h - header file
//
//	David Henry - tfc_duke@club-internet.fr
//

#ifndef _WIN32
	#include <cstring>
#endif

#include <string>
#include <fstream>
#include "../md2/maths.h"
#include "../manager/texture.h"

#include <GL/glext.h>
#include "../md2/gl_ext.h"
#include "../md2/md2.h"


/////////////////////////////////////////////////
//					CModel					   //
/////////////////////////////////////////////////


// table de 162 vecteurs normaux précalculés
vec3_t CMD2Model::m_kAnorms[ NUMVERTEXNORMALS ] = {
#include	"anorms.h"
};



// ----------------------------------------------
// constructeur - initialisation des variables à 0.
// ----------------------------------------------

CMD2Model::CMD2Model( void )
{
	m_pSkins		= 0;
	m_pTexCoords	= 0;
	m_pTriangles	= 0;
	m_pFrames		= 0;
	m_pGLcmds		= 0;

	m_fScale		= 1.0;

	m_uiTexID		= 0;
	m_uiNormalMap	= 0;

	this->logM = LogManager::getInstance();
	this->optM = OptionManager::getInstance();
}



// ----------------------------------------------
// LoadModel() - charge un modèle à partir d'un
// fichier <.md2>.
// ----------------------------------------------

bool CMD2Model::LoadModel( std::string szFileName )
{
	std::fstream	file;	// fichier


	// tentative d'ouverture du fichier
	file.open(szFileName.c_str(), std::ios::in | std::ios::binary);

	if(file.fail())
	{
		if(this->optM->isDebug())
			this->logM->logIt(std::string("Unable to open md2 file: "),szFileName);
		return false;
	}

	// lecture du header
	file.read( (char *)&m_kHeader, sizeof( md2_header_t ) );

	// vérification de l'authenticité du modèle
	if( (m_kHeader.version != MD2_VERSION) || m_kHeader.ident != MD2_IDENT )
	{
		if(this->optM->isDebug())
			this->logM->logIt(std::string("Invalid md2 file: "),szFileName);
		return false;
	}


	// allocation de mémoire pour les données du modèle
	m_pSkins = new md2_skin_t[ m_kHeader.num_skins ];
	m_pTexCoords = new md2_texCoord_t[ m_kHeader.num_st ];
	m_pTriangles = new md2_triangle_t[ m_kHeader.num_tris ];
	m_pFrames = new md2_frame_t[ m_kHeader.num_frames ];
	m_pGLcmds = new int[ m_kHeader.num_glcmds ];

	// lecture des noms de skins
	file.seekg( m_kHeader.offset_skins, std::ios::beg );
	file.read( (char *)m_pSkins, sizeof( char ) * 68 * m_kHeader.num_skins );

	// lecture des coordonnées de texture
	file.seekg( m_kHeader.offset_st, std::ios::beg );
	file.read( (char *)m_pTexCoords, sizeof( md2_texCoord_t ) * m_kHeader.num_st );

	// lecture des triangles
	file.seekg( m_kHeader.offset_tris, std::ios::beg );
	file.read( (char *)m_pTriangles, sizeof( md2_triangle_t ) * m_kHeader.num_tris );

	// lecture des frames
	file.seekg( m_kHeader.offset_frames, std::ios::beg );

	for( int i = 0; i < m_kHeader.num_frames; i++ )
	{
		// allocation de mémoire pour les vertices
		m_pFrames[i].verts = new md2_vertex_t[ m_kHeader.num_vertices ];

		// lecture des données de la frame
		file.read( (char *)&m_pFrames[i].scale, sizeof( vec3_t ) );
		file.read( (char *)&m_pFrames[i].translate, sizeof( vec3_t ) );
		file.read( (char *)&m_pFrames[i].name, sizeof( char ) * 16 );
		file.read( (char *)m_pFrames[i].verts, sizeof( md2_vertex_t ) * m_kHeader.num_vertices );
	}

	// lecture des commandes opengl
	file.seekg( m_kHeader.offset_glcmds, std::ios::beg );
	file.read( (char *)m_pGLcmds, sizeof( int ) * m_kHeader.num_glcmds );


	// fermeture du fichier
	file.close();

	if(this->optM->isDebug())
		this->logM->logIt(std::string("Loading md2 file: "),szFileName);

	// succès
	return true;
}



// ----------------------------------------------
// LoadTextures() - charge la texture du modèle
// à partir du chemin d'accès spécifié.
// ----------------------------------------------

void CMD2Model::LoadTexture( std::string szFileName )
{
	// pointeur sur le texture manager
	CTextureManager *pTexMgr = CTextureManager::getInstance();

	// chargement de la texture
	m_uiTexID = pTexMgr->LoadTexture( szFileName, true );
}



// ----------------------------------------------
// LoadTextures() - charge la texture du modèle
// à partir du chemin d'accès spécifié.
// ----------------------------------------------

void CMD2Model::LoadNormalMap( std::string szFileName )
{
	// pointeur sur le texture manager
	CTextureManager *pTexMgr = CTextureManager::getInstance();

	// chargement de la texture
	m_uiNormalMap = pTexMgr->LoadTexture( szFileName, true );
}



// ----------------------------------------------
// FreeModel() - libère la mémoire allouée pour
// le modèle.
// ----------------------------------------------

void CMD2Model::FreeModel( void )
{
	this->optM = OptionManager::getInstance();

	if(this->optM->isDebug())
		this->logM->logIt(std::string("Releasing"),std::string("model"));

	if( m_pSkins )
		delete [] m_pSkins;

	if( m_pTexCoords )
		delete [] m_pTexCoords;

	if( m_pTriangles )
		delete [] m_pTriangles;

	if( m_pFrames )
	{
		for( int i = 0; i < m_kHeader.num_frames; i++ )
		{
			if( m_pFrames[i].verts )
				delete [] m_pFrames[i].verts;
		}

		delete [] m_pFrames;
	}

	if( m_pGLcmds )
		delete [] m_pGLcmds;
}



// ----------------------------------------------
// RenderFrame() - dessine le modèle à la frame
// spécifiée.
// ----------------------------------------------

void CMD2Model::RenderFrame( int iFrame )
{
	// calcul de l'index maximum d'une frame du modèle
	int iMaxFrame = m_kHeader.num_frames - 1;

	// vérification de la validité de iFrame
	if( (iFrame < 0) || (iFrame > iMaxFrame) )
		return;


	// activation de la texture du modèle
	glBindTexture( GL_TEXTURE_2D, m_uiTexID );

	// dessin du modèle
	glBegin( GL_TRIANGLES );
		// dessine chaque triangle
		for( int i = 0; i < m_kHeader.num_tris; i++ )
		{
			// dessigne chaque vertex du triangle
			for( int k = 2; k >=0; k-- )
			{
				md2_frame_t *pFrame = &m_pFrames[ iFrame ];
				md2_vertex_t *pVert = &pFrame->verts[ m_pTriangles[i].vertex[k] ];


				// [coordonnées de texture]
				GLfloat s = (GLfloat)m_pTexCoords[ m_pTriangles[i].st[k] ].s / m_kHeader.skinwidth;
				GLfloat t = (GLfloat)m_pTexCoords[ m_pTriangles[i].st[k] ].t / m_kHeader.skinwidth;

				// application des coordonnées de texture
				glTexCoord2f( s, t );

				// [normale]
				vec3_t n;

				memcpy( n, m_kAnorms[ pVert->normalIndex ], sizeof( vec3_t ) );
				vec_RotateX( &n, (float)(-PI/2.0) );
				vec_RotateY( &n, (float)(-PI/2.0) );

				glNormal3fv( n );

				// [vertex]
				vec3_t v;

				// calcul de la position absolue du vertex et redimensionnement
				v[0] = (pFrame->scale[0] * pVert->v[0] + pFrame->translate[0]) * m_fScale;
				v[1] = (pFrame->scale[1] * pVert->v[1] + pFrame->translate[1]) * m_fScale;
				v[2] = (pFrame->scale[2] * pVert->v[2] + pFrame->translate[2]) * m_fScale;

				vec_RotateX( &v, (float)(-PI/2.0) );
				vec_RotateY( &v, (float)(-PI/2.0) );

				glVertex3fv( v );
			}
		}
	glEnd();
}



void CMD2Model::ReverseRenderFrame( int iFrame )
{
	// calcul de l'index maximum d'une frame du modèle
	int iMaxFrame = m_kHeader.num_frames - 1;

	// vérification de la validité de iFrame
	if( (iFrame < 0) || (iFrame > iMaxFrame) )
		return;


	// activation de la texture du modèle
	glBindTexture( GL_TEXTURE_2D, m_uiTexID );

	// dessin du modèle
	glBegin( GL_TRIANGLES );
		// dessine chaque triangle
		for( int i = m_kHeader.num_tris - 1; i >= 0; i-- )
		{
			// dessigne chaque vertex du triangle
			for( int k = 0; k < 3; k++ )
			{
				md2_frame_t *pFrame = &m_pFrames[ iFrame ];
				md2_vertex_t *pVert = &pFrame->verts[ m_pTriangles[i].vertex[k] ];


				// [coordonnées de texture]
				GLfloat s = (GLfloat)m_pTexCoords[ m_pTriangles[i].st[k] ].s / m_kHeader.skinwidth;
				GLfloat t = (GLfloat)m_pTexCoords[ m_pTriangles[i].st[k] ].t / m_kHeader.skinwidth;

				// application des coordonnées de texture
				glTexCoord2f( s, t );

				// [normale]
				vec3_t n;

				memcpy( n, m_kAnorms[ pVert->normalIndex ], sizeof( vec3_t ) );
				vec_RotateX( &n, (float)(-PI/2.0) );
				vec_RotateY( &n, (float)(-PI/2.0) );

				glNormal3fv( n );

				// [vertex]
				vec3_t v;

				// calcul de la position absolue du vertex et redimensionnement
				v[0] = (pFrame->scale[0] * pVert->v[0] + pFrame->translate[0]) * m_fScale;
				v[1] = (pFrame->scale[1] * pVert->v[1] + pFrame->translate[1]) * m_fScale;
				v[2] = (pFrame->scale[2] * pVert->v[2] + pFrame->translate[2]) * m_fScale;

				vec_RotateX( &v, (float)(-PI/2.0) );
				vec_RotateY( &v, (float)(-PI/2.0) );

				glVertex3fv( v );
			}
		}
	glEnd();
}


// ----------------------------------------------
// RenderFrameWithGLcmds() - dessine le modèle à
// la frame spécifiée en utilisant les commandes
// OpenGL.
// ----------------------------------------------

void CMD2Model::RenderFrameWithGLcmds( int iFrame )
{
	// calcul de l'index maximum d'une frame du modèle
	int iMaxFrame = m_kHeader.num_frames - 1;

	// vérification de la validité des indices de frame
	if( (iFrame < 0) || (iFrame > iMaxFrame) )
		return;


	// activation de la texture du modèle
	glBindTexture( GL_TEXTURE_2D, m_uiTexID );

	// pointeur sur les commandes opengl
	int	*pGLCmds = &m_pGLcmds[0];

	// on dessine chaque triangle!
	while( int i = *(pGLCmds++) )
	{
		if( i < 0 )
		{
			glBegin( GL_TRIANGLE_FAN );
			i = -i;
		}
		else
		{
			glBegin( GL_TRIANGLE_STRIP );
		}


		for( /* rien */; i > 0; i--, pGLCmds += 3 )
		{
			// pGLCmds[0] : coordonnée de texture s
			// pGLCmds[1] : coordonnée de texture t
			// pGLCmds[2] : index vertex à dessiner

			md2_glcmd_t *pGLcmd = (md2_glcmd_t *)pGLCmds;
			md2_frame_t *pFrame = &m_pFrames[ iFrame ];
			md2_vertex_t *pVert = &pFrame->verts[ pGLcmd->index ];


			// [coordonnées de texture]
			glTexCoord2f( pGLcmd->s, pGLcmd->t );

			// [normale]
			vec3_t n;

			memcpy( n, m_kAnorms[ pVert->normalIndex ], sizeof( vec3_t ) );
			vec_RotateX( &n, (float)(-PI/2.0) );
			vec_RotateY( &n, (float)(-PI/2.0) );

			glNormal3fv( n );

			// [vertex]
			vec3_t v;

			// calcul de la position absolue du vertex et redimensionnement
			v[0] = (pFrame->scale[0] * pVert->v[0] + pFrame->translate[0]) * m_fScale;
			v[1] = (pFrame->scale[1] * pVert->v[1] + pFrame->translate[1]) * m_fScale;
			v[2] = (pFrame->scale[2] * pVert->v[2] + pFrame->translate[2]) * m_fScale;

			vec_RotateX( &v, (float)(-PI/2.0) );
			vec_RotateY( &v, (float)(-PI/2.0) );

			glVertex3fv( v );
		}

		glEnd();
	}
}



// ----------------------------------------------
// RenderFrameBump() - dessine le modèle à la frame
// spécifiée.
// ----------------------------------------------

void CMD2Model::RenderFrameBump( int iFrame, vec3_t &kObjLightPosition )
{
	// calcul de l'index maximum d'une frame du modèle
	int iMaxFrame = m_kHeader.num_frames - 1;

	// vérification de la validité de iFrame
	if( (iFrame < 0) || (iFrame > iMaxFrame) )
		return;


	/////////////////////////////////////////////
	// bump		: GL_TEXTURE0
	// cube map	: GL_TEXTURE1

	// set up texture environment to do (tex0 dot tex1)*color
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_uiNormalMap );
	glEnable( GL_TEXTURE_2D );

	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB );
	glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE );
	glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE );

	// bind normalisation cube map to texture unit 1
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, CTextureManager::getInstance()->GetNormCubeMapID() );
	glEnable( GL_TEXTURE_CUBE_MAP_ARB );

	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB );
	glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE );
	glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_DOT3_RGB_ARB );
	glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB );
	/////////////////////////////////////////////

	// dessin du modèle
	glBegin( GL_TRIANGLES );
		// dessine chaque triangle
		for( int i = 0; i < m_kHeader.num_tris; i++ )
		{
			vec3_t	v[3];		// vertices du triangle
			vec3_t	normal[3];	// normales des vertices
			vec3_t	tangent;	// tangentes S
			vec3_t	binormal;	// tangentes T
			vec3_t	ppt;		// dérivée
			GLfloat	s[3];		// coordonnées de texture S
			GLfloat	t[3];		// coordonnées de texture T
			int		k;			// variable de contrôle


			// calculs position vertices, normales et coordonnées de texture
			for( k = 0; k < 3; k++ )
			{
				md2_frame_t *pFrame = &m_pFrames[ iFrame ];
				md2_vertex_t *pVert = &pFrame->verts[ m_pTriangles[i].vertex[k] ];


				// [coordonnées de texture]
				s[k] = (GLfloat)m_pTexCoords[ m_pTriangles[i].st[k] ].s / m_kHeader.skinwidth;
				t[k] = (GLfloat)m_pTexCoords[ m_pTriangles[i].st[k] ].t / m_kHeader.skinwidth;

				// [normale]
				memcpy( normal[k], m_kAnorms[ pVert->normalIndex ], sizeof( vec3_t ) );
				vec_RotateX( &normal[k], (float)(-PI/2.0) );
				vec_RotateY( &normal[k], (float)(-PI/2.0) );

				// [vertex]
				// calcul de la position absolue du vertex et redimensionnement
				v[k][0] = (pFrame->scale[0] * pVert->v[0] + pFrame->translate[0]) * m_fScale;
				v[k][1] = (pFrame->scale[1] * pVert->v[1] + pFrame->translate[1]) * m_fScale;
				v[k][2] = (pFrame->scale[2] * pVert->v[2] + pFrame->translate[2]) * m_fScale;

				vec_RotateX( &v[k], (float)(-PI/2.0) );
				vec_RotateY( &v[k], (float)(-PI/2.0) );
			}

			vec3_t dv[2];
			dv[0][0] = v[1][0] - v[0][0];
			dv[0][1] = v[1][1] - v[0][1];
			dv[0][2] = v[1][2] - v[0][2];

			dv[1][0] = v[2][0] - v[0][0];
			dv[1][1] = v[2][1] - v[0][1];
			dv[1][2] = v[2][2] - v[0][2];

			float ds[2];
			ds[0] = s[1] - s[0];
			ds[1] = s[2] - s[0];

			ppt[0] = (ds[0] * dv[1][0]) - (dv[0][0] * ds[1]);
			ppt[1] = (ds[0] * dv[1][1]) - (dv[0][1] * ds[1]);
			ppt[2] = (ds[0] * dv[1][2]) - (dv[0][2] * ds[1]);

			// normalisation
			vec_Normalize( &ppt );

			// calcul de la tangente et de la binormale
			for( k = 0; k < 3; k++ )
			{
				// calcul de la tangente et de la binormale
				float d = vec_DotProduct( ppt, normal[k] );
				tangent[0] = ppt[0] - (d * normal[k][0]);
				tangent[1] = ppt[1] - (d * normal[k][1]);
				tangent[2] = ppt[2] - (d * normal[k][2]);

				// produit vectoriel entre tangent[k] et normal[k]
				binormal[0] = (tangent[0] * normal[k][2]) - (tangent[2] * normal[k][1]);
				binormal[1] = (tangent[2] * normal[k][0]) - (tangent[0] * normal[k][2]);
				binormal[2] = (tangent[0] * normal[k][1]) - (tangent[1] * normal[k][0]);


				// tangent space light
				vec3_t lightVector;
				vec3_t tangentSpaceLight;

				lightVector[0] = kObjLightPosition[0] - v[k][0];
				lightVector[1] = kObjLightPosition[1] - v[k][1];
				lightVector[2] = kObjLightPosition[2] - v[k][2];

				// normalisation
				vec_Normalize( &lightVector );

				tangentSpaceLight[0] = vec_DotProduct( tangent, lightVector );
				tangentSpaceLight[1] = vec_DotProduct( binormal, lightVector );
				tangentSpaceLight[2] = vec_DotProduct( normal[k], lightVector );

				// coordonnées de texture
				glMultiTexCoord2f( GL_TEXTURE0, s[k], t[k] );

				// tangent space light vector
				glMultiTexCoord3fv( GL_TEXTURE1, tangentSpaceLight );

				// position vertex
				glVertex3fv( v[k] );
			}
		}
	glEnd();

	/////////////////////////////////////////////
	// désactivation de l'application de texture
	glActiveTexture( GL_TEXTURE1 );
	glDisable( GL_TEXTURE_CUBE_MAP_ARB );
	glActiveTexture( GL_TEXTURE0 );
	glDisable( GL_TEXTURE_2D );

	// retour au TexEnv par défaut
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	/////////////////////////////////////////////
}



// ----------------------------------------------
// DrawModelItp() - dessine le modèle avec une
// interpolation de deux frames données.
// ----------------------------------------------

void CMD2Model::DrawModelItp( int iFrameA, int iFrameB, float fInterp )
{
	// calcul de l'index maximum d'une frame du modèle
	int iMaxFrame = m_kHeader.num_frames - 1;

	// vérification de la validité des indices de frame
	if( (iFrameA < 0) || (iFrameB < 0) )
		return;

	if( (iFrameA > iMaxFrame) || (iFrameB > iMaxFrame) )
		return;


	// activation de la texture du modèle
	glBindTexture( GL_TEXTURE_2D, m_uiTexID );

	// dessin du modèle
	glBegin( GL_TRIANGLES );
		// dessine chaque triangle
		for( int i = 0; i < m_kHeader.num_tris; i++ )
		{
			// dessigne chaque vertex du triangle
			for( int k = 0; k < 3; k++ )
			{
				md2_frame_t *pFrameA = &m_pFrames[ iFrameA ];
				md2_frame_t *pFrameB = &m_pFrames[ iFrameB ];

				md2_vertex_t *pVertA = &pFrameA->verts[ m_pTriangles[i].vertex[k] ];
				md2_vertex_t *pVertB = &pFrameB->verts[ m_pTriangles[i].vertex[k] ];


				// [coordonnées de texture]
				GLfloat s = (GLfloat)m_pTexCoords[ m_pTriangles[i].st[k] ].s / m_kHeader.skinwidth;
				GLfloat t = (GLfloat)m_pTexCoords[ m_pTriangles[i].st[k] ].t / m_kHeader.skinwidth;

				// application des coordonnées de texture
				glTexCoord2f( s, t );

				// [normale]
				vec3_t normA, normB, n;

				memcpy( normA, m_kAnorms[ pVertA->normalIndex ], 3 * sizeof( float ) );
				memcpy( normB, m_kAnorms[ pVertB->normalIndex ], 3 * sizeof( float ) );

				// interpolation linéaire
				n[0] = normA[0] + fInterp * (normB[0] - normA[0]);
				n[1] = normA[1] + fInterp * (normB[1] - normA[1]);
				n[2] = normA[2] + fInterp * (normB[2] - normA[2]);

				vec_RotateX( &n, (float)(-PI/2.0) );
				vec_RotateY( &n, (float)(-PI/2.0) );

				// spécification de la normale
				glNormal3fv( n );

				// [vertex]
				vec3_t vecA, vecB, v;

				// calcul de la position absolue des vertices
				vecA[0] = pFrameA->scale[0] * pVertA->v[0] + pFrameA->translate[0];
				vecA[1] = pFrameA->scale[1] * pVertA->v[1] + pFrameA->translate[1];
				vecA[2] = pFrameA->scale[2] * pVertA->v[2] + pFrameA->translate[2];

				vecB[0] = pFrameB->scale[0] * pVertB->v[0] + pFrameB->translate[0];
				vecB[1] = pFrameB->scale[1] * pVertB->v[1] + pFrameB->translate[1];
				vecB[2] = pFrameB->scale[2] * pVertB->v[2] + pFrameB->translate[2];

				// interpolation linéaire et redimensionnement
				v[0] = (vecA[0] + fInterp * (vecB[0] - vecA[0])) * m_fScale;
				v[1] = (vecA[1] + fInterp * (vecB[1] - vecA[1])) * m_fScale;
				v[2] = (vecA[2] + fInterp * (vecB[2] - vecA[2])) * m_fScale;

				vec_RotateX( &v, (float)(-PI/2.0) );
				vec_RotateY( &v, (float)(-PI/2.0) );

				// dessin du vertex
				glVertex3fv( v );
			}
		}
	glEnd();
}



// ----------------------------------------------
// DrawModelItpWithGLcmds() - dessine le modèle
// avec une interpolation de deux frames données
// en utilisant les commandes OpenGL.
// ----------------------------------------------

void CMD2Model::DrawModelItpWithGLcmds( int iFrameA, int iFrameB, float fInterp )
{
	// calcul de l'index maximum d'une frame du modèle
	int iMaxFrame = m_kHeader.num_frames - 1;

	// vérification de la validité des indices de frame
	if( (iFrameA < 0) || (iFrameB < 0) )
		return;

	if( (iFrameA > iMaxFrame) || (iFrameB > iMaxFrame) )
		return;


	// activation de la texture du modèle
	glBindTexture( GL_TEXTURE_2D, m_uiTexID );

	// pointeur sur les commandes opengl
	int	*pGLCmds = &m_pGLcmds[0];

	// on dessine chaque triangle!
	while( int i = *(pGLCmds++) )
	{
		if( i < 0 )
		{
			glBegin( GL_TRIANGLE_FAN );
			i = -i;
		}
		else
		{
			glBegin( GL_TRIANGLE_STRIP );
		}


		for( /* rien */; i > 0; i--, pGLCmds += 3 )
		{
			md2_glcmd_t *pGLcmd = (md2_glcmd_t *)pGLCmds;

			md2_frame_t *pFrameA = &m_pFrames[ iFrameA ];
			md2_frame_t *pFrameB = &m_pFrames[ iFrameB ];

			md2_vertex_t *pVertA = &pFrameA->verts[ pGLcmd->index ];
			md2_vertex_t *pVertB = &pFrameB->verts[ pGLcmd->index ];


			// [coordonnées de texture]
			glTexCoord2f( pGLcmd->s, pGLcmd->t );

			// [normale]
			vec3_t normA, normB, n;

			memcpy( normA, m_kAnorms[ pVertA->normalIndex ], 3 * sizeof( float ) );
			memcpy( normB, m_kAnorms[ pVertB->normalIndex ], 3 * sizeof( float ) );

			// interpolation linéaire
			n[0] = normA[0] + fInterp * (normB[0] - normA[0]);
			n[1] = normA[1] + fInterp * (normB[1] - normA[1]);
			n[2] = normA[2] + fInterp * (normB[2] - normA[2]);

			vec_RotateX( &n, (float)(-PI/2.0) );
			vec_RotateY( &n, (float)(-PI/2.0) );

			// spécification de la normale
			glNormal3fv( n );

			// [vertex]
			vec3_t vecA, vecB, v;

			// calcul de la position absolue des vertices
			vecA[0] = pFrameA->scale[0] * pVertA->v[0] + pFrameA->translate[0];
			vecA[1] = pFrameA->scale[1] * pVertA->v[1] + pFrameA->translate[1];
			vecA[2] = pFrameA->scale[2] * pVertA->v[2] + pFrameA->translate[2];

			vecB[0] = pFrameB->scale[0] * pVertB->v[0] + pFrameB->translate[0];
			vecB[1] = pFrameB->scale[1] * pVertB->v[1] + pFrameB->translate[1];
			vecB[2] = pFrameB->scale[2] * pVertB->v[2] + pFrameB->translate[2];

			// interpolation linéaire et redimensionnement
			v[0] = (vecA[0] + fInterp * (vecB[0] - vecA[0])) * m_fScale;
			v[1] = (vecA[1] + fInterp * (vecB[1] - vecA[1])) * m_fScale;
			v[2] = (vecA[2] + fInterp * (vecB[2] - vecA[2])) * m_fScale;

			vec_RotateX( &v, (float)(-PI/2.0) );
			vec_RotateY( &v, (float)(-PI/2.0) );

			// dessin du vertex
			glVertex3fv( v );
		}

		glEnd();
	}
}



// ----------------------------------------------
// DrawModelItpBump() - dessine le modèle avec une
// interpolation de deux frames données.
// ----------------------------------------------

void CMD2Model::DrawModelItpBump( int iFrameA, int iFrameB, float fInterp, vec3_t &kObjLightPosition )
{
	// calcul de l'index maximum d'une frame du modèle
	int iMaxFrame = m_kHeader.num_frames - 1;

	// vérification de la validité des indices de frame
	if( (iFrameA < 0) || (iFrameB < 0) )
		return;

	if( (iFrameA > iMaxFrame) || (iFrameB > iMaxFrame) )
		return;


	/////////////////////////////////////////////
	// bump		: GL_TEXTURE0
	// cube map	: GL_TEXTURE1

	// set up texture environment to do (tex0 dot tex1)*color
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_uiNormalMap );
	glEnable( GL_TEXTURE_2D );

	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB );
	glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE );
	glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE );

	// bind normalisation cube map to texture unit 1
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, CTextureManager::getInstance()->GetNormCubeMapID() );
	glEnable( GL_TEXTURE_CUBE_MAP_ARB );

	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB );
	glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE );
	glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_DOT3_RGB_ARB );
	glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB );
	/////////////////////////////////////////////

	// dessin du modèle
	glBegin( GL_TRIANGLES );
		// dessine chaque triangle
		for( int i = 0; i < m_kHeader.num_tris; i++ )
		{
			vec3_t	v[3];		// vertices du triangle
			vec3_t	normal[3];	// normales des vertices
			vec3_t	tangent;	// tangentes S
			vec3_t	binormal;	// tangentes T
			vec3_t	ppt;		// dérivée
			GLfloat	s[3];		// coordonnées de texture S
			GLfloat	t[3];		// coordonnées de texture T
			int		k;			// variable de contrôle


			// calculs position vertices, normales et coordonnées de texture
			for( k = 0; k < 3; k++ )
			{
				md2_frame_t *pFrameA = &m_pFrames[ iFrameA ];
				md2_frame_t *pFrameB = &m_pFrames[ iFrameB ];

				md2_vertex_t *pVertA = &pFrameA->verts[ m_pTriangles[i].vertex[k] ];
				md2_vertex_t *pVertB = &pFrameB->verts[ m_pTriangles[i].vertex[k] ];


				// [coordonnées de texture]
				s[k] = (GLfloat)m_pTexCoords[ m_pTriangles[i].st[k] ].s / m_kHeader.skinwidth;
				t[k] = (GLfloat)m_pTexCoords[ m_pTriangles[i].st[k] ].t / m_kHeader.skinwidth;

				// [normale]
				vec3_t normA, normB;

				memcpy( normA, m_kAnorms[ pVertA->normalIndex ], 3 * sizeof( float ) );
				memcpy( normB, m_kAnorms[ pVertB->normalIndex ], 3 * sizeof( float ) );

				// interpolation linéaire
				normal[k][0] = normA[0] + fInterp * (normB[0] - normA[0]);
				normal[k][1] = normA[1] + fInterp * (normB[1] - normA[1]);
				normal[k][2] = normA[2] + fInterp * (normB[2] - normA[2]);

				vec_RotateX( &normal[k], (float)(-PI/2.0) );
				vec_RotateY( &normal[k], (float)(-PI/2.0) );

				// [vertex]
				vec3_t vecA, vecB;

				// calcul de la position absolue des vertices
				vecA[0] = pFrameA->scale[0] * pVertA->v[0] + pFrameA->translate[0];
				vecA[1] = pFrameA->scale[1] * pVertA->v[1] + pFrameA->translate[1];
				vecA[2] = pFrameA->scale[2] * pVertA->v[2] + pFrameA->translate[2];

				vecB[0] = pFrameB->scale[0] * pVertB->v[0] + pFrameB->translate[0];
				vecB[1] = pFrameB->scale[1] * pVertB->v[1] + pFrameB->translate[1];
				vecB[2] = pFrameB->scale[2] * pVertB->v[2] + pFrameB->translate[2];

				// interpolation linéaire et redimensionnement
				v[k][0] = (vecA[0] + fInterp * (vecB[0] - vecA[0])) * m_fScale;
				v[k][1] = (vecA[1] + fInterp * (vecB[1] - vecA[1])) * m_fScale;
				v[k][2] = (vecA[2] + fInterp * (vecB[2] - vecA[2])) * m_fScale;

				vec_RotateX( &v[k], (float)(-PI/2.0) );
				vec_RotateY( &v[k], (float)(-PI/2.0) );
			}

			vec3_t dv[2];
			dv[0][0] = v[1][0] - v[0][0];
			dv[0][1] = v[1][1] - v[0][1];
			dv[0][2] = v[1][2] - v[0][2];

			dv[1][0] = v[2][0] - v[0][0];
			dv[1][1] = v[2][1] - v[0][1];
			dv[1][2] = v[2][2] - v[0][2];

			float ds[2];
			ds[0] = s[1] - s[0];
			ds[1] = s[2] - s[0];

			ppt[0] = (ds[0] * dv[1][0]) - (dv[0][0] * ds[1]);
			ppt[1] = (ds[0] * dv[1][1]) - (dv[0][1] * ds[1]);
			ppt[2] = (ds[0] * dv[1][2]) - (dv[0][2] * ds[1]);

			// normalisation
			vec_Normalize( &ppt );


			// dessigne chaque vertex du triangle
			for( k = 0; k < 3; k++ )
			{
				// calcul de la tangente et de la binormale
				float d = vec_DotProduct( ppt, normal[k] );
				tangent[0] = ppt[0] - (d * normal[k][0]);
				tangent[1] = ppt[1] - (d * normal[k][1]);
				tangent[2] = ppt[2] - (d * normal[k][2]);

				// produit vectoriel entre tangent[k] et normal[k]
				binormal[0] = (tangent[0] * normal[k][2]) - (tangent[2] * normal[k][1]);
				binormal[1] = (tangent[2] * normal[k][0]) - (tangent[0] * normal[k][2]);
				binormal[2] = (tangent[0] * normal[k][1]) - (tangent[1] * normal[k][0]);


				// tangent space light
				vec3_t lightVector;
				vec3_t tangentSpaceLight;

				lightVector[0] = kObjLightPosition[0] - v[k][0];
				lightVector[1] = kObjLightPosition[1] - v[k][1];
				lightVector[2] = kObjLightPosition[2] - v[k][2];

				// normalisation
				vec_Normalize( &lightVector );

				tangentSpaceLight[0] = vec_DotProduct( tangent, lightVector );
				tangentSpaceLight[1] = vec_DotProduct( binormal, lightVector );
				tangentSpaceLight[2] = vec_DotProduct( normal[k], lightVector );

				// coordonnées de texture
				glMultiTexCoord2f( GL_TEXTURE0, s[k], t[k] );

				// tangent space light vector
				glMultiTexCoord3fv( GL_TEXTURE1, tangentSpaceLight );

				// position vertex
				glVertex3fv( v[k] );
			}
		}
	glEnd();

	/////////////////////////////////////////////
	// désactivation de l'application de texture
	glActiveTexture( GL_TEXTURE1 );
	glDisable( GL_TEXTURE_CUBE_MAP_ARB );
	glActiveTexture( GL_TEXTURE0 );
	glDisable( GL_TEXTURE_2D );

	// retour au TexEnv par défaut
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	/////////////////////////////////////////////
}



/////////////////////////////////////////////////
//					CEntity					   //
/////////////////////////////////////////////////


// ----------------------------------------------
// constructeur - initialisation des variables à 0.
// ----------------------------------------------

CEntity::CEntity( void )
{
	m_pModel		= 0;

	m_iCurrFrame	= 0;
	m_iNextFrame	= 0;
	m_fInterp		= 0.0;

	m_fPercent		= 0.0;
	m_fScale		= 1.0;
}



// ----------------------------------------------
// DrawEntity() - dessine le modèle de l'entitée.
// si le modèle n'est pas animé, on dessine la
// frame iFrame. Si cette dernière est négative,
// on dessine la frame courante.
// ----------------------------------------------

void CEntity::DrawEntity( int iFrame, int iMode, vec3_t &kObjLightPosition )
{
	// sauvegarde les paramètres OpenGL
	glPushAttrib( GL_ENABLE_BIT | GL_POLYGON_BIT );

	// redimensionnement du modèle
	m_pModel->SetScale( m_fScale );


	if( iMode & MD2E_DRAWBUMP )
	{
		// couleur éclairage par pixel
		float fLightColor[4];
		glGetLightfv( GL_LIGHT0, GL_DIFFUSE, fLightColor );
		glColor4fv( fLightColor );
	}
	else
	{
		glColor4f( 1.0, 1.0, 1.0, 1.0 );
	}


	if( iMode & MD2E_CULLBACKFACE )
	{
		glEnable( GL_CULL_FACE );

		// les triangles sont dessinés dans le sens inverse
		// des aiguilles d'une montre !
		glFrontFace( GL_CW );
		glCullFace( GL_BACK );
	}


	// rendu du modèle
	if( iMode & MD2E_ANIMATED )
	{
		if( iMode & MD2E_DRAWBUMP )
		{
			glDisable( GL_LIGHTING );
			glEnable( GL_POLYGON_OFFSET_FILL );

			glPolygonOffset( 1.0, 1.0 );

			// dessine le modèle avec la normal map
			m_pModel->DrawModelItpBump( m_iCurrFrame, m_iNextFrame, m_fInterp, kObjLightPosition );

			glBlendFunc( GL_DST_COLOR, GL_ZERO );
			glEnable( GL_BLEND );
			glDisable( GL_POLYGON_OFFSET_FILL );
		}

		if( iMode & MD2E_DRAWNORMAL )
		{
			if( iMode & MD2E_TEXTURED )
				glEnable( GL_TEXTURE_2D );

			if( iMode & MD2E_USEGLCMDS )
			{
				// dessine le modèle en utilisant les commandes OpenGL
				m_pModel->DrawModelItpWithGLcmds( m_iCurrFrame, m_iNextFrame, m_fInterp );
			}
			else
			{
				// dessine chaque triangle du modèle
				m_pModel->DrawModelItp( m_iCurrFrame, m_iNextFrame, m_fInterp );
			}
		}

		// incrémentation du pourcentage d'interpolation entre les deux frames
		m_fInterp += m_fPercent;
	}
	else
	{
		if( iMode & MD2E_DRAWBUMP )
		{
			glDisable( GL_LIGHTING );
			glEnable( GL_POLYGON_OFFSET_FILL );

			glPolygonOffset( 1.0, 1.0 );

			// dessine le modèle avec la normal map
			m_pModel->RenderFrameBump( m_iCurrFrame, kObjLightPosition );

			glBlendFunc( GL_DST_COLOR, GL_ZERO );
			glEnable( GL_BLEND );
			glDisable( GL_POLYGON_OFFSET_FILL );
		}

		if( iMode & MD2E_DRAWNORMAL )
		{
			if( iMode & MD2E_TEXTURED )
				glEnable( GL_TEXTURE_2D );

			if( iMode & MD2E_USEGLCMDS )
			{
				// dessine le modèle en utilisant les commandes OpenGL
				if( iFrame < 0 )
					m_pModel->RenderFrameWithGLcmds( m_iCurrFrame );
				else
					m_pModel->RenderFrameWithGLcmds( iFrame );
			}
			else
			{
				if( iMode &MD2E_REVERSE )
				{
					// dessine chaque triangle du modèle
					if( iFrame == -1 )
						m_pModel->ReverseRenderFrame( m_iCurrFrame );
					else
						m_pModel->ReverseRenderFrame( iFrame );
				}
				else
				{
					// dessine chaque triangle du modèle
					if( iFrame == -1 )
						m_pModel->RenderFrame( m_iCurrFrame );
					else
						m_pModel->RenderFrame( iFrame );
				}
			}
		}
	}

	// restaure les paramètres OpenGL
	glPopAttrib();
}



// ----------------------------------------------
// Animate() - calcul les frames courante et
// suivante à partir des frames de début et de
// fin d'animation et du pourcentage d'interpolation.
// ----------------------------------------------

void CEntity::Animate( int iStartFrame, int iEndFrame, float fPercent )
{
	// m_iCurrFrame doit être compris entre iStartFrame et iEndFrame
	if( m_iCurrFrame < iStartFrame )
		m_iCurrFrame = iStartFrame;

	if( m_iCurrFrame > iEndFrame )
		m_iCurrFrame = iStartFrame;

	m_fPercent = fPercent;

	// animation : calcul des frames courante et suivante
	if( m_fInterp >= 1.0 )
	{
		m_fInterp = 0.0f;
		m_iCurrFrame++;

		if( m_iCurrFrame >= iEndFrame )
			m_iCurrFrame = iStartFrame;

		m_iNextFrame = m_iCurrFrame + 1;

		if( m_iNextFrame >= iEndFrame )
			m_iNextFrame = iStartFrame;
	}
}
