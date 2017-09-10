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

#ifndef __LANGMANAGER_H
#define __LANGMANAGER_H

#include <vector>
#include <string>

#include "../includes/singleton.h"

enum 
{
	TEXT_LANGNAME,
	TEXT_GAME, 
	TEXT_OPTIONS, 
	TEXT_INFO, 
	TEXT_QUIT,
	TEXT_RETURN,
	TEXT_RESUME,
	TEXT_MAINMENU,
	TEXT_DISPLAY,
	TEXT_SOUND,
	TEXT_CONTROL,
	TEXT_MOD,
	TEXT_MODLIST,
	TEXT_MOUSE,
	TEXT_RESOLUTION,
	TEXT_FULLSCREEN,
	TEXT_FOG,
	TEXT_TEXTURESQUALITY,
	TEXT_LIGHTQUALITY,
	TEXT_ANTIALIASING,
	TEXT_ANISOTROPICFILTER,
	TEXT_IMPACTQUANTITY,
	TEXT_PARTICLEQUANTITY,
	TEXT_VIEWDEPTH,
	TEXT_GAMMA,
	TEXT_LANGUAGE,
	TEXT_MOUSESENSIBILITY,
	TEXT_INVERTMOUSEYAXIS,
	TEXT_MOUSESMOOTHING,
	TEXT_YES,
	TEXT_NO,
	TEXT_ACTIVATED,
	TEXT_DISABLED,
	TEXT_VERYLOW,
	TEXT_LOW,
	TEXT_MEDIUM,
	TEXT_HIGH,
	TEXT_VERYHIGH,
	TEXT_NEAR0,
	TEXT_NEAR1,
	TEXT_NEAR2,
	TEXT_FAR0,
	TEXT_FAR1,
	TEXT_FAR2,
	TEXT_SOUNDEFFECTS,
	TEXT_MUSIC,
	TEXT_LOADING,
	TEXT_RES_640,
	TEXT_RES_800,
	TEXT_RES_1024,
	TEXT_RES_1280,
	TEXT_RES_1600,
	TEXT_RES_1280W,
	TEXT_RES_1440W,
	TEXT_RES_1680W,
	TEXT_RES_1920W,
	TEXT_0X,
	TEXT_2X,
	TEXT_4X,
	TEXT_8X,
	TEXT_16X,
	TEXT_PERCENT0,
	TEXT_PERCENT10,
	TEXT_PERCENT20,
	TEXT_PERCENT30,
	TEXT_PERCENT40,
	TEXT_PERCENT50,
	TEXT_PERCENT60,
	TEXT_PERCENT70,
	TEXT_PERCENT80,
	TEXT_PERCENT90,
	TEXT_PERCENT100,
	TEXT_PERCENT110,
	TEXT_PERCENT120,
	TEXT_PERCENT130,
	TEXT_PERCENT140,
	TEXT_PERCENT150,
	TEXT_PERCENT160,
	TEXT_PERCENT170,
	TEXT_PERCENT180,
	TEXT_PERCENT190,
	TEXT_PERCENT200,
	TEXT_WARNINGMENUDISPLAY,
	TEXT_CONTROLCHANGE,
	TEXT_CONTROLFORWARD,
	TEXT_CONTROLBACKWARD,
	TEXT_CONTROLSTRAFELEFT,
	TEXT_CONTROLSTRAFERIGHT,
	TEXT_CONTROLTURNLEFT,
	TEXT_CONTROLTURNRIGHT,
	TEXT_CONTROLLOOKUP,
	TEXT_CONTROLLOOKDOWN,
	TEXT_CONTROLCENTERVIEW,
	TEXT_CONTROLFLASHLIGHT,
	TEXT_CONTROLCROUCH,
	TEXT_CONTROLJUMP,
	TEXT_CONTROLFIRE,
	TEXT_CONTROLPREVIOUSWEAPON,
	TEXT_CONTROLNEXTWEAPON,
	TEXT_CONTROLRELOAD,
	TEXT_CONTROLHANDGUN,
	TEXT_CONTROLSHOTGUN,
	TEXT_DESCINFO1,
	TEXT_DESCINFO2,
	TEXT_DESCINFO3,
	TEXT_DESCINFO4,
	TEXT_DESCINFO5,
	TEXT_DESCINFO6,
	TEXT_DESCINFO7,
	TEXT_DESCINFO8,
	TEXT_DESCINFO9,
	TEXT_DESCINFO10,
	TEXT_DESCINFO11,
	TEXT_LOADINGSTATE1,
	TEXT_LOADINGSTATE2,
	TEXT_LOADINGSTATE3,
	TEXT_LOADINGSTATE4,
	TEXT_LOADINGSTATE5,
	TEXT_LOADINGSTATE6,
	TEXT_MOD_NONAME,
	TEXT_MOD_INVALID,
	TEXT_START_LEVEL,
	TEXT_INFORMATION_KEY_REQUIERED,
	TEXT_INFORMATION_KEY_ACQUIRED,
	TEXT_INFORMATION_KEY_HAVE,
	TEXT_INFORMATION_FLASHLIGHT_ACQUIRED,
	TEXT_INFORMATION_FLASHLIGHT_HAVE,
	TEXT_INFORMATION_HANDGUN_ACQUIRED,
	TEXT_INFORMATION_HANDGUN_HAVE,
	TEXT_INFORMATION_SHOTGUN_ACQUIRED,
	TEXT_INFORMATION_SHOTGUN_HAVE,
	TEXT_INFORMATION_HANDGUN_AMMO_ACQUIRED,
	TEXT_INFORMATION_HANDGUN_AMMO_NEED_WEAPON,
	TEXT_INFORMATION_HANDGUN_AMMO_HAVE,
	TEXT_INFORMATION_SHOTGUN_AMMO_ACQUIRED,
	TEXT_INFORMATION_SHOTGUN_AMMO_NEED_WEAPON,
	TEXT_INFORMATION_SHOTGUN_AMMO_HAVE,
	TEXT_INFORMATION_FIRSTAID_ACQUIRED,
	TEXT_INFORMATION_HEALTH_MAX,
	TEXT_INFORMATION_VEST_ACQUIRED,
	TEXT_INFORMATION_VEST_MAX,
	TEXT_INFORMATION_DEAD
};


/**
* LangManager Class
* Handle language texts packs
* @author Stephane Baudoux
*/
class LangManager : public CSingleton<LangManager>
{
	friend class CSingleton<LangManager>;

private:
	std::vector<std::string> langFilename;
	std::vector<std::string> langDatabase;

public:
	void loadLangFileList();
	void reloadLangDatabase();
	std::string getText(unsigned int v);
	int getNumberOfLang();

private:
	LangManager();
	~LangManager();
};

#endif
