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
#include <SDL/SDL_mixer.h>
#include "../includes/global_var.h"
#include "../tinyXML/tinyxml.h"
#include "../manager/optionManager.h"

/**
* Constructor
*/
OptionManager::OptionManager()
{
	this->logM = LogManager::getInstance();

	this->resolutionXTable[0] = 640;
	this->resolutionXTable[1] = 800;
	this->resolutionXTable[2] = 1024;
	this->resolutionXTable[3] = 1280;
	this->resolutionXTable[4] = 1600;
	this->resolutionXTable[5] = 1280;
	this->resolutionXTable[6] = 1440;
	this->resolutionXTable[7] = 1680;
	this->resolutionXTable[8] = 1920;

	this->resolutionYTable[0] = 480;
	this->resolutionYTable[1] = 600;
	this->resolutionYTable[2] = 768;
	this->resolutionYTable[3] = 960;
	this->resolutionYTable[4] = 1200;
	this->resolutionYTable[5] = 800;
	this->resolutionYTable[6] = 900;
	this->resolutionYTable[7] = 1050;
	this->resolutionYTable[8] = 1200;

	this->language = 0;
	this->noClip = false;
	this->log = false;
	this->fogQuality = 2;
	this->fogMode = 2;
	this->debugModeActivated = false;

	this->resolution = 1;
	this->isFullScreenEnabled = false;
	this->isWideScreenEnabled = false;
	this->isFogEnabled = true;
	this->globalVisibility = 6;
	this->mouseSensibility = 2;
	this->invertYMouseAxis = 0;
	this->mouseSmoothing = 0;
	this->antialiasingLevel = 0;
	this->impactNumber = 1;
	setLightQuality(3);
	setTexturesQuality(2);
	setGamma(5);
	setSoundVolume(5);
	setMusicVolume(5);
	setImpactNumberValue(3);
	setParticleModifierValue(3);

	// Open option file
	TiXmlDocument doc("conf/daidalos_options.xml");
	if(doc.LoadFile())
	{
		TiXmlHandle hdl(&doc);
		TiXmlElement * elem;

		elem = hdl.FirstChildElement("game_options").FirstChildElement("resolution").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setResolution((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("fullscreen").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setFullScreen((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("visibility").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setGlobalVisibility((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("mouse_sensibility").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setMouseSensibility((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("invert_y_axis").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setInvertYMouseAxis((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("mouse_smoothing").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setMouseSmoothing((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("gamma").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setGamma((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("antialiasing").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setAntialiasingLevel((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("filtering").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setTextureFilteringLevel((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("light_quality").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setLightQuality((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("textures_quality").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setTexturesQuality((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("impacts_value").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setImpactNumberValue((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("particles_value").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setParticleModifierValue((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("sound_volume").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setSoundVolume((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("music_volume").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setMusicVolume((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("language").Element();
		if(elem != NULL && elem->GetText() != NULL)
			setLanguage((int)atoi(elem->GetText()));

		elem = hdl.FirstChildElement("game_options").FirstChildElement("debug").Element();
		if(elem != NULL && elem->GetText() != NULL)
		{
			int tmpInt = (int)atoi(elem->GetText());
			if(tmpInt == 1)
				this->debugModeActivated = true;
			else
				this->debugModeActivated = false;
		}
	}
	else
	{
		if(this->debugModeActivated)
			this->logM->logIt(std::string("Unable to open configuration file:"), std::string("conf/daidalos_options.xml"));
	}
}

/**
* Save the current configuration
*/
void OptionManager::save()
{
	if(this->debugModeActivated)
		this->logM->logIt(std::string("Saving"),std::string("configuration file"));

	std::ostringstream stringstream;

	TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    doc.LinkEndChild( decl );
    
    TiXmlElement * root = new TiXmlElement("game_options");
    doc.LinkEndChild(root);

	stringstream.str("");
	stringstream << getResolutionNumber();
	TiXmlElement * resolution = new TiXmlElement("resolution");
    TiXmlText * resolution_text = new TiXmlText(stringstream.str());
	resolution->LinkEndChild(resolution_text);
	root->LinkEndChild(resolution);

	stringstream.str("");
	if(isFullScreen())
			stringstream << 1;
		else
			stringstream << 0;
	TiXmlElement * fullscreen = new TiXmlElement("fullscreen");
    TiXmlText * fullscreen_text = new TiXmlText(stringstream.str());
	fullscreen->LinkEndChild(fullscreen_text);
	root->LinkEndChild(fullscreen);

	stringstream.str("");
	stringstream << getGlobalVisibility();
	TiXmlElement * visibility = new TiXmlElement("visibility");
    TiXmlText * visibility_text = new TiXmlText(stringstream.str());
	visibility->LinkEndChild(visibility_text);
	root->LinkEndChild(visibility);

	stringstream.str("");
	stringstream << getMouseSensibility();
	TiXmlElement * sensibility = new TiXmlElement("mouse_sensibility");
    TiXmlText * sensibility_text = new TiXmlText(stringstream.str());
	sensibility->LinkEndChild(sensibility_text);
	root->LinkEndChild(sensibility);

	stringstream.str("");
	stringstream << getInvertYMouseAxis();
	TiXmlElement * yaxis = new TiXmlElement("invert_y_axis");
    TiXmlText * yaxis_text = new TiXmlText(stringstream.str());
	yaxis->LinkEndChild(yaxis_text);
	root->LinkEndChild(yaxis);

	stringstream.str("");
	stringstream << getMouseSmoothing();
	TiXmlElement * smoothing = new TiXmlElement("mouse_smoothing");
    TiXmlText * smoothing_text = new TiXmlText(stringstream.str());
	smoothing->LinkEndChild(smoothing_text);
	root->LinkEndChild(smoothing);

	stringstream.str("");
	stringstream << getGamma();
	TiXmlElement * gamma = new TiXmlElement("gamma");
    TiXmlText * gamma_text = new TiXmlText(stringstream.str());
	gamma->LinkEndChild(gamma_text);
	root->LinkEndChild(gamma);

	stringstream.str("");
	stringstream << getAntialiasingLevel();
	TiXmlElement * antialiasing = new TiXmlElement("antialiasing");
    TiXmlText * antialiasing_text = new TiXmlText(stringstream.str());
	antialiasing->LinkEndChild(antialiasing_text);
	root->LinkEndChild(antialiasing);

	stringstream.str("");
	stringstream << getTextureFilteringLevel();
	TiXmlElement * filtering = new TiXmlElement("filtering");
    TiXmlText * filtering_text = new TiXmlText(stringstream.str());
	filtering->LinkEndChild(filtering_text);
	root->LinkEndChild(filtering);

	stringstream.str("");
	stringstream << getLightQuality();
	TiXmlElement * light = new TiXmlElement("light_quality");
    TiXmlText * light_text = new TiXmlText(stringstream.str());
	light->LinkEndChild(light_text);
	root->LinkEndChild(light);

	stringstream.str("");
	stringstream << getTexturesQuality();
	TiXmlElement * textures = new TiXmlElement("textures_quality");
    TiXmlText * textures_text = new TiXmlText(stringstream.str());
	textures->LinkEndChild(textures_text);
	root->LinkEndChild(textures);

	stringstream.str("");
	stringstream << getSoundVolume();
	TiXmlElement * sound = new TiXmlElement("sound_volume");
    TiXmlText * sound_text = new TiXmlText(stringstream.str());
	sound->LinkEndChild(sound_text);
	root->LinkEndChild(sound);

	stringstream.str("");
	stringstream << getMusicVolume();
	TiXmlElement * music = new TiXmlElement("music_volume");
    TiXmlText * music_text = new TiXmlText(stringstream.str());
	music->LinkEndChild(music_text);
	root->LinkEndChild(music);

	stringstream.str("");
	stringstream << getImpactNumberValue();
	TiXmlElement * impacts = new TiXmlElement("impacts_value");
    TiXmlText * impacts_text = new TiXmlText(stringstream.str());
	impacts->LinkEndChild(impacts_text);
	root->LinkEndChild(impacts);

	stringstream.str("");
	stringstream << getParticleModifierValue();
	TiXmlElement * particles = new TiXmlElement("particles_value");
    TiXmlText * particles_text = new TiXmlText(stringstream.str());
	particles->LinkEndChild(particles_text);
	root->LinkEndChild(particles);

	stringstream.str("");
	stringstream << getLanguage();
	TiXmlElement * language = new TiXmlElement("language");
    TiXmlText * language_text = new TiXmlText(stringstream.str());
	language->LinkEndChild(language_text);
	root->LinkEndChild(language);

	stringstream.str("");
	if(isDebug())
			stringstream << 1;
		else
			stringstream << 0;
	TiXmlElement * tiDebug = new TiXmlElement("debug");
    TiXmlText * debug_text = new TiXmlText(stringstream.str());
	tiDebug->LinkEndChild(debug_text);
	root->LinkEndChild(tiDebug);
    
    doc.SaveFile( "conf/daidalos_options.xml" );
}

/**
* Return if logs are activated
* @return Log state
*/
bool OptionManager::isLogActivated()
{
	return this->log;
}

/**
* Return if no Clip is activated (no collisions)
* @return No clip state
*/
bool OptionManager::isNoClipActivated()
{
	return this->noClip;
}

/**
* Return if full screen is activated
* @return Full screen state
*/
bool OptionManager::isFullScreen()
{
	return this->isFullScreenEnabled;
}

/**
* Return if wide screen is activated
* @return Wide screen state
*/
bool OptionManager::isWideScreen()
{
	return this->isWideScreenEnabled;
}

/**
* Return if the fog is activated
* @return Fog state
*/
bool OptionManager::isFog()
{
	return this->isFogEnabled;
}

/**
* Return if the debug mode is actvated
* @return Debug mode state
*/
bool OptionManager::isDebug()
{
	return this->debugModeActivated;
}

/**
* Return the fog mode choosen
* @return Fog mode
*/
GLint OptionManager::getFogMode()
{
	GLint v;
	if(this->fogMode == 0)
		v = GL_EXP;
	else if(this->fogMode == 1)
		v = GL_EXP2;
	else
		v = GL_LINEAR;

	return v;
}

/**
* Return the fog quality
* @return Fog quality
*/
GLenum OptionManager::getFogQuality()
{
	GLenum v;
	if(this->fogQuality == 0)
		v = GL_DONT_CARE;
	else if(this->fogQuality == 1)
		v = GL_FASTEST;
	else
		v = GL_NICEST;

	return v;
}

/**
* Return the language
* @return Language choosen
*/
int OptionManager::getLanguage()
{
	return this->language;
}

/**
* Return the resolution
* @param resX Reference to width variable
* @param resY Reference to height variable
*/
void OptionManager::getResolution(int &resX, int &resY)
{
	resX = this->resolutionXTable[this->resolution];
	resY = this->resolutionYTable[this->resolution];
}

/**
* Return the Height resolution
* @param resNumber Resolution number
* @return Height
*/
int OptionManager::getResolutionHeight(int resNumber)
{
	return this->resolutionYTable[resNumber];
}

/**
* Return the Width resolution
* @param resNumber Resolution number
* @return Width
*/
int OptionManager::getResolutionWidth(int resNumber)
{
	return this->resolutionXTable[resNumber];
}

/**
* Return the Height resolution
* @return Height
*/
int OptionManager::getResolutionHeight()
{
	return this->resolutionYTable[this->resolution];
}

/**
* Return the Width resolution
* @return Width
*/
int OptionManager::getResolutionWidth()
{
	return this->resolutionXTable[this->resolution];
}

/**
* Return the resolution number
* @return Resolution number
*/
int OptionManager::getResolutionNumber()
{
	return this->resolution;
}

/**
* Return the visibility
* @return Visibility
*/
int OptionManager::getGlobalVisibility()
{
	return this->globalVisibility;
}

/**
* Return the mouse sensibility
* @return Mouse sensibility
*/
int OptionManager::getMouseSensibility()
{
	return this->mouseSensibility;
}

/**
* Return if the Y axis is inverted
* @return Y axis state
*/
int OptionManager::getInvertYMouseAxis()
{
	return this->invertYMouseAxis;
}

/**
* Return the mouse smoothing value
* @return Mouse smoothing value
*/
int OptionManager::getMouseSmoothing()
{
	return this->mouseSmoothing;
}

/**
* Return the antialiasing level
* @return Antialiasing level
*/
int OptionManager::getAntialiasingLevel()
{
	return this->antialiasingLevel;
}

/**
* Return the texture filtering level
* =0: Trilinear
* >0: Trilinear Anisotropic Vx
* @return Filtering level
*/
int OptionManager::getTextureFilteringLevel()
{
	return this->textureFilteringLevel;
}

/**
* Return the blocks quality
* @return Blocks quality
*/
int OptionManager::getBlocQuality()
{
	return this->blocQuality;
}

/**
* Return the blocs quality
* @param v Quality choosen
* @return Block quality
*/
int OptionManager::getBlocQuality(int v)
{
	int ret = 0;
	switch(v)
	{
		case 0:
		ret = BLOCQUALITY0;
		break;

		case 1:
		ret = BLOCQUALITY1;
		break;

		case 2:
		ret = BLOCQUALITY2;
		break;

		case 3:
		ret = BLOCQUALITY3;
		break;
	}
	return ret;
}

/**
* Return the ground quality
* @return Ground quality
*/
int OptionManager::getGroundQuality()
{
	return this->groundQuality;
}

/**
* Return the ground quality
* @param v Quality choosen
* @return Ground quality
*/
int OptionManager::getGroundQuality(int v)
{
	int ret = 0;
	switch(v)
	{
		case 0:
		ret = GROUNDQUALITY0;
		break;

		case 1:
		ret = GROUNDQUALITY1;
		break;

		case 2:
		ret = GROUNDQUALITY2;
		break;

		case 3:
		ret = GROUNDQUALITY3;
		break;
	}
	return ret;
}

/**
* Return the crates quality
* @return Crate quality
*/
int OptionManager::getCrateQuality()
{
	return this->crateQuality;
}

/**
* Return the crates quality for v quality
* @param v Quality choosen
* @return Crate quality
*/
int OptionManager::getCrateQuality(int v)
{
	int ret = 0;
	switch(v)
	{
		case 0:
		ret = CRATEQUALITY0;
		break;

		case 1:
		ret = CRATEQUALITY1;
		break;

		case 2:
		ret = CRATEQUALITY2;
		break;

		case 3:
		ret = CRATEQUALITY3;
		break;
	}
	return ret;
}

/**
* Return the pillars quality
* @return Pillar quality
*/
int OptionManager::getPillarQuality()
{
	return this->pillarQuality;
}

/**
* Return the pillars quality
* @param v Quality choosen
* @return Pillar quality
*/
int OptionManager::getPillarQuality(int v)
{
	int ret = 0;
	switch(v)
	{
		case 0:
		ret = PILLARQUALITY0;
		break;

		case 1:
		ret = PILLARQUALITY1;
		break;

		case 2:
		ret = PILLARQUALITY2;
		break;

		case 3:
		ret = PILLARQUALITY3;
		break;
	}
	return ret;
}

/**
* Return the small walls quality
* @return Small wall quality
*/
int OptionManager::getSmallWallQuality()
{
	return this->smallWallQuality;
}

/**
* Return the small walls quality
* @param v Quality choosen
* @return Small wall quality
*/
int OptionManager::getSmallWallQuality(int v)
{
	int ret = 0;
	switch(v)
	{
		case 0:
		ret = SMALLWALLQUALITY0;
		break;

		case 1:
		ret = SMALLWALLQUALITY1;
		break;

		case 2:
		ret = SMALLWALLQUALITY2;
		break;

		case 3:
		ret = SMALLWALLQUALITY3;
		break;
	}
	return ret;
}

/**
* Return the door quality
* @return Door quality
*/
int OptionManager::getDoorQuality()
{
	return this->doorQuality;
}

/**
* Return the door quality
* @param v Quality choosen
* @return Door quality
*/
int OptionManager::getDoorQuality(int v)
{
	int ret = 0;
	switch(v)
	{
		case 0:
		ret = DOORQUALITY0;
		break;

		case 1:
		ret = DOORQUALITY1;
		break;

		case 2:
		ret = DOORQUALITY2;
		break;

		case 3:
		ret = DOORQUALITY3;
		break;
	}
	return ret;
}

/**
* Return the door upper quality
* @return Door upper quality
*/
int OptionManager::getDoorUpperQuality()
{
	return this->doorUpperQuality;
}

/**
* Return the door upper quality
* @param v Quality choosen
* @return Door upper quality
*/
int OptionManager::getDoorUpperQuality(int v)
{
	int ret = 0;
	switch(v)
	{
		case 0:
		ret = DOORUPPERQUALITY0;
		break;

		case 1:
		ret = DOORUPPERQUALITY1;
		break;

		case 2:
		ret = DOORUPPERQUALITY2;
		break;

		case 3:
		ret = DOORUPPERQUALITY3;
		break;
	}
	return ret;
}

/**
* Return the red gamma
* @return Red gamma
*/
int OptionManager::getGammaR()
{
	return this->gammaR;
}

/**
* Return the green gamma
* @return Green gamma
*/
int OptionManager::getGammaG()
{
	return this->gammaG;
}

/**
* Return the blue gamma
* @return Blue gamma
*/
int OptionManager::getGammaB()
{
	return this->gammaB;
}

/**
* Return the gamma
* @return Gamma
*/
int OptionManager::getGamma()
{
	return this->gamma;
}

/**
* Return the textures quality
* @return Texture quality
*/
int OptionManager::getTexturesQuality()
{
	return this->texturesQuality;
}

/**
* Return the light quality
* @return Light quality
*/
int OptionManager::getLightQuality()
{
	return this->lightQuality;
}

/**
* Return the sound volume
* @return Sound volume
*/
int OptionManager::getSoundVolume()
{
	return this->soundVolume;
}

/**
* Return the music volume
* @return Music volume
*/
int OptionManager::getMusicVolume()
{
	return this->musicVolume;
}

/**
* Return the impact number value
* @return Impact number value
*/
int OptionManager::getImpactNumberValue()
{
	return this->impactNumber;
}

/**
* Return the impact number
* @return Impact number
*/
int OptionManager::getImpactNumber()
{
	int ret = 0;
	switch(this->impactNumber)
	{
		case 0:
		ret = 0;
		break;

		case 1:
		ret = 100;
		break;

		case 2:
		ret = 300;
		break;

		case 3:
		ret = 600;
		break;
	}
	return ret;
}

/**
* Return the particle modifier value
* @return Particle modifier value
*/
int OptionManager::getParticleModifierValue()
{
	return this->particleModifier;
}

/**
* Return the particle modifier
* @return Particle modifier
*/
int OptionManager::getParticleModifier()
{
	int ret = 0;
	switch(this->particleModifier)
	{
		case 0:
		ret = 0;
		break;

		case 1:
		ret = 4;
		break;

		case 2:
		ret = 2;
		break;

		case 3:
		ret = 1;
		break;
	}
	return ret;
}

/**
* Set the language
* @param v New language number
*/
void OptionManager::setLanguage(int v)
{
	this->language = v;
}

/**
* Set no clip mode
* @param v No clip state
*/
void OptionManager::setNoClip(bool v)
{
	this->noClip = v;
}

/**
* Set full screen
* @param v Full screen state
*/
void OptionManager::setFullScreen(int v)
{
	if(v == 0)
		this->isFullScreenEnabled = false;
	else
		this->isFullScreenEnabled = true;
}

/**
* Set wide screen
* @param v Wide screen state
*/
void OptionManager::setWideScreen(int v)
{
	if(v == 0)
		this->isWideScreenEnabled = false;
	else
		this->isWideScreenEnabled = true;
}

/**
* Set the fog
* @param v Fog state
*/
void OptionManager::setFog(int v)
{
	if(v == 0)
		this->isFogEnabled = false;
	else
		this->isFogEnabled = true;
}

/**
* Set the fog mode
* @param v Fog mode
*/
void OptionManager::setFogMode(int v)
{
	this->fogMode = v;
}

/**
* Set the fog quality
* @param v Fog quality
*/
void OptionManager::setFogQuality(int v)
{
	this->fogQuality = v;
}

/**
* Set the resolution
* @param v Resolution number
*/
void OptionManager::setResolution(int v)
{
	if(v >= 0 && v < 9)
		this->resolution = v;
	else
		this->resolution = 0;
}

/**
* Set the visibility
* @param v Visibility
*/
void OptionManager::setGlobalVisibility(int v)
{
	if(v >= 0 && v < 7)
		this->globalVisibility = v;
	else
		this->globalVisibility = 6;
}

/**
* Set the mouse sensibility
* @param v Mouse sensibility
*/
void OptionManager::setMouseSensibility(int v)
{
	if(v >= 0 && v <= 10)
		this->mouseSensibility = v;
	else
		this->mouseSensibility = 4;
}

/**
* Set the mouse sensibility
* @param v Invert Y mouse state
*/
void OptionManager::setInvertYMouseAxis(int v)
{
	if(v == 1)
		this->invertYMouseAxis = 1;
	else
		this->invertYMouseAxis = 0;
}

/**
* Set the mouse smoothing
* @param v Mouse smoothing value
*/
void OptionManager::setMouseSmoothing(int v)
{
	if(v >= 0 && v < 5)
		this->mouseSmoothing = v;
	else
		this->mouseSmoothing = 0;
}

/**
* Set the antialiasing level
* @param v Antialiasing level
*/
void OptionManager::setAntialiasingLevel(int v)
{
	if(v > 0 && v <= 16 && v%2 == 0)
		this->antialiasingLevel = v;
	else
		this->antialiasingLevel = 0;
}

/**
* Set the texture filtering level
* @param v Filetring level
*/
void OptionManager::setTextureFilteringLevel(int v)
{
	if(v > 0 && v <= 16 && v%2 == 0)
		this->textureFilteringLevel = v;
	else
		this->textureFilteringLevel = 0;
}

/**
* Set the block quality
* @param v Block quality
*/
void OptionManager::setBlocQuality(int v)
{
	this->blocQuality = v;
}

/**
* Set the ground quality
* @param v Ground quality
*/
void OptionManager::setGroundQuality(int v)
{
	this->groundQuality = v;
}

/**
* Set the crate quality
* @param v Crate quality
*/
void OptionManager::setCrateQuality(int v)
{
	this->crateQuality = v;
}

/**
* Set the pillar quality
* @param v Pillar quality
*/
void OptionManager::setPillarQuality(int v)
{
	this->pillarQuality = v;
}

/**
* Set the small wall quality
* @param v Small wall quality
*/
void OptionManager::setSmallWallQuality(int v)
{
	this->smallWallQuality = v;
}

/**
* Set the door quality
* @param v Door quality
*/
void OptionManager::setDoorQuality(int v)
{
	this->doorQuality = v;
}

/**
* Set the door upper quality
* @param v Door upper quality
*/
void OptionManager::setDoorUpperQuality(int v)
{
	this->doorUpperQuality = v;
}

/**
* Set the red gamma
* @param r Red gamma
*/
void OptionManager::setGammaR(int r)
{
	if(r >= 0 && r < 17)
		this->gammaR = r;
	else
		this->gammaR = 5;
}

/**
* Set the green gamma
* @param g Green gamma
*/
void OptionManager::setGammaG(int g)
{
	if(g >= 0 && g < 10)
		this->gammaG = g;
	else
		this->gammaG = 5;
}

/**
* Set the blue gamma
* @param b Blue gamma
*/
void OptionManager::setGammaB(int b)
{
	if(b >= 0 && b < 10)
		this->gammaB = b;
	else
		this->gammaB = 5;
}

/**
* Set the gamma
* @param v Gamma
*/
void OptionManager::setGamma(int v)
{
	if(v >= 0 && v < 16)
	{
		this->gammaR = v;
		this->gammaG = v;
		this->gammaB = v;
		this->gamma = v;
	}
	else
	{
		this->gammaR = 5;
		this->gammaG = 5;
		this->gammaB = 5;
		this->gamma = 5;
	}
}

/**
* Set textures quality
* @param v Texture quality
*/
void OptionManager::setTexturesQuality(int v)
{
	if(v == 0)
		this->texturesQuality = 0;
	else if(v == 1)
		this->texturesQuality = 1;
	else if(v == 2)
		this->texturesQuality = 2;
	else
		this->texturesQuality = 3;
}

/**
* Set the light quality
* @param v Light quality
*/
void OptionManager::setLightQuality(int v)
{
	if(v == 0)
	{
		this->blocQuality = BLOCQUALITY0;
		this->groundQuality = GROUNDQUALITY0;
		this->crateQuality = CRATEQUALITY0;
		this->pillarQuality = PILLARQUALITY0;
		this->smallWallQuality = SMALLWALLQUALITY0;
		this->doorQuality = DOORQUALITY0;
		this->doorUpperQuality = DOORUPPERQUALITY0;
		this->lightQuality = 0;
	}
	else
	{
		if(v == 1)
		{
			this->blocQuality = BLOCQUALITY1;
			this->groundQuality = GROUNDQUALITY1;
			this->crateQuality = CRATEQUALITY1;
			this->pillarQuality = PILLARQUALITY1;
			this->smallWallQuality = SMALLWALLQUALITY1;
			this->doorQuality = DOORQUALITY1;
			this->doorUpperQuality = DOORUPPERQUALITY1;
			this->lightQuality = 1;
		}
		else
		{
			if(v == 2)
			{
				this->blocQuality = BLOCQUALITY2;
				this->groundQuality = GROUNDQUALITY2;
				this->crateQuality = CRATEQUALITY2;
				this->pillarQuality = PILLARQUALITY2;
				this->smallWallQuality = SMALLWALLQUALITY2;
				this->doorQuality = DOORQUALITY2;
				this->doorUpperQuality = DOORUPPERQUALITY2;
				this->lightQuality = 2;
			}
			else
			{
				this->blocQuality = BLOCQUALITY3;
				this->groundQuality = GROUNDQUALITY3;
				this->crateQuality = CRATEQUALITY3;
				this->pillarQuality = PILLARQUALITY3;
				this->smallWallQuality = SMALLWALLQUALITY3;
				this->doorQuality = DOORQUALITY3;
				this->doorUpperQuality = DOORUPPERQUALITY3;
				this->lightQuality = 3;
			}
		}
	}
}

/**
* Set the sound volume
* @param v Volume quality
*/
void OptionManager::setSoundVolume(int v)
{
	if(v >= 0 && v <= 10)
		this->soundVolume = v;
	else
		this->soundVolume = 10;
}

/**
* Set the music volume
* @param v Music quality
*/
void OptionManager::setMusicVolume(int v)
{
	if(v >= 0 && v <= 10)
		this->musicVolume = v;
	else
		this->musicVolume = 10;
}

/**
* Set the impact number value
* @param v Impact number value
*/
void OptionManager::setImpactNumberValue(int v)
{
	if(v >= 0 && v <= 3)
		this->impactNumber = v;
	else
		this->impactNumber = 0;
}

/**
* Set the particle modifier value
* @param v Particle modifier value
*/
void OptionManager::setParticleModifierValue(int v)
{
	if(v >= 0 && v <= 3)
		this->particleModifier = v;
	else
		this->particleModifier = 0;
}
