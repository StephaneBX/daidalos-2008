//
//	texture.h - header file
//
//	David Henry - tfc_duke@club-internet.fr
//

#ifndef		__TEXTURE_H
#define		__TEXTURE_H

#include	<map>
#include	<string>
#include	"../includes/singleton.h"
#include	"../manager/optionManager.h"
#include	"../manager/logManager.h"
#include	"../manager/modManager.h"

// ==============================================
// CTextureManager - OpenGL texture manager.
// ==============================================

class CTextureManager : public CSingleton<CTextureManager>
{
	friend class CSingleton<CTextureManager>;

private:
	// constructeur/destructeur
	CTextureManager( void ) { this->optM = OptionManager::getInstance(); this->textureFilteringLevel = this->optM->getTextureFilteringLevel(); this->modM = ModManager::getInstance(); this->logM = LogManager::getInstance(); m_kDataBase.clear(); }
	~CTextureManager( void ) { ReleaseTextures(); }


public:
	// functions publiques
	void	Initialize( void );
	void	MakeNormalizeVectorCubeMap( int iSize );

	GLuint	LoadTexture( std::string szFilename, bool bFlip = false );

	GLuint	GetTexture( std::string szFilename );
	char	*GetTextureName( GLuint id );

	GLuint	GetDefaultTextureID( void )		{ return *m_pDefaultTextureID; }
	GLuint	GetNormCubeMapID( void )		{ return *m_pNormCubeMapID; }

	void	ReleaseTextures( void );

	void	DeleteTexture( std::string szName );
	void	DeleteTexture( GLuint id );

	int textureFilteringLevel;


private:
	// fonctions privées
	void	GetCubeVector( int i, int cubesize, int x, int y, float *vector );


private:
	// texture map<>
	std::map<std::string, GLuint>::iterator	m_kItor;
	std::map<std::string, GLuint>			m_kDataBase;

	GLuint	*m_pDefaultTextureID;
	GLuint	*m_pNormCubeMapID;

	ModManager * modM;
	OptionManager * optM;
	LogManager * logM;
};

#endif	// __TEXTURE_H
