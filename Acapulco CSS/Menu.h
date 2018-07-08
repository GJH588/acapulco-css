#pragma once

#include <windows.h>

typedef struct CMenuItems_t
{
	char szTitle[30];
	float* value;
	float flMin;
	float flMax;
	float flStep;
	bool isTab;
};

class CMenu
{
public:
	int add(int index, char szTitle[30], float *value, float flMin, float flMax, float flStep, bool isTab);
	void Render(void);
	void DrawMenu(void);

	bool bActive;
	int iIndex;
	int iItems;
	CMenuItems_t pMenu[120];
};
extern CMenu gMenu;