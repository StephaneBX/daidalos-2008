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
#else
	#include <dirent.h>
#endif

#include <sstream>
#include <fstream>
#include "../tinyXML/tinyxml.h"
#include "../manager/modManager.h"

/**
* Constructor
*/
ModManager::ModManager()
{
	this->langM = LangManager::getInstance();
	this->logM = LogManager::getInstance();
	this->optM = OptionManager::getInstance();

	this->mainModDirectoryName = "Main";
	this->numberOfLevelsPerMod = NULL;
	this->startConfigurationValue = NULL;
	this->ambientLight = NULL;
	this->intersectionBlock = NULL;
	this->levelsFile = NULL;
	this->levelsName = NULL;
	this->musicsFile = NULL;
	this->modsName = NULL;
	this->modRootDirectory = NULL;
	this->interfacePosition = NULL;
	this->startConfiguration = NULL;
	this->levelsConfiguration = NULL;
	refreshModList();
	this->modChoosen = this->mainModDirectoryName;
}

/**
* Destructor
*/
ModManager::~ModManager()
{
	freeLists();
}

/**
* Free the modManager's lists
*/
void ModManager::freeLists()
{
	if(this->modsName != NULL)
	{
		delete[] this->modsName;
		this->modsName = NULL;
	}

	if(this->modRootDirectory != NULL)
	{
		delete[] this->modRootDirectory;
		this->modRootDirectory = NULL;
	}

	if(this->numberOfLevelsPerMod != NULL)
	{
		delete[] this->numberOfLevelsPerMod;
		this->numberOfLevelsPerMod = NULL;
	}

	if(this->startConfigurationValue != NULL)
	{
		for(int i = 0 ; i < this->numberOfMod ; i++)
		{
			if(this->startConfigurationValue[i] != NULL)
			{
				delete[] this->startConfigurationValue[i];
				this->startConfigurationValue[i] = NULL;
			}
		}
		delete this->startConfigurationValue;
		this->startConfigurationValue = NULL;
	}

	if(this->levelsFile != NULL)
	{
		for(int i = 0 ; i < this->numberOfMod ; i++)
		{
			if(this->levelsFile[i] != NULL)
			{
				delete[] this->levelsFile[i];
				this->levelsFile[i] = NULL;
			}
		}
		delete this->levelsFile;
		this->levelsFile = NULL;
	}

	if(this->levelsName != NULL)
	{
		for(int i = 0 ; i < this->numberOfMod ; i++)
		{
			if(this->levelsName[i] != NULL)
			{
				delete[] this->levelsName[i];
				this->levelsName[i] = NULL;
			}
		}
		delete this->levelsName;
		this->levelsName = NULL;
	}

	if(this->musicsFile != NULL)
	{
		for(int i = 0 ; i < this->numberOfMod ; i++)
		{
			if(this->musicsFile[i] != NULL)
			{
				delete[] this->musicsFile[i];
				this->musicsFile[i] = NULL;
			}
		}
		delete this->musicsFile;
		this->musicsFile = NULL;
	}

	if(this->ambientLight != NULL)
	{
		for(int i = 0 ; i < this->numberOfMod ; i++)
		{
			if(this->ambientLight[i] != NULL)
			{
				delete[] this->ambientLight[i];
				this->ambientLight[i] = NULL;
			}
		}
		delete this->ambientLight;
		this->ambientLight = NULL;
	}

	if(this->startConfiguration != NULL)
	{
		for(int i = 0 ; i < this->numberOfMod ; i++)
		{
			if(this->startConfiguration[i] != NULL)
			{
				delete[] this->startConfiguration[i];
				this->startConfiguration[i] = NULL;
			}
		}
		delete this->startConfiguration;
		this->startConfiguration = NULL;
	}

	if(this->levelsConfiguration != NULL)
	{
		for(int i = 0 ; i < this->numberOfMod ; i++)
		{
			if(this->levelsConfiguration[i] != NULL)
			{
				delete[] this->levelsConfiguration[i];
				this->levelsConfiguration[i] = NULL;
			}
		}
		delete this->levelsConfiguration;
		this->levelsConfiguration = NULL;
	}

	if(this->interfacePosition != NULL)
	{
		for(int i = 0 ; i < this->numberOfMod ; i++)
		{
			if(this->interfacePosition[i] != NULL)
			{
				delete[] this->interfacePosition[i];
				this->interfacePosition[i] = NULL;
			}
		}
		delete this->interfacePosition;
		this->interfacePosition = NULL;
	}

	if(this->intersectionBlock != NULL)
	{
		for(int i = 0 ; i < this->numberOfMod ; i++)
		{
			if(this->intersectionBlock[i] != NULL)
			{
				delete[] this->intersectionBlock[i];
				this->intersectionBlock[i] = NULL;
			}
		}
		delete this->intersectionBlock;
		this->intersectionBlock = NULL;
	}

	this->numberOfLevelsPerMod = 0;
	this->numberOfMod = 0;
}

