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

#include <math.h>
#include "../includes/global_var.h"
#include "../manager/optionManager.h"
#include "../manager/impactManager.h"

/**
* Constructor
*/
ImpactManager::ImpactManager()
{
	this->impactNumber = OptionManager::getInstance()->getImpactNumber();
	createImpactsList();
}

/**
* Destructor
*/
ImpactManager::~ImpactManager()
{
	// Free impacts memory allocations
	freeImpacts();

	if(this->lifeTime != NULL)
	{
		delete[] this->lifeTime;
		this->lifeTime = NULL;
	}

	if(this->impactList != NULL)
	{
		delete[] this->impactList;
		this->impactList = NULL;
	}
}

/**
* Refresh the ImpactManager
* @param time Elapsed time
*/
void ImpactManager::refresh(long time)
{
	for(int i = 0 ; i < this->impactNumber ; i++)
	{
		if(this->impactList[i] != NULL)
		{
			this->lifeTime[i] += time;
			if(this->lifeTime[i] >= IMPACTLIFETIME)
			{
				delete this->impactList[i];
				this->impactList[i] = NULL;
			}
		}
	}
}

/**
* Draw the Impacts
* Draw only the impacts that are in the blocks that are drawn
* @param blocksNumber Blocks number that are drawn
* @param levelWidth Width of the level
*/
void ImpactManager::draw(std::vector<int> blocksNumber, int levelWidth)
{
	for(int i = 0 ; i < this->impactNumber ; i++)
	{
		if(this->impactList[i] != NULL)
		{
			bool impactValid = false;
			int impactBlockNumber = levelWidth * (int)(this->impactList[i]->getCoor()[2] / BLOCDEFAULTSIZE) + (int)(-this->impactList[i]->getCoor()[0] / BLOCDEFAULTSIZE);

			// Draw only impacts in a block that has been drawn in this frame
			for(int j = 0 ; j < (int)blocksNumber.size() && !impactValid ; j++)
				if(blocksNumber[j] == impactBlockNumber)
					impactValid = true;

			if(impactValid)
				this->impactList[i]->draw();
		}
	}
}

