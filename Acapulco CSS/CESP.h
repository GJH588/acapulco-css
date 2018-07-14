#pragma once
#include "SDK.h"
#include "CDrawManager.h"

class CESP
{
public:
	void DrawPlayers(int i);
	void DrawItems(int i);

private:
	void DrawBone(CBaseEntity * pEntity, int * iBones, int count, Color dwCol);
};