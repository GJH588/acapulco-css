#pragma once
#include "SDK.h"

class CPlayerVariables
{
public:
	void FindOffsets();
	void DumpTable(RecvTable *pTable,FILE* fp);
	void DumpOffset(char* file);
	int getOffset(char *szClassName, char *szVariable);
	
	int m_lifeState;
	int m_iHealth;
	int m_iTeamNum;
	int m_fFlags;
	int m_hActiveWeapon;
	int m_hOwnerEntity;

	int m_ArmorValue;
	int m_bIsDefusing;
	int m_iClip1;
	int m_iAccount;

	int m_vecPunch;
};
extern CPlayerVariables gPlayerVars;