/**
* Add an Impact to the ImpactManager
* @param impactId Id of the texture
* @param size Size
* @param coor Coordinates
* @param angle Angles
* @param modif Replacement direction
* @param replacementPolicy Replacement policy
*/
void ImpactManager::addImpact(int impactId, float size, Vector3d coor, Vector3d angle, int modif, int replacementPolicy)
{
	if(this->index >= this->impactNumber)
		this->index = 0;

	if(this->impactList[this->index] != NULL)
		delete this->impactList[this->index];

	this->impactList[this->index] = new Impact(impactId, size, coor, angle);
	this->lifeTime[this->index] = 0;

	// Used to prevent graphic glitches when two impacts overloads
	if(DISABLEIMPACTOVERLAPPING && replacementPolicy == IMPACT_REPLACEMENT_POLICY_CREATE)
	{
		bool affected = false;
		float val = 0;
		for(int i = 0 ; i < this->impactNumber ; i++)
		{
			if(i != this->index && this->impactList[i] != NULL)
			{
				if(sqrt((this->impactList[i]->getCoor()[0] - this->impactList[index]->getCoor()[0]) * (this->impactList[i]->getCoor()[0] - this->impactList[index]->getCoor()[0]) + (this->impactList[i]->getCoor()[1] - this->impactList[index]->getCoor()[1]) * (this->impactList[i]->getCoor()[1] - this->impactList[index]->getCoor()[1]) + (this->impactList[i]->getCoor()[2] - this->impactList[index]->getCoor()[2]) * (this->impactList[i]->getCoor()[2] - this->impactList[index]->getCoor()[2])) < (this->impactList[i]->getSize() + this->impactList[index]->getSize()))
				{
					// Type of replacement
					switch(modif)
					{
						case IMPACT_REPLACEMENT_MINUS_X:
						if(!affected)
						{
							val = this->impactList[i]->getCoor()[0];
							affected = true;
						}
						if(val > this->impactList[i]->getCoor()[0])
							val = this->impactList[i]->getCoor()[0];
						break;

						case IMPACT_REPLACEMENT_X:
						if(!affected)
						{
							val = this->impactList[i]->getCoor()[0];
							affected = true;
						}
						if(val < this->impactList[i]->getCoor()[0])
							val = this->impactList[i]->getCoor()[0];
						break;

						case IMPACT_REPLACEMENT_MINUS_Y:
						if(!affected)
						{
							val = this->impactList[i]->getCoor()[1];
							affected = true;
						}
						if(val > this->impactList[i]->getCoor()[1])
							val = this->impactList[i]->getCoor()[1];
						break;

						case IMPACT_REPLACEMENT_Y:
						if(!affected)
						{
							val = this->impactList[i]->getCoor()[1];
							affected = true;
						}
						if(val < this->impactList[i]->getCoor()[1])
							val = this->impactList[i]->getCoor()[1];
						break;

						case IMPACT_REPLACEMENT_MINUS_Z:
						if(!affected)
						{
							val = this->impactList[i]->getCoor()[2];
							affected = true;
						}
						if(val > this->impactList[i]->getCoor()[2])
							val = this->impactList[i]->getCoor()[2];
						break;

						case IMPACT_REPLACEMENT_Z:
						if(!affected)
						{
							val = this->impactList[i]->getCoor()[2];
							affected = true;
						}
						if(val < this->impactList[i]->getCoor()[2])
							val = this->impactList[i]->getCoor()[2];
						break;
					}
				}
			}
		}

		// Perform the replacement
		switch(modif)
		{
			case IMPACT_REPLACEMENT_MINUS_X:
			if(affected)
				this->impactList[this->index]->setCoor(0,val - 0.001f);
			break;

			case IMPACT_REPLACEMENT_X:
			if(affected)
				this->impactList[this->index]->setCoor(0,val + 0.001f);
			break;

			case IMPACT_REPLACEMENT_MINUS_Y:
			if(affected)
				this->impactList[this->index]->setCoor(1,val - 0.001f);
			break;

			case IMPACT_REPLACEMENT_Y:
			if(affected)
				this->impactList[this->index]->setCoor(1,val + 0.001f);
			break;

			case IMPACT_REPLACEMENT_MINUS_Z:
			if(affected)
				this->impactList[this->index]->setCoor(2,val - 0.001f);
			break;

			case IMPACT_REPLACEMENT_Z:
			if(affected)
				this->impactList[this->index]->setCoor(2,val + 0.001f);
			break;
		}
		this->index++;
	}

	// replacementPolicy to IMPACT_REPLACEMENT_POLICY_REPLACE. We delete all the Impacts that are close to the new Impact
	if(replacementPolicy == IMPACT_REPLACEMENT_POLICY_REPLACE)
	{
		for(int i = 0 ; i < this->impactNumber ; i++)
		{
			if(i != this->index && this->impactList[i] != NULL)
			{
				if(sqrt((this->impactList[i]->getCoor()[0] - this->impactList[index]->getCoor()[0]) * (this->impactList[i]->getCoor()[0] - this->impactList[index]->getCoor()[0]) + (this->impactList[i]->getCoor()[1] - this->impactList[index]->getCoor()[1]) * (this->impactList[i]->getCoor()[1] - this->impactList[index]->getCoor()[1]) + (this->impactList[i]->getCoor()[2] - this->impactList[index]->getCoor()[2]) * (this->impactList[i]->getCoor()[2] - this->impactList[index]->getCoor()[2])) < (this->impactList[i]->getSize() + this->impactList[index]->getSize()))
				{
					delete this->impactList[i];
					this->impactList[i] = NULL;
				}
			}
		}
		this->index++;
	}
}

/**
* Free the Impacts
*/
void ImpactManager::freeImpacts()
{
	if(this->impactList != NULL)
	{
		for(int i = 0 ; i < this->impactNumber ; i++)
		{
			this->lifeTime[i] = 0;
			if(this->impactList[i] != NULL)
			{
				delete this->impactList[i];
				this->impactList[i] = NULL;
			}
		}
		this->index = 0;
	}
}

/**
* Update the Impacts value
*/
void ImpactManager::updateImpactValue()
{
	freeImpacts();
	this->impactNumber = OptionManager::getInstance()->getImpactNumber();
	if(this->impactNumber > 0)
		createImpactsList();
}

/**
* Allocate the memmory space for the Impacts
*/
void ImpactManager::createImpactsList()
{
	this->impactList = new Impact * [this->impactNumber];
	this->lifeTime = new long[this->impactNumber];

	for(int i = 0 ; i < this->impactNumber ; i++)
	{
		this->impactList[i] = NULL;
		this->lifeTime[i] = 0;
	}
	this->index = 0;
}
