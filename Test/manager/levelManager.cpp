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

#include <sstream>
#include <fstream>
#include <math.h>
#include <GL/gl.h>
#include "../includes/global_var.h"
#include "../objs/immovableObjs/ark.h"
#include "../objs/immovableObjs/barrel.h"
#include "../objs/immovableObjs/bigWall.h"
#include "../objs/immovableObjs/crate.h"
#include "../objs/immovableObjs/lowCeiling.h"
#include "../objs/immovableObjs/smallWall.h"
#include "../objs/trapObjs/movablePikes.h"
#include "../objs/trapObjs/pikes.h"
#include "../objs/trapObjs/tower.h"
#include "../objs/trapObjs/tower2.h"
#include "../objs/trapObjs/ventilation.h"
#include "../objs/usableObjs/armor.h"
#include "../objs/usableObjs/door.h"
#include "../objs/usableObjs/flashlight.h"
#include "../objs/usableObjs/gate.h"
#include "../objs/usableObjs/gate2.h"
#include "../objs/usableObjs/handgunObject.h"
#include "../objs/usableObjs/handgunAmmo.h"
#include "../objs/usableObjs/health.h"
#include "../objs/usableObjs/key.h"
#include "../objs/usableObjs/lockedDoor.h"
#include "../objs/usableObjs/shotgunObject.h"
#include "../objs/usableObjs/shotgunAmmo.h"
#include "../enemies/immobileEnemies/automaticTurret.h"
#include "../manager/levelManager.h"

/**
* Constructor
*/
LevelManager::LevelManager()
{
	this->matSpec[0] = 1.0f;
	this->matSpec[1] = 1.0f;
	this->matSpec[2] = 1.0f;
	this->matSpec[3] = 1.0f;

	this->matDif[0] = 1.0f;
	this->matDif[1] = 1.0f;
	this->matDif[2] = 1.0f;
	this->matDif[3] = 1.0f;

	this->matAmb[0] = 1.0f;
	this->matAmb[1] = 1.0f;
	this->matAmb[2] = 1.0f;
	this->matAmb[3] = 1.0f;
	
	this->logM = LogManager::getInstance();
	this->soundM = SoundManager::getInstance();
	this->optM = OptionManager::getInstance();
	this->impactM = ImpactManager::getInstance();
	this->loadingM = LoadingManager::getInstance();
	this->particleM = ParticleManager::getInstance();
	this->modM = ModManager::getInstance();
	this->langM = LangManager::getInstance();
	this->messageM = MessageManager::getInstance();
	this->projectileM = ProjectileManager::getInstance();
	this->enemyM = EnemyManager::getInstance();
	this->objectNumber = 0;
	this->player1 = NULL;
	this->playerStartHealth = this->modM->getStartConfigurationValue(MOD_CONFIGURATION_START_VALUE_HEALTH);
	this->playerStartArmor = this->modM->getStartConfigurationValue(MOD_CONFIGURATION_START_VALUE_ARMOR);
	this->playerStartFlashlight = this->modM->getStartConfiguration(MOD_CONFIGURATION_START_ELEMENTS_FLASHLIGHT);
}

