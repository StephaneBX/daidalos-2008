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

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "../includes/global_var.h"
#include "../menu/controlMenu.h"
#include "../menu/displayMenu.h"
#include "../menu/gameMenu.h"
#include "../menu/informationMenu.h"
#include "../menu/introMenu.h"
#include "../menu/mainMenu.h"
#include "../menu/modMenu.h"
#include "../menu/mouseMenu.h"
#include "../menu/optionMenu.h"
#include "../menu/soundMenu.h"
#include "../game/game.h"

/**
* Constructor
* @param width Resolution width
* @param height Resolution height
* @param defaultMap Default map to be used
*/
Game::Game(int width, int height, std::string defaultMap)
{
	this->width = width;
	this->height = height;
	this->quit = false;
	this->mode = MODE_MAINMENU;
	this->mapTestPath = defaultMap;
	this->levelNumber = 0;
	
	this->light1Pos[0] = 15.0f;
	this->light1Pos[1] = 9.0f;
	this->light1Pos[2] = 20.0f;
	this->light1Pos[3] = 1.0f;

	this->light1Dif[0] = 0.7f;
	this->light1Dif[1] = 0.7f;
	this->light1Dif[2] = 0.7f;
	this->light1Dif[3] = 1.0f;

	this->light1Spec[0] = 0.4f;
	this->light1Spec[1] = 0.4f;
	this->light1Spec[2] = 0.4f;
	this->light1Spec[3] = 1.0f;

	this->light1Amb[0] = 0.5f;
	this->light1Amb[1] = 0.5f;
	this->light1Amb[2] = 0.5f;
	this->light1Amb[3] = 1.0f;

	this->spot1Dir[0] = 0.0f;
	this->spot1Dir[1] = 0.0f;
	this->spot1Dir[2] = -1.0f;

	this->light1Dir[0] = -1.0f;
	this->light1Dir[1] = 0.0f;
	this->light1Dir[2] = 0.0f;

	this->fogColor[0]= 0.0f;
	this->fogColor[1]= 0.0f;
	this->fogColor[2]= 0.0f;
	this->fogColor[3]= 1.0f;
	
	initSDL();
	initOpenGl();
	loadManagers();
	resize(this->width,this->height);
	menuM = new IntroMenu();
}

/**
* Destructor
*/
void Game::kill()
{
	InterfaceManager::kill();
	KeyboardManager::kill();
	LangManager::kill();
	LevelManager::kill();
	MouseManager::kill();
	SoundManager::kill();
	TextManager::kill();
	MessageManager::kill();
	ModManager::kill();
	LoadingManager::kill();
	CTextureManager::kill();
	ParticleManager::kill();
	ProjectileManager::kill();
	ModelManager::kill();
	OptionManager::kill();
	EnemyManager::kill();

	if(this->menuM != NULL)
		delete this->menuM;

	if(this->gMenu != NULL)
		delete this->gMenu;

	LogManager::kill();
}

/**
* Initialization of SDL
*/
void Game::initSDL()
{
#ifdef _WIN32
	_putenv("SDL_VIDEO_WINDOW_POS=center");
#else
	putenv((char*)"SDL_VIDEO_WINDOW_POS=center");
#endif

	SDL_Init(SDL_INIT_VIDEO);
	atexit(SDL_Quit);
	SDL_WM_SetCaption("Daidalos -  Alpha Version 0.85",NULL);

	this->optM = OptionManager::getInstance();

	// Choose 640x480 windowed video mode if the current one is not supported
	if(SDL_VideoModeOK(this->width,this->height,32,SDL_FULLSCREEN) == 0)
	{
		this->optM = OptionManager::getInstance();
		this->optM->setResolution(0);
		this->optM->setFullScreen(0);
		this->width = this->optM->getResolutionWidth(0);
		this->height = this->optM->getResolutionHeight(0);
	}
	
	// Antialiasing activation if activated
	this->antialiasing = this->optM->getAntialiasingLevel();
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, this->antialiasing);

	// Resolution
	this->fullScreen = this->optM->isFullScreen();
	if(this->fullScreen)
		screen = SDL_SetVideoMode(width,height,32, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_SWSURFACE | SDL_HWSURFACE | SDL_FULLSCREEN);
	else
		screen = SDL_SetVideoMode(width,height,32, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_SWSURFACE | SDL_HWSURFACE);
	
	SDL_ShowCursor(0);
	SDL_WarpMouse(this->width/2,this->height/2);
	SDL_SetGamma((float)this->optM->getGammaR() / 10.0f + 0.5f, (float)this->optM->getGammaG() / 10.0f + 0.5f, (float)this->optM->getGammaB() / 10.0f + 0.5f);
}

