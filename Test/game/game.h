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

#ifndef __GAME_H
#define __GAME_H

#include <SDL/SDL.h>
#include <string>
#include "../manager/levelManager.h"
#include "../manager/interfaceManager.h"
#include "../manager/keyboardManager.h"
#include "../manager/langManager.h"
#include "../manager/levelManager.h"
#include "../manager/loadingManager.h"
#include "../manager/mouseManager.h"
#include "../manager/optionManager.h"
#include "../manager/soundManager.h"
#include "../manager/textManager.h"
#include "../manager/texture.h"
#include "../manager/modManager.h"
#include "../manager/particleManager.h"
#include "../manager/messageManager.h"
#include "../menu/mainMenu.h"
#include "../menu/displayMenu.h"
#include "../menu/optionMenu.h"
#include "../menu/soundMenu.h"
#include "../menu/mouseMenu.h"
#include "../menu/controlMenu.h"
#include "../menu/gameMenu.h"
#include "../menu/informationMenu.h"
#include "../menu/modMenu.h"
#include "../menu/introMenu.h"

#define MAXIMUM_INTERVAL_TIME 40
#define SLEEP_MENU_TIMER 32

/**
* Game Class
* Handle general instructions
* @author Stephane Baudoux
*/
class Game
{
private:
	int width;
	int height;
	int antialiasing;
	int mode;
	int levelNumber;
	bool fullScreen;
	bool quit;
	
	// Light variables
	float light1Pos[4];
	float light1Dif[4];
	float light1Spec[4];
	float light1Amb[4];
	float spot1Dir[3];
	float light1Dir[3];
	float fogColor[4];

	std::string mapTestPath;
	SDL_Surface * screen;
	InterfaceManager * interfaceM;
	KeyboardManager * keyM;
	LangManager * langM;
	LevelManager * levelM;
	LoadingManager * loadingM;
	MouseManager * mouseM;
	OptionManager * optM;
	SoundManager * soundM;
	TextManager * textM;
	CTextureManager * texturesManager;
	ModManager * modM;
	ParticleManager * particleM;
	MessageManager * messageM;
	Menu * menuM;
	GameMenu * gMenu;

public:
	Game(int width, int height, std::string defaultMap);
	void kill();
	void start();
	void loadLevel(int number);
	void draw(long time);

private:
	void initSDL();
	void initOpenGl();
	void loadManagers();
	void resize(int width, int height);
};

#endif