/**
* Load a Map
* @param mapPath Path to the level file
* @param musicPath Path to the music file
* @param interBlock Enable/Disable the InterBlocks elements
* @param ambient_light Defaut Ambient light value
*/
void LevelManager::loadMap(std::string mapPath, std::string musicPath, bool interBlock, int ambient_light)
{
	std::ostringstream stringstream;

	this->enableInterBloc = interBlock;
	this->ambient_light = ambient_light;

	// Temporary variable for the loading state
	int percent = 0;

	// If a map is already loaded, we unload it first
	if(this->objectNumber > 0)
		unloadMap();

	// Open level file
	std::ifstream file;
	file.open(mapPath.c_str(), std::ios::in);

	int value = 0;
	int cmpt = 0;
	int playerAngle = 0;
	this->objectNumber = 1;

	int objectTex, objectId, objectType, objectX, objectZ;

	// Read file
	if(file)
    {
		int tmp;
		file >> this->width >> this->height;

		// Player Z coordinate
		file >> tmp;
		tmp = tmp * 10 + 7;
		this->playerCoor[2] = tmp;

		// Player Y coordinate
		this->playerCoor[1] = 0;

		// Player X coordinate
		file >> tmp;
		tmp = tmp * -10 - 5;
		this->playerCoor[0] = tmp;

		// View orientation
		file >> tmp;
		this->playerCoor[3] = 0;
		this->playerCoor[4] = 4;
		this->playerCoor[5] = 10;
		playerAngle = tmp * 90;

		this->player1 = new Player(this->playerCoor, playerAngle, this->playerStartHealth, this->playerStartArmor, this->playerStartFlashlight);
		this->particleM->setPlayer(this->player1);

		// Update the loading screen
		stringstream.str("");
		stringstream << this->langM->getText(TEXT_LOADINGSTATE1) << " : 0%"; 
		this->loadingM->update(stringstream.str(),0);

		// Blocs creation
		while( cmpt < (this->width * this->height))
		{
			file >> value;
			if( value == 0 )
				this->blocs.push_back(NULL);
			else
				this->blocs.push_back(new Block(cmpt/this->width,cmpt%this->width,value - ((value/100)  * 100),value / 100,objectNumber,this->ambient_light));

			cmpt++;
			objectNumber += 20;

			if(((cmpt * 100)/(this->width * this->height)) % 5 == 0 && ((cmpt * 100)/(this->width * this->height)) != percent)
			{
				// Update the loading screen
				percent = ((cmpt * 100)/(this->width * this->height));
				stringstream.str("");
				stringstream << this->langM->getText(TEXT_LOADINGSTATE1) << " : " << percent << "%"; 
				this->loadingM->update(stringstream.str(),0);
			}
		}

		// Update the loading screen
		this->loadingM->update(this->langM->getText(TEXT_LOADINGSTATE2),0);

		// Objects Tab initialization
		this->objects = new Obj ** [(this->height + 1) * 5];
		for(int i = 0 ; i < (this->height + 1) * 5 ; i++)
		{
			this->objects[i] = new Obj * [this->width * 5];
			for(int j = 0 ; j < this->width * 5 ; j++)
				this->objects[i][j] = NULL;
		}

		while( !file.eof() )
		{
			// Objects creation
			file >> objectTex;
			file >> objectId;
			file >> objectType;
			file >> objectX;
			file >> objectZ;

			switch(objectId)
			{
				case 1:
				this->objects[objectZ][objectX] = new Crate(objectX, objectZ, this->player1, objectType, objectTex, objectNumber, this->ambient_light);
				objectNumber += 4;
				break;

				case 2:
				this->objects[objectZ][objectX] = new Barrel(objectX, objectZ, this->player1, objectType, objectTex, this->ambient_light);
				break;

				case 3:
				this->objects[objectZ][objectX] = new SmallWall(objectX, objectZ, this->player1, objectType, blocs[(objectZ / 5) * this->width + (objectX / 5)]->getEnvironment(), objectNumber, this->ambient_light);
				objectNumber += 4;
				break;

				case 4:
				this->objects[objectZ][objectX] = new BigWall(objectX, objectZ, this->player1, blocs[(objectZ / 5) * this->width + (objectX / 5)]->getEnvironment(), objectNumber, this->ambient_light);
				objectNumber += 4;
				break;

				case 5:
				this->objects[objectZ][objectX] = new LowCeiling(objectX, objectZ, this->player1, blocs[(objectZ / 5) * this->width + (objectX / 5)]->getEnvironment(), objectNumber, this->ambient_light);
				objectNumber += 4;
				break;

				case 10:
				this->objects[objectZ][objectX] = new Health(objectX, objectZ, this->player1, this->ambient_light);
				break;

				case 11:
				this->objects[objectZ][objectX] = new Armor(objectX, objectZ, this->player1, this->ambient_light);
				break;

				case 12:
				this->objects[objectZ][objectX] = new HandgunAmmo(objectX, objectZ, this->player1, this->ambient_light);
				break;

				case 13:
				this->objects[objectZ][objectX] = new ShotgunObject(objectX, objectZ, this->player1, this->ambient_light);
				break;

				case 14:
				this->objects[objectZ][objectX] = new ShotgunAmmo(objectX, objectZ, this->player1, this->ambient_light);
				break;

				case 15:
				this->objects[objectZ][objectX] = new Gate(objectX, objectZ, this->player1, this->ambient_light);
				break;

				case 16:
				this->objects[objectZ][objectX] = new Door(objectX, objectZ, this->player1, objectType, objectTex, blocs[(objectZ / 5) * this->width + (objectX / 5)]->getEnvironment(),objectNumber, this->ambient_light);
				objectNumber += 4;
				break;

				case 17:
				this->objects[objectZ][objectX] = new HandgunObject(objectX, objectZ, this->player1, this->ambient_light);
				break;

				case 18:
				this->objects[objectZ][objectX] = new Flashlight(objectX, objectZ, this->player1, this->ambient_light);
				break;

				case 19:
				this->objects[objectZ][objectX] = new LockedDoor(objectX, objectZ, this->player1, objectType, OBJECTTYPE_KEY_YELLOW, objectTex, blocs[(objectZ / 5) * this->width + (objectX / 5)]->getEnvironment(),objectNumber, this->ambient_light);
				objectNumber += 4;
				break;

				case 20:
				this->objects[objectZ][objectX] = new LockedDoor(objectX, objectZ, this->player1, objectType, OBJECTTYPE_KEY_BLUE, objectTex, blocs[(objectZ / 5) * this->width + (objectX / 5)]->getEnvironment(),objectNumber, this->ambient_light);
				objectNumber += 4;
				break;

				case 21:
				this->objects[objectZ][objectX] = new LockedDoor(objectX, objectZ, this->player1, objectType, OBJECTTYPE_KEY_RED, objectTex, blocs[(objectZ / 5) * this->width + (objectX / 5)]->getEnvironment(),objectNumber, this->ambient_light);
				objectNumber += 4;
				break;

				case 22:
				this->objects[objectZ][objectX] = new LockedDoor(objectX, objectZ, this->player1, objectType, OBJECTTYPE_KEY_GREEN, objectTex, blocs[(objectZ / 5) * this->width + (objectX / 5)]->getEnvironment(),objectNumber, this->ambient_light);
				objectNumber += 4;
				break;

				case 23:
				this->objects[objectZ][objectX] = new Key(objectX, objectZ, this->player1,OBJECTTYPE_KEY_YELLOW, this->ambient_light);
				break;

				case 24:
				this->objects[objectZ][objectX] = new Key(objectX, objectZ, this->player1,OBJECTTYPE_KEY_BLUE, this->ambient_light);
				break;

				case 25:
				this->objects[objectZ][objectX] = new Key(objectX, objectZ, this->player1,OBJECTTYPE_KEY_RED, this->ambient_light);
				break;

				case 26:
				this->objects[objectZ][objectX] = new Key(objectX, objectZ, this->player1,OBJECTTYPE_KEY_GREEN, this->ambient_light);
				break;

				case 27:
				this->objects[objectZ][objectX] = new Ventilation(objectX, objectZ, this->player1, objectNumber, this->ambient_light);
				objectNumber += 4;
				break;

				case 28:
				this->objects[objectZ][objectX] = new Pikes(objectX, objectZ, this->player1, objectNumber, this->ambient_light);
				objectNumber += 4;
				break;

				case 29:
				this->objects[objectZ][objectX] = new Tower(objectX, objectZ, this->player1, this->ambient_light);
				break;

				case 30:
				this->objects[objectZ][objectX] = new Tower2(objectX, objectZ, this->player1, blocs[(objectZ / 5) * this->width + (objectX / 5)]->getEnvironment(), objectNumber, this->ambient_light);
				objectNumber += 4;
				break;

				case 31:
				this->objects[objectZ][objectX] = new MovablePikes(objectX, objectZ, this->player1, objectNumber, this->ambient_light);
				objectNumber += 4;
				break;

				case 32:
				this->objects[objectZ][objectX] = new Ark(objectX, objectZ, this->player1, blocs[(objectZ / 5) * this->width + (objectX / 5)]->getEnvironment(),objectNumber, this->ambient_light);
				objectNumber += 4;
				break;

				default:
				break;
			}

			// Prevent the last line (which is empty) to be count as an object
			objectId = -1;
		}
		file.close();

		// Level start Gate
		this->objects[(int)(this->playerCoor[2] - 2.0f) / 2][-this->playerCoor[0] / 2] = new Gate2(-this->playerCoor[0] / 2, (int)(this->playerCoor[2] - 2.0f) / 2, this->player1, this->ambient_light);

		// Update the loading screen
		stringstream.str("");
		stringstream << this->langM->getText(TEXT_LOADINGSTATE3) << " : 0%"; 
		this->loadingM->update(stringstream.str(),0);
		percent = 0;

		// Pillars and interBlocs generation
		for(int i = 0 ; i < (int)this->blocs.size() ; i++)
		{
			if(this->blocs[i] != NULL)
			{
				if(this->blocs[i]->getType() == 1 || this->blocs[i]->getType() == 12 || this->blocs[i]->getType() == 14)
				{
					this->pillars.push_back(new Pillar((float)(i%this->width)*BLOCDEFAULTSIZE,(float)((i/this->width)+1)*BLOCDEFAULTSIZE,1,objectNumber, this->ambient_light));
					objectNumber += 4;
				}

				if(this->blocs[i]->getType() == 2 || this->blocs[i]->getType() == 11 || this->blocs[i]->getType() == 14)
				{
					this->pillars.push_back(new Pillar((float)((i%this->width)+1)*BLOCDEFAULTSIZE,(float)((i/this->width)+1)*BLOCDEFAULTSIZE,1,objectNumber, this->ambient_light));
					objectNumber += 4;
				}

				if(this->blocs[i]->getType() == 3 || this->blocs[i]->getType() == 12 || this->blocs[i]->getType() == 13)
				{
					this->pillars.push_back(new Pillar((float)(i%this->width)*BLOCDEFAULTSIZE,(float)(i/this->width)*BLOCDEFAULTSIZE,1,objectNumber, this->ambient_light));
					objectNumber += 4;
				}

				if(this->blocs[i]->getType() == 4 || this->blocs[i]->getType() == 11 || this->blocs[i]->getType() == 13)
				{
					this->pillars.push_back(new Pillar((float)((i%this->width)+1)*BLOCDEFAULTSIZE,(float)(i/this->width)*BLOCDEFAULTSIZE,1,objectNumber, this->ambient_light));
					objectNumber += 4;
				}

				if((this->blocs[i]->getType() == 3 || this->blocs[i]->getType() == 5 || this->blocs[i]->getType() == 8 || this->blocs[i]->getType() == 9  || this->blocs[i]->getType() == 13) && (i + this->width - 1) < (int)this->blocs.size() && i%this->width != 0)
				{
					if(this->blocs[(i+this->width - 1)] != NULL && this->blocs[(i+this->width - 1)]->isDownBoxExists())
					{
						this->pillars.push_back(new Pillar((float)(i%this->width)*BLOCDEFAULTSIZE,(float)((i/this->width)+1)*BLOCDEFAULTSIZE,1,objectNumber, this->ambient_light));
						objectNumber += 4;
					}
				}

				if((this->blocs[i]->getType() == 1 || this->blocs[i]->getType() == 5 || this->blocs[i]->getType() == 7 || this->blocs[i]->getType() == 8 || this->blocs[i]->getType() == 14) && (i - this->width - 1) >= 0 && (i - this->width - 1) < (int)this->blocs.size() && i%this->width != this->width)
				{
					if(blocs[i - this->width - 1] != NULL && blocs[i - this->width - 1]->isUpBoxExists())
					{
						this->pillars.push_back(new Pillar((float)(i%this->width)*BLOCDEFAULTSIZE,(float)(i/this->width)*BLOCDEFAULTSIZE,1,objectNumber, this->ambient_light));
						objectNumber += 4;
					}
				}

				if((this->blocs[i]->getType() == 4 || this->blocs[i]->getType() == 5 || this->blocs[i]->getType() == 9 || this->blocs[i]->getType() == 10 || this->blocs[i]->getType() == 13) && (i + this->width + 1) < (int)this->blocs.size() && i%this->width != 0)
				{
					if(this->blocs[(i+this->width + 1)] != NULL && this->blocs[(i+this->width + 1)]->isDownBoxExists())
					{
						this->pillars.push_back(new Pillar((float)((i%this->width)+1)*BLOCDEFAULTSIZE,(float)((i/this->width)+1)*BLOCDEFAULTSIZE,1,objectNumber, this->ambient_light));
						objectNumber += 4;
					}
				}

				if((this->blocs[i]->getType() == 2 || this->blocs[i]->getType() == 5 || this->blocs[i]->getType() == 7 || this->blocs[i]->getType() == 10 || this->blocs[i]->getType() == 14) && (i - this->width + 1) >= 0 && (i - this->width - 1) < (int)this->blocs.size() && i%this->width != this->width)
				{
					if(this->blocs[i - this->width + 1] != NULL && this->blocs[i - this->width + 1]->isUpBoxExists())
					{
						this->pillars.push_back(new Pillar((float)((i%this->width)+1)*BLOCDEFAULTSIZE,(float)(i/this->width)*BLOCDEFAULTSIZE,1,objectNumber, this->ambient_light));
						objectNumber += 4;
					}
				}

				// Pillars when two blocs have different environment and interBlocs between those blocs
				if((i - this->width) >= 0 && blocs[i] != NULL && blocs[i - this->width] != NULL && blocs[i]->getEnvironment() != blocs[i - this->width]->getEnvironment())
				{
					if(this->blocs[i] != NULL && this->blocs[i]->isLeftBoxExists())
					{
						this->pillars.push_back(new Pillar((float)(i%this->width)*BLOCDEFAULTSIZE,(float)(i/this->width)*BLOCDEFAULTSIZE,1,objectNumber, this->ambient_light));
						objectNumber += 4;
					}
					if(this->blocs[i] != NULL && this->blocs[i]->isRightBoxExists())
					{
						this->pillars.push_back(new Pillar((float)((i%this->width)+1)*BLOCDEFAULTSIZE,(float)(i/this->width)*BLOCDEFAULTSIZE,1,objectNumber, this->ambient_light));
						objectNumber += 4;
					}
					
					if(this->enableInterBloc)
						this->interBlocs.push_back(new InterBlock((float)((i%this->width) + 1)*BLOCDEFAULTSIZE,(float)(i/this->width)*BLOCDEFAULTSIZE,1,1,objectNumber++, this->ambient_light));
				}

				if((i%this->width) != 0 && (i - 1) >= 0 && this->blocs[i] != NULL && this->blocs[i - 1] != NULL &&  this->blocs[i]->getEnvironment() != this->blocs[i - 1]->getEnvironment())
				{
					if(this->blocs[i]->isUpBoxExists())
					{
						this->pillars.push_back(new Pillar((float)(i%this->width)*BLOCDEFAULTSIZE,(float)((i/this->width) + 1)*BLOCDEFAULTSIZE,1,objectNumber, this->ambient_light));
						objectNumber += 4;
					}
					if(this->blocs[i]->isDownBoxExists())
					{
						this->pillars.push_back(new Pillar((float)(i%this->width)*BLOCDEFAULTSIZE,(float)(i/this->width)*BLOCDEFAULTSIZE,1,objectNumber, this->ambient_light));
						objectNumber += 4;
					}

					if(this->enableInterBloc)
						this->interBlocs.push_back(new InterBlock((float)(i%this->width)*BLOCDEFAULTSIZE,(float)(i/this->width)*BLOCDEFAULTSIZE,2,1,objectNumber++, this->ambient_light));
				}
			}

			if(((i * 100)/(this->width * this->height)) % 5 == 0 && ((i * 100)/(this->width * this->height)) != percent)
			{
				// Update the loading screen
				percent = ((i * 100)/(this->width * this->height));
				stringstream.str("");
				stringstream << this->langM->getText(TEXT_LOADINGSTATE3) << " : " << percent << "%"; 
				this->loadingM->update(stringstream.str(),0);
			}
		}

		this->enemyM->addEnemy(new AutomaticTurret(Vector3d(-50,1,50),this->player1,0,this->ambient_light));

		// Update the loading screen
		this->loadingM->update(this->langM->getText(TEXT_LOADINGSTATE4),0);

		// Load music
		this->soundM->loadMusic(musicPath);
		this->soundM->playMusic();

		// Update the loading screen
		this->loadingM->update(this->langM->getText(TEXT_LOADINGSTATE5),0);

		this->weaponM = WeaponManager::getInstance();

		// Update the loading screen
		this->loadingM->update(this->langM->getText(TEXT_LOADINGSTATE6),2);

		// EndOfLevel to false
		this->endOfLevel = false;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		if(this->optM->isDebug())
			this->logM->logIt(std::string("Unable to open level:"),mapPath);
	}

	stringstream.str("");
	stringstream << this->langM->getText(TEXT_START_LEVEL) << this->modM->getLevelName(mapPath);

	this->messageM->addMessage(stringstream.str(),TEXTMANAGER_ALIGN_LEFT,0.8f,0.3f,0.3f);
}

