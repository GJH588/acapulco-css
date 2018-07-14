#pragma once
#include "SDK.h"

enum BOXES_TYPE
{
	BOXES_DEFAULT,
	BOXES_OUTLINE
};

enum BHOP_TYPE
{
	BHOP_DEFAULT,
	BHOP_SMAC
};

namespace Settings
{
	namespace Menu
	{
		extern bool Enabled;
		extern int Tab;
	}
	namespace ESP
	{
		extern bool Enabled;
		
		extern bool Boxes;
		extern BOXES_TYPE BoxesType;
		
		extern bool Name;
		extern bool Health;
		extern bool Armor;
		extern bool HealthBar;
		extern bool Distance;
		extern bool Bones;
		extern bool HeadDot;
		extern bool Items;
		extern bool DrawTeam;
	}
	
	namespace Bhop
	{
		extern bool Enabled;
		extern BHOP_TYPE BhopType;
	}

	namespace AutoStrafe
	{
		extern bool Enabled;
	}

	namespace Misc
	{
		extern bool AntiSS;
		extern bool Recorder;
	}
}