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

#include "../../elements/2D/message.h"

/**
* Constructor
* @param text The text to be used
* @param maxLife Life time to the Message
* @param x X coordinate of the Message
* @param y Y coordinate of the Message
* @param size Size of the police
* @param type Type of horizontal alignment choosen
* @param r Red color
* @param g Green color
* @param b Blue color
*/
Message::Message(std::string text, long maxLife, float x, float y, float size, int type, float r, float g, float b)
{
	this->type = type;
	this->alpha = 0.0f;
	this->x = x;
	this->y = y;
	this->size = size;
	this->r = r;
	this->g = g;
	this->b = b;
	this->life = 0;
	this->maxLife = maxLife;
	this->textM = TextManager::getInstance();
	this->text = text;
}

/**
* Destructor
*/
Message::~Message(){}

/**
* Return the current life of the Message
* @return Life of the Message
*/
long Message::getLife()
{
	return this->life;
}

/**
* Return the max Life time of the Message
* @return Max Life of the Message
*/
long Message::getMaxLife()
{
	return this->maxLife;
}

/**
* Return the text Message
* @return Text of the Message
*/
std::string Message::getText()
{
	return this->text;
}

/**
* Refresh the Message
* @param time Elapsed time
*/
void Message::refresh(long time)
{
	this->life += time;

	if(this->life <= 500)
		this->alpha = (this->life / 500.0f);
	else
		if(this->life >= 2500)
			this->alpha = 1.0f - ((this->life - (this->maxLife - 500.0f)) / 500.0f);
}

/**
* Draw the Message
*/
void Message::draw()
{
	this->textM->drawText(this->text,this->x, this->y, this->size, this->type,this->r, this->g, this->b, this->alpha);
}
