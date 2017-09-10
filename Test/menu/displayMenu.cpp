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

#include "../manager/impactManager.h"
#include "../manager/loadingManager.h"
#include "../menu/displayMenu.h"

/**
* Constructor
*/
DisplayMenu::DisplayMenu() : MenuModification()
{
	this->level[0] = 0;
	this->level[1] = 2;
	this->level[2] = 4;
	this->level[3] = 8;
	this->level[4] = 16;

	this->actionSize = 11;
	this->modificationSize = new int[this->actionSize];
	this->modificationChoosen = new int[this->actionSize];

	this->modificationSize[0]  = 9;
	this->modificationSize[1]  = 2;
	this->modificationSize[2]  = 4;
	this->modificationSize[3]  = 4;
	this->modificationSize[4]  = 5;
	this->modificationSize[5]  = 5;
	this->modificationSize[6]  = 4;
	this->modificationSize[7]  = 4;
	this->modificationSize[8]  = 7;
	this->modificationSize[9]  = 17;
	this->modificationSize[10] = this->langM->getNumberOfLang();

	this->resolutionCompatibility = new int[this->modificationSize[0]];

	this->modificationChoosen[0] = optM->getResolutionNumber();

	if(this->optM->isFullScreen())
		this->modificationChoosen[1] = 1;
	else
		this->modificationChoosen[1] = 0;

	this->modificationChoosen[2] = this->optM->getTexturesQuality();
	this->modificationChoosen[3] = this->optM->getLightQuality();

	switch(this->optM->getAntialiasingLevel())
	{
	case 0:
		this->modificationChoosen[4] = 0;
		break;

	case 2:
		this->modificationChoosen[4] = 1;
		break;

	case 4:
		this->modificationChoosen[4] = 2;
		break;

	case 8:
		this->modificationChoosen[4] = 3;
		break;

	case 16:
		this->modificationChoosen[4] = 4;
		break;
	}

	switch(this->optM->getTextureFilteringLevel())
	{
	case 0:
		this->modificationChoosen[5] = 0;
		break;

	case 2:
		this->modificationChoosen[5] = 1;
		break;

	case 4:
		this->modificationChoosen[5] = 2;
		break;

	case 8:
		this->modificationChoosen[5] = 3;
		break;

	case 16:
		this->modificationChoosen[5] = 4;
		break;
	}
	this->modificationChoosen[6] = this->optM->getImpactNumberValue();
	this->modificationChoosen[7] = this->optM->getParticleModifierValue();
	this->modificationChoosen[8] = this->optM->getGlobalVisibility();
	this->modificationChoosen[9] = this->optM->getGamma();
	this->modificationChoosen[10] = this->optM->getLanguage();

	this->modificationList = new int * [this->actionSize];
	for(int i = 0 ; i < this->actionSize ; i++)
	{
		modificationList[i] = new int [this->modificationSize[i]];
		for(int j = 0 ; j < this->modificationSize[i] ; j++)
			modificationList[i][j] = 0;
	}

	this->title = TEXT_DISPLAY;
	this->previousMenuCode = ID_MENUOPTION;
	this->menuCode = ID_MENUOPTIONDISPLAY;
	this->choosenMenuCode = this->menuCode;

	this->actionList = new int[this->actionSize];
	this->actionList[0] = TEXT_RESOLUTION;
	this->actionList[1] = TEXT_FULLSCREEN;
	this->actionList[2] = TEXT_TEXTURESQUALITY;
	this->actionList[3] = TEXT_LIGHTQUALITY;
	this->actionList[4] = TEXT_ANTIALIASING;
	this->actionList[5] = TEXT_ANISOTROPICFILTER;
	this->actionList[6] = TEXT_IMPACTQUANTITY;
	this->actionList[7] = TEXT_PARTICLEQUANTITY;
	this->actionList[8] = TEXT_VIEWDEPTH;
	this->actionList[9] = TEXT_GAMMA;
	this->actionList[10] = TEXT_LANGUAGE;

	// Resolution
	this->modificationList[0][0] = TEXT_RES_640;
	this->modificationList[0][1] = TEXT_RES_800;
	this->modificationList[0][2] = TEXT_RES_1024;
	this->modificationList[0][3] = TEXT_RES_1280;
	this->modificationList[0][4] = TEXT_RES_1600;
	this->modificationList[0][5] = TEXT_RES_1280W;
	this->modificationList[0][6] = TEXT_RES_1440W;
	this->modificationList[0][7] = TEXT_RES_1680W;
	this->modificationList[0][8] = TEXT_RES_1920W;

	// Resolution compatibility
	for(int i = 0 ; i < this->modificationSize[0] ; i++)
		this->resolutionCompatibility[i] = SDL_VideoModeOK(this->optM->getResolutionWidth(i),this->optM->getResolutionHeight(i),32,SDL_FULLSCREEN);

	// FullScreen
	this->modificationList[1][0] = TEXT_NO;
	this->modificationList[1][1] = TEXT_YES;

	// Textures
	this->modificationList[2][0] = TEXT_VERYLOW;
	this->modificationList[2][1] = TEXT_LOW;
	this->modificationList[2][2] = TEXT_MEDIUM;
	this->modificationList[2][3] = TEXT_HIGH;

	// Light
	this->modificationList[3][0] = TEXT_VERYLOW;
	this->modificationList[3][1] = TEXT_LOW;
	this->modificationList[3][2] = TEXT_MEDIUM;
	this->modificationList[3][3] = TEXT_HIGH;

	// Antialiasing
	this->modificationList[4][0] = TEXT_0X;
	this->modificationList[4][1] = TEXT_2X;
	this->modificationList[4][2] = TEXT_4X;
	this->modificationList[4][3] = TEXT_8X;
	this->modificationList[4][4] = TEXT_16X;

	// Filtering
	this->modificationList[5][0] = TEXT_0X;
	this->modificationList[5][1] = TEXT_2X;
	this->modificationList[5][2] = TEXT_4X;
	this->modificationList[5][3] = TEXT_8X;
	this->modificationList[5][4] = TEXT_16X;

	// Impacts
	this->modificationList[6][0] = TEXT_DISABLED;
	this->modificationList[6][1] = TEXT_LOW;
	this->modificationList[6][2] = TEXT_MEDIUM;
	this->modificationList[6][3] = TEXT_HIGH;

	// Particles
	this->modificationList[7][0] = TEXT_DISABLED;
	this->modificationList[7][1] = TEXT_LOW;
	this->modificationList[7][2] = TEXT_MEDIUM;
	this->modificationList[7][3] = TEXT_HIGH;

	// View
	this->modificationList[8][0] = TEXT_NEAR0;
	this->modificationList[8][1] = TEXT_NEAR1;
	this->modificationList[8][2] = TEXT_NEAR2;
	this->modificationList[8][3] = TEXT_MEDIUM;
	this->modificationList[8][4] = TEXT_FAR0;
	this->modificationList[8][5] = TEXT_FAR1;
	this->modificationList[8][6] = TEXT_FAR2;

	// Gamma
	this->modificationList[9][0] = TEXT_PERCENT40;
	this->modificationList[9][1] = TEXT_PERCENT50;
	this->modificationList[9][2] = TEXT_PERCENT60;
	this->modificationList[9][3] = TEXT_PERCENT70;
	this->modificationList[9][4] = TEXT_PERCENT80;
	this->modificationList[9][5] = TEXT_PERCENT90;
	this->modificationList[9][6] = TEXT_PERCENT100;
	this->modificationList[9][7] = TEXT_PERCENT110;
	this->modificationList[9][8] = TEXT_PERCENT120;
	this->modificationList[9][9] = TEXT_PERCENT130;
	this->modificationList[9][10] = TEXT_PERCENT140;
	this->modificationList[9][11] = TEXT_PERCENT150;
	this->modificationList[9][12] = TEXT_PERCENT160;
	this->modificationList[9][13] = TEXT_PERCENT170;
	this->modificationList[9][14] = TEXT_PERCENT180;
	this->modificationList[9][15] = TEXT_PERCENT190;
	this->modificationList[9][16] = TEXT_PERCENT200;

	// Language
	for(int i = 0 ; i < this->langM->getNumberOfLang() ; i++)
		this->modificationList[10][i] = TEXT_LANGNAME;
}

