#include "sdk.h"
#include <time.h>

#include <cstdint>
#include <psapi.h>

#define INRANGE(x, a, b) (x >= a && x <= b) 
#define getBits(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (INRANGE(x, '0', '9') ? x - '0': 0))
#define getByte(x) (getBits(x[0]) << 4 | getBits(x[1]))

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

uintptr_t Utils::FindPatternEx(const char* szModule, const char* szSignature)
{
	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
	uintptr_t startAddress = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll);
	uintptr_t endAddress = startAddress + modInfo.SizeOfImage;
	const char* pat = szSignature;
	uintptr_t firstMatch = 0;
	for (uintptr_t pCur = startAddress; pCur < endAddress; pCur++)
	{
		if (!*pat) return firstMatch;
		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch) firstMatch = pCur;
			if (!pat[2]) return firstMatch;
			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
			else pat += 2;
		}
		else
		{
			pat = szSignature;
			firstMatch = 0;
		}
	}

	return NULL;
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