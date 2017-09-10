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

#ifndef __MODMANAGER_H
#define __MODMANAGER_H

#include <string>
#include "../manager/langManager.h"
#include "../manager/optionManager.h"
#include "../manager/logManager.h"

enum
{
	MOD_INTERFACE_POSITION_HEALTH_TEXT_X,
	MOD_INTERFACE_POSITION_HEALTH_TEXT_Y,
	MOD_INTERFACE_POSITION_HEALTH_TEXT_SIZE,
	MOD_INTERFACE_POSITION_HEALTH_LOGO_X,
	MOD_INTERFACE_POSITION_HEALTH_LOGO_Y,
	MOD_INTERFACE_POSITION_HEALTH_LOGO_SIZE_X,
	MOD_INTERFACE_POSITION_HEALTH_LOGO_SIZE_Y,
	MOD_INTERFACE_POSITION_ARMOR_TEXT_X,
	MOD_INTERFACE_POSITION_ARMOR_TEXT_Y,
	MOD_INTERFACE_POSITION_ARMOR_TEXT_SIZE,
	MOD_INTERFACE_POSITION_ARMOR_LOGO_X,
	MOD_INTERFACE_POSITION_ARMOR_LOGO_Y,
	MOD_INTERFACE_POSITION_ARMOR_LOGO_SIZE_X,
	MOD_INTERFACE_POSITION_ARMOR_LOGO_SIZE_Y,
	MOD_INTERFACE_POSITION_AMMO_TEXT_X,
	MOD_INTERFACE_POSITION_AMMO_TEXT_Y,
	MOD_INTERFACE_POSITION_AMMO_TEXT_SIZE,
	MOD_INTERFACE_POSITION_AMMO_LOGO_X,
	MOD_INTERFACE_POSITION_AMMO_LOGO_Y,
	MOD_INTERFACE_POSITION_AMMO_LOGO_SIZE_X,
	MOD_INTERFACE_POSITION_AMMO_LOGO_SIZE_Y,
	MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_X,
	MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_Y,
	MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_SIZE_X,
	MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_SIZE_Y,
	MOD_INTERFACE_POSITION_BLUEKEY_LOGO_X,
	MOD_INTERFACE_POSITION_BLUEKEY_LOGO_Y,
	MOD_INTERFACE_POSITION_BLUEKEY_LOGO_SIZE_X,
	MOD_INTERFACE_POSITION_BLUEKEY_LOGO_SIZE_Y,
	MOD_INTERFACE_POSITION_REDKEY_LOGO_X,
	MOD_INTERFACE_POSITION_REDKEY_LOGO_Y,
	MOD_INTERFACE_POSITION_REDKEY_LOGO_SIZE_X,
	MOD_INTERFACE_POSITION_REDKEY_LOGO_SIZE_Y,
	MOD_INTERFACE_POSITION_GREENKEY_LOGO_X,
	MOD_INTERFACE_POSITION_GREENKEY_LOGO_Y,
	MOD_INTERFACE_POSITION_GREENKEY_LOGO_SIZE_X,
	MOD_INTERFACE_POSITION_GREENKEY_LOGO_SIZE_Y,
	MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_X,
	MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_Y,
	MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_SIZE_X,
	MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_SIZE_Y,
	MOD_INTERFACE_POSITION_MESSAGE_X,
	MOD_INTERFACE_POSITION_MESSAGE_Y,
	MOD_INTERFACE_POSITION_MESSAGE_SIZE,
	MOD_INTERFACE_POSITION_MESSAGE_MAX_LINES,
	MOD_INTERFACE_POSITION_ARRAY_SIZE
};

enum
{
	MOD_CONFIGURATION_LEVELS_LOADING,
	MOD_CONFIGURATION_LEVELS_TEXTURES,
	MOD_CONFIGURATION_LEVELS_SOUNDS,
	MOD_CONFIGURATION_LEVELS_ARRAY_SIZE
};

enum
{
	MOD_CONFIGURATION_START_ELEMENTS_FLASHLIGHT,
	MOD_CONFIGURATION_START_ELEMENTS_HANDGUN,
	MOD_CONFIGURATION_START_ELEMENTS_SHOTGUN,
	MOD_CONFIGURATION_START_ELEMENTS_ARRAY_SIZE
};

enum
{
	MOD_CONFIGURATION_START_VALUE_HEALTH,
	MOD_CONFIGURATION_START_VALUE_ARMOR,
	MOD_CONFIGURATION_START_VALUE_HANDGUN,
	MOD_CONFIGURATION_START_VALUE_SHOTGUN,
	MOD_CONFIGURATION_START_VALUE_ARRAY_SIZE
};

/**
* ModManager Class
* Handle mods and levels selection
* @author Stephane Baudoux
*/
class ModManager : public CSingleton<ModManager>
{
	friend class CSingleton<ModManager>;

private:
	int numberOfMod;
	std::string mainModDirectoryName;
	std::string modChoosen;
	int * numberOfLevelsPerMod;
	int ** startConfigurationValue;
	int ** ambientLight;
	float ** interfacePosition;
	bool ** startConfiguration;
	bool ** levelsConfiguration;
	bool ** intersectionBlock;
	std::string ** levelsFile;
	std::string ** levelsName;
	std::string ** musicsFile;
	std::string * modsName;
	std::string * modRootDirectory;
	LangManager * langM;
	OptionManager * optM;
	LogManager * logM;

public:
	void refreshModList();
	void freeLists();
	void setDefaultMod();
	void setModChoosen(int v);
	int getModChoosenNumber();
	int getNumberOfMod();
	int getNumberOfLevel();
	int getStartConfigurationValue(int value);
	int getAmbientLight(int v);
	float getInterfacePosition(int value);
	bool getStartConfiguration(int value);
	bool getLevelsConfiguration(int value);
	bool getIntersectionBlock(int v);
	std::string getModChoosen();
	std::string getModRootDirectory();
	std::string getModName(int v);
	std::string getLevelFilePath(int v);
	std::string getLevelName(std::string filename);
	std::string getMusicFilePath(int v);

private:
	ModManager();
	~ModManager();
};

#endif