/**
* Unload the current map
*/
void LevelManager::unloadMap()
{
	// Update the loading screen
	this->loadingM->update(1);

	this->enemyM->destroyEnemies();

	// We save the last player health and armor
	this->playerStartHealth = player1->getHealth();
	this->playerStartArmor = player1->getArmor();
	this->playerStartFlashlight = player1->hasFlashlight();

	// We clear the objects in the objects tab and the tab itself
	for(int i = 0 ; i < (this->height + 1) * 5 ; i++)
	{
		for(int j = 0 ; j < this->width * 5 ; j++)
		{
			delete this->objects[i][j];
			this->objects[i][j] = NULL;
		}
		delete[] this->objects[i];
		this->objects[i] = NULL;
	}
	delete[] this->objects;
	this->objects = NULL;

	// Free the impats
	ImpactManager::getInstance()->freeImpacts();

	// We clear the objects in the pillars vector and free the vector
	for(unsigned int i = 0 ; i < pillars.size() ; i++)
	{
		delete this->pillars[i];
		this->pillars[i] = NULL;
	}
	this->pillars.clear();

	// We clear the objects in the blocs vector and free the vector
	for(unsigned int i = 0 ; i < blocs.size() ; i++)
	{
		delete this->blocs[i];
		this->blocs[i] = NULL;
	}
	this->blocs.clear();

	// We clear the objects in the interbloc vector and free the vector
	for(unsigned int i = 0 ; i < interBlocs.size() ; i++)
	{
		delete this->interBlocs[i];
		this->interBlocs[i] = NULL;
	}
	this->interBlocs.clear();

	// Clear player
	delete this->player1;
	this->player1 = NULL;

	// Delete the Lists
	glDeleteLists(1,this->objectNumber);
	this->objectNumber = 0;

	this->particleM->freeParticles();
	this->messageM->freeList();
	this->projectileM->freeProjectiles();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
* Destructor
*/
LevelManager::~LevelManager()
{
	this->impactM->kill();
	this->impactM = NULL;

	if(this->objectNumber > 0)
		unloadMap();
}

/**
* Set health, armor and flashlight values to default
*/
void LevelManager::resetPlayerStats()
{
	this->playerStartHealth = this->modM->getStartConfigurationValue(MOD_CONFIGURATION_START_VALUE_HEALTH);
	this->playerStartArmor = this->modM->getStartConfigurationValue(MOD_CONFIGURATION_START_VALUE_ARMOR);
	this->playerStartFlashlight = this->modM->getStartConfiguration(MOD_CONFIGURATION_START_ELEMENTS_FLASHLIGHT);
}

/**
* Geturn Player instance
* @return The Player instance
*/
Player * LevelManager::getPlayer()
{
	return this->player1;
}

/**
* Get the level state
* @return If the level is finished
*/
bool LevelManager::isLevelFinished()
{
	return this->endOfLevel;
}

/**
* refresh level data
* @param time Elapsed time
*/
void LevelManager::refresh(long time)
{
	player1->refresh(time, this->blocs[this->width*(int)((player1->getCoor()[2]) / BLOCDEFAULTSIZE) + (int)(-player1->getCoor()[0] / BLOCDEFAULTSIZE)]->getEnvironment());
	if(!OptionManager::getInstance()->isNoClipActivated())
		collisions(time);

	if(this->player1->getFired())
	{
		this->player1->setFired(false);

		for(int i = 0 ; i < this->weaponM->getSelectedWeapon()->getImpactPerShot() ; i++)
			weaponHit();
	}

	for(int i = 0 ; i < this->height * 5 ; i++)
		for(int j = 0 ; j < this->width * 5 ; j++)
			if(objects[i][j] != NULL && objects[i][j]->getType() > 0)
				objects[i][j]->refresh(time);

	this->enemyM->refresh(time);
	this->weaponM->refresh(time,player1->getCoor()[0],player1->getCoor()[1],player1->getCoor()[2] + 2.0f,player1->getAngleXZ(),player1->getAngleY());
	this->impactM->refresh(time);
	this->particleM->refresh(time);
	this->projectileM->refresh(time);
}

/**
* Draw the level
* @param lightPos Position of the Light
*/
void LevelManager::draw(float * lightPos)
{
	doViewingFrustumCulling(this->width*(int)((player1->getCoor()[2]) / BLOCDEFAULTSIZE) + (int)(-player1->getCoor()[0] / BLOCDEFAULTSIZE));
	doOcculisionCulling(lightPos);
	this->impactM->draw(this->drawableBlocs,this->width);
	this->projectileM->draw(this->drawableBlocs,this->width);
	this->enemyM->draw();
	this->particleM->draw(this->drawableBlocs,this->width);

	this->drawableBlocs.clear();
	this->drawablePillars.clear();
	this->drawableInterBlocs.clear();

	glMaterialfv(GL_FRONT,GL_SPECULAR,this->matSpec);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,this->matDif);
	glMaterialfv(GL_FRONT,GL_AMBIENT,this->matAmb);
	this->weaponM->drawWeapon(player1->getCoor()[0],player1->getCoor()[1],player1->getCoor()[2] + 2.0f,player1->getAngleXZ(),player1->getAngleY(),lightPos);
}

/**
* Detect wall collisions with the Player
*/
void LevelManager::wallCollision()
{
	for(float x = - PLAYERDETECTION ; x <= PLAYERDETECTION ; x+= 0.11f)
	{
		for(float z = - PLAYERDETECTION ; z <= PLAYERDETECTION ; z+= 0.11f)
		{
			int currentBlock = this->width*(int)((player1->getCoor()[2] + z) / BLOCDEFAULTSIZE) + (int)(-(player1->getCoor()[0] + x) / BLOCDEFAULTSIZE);
			int previousBlock = this->width*(int)((player1->getPreviousCoor()[2] + z) / BLOCDEFAULTSIZE) + (int)(-(player1->getPreviousCoor()[0] + x) / BLOCDEFAULTSIZE);

			if(currentBlock != previousBlock)
				currentBlock = previousBlock;

			if(currentBlock >= 0  && currentBlock < (this->width * this->height) && blocs[currentBlock] != NULL)
			{
				if(blocs[currentBlock]->isLeftBoxExists() && blocs[currentBlock]->getLeftBox()->isCollision(player1->getCoor()[0] + x,player1->getCoor()[2] + z))
					player1->setCoor(0,blocs[currentBlock]->getLeftBox()->getXMin() - PLAYERDETECTION - 0.01f);

				if(blocs[currentBlock]->isUpBoxExists() && blocs[currentBlock]->getUpBox()->isCollision(player1->getCoor()[0] + x,player1->getCoor()[2] + z))
					player1->setCoor(2,blocs[currentBlock]->getUpBox()->getZMin() - PLAYERDETECTION - 0.01f);

				if(blocs[currentBlock]->isRightBoxExists() && blocs[currentBlock]->getRightBox()->isCollision(player1->getCoor()[0] + x,player1->getCoor()[2] + z))
					player1->setCoor(0,blocs[currentBlock]->getRightBox()->getXMax() + PLAYERDETECTION + 0.01f);

				if(blocs[currentBlock]->isDownBoxExists() && blocs[currentBlock]->getDownBox()->isCollision(player1->getCoor()[0] + x,player1->getCoor()[2] + z))
					player1->setCoor(2,blocs[currentBlock]->getDownBox()->getZMax() + PLAYERDETECTION + 0.01f);
			}
		}
	}
}

