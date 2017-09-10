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
#include "../includes/global_var.h"
#include "../manager/interfaceManager.h"

/**
* Constructor
*/
InterfaceManager::InterfaceManager()
{
	this->weaponM = WeaponManager::getInstance();
	this->textM = TextManager::getInstance();
	this->texturesManager = CTextureManager::getInstance();
	this->modM = ModManager::getInstance();
	this->debug = OptionManager::getInstance()->isDebug();
	this->healthLogo		= new Logo("imgs/interface/health.tga"		,this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_HEALTH_LOGO_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_HEALTH_LOGO_Y), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_HEALTH_LOGO_SIZE_X), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_HEALTH_LOGO_SIZE_Y));
	this->armorLogo			= new Logo("imgs/interface/armor.tga"		,this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_ARMOR_LOGO_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_ARMOR_LOGO_Y), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_ARMOR_LOGO_SIZE_X), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_ARMOR_LOGO_SIZE_Y));
	this->ammoLogo			= new Logo("imgs/interface/ammo.tga"		,this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_AMMO_LOGO_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_AMMO_LOGO_Y), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_AMMO_LOGO_SIZE_X), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_AMMO_LOGO_SIZE_Y));
	this->yellowKeyLogo		= new Logo("imgs/interface/key_yellow.tga"	,this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_Y), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_SIZE_X), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_YELLOWKEY_LOGO_SIZE_Y));
	this->blueKeyLogo		= new Logo("imgs/interface/key_blue.tga"	,this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_BLUEKEY_LOGO_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_BLUEKEY_LOGO_Y), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_BLUEKEY_LOGO_SIZE_X), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_BLUEKEY_LOGO_SIZE_Y));
	this->redKeyLogo		= new Logo("imgs/interface/key_red.tga"		,this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_REDKEY_LOGO_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_REDKEY_LOGO_Y), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_REDKEY_LOGO_SIZE_X), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_REDKEY_LOGO_SIZE_Y));
	this->greenKeyLogo		= new Logo("imgs/interface/key_green.tga"	,this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_GREENKEY_LOGO_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_GREENKEY_LOGO_Y), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_GREENKEY_LOGO_SIZE_X), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_GREENKEY_LOGO_SIZE_Y));
	this->flashlightLogo	= new Logo("imgs/interface/flashlight.tga"	,this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_Y), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_SIZE_X), this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_FLASHLIGHT_LOGO_SIZE_Y));
	this->cursor1			= new Cursor();
	this->blood1			= new Blood();
	this->hud1				= new Hud();
}

/**
* Destructor
*/
InterfaceManager::~InterfaceManager()
{
	delete this->healthLogo;
	this->healthLogo = NULL;
	delete this->armorLogo;
	this->armorLogo = NULL;
	delete this->ammoLogo;
	this->ammoLogo = NULL;
	delete this->yellowKeyLogo;
	this->yellowKeyLogo = NULL;
	delete this->blueKeyLogo;
	this->blueKeyLogo = NULL;
	delete this->redKeyLogo;
	this->redKeyLogo = NULL;
	delete this->greenKeyLogo;
	this->greenKeyLogo = NULL;
	delete this->flashlightLogo;
	this->flashlightLogo = NULL;
	delete this->cursor1;
	this->cursor1 = NULL;
	delete this->hud1;
	this->hud1 = NULL;
	delete this->blood1;
	this->blood1 = NULL;
}

/**
* Refresh the Interface Manager
* @param pl Pointer to the Player
*/
void InterfaceManager::refresh(Player * pl)
{
	if(this->weaponM->getHasWeapon())
		this->cursor1->refresh(pl,this->weaponM);

	if(pl->getDeathTimer() > 0)
	{
		float alpha = pl->getDeathTimer() / (float)(PLAYERDEATHTIMER / 1.5f);
		if(alpha > 0.6f)
			alpha = 0.6f;
		this->blood1->refresh(alpha);
	}
	else
	{
		if(pl->getDamageTimer() > (PLAYERDAMAGETIMER * 3.0) / 4)
		{
			this->blood1->refresh(0.4f - (((float)(pl->getDamageTimer() - (float)((PLAYERDAMAGETIMER * 3.0f) / 4.0f)) / (float)(PLAYERDAMAGETIMER / 4.0f)) * 0.4f));
		}
		else
		{
			if(pl->getDamageTimer() < PLAYERDAMAGETIMER / 4)
				this->blood1->refresh((float)(pl->getDamageTimer() / (float)(PLAYERDAMAGETIMER / 4.0f)) * 0.4f);
			else
				this->blood1->refresh(0.4f);
		}
	}
}

