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
#include "../manager/soundManager.h"

/**
* Constructor
*/
SoundManager::SoundManager()
{
	this->music = NULL;
	this->modM = ModManager::getInstance();
	this->logM = LogManager::getInstance();
	this->optM = OptionManager::getInstance();
	freeSoundDatabase();
	refreshMusicVolume();
	refreshSoundVolume();
	openAudio();
}

/**
* Destructor
*/
SoundManager::~SoundManager()
{
	if(this->music != NULL)
	{
		if(Mix_PlayingMusic())
			Mix_PauseMusic();
		Mix_FreeMusic(this->music);
		this->music = NULL;
	}
	freeSoundDatabase();
	closeAudio();
}

/**
* Free the sound database
*/
void SoundManager::freeSoundDatabase()
{
	for( soundIterator = soundDatabase.begin(); soundIterator != soundDatabase.end(); )
	{
		Mix_FreeChunk((*soundIterator).second);
		soundDatabase.erase( soundIterator++ );
	}
	soundDatabase["dummy"] = NULL;

	if(this->music != NULL)
	{
		Mix_FreeMusic(this->music);
		this->music = NULL;
	}

	if(this->optM->isDebug())
			this->logM->logIt(std::string("Releasing"),std::string("sounds"));
}

/**
* Search for the sound in the database
* @param soudPath Path of the sound
* @return A pointer to the sound ou NULL
*/
Mix_Chunk * SoundManager::getSound(std::string soundPath)
{
	if((soundIterator = soundDatabase.find( soundPath )) != soundDatabase.end())
		return (*soundIterator).second;
	else
		return NULL;
}

/**
* Load and return a pointer to the sound
* @param soundPath Path of the sound
* @return A pointer to the sound
*/
Mix_Chunk * SoundManager::loadSound(std::string soundPath)
{
	std::ostringstream stringstream;

	// Verify if the custom sounds are enabled
	if(this->modM->getLevelsConfiguration(MOD_CONFIGURATION_LEVELS_SOUNDS))
	{
			std::ifstream file;
			stringstream.str("");
			stringstream << this->modM->getModRootDirectory() << soundPath;
			file.open(stringstream.str().c_str(), std::ios::in);
			if(file)
			{
				soundPath = stringstream.str();
				file.close();
			}
	}

	// Verify if the sound is already loaded
	if(getSound(soundPath) == NULL)
	{
		if(this->optM->isDebug())
			this->logM->logIt(std::string("Loading sound:"),soundPath);
		soundDatabase[soundPath] = Mix_LoadWAV(soundPath.c_str());
		return soundDatabase[soundPath];
	}
	else
		return getSound(soundPath);
}

/**
* Load the music
* @param musicPath Music file path
*/
void SoundManager::loadMusic(std::string musicPath)
{
	if(this->music != NULL)
	{
		if(Mix_PlayingMusic())
			Mix_PauseMusic();
		Mix_FreeMusic(this->music);
		this->music = NULL;
	}
	if(this->optM->isDebug())
			this->logM->logIt(std::string(std::string("Loading music:")),musicPath);
	this->music = Mix_LoadMUS(musicPath.c_str());
}

/**
* Play a sound
* @param soundPath Sound file path
*/
void SoundManager::playSound(Mix_Chunk * soundPath)
{
	if(this->soundVolume > 0 && soundPath != NULL)
	{
		Mix_VolumeChunk(soundPath, this->soundVolume);
		Mix_PlayChannel(-1,soundPath,0);
	}
}

/**
* Play a sound with a modified volume
* @param soundPath Sound file path
* @param volume Volume modifier
*/
void SoundManager::playSound(Mix_Chunk * soundPath, int volume)
{
	if((int)(this->soundVolume / volume) > 0 && soundPath != NULL)
	{
		Mix_VolumeChunk(soundPath, (int)(this->soundVolume / volume));
		Mix_PlayChannel(-1,soundPath,0);
	}
}

/**
* Play the music
*/
void SoundManager::playMusic()
{
	Mix_PlayMusic(this->music,-1);
}

/**
* Stop the music
*/
void SoundManager::stopMusic()
{
	Mix_PauseMusic();
}

/**
* Refresh the sound volume
*/
void SoundManager::refreshSoundVolume()
{
	this->soundVolume = (int)(OptionManager::getInstance()->getSoundVolume() * MIX_MAX_VOLUME / 10);
}

/**
* Refresh the music volume
*/
void SoundManager::refreshMusicVolume()
{
	this->musicVolume = (int)(OptionManager::getInstance()->getMusicVolume() * MIX_MAX_VOLUME / 10);
	Mix_VolumeMusic(this->musicVolume);
}

/**
* Open the audio system
*/
void SoundManager::openAudio()
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
	Mix_AllocateChannels(64);
}

/**
* Close the audio system
*/
void SoundManager::closeAudio()
{
	Mix_CloseAudio();
}
