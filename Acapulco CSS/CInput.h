#pragma once
#include "SDK.h"

class CInput
{
public:
	CUserCmd * GetUserCmd(int seq)
	{
		typedef CUserCmd* (__thiscall* OriginalFn)(PVOID, int);
		return getvfunc<OriginalFn>(this, 8)(this, seq);
	}
};