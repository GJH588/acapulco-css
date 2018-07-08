#include <windows.h>
#include "Menu.h"
#include "CDrawManager.h"
#include "CColorManager.h"

CMenu gMenu;

float tabEsp = false;
float tabOther = false;

int CMenu::add(int index, char szTitle[30], float* value, float flMin, float flMax, float flStep, bool isTab)
{
	strcpy(pMenu[index].szTitle, szTitle);
	pMenu[index].value = value;
	pMenu[index].flMin = flMin;
	pMenu[index].flMax = flMax;
	pMenu[index].flStep = flStep;
	pMenu[index].isTab = isTab;
	return(index + 1);
}

void RGB_DWORD(DWORD &dwColor, BYTE r, BYTE g, BYTE b)
{
	dwColor = (r << 24) | (g << 16) | (b << 8);
}

void CMenu::Render(void)
{
	int i = 0;
	{
		i = add(i, "ESP", &tabEsp, 0, 1, 1, true);
		if (tabEsp)
		{
			i = add(i, "  - Boxes", &Settings::ESP::Boxes, 0, 1, 1, false);
			i = add(i, "  - Names", &Settings::ESP::Name, 0, 1, 1, false);
			i = add(i, "  - Draw Team", &Settings::ESP::DrawTeam, 0, 1, 1, false);
			i = add(i, "  - Health", &Settings::ESP::Health, 0, 1, 1, false);
			i = add(i, "  - Health Bar", &Settings::ESP::HealthBar, 0, 1, 1, false);
			i = add(i, "  - Distance", &Settings::ESP::Distance, 0, 1, 1, false);
			i = add(i, "  - Items", &Settings::ESP::Items, 0, 1, 1, false);
		}

		i = add(i, "Other", &tabOther, 0, 1, 1, true);
		if (tabOther)
		{
			i = add(i, "  - Bhop", &Settings::Bhop::Enabled, 0, 1, 1, false);
			i = add(i, "  - Auto Strafe", &Settings::AutoStrafe::Enabled, 0, 1, 1, false);
		}
	}
	iItems = i;
}

void CMenu::DrawMenu(void)
{
	int x = 10,
		xx = x + 100,
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
				gDrawManager.DrawString(true, x + 2, y + (13 * i), gColorManager.GetColor("menu_off"), pMenu[i].szTitle);
				gDrawManager.DrawString(true, xx, y + (13 * i), gColorManager.GetColor("menu_off"), "%0.0f", pMenu[i].value[0]);
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
				gDrawManager.DrawRect(x + 1, y + (13 * i), w - 2, h, COLORCODE(255, 255, 255, 80));
				gDrawManager.DrawString(true, x + 2, y + (13 * i), COLORCODE(255, 0, 255, 255), pMenu[i].szTitle);
				gDrawManager.DrawString(true, xx, y + (13 * i), COLORCODE(255, 0, 255, 255), "%0.0f", pMenu[i].value[0]);
			}
		}
	}
}