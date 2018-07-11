#include <windows.h>
#include "Menu.h"
#include "CDrawManager.h"
#include "CColorManager.h"

CMenu gMenu;

float tabEsp = false;
float tabOther = false;
float tabOptions = false;

int CMenu::add(int index, char szTitle[30], float* value, float flMin, float flMax, float flStep, bool isTab, std::vector<std::string> arr)
{
	strcpy(pMenu[index].szTitle, szTitle);
	pMenu[index].value = value;
	pMenu[index].flMin = flMin;
	pMenu[index].flMax = flMax;
	pMenu[index].flStep = flStep;
	pMenu[index].isTab = isTab;
	pMenu[index].arr = arr;
	return(index + 1);
}

void RGB_DWORD(DWORD &dwColor, BYTE r, BYTE g, BYTE b)
{
	dwColor = (r << 24) | (g << 16) | (b << 8);
}

void CMenu::Render()
{
	int i = 0;
	{
		i = add(i, "ESP", &tabEsp, 0, 1, 1, true, {});
		if (tabEsp)
		{
			i = add(i, "  - Boxes", &Settings::ESP::Boxes, 0, 1, 1, false, {});
			i = add(i, "  - Names", &Settings::ESP::Name, 0, 1, 1, false, {});
			i = add(i, "  - Draw Team", &Settings::ESP::DrawTeam, 0, 1, 1, false, {});
			i = add(i, "  - Health", &Settings::ESP::Health, 0, 1, 1, false, {});
			i = add(i, "  - Health Bar", &Settings::ESP::HealthBar, 0, 1, 1, false, {});
			i = add(i, "  - Armor", &Settings::ESP::Armor, 0, 1, 1, false, {});
			i = add(i, "  - Bones", &Settings::ESP::Bones, 0, 1, 1, false, {});
			i = add(i, "  - Head Dot", &Settings::ESP::HeadDot, 0, 1, 1, false, {});
			i = add(i, "  - Distance", &Settings::ESP::Distance, 0, 1, 1, false, {});
			i = add(i, "  - Items", &Settings::ESP::Items, 0, 1, 1, false, {});
		}

		i = add(i, "Other", &tabOther, 0, 1, 1, true, {});
		if (tabOther)
		{
			i = add(i, "  - Bhop", &Settings::Bhop::Enabled, 0, 1, 1, false, {});
			i = add(i, "  - Auto Strafe", &Settings::AutoStrafe::Enabled, 0, 1, 1, false, {});
			i = add(i, "  - Anti SS", &Settings::Misc::AntiSS, 0, 1, 1, false, {});
		}

		i = add(i, "Options", &tabOptions, 0, 1, 1, true, {});
		if (tabOptions)
		{
			i = add(i, "  - Bhop", (float*)&Settings::Bhop::BhopType, 0, 1, 1, false, { "Default", "SMAC" });
			i = add(i, "  - Boxes", (float*)&Settings::ESP::BoxesType, 0, 1, 1, false, { "Default", "Outline" });
		}
	}
	iItems = i;
}

void CMenu::DrawMenu()
{
	int x = 10,
		xx = x + 75,
		y = 300,
		w = 120,
		h = 13;

	gDrawManager.DrawRect(x, y - (h + 4), w, iItems * 13 + 21, COLORCODE(0, 0, 0, 200));
	gDrawManager.OutlineRect(x, y - (h + 4), w, iItems * 13 + 21, COLORCODE(255, 0, 255, 255));

	gDrawManager.DrawString(true, x + 10, y - 15, COLORCODE(255, 0, 255, 255), "Acapulco CSS");

	for (int i = 0; i < iItems; i++)
	{
		if (i != iIndex)
		{
			if (pMenu[i].isTab)
			{
				if (pMenu[i].value[0])
				{
					gDrawManager.DrawString(true, x + 2, y + (13 * i), COLORCODE(255, 0, 255, 255), " [-] %s", pMenu[i].szTitle);
				}
				else
				{
					gDrawManager.DrawString(true, x + 2, y + (13 * i), COLORCODE(255, 0, 255, 255), " [+] %s", pMenu[i].szTitle);
				}
			}
			else
			{
				
				if (pMenu[i].arr.empty())
				{
					gDrawManager.DrawString(true, xx, y + (13 * i), COLORCODE(255, 255, 255, 255), "%0.0f", pMenu[i].value[0]);
					gDrawManager.DrawString(true, x + 2, y + (13 * i), COLORCODE(255, 255, 255, 255), pMenu[i].szTitle);
				}
				else
				{
					pMenu[i].arr[pMenu[i].value[0]] == pMenu[i].arr[-1] ? pMenu[i].arr[0] : pMenu[i].arr[pMenu[i].value[0]];
					gDrawManager.DrawString(true, x + 2, y + (13 * i), COLORCODE(255, 255, 255, 255), pMenu[i].szTitle);
					gDrawManager.DrawString(true, xx, y + (13 * i), COLORCODE(255, 255, 255, 255), "%s", pMenu[i].arr[pMenu[i].value[0]].c_str());
				}
			}
		}
		else
		{
			if (pMenu[i].isTab)
			{
				gDrawManager.DrawRect(x + 1, y + (13 * i), w - 2, h, COLORCODE(255, 255, 255, 80));
				gDrawManager.DrawString(true, x + 2, y + (13 * i), COLORCODE(255, 0, 255, 255), " [+] %s", pMenu[i].szTitle);
				if (pMenu[i].value[0])
					gDrawManager.DrawString(true, x + 2, y + (13 * i), COLORCODE(255, 0, 255, 255), " [-] %s", pMenu[i].szTitle);
			}
			else
			{
				if (pMenu[i].arr.empty())
				{
					gDrawManager.DrawRect(x + 1, y + (13 * i), w - 2, h, COLORCODE(255, 255, 255, 80));
					gDrawManager.DrawString(true, xx, y + (13 * i), COLORCODE(255, 255, 255, 255), "%0.0f", pMenu[i].value[0]);
					gDrawManager.DrawString(true, x + 2, y + (13 * i), COLORCODE(255, 255, 255, 255), pMenu[i].szTitle);
				}
				else
				{
					gDrawManager.DrawRect(x + 1, y + (13 * i), w - 2, h, COLORCODE(255, 255, 255, 80));
					pMenu[i].arr[pMenu[i].value[0]] == pMenu[i].arr[-1] ? pMenu[i].arr[0] : pMenu[i].arr[pMenu[i].value[0]];
					gDrawManager.DrawString(true, x + 2, y + (13 * i), COLORCODE(255, 255, 255, 255), pMenu[i].szTitle);
					gDrawManager.DrawString(true, xx, y + (13 * i), COLORCODE(255, 255, 255, 255), "%s", pMenu[i].arr[pMenu[i].value[0]].c_str());
				}
			}
		}
	}
}