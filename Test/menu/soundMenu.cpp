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

#include "../menu/soundMenu.h"

/**
* Constructor
*/
SoundMenu::SoundMenu() : MenuModification()
{
	this->actionSize = 2;
	this->modificationSize = new int[this->actionSize];
	this->modificationChoosen = new int[this->actionSize];

	this->modificationSize[0] = 11;
	this->modificationSize[1] = 11;

	this->modificationChoosen[0] = this->optM->getSoundVolume();
	this->modificationChoosen[1] = this->optM->getMusicVolume();

	this->modificationList = new int * [this->actionSize];
	for(int i = 0 ; i < this->actionSize ; i++)
	{
		modificationList[i] = new int [this->modificationSize[i]];
		for(int j = 0 ; j < this->modificationSize[i] ; j++)
			modificationList[i][j] = 0;
	}

	this->title = TEXT_SOUND;
	this->previousMenuCode = ID_MENUOPTION;
	this->menuCode = ID_MENUOPTIONSOUND;
	this->choosenMenuCode = this->menuCode;

	this->actionList = new int[this->actionSize];
	this->actionList[0] = TEXT_SOUNDEFFECTS;
	this->actionList[1] = TEXT_MUSIC;

	// Sound effects
	this->modificationList[0][0]  = TEXT_PERCENT0;
	this->modificationList[0][1]  = TEXT_PERCENT10;
	this->modificationList[0][2]  = TEXT_PERCENT20;
	this->modificationList[0][3]  = TEXT_PERCENT30;
	this->modificationList[0][4]  = TEXT_PERCENT40;
	this->modificationList[0][5]  = TEXT_PERCENT50;
	this->modificationList[0][6]  = TEXT_PERCENT60;
	this->modificationList[0][7]  = TEXT_PERCENT70;
	this->modificationList[0][8]  = TEXT_PERCENT80;
	this->modificationList[0][9]  = TEXT_PERCENT90;
	this->modificationList[0][10] = TEXT_PERCENT100;

	// FullScreen
	this->modificationList[1][0]  = TEXT_PERCENT0;
	this->modificationList[1][1]  = TEXT_PERCENT10;
	this->modificationList[1][2]  = TEXT_PERCENT20;
	this->modificationList[1][3]  = TEXT_PERCENT30;
	this->modificationList[1][4]  = TEXT_PERCENT40;
	this->modificationList[1][5]  = TEXT_PERCENT50;
	this->modificationList[1][6]  = TEXT_PERCENT60;
	this->modificationList[1][7]  = TEXT_PERCENT70;
	this->modificationList[1][8]  = TEXT_PERCENT80;
	this->modificationList[1][9]  = TEXT_PERCENT90;
	this->modificationList[1][10] = TEXT_PERCENT100;

	this->soundTest = soundM->loadSound("sounds/misc/soundTest.wav");
}

/**
* Destructor
*/
SoundMenu::~SoundMenu(){}

/**
* Refresh the Sound menu
* @param time Elapsed time
*/
void SoundMenu::refresh(long time)
{
	MenuModification::refresh(time);
	if(this->modificationMoved)
	{
		this->modificationMoved = false;
		switch(this->actionChoosen)
		{
		case 0:
			optM->setSoundVolume(this->modificationChoosen[this->actionChoosen]);
			this->soundM->refreshSoundVolume();
			this->soundM->playSound(this->soundTest);
			break;

		case 1:
			optM->setMusicVolume(this->modificationChoosen[this->actionChoosen]);
			this->soundM->refreshMusicVolume();
			break;
		}
	}
}
