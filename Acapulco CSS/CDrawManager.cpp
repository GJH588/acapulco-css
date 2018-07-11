#include "CDrawManager.h"
#include "CGlobalVars.h"
#include "Hooks.h"

CDrawManager gDrawManager;

#define ESP_HEIGHT 11
#define HUD_HEIGHT 13

void CDrawManager::Initialize()
{
	if (!pSurface)
		return;

	pEngine->GetScreenSize(gScreen.iWidth, gScreen.iHeight);

	m_ESPFont = pSurface->CreateFont();
	m_HUDFont = pSurface->CreateFont();
	
	pSurface->SetFontGlyphSet(m_ESPFont, "Tahoma", ESP_HEIGHT, 300, 0, 0, 0x080);
	pSurface->SetFontGlyphSet(m_HUDFont, "Tahoma", HUD_HEIGHT, 500, 0, 0, 0x200);
}

void CDrawManager::DrawString(bool HUDFont, int x, int y, DWORD dwColor, const wchar_t *pszText)
{
	if (!pszText)
		return;

	pSurface->DrawSetTextPos(x, y);
	pSurface->DrawSetTextFont(HUDFont ? m_HUDFont : m_ESPFont);
	pSurface->DrawSetTextColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	pSurface->DrawPrintText(pszText, wcslen(pszText));
}

void CDrawManager::DrawString(bool HUDFont, int x, int y, DWORD dwColor, const char *pszText, ...)
{
	if (!pszText)
		return;

	va_list va_alist;
	char szBuffer[1024] = { '\0' };
	wchar_t szString[1024] = { '\0' };

	va_start(va_alist, pszText);
	vsprintf_s(szBuffer, pszText, va_alist);
	va_end(va_alist);

	wsprintfW(szString, L"%S", szBuffer);

	pSurface->DrawSetTextPos(x, y);
	pSurface->DrawSetTextFont(HUDFont ? m_HUDFont : m_ESPFont);
	pSurface->DrawSetTextColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	pSurface->DrawPrintText(szString, wcslen(szString));
}

byte CDrawManager::GetESPHeight()
{
	return ESP_HEIGHT;
}

byte CDrawManager::GetHUDHeight()
{
	return HUD_HEIGHT;
}

void CDrawManager::DrawRect(int x, int y, int w, int h, DWORD dwColor)
{
	pSurface->DrawSetColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	pSurface->DrawFilledRect(x, y, x + w, y + h);
}

void CDrawManager::DrawLine(int x, int y, int x1, int y1, DWORD dwColor)
{
	pSurface->DrawSetColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	pSurface->DrawLine(x, y, x1, y1);
}

void CDrawManager::OutlineRect(int x, int y, int w, int h, DWORD dwColor)
{
	pSurface->DrawSetColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	pSurface->DrawOutlinedRect(x, y, x + w, y + h);
}

void CDrawManager::OutlinedBox(int x, int y, int w, int h, DWORD dwColor)
{
	OutlineRect(x - w, y, w * 2, h, dwColor);
	OutlineRect(x - w - 1, y - 1, w * 2 + 2, h + 2, COLORCODE(0, 0, 0, 255));
	OutlineRect(x - w + 1, y + 1, w * 2 - 2, h - 2, COLORCODE(0, 0, 0, 255));
}

void CDrawManager::DrawBox(Vector vOrigin, int r, int g, int b, int alpha, int box_width, int radius)
{
	Vector vScreen;

	if (!WorldToScreen(vOrigin, vScreen))
		return;

	int radius2 = radius << 1;

	OutlineRect(vScreen.x - radius + box_width, vScreen.y - radius + box_width, radius2 - box_width, radius2 - box_width, 0x000000FF);
	OutlineRect(vScreen.x - radius - 1, vScreen.y - radius - 1, radius2 + (box_width + 2), radius2 + (box_width + 2), 0x000000FF);
	DrawRect(vScreen.x - radius + box_width, vScreen.y - radius, radius2 - box_width, box_width, COLORCODE(r, g, b, alpha));
	DrawRect(vScreen.x - radius, vScreen.y + radius, radius2, box_width, COLORCODE(r, g, b, alpha));
	DrawRect(vScreen.x - radius, vScreen.y - radius, box_width, radius2, COLORCODE(r, g, b, alpha));
	DrawRect(vScreen.x + radius, vScreen.y - radius, box_width, radius2 + box_width, COLORCODE(r, g, b, alpha));
}

bool CDrawManager::WorldToScreen(Vector &vOrigin, Vector &vScreen)
{
	const matrix3x4& worldToScreen = pEngine->WorldToScreenMatrix();

	float w = worldToScreen[3][0] * vOrigin[0] + worldToScreen[3][1] * vOrigin[1] + worldToScreen[3][2] * vOrigin[2] + worldToScreen[3][3];
	vScreen.z = 0;

	if (w > 0.001)
	{
		float fl1DBw = 1 / w;
		vScreen.x = (gScreen.iWidth / 2) + (0.5 * ((worldToScreen[0][0] * vOrigin[0] + worldToScreen[0][1] * vOrigin[1] + worldToScreen[0][2] * vOrigin[2] + worldToScreen[0][3]) * fl1DBw) * gScreen.iWidth + 0.5);
		vScreen.y = (gScreen.iHeight / 2) - (0.5 * ((worldToScreen[1][0] * vOrigin[0] + worldToScreen[1][1] * vOrigin[1] + worldToScreen[1][2] * vOrigin[2] + worldToScreen[1][3]) * fl1DBw) * gScreen.iHeight + 0.5);
		return true;
	}

	return false;
}

bool CDrawManager::ScreenTransform(const Vector &point, Vector &screen)
{
	float w;
	const matrix3x4 &worldToScreen = pEngine->WorldToScreenMatrix();
	screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
	w = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];
	screen.z = 0.0f;

	bool behind = false;

	if (w < 0.001f)
	{
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else
	{
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}
	return behind;
}

void CDrawManager::DrawPlayerBox(CBaseEntity *pEnt, DWORD dwColor)
{
	if (!pEnt)
		return;

	Vector mon, nom;

	nom = pEnt->GetAbsOrigin();

	if (gPlayers[pEnt->GetIndex()].GetFlags() & FL_DUCKING)
	{
		mon = nom + Vector(0, 0, 50.f);
	}
	else
	{
		mon = nom + Vector(0, 0, 70.f);
	}

	Vector bot, top;

	if (WorldToScreen(nom, bot) && WorldToScreen(mon, top))
	{
		float h = (bot.y - top.y);

		float w = h / 4.f;

		OutlineRect(top.x - w, top.y, w * 2, h, dwColor);
	}

}

void CDrawManager::DrawHealthBar(int x, int y, float health, int w, int h, DWORD color)
{
	float red = 255 - (health * 2.55);
	float green = health * 2.55;
	x -= w / 2;
	y -= h / 2;
	DrawRect(x, y, w, h + 1, COLORCODE(20, 20, 20, 255));
	UINT hw = (UINT)(((w - 2) * health) / 100);
	DrawRect(x + 1, y + 1, hw, h - 1, COLORCODE((int)red, (int)green, 0, 255));
}

bool CDrawManager::GetBonePosition(CBaseEntity* pPlayer, Vector& Hitbox, int Bone)
{
	matrix3x4 MatrixArray[128];

	if (!pPlayer->SetupBones(MatrixArray, 128, 0x00000100, pEngine->Time()))
		return FALSE;

	Hitbox = Vector(MatrixArray[Bone][0][3], MatrixArray[Bone][1][3], MatrixArray[Bone][2][3]);

	return true;
}