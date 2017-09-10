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

#ifndef __MOUSEMANAGER_H
#define __MOUSEMANAGER_H

#include <SDL/SDL.h>
#include "../includes/singleton.h"

enum
{
	MOUSE_LEFT_BUTTON,
	MOUSE_MIDDLE_BUTTON,
	MOUSE_RIGHT_BUTTON,
	MOUSE_WHEEL_UP_BUTTON,
	MOUSE_WHEEL_DOWN_BUTTON,
	MOUSE_ARRAY_SIZE
};

enum
{
	MOUSE_RULE_RESET_ON_UP,
	MOUSE_RULE_DO_NOTHING_ON_UP
};

/**
* MouseManager Class
* Handle mouse stroke
* @author Stephane Baudoux
*/
class MouseManager : public CSingleton<MouseManager>
{
	friend class CSingleton<MouseManager>;

private:
	float mouseSensibility;
	int mouseDelay;
	int invertYMouseAxis;
	int mouseSmoothing;
	int ** mouseButtons;

	SDL_Event event;

public:
	void mouseManagment(SDL_Event event);
	float getNewViewX();
	float getNewViewY();
	void refreshValues();
	bool isMouseButtonPressed(int v);
	void setMouseButton(int button, int v);

private:
	MouseManager();
	~MouseManager();
};

#endif
