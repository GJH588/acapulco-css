#pragma once
#include "SDK.h"

typedef void(__cdecl *MsgFn)(char const* pMsg, va_list);

class Utils
{
private:
	bool bDataCompare(const BYTE*, const BYTE*, const char*);
	char szDirectory[255];

public:
	DWORD FindPattern(DWORD, DWORD, BYTE*, char*);
	uintptr_t FindPatternEx(const char * szModule, const char * szSignature);
	HMODULE GetModuleHandleSafe(const char* pszModuleName);
	DWORD GetClientSignature(char* chPattern, char* chMask);
	DWORD GetEngineSignature(char* chPattern, char* chMask);
	void BaseUponModule(HMODULE hmModule);
	char* GetDirectoryFile(char *szFile);
	
	void __cdecl Msg(char const* msg, ...)
	{
		static MsgFn oMsg = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg");
		char buffer[989];
		va_list list;
		va_start(list, msg);
		vsprintf(buffer, msg, list);
		va_end(list);
		oMsg(buffer, list);
	}
};
extern Utils gUtils;