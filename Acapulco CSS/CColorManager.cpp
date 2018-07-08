#include "CColorManager.h"
#include "SDK.h"

CColorManager gColorManager;

void CColorManager::Initialize(void)
{
	AddColor("objectives", COLORCODE(255, 255, 255, 255));
	AddColor("teamone", COLORCODE(153, 194, 216, 255));
	AddColor("teamtwo", COLORCODE(255, 0, 0, 255));
	AddColor("teamthree", COLORCODE(0, 200, 255, 255));
	AddColor("teamfour", COLORCODE(255, 128, 0, 255));
}

DWORD CColorManager::GetColor(int iIndex)
{
	if (iIndex < 0 || (unsigned int)iIndex > m_Colors.size())
		return 0xFFFFFFFF;

	return m_Colors[iIndex].dwColor;
}

DWORD CColorManager::GetColor(const char* pszName)
{
	for (auto item = m_Colors.begin(); item != m_Colors.end(); item++)
		if (streql(item->strName.c_str(), pszName))
			return item->dwColor;
	return 0xFFFFFFFF;
}

void CColorManager::AddColor(const char* pszName, DWORD dwNewColor)
{
	color_t color;
	color.dwColor = dwNewColor;
	color.strName = pszName;

	m_Colors.push_back(color);
}