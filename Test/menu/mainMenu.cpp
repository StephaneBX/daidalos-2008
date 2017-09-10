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

#include "../menu/mainMenu.h"

/**
* Constructor
*/
MainMenu::MainMenu() : MenuNavigation()
{
	this->previousMenuCode = ID_MENUMAIN;
	this->menuCode = ID_MENUMAIN;
	this->choosenMenuCode = this->menuCode;
	this->actionSize = 5;
	this->actionList = new int[this->actionSize];
	this->actionEffect = new int[this->actionSize];
	this->actionList[0]		= TEXT_GAME;
	this->actionList[1]		= TEXT_MOD;
	this->actionList[2]		= TEXT_OPTIONS;
	this->actionList[3]		= TEXT_INFO;
	this->actionList[4]		= TEXT_QUIT;
	this->actionEffect[0]	= STARTGAME;
	this->actionEffect[1]	= ID_MENUMOD;
	this->actionEffect[2]	= ID_MENUOPTION;
	this->actionEffect[3]	= ID_MENUINFO;
	this->actionEffect[4]	= QUITGAME;
}

/**
* Destructor
*/
MainMenu::~MainMenu(){}