/**
* OpenGL initialization
*/
void Game::initOpenGl()
{
	glClearColor(0.0,0.0,0.0,0.0);

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glPolygonMode(GL_FRONT,GL_FILL);
	// glPolygonMode(GL_FRONT,GL_LINE);

	// Light initialization
	glLightfv(GL_LIGHT0, GL_DIFFUSE, this->light1Dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, this->light1Spec);
	glLightfv(GL_LIGHT0, GL_AMBIENT, this->light1Amb);
	glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, 70);
	glLighti(GL_LIGHT0, GL_SPOT_EXPONENT,40);
	glEnable(GL_LIGHTING);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	// Fog initialization
	glFogi(GL_FOG_MODE, OptionManager::getInstance()->getFogMode());
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.52f);
	glHint(GL_FOG_HINT, OptionManager::getInstance()->getFogQuality());
	glFogf(GL_FOG_START, 5.0f);
	glFogf(GL_FOG_END, (float)OptionManager::getInstance()->getGlobalVisibility() * GLOBALVISIBILITYMODIFIER + 15);
	if(OptionManager::getInstance()->isFog())
		glEnable(GL_FOG);

	glColor3d(1.0,1.0,1.0);
}

/**
* Managers initialization
*/
void Game::loadManagers()
{
	this->interfaceM = NULL;
	this->keyM = KeyboardManager::getInstance();
	this->langM = LangManager::getInstance();
	this->levelM = LevelManager::getInstance();
	this->mouseM = MouseManager::getInstance();
	this->modM = ModManager::getInstance();
	this->optM = OptionManager::getInstance();
	this->soundM = SoundManager::getInstance();
	this->textM = TextManager::getInstance();
	this->texturesManager = CTextureManager::getInstance();
	this->particleM = ParticleManager::getInstance();
	this->messageM = MessageManager::getInstance();
	this->loadingM = LoadingManager::getInstance();
	this->soundM->loadMusic();
	this->gMenu = NULL;
}

/**
* Load a level
* @param number Level number in the selected mod list
*/
void Game::loadLevel(int number = 0)
{
	if(this->mapTestPath == "")
		this->levelM->loadMap(this->modM->getLevelFilePath(number),this->modM->getMusicFilePath(number), this->modM->getIntersectionBlock(number), this->modM->getAmbientLight(number));
	else
		this->levelM->loadMap(this->mapTestPath,this->modM->getMusicFilePath(0), true, 100);

	this->mode = MODE_INGAME;
}

