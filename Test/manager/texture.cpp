#ifdef _WIN32
#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

#include "../manager/texture.h"
#include "../md2/tga.h"

void CTextureManager::Initialize( void )
{
	// vide la base de données
	ReleaseTextures();

	// c'est la première texture chargée en mémoire. Si
	// une texture ne peut être lue ou chargée, on utilise
	// celle si à la place.

//	std::cout << "CTextureManager : Texture list is empty, creating default texture..." << std::endl;
	GLuint id;

	// création et initialisation de la texture
	glGenTextures( 1, &id );
	glBindTexture( GL_TEXTURE_2D, id );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	// ajout à la base de donnée
	m_kDataBase[ "default" ] = id;

//	std::cout << "CTextureManager : creating default texture, id [" << id << "]" << std::endl;


	// création d'une texture échiquier pour la texture par défaut
	int i, j, c;			// variables de contrôle
	GLubyte *checker;		// données texels

	checker = new GLubyte[ 64 * 64 * 4 ];

	// construction de l'échiquier
	for( i = 0; i < 64; i++ )
	{
		for( j = 0; j < 64; j++ )
		{
			c = ( !(i & 8) ^ !(j & 8)) * 255;

			checker[ (i * 256) + (j * 4) + 0 ] = (GLubyte)c;
			checker[ (i * 256) + (j * 4) + 1 ] = (GLubyte)c;
			checker[ (i * 256) + (j * 4) + 2 ] = (GLubyte)c;
			checker[ (i * 256) + (j * 4) + 3 ] = (GLubyte)255;
		}
	}

	glTexImage2D( GL_TEXTURE_2D, 0, 4, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checker );
	delete [] checker;
}



// ----------------------------------------------
// MakeNormalizeVectorCubeMap() - initialise une
// texture cube map pour bump mapping.
// ----------------------------------------------

void CTextureManager::MakeNormalizeVectorCubeMap( int iSize )
{
	float		vector[3];
	int			i, x, y;
	GLubyte		*pixels;
	GLubyte		*ptr;
	GLuint		id;

	pixels = new GLubyte[ iSize * iSize * 3 ];

	// création et initialisation de la texture
	glGenTextures( 1, &id );
	glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, id );

	glTexParameteri( GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

	m_kDataBase[ "Normalized Cube Map" ] = id;
	m_pNormCubeMapID = &m_kDataBase[ "Normalized Cube Map" ];

//	std::cout << "CTextureManager : building normalized cube map, id [" << id << "]" << std::endl;


	// construit le cube
	for( i = 0; i < 6; i++ )
	{
		ptr = pixels;

		for( y = 0; y < iSize; y++ )
		{
			for( x = 0; x < iSize; x++, ptr += 3 )
			{
				GetCubeVector( i, iSize, x, y, vector );

				ptr[0] = (GLubyte)(128 + (127 * vector[0]));
				ptr[1] = (GLubyte)(128 + (127 * vector[1]));
				ptr[2] = (GLubyte)(128 + (127 * vector[2]));
			}
		}

		// création de la texture cube map de cette face
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT + i, 0, GL_RGB8, iSize, iSize, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );
	}

	delete [] pixels;
}



// ----------------------------------------------
// GetCubeVector() - retourne le vecteur normalisé
// correspondant à la face donnée.
// ----------------------------------------------

void CTextureManager::GetCubeVector( int i, int cubesize, int x, int y, float *vector )
{
	float s, t, sc, tc, mag;

	s = ((float)x + 0.5f) / (float)cubesize;
	t = ((float)y + 0.5f) / (float)cubesize;
	sc = (s * 2.0f) - 1.0f;
	tc = (t * 2.0f) - 1.0f;

	switch( i )
	{
		case 0:
			// x positif
			vector[0] = 1.0;
			vector[1] = -tc;
			vector[2] = -sc;
			break;

		case 1:
			// x négatif
			vector[0] = -1.0;
			vector[1] = -tc;
			vector[2] = sc;
			break;

		case 2:
			// y positif
			vector[0] = sc;
			vector[1] = 1.0;
			vector[2] = tc;
			break;
  
		case 3:
			// y négatif
			vector[0] = sc;
			vector[1] = -1.0;
			vector[2] = -tc;
    		break;

		case 4:
			// z positif
			vector[0] = sc;
			vector[1] = -tc;
			vector[2] = 1.0;
			break;

		case 5:
			// z négatif
			vector[0] = -sc;
			vector[1] = -tc;
			vector[2] = -1.0;
			break;
	}

	// normalisation du vecteur
	mag = 1.0f / sqrt( (vector[0] * vector[0]) + (vector[1] * vector[1]) + (vector[2] * vector[2]) );
	vector[0] *= mag;
	vector[1] *= mag;
	vector[2] *= mag;
}



// ----------------------------------------------
// LoadTexture() - charge une texture. Vérifie
// si elle n'est pas déjà stockée en mémoire. Si
// c'est le cas, la fonction retourne l'id de la
// texture déjà stockée. Sinon, elle retourne
// l'id de la nouvelle texture.
// ----------------------------------------------

