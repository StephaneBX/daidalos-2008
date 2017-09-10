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

#ifndef __MESSAGEMANAGER_H
#define __MESSAGEMANAGER_H

#include <string>
#include "../elements/2D/message.h"
#include "../includes/singleton.h"
#include "../manager/modManager.h"
#include "../manager/langManager.h"

#define MESSAGEMANAGER_DEFAULT_LIFETIME 3000

/**
* MessageManager Class
* Handle in-game messages
* @author Stephane Baudoux
*/
class MessageManager : public CSingleton<MessageManager>
{
	friend class CSingleton<MessageManager>;

private:
	int maxMessages;
	int index;
	Message * simpleMessage;
	ModManager * modM;
	LangManager * langM;
	Message ** messageList;

public:
	bool messageExists(std::string text);
	void refresh(long time);
	void draw();
	void addMessage(std::string text, int type, float r, float g, float b);
	void addMessage(int textNumber, int type, float r, float g, float b);
	void freeList();
	void reloadList();

private:
	MessageManager();
	~MessageManager();
	void createList();
	void destroyList();
};

#endif