/**
* Start the game
*/
void Game::start()
{
/*
	// ------------------------------------------------------
	// TEST SHADERS
	// ------------------------------------------------------
	PFNGLCREATESHADERPROC		glCreateShader			= (PFNGLCREATESHADERPROC)		SDL_GL_GetProcAddress("glCreateShader");
	PFNGLDELETESHADERPROC		glDeleteShader			= (PFNGLDELETESHADERPROC)		SDL_GL_GetProcAddress("glDeleteShader");
	PFNGLSHADERSOURCEPROC		glShaderSource			= (PFNGLSHADERSOURCEPROC)		SDL_GL_GetProcAddress("glShaderSource");
	PFNGLCOMPILESHADERPROC		glCompileShader			= (PFNGLCOMPILESHADERPROC)		SDL_GL_GetProcAddress("glCompileShader");
	PFNGLCREATEPROGRAMPROC		glCreateProgram			= (PFNGLCREATEPROGRAMPROC)		SDL_GL_GetProcAddress("glCreateProgram");
	PFNGLDELETEPROGRAMPROC		glDeleteProgram			= (PFNGLDELETEPROGRAMPROC)		SDL_GL_GetProcAddress("glDeleteProgram");
	PFNGLATTACHSHADERPROC		glAttachShader			= (PFNGLATTACHSHADERPROC)		SDL_GL_GetProcAddress("glAttachShader");
	PFNGLDETACHSHADERPROC		glDetachShader			= (PFNGLDETACHSHADERPROC)		SDL_GL_GetProcAddress("glDetachShader");
	PFNGLLINKPROGRAMPROC		glLinkProgram			= (PFNGLLINKPROGRAMPROC)		SDL_GL_GetProcAddress("glLinkProgram");
	PFNGLUSEPROGRAMPROC			glUseProgram			= (PFNGLUSEPROGRAMPROC)			SDL_GL_GetProcAddress("glUseProgram");
	PFNGLGETSHADERIVPROC		glGetShaderiv			= (PFNGLGETSHADERIVPROC)		SDL_GL_GetProcAddress("glGetShaderiv");
	PFNGLGETSHADERINFOLOGPROC	glGetShaderInfoLog		= (PFNGLGETSHADERINFOLOGPROC)	SDL_GL_GetProcAddress("glGetShaderInfoLog");
	PFNGLGETUNIFORMLOCATIONPROC	glGetUniformLocation	= (PFNGLGETUNIFORMLOCATIONPROC)	SDL_GL_GetProcAddress("glGetUniformLocation");
	PFNGLUNIFORM1IPROC			glUniform1i				= (PFNGLUNIFORM1IPROC)			SDL_GL_GetProcAddress("glUniform1i");

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	program = glCreateProgram();

	std::string vertex = "varying vec3 normal, lightDir, eyeVec, pos;varying float att; void main(){normal = gl_NormalMatrix * gl_Normal; vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex); lightDir = vec3(gl_LightSource[0].position.xyz - vVertex); eyeVec = -vVertex; float d = length(lightDir); att = 1.0 / ( gl_LightSource[0].constantAttenuation + (gl_LightSource[0].linearAttenuation*d) + (gl_LightSource[0].quadraticAttenuation*d*d) ); pos = gl_Vertex.xyz;gl_TexCoord[0] = gl_MultiTexCoord0;gl_Position = ftransform();}";
	std::string fragment = "uniform sampler2D text1;varying vec3 normal, lightDir, eyeVec, pos;varying float att;void main (void){vec4 final_color = (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + (gl_LightSource[0].ambient * gl_FrontMaterial.ambient)*att;				vec3 N = normalize(normal);vec3 L = normalize(lightDir);vec4 texval1;float lambertTerm = dot(N,L);if(lambertTerm > 0.0){final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm * att;vec3 E = normalize(eyeVec);vec3 R = reflect(-L, N);float specular = pow( max(dot(R, E), 0.0),gl_FrontMaterial.shininess );final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular * att;}texval1 = texture2D(text1, vec2(gl_TexCoord[0]));gl_FragColor = (final_color * texval1 ) / 4 ;}";

	const char * vertex_c	= vertex.c_str();
	const char * fragment_c	= fragment.c_str();

	glShaderSource(vertexShader, 1, (const GLchar**)&vertex_c, NULL);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragment_c, NULL);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	delete[] vertex_c;
	delete[] fragment_c;
	vertex_c = NULL;
	fragment_c = NULL;

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glUseProgram(program);
	// ------------------------------------------------------
	// TEST
	// ------------------------------------------------------
*/

	long previousTime = SDL_GetTicks();
	long time;

	// General loop
	while(!this->quit)
	{
		time = SDL_GetTicks() - previousTime;
		previousTime = SDL_GetTicks();

		if(time > MAXIMUM_INTERVAL_TIME)
			time = MAXIMUM_INTERVAL_TIME;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				// Mouse event
				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					if(this->mode == MODE_INGAME)
						this->mouseM->mouseManagment(event);
					SDL_WarpMouse(this->width/2,this->height/2);
					break;

				// Key event
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					this->keyM->keyManagment(event, this->mode);
					break;

				case SDL_QUIT:
					exit(0);
					quit = true;
					break;

				case SDL_VIDEORESIZE:
					resize(event.resize.w,event.resize.h);
			}
		}
		draw(time);
	}
}

