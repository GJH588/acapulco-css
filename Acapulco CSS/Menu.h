#pragma once

#include <windows.h>
#include <iostream>
#include <array>
#include <vector>

typedef struct CMenuItems_t
{
	char szTitle[30];
	float* value;
	float flMin;
	float flMax;
	float flStep;
	bool isTab;
	std::vector<std::string> arr;
};

class CMenu
{
public:
	//int add(int index, char szTitle[30], float *value, float flMin, float flMax, float flStep, bool isTab);
	int add(int index, char szTitle[30], float * value, float flMin, float flMax, float flStep, bool isTab, std::vector<std::string> arr);
	void Render();
	void DrawMenu();

	bool bActive;
	int iIndex;
	int iItems;
	CMenuItems_t pMenu[120];
};
extern CMenu gMenu;