/**
* Draw the Interface
* @param pl Pointer to the Player
*/
void InterfaceManager::drawPlayerInterface(Player * pl)
{
	this->hud1->draw();

	if(this->weaponM->getHasWeapon())
		this->cursor1->draw();

	std::ostringstream ammo, health, armor;

	// Health elements
	health << pl->getHealth() << "%";
	if(pl->getHealth() < 30)
		this->textM->drawText(health.str(),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_HEALTH_TEXT_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_HEALTH_TEXT_Y),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_HEALTH_TEXT_SIZE),TEXTMANAGER_ALIGN_RIGHT,1.0f,0.0f,0.0f);
	else if(pl->getHealth() >= 90)
		this->textM->drawText(health.str(),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_HEALTH_TEXT_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_HEALTH_TEXT_Y),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_HEALTH_TEXT_SIZE),TEXTMANAGER_ALIGN_RIGHT,0.0f,1.0f,0.0f);
	else this->textM->drawText(health.str(),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_HEALTH_TEXT_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_HEALTH_TEXT_Y),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_HEALTH_TEXT_SIZE),TEXTMANAGER_ALIGN_RIGHT,1.0f,1.0f,1.0f);

	// Armor elements
	armor << pl->getArmor() << "%";
	if(pl->getArmor() < 30)
		this->textM->drawText(armor.str(),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_ARMOR_TEXT_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_ARMOR_TEXT_Y),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_ARMOR_TEXT_SIZE),TEXTMANAGER_ALIGN_RIGHT,1.0f,0.0f,0.0f);
	else if(pl->getArmor() >= 90)
		this->textM->drawText(armor.str(),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_ARMOR_TEXT_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_ARMOR_TEXT_Y),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_ARMOR_TEXT_SIZE),TEXTMANAGER_ALIGN_RIGHT,0.0f,1.0f,0.0f);
	else this->textM->drawText(armor.str(),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_ARMOR_TEXT_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_ARMOR_TEXT_Y),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_ARMOR_TEXT_SIZE),TEXTMANAGER_ALIGN_RIGHT,1.0f,1.0f,1.0f);

	// Ammo elements
	if(this->weaponM->getHasWeapon())
	{
		ammo <<  this->weaponM->getSelectedWeapon()->getLoaderAmount() << "/" << this->weaponM->getSelectedWeapon()->getAmmo();
		if(this->weaponM->getSelectedWeapon()->getAmmo() < 10 || this->weaponM->getSelectedWeapon()->getLoaderAmount() <= (this->weaponM->getSelectedWeapon()->getLoaderCapacity() / 4))
			this->textM->drawText(ammo.str(),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_AMMO_TEXT_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_AMMO_TEXT_Y),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_AMMO_TEXT_SIZE),TEXTMANAGER_ALIGN_RIGHT,1.0f,0.0f,0.0f);
		else
			this->textM->drawText(ammo.str(),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_AMMO_TEXT_X),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_AMMO_TEXT_Y),this->modM->getInterfacePosition(MOD_INTERFACE_POSITION_AMMO_TEXT_SIZE),TEXTMANAGER_ALIGN_RIGHT,1.0f,1.0f,1.0f);
	}

	this->healthLogo->draw();
	this->armorLogo->draw();
	if(this->weaponM->getHasWeapon())
		this->ammoLogo->draw();

	if(pl->hasKey(OBJECTTYPE_KEY_YELLOW))
		this->yellowKeyLogo->draw();

	if(pl->hasKey(OBJECTTYPE_KEY_BLUE))
		this->blueKeyLogo->draw();

	if(pl->hasKey(OBJECTTYPE_KEY_RED))
		this->redKeyLogo->draw();

	if(pl->hasKey(OBJECTTYPE_KEY_GREEN))
		this->greenKeyLogo->draw();

	if(pl->hasFlashlight())
		this->flashlightLogo->draw();

		this->blood1->draw();
}
