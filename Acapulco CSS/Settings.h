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
	BHOP_HUMANIZED
};

namespace Settings
{
	namespace ESP
	{
		extern float Enabled;
		
		extern float Boxes;
		extern BOXES_TYPE BoxesType;
		
		extern float Name;
		extern float Health;
		extern float HealthBar;
		extern float Distance;
		extern float Items;
		extern float DrawTeam;
	}
	
	namespace Bhop
	{
		extern float Enabled;
		extern BHOP_TYPE BhopType;
	}

	namespace AutoStrafe
	{
		extern float Enabled;
	}
}