GLuint CTextureManager::LoadTexture( std::string szFilename, bool bFlip /* = false */ )
{
	GLuint		id = 0;			// id de la texture à retourner
	GLubyte		*pixels = 0;	// tableau de pixels de la texture
	int			width, height;	// dimensions de la texture
	std::ostringstream stringstream;

	// Verification si les textures custom sont activées
	if(this->modM->getLevelsConfiguration(MOD_CONFIGURATION_LEVELS_TEXTURES))
	{
			std::ifstream file;
			stringstream.str("");
			stringstream << this->modM->getModRootDirectory() << szFilename;
			file.open(stringstream.str().c_str(), std::ios::in);
			if(file)
			{
				szFilename = stringstream.str();
				file.close();
			}
	}

	// on tente d'obtenir la texture depuis la base de données.
	// Si elle a déjà été chargée au préalable, on retourne son
	// id, sinon l'id retournée est celle de la texture par défaut.
	// On tente alors de la charger dans la base.
	id = GetTexture( szFilename );

	if( id != GetTexture( "default" ) )
	{
		// la texture a déjà été chargée
//		std::cout << "CTextureManager : " << szFilename << " is already stored, id [" << id << "]" << std::endl;
	}
	else
	{
		// on tente de charger la texture
		if( LoadFileTGA( szFilename.c_str(), &pixels, &width, &height, bFlip ) > 0 )
		{
			// création et initialisation de la texture opengl
			glGenTextures( 1, &id );
			glBindTexture( GL_TEXTURE_2D, id );
			
			// anisotropic filtering
			if(this->textureFilteringLevel > 0)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, this->textureFilteringLevel);

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			gluBuild2DMipmaps( GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

			// création d'une nouvelle texture et ajout à la base de données
			m_kDataBase[ szFilename ] = id;

			if(this->optM->isDebug())
				this->logM->logIt(std::string("Loading texture: "),szFilename);
		}
		else
		{
			if(this->optM->isDebug())
				this->logM->logIt(std::string("Unable to open the texture: "),szFilename);
		}

		// désallocation de mémoire - OpenGL possède sa propre copie de l'image
		if( pixels )
			delete [] pixels;
	}

	return id;
}



// ----------------------------------------------
// GetTexture() - retourne l'id de la texture
// filename si déjà chargée.
// ----------------------------------------------

GLuint CTextureManager::GetTexture( std::string szFilename )
{
	if( (m_kItor = m_kDataBase.find( szFilename )) != m_kDataBase.end() )
	{
		return (*m_kItor).second;
	}
	else
	{
		return m_kDataBase[ "default" ];
	}
}



// ----------------------------------------------
// GetTextureName() - retourne un pointeur sur le
// nom de l'image d'id spécifiée.
// ----------------------------------------------

char *CTextureManager::GetTextureName( GLuint id )
{
	if( glIsTexture( id ) )
	{
		for( m_kItor = m_kDataBase.begin(); m_kItor != m_kDataBase.end(); ++m_kItor )
		{
			if( (*m_kItor).second == id )
			{
				return (char *)((*m_kItor).first.c_str());
			}
		}
	}

	// la texture n'a pas été trouvée
	return (char *)"failed";
}



// ----------------------------------------------
// ReleaseTextures() - détruit toutes les textures
// stockées dans la base de données.
// ----------------------------------------------

void CTextureManager::ReleaseTextures( void )
{
	
	if(this->optM->isDebug())
		this->logM->logIt(std::string("Releasing"),std::string("textures"));

	// on détruit chaque texture de la base de données du manager
	for( m_kItor = m_kDataBase.begin(); m_kItor != m_kDataBase.end(); )
	{
		glDeleteTextures( 1, &(*m_kItor).second );
		m_kDataBase.erase( m_kItor++ );
	}
}



// ----------------------------------------------
// DeleteTexture() - détruit la texture szName.
// ----------------------------------------------

void CTextureManager::DeleteTexture( std::string szName )
{
	// on recherche l'id de la texture à détruire
	if( (m_kItor = m_kDataBase.find( szName )) != m_kDataBase.end() )
	{
//		std::cout << "CTextureManager : erasing " << szName << std::endl;

		// destruction de la texture et retrait de l'id
		// de la base de données du manager
		glDeleteTextures( 1, &(*m_kItor).second );
		m_kDataBase.erase( m_kItor );
	}
	else
	{
//		std::cout << "CTextureManager : couldn't erase " << szName << std::endl;
	}
}



// ----------------------------------------------
// DeleteTexture() - détruit la texture de l'id
// spécifiée.
// ----------------------------------------------

void CTextureManager::DeleteTexture( GLuint id )
{
	if( glIsTexture( id ) )
	{
		for( m_kItor = m_kDataBase.begin(); m_kItor != m_kDataBase.end(); ++m_kItor )
		{
			if( (*m_kItor).second == id )
			{
//				std::cout << "CTextureManager : erasing " << (*m_kItor).first<< " [" << (*m_kItor).second << "], size = " << m_kDataBase.size() << std::endl;

				// destruction de la texture et retrait de l'id
				// de la base de données du manager
				glDeleteTextures( 1, &(*m_kItor).second );
				m_kDataBase.erase( m_kItor );

				return;
			}
		}
	}
}
