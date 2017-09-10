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
#include <SDL/SDL.h>
#include "../manager/texture.h"
#include "../manager/textManager.h"
#include "../manager/loadingManager.h"

/**
* Constructor
*/
LoadingManager::LoadingManager()
{
	this->langM = LangManager::getInstance();
	this->modM = ModManager::getInstance();
	this->optionM = OptionManager::getInstance();
	this->sentence = langM->getText(TEXT_LOADING);
	loadLoading();
}

/**
* Load the loading texture
*/
void LoadingManager::loadLoading()
{
	// Wide resolution
	if(this->optionM->getResolutionNumber() > 4)
	{
		this->loadingId = CTextureManager::getInstance()->LoadTexture("imgs/menus/loading_wide.tga");
		this->yRatio = 0.625f;
	}
	else
	{
		// 4/3 resolution
		this->loadingId = CTextureManager::getInstance()->LoadTexture("imgs/menus/loading.tga");
		this->yRatio = 0.75f;
	}
}

/**
* Update the loading and draw it
* @param sentence Text
* @param sleepTime Time to sleep before the next update
*/
void LoadingManager::update(std::string sentence, int sleepTime)
{
	this->sentence = sentence;
	draw();

	// Add a sleep to decrease a little the drawing speed
	if(sleepTime > 0)
	{
		#ifdef _WIN32
			Sleep(sleepTime * 1000);
		#else
			sleep(sleepTime);	
		#endif
	}
}

/**
* Update the loading and draw it
* @param sleepTime Time to sleep before the next update
*/
void LoadingManager::update(int sleepTime)
{
	drawWithoutState();

	// Add a sleep to decrease a little the drawing speed
	if(sleepTime > 0)
	{
		#ifdef _WIN32
			Sleep(sleepTime * 1000);
		#else
			sleep(sleepTime);	
		#endif
	}
}

/**
* Draw the loading
*/
void LoadingManager::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBindTexture  ( GL_TEXTURE_2D, loadingId );
	glBegin(GL_QUADS);
		glTexCoord2d(0.0,0.0);			glVertex2d(0.0,0.0);
		glTexCoord2d(1.0,0.0);			glVertex2d(1.0,0.0);
		glTexCoord2d(1.0,this->yRatio);	glVertex2d(1.0,1.0); 	
		glTexCoord2d(0.0,this->yRatio);	glVertex2d(0.0,1.0);
	glEnd();

	if(this->modM->getLevelsConfiguration(MOD_CONFIGURATION_LEVELS_LOADING))
		TextManager::getInstance()->drawText(langM->getText(TEXT_LOADING),0.5f,0.5f,3.0f,TEXTMANAGER_ALIGN_CENTER,1.0f,1.0f,1.0f);

	// Draw the loading text
	TextManager::getInstance()->drawText(this->sentence,0.5f,0.3f,1.0f,TEXTMANAGER_ALIGN_CENTER,1.0f,1.0f,1.0f);
	SDL_GL_SwapBuffers();
}

/**
* Draw the loading without text
*/
void LoadingManager::drawWithoutState()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBindTexture  ( GL_TEXTURE_2D, loadingId );
	glBegin(GL_QUADS);
		glTexCoord2d(0.0,0.0);			glVertex2d(0.0,0.0);
		glTexCoord2d(1.0,0.0);			glVertex2d(1.0,0.0);
		glTexCoord2d(1.0,this->yRatio);	glVertex2d(1.0,1.0); 	
		glTexCoord2d(0.0,this->yRatio);	glVertex2d(0.0,1.0);
	glEnd();

	if(this->modM->getLevelsConfiguration(MOD_CONFIGURATION_LEVELS_LOADING))
		TextManager::getInstance()->drawText(langM->getText(TEXT_LOADING),0.5f,0.5f,3.0f,TEXTMANAGER_ALIGN_CENTER,1.0f,1.0f,1.0f);

	SDL_GL_SwapBuffers();
}
