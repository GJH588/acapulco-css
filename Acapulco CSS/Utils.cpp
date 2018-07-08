#include "sdk.h"
#include <time.h>

bool Utils::bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;

	return (*szMask) == NULL;
}

DWORD Utils::FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask)
{
	for (DWORD i = NULL; i < dwLen; i++)
		if (bDataCompare((BYTE*)(dwAddress + i), bMask, szMask))
			return (DWORD)(dwAddress + i);

	return 0;
}

HMODULE Utils::GetModuleHandleSafe(const char* pszModuleName)
{
	HMODULE hmModuleHandle = NULL;
	do
	{
		hmModuleHandle = GetModuleHandle(pszModuleName);
		Sleep(1);
	} while (hmModuleHandle == NULL);

	return hmModuleHandle;
}

DWORD Utils::GetClientSignature(char* chPattern, char* chMask)
{
	static DWORD dwClientBase = (DWORD)GetModuleHandleSafe("client.dll");
	return FindPattern(dwClientBase, 0x640000, (PBYTE)chPattern, chMask);
}

DWORD Utils::GetEngineSignature(char* chPattern, char* chMask)
{
	static DWORD dwEngineBase = (DWORD)GetModuleHandleSafe("engine.dll");
	return FindPattern(dwEngineBase, 0x640000, (PBYTE)chPattern, chMask);
}

void Utils::BaseUponModule(HMODULE hmModule)
{
	GetModuleFileName(hmModule, szDirectory, 512);
	for (int i = (int)strlen(szDirectory); i > 0; i--)
	{
		if (szDirectory[i] == '\\')
		{
			szDirectory[i + 1] = 0; break;
		}
	}
}

char* Utils::GetDirectoryFile(char *szFile)
{
	static char path[320];
	strcpy(path, szDirectory);
	strcat(path, szFile);
	return path;
}
Utils gUtils;