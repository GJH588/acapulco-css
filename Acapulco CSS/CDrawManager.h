#pragma once
#include "SDK.h"

#define RED(COLORCODE)			((int) ( COLORCODE >> 24) )
#define BLUE(COLORCODE)			((int) ( COLORCODE >> 8 ) & 0xFF )
#define GREEN(COLORCODE)		((int) ( COLORCODE >> 16 ) & 0xFF )
#define ALPHA(COLORCODE)		((int) COLORCODE & 0xFF )

#define COLORCODE(r,g,b,a)		((DWORD) ((((r) & 0xff) << 24) | (((g) & 0xff) << 16) | (((b) & 0xff) << 8) | ((a) & 0xff)))

class CDrawManager
{
public:
	void Initialize();
	void DrawString(HFont font, int x, int y, Color color, const wchar_t * pszText);
	void DrawString(HFont font, int x, int y, Color color, const char * pszText, ...);
	void DrawBox(Vector vOrigin, int r, int g, int b, int alpha, int box_width, int radius );
	void DrawRect( int x, int y, int w, int h, Color dwColor );
	void DrawLine(int x, int y, int x1, int y1, Color dwColor);
	void OutlineRect(int x, int y, int w, int h, Color dwColor);
	bool WorldToScreen(Vector &vOrigin, Vector &vScreen);
	bool ScreenTransform( const Vector &point, Vector &screen);
	void DrawPlayerBox(CBaseEntity * pEnt, Color dwColor);
	void DrawHealthBar(int x, int y, float health, int w, int h, Color color);
	bool GetBonePosition(CBaseEntity* pPlayer, Vector& Hitbox, int Bone);
	void OutlinedBox(int x, int y, int w, int h, Color dwColor);
	byte GetESPHeight();
	byte GetHUDHeight();
private:
	unsigned long m_ESPFont;
	unsigned long m_HUDFont;
};
extern CDrawManager gDrawManager;