/**
* Set the game mod to the default one
*/
void ModManager::setDefaultMod()
{
	this->modChoosen = this->mainModDirectoryName;
}

/**
* Set the game mod
* @param v Mod number
*/
void ModManager::setModChoosen(int v)
{
	this->modChoosen = this->modsName[v];
}

/**
* Return an interface element position
* @param value Element choosen
* @return Value to the element choosen
*/
float ModManager::getInterfacePosition(int value)
{
	return this->interfacePosition[getModChoosenNumber()][value];
}

/**
* Return the choosen mod name
* @param v Mod number
* @return The mod's name
*/
std::string ModManager::getModName(int v)
{
	return this->modsName[v];
}

/**
* Return the current mod name
* @return Current mod's name
*/
std::string ModManager::getModChoosen()
{
	return this->modChoosen;
}

/**
* Return the current mod root directory
* @return Mod's root directory
*/
std::string ModManager::getModRootDirectory()
{
	return this->modRootDirectory[getModChoosenNumber()];
}

/**
* Search in the modsname list to get the index of the current mod
* @return Mod number
*/
int ModManager::getModChoosenNumber()
{
	int ret = 0;
	bool founded = false;
	for(int i = 0 ; i < this->numberOfMod && !founded ; i++)
	{
		if(strcmp(this->modChoosen.c_str(),this->modsName[i].c_str()) == 0)
			ret = i;
	}
	return ret;
}

/**
* Return the number of mods
* @return Number of mods
*/
int ModManager::getNumberOfMod()
{
	return this->numberOfMod;
}

/**
* Return the number of levels for the current mod
* @return Number of levels
*/
int ModManager::getNumberOfLevel()
{
	return this->numberOfLevelsPerMod[getModChoosenNumber()];
}

/**
* Return the value of the start configuration element choosen
* @param value Element choosen
* @return Value to the element choosen
*/
int ModManager::getStartConfigurationValue(int value)
{
	return this->startConfigurationValue[getModChoosenNumber()][value];
}

/**
* Return the ambient light for the level
* @param v Level number
* @return Ambient light value
*/
int ModManager::getAmbientLight(int v)
{
	return this->ambientLight[getModChoosenNumber()][v];
}

/**
* Return the level filename choosen of the current mod
* @param v Level number
* @return Level filename
*/
std::string ModManager::getLevelFilePath(int v)
{
	return this->levelsFile[getModChoosenNumber()][v];
}

/**
* Return the level name choosen of the current mod
* @param filename Level filename
* @return Level name
*/
std::string ModManager::getLevelName(std::string filename)
{
	int index = -1;
	std::string text = "";
	for(int i = 0 ; i < numberOfLevelsPerMod[getModChoosenNumber()] && index == -1; i++)
		if(this->levelsFile[getModChoosenNumber()][i] == filename)
			index = i;

	if(index > -1)
		text = this->levelsName[getModChoosenNumber()][index];
	return text;
}