/**
* Detect object collisions with the Player.
* Replace the Player if needed and use the objects on collision
*/
void LevelManager::objectCollision()
{	
	bool collisionObj = false;
	float XTest = 0;
	float ZTest = 0;

	for(int b = 0 ; b < 5 ; b++)
	{
		for(int i = 0 ; i < 3 ; i ++)
		{
			if(i == 0)
				XTest = 0;

			if(i == 1)
				XTest = PLAYERDETECTION;

			if(i == 2)
				XTest = -PLAYERDETECTION;

			for(int j = 0 ; j < 3 ; j ++)
			{
				if(j == 0)
					ZTest = 0;

				if(j == 1)
					ZTest = PLAYERDETECTION;

				if(j == 2)
					ZTest = -PLAYERDETECTION;

				if((objects[player1->getCurrentObjectCoordinateZ(ZTest,1)][player1->getCurrentObjectCoordinateX(XTest)] != NULL || objects[player1->getCurrentObjectCoordinateZ(ZTest,-1)][player1->getCurrentObjectCoordinateX(XTest)] != NULL ))
				{
					if(objects[player1->getCurrentObjectCoordinateZ(ZTest,0)][player1->getCurrentObjectCoordinateX(XTest)] != NULL  )
					{
						if(objects[player1->getCurrentObjectCoordinateZ(ZTest,0)][player1->getCurrentObjectCoordinateX(XTest)]->getType() == OBJECTTYPE_IMMOVABLE || objects[player1->getCurrentObjectCoordinateZ(ZTest,0)][player1->getCurrentObjectCoordinateX(XTest)]->getType() == OBJECTTYPE_IMMOVABLE_TRAP)
						{
							if(objects[player1->getCurrentObjectCoordinateZ(ZTest,0)][player1->getCurrentObjectCoordinateX(XTest)]->needReplacement())
							{
								if(player1->getPreviousObjectCoordinateX() < player1->getCurrentObjectCoordinateX(XTest) && (player1->getPreviousObjectCoordinateZ(-1) == player1->getCurrentObjectCoordinateZ(ZTest,-1) || player1->getPreviousObjectCoordinateZ(1) == player1->getCurrentObjectCoordinateZ(ZTest,1)))
								{
									player1->setCoor(0,player1->getCurrentObjectCoordinateX(XTest) * -2 + PLAYERDETECTION + 0.01f);
									collisionObj = true;
								}
								else
								{
									if(player1->getPreviousObjectCoordinateX() > player1->getCurrentObjectCoordinateX(XTest) && (player1->getPreviousObjectCoordinateZ(-1) == player1->getCurrentObjectCoordinateZ(ZTest,-1) || player1->getPreviousObjectCoordinateZ(1) == player1->getCurrentObjectCoordinateZ(ZTest,1)))
									{
										player1->setCoor(0,player1->getCurrentObjectCoordinateX(XTest) * -2 - PLAYERDETECTION * 2 - (2.0f - PLAYERDETECTION) - 0.01f);
										collisionObj = true;
									}
									else
									{
										if(player1->getPreviousObjectCoordinateZ(0) < player1->getCurrentObjectCoordinateZ(ZTest,0) && player1->getPreviousObjectCoordinateX() == player1->getCurrentObjectCoordinateX(XTest))
										{
											player1->setCoor(2,player1->getCurrentObjectCoordinateZ(ZTest,0) * 2 - PLAYERDETECTION - 0.01f);
											collisionObj = true;
										}
										else
										{
											if(player1->getPreviousObjectCoordinateZ(0) > player1->getCurrentObjectCoordinateZ(ZTest,0) && player1->getPreviousObjectCoordinateX() == player1->getCurrentObjectCoordinateX(XTest))
											{
												player1->setCoor(2,player1->getCurrentObjectCoordinateZ(ZTest,0) * 2 + 2 + PLAYERDETECTION * 2 -PLAYERDETECTION + 0.01f);
												collisionObj = true;
											}
										}
									}
								}
							}
						}
						else
						{
							
							if(objects[player1->getCurrentObjectCoordinateZ(ZTest,0)][player1->getCurrentObjectCoordinateX(XTest)]->getType() == OBJECTTYPE_GATE)
							{
								this->endOfLevel = true;
							}
							else
							{
								if(objects[player1->getCurrentObjectCoordinateZ(ZTest,0)][player1->getCurrentObjectCoordinateX(XTest)]->getType() == OBJECTTYPE_DOOR_VERTICAL || objects[player1->getCurrentObjectCoordinateZ(ZTest,0)][player1->getCurrentObjectCoordinateX(XTest)]->getType() == OBJECTTYPE_DOOR_HORIZONTAL || objects[player1->getCurrentObjectCoordinateZ(ZTest,0)][player1->getCurrentObjectCoordinateX(XTest)]->getType() == OBJECTTYPE_LOCKEDDOOR_HORIZONTAL || objects[player1->getCurrentObjectCoordinateZ(ZTest,0)][player1->getCurrentObjectCoordinateX(XTest)]->getType() == OBJECTTYPE_LOCKEDDOOR_VERTICAL)
								{
									if(objects[player1->getCurrentObjectCoordinateZ(ZTest,0)][player1->getCurrentObjectCoordinateX(XTest)]->getSecondType() != DOOR_OPENED)
									{
										// Open only if the player has the key or if the door is not locked
										if(this->player1->hasKey(objects[player1->getCurrentObjectCoordinateZ(ZTest,0)][player1->getCurrentObjectCoordinateX(XTest)]->getKeyNeeded()))
										{
											objects[player1->getCurrentObjectCoordinateZ(ZTest,0)][player1->getCurrentObjectCoordinateX(XTest)]->useObject();

											// Open neightboring doors
											openNeighboringDoors(player1->getCurrentObjectCoordinateX(XTest),player1->getCurrentObjectCoordinateZ(ZTest,0));
										}
										else
											this->messageM->addMessage(TEXT_INFORMATION_KEY_REQUIERED,TEXTMANAGER_ALIGN_LEFT,1.0f,1.0f,1.0f);

										if(player1->getPreviousObjectCoordinateX() < player1->getCurrentObjectCoordinateX(XTest) && (player1->getPreviousObjectCoordinateZ(-1) == player1->getCurrentObjectCoordinateZ(ZTest,-1) || player1->getPreviousObjectCoordinateZ(1) == player1->getCurrentObjectCoordinateZ(ZTest,1)))
										{
											player1->setCoor(0,player1->getCurrentObjectCoordinateX(XTest) * -2 + PLAYERDETECTION + 0.01f);
											collisionObj = true;
										}
										else
										{
											if(player1->getPreviousObjectCoordinateX() > player1->getCurrentObjectCoordinateX(XTest) && (player1->getPreviousObjectCoordinateZ(-1) == player1->getCurrentObjectCoordinateZ(ZTest,-1) || player1->getPreviousObjectCoordinateZ(1) == player1->getCurrentObjectCoordinateZ(ZTest,1)))
											{
												player1->setCoor(0,player1->getCurrentObjectCoordinateX(XTest) * -2 - PLAYERDETECTION * 2 - (2.0f - PLAYERDETECTION) - 0.01f);
												collisionObj = true;
											}
											else
											{
												if(player1->getPreviousObjectCoordinateZ(0) < player1->getCurrentObjectCoordinateZ(ZTest,0) && player1->getPreviousObjectCoordinateX() == player1->getCurrentObjectCoordinateX(XTest))
												{
													player1->setCoor(2,player1->getCurrentObjectCoordinateZ(ZTest,0) * 2 - PLAYERDETECTION - 0.01f);
													collisionObj = true;
												}
												else
												{
													if(player1->getPreviousObjectCoordinateZ(0) > player1->getCurrentObjectCoordinateZ(ZTest,0) && player1->getPreviousObjectCoordinateX() == player1->getCurrentObjectCoordinateX(XTest))
													{
														player1->setCoor(2,player1->getCurrentObjectCoordinateZ(ZTest,0) * 2 + 2 + PLAYERDETECTION * 2 -PLAYERDETECTION + 0.01f);
														collisionObj = true;
													}
												}
											}
										}
									}
								}
								else
								{
									// Generic object use patern
									if(objects[player1->getCurrentObjectCoordinateZ(ZTest,0)][player1->getCurrentObjectCoordinateX(XTest)]->useObject())
									{
										delete objects[player1->getCurrentObjectCoordinateZ(ZTest,0)][player1->getCurrentObjectCoordinateX(XTest)];
										objects[player1->getCurrentObjectCoordinateZ(ZTest,0)][player1->getCurrentObjectCoordinateX(XTest)] = NULL;
									}
								}
							}			
						}
					}
				}
			}
		}
	}
}

