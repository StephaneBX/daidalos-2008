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

#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

/**
* Overval variables
* Modify it at your own risk
* @author Stephane Baudoux
*/

#define PLAYERDETECTION 1.0f
#define BLOCDEFAULTHEIGHT 8
#define BLOCDEFAULTSIZE 10
#define BLOCDEFAULTTICKNESS 0.2f
#define PILLARDEFAULTTICKNESS 0.4f
#define PILLARDEFAULTHEIGHT 8
#define INTERBLOCHEIGHT 0.006f
#define INTERBLOCHEIGHT2 0.012f
#define INTERBLOCDEFAULTTICKNESS 0.1f
#define DOORDEFAULTTIKNESS 0.25f
#define DOORDEFAULTHEIGHT 3.0f
#define DOORWAITTIME 6000
#define MORTALSMOKEBLOCHEIGHT 0.05f
#define PIKESHEIGHT 0.5f
#define PIKESPERLINE 8
#define SMALLCEILINGHEIGHT 2.8f
#define TRAPSPROJECTILEACTIVATIONRADIUS 50.0f
#define MINIMUMIMPACTHEIGHT 0.002f
#define SOUNDDISTANCEADDITION 1
#define SOUNDDISTANCEMODIFIER 2
#define SOUNDDISTANCEMODIFIER2 5

#define BLOCQUALITY0 1
#define BLOCQUALITY1 10
#define BLOCQUALITY2 16
#define BLOCQUALITY3 30

#define GROUNDQUALITY0 1
#define GROUNDQUALITY1 5
#define GROUNDQUALITY2 10
#define GROUNDQUALITY3 15

#define CRATEQUALITY0	1
#define CRATEQUALITY1	2
#define CRATEQUALITY2	5
#define CRATEQUALITY3	10

#define PILLARQUALITY0	1
#define PILLARQUALITY1	2
#define PILLARQUALITY2	5
#define PILLARQUALITY3	10

#define SMALLWALLQUALITY0 1
#define SMALLWALLQUALITY1 1
#define SMALLWALLQUALITY2 4
#define SMALLWALLQUALITY3 6

#define DOORQUALITY0 1
#define DOORQUALITY1 2
#define DOORQUALITY2 6
#define DOORQUALITY3 10

#define DOORUPPERQUALITY0 1
#define DOORUPPERQUALITY1 2
#define DOORUPPERQUALITY2 4
#define DOORUPPERQUALITY3 6

#define VIEWQUALITYMIN0 50.0f
#define VIEWQUALITYMIN1 30.0f
#define VIEWQUALITYMIN2 10.0f

#define MAXRISENPARTICLES 400

#define PI 3.14159265f

enum 
{
	MODE_MAINMENU,
	MODE_GAMEMENU,
	MODE_INGAME,
	MODE_CHANGELEVEL
};

enum 
{
	OBJECTTYPE_IMMOVABLE,
	OBJECTTYPE_HEALTH,
	OBJECTTYPE_AMMO,
	OBJECTTYPE_ARMOR,
	OBJECTTYPE_WEAPON,
	OBJECTTYPE_GATE,
	OBJECTTYPE_GATE2,
	OBJECTTYPE_DOOR_VERTICAL,
	OBJECTTYPE_DOOR_HORIZONTAL,
	OBJECTTYPE_LOCKEDDOOR_VERTICAL,
	OBJECTTYPE_LOCKEDDOOR_HORIZONTAL,
	OBJECTTYPE_FLASHLIGHT,
	OBJECTTYPE_KEY_YELLOW,
	OBJECTTYPE_KEY_RED,
	OBJECTTYPE_KEY_BLUE,
	OBJECTTYPE_KEY_GREEN,
	OBJECTTYPE_TRAP,
	OBJECTTYPE_IMMOVABLE_TRAP
};

enum
{
	IMPACT_REPLACEMENT_POLICY_REPLACE,
	IMPACT_REPLACEMENT_POLICY_CREATE,
	IMPACT_REPLACEMENT_MINUS_X,
	IMPACT_REPLACEMENT_X,
	IMPACT_REPLACEMENT_MINUS_Y,
	IMPACT_REPLACEMENT_Y,
	IMPACT_REPLACEMENT_MINUS_Z,
	IMPACT_REPLACEMENT_Z
};

#endif