/**
* Return the music filename choosen of the current mod
* @param v Level number
* @return Music filename
*/
std::string ModManager::getMusicFilePath(int v)
{
	return this->musicsFile[getModChoosenNumber()][v];
}

/**
* Return if the element is given at the begining of the mod
* @param value Element
* @return State of the element
*/
bool ModManager::getStartConfiguration(int value)
{
	return this->startConfiguration[getModChoosenNumber()][value];
}

/**
* Return a level configuration
* @param value Element to the level configuration
* @return Element state
*/
bool ModManager::getLevelsConfiguration(int value)
{
	return this->levelsConfiguration[getModChoosenNumber()][value];
}

/**
* Return if intersection blocks are enabled
* @param v Level number
* @return InterBlock state
*/
bool ModManager::getIntersectionBlock(int v)
{
	return this->intersectionBlock[getModChoosenNumber()][v];
}

/**
* Refresh the mod list
*/
void ModManager::refreshModList()
{
	std::ostringstream stringstream;

	// If variables have already been initialized, we free them
	freeLists();

	if(this->optM->isDebug())
		this->logM->logIt(std::string("Reading mod"),std::string("directory"));

	// Find the number of mods
#ifdef _WIN32

	HANDLE hfind;
	WIN32_FIND_DATA wfd;
	hfind = FindFirstFileW(L"mods/*.*", &wfd);
	do
	{
		// If it's a directory, we increase numberOfMod value
		if (wfd.cFileName[0] != '.' && wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			this->numberOfMod++;
	}
	while(FindNextFile(hfind, &wfd));
	FindClose(hfind);

#else

	DIR *pDIR;
    struct dirent *pDirEnt;
	pDIR = opendir("mods/");
	pDirEnt = readdir( pDIR );

	while ( pDirEnt != NULL )
	{
		// Make sure it's not ./ or ../
		if(strcmp(pDirEnt->d_name,".") != 0 && strcmp(pDirEnt->d_name,"..") != 0)
		{
			// If it's a directory
			if (pDirEnt->d_type == DT_DIR)
				this->numberOfMod++;
		}
		pDirEnt = readdir( pDIR );
	}
	closedir( pDIR );

#endif

	// Variables initialization
	this->modsName = new std::string[this->numberOfMod];
	this->modRootDirectory = new std::string[this->numberOfMod];
	this->numberOfLevelsPerMod = new int[this->numberOfMod];

	this->levelsFile = new std::string*[this->numberOfMod];
	this->levelsName = new std::string*[this->numberOfMod];
	this->musicsFile = new std::string*[this->numberOfMod];
	this->ambientLight = new int*[this->numberOfMod];
	this->startConfigurationValue = new int*[this->numberOfMod];
	this->intersectionBlock = new bool*[this->numberOfMod];
	this->startConfiguration = new bool*[this->numberOfMod];
	this->levelsConfiguration = new bool*[this->numberOfMod];
	this->interfacePosition = new float*[this->numberOfMod];

	for(int i = 0 ; i < this->numberOfMod ; i++)
	{
		// Custumization levels data initialization
		this->levelsFile[i] = NULL;
		this->levelsName[i] = NULL;
		this->musicsFile[i] = NULL;
		this->ambientLight[i] = NULL;
		this->intersectionBlock[i] = NULL;

		// Levels configuration initialization
		this->levelsConfiguration[i] = new bool[MOD_CONFIGURATION_LEVELS_ARRAY_SIZE];
		this->levelsConfiguration[i][MOD_CONFIGURATION_LEVELS_LOADING] = true;
		this->levelsConfiguration[i][MOD_CONFIGURATION_LEVELS_TEXTURES] = false;
		this->levelsConfiguration[i][MOD_CONFIGURATION_LEVELS_SOUNDS] = false;

		// Start configuration initialization
		this->startConfiguration[i] = new bool[MOD_CONFIGURATION_START_ELEMENTS_ARRAY_SIZE];
		this->startConfiguration[i][MOD_CONFIGURATION_START_ELEMENTS_FLASHLIGHT] = true;
		this->startConfiguration[i][MOD_CONFIGURATION_START_ELEMENTS_HANDGUN] = false;
		this->startConfiguration[i][MOD_CONFIGURATION_START_ELEMENTS_SHOTGUN] = false;

		// Start configuration value
		this->startConfigurationValue[i] = new int[MOD_CONFIGURATION_START_VALUE_ARRAY_SIZE];
		this->startConfigurationValue[i][MOD_CONFIGURATION_START_VALUE_HEALTH] = 100;
		this->startConfigurationValue[i][MOD_CONFIGURATION_START_VALUE_ARMOR] = 0;
		this->startConfigurationValue[i][MOD_CONFIGURATION_START_VALUE_HANDGUN] = 0;
		this->startConfigurationValue[i][MOD_CONFIGURATION_START_VALUE_SHOTGUN] = 0;

		// Interface position initialization
		this->interfacePosition[i] = new float[MOD_INTERFACE_POSITION_ARRAY_SIZE];
		this->interfacePosition[i][MOD_INTERFACE_POSITION_HEALTH_TEXT_X] = 0.17f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_HEALTH_TEXT_Y] = 0.02f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_HEALTH_TEXT_SIZE] = 1.5f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_HEALTH_LOGO_X] = 0.03f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_HEALTH_LOGO_Y] = 0.02f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_HEALTH_LOGO_SIZE_X] = 0.03f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_HEALTH_LOGO_SIZE_Y] = 0.03f;

		this->interfacePosition[i][MOD_INTERFACE_POSITION_ARMOR_TEXT_X] = 0.38f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_ARMOR_TEXT_Y] = 0.02f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_ARMOR_TEXT_SIZE] = 1.5f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_ARMOR_LOGO_X] = 0.25f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_ARMOR_LOGO_Y] = 0.02f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_ARMOR_LOGO_SIZE_X] = 0.03f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_ARMOR_LOGO_SIZE_Y] = 0.03f;

		this->interfacePosition[i][MOD_INTERFACE_POSITION_AMMO_TEXT_X] = 0.98f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_AMMO_TEXT_Y] = 0.02f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_AMMO_TEXT_SIZE] = 1.5f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_AMMO_LOGO_X] = 0.82f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_AMMO_LOGO_Y] = 0.02f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_AMMO_LOGO_SIZE_X] = 0.03f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_AMMO_LOGO_SIZE_Y] = 0.03f;

		this->interfacePosition[i][MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_X] = 0.85f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_Y] = 0.90f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_SIZE_X] = 0.2f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_SIZE_Y] = 0.02f;

		this->interfacePosition[i][MOD_INTERFACE_POSITION_BLUEKEY_LOGO_X] = 0.85f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_BLUEKEY_LOGO_Y] = 0.87f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_BLUEKEY_LOGO_SIZE_X] = 0.2f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_BLUEKEY_LOGO_SIZE_Y] = 0.02f;

		this->interfacePosition[i][MOD_INTERFACE_POSITION_REDKEY_LOGO_X] = 0.85f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_REDKEY_LOGO_Y] = 0.84f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_REDKEY_LOGO_SIZE_X] = 0.2f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_REDKEY_LOGO_SIZE_Y] = 0.02f;

		this->interfacePosition[i][MOD_INTERFACE_POSITION_GREENKEY_LOGO_X] = 0.85f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_GREENKEY_LOGO_Y] = 0.81f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_GREENKEY_LOGO_SIZE_X] = 0.2f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_GREENKEY_LOGO_SIZE_Y] = 0.02f;

		this->interfacePosition[i][MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_X] = 0.95f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_Y] = 0.95f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_SIZE_X] = 0.04f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_SIZE_Y] = 0.04f;

		this->interfacePosition[i][MOD_INTERFACE_POSITION_MESSAGE_X] = 0.03f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_MESSAGE_Y] = 0.4f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_MESSAGE_SIZE] = 0.8f;
		this->interfacePosition[i][MOD_INTERFACE_POSITION_MESSAGE_MAX_LINES] = 3.0f;
	}

	int index = 0;

	// Read mods data
