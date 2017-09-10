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

#ifndef __TEXTMANAGER_H
#define __TEXTMANAGER_H

#include <SDL/SDL_mixer.h>
#include <string>
#include <map>
#include "../manager/texture.h"
#include "../includes/singleton.h"

#define CHARACTERSCREENSIZE 0.02f
#define CHARACTERFONTSIZE 0.0624f
#define CHARACTERFONTSIZE2 0.0590f

enum
{
	TEXTMANAGER_ALIGN_LEFT,
	TEXTMANAGER_ALIGN_CENTER,
	TEXTMANAGER_ALIGN_RIGHT
};

/**
* TextManager Class
* Handle text in the game
* @author Stephane Baudoux
*/
class TextManager : public CSingleton<TextManager>
{
	friend class CSingleton<TextManager>;

private:
	float ratio;
	int fontId;
	int height;
	int width;
	CTextureManager * texturesManager;
	std::map<std::string, Mix_Chunk *>::iterator extendedIterator;
	std::map<std::string, Mix_Chunk *>extended;

public:
	void loadText();
	void drawText(std::string text, float x, float y, float size, int type, float r, float g, float b, float alpha = 1.0f);

private:
	TextManager();
	~TextManager();
};

#endif
