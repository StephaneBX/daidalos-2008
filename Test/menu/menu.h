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

#ifndef __MENU_H
#define __MENU_H

#include "../manager/langManager.h"
#include "../manager/texture.h"
#include "../manager/textManager.h"
#include "../manager/keyboardManager.h"
#include "../manager/optionManager.h"
#include "../manager/soundManager.h"

#define STARTGAME -2
#define QUITGAME -1
#define TEXTSPACE 0.1f
#define TEXTSPACE2 0.06f
#define TEXTSPACE3 0.05f

enum
{
	ID_MENUINTRO,
	ID_MENUMAIN,
	ID_MENUGAME,
	ID_MENUOPTION,
	ID_MENUINFO,
	ID_MENUQUIT,
	ID_MENUOPTIONDISPLAY,
	ID_MENUOPTIONSOUND,
	ID_MENUOPTIONCONTROL,
	ID_MENUOPTIONMOUSE,
	ID_MENUMOD,
	ID_GAMEMENU,
	ID_RESUME
};

#define TIMEDELAYMENU 300

/**
* Menu Class
* @author Stephane Baudoux
*/
class Menu
{
protected:
	int choosenMenuCode;
	int menuCode;
	int previousMenuCode;
	int actionChoosen;
	int image;
	int * actionList;
	int actionSize;
	int title;
	float yRatio;
	CTextureManager * texturesManager;
	TextManager * textM;
	LangManager * langM;
	KeyboardManager * keyM;
	OptionManager * optM;
	SoundManager * soundM;

public:
	Menu();
	virtual ~Menu();
	virtual int getChoosenMenuCode();
	virtual int getMenuCode();
	virtual void loadTexture();
	virtual void refresh(long time);
	virtual void draw();
};

#endif
