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

#include "../manager/messageManager.h"

/**
* Constructor
*/
MessageManager::MessageManager()
{
	this->modM = ModManager::getInstance();
	this->langM = LangManager::getInstance();
	createList();
}

/**
* Destructor
*/
MessageManager::~MessageManager()
{
	destroyList();
}

/**
* Test if the message exists in the list
* @param text Text to test
* @return If the text exists in the list
*/
bool MessageManager::messageExists(std::string text)
{
	bool founded = false;
	int index2 = this->index - 1;
	if(index2 < 0)
		index2 = (int)this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_MESSAGE_MAX_LINES) - 1;

	if(this->messageList[index2] != NULL && this->messageList[index2]->getText() == text && this->messageList[index2]->getLife() < MESSAGE_MINIMUM_DELAY)
		founded = true;
	return founded;
}

/**
* Create the Message list
*/
void MessageManager::createList()
{
	this->messageList = new Message*[(int)this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_MESSAGE_MAX_LINES)];
	for(int i = 0 ; i < (int)this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_MESSAGE_MAX_LINES) ; i++)
		this->messageList[i] = NULL;

	this->simpleMessage = NULL;
	this->index = 0;
}

/**
* Free the used instances and destroy the list
*/
void MessageManager::destroyList()
{
	if(this->messageList != NULL)
	{
		for(int i = 0 ; i < (int)this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_MESSAGE_MAX_LINES) ; i++)
		{
			if(this->messageList[i] != NULL)
				delete this->messageList[i];
		}
		delete this->messageList;
		this->messageList = NULL;
	}

	if(this->simpleMessage != NULL)
	{
		delete this->simpleMessage;
		this->simpleMessage = NULL;
	}
	this->index = 0;
}

/**
* Free the used instances
*/
void MessageManager::freeList()
{
	if(this->messageList != NULL)
	{
		for(int i = 0 ; i < (int)this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_MESSAGE_MAX_LINES) ; i++)
		{
			if(this->messageList[i] != NULL)
			{
				delete this->messageList[i];
				this->messageList[i] = NULL;
			}
		}
	}
	this->index = 0;
}

/**
* Free the used instances and initialize the variables
*/
void MessageManager::reloadList()
{
	destroyList();
	createList();
}

/**
* Refresh the message list
*/
void MessageManager::refresh(long time)
{
	int count = 0;
	for(int i = 0 ; i < (int)this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_MESSAGE_MAX_LINES) ; i++)
	{
		if(this->messageList[i] != NULL)
		{
			this->messageList[i]->refresh(time);
			if(this->messageList[i]->getLife() >= this->messageList[i]->getMaxLife())
			{
				delete this->messageList[i];
				this->messageList[i] = NULL;
			}
			else
				count++;
		}
	}
	if(count == 0)
		this->index = 0;
}

/**
* Draw the message list
*/
void MessageManager::draw()
{
	for(int i = 0 ; i < (int)this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_MESSAGE_MAX_LINES) ; i++)
		if(this->messageList[i] != NULL)
			this->messageList[i]->draw();
}

/**
* Add a message to the list
* @param text Text
* @param type Horizontal alignment
* @param r Red color
* @param g Green color
* @param b Blue color
*/
void MessageManager::addMessage(std::string text, int type, float r, float g, float b)
{
	if(!messageExists(text))
	{
		if(this->messageList[this->index] != NULL)
		{
			delete this->messageList[this->index];
			this->messageList[this->index] = NULL;
		}
		this->messageList[this->index] = new Message(text, MESSAGEMANAGER_DEFAULT_LIFETIME, this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_MESSAGE_X), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_MESSAGE_Y) - (this->index * this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_MESSAGE_Y) / 10.0f), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_MESSAGE_SIZE), type, r, g, b);

		this->index++; 
		if(this->index >= (int)this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_MESSAGE_MAX_LINES))
			this->index = 0;
	}
}

/**
* Add a message to the list
* @param textNumber Text number in the Lang Manager list
* @param type Horizontal alignment
* @param r Red color
* @param g Green color
* @param b Blue color
*/
void MessageManager::addMessage(int textNumber, int type, float r, float g, float b)
{
	addMessage(this->langM->getText(textNumber), type, r, g, b);
}