/**
* Draw the Game
*/
void Game::draw(long time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// Sleep timer used in the menus
	int sleepTimer = SLEEP_MENU_TIMER - time;

	// If Echap id pressed during the game, draw the game menu
	if(this->mode == MODE_INGAME && this->keyM->isKeyPressed(KEYBOARD_MENU_CANCEL_KEY))
	{
		this->mode = MODE_GAMEMENU;
		this->gMenu = new GameMenu();
	}
	
	// In-game
	if(this->mode == MODE_INGAME)
	{
		// Refresh data
		this->levelM->refresh(time);
		this->messageM->refresh(time);
		this->interfaceM->refresh(this->levelM->getPlayer());

		// If the player hasn't finished the level
		if(!this->levelM->isLevelFinished())
		{
			// Update light location data
			this->light1Dir[0] = (float)this->levelM->getPlayer()->getView(0) - (float)this->levelM->getPlayer()->getCoor()[0];
			this->light1Dir[1] = (float)this->levelM->getPlayer()->getView(1) - (float)this->levelM->getPlayer()->getCoor()[1] + 3.0f;
			this->light1Dir[2] = (float)this->levelM->getPlayer()->getView(2) - (float)this->levelM->getPlayer()->getCoor()[2];

			this->light1Pos[0] = (float)this->levelM->getPlayer()->getCoor()[0];
			this->light1Pos[1] = (float)this->levelM->getPlayer()->getCoor()[1] + 3.0f;
			this->light1Pos[2] = (float)this->levelM->getPlayer()->getCoor()[2];

			// Update view location
			gluLookAt(this->levelM->getPlayer()->getCoor()[0],this->levelM->getPlayer()->getCoor()[1] + 3.0,this->levelM->getPlayer()->getCoor()[2],this->levelM->getPlayer()->getView(0),this->levelM->getPlayer()->getView(1),this->levelM->getPlayer()->getView(2),0.0,1.0,0.0);

			glLightfv(GL_LIGHT0, GL_POSITION, this->light1Pos);
			glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, this->light1Dir);
			glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1/20.0f);
			glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f);
			glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0002f);

			// Draw the game (3D part)
			this->levelM->draw(this->light1Dir);
		}
		else
			this->mode = MODE_CHANGELEVEL;
	}

    glMatrixMode(GL_PROJECTION);

    glPushMatrix();
        glLoadIdentity();
        glOrtho( 0, 1, 0, 1, -1, 1 );
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glLoadIdentity();

			// 2D menu
			glDisable(GL_LIGHTING);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_DEPTH_TEST);

			if(this->mode == MODE_INGAME)
			{
				// Draw the game interface
				this->messageM->draw();
				interfaceM->drawPlayerInterface(this->levelM->getPlayer());
			}
			else
			{
				// Game main menu
				if(this->mode == MODE_MAINMENU)
				{
					// Refresh data
					menuM->refresh(time);
					// If the player change menu
					if(menuM->getChoosenMenuCode() != menuM->getMenuCode())
					{
						// Start the music
						if(menuM->getMenuCode() == ID_MENUINTRO)
							this->soundM->playMusic();

						// Draw the correct menu
						switch(menuM->getChoosenMenuCode())
						{
						case QUITGAME:
							this->quit = true;
							break;

						case STARTGAME:
							delete menuM;
							menuM = NULL;
							this->soundM->stopMusic();
							this->interfaceM = InterfaceManager::getInstance();
							this->loadingM->loadLoading();
							this->levelM->resetPlayerStats();
							loadLevel();
							break;

						case ID_MENUMOD:
							delete menuM;
							menuM = NULL;
							menuM = new ModMenu();
							menuM->draw();
							break;

						case ID_MENUOPTION:
							delete menuM;
							menuM = NULL;
							// If some graphic options changed, reload SDL and OpenGL with correct options
							if(this->height != this->optM->getResolutionHeight() || this->width != this->optM->getResolutionWidth() || this->antialiasing != this->optM->getAntialiasingLevel() || this->fullScreen != this->optM->isFullScreen())
							{
								this->optM->getResolution(this->width, this->height);
								this->soundM->closeAudio();
								SDL_Quit();
								initSDL();
								initOpenGl();
								resize(this->width,this->height);
								texturesManager->ReleaseTextures();
								this->textM->loadText();
								this->loadingM->loadLoading();
								this->soundM->openAudio();
								this->soundM->playMusic();
							}
							menuM = new OptionMenu();
							menuM->draw();
							break;

						case ID_MENUMAIN:
							delete menuM;
							menuM = NULL;
							menuM = new MainMenu();
							menuM->draw();
							break;

						case ID_MENUOPTIONDISPLAY:
							delete menuM;
							menuM = NULL;
							menuM = new DisplayMenu();
							menuM->draw();
							break;

						case ID_MENUOPTIONSOUND:
							delete menuM;
							menuM = NULL;
							menuM = new SoundMenu();
							menuM->draw();
							break;

						case ID_MENUOPTIONMOUSE:
							delete menuM;
							menuM = NULL;
							menuM = new MouseMenu();
							menuM->draw();
							break;

						case ID_MENUOPTIONCONTROL:
							delete menuM;
							menuM = NULL;
							menuM = new ControlMenu();
							menuM->draw();
							break;

						case ID_MENUINFO:
							delete menuM;
							menuM = NULL;
							menuM = new InformationMenu();
							menuM->draw();
							break;

						default:
							break;
						}
					}
					else
					{
						menuM->draw();

						// Prevent the computer from calculating too much frames for nothing
						if(sleepTimer > 0)
						{
							#ifdef _WIN32
								Sleep(sleepTimer);
							#else
								usleep(sleepTimer);	
							#endif
						}
					}
				}
				else
				{
					// In-Game menu
					if(this->mode == MODE_GAMEMENU)
					{
						gMenu->refresh(time);
						if(gMenu->getChoosenMenuCode() != gMenu->getMenuCode())
						{
							switch(gMenu->getChoosenMenuCode())
							{
							case ID_RESUME:
								delete gMenu;
								gMenu = NULL;
								this->mode = MODE_INGAME;
								break;

							case ID_MENUMAIN:
								// Remove the game menu
								delete gMenu;
								gMenu = NULL;
								// Unload the sounds
								this->soundM->freeSoundDatabase();
								// Free the level
								this->levelM->unloadMap();
								// Kill the weapon manager
								WeaponManager::getInstance()->kill();
								// Free models
								ModelManager::getInstance()->freeModelDatabase();
								// Change mode (Main menu)
								this->mode = MODE_MAINMENU;
								// Free the textures
								this->texturesManager->ReleaseTextures();
								// Free the interface
								this->interfaceM->kill();
								// Load default music
								this->soundM->loadMusic();
								// Load text
								this->textM->loadText();
								// Load loading
								this->loadingM->loadLoading();
								// Play music
								this->soundM->playMusic();
								// Set levelNumber to 0
								this->levelNumber = 0;
								menuM = new MainMenu();
								menuM->draw();
								break;

							default:
								break;
							}
						}
						else
						{
							gMenu->draw();

							// Prevent the computer from calculating too much frames for nothing
							if(sleepTimer > 0)
							{
								#ifdef _WIN32
									Sleep(sleepTimer);
								#else
									usleep(sleepTimer);	
								#endif
							}
						}
					}
					else
					{
						// Change level mode
						if(this->mode == MODE_CHANGELEVEL)
						{
							// Stop the music
							this->soundM->stopMusic();
							// Free the level
							this->levelM->unloadMap();
							// Initlialize the keys
							this->keyM->initializeKeys();

							// If we are in test mode, we return to main menu
							if(this->mapTestPath != "")
							{
								// Unload the sounds
								this->soundM->freeSoundDatabase();
								// Kill the weapon manager
								WeaponManager::getInstance()->kill();
								// Free models
								ModelManager::getInstance()->freeModelDatabase();
								// Free the textures
								this->texturesManager->ReleaseTextures();
								// Free the interface
								this->interfaceM->kill();
								// Load default music
								this->soundM->loadMusic();
								// Load text
								this->textM->loadText();
								// Load loading
								this->loadingM->loadLoading();
								// Play music
								this->soundM->playMusic();
								menuM = new MainMenu();
								menuM->draw();
								// Change mode (Main menu)
								this->mode = MODE_MAINMENU;
							}
							else
							{
								// If we are in normal mode, we search the next level if there is one and we load it
								if(++this->levelNumber < this->modM->getNumberOfLevel())
								{
									loadLevel(this->levelNumber);
								}
								else
								{
									// If not, we return to the main menu

									// Reset Player stats to default
									this->levelM->resetPlayerStats();
									// Unload the sounds
									this->soundM->freeSoundDatabase();
									// Kill the weapon manager
									WeaponManager::getInstance()->kill();
									// Free models
									ModelManager::getInstance()->freeModelDatabase();
									// Change mode (Main menu)
									this->mode = MODE_MAINMENU;
									// Free the textures
									this->texturesManager->ReleaseTextures();
									// Free the interface
									this->interfaceM->kill();
									// Set levelNumber to 0
									this->levelNumber = 0;
									// Load default music
									this->soundM->loadMusic();
									// Load text
									this->textM->loadText();
									// Load loading
									this->loadingM->loadLoading();
									// Play music
									this->soundM->playMusic();
									menuM = new MainMenu();
									menuM->draw();
								}
							}
						}
					}
				}
			}

			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			glEnable(GL_LIGHTING);

			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	SDL_GL_SwapBuffers();
}

/**
* Resize the window and the frustum
*/
void Game::resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glFrustum(-ar / 3.0, ar / 3.0, -0.35, .35, 0.5, OptionManager::getInstance()->getGlobalVisibility() * GLOBALVISIBILITYMODIFIER + 15);
}
