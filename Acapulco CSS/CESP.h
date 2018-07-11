#pragma once
#include "SDK.h"
#include "CDrawManager.h"
#include "CColorManager.h"

class CESP
{
public:
	void DrawBone(CBaseEntity * pEntity, int * iBones, int count, DWORD dwCol);
	void DrawPlayers(int i);
	void DrawItems(int i);	
};