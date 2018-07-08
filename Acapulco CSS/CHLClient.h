#pragma once
#include "SDK.h"

class CHLClient
{
public:
	ClientClass * GetAllClasses(void)
	{
		typedef ClientClass* (__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 8)(this);
	}
};