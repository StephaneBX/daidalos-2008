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

#ifndef __OPTIONMANAGER_H
#define __OPTIONMANAGER_H

#include <GL/gl.h>
#include "../includes/singleton.h"
#include "../manager/logManager.h"

#define GLOBALVISIBILITYMODIFIER 15
#define MOUSESENSIBILITYMODIFIER 0.2f

/**
* OptionManager Class
* Handle game's options
* @author Stephane Baudoux
*/
class OptionManager : public CSingleton<OptionManager>
{
	friend class CSingleton<OptionManager>;

private:
	// language
	int language;

	// Debug options
	bool noClip;
	bool log;
	bool debugModeActivated;

	// In-game menu options
	int resolution;
	int resolutionXTable[9];
	int resolutionYTable[9];
	int mouseSensibility;
	int invertYMouseAxis;
	int mouseSmoothing;
	int gamma;
	int soundVolume;
	int musicVolume;
	int antialiasingLevel;
	int textureFilteringLevel;
	int texturesQuality;
	int lightQuality;
	int globalVisibility;
	int impactNumber;
	int particleModifier;
	bool isFullScreenEnabled;
	bool isWideScreenEnabled;
	bool isFogEnabled;

	// In-Game options
	int fogMode;
	int fogQuality;
	int blocQuality;
	int groundQuality;
	int crateQuality;
	int pillarQuality;
	int smallWallQuality;
	int doorQuality;
	int doorUpperQuality;
	int gammaR;
	int gammaG;
	int gammaB;

	LogManager * logM;

public:
	void save();
	bool isNoClipActivated();
	bool isLogActivated();
	bool isFullScreen();
	bool isWideScreen();
	bool isFog();
	bool isDebug();
	GLint getFogMode();
	GLenum getFogQuality();
	void getResolution(int &resX, int &resY);
	int getResolutionHeight(int resNumber);
	int getResolutionWidth(int resNumber);
	int getResolutionHeight();
	int getResolutionWidth();
	int getResolutionNumber();
	int getGlobalVisibility();
	int getMouseSensibility();
	int getInvertYMouseAxis();
	int getMouseSmoothing();
	int getLanguage();
	int getAntialiasingLevel();
	int getTextureFilteringLevel();
	int getBlocQuality();
	int getBlocQuality(int v);
	int getGroundQuality();
	int getGroundQuality(int v);
	int getCrateQuality();
	int getCrateQuality(int v);
	int getPillarQuality();
	int getPillarQuality(int v);
	int getSmallWallQuality();
	int getSmallWallQuality(int v);
	int getDoorQuality();
	int getDoorQuality(int v);
	int getDoorUpperQuality();
	int getDoorUpperQuality(int v);
	int getGammaR();
	int getGammaG();
	int getGammaB();
	int getGamma();
	int getTexturesQuality();
	int getLightQuality();
	int getSoundVolume();
	int getMusicVolume();
	int getImpactNumber();
	int getImpactNumberValue();
	int getParticleModifier();
	int getParticleModifierValue();

	void setLanguage(int v);
	void setNoClip(bool v);
	void setFullScreen(int v);
	void setWideScreen(int v);
	void setFog(int v);
	void setFogMode(int v);
	void setFogQuality(int v);
	void setResolution(int v);
	void setGlobalVisibility(int v);
	void setMouseSensibility(int v);
	void setInvertYMouseAxis(int v);
	void setMouseSmoothing(int v);
	void setAntialiasingLevel(int v);
	void setTextureFilteringLevel(int v);
	void setBlocQuality(int v);
	void setGroundQuality(int v);
	void setCrateQuality(int v);
	void setPillarQuality(int v);
	void setSmallWallQuality(int v);
	void setDoorQuality(int v);
	void setDoorUpperQuality(int v);
	void setGammaR(int r);
	void setGammaG(int g);
	void setGammaB(int b);
	void setGamma(int v);
	void setTexturesQuality(int v);
	void setLightQuality(int v);
	void setSoundVolume(int v);
	void setMusicVolume(int v);
	void setImpactNumberValue(int v);
	void setParticleModifierValue(int v);

private:
	OptionManager();
};

#endif