#ifdef _WIN32

	hfind = FindFirstFileW(L"mods/*.*", &wfd);
	do
	{
		// Make sure it's not ./ or ../
		if(wfd.cFileName[0] != '.')
		{
			// If it's a directory
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				std::ifstream file;

				// Convert WCHAR to String
				std::wstring wtest = wfd.cFileName;
				std::string tmpString(wtest.begin(),wtest.end());
				tmpString.assign(wtest.begin(),wtest.end());
#else

	pDIR = opendir("mods/");
	pDirEnt = readdir( pDIR );

	while ( pDirEnt != NULL )
	{
		// Make sure it's not ./ or ../
		if(strcmp(pDirEnt->d_name,".") != 0 && strcmp(pDirEnt->d_name,"..") != 0)
		{
			// If it's a directory
			if (pDirEnt->d_type == DT_DIR)
			{
				std::string tmpString = pDirEnt->d_name;
#endif
				
				if(this->optM->isDebug())
					this->logM->logIt(std::string("Reading mod directory:"),tmpString);

				// Read the conf.xml file
				stringstream.str("");
				stringstream << "mods/" << tmpString << "/conf.xml";
				TiXmlDocument doc(stringstream.str());
				if(doc.LoadFile())
				{
					float tmpFloat;
					int tmpInt;
					std::string tmpString2 = langM->getText(TEXT_MOD_NONAME);
					std::istringstream iss;

					TiXmlHandle hdl(&doc);
					TiXmlElement * elem1 = NULL;
					TiXmlElement * elem2 = NULL;
					TiXmlElement * elem3 = NULL;

					// Mod name
					elem1 = hdl.FirstChildElement("mod").Element();
					if(elem1 != NULL)
					{
						if(elem1->Attribute("name") != NULL)
							tmpString2 = elem1->Attribute("name");
					}
					this->modsName[index] = tmpString2;

					// Mod Root Directory
					stringstream.str("");
					stringstream << "mods/" << tmpString << "/";
					this->modRootDirectory[index] = stringstream.str();

					// Levels informations
					elem1 = hdl.FirstChildElement("mod").FirstChildElement("levels").Element();
					if(elem1 != NULL)
					{
						if(elem1->Attribute("number"))
							tmpInt = (int)atoi(elem1->Attribute("number"));
						else
							tmpInt = 1;

						// Fill variables
						this->numberOfLevelsPerMod[index] = tmpInt;
						this->levelsFile[index] = new std::string[tmpInt];
						this->levelsName[index] = new std::string[tmpInt];
						this->musicsFile[index] = new std::string[tmpInt];
						this->intersectionBlock[index] = new bool[tmpInt];
						this->ambientLight[index] = new int[tmpInt];

						// Fill the level information with default value
						for(int i = 0 ; i < tmpInt ; i++)
						{
							stringstream.str("");
							stringstream << "mods/" << this->mainModDirectoryName << "/levels/level1.dlf";
							this->levelsFile[index][i] = stringstream.str();
							this->levelsName[index][i] = this->langM->getText(TEXT_MOD_NONAME);
							this->musicsFile[index][i] = "sounds/musics/intro.wav";
							this->intersectionBlock[index][i] = true;
							this->ambientLight[index][i] = 100;
						}

						// Fill the level information
						for(int i = 0 ; i < tmpInt ; i++)
						{
							if(i == 0)
								elem2 = elem1->FirstChildElement("level");
							else
								elem2 = elem2->NextSiblingElement("level");

							if(elem2 != NULL)
							{
								// Get the position, must be present
								int levelId = -1;
								elem3 = elem2->FirstChildElement("position");
								if(elem3 != NULL && elem3->GetText() != NULL)
									levelId = (int)atoi(elem3->GetText());

								if(levelId > -1 && levelId < tmpInt)
								{
									// Get the level filename
									elem3 = elem2->FirstChildElement("filename");
									if(elem3 != NULL && elem3->GetText() != NULL)
									{
										stringstream.str("");
										stringstream << "mods/" << tmpString << "/levels/" << elem3->GetText() << ".dlf";
										this->levelsFile[index][levelId] = stringstream.str();
									}

									// Get the level filename
									elem3 = elem2->FirstChildElement("name");
									if(elem3 != NULL && elem3->GetText() != NULL)
										this->levelsName[index][levelId] = elem3->GetText();

									// Get the music filename
									elem3 = elem2->FirstChildElement("music");
									if(elem3 != NULL && elem3->GetText() != NULL)
									{
										stringstream.str("");
										stringstream << "mods/" << tmpString << "/musics/" << elem3->GetText() << ".wav";
										
										std::ifstream fileMusicExist;
										fileMusicExist.open(stringstream.str().c_str(), std::ios::in);

										// If it exists we use it
										if(fileMusicExist)
										{
											this->musicsFile[index][levelId] = stringstream.str();
											fileMusicExist.close();
										}
										else
										{
											// If not, we use the default music directory
											stringstream.str("");
											stringstream << "sounds/musics/" << elem3->GetText() << ".wav";
											this->musicsFile[index][levelId] = stringstream.str();
										}
									}

									// Get the intersection blocks option
									elem3 = elem2->FirstChildElement("intersection_block");
									if(elem3 != NULL && elem3->GetText() != NULL)
									{
										if(strcmp(elem3->GetText(),"enabled") == 0)
											this->intersectionBlock[index][levelId] = true;
										else
											this->intersectionBlock[index][levelId] = false;
									}

									// Get the ambient light option
									elem3 = elem2->FirstChildElement("ambient_light");
									if(elem3 != NULL && elem3->GetText() != NULL)
										this->ambientLight[index][levelId] = (int)atoi(elem3->GetText());
								}
							}
						}
					}

					// Player information modification
					elem1 = hdl.FirstChildElement("mod").FirstChildElement("player").FirstChildElement("health_start").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpInt = (int)atoi(elem1->GetText());
						if(tmpInt > 0 && tmpInt <= 100)
							this->startConfigurationValue[index][MOD_CONFIGURATION_START_VALUE_HEALTH] = tmpInt;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("player").FirstChildElement("armor_start").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpInt = (int)atoi(elem1->GetText());
						if(tmpInt > 0 && tmpInt <= 100)
							this->startConfigurationValue[index][MOD_CONFIGURATION_START_VALUE_ARMOR] = tmpInt;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("player").FirstChildElement("handgun").FirstChildElement("start").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						if(strcmp(elem1->GetText(),"yes") == 0)
							this->startConfiguration[index][MOD_CONFIGURATION_START_ELEMENTS_HANDGUN] = true;
						else
							this->startConfiguration[index][MOD_CONFIGURATION_START_ELEMENTS_HANDGUN] = false;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("player").FirstChildElement("shotgun").FirstChildElement("start").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						if(strcmp(elem1->GetText(),"yes") == 0)
							this->startConfiguration[index][MOD_CONFIGURATION_START_ELEMENTS_SHOTGUN] = true;
						else
							this->startConfiguration[index][MOD_CONFIGURATION_START_ELEMENTS_SHOTGUN] = false;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("player").FirstChildElement("handgun").FirstChildElement("ammo").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpInt = (int)atoi(elem1->GetText());
						if(tmpInt > 0)
							this->startConfigurationValue[index][MOD_CONFIGURATION_START_VALUE_HANDGUN] = tmpInt;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("player").FirstChildElement("shotgun").FirstChildElement("ammo").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpInt = (int)atoi(elem1->GetText());
						if(tmpInt > 0)
							this->startConfigurationValue[index][MOD_CONFIGURATION_START_VALUE_SHOTGUN] = tmpInt;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("player").FirstChildElement("flashlight_start").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						if(strcmp(elem1->GetText(),"yes") == 0)
							this->startConfiguration[index][MOD_CONFIGURATION_START_ELEMENTS_FLASHLIGHT] = true;
						else
							this->startConfiguration[index][MOD_CONFIGURATION_START_ELEMENTS_FLASHLIGHT] = false;
					}


					// Interface Modifications
					// Health Text
					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("health").FirstChildElement("text_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_HEALTH_TEXT_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("health").FirstChildElement("text_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_HEALTH_TEXT_Y] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("health").FirstChildElement("text_size").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_HEALTH_TEXT_SIZE] = tmpFloat;
					}

					// Health Logo
					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("health").FirstChildElement("logo_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_HEALTH_LOGO_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("health").FirstChildElement("logo_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_HEALTH_LOGO_Y] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("health").FirstChildElement("logo_size_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_HEALTH_LOGO_SIZE_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("health").FirstChildElement("logo_size_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_HEALTH_LOGO_SIZE_Y] = tmpFloat;
					}

					// Armor Text
					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("armor").FirstChildElement("text_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_ARMOR_TEXT_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("armor").FirstChildElement("text_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_ARMOR_TEXT_Y] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("armor").FirstChildElement("text_size").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_ARMOR_TEXT_SIZE] = tmpFloat;
					}

					// Armor Logo
					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("armor").FirstChildElement("logo_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_ARMOR_LOGO_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("armor").FirstChildElement("logo_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_ARMOR_LOGO_Y] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("armor").FirstChildElement("logo_size_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_ARMOR_LOGO_SIZE_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("armor").FirstChildElement("logo_size_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_ARMOR_LOGO_SIZE_Y] = tmpFloat;
					}

					// Ammo Text
					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("ammo").FirstChildElement("text_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_AMMO_TEXT_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("ammo").FirstChildElement("text_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_AMMO_TEXT_Y] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("ammo").FirstChildElement("text_size").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_AMMO_TEXT_SIZE] = tmpFloat;
					}

					// Ammo Logo
					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("ammo").FirstChildElement("logo_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_AMMO_LOGO_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("ammo").FirstChildElement("logo_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_AMMO_LOGO_Y] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("ammo").FirstChildElement("logo_size_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_AMMO_LOGO_SIZE_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("ammo").FirstChildElement("logo_size_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_AMMO_LOGO_SIZE_Y] = tmpFloat;
					}

					// Keys
					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("yellow_key").FirstChildElement("logo_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("yellow_key").FirstChildElement("logo_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_Y] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("yellow_key").FirstChildElement("logo_size_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_SIZE_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("yellow_key").FirstChildElement("logo_size_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_SIZE_Y] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("blue_key").FirstChildElement("logo_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_BLUEKEY_LOGO_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("blue_key").FirstChildElement("logo_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_BLUEKEY_LOGO_Y] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("blue_key").FirstChildElement("logo_size_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_BLUEKEY_LOGO_SIZE_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("blue_key").FirstChildElement("logo_size_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_BLUEKEY_LOGO_SIZE_Y] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("red_key").FirstChildElement("logo_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_REDKEY_LOGO_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("red_key").FirstChildElement("logo_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_REDKEY_LOGO_Y] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("red_key").FirstChildElement("logo_size_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_REDKEY_LOGO_SIZE_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("red_key").FirstChildElement("logo_size_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_REDKEY_LOGO_SIZE_Y] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("green_key").FirstChildElement("logo_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_GREENKEY_LOGO_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("green_key").FirstChildElement("logo_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_GREENKEY_LOGO_Y] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("green_key").FirstChildElement("logo_size_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_GREENKEY_LOGO_SIZE_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("green_key").FirstChildElement("logo_size_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_GREENKEY_LOGO_SIZE_Y] = tmpFloat;
					}

					// Flashlight logo
					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("flashlight").FirstChildElement("logo_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("flashlight").FirstChildElement("logo_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_Y] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("flashlight").FirstChildElement("logo_size_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_SIZE_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("flashlight").FirstChildElement("logo_size_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_SIZE_Y] = tmpFloat;
					}

					// Messages
					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("messages").FirstChildElement("text_x").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_MESSAGE_X] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("messages").FirstChildElement("text_y").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						this->interfacePosition[index][MOD_INTERFACE_POSITION_MESSAGE_Y] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("messages").FirstChildElement("text_size").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_MESSAGE_SIZE] = tmpFloat;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("interface").FirstChildElement("messages").FirstChildElement("max_lines").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						tmpFloat = (float)atof(elem1->GetText());
						if(tmpFloat > 0)
							this->interfacePosition[index][MOD_INTERFACE_POSITION_MESSAGE_MAX_LINES] = tmpFloat;
					}

					// Misc Modifications
					elem1 = hdl.FirstChildElement("mod").FirstChildElement("misc").FirstChildElement("loading_text").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						if(strcmp(elem1->GetText(),"enabled") == 0)
							this->levelsConfiguration[index][MOD_CONFIGURATION_LEVELS_LOADING] = true;
						else
							this->levelsConfiguration[index][MOD_CONFIGURATION_LEVELS_LOADING] = false;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("misc").FirstChildElement("custom_textures").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						if(strcmp(elem1->GetText(),"enabled") == 0)
							this->levelsConfiguration[index][MOD_CONFIGURATION_LEVELS_TEXTURES] = true;
						else
							this->levelsConfiguration[index][MOD_CONFIGURATION_LEVELS_TEXTURES] = false;
					}

					elem1 = hdl.FirstChildElement("mod").FirstChildElement("misc").FirstChildElement("custom_sounds").Element();
					if(elem1 != NULL && elem1->GetText() != NULL)
					{
						if(strcmp(elem1->GetText(),"enabled") == 0)
							this->levelsConfiguration[index][MOD_CONFIGURATION_LEVELS_SOUNDS] = true;
						else
							this->levelsConfiguration[index][MOD_CONFIGURATION_LEVELS_SOUNDS] = false;
					}
				}
				else
				{
					if(this->optM->isDebug())
						this->logM->logIt(std::string("Cannot find conf.txt file."),std::string("Using default values"));

					// If conf.xml file is not found, we fill with default value to prevent crash
					this->modsName[index] = langM->getText(TEXT_MOD_INVALID);
					this->numberOfLevelsPerMod[index] = 1;
					this->levelsFile[index] = new std::string[1];
					this->musicsFile[index] = new std::string[1];
					this->intersectionBlock[index] = new bool[1];
					this->ambientLight[index] = new int[1];

					stringstream.str("");
					stringstream << "mods/" << this->mainModDirectoryName << "/levels/level1.dlf";
					this->levelsFile[index][0] = stringstream.str();
					this->musicsFile[index][0] = "sounds/musics/music1.wav";
					this->intersectionBlock[index][0] = true;
					this->ambientLight[index][0] = 100;
				}

				// Increase index
				index++;
			}
		}

#ifdef _WIN32

	}
	while(FindNextFile(hfind, &wfd));
	FindClose(hfind);

#else
	
	pDirEnt = readdir( pDIR );
	}
	closedir( pDIR );

	delete pDirEnt;
	pDIR = NULL;
	pDirEnt = NULL;

#endif

}