/**
* Destructor
*/
DisplayMenu::~DisplayMenu()
{
	delete[] this->resolutionCompatibility;
	this->resolutionCompatibility = NULL;
}

/**
* Refresh the Display menu
* @param time Elapsed time
*/
void DisplayMenu::refresh(long time)
{
	Menu::refresh(time);
	if(keyM->isKeyPressed(KEYBOARD_MENU_LEFT_KEY))
	{
		this->keyM->setKey(KEYBOARD_MENU_LEFT_KEY,0);
		this->modificationMoved = true;
		if(--this->modificationChoosen[this->actionChoosen] < 0)
			this->modificationChoosen[this->actionChoosen] = this->modificationSize[this->actionChoosen] - 1;

		if(this->actionChoosen == 0)
		{
			while(this->resolutionCompatibility[this->modificationChoosen[0]] == 0)
			{
				if(--this->modificationChoosen[0] < 0)
					this->modificationChoosen[0] = this->modificationSize[0] - 1;
			}
		}
	}

	if(keyM->isKeyPressed(KEYBOARD_MENU_RIGHT_KEY))
	{
		this->keyM->setKey(KEYBOARD_MENU_RIGHT_KEY,0);
		this->modificationMoved = true;
		if(++this->modificationChoosen[this->actionChoosen] >= this->modificationSize[this->actionChoosen])
			this->modificationChoosen[this->actionChoosen] = 0;

		if(this->actionChoosen == 0)
		{
			while(this->resolutionCompatibility[this->modificationChoosen[0]] == 0)
			{
				if(++this->modificationChoosen[0] >= this->modificationSize[0])
					this->modificationChoosen[0] = 0;
			}
		}
	}

	if(this->modificationMoved)
	{
		this->modificationMoved = false;
		switch(this->actionChoosen)
		{
		case 0:
			optM->setResolution(this->modificationChoosen[this->actionChoosen]);
			break;

		case 1:
			optM->setFullScreen(this->modificationChoosen[this->actionChoosen]);
			break;

		case 2:
			optM->setTexturesQuality(this->modificationChoosen[this->actionChoosen]);
			texturesManager->ReleaseTextures();
			textM->loadText();
			LoadingManager::getInstance()->loadLoading();
			loadTexture();
			break;

		case 3:
			optM->setLightQuality(this->modificationChoosen[this->actionChoosen]);
			break;

		case 4:
			optM->setAntialiasingLevel(this->level[this->modificationChoosen[this->actionChoosen]]);
			break;

		case 5:
			optM->setTextureFilteringLevel(this->level[this->modificationChoosen[this->actionChoosen]]);
			break;

		case 6:
			optM->setImpactNumberValue(this->modificationChoosen[this->actionChoosen]);
			ImpactManager::getInstance()->updateImpactValue();
			break;

		case 7:
			optM->setParticleModifierValue(this->modificationChoosen[this->actionChoosen]);
			break;

		case 8:
			optM->setGlobalVisibility(this->modificationChoosen[this->actionChoosen]);
			break;

		case 9:
			optM->setGamma(this->modificationChoosen[this->actionChoosen]);
			SDL_SetGamma((float)this->modificationChoosen[this->actionChoosen] / 10.0f + 0.5f, (float)this->modificationChoosen[this->actionChoosen] / 10.0f + 0.5f, (float)this->modificationChoosen[this->actionChoosen] / 10.0f + 0.5f);
			break;

		case 10:
			optM->setLanguage(this->modificationChoosen[this->actionChoosen]);
			langM->reloadLangDatabase();
			break;
		}
	}
}