/**
* Detect projectiles collisions with the environment and the Player
*/
void LevelManager::projectileCollision(long time)
{
	for(int i = 0 ; i < PROJECTILESLISTSIZE ; i++)
	{
		if(this->projectileM->getProjectile(i) != NULL)
		{
			// The cmpModifier is dependent of the time beetween 2 frames because the distance performed by the projectile depend on the speed and the time
			float timeRatio = (time / 15.0f) * 2.0f;
			float cmptModifier = 100.0f / (this->projectileM->getProjectile(i)->getSpeed() * 100.0f * timeRatio);

			bool hit = false;
			Vector3d interValues = this->projectileM->getProjectile(i)->getCurrentCoordinates() - this->projectileM->getProjectile(i)->getPreviousCoordinates();

			bool fastSpeed = false;
			// Case where the computer is extremely fast (less than 6ms per frame)
			// Prevent a projectile from having a too small interValues value to be able to detect collision if the projectile is too slow
			if(timeRatio < 0.8f && this->projectileM->getProjectile(i)->getSpeed() <= PROJECTILE_SLOW_TYPE)
			{
				interValues = this->projectileM->getProjectile(i)->getCurrentCoordinates() - this->projectileM->getProjectile(i)->getStartCoordinates();
				timeRatio = interValues.length() * 2.0f;
				cmptModifier = 100.0f / (this->projectileM->getProjectile(i)->getSpeed() * 100.0f * timeRatio);
				fastSpeed = true;
			}

			// Projectile test
			for(float cmpt = cmptModifier ; cmpt < 1.0f && !hit ; cmpt += cmptModifier)
			{
				Vector3d tmpValues;
				Vector3d tmpValuesPrevious;
				if(!fastSpeed)
				{
					tmpValues = this->projectileM->getProjectile(i)->getPreviousCoordinates() + interValues * cmpt;
					tmpValuesPrevious = this->projectileM->getProjectile(i)->getPreviousCoordinates() + interValues * (cmpt - cmptModifier);
				}
				else
				{
					tmpValues = this->projectileM->getProjectile(i)->getStartCoordinates() + interValues * cmpt;
					tmpValuesPrevious = this->projectileM->getProjectile(i)->getStartCoordinates() + interValues * (cmpt - cmptModifier);
				}

				int currentBlock = this->width*(int)(tmpValues[2] / BLOCDEFAULTSIZE) + (int)(-tmpValues[0]) / BLOCDEFAULTSIZE;

				if(currentBlock >= 0 && currentBlock < (int)blocs.size() && blocs[currentBlock] != NULL)
				{
					// Objects Impacts
					if(this->objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)] != NULL && (int)(tmpValues[2] / 2) >= 0 && (int)(tmpValues[2] / 2) < (this->height * 5) && (int)(-tmpValues[0] / 2) >= 0 && (int)(-tmpValues[0] / 2) < (this->width * 5) && (this->objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)]->getType() == OBJECTTYPE_IMMOVABLE || this->objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)]->getType() == OBJECTTYPE_DOOR_HORIZONTAL || this->objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)]->getType() == OBJECTTYPE_DOOR_VERTICAL || this->objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)]->getType() == OBJECTTYPE_LOCKEDDOOR_HORIZONTAL || this->objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)]->getType() == OBJECTTYPE_LOCKEDDOOR_VERTICAL || this->objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)]->getType() == OBJECTTYPE_TRAP || this->objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)]->getType() == OBJECTTYPE_IMMOVABLE_TRAP) && this->objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)]->isCollision(tmpValues[0],tmpValues[2],tmpValues[1]))
					{
						hit = true;

						if(this->projectileM->getProjectile(i)->getType() != PROJECTILE_TYPE_ATTACK_TEST)
						{
							// Specific to doors
							if(objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)]->getType() == OBJECTTYPE_DOOR_VERTICAL || objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)]->getType() == OBJECTTYPE_DOOR_HORIZONTAL)
								openNeighboringDoors((int)(-tmpValues[0] / 2),(int)(tmpValues[2] / 2));
							// Specific to doors

							this->objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)]->playSound(SOUNDDISTANCEADDITION + (int)(sqrt(((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]))) / SOUNDDISTANCEMODIFIER);
							if(this->objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)]->getImpactEnabled() && this->optM->getImpactNumberValue() > 0)
							{
								float impactX,impactY,impactZ,impactAngleX,impactAngleY,impactAngleZ;
								int modif;
								this->objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)]->impactGetLocation(tmpValuesPrevious[0],tmpValuesPrevious[1],tmpValuesPrevious[2],tmpValues[0],tmpValues[1],tmpValues[2],impactX,impactY,impactZ,impactAngleX,impactAngleY,impactAngleZ,modif,this->projectileM->getProjectile(i)->getImpactRadius());
								if(modif != 0)
									this->impactM->addImpact(this->projectileM->getProjectile(i)->getImpactId(),this->projectileM->getProjectile(i)->getImpactRadius(),Vector3d(impactX,impactY,impactZ),Vector3d(impactAngleX,impactAngleY,impactAngleZ),modif,this->projectileM->getProjectile(i)->getImpactPolicy());
							}
							if(this->objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)]->getMatType() != OBJECTMAT_STEEL)
								this->particleM->addParticle(PARTICLE_IMPACT_SMOKE,	Vector3d(tmpValues[0] - interValues[0] * cmptModifier,tmpValues[1] - interValues[1] * cmptModifier,tmpValues[2] - interValues[2] * cmptModifier),30);
							if(this->objects[(int)(tmpValues[2] / 2)][(int)(-tmpValues[0] / 2)]->getMatType() != OBJECTMAT_WOOD)
								this->particleM->addParticle(PARTICLE_IMPACT,		Vector3d(tmpValues[0] - interValues[0] * cmptModifier * 2.0f,tmpValues[1] - interValues[1] * cmptModifier * 2.0f,tmpValues[2] - interValues[2] * cmptModifier * 2.0f),30);
						}
						else
							this->enemyM->getEnemy(this->projectileM->getProjectile(i)->getSource())->playerDetection(false);
					}
					else
					{
						// Blocks Impacts
						if(tmpValues[1] <= 0)
						{
							hit = true;
							if(this->projectileM->getProjectile(i)->getType() != PROJECTILE_TYPE_ATTACK_TEST)
							{
								if(this->optM->getImpactNumberValue() > 0)
									this->impactM->addImpact(this->projectileM->getProjectile(i)->getImpactId(),this->projectileM->getProjectile(i)->getImpactRadius(),Vector3d(tmpValues[0],MINIMUMIMPACTHEIGHT,tmpValues[2]),Vector3d(90.0f,0.0f,0.0f),IMPACT_REPLACEMENT_Y,this->projectileM->getProjectile(i)->getImpactPolicy());
								blocs[currentBlock]->playSound(SOUNDDISTANCEADDITION + (int)(sqrt(((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]))) / SOUNDDISTANCEMODIFIER);
								this->particleM->addParticle(PARTICLE_IMPACT_SMOKE,	Vector3d(tmpValues[0], MINIMUMIMPACTHEIGHT, tmpValues[2]),30);
								this->particleM->addParticle(PARTICLE_IMPACT,		Vector3d(tmpValues[0], MINIMUMIMPACTHEIGHT + 0.001f, tmpValues[2]),30);
							}
							else
								this->enemyM->getEnemy(this->projectileM->getProjectile(i)->getSource())->playerDetection(false);
						}
						else
						{
							if(tmpValues[1] >= BLOCDEFAULTHEIGHT)
							{
								hit = true;
								if(this->projectileM->getProjectile(i)->getType() != PROJECTILE_TYPE_ATTACK_TEST)
								{
									if(this->optM->getImpactNumberValue() > 0)
										this->impactM->addImpact(this->projectileM->getProjectile(i)->getImpactId(),this->projectileM->getProjectile(i)->getImpactRadius(),Vector3d(tmpValues[0],BLOCDEFAULTHEIGHT - 0.001f,tmpValues[2]),Vector3d(-90.0,0.0f,0.0f),IMPACT_REPLACEMENT_MINUS_Y,this->projectileM->getProjectile(i)->getImpactPolicy());
									blocs[currentBlock]->playSound(SOUNDDISTANCEADDITION + (int)(sqrt(((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]))) / SOUNDDISTANCEMODIFIER);
									this->particleM->addParticle(PARTICLE_IMPACT_SMOKE_REVERSE,	Vector3d(tmpValues[0], BLOCDEFAULTHEIGHT - 0.01f,tmpValues[2]),30);
									this->particleM->addParticle(PARTICLE_IMPACT,				Vector3d(tmpValues[0], BLOCDEFAULTHEIGHT - 0.02f,tmpValues[2]),30);
								}
								else
									this->enemyM->getEnemy(this->projectileM->getProjectile(i)->getSource())->playerDetection(false);
							}
							else
							{
								if(blocs[currentBlock]->isLeftBoxExists() && blocs[currentBlock]->getLeftBox()->isCollision(tmpValues[0],tmpValues[2],tmpValues[1]))
								{
									hit = true;
									if(this->projectileM->getProjectile(i)->getType() != PROJECTILE_TYPE_ATTACK_TEST)
									{
										if(this->optM->getImpactNumberValue() > 0)
											this->impactM->addImpact(this->projectileM->getProjectile(i)->getImpactId(),this->projectileM->getProjectile(i)->getImpactRadius(),Vector3d(blocs[currentBlock]->getLeftBox()->getXMin() - 0.001f,tmpValues[1],tmpValues[2]),Vector3d(0.0f,90.0f,0.0f),IMPACT_REPLACEMENT_MINUS_X,this->projectileM->getProjectile(i)->getImpactPolicy());
										blocs[currentBlock]->playSound(SOUNDDISTANCEADDITION + (int)(sqrt(((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]))) / SOUNDDISTANCEMODIFIER);
										this->particleM->addParticle(PARTICLE_IMPACT_SMOKE,	Vector3d(blocs[currentBlock]->getLeftBox()->getXMin() - 0.01f, tmpValues[1], tmpValues[2]),30);
										this->particleM->addParticle(PARTICLE_IMPACT,		Vector3d(blocs[currentBlock]->getLeftBox()->getXMin() - 0.02f, tmpValues[1], tmpValues[2]),30);
									}
									else
										this->enemyM->getEnemy(this->projectileM->getProjectile(i)->getSource())->playerDetection(false);
								}
								else
								{
									if(blocs[currentBlock]->isRightBoxExists() && blocs[currentBlock]->getRightBox()->isCollision(tmpValues[0],tmpValues[2],tmpValues[1]))
									{
										hit = true;
										if(this->projectileM->getProjectile(i)->getType() != PROJECTILE_TYPE_ATTACK_TEST)
										{
											if(this->optM->getImpactNumberValue() > 0)
												this->impactM->addImpact(this->projectileM->getProjectile(i)->getImpactId(),this->projectileM->getProjectile(i)->getImpactRadius(),Vector3d(blocs[currentBlock]->getRightBox()->getXMax() + 0.001f,tmpValues[1],tmpValues[2]),Vector3d(0.0f,-90.0f,0.0f),IMPACT_REPLACEMENT_X,this->projectileM->getProjectile(i)->getImpactPolicy());
											blocs[currentBlock]->playSound(SOUNDDISTANCEADDITION + (int)(sqrt(((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]))) / SOUNDDISTANCEMODIFIER);
											this->particleM->addParticle(PARTICLE_IMPACT_SMOKE,	Vector3d(blocs[currentBlock]->getRightBox()->getXMax() + 0.101f, tmpValues[1],tmpValues[2]),30);
											this->particleM->addParticle(PARTICLE_IMPACT,		Vector3d(blocs[currentBlock]->getRightBox()->getXMax() + 0.102f, tmpValues[1],tmpValues[2]),30);
										}
										else
											this->enemyM->getEnemy(this->projectileM->getProjectile(i)->getSource())->playerDetection(false);
									}
									else
									{
										if(blocs[currentBlock]->isDownBoxExists() && blocs[currentBlock]->getDownBox()->isCollision(tmpValues[0],tmpValues[2],tmpValues[1]))
										{
											hit = true;
											if(this->projectileM->getProjectile(i)->getType() != PROJECTILE_TYPE_ATTACK_TEST)
											{
												if(this->optM->getImpactNumberValue() > 0)
													this->impactM->addImpact(this->projectileM->getProjectile(i)->getImpactId(),this->projectileM->getProjectile(i)->getImpactRadius(),Vector3d(tmpValues[0],tmpValues[1],blocs[currentBlock]->getDownBox()->getZMax() + 0.001f),Vector3d(0.0f,180.0f,0.0f),IMPACT_REPLACEMENT_Z,this->projectileM->getProjectile(i)->getImpactPolicy());
												blocs[currentBlock]->playSound(SOUNDDISTANCEADDITION + (int)(sqrt(((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]))) / SOUNDDISTANCEMODIFIER);
												this->particleM->addParticle(PARTICLE_IMPACT_SMOKE,	Vector3d(tmpValues[0], tmpValues[1], blocs[currentBlock]->getDownBox()->getZMax() + 0.01f),30);
												this->particleM->addParticle(PARTICLE_IMPACT,		Vector3d(tmpValues[0], tmpValues[1], blocs[currentBlock]->getDownBox()->getZMax() + 0.02f),30);
											}
											else
												this->enemyM->getEnemy(this->projectileM->getProjectile(i)->getSource())->playerDetection(false);
										}
										else
										{
											if(blocs[currentBlock]->isUpBoxExists() && blocs[currentBlock]->getUpBox()->isCollision(tmpValues[0],tmpValues[2],tmpValues[1]))
											{
												hit = true;
												if(this->projectileM->getProjectile(i)->getType() != PROJECTILE_TYPE_ATTACK_TEST)
												{
													if(this->optM->getImpactNumberValue() > 0)
														this->impactM->addImpact(this->projectileM->getProjectile(i)->getImpactId(),this->projectileM->getProjectile(i)->getImpactRadius(),Vector3d(tmpValues[0],tmpValues[1],blocs[currentBlock]->getUpBox()->getZMin() - 0.001f),Vector3d(0.0f,0.0f,0.0f),IMPACT_REPLACEMENT_MINUS_Z,this->projectileM->getProjectile(i)->getImpactPolicy());
													blocs[currentBlock]->playSound(SOUNDDISTANCEADDITION + (int)(sqrt(((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]))) / SOUNDDISTANCEMODIFIER);
													this->particleM->addParticle(PARTICLE_IMPACT_SMOKE,	Vector3d(tmpValues[0], tmpValues[1], blocs[currentBlock]->getUpBox()->getZMin() - 0.01f),30);
													this->particleM->addParticle(PARTICLE_IMPACT,		Vector3d(tmpValues[0], tmpValues[1], blocs[currentBlock]->getUpBox()->getZMin() - 0.02f),30);
												}
												else
													this->enemyM->getEnemy(this->projectileM->getProjectile(i)->getSource())->playerDetection(false);
											}
											else
											{
												if(this->player1->isCollision(tmpValues[0],tmpValues[2],tmpValues[1]) && this->projectileM->getProjectile(i)->getSource() != PROJECTILE_SOURCE_PLAYER)
												{
													hit = true;
													if(this->projectileM->getProjectile(i)->getType() != PROJECTILE_TYPE_ATTACK_TEST)
														this->player1->takeDamage((int)this->projectileM->getProjectile(i)->getPower());
													else
														this->enemyM->getEnemy(this->projectileM->getProjectile(i)->getSource())->playerDetection(true);
												}
												else
												{
													//Enemies impacts
													for(int j = 0 ; j < this->enemyM->getEnemiesNumber() && !hit ; j++)
													{
														if(this->enemyM->getEnemy(j)->isCollision(tmpValues[0],tmpValues[2],tmpValues[1]) && this->projectileM->getProjectile(i)->getSource() == PROJECTILE_SOURCE_PLAYER)
														{
															hit = true;
															this->enemyM->getEnemy(j)->takeDamage((int)this->projectileM->getProjectile(i)->getPower());
															this->particleM->addParticle(PARTICLE_IMPACT, Vector3d(tmpValues[0] - interValues[0] * cmptModifier * 2.0f,tmpValues[1] - interValues[1] * cmptModifier * 2.0f,tmpValues[2] - interValues[2] * cmptModifier * 2.0f),30);
														}
													}

													// Pillars impacts
													for(unsigned int j = 0 ; j < this->pillars.size() && !hit ; j++)
													{
														if(this->pillars[j]->isCollision(tmpValues[0],tmpValues[2],tmpValues[1]))
														{
															hit = true;
															if(this->projectileM->getProjectile(i)->getType() != PROJECTILE_TYPE_ATTACK_TEST)
															{
																if(this->optM->getImpactNumberValue() > 0)
																{
																	float impactX,impactY,impactZ,impactAngleX,impactAngleY,impactAngleZ;
																	int modif;
																	this->pillars[j]->impactGetLocation(tmpValuesPrevious[0],tmpValuesPrevious[1],tmpValuesPrevious[2],tmpValues[0],tmpValues[1],tmpValues[2],impactX,impactY,impactZ,impactAngleX,impactAngleY,impactAngleZ,modif,this->projectileM->getProjectile(i)->getImpactRadius());
																	this->impactM->addImpact(this->projectileM->getProjectile(i)->getImpactId(),this->projectileM->getProjectile(i)->getImpactRadius(),Vector3d(impactX,impactY,impactZ),Vector3d(impactAngleX,impactAngleY,impactAngleZ),modif,this->projectileM->getProjectile(i)->getImpactPolicy());
																}
																this->pillars[j]->playSound(SOUNDDISTANCEADDITION + (int)(sqrt(((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[0] + interValues[0] * cmpt) - this->player1->getCoor()[0]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[1] + interValues[1] * cmpt) - this->player1->getCoor()[1]) + ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]) * ((this->projectileM->getProjectile(i)->getPreviousCoordinates()[2] + interValues[2] * cmpt) - this->player1->getCoor()[2]))) / SOUNDDISTANCEMODIFIER);
																this->particleM->addParticle(PARTICLE_IMPACT_SMOKE,	Vector3d(tmpValues[0] - interValues[0] * cmptModifier,tmpValues[1] - interValues[1] * cmptModifier,tmpValues[2] - interValues[2] * cmptModifier),30);
																this->particleM->addParticle(PARTICLE_IMPACT,		Vector3d(tmpValues[0] - interValues[0] * cmptModifier * 2.0f,tmpValues[1] - interValues[1] * cmptModifier * 2.0f,tmpValues[2] - interValues[2] * cmptModifier * 2.0f),30);
															}
															else
																this->enemyM->getEnemy(this->projectileM->getProjectile(i)->getSource())->playerDetection(true);
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			if(hit)
				this->projectileM->deleteProjectile(i);
		}

		// If the projectile is still alive and is in an empty block, we delete it
		if(this->projectileM->getProjectile(i) != NULL)
		{
			int currentBlock = this->width*(int)((int)this->projectileM->getProjectile(i)->getCurrentCoordinates()[2] / BLOCDEFAULTSIZE) + (int)(-this->projectileM->getProjectile(i)->getCurrentCoordinates()[0]) / BLOCDEFAULTSIZE;
			if(this->blocs[currentBlock] == NULL)
				this->projectileM->deleteProjectile(i);
		}
	}
}

/**
* Call collision functions
* @param time Elapsed time
*/
void LevelManager::collisions(long time)
{
	objectCollision();
	wallCollision();
	projectileCollision(time);
}

/**
* Fire with the Player
*/
void LevelManager::weaponHit()
{
	// Fire's accuracy
	float accuracy = 10.0f - (float)this->weaponM->getSelectedWeapon()->getAccuracy();

	// Crouch
	if(this->player1->getMovementStatut() == PLAYER_MOVEMENT_CROUCH)
	{
		// Has moved
		if(this->player1->getMoved())
			accuracy = (accuracy * 2) / 3;
		else
			// Not moved
			accuracy /= 3;
	}
	else
	{
		// Stand and hasn't moved
		if(this->player1->getMovementStatut() == PLAYER_MOVEMENT_STAND && !this->player1->getMoved())
			accuracy = (accuracy * 2) / 3;

		// No bonus if the player is in jump mode
	}

	accuracy /= 2.0;
	this->projectileM->addProjectile(Vector3d(player1->getCoor()[0], player1->getCoor()[1] + 3.0f, player1->getCoor()[2]), PROJECTILE_TYPE_INVISIBLE, PROJECTILE_SOURCE_PLAYER, this->weaponM->getSelectedWeapon()->getImpactId(), IMPACT_REPLACEMENT_POLICY_CREATE, this->weaponM->getSelectedWeapon()->getImpactSize(), 50.0f, (float)this->weaponM->getSelectedWeapon()->getRange() * 10.0f,this->player1->getAngleXZ(), this->player1->getAngleY() + 45, accuracy, (float)this->weaponM->getSelectedWeapon()->getPower());
}

/**
* Experimental - Viewing Frustum Culling
* @param blockNumber Block to test
*/
void LevelManager::doViewingFrustumCulling(int blockNumber)
{
	if(this->blocs[blockNumber] != NULL && blockNumber >= 0 && blockNumber < (this->width * this->height))
	{
		bool alreadyChoosen = false;
		for(unsigned int i = 0 ; i < this->drawableBlocs.size() && !alreadyChoosen ; i++)
			if(this->drawableBlocs[i] == blockNumber)
				alreadyChoosen = true;

		if(!alreadyChoosen)
		{
			this->drawableBlocs.push_back(blockNumber);

			// South Part
			if(Vector2d::intersectionTriangleSegment(player1->getViewSector()[0],player1->getViewSector()[1],player1->getViewSector()[2],Vector2d((float)(this->blocs[blockNumber]->getPositionX() * BLOCDEFAULTSIZE - BLOCDEFAULTSIZE),(float)(this->blocs[blockNumber]->getPositionZ() * BLOCDEFAULTSIZE)),Vector2d((float)(this->blocs[blockNumber]->getPositionX() * BLOCDEFAULTSIZE),(float)(this->blocs[blockNumber]->getPositionZ() * BLOCDEFAULTSIZE))) && this->blocs[blockNumber]->isDownPortalExists())
				doViewingFrustumCulling(blockNumber - this->width);

			// West Part
			if(Vector2d::intersectionTriangleSegment(player1->getViewSector()[0],player1->getViewSector()[1],player1->getViewSector()[2],Vector2d((float)(this->blocs[blockNumber]->getPositionX() * BLOCDEFAULTSIZE),(float)(this->blocs[blockNumber]->getPositionZ() * BLOCDEFAULTSIZE)),Vector2d((float)(this->blocs[blockNumber]->getPositionX() * BLOCDEFAULTSIZE),(float)(this->blocs[blockNumber]->getPositionZ() * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE))) && this->blocs[blockNumber]->isLeftPortalExists())
				doViewingFrustumCulling(blockNumber - 1);

			// North Part
			if(Vector2d::intersectionTriangleSegment(player1->getViewSector()[0],player1->getViewSector()[1],player1->getViewSector()[2],Vector2d((float)(this->blocs[blockNumber]->getPositionX() * BLOCDEFAULTSIZE),(float)(this->blocs[blockNumber]->getPositionZ() * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE)),Vector2d((float)(this->blocs[blockNumber]->getPositionX() * BLOCDEFAULTSIZE - BLOCDEFAULTSIZE),(float)(this->blocs[blockNumber]->getPositionZ() * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE))) && this->blocs[blockNumber]->isUpPortalExists())
					doViewingFrustumCulling(blockNumber + this->width);

			// East Part
			if(Vector2d::intersectionTriangleSegment(player1->getViewSector()[0],player1->getViewSector()[1],player1->getViewSector()[2],Vector2d((float)(this->blocs[blockNumber]->getPositionX() * BLOCDEFAULTSIZE - BLOCDEFAULTSIZE),(float)(this->blocs[blockNumber]->getPositionZ() * BLOCDEFAULTSIZE)),Vector2d((float)(this->blocs[blockNumber]->getPositionX() * BLOCDEFAULTSIZE - BLOCDEFAULTSIZE),(float)(this->blocs[blockNumber]->getPositionZ() * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE))) && this->blocs[blockNumber]->isRightPortalExists())
					doViewingFrustumCulling(blockNumber + 1);
		}
	}
}

/**
* Experimental - Occlusion Culling
* Need more work to be more effective
* @param lightPos Light position
*/
void LevelManager::doOcculisionCulling(float * lightPos)
{
	for(unsigned int i = 0 ; i < this->drawableBlocs.size() ; i++)
	{
		bool southTest = false;
		bool westTest = false;
		bool northTest = false;
		bool eastTest = false;
		bool viewAvailible = false;
		bool viewAvailible1 = true;

		if(this->blocs[this->drawableBlocs[i]]->isDownPortalExists())
			southTest = true;

		if(this->blocs[this->drawableBlocs[i]]->isLeftPortalExists())
			westTest = true;

		if(this->blocs[this->drawableBlocs[i]]->isUpPortalExists())
			northTest = true;

		if(this->blocs[this->drawableBlocs[i]]->isRightPortalExists())
			eastTest = true;

		for(unsigned int j = 0 ; j < this->drawableBlocs.size() && viewAvailible1 ; j++)
		{
			if(i != j)
			{
				bool viewAvailible2 = false;

				if(southTest && !viewAvailible2)
				{
					for(int k = 0 ; k < BLOCDEFAULTSIZE && !viewAvailible2; k++)
					{
						if(!this->blocs[this->drawableBlocs[j]]->checkIntersection(this->player1->getViewSector()[0],Vector2d((float)this->blocs[this->drawableBlocs[i]]->getPositionX() * BLOCDEFAULTSIZE - k / 2.0f,(float)this->blocs[this->drawableBlocs[i]]->getPositionZ() * BLOCDEFAULTSIZE)) || !this->blocs[this->drawableBlocs[j]]->checkIntersection(this->player1->getViewSector()[0],Vector2d((float)this->blocs[this->drawableBlocs[i]]->getPositionX() * BLOCDEFAULTSIZE - BLOCDEFAULTSIZE + k / 2.0f,(float)this->blocs[this->drawableBlocs[i]]->getPositionZ() * BLOCDEFAULTSIZE)))
								viewAvailible2 = true;
					}
				}

				if(westTest && !viewAvailible2)
				{
					for(int k = 0 ; k < BLOCDEFAULTSIZE && !viewAvailible2; k++)
					{
						if(!this->blocs[this->drawableBlocs[j]]->checkIntersection(this->player1->getViewSector()[0],Vector2d((float)this->blocs[this->drawableBlocs[i]]->getPositionX() * BLOCDEFAULTSIZE,(float)this->blocs[this->drawableBlocs[i]]->getPositionZ() * BLOCDEFAULTSIZE + k / 2.0f)) || !this->blocs[this->drawableBlocs[j]]->checkIntersection(this->player1->getViewSector()[0],Vector2d((float)this->blocs[this->drawableBlocs[i]]->getPositionX() * BLOCDEFAULTSIZE,(float)this->blocs[this->drawableBlocs[i]]->getPositionZ() * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE - k / 2.0f)))
								viewAvailible2 = true;
					}
				}

				if(northTest && !viewAvailible2)
				{
					for(int k = 0 ; k < BLOCDEFAULTSIZE && !viewAvailible2; k++)
					{
						if(!this->blocs[this->drawableBlocs[j]]->checkIntersection(this->player1->getViewSector()[0],Vector2d((float)this->blocs[this->drawableBlocs[i]]->getPositionX() * BLOCDEFAULTSIZE - k / 2.0f,(float)this->blocs[this->drawableBlocs[i]]->getPositionZ() * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE)) || !this->blocs[this->drawableBlocs[j]]->checkIntersection(this->player1->getViewSector()[0],Vector2d((float)this->blocs[this->drawableBlocs[i]]->getPositionX() * BLOCDEFAULTSIZE - BLOCDEFAULTSIZE + k / 2.0f,(float)this->blocs[this->drawableBlocs[i]]->getPositionZ() * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE)))
								viewAvailible2 = true;
					}
				}

				if(eastTest && !viewAvailible2)
				{
					for(int k = 0 ; k < BLOCDEFAULTSIZE && !viewAvailible2; k++)
					{
						if(!this->blocs[this->drawableBlocs[j]]->checkIntersection(this->player1->getViewSector()[0],Vector2d((float)this->blocs[this->drawableBlocs[i]]->getPositionX() * BLOCDEFAULTSIZE - BLOCDEFAULTSIZE,(float)this->blocs[this->drawableBlocs[i]]->getPositionZ() * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE - k / 2.0f)) || !this->blocs[this->drawableBlocs[j]]->checkIntersection(this->player1->getViewSector()[0],Vector2d((float)this->blocs[this->drawableBlocs[i]]->getPositionX() * BLOCDEFAULTSIZE - BLOCDEFAULTSIZE,(float)this->blocs[this->drawableBlocs[i]]->getPositionZ() * BLOCDEFAULTSIZE + k / 2.0f)))
								viewAvailible2 = true;
					}
				}
				if(!viewAvailible2)
					viewAvailible1 = false;
			}
		}
		if(viewAvailible1)
			// We draw the block and the objects inside
			drawBloc(lightPos, this->drawableBlocs[i]);
	}
}

/**
* Draw a block and the objects inside
* @param lightPos Light position
* @param blockNumber Block number to be drawn
*/
void LevelManager::drawBloc(float *lightPos, int blockNumber)
{
	// Draw the block
	this->blocs[blockNumber]->draw(this->player1->getCoor()[0], this->player1->getCoor()[2]);

	// Draw the objects
	for(int i = 0 ; i < 5 ; i++)
	{
		int tmpI = (this->blocs[blockNumber]->getPositionZ() * BLOCDEFAULTSIZE) / 2 + i;
		for(int j = 0 ; j < 5 ; j++)
		{
			int tmpJ = -(this->blocs[blockNumber]->getPositionX() * BLOCDEFAULTSIZE) / 2 + j;
			if(this->objects[tmpI][tmpJ] != NULL)
				this->objects[tmpI][tmpJ]->draw(lightPos);
		}
	}

	// Draw the Pillars
	for(unsigned int i = 0 ; i < this->pillars.size() ; i++)
	{
		if(Vector2d::pointInRectangle(Vector2d((float)this->blocs[blockNumber]->getPositionX() * BLOCDEFAULTSIZE + PILLARDEFAULTTICKNESS, (float)this->blocs[blockNumber]->getPositionZ() * BLOCDEFAULTSIZE - PILLARDEFAULTTICKNESS), Vector2d((float)this->blocs[blockNumber]->getPositionX() * BLOCDEFAULTSIZE - BLOCDEFAULTSIZE - PILLARDEFAULTTICKNESS, (float)this->blocs[blockNumber]->getPositionZ() * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE + PILLARDEFAULTTICKNESS), Vector2d(this->pillars[i]->getX(), this->pillars[i]->getZ())))
		{
			bool pillarAlreadyDrawn = false;
			for(unsigned j = 0 ; j < this->drawablePillars.size() ; j++)
				if(this->drawablePillars[j] == this->pillars[i]->getObjectNumber())
					pillarAlreadyDrawn = true;

			if(!pillarAlreadyDrawn)
			{
				this->drawablePillars.push_back(this->pillars[i]->getObjectNumber());
				this->pillars[i]->draw(this->player1->getCoor()[0], this->player1->getCoor()[2]);
			}
		}
	}

	// Draw the inter blocs
	for(unsigned int i = 0 ; i < this->interBlocs.size() ; i++)
	{
		if(Vector2d::pointInRectangle(Vector2d((float)this->blocs[blockNumber]->getPositionX() * BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS, (float)this->blocs[blockNumber]->getPositionZ() * BLOCDEFAULTSIZE - INTERBLOCDEFAULTTICKNESS), Vector2d((float)this->blocs[blockNumber]->getPositionX() * BLOCDEFAULTSIZE - BLOCDEFAULTSIZE - INTERBLOCDEFAULTTICKNESS, (float)this->blocs[blockNumber]->getPositionZ() * BLOCDEFAULTSIZE + BLOCDEFAULTSIZE + INTERBLOCDEFAULTTICKNESS), Vector2d(this->interBlocs[i]->getX(), this->interBlocs[i]->getZ())))
		{
			bool interBlocAlreadyDrawn = false;
			for(unsigned j = 0 ; j < this->drawableInterBlocs.size() ; j++)
				if(this->drawableInterBlocs[j] == this->interBlocs[i]->getObjectNumber())
					interBlocAlreadyDrawn = true;

			if(!interBlocAlreadyDrawn)
			{
				this->drawableInterBlocs.push_back(this->interBlocs[i]->getObjectNumber());
				this->interBlocs[i]->draw();
			}
		}
	}
}

/**
* Open neighboring doors depending of the door triggered
* @param x X coordinate of the triggered door
* @param z Z coordinate of the trigerred door
*/
void LevelManager::openNeighboringDoors(int x, int z)
{
	if(objects[z][x]->getType() == OBJECTTYPE_DOOR_VERTICAL || objects[z][x]->getType() == OBJECTTYPE_LOCKEDDOOR_VERTICAL)
	{
		int doorX = x;
		int doorZ = z - 1;
		while(doorZ >= 0 && objects[doorZ][doorX] != NULL && (objects[doorZ][doorX]->getType() == OBJECTTYPE_DOOR_VERTICAL || objects[doorZ][doorX]->getType() == OBJECTTYPE_LOCKEDDOOR_VERTICAL))
		{
			objects[doorZ][doorX]->useObject();
			doorZ--;
		}

		doorX = x;
		doorZ = z + 1;
		while(doorZ < this->height * 5 && objects[doorZ][doorX] != NULL && (objects[doorZ][doorX]->getType() == OBJECTTYPE_DOOR_VERTICAL || objects[doorZ][doorX]->getType() == OBJECTTYPE_LOCKEDDOOR_VERTICAL))
		{
			objects[doorZ][doorX]->useObject();
			doorZ++;
		}
	}
	else
	{
		if(objects[z][x]->getType() == OBJECTTYPE_DOOR_HORIZONTAL || objects[z][x]->getType() == OBJECTTYPE_LOCKEDDOOR_HORIZONTAL)
		{
			int doorX = x - 1;
			int doorZ = z;
			while(doorX >= 0 && objects[doorZ][doorX] != NULL && (objects[doorZ][doorX]->getType() == OBJECTTYPE_DOOR_HORIZONTAL || objects[doorZ][doorX]->getType() == OBJECTTYPE_LOCKEDDOOR_HORIZONTAL))
			{
				objects[doorZ][doorX]->useObject();
				doorX--;
			}

			doorX = x + 1;
			doorZ = z;
			while(doorX < this->width * 5 && objects[doorZ][doorX] != NULL && (objects[doorZ][doorX]->getType() == OBJECTTYPE_DOOR_HORIZONTAL || objects[doorZ][doorX]->getType() == OBJECTTYPE_LOCKEDDOOR_HORIZONTAL))
			{
				objects[doorZ][doorX]->useObject();
				doorX++;
			}
		}
	}
}

/**
* Generic function to replace the player after a collision
* @param XTest X decal test
* @param ZTest Z decal test
*/
void LevelManager::performPlayerReplacement(float XTest, float ZTest)
{
	if(player1->getPreviousObjectCoordinateX() < player1->getCurrentObjectCoordinateX(XTest) && (player1->getPreviousObjectCoordinateZ(-3) == player1->getCurrentObjectCoordinateZ(ZTest,-3) || player1->getPreviousObjectCoordinateZ(-1) == player1->getCurrentObjectCoordinateZ(ZTest,-1)))
	{
		player1->setCoor(0,player1->getCurrentObjectCoordinateX(XTest) * -2 + PLAYERDETECTION + 0.01f);
	}
	else
	{
		if(player1->getPreviousObjectCoordinateX() > player1->getCurrentObjectCoordinateX(XTest) && (player1->getPreviousObjectCoordinateZ(-3) == player1->getCurrentObjectCoordinateZ(ZTest,-3) || player1->getPreviousObjectCoordinateZ(-1) == player1->getCurrentObjectCoordinateZ(ZTest,-1)))
		{
			player1->setCoor(0,player1->getCurrentObjectCoordinateX(XTest) * -2 - PLAYERDETECTION * 2 - (2.0f - PLAYERDETECTION) - 0.01f);
		}
		else
		{
			if(player1->getPreviousObjectCoordinateZ(-2) < player1->getCurrentObjectCoordinateZ(ZTest,-2) && player1->getPreviousObjectCoordinateX() == player1->getCurrentObjectCoordinateX(XTest))
			{
				player1->setCoor(2,player1->getCurrentObjectCoordinateZ(ZTest,0) * 2 - PLAYERDETECTION - 0.01f);
			}
			else
			{
				if(player1->getPreviousObjectCoordinateZ(-2) > player1->getCurrentObjectCoordinateZ(ZTest,-2) && player1->getPreviousObjectCoordinateX() == player1->getCurrentObjectCoordinateX(XTest))
					player1->setCoor(2,player1->getCurrentObjectCoordinateZ(ZTest,0) * 2 + 2 + PLAYERDETECTION + 0.01f);
			}
		}
	}
}
