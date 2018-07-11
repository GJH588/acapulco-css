#pragma once
#include "SDK.h"

#define LOCALPLAYER 2137

class CSetupPlayer
{
public:

	inline void Initialize(unsigned int nIndex)
	{
		m_nIndex = nIndex;
	}
	inline CBaseEntity *BaseEnt() const
	{
		return pEntList->GetClientEntity(m_nIndex);
	}

	inline Vector getPunch()
	{
		return *MakePtr(Vector*, BaseEnt(), gPlayerVars.m_vecPunch);
	}
	inline Vector GetPunchAngle()
	{
		return *MakePtr(Vector*, BaseEnt(), gPlayerVars.m_vecPunchAngle);
	}
	inline int GetTeam()
	{
		return *MakePtr(int*, BaseEnt(), gPlayerVars.m_iTeamNum);
	}

	inline byte GetLifeState()
	{
		return *MakePtr(byte*, BaseEnt(), gPlayerVars.m_lifeState);
	}

	inline int GetHealth()
	{
		return *MakePtr(int*, BaseEnt(), gPlayerVars.m_iHealth);
	}

	inline int GetClip()
	{
		return *MakePtr(int*, GetActiveWeapon(), gPlayerVars.m_iClip1);
	}

	inline int GetArmor()
	{
		return *MakePtr(int*, BaseEnt(), gPlayerVars.m_ArmorValue);
	}

	inline CBaseEntity* GetActiveWeapon()
	{
		return pEntList->GetClientEntityFromHandle(*MakePtr(int*, BaseEnt(), gPlayerVars.m_hActiveWeapon));
	}

	inline int GetFlags()
	{
		return *MakePtr(int*, BaseEnt(), gPlayerVars.m_fFlags);
	}

	inline bool IsDefusing()
	{
		if (GetTeam() != 3)
			return false;
		return *MakePtr(bool*, BaseEnt(), gPlayerVars.m_bIsDefusing);
	}
	inline int GetMoney()
	{
		return *MakePtr(int*, BaseEnt(), gPlayerVars.m_iAccount);
	}
private:
	int m_nIndex;
};

class CPlayers
{
public:
	inline CPlayers(void)
	{
		pPlayers = new CSetupPlayer[65];

		for (unsigned int i = 0; i <= 64; i++)
			pPlayers[i].Initialize(i);
	}

	inline ~CPlayers()
	{
		delete[] pPlayers;
	}

	inline CSetupPlayer& operator [] (unsigned int i) const
	{
		if (i == LOCALPLAYER)
			return pPlayers[pEngine->GetLocalPlayer()];

		else if (i > 64 || i <= 0)
			return pPlayers[0];


		return pPlayers[i];
	}

private:
	CSetupPlayer * pPlayers;
};
extern CPlayers gPlayers;