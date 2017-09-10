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

#include "../manager/optionManager.h"
#include "../manager/mouseManager.h"

/**
* Constructor
*/
MouseManager::MouseManager()
{
	this->mouseButtons = new int*[MOUSE_ARRAY_SIZE];
	for(int i = 0 ; i < MOUSE_ARRAY_SIZE ; i++)
		this->mouseButtons[i] = new int[3];

	this->mouseButtons[MOUSE_LEFT_BUTTON][0] = 0;
	this->mouseButtons[MOUSE_RIGHT_BUTTON][0] = 0;
	this->mouseButtons[MOUSE_MIDDLE_BUTTON][0] = 0;
	this->mouseButtons[MOUSE_WHEEL_UP_BUTTON][0] = 0;
	this->mouseButtons[MOUSE_WHEEL_DOWN_BUTTON][0] = 0;

	this->mouseButtons[MOUSE_LEFT_BUTTON][1] = SDL_BUTTON_LEFT;
	this->mouseButtons[MOUSE_RIGHT_BUTTON][1] = SDL_BUTTON_RIGHT;
	this->mouseButtons[MOUSE_MIDDLE_BUTTON][1] = SDL_BUTTON_MIDDLE;
	this->mouseButtons[MOUSE_WHEEL_UP_BUTTON][1] = SDL_BUTTON_WHEELUP;
	this->mouseButtons[MOUSE_WHEEL_DOWN_BUTTON][1] = SDL_BUTTON_WHEELDOWN;

	this->mouseButtons[MOUSE_LEFT_BUTTON][2] = MOUSE_RULE_RESET_ON_UP;
	this->mouseButtons[MOUSE_RIGHT_BUTTON][2] = MOUSE_RULE_RESET_ON_UP;
	this->mouseButtons[MOUSE_MIDDLE_BUTTON][2] = MOUSE_RULE_RESET_ON_UP;
	this->mouseButtons[MOUSE_WHEEL_UP_BUTTON][2] = MOUSE_RULE_DO_NOTHING_ON_UP;
	this->mouseButtons[MOUSE_WHEEL_DOWN_BUTTON][2] = MOUSE_RULE_DO_NOTHING_ON_UP;

	this->mouseDelay = 0;
	refreshValues();
}

/**
* Destructor
*/
MouseManager::~MouseManager()
{
	for(int i = 0 ; i < MOUSE_ARRAY_SIZE ; i++)
	{
		delete this->mouseButtons[i];
		this->mouseButtons[i] = NULL;
	}
	delete this->mouseButtons;
	this->mouseButtons = NULL;
}

/**
* Refresh mouse information
*/
void MouseManager::refreshValues()
{
	this->mouseSensibility = (float)OptionManager::getInstance()->getMouseSensibility() / 10.0f + MOUSESENSIBILITYMODIFIER;
	this->invertYMouseAxis = OptionManager::getInstance()->getInvertYMouseAxis();
	this->mouseSmoothing = OptionManager::getInstance()->getMouseSmoothing() + 5;
}

/**
* Manage mouse event
* @param event Mouse event
*/
void MouseManager::mouseManagment(SDL_Event event)
{
	if(event.type == SDL_MOUSEMOTION)
	{
		int tmpX, tmpY;
		SDL_GetMouseState(&tmpX, &tmpY);

		this->event = event;
		this->mouseDelay = this->mouseSmoothing;
	}
	else
	{
		if(event.type == SDL_MOUSEBUTTONDOWN)
		{
			bool found = false;
			for(int i = 0 ; i < MOUSE_ARRAY_SIZE && !found ; i++)
			{
				if(event.button.button == this->mouseButtons[i][1])
				{
					this->mouseButtons[i][0] = 1;
					found = true;
				}
			}
		}
		else
		{
			if(event.type == SDL_MOUSEBUTTONUP)
			{
				bool found = false;
				for(int i = 0 ; i < MOUSE_ARRAY_SIZE && !found ; i++)
				{
					if(event.button.button == this->mouseButtons[i][1] && this->mouseButtons[i][2] == MOUSE_RULE_RESET_ON_UP)
					{
						this->mouseButtons[i][0] = 0;
						found = true;
					}
				}
			}
		}
	}
}

/**
* Return horizontal stroke on the screen.
* Affected by mouse sensibility adjustment
*/
float MouseManager::getNewViewX()
{
	float x;
	if(this->mouseDelay > 0)
		x = event.motion.xrel * this->mouseSensibility;
	else
		x = 0;

	return x;
}

/**
* Return vertical stroke on the screen
* Affected by mouse sensibility adjustment
*/
float MouseManager::getNewViewY()
{
	float y;
	if(this->mouseDelay > 0)
	{
		y = event.motion.yrel * this->mouseSensibility;
		if(this->invertYMouseAxis == 1)
			y *= -1;
		this->mouseDelay--;
	}
	else
	{
		y = 0;
	}

	return y;
}

/**
* If a mouse button is pressed
* @param v Mouse button number
* @return If the button is pressed
*/
bool MouseManager::isMouseButtonPressed(int v)
{
	bool ret = false;
	if(this->mouseButtons[v][0] == 1)
		ret = true;
	return ret;
}

/**
* Set a mouse button to v
* @param button Mouse button number
* @param v Button value
*/
void MouseManager::setMouseButton(int button, int v)
{
	this->mouseButtons[button][0] = v;
}
