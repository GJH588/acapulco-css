#include "CESP.h"

void CESP::DrawPlayers(int i)
{
	player_info_s pInfo;

	CSetupPlayer _pLocalBaseEntity = gPlayers[me];
	CBaseEntity* pLocalBaseEntity = _pLocalBaseEntity.BaseEnt();

	if (i == pLocalBaseEntity->GetIndex())
		return;

	CSetupPlayer _pBaseEntity = gPlayers[i];
	CBaseEntity* pBaseEntity = _pBaseEntity.BaseEnt();

	Vector vHead;
	Vector vScreenHead;
	Vector vScreenLeg;
	Vector vScreen;
	Vector vWorldPos;

	if (!pBaseEntity)
		return;

	if (!Settings::ESP::DrawTeam && _pLocalBaseEntity.GetTeam() == _pBaseEntity.GetTeam())
		return;

	if (!gDrawManager.GetBonePosition(pBaseEntity, vHead, 10))
		return;

	if (!_pBaseEntity.GetLifeState() == LIFE_ALIVE)
		return;

	if (pBaseEntity->IsDormant())
		return;

	if (!pEngine->GetPlayerInfo(i, &pInfo))
		return;

	int iTeam = _pBaseEntity.GetTeam();
	DWORD dwTeamColor = gColorManager.GetColor(iTeam);

	Vector vOrigin = pBaseEntity->GetAbsOrigin();

	vHead += 25;

	if (gDrawManager.WorldToScreen(vHead, vScreenHead) && gDrawManager.WorldToScreen(vOrigin, vScreen))
	{
		float Height = abs((vScreenHead.y - 10) - vScreen.y);
		float Width = Height * 0.45f;

		float Distance = flGetDistance(pBaseEntity->GetAbsOrigin(), pLocalBaseEntity->GetAbsOrigin());
		float iRadius = 450.0 / Distance;

		if (Settings::ESP::Boxes)
		{
			gDrawManager.OutlineRect(vScreen.x - Width / 2, vScreenHead.y, Width, Height, dwTeamColor);
			gDrawManager.OutlineRect(vScreen.x - Width / 2 - 1, vScreenHead.y - 1, Width + 2, Height + 2, COLORCODE(0, 0, 0, 255));
			gDrawManager.OutlineRect(vScreen.x - Width / 2 + 1, vScreenHead.y + 1, Width - 2, Height - 2, COLORCODE(0, 0, 0, 255));
		}
		if (Settings::ESP::Name)
		{
			gDrawManager.DrawString(false, vScreen.x + (Width / 2) + 2, vScreen.y - Height + 6, COLORCODE(255, 255, 255, 255), "%s", pInfo.name);
			vScreen.y += gDrawManager.GetESPHeight();
		}
		if (Settings::ESP::Distance)
		{
			gDrawManager.DrawString(false, vScreen.x + (Width / 2) + 2, vScreen.y - Height + 6, COLORCODE(255, 255, 255, 255), "[%.0fm]", Distance);
			vScreen.y += gDrawManager.GetESPHeight();
		}
		if (Settings::ESP::Health)
		{
			gDrawManager.DrawString(false, vScreen.x + (Width / 2) + 2, vScreen.y - Height + 6, COLORCODE(255, 255, 255, 255), "%i HP", _pBaseEntity.GetHealth());
			vScreen.y += gDrawManager.GetESPHeight();
		}
		if (Settings::ESP::HealthBar)
		{
			gDrawManager.DrawHealthBar(vScreen.x, vScreenHead.y + Height + 1, _pBaseEntity.GetHealth(), Width + 5, 2, COLORCODE(0, 255, 0, 255));
		}
	}
}

void CESP::DrawItems(int i)
{
	if (!Settings::ESP::Items)
		return;

	CBaseEntity* pLocalBaseEntity = gPlayers[me].BaseEnt();
	CBaseEntity* pBaseEntity = pEntList->GetClientEntity(i);

	if (!pBaseEntity)
		return;

	if (!pBaseEntity->IsDormant())
	{
		Vector vScreen;
		int iTeamNum;

		if (!gDrawManager.WorldToScreen(pBaseEntity->GetAbsOrigin(), vScreen))
			return;

		DWORD dwTeamColor = gColorManager.GetColor(*MakePtr(int*, pBaseEntity, gPlayerVars.m_iTeamNum));
		if (!(pEntList->GetClientEntityFromHandle(*MakePtr(int*, pBaseEntity, gPlayerVars.m_hOwnerEntity))))
		{
			if (strstr(pBaseEntity->GetClientClass()->GetName(), "CWeapon"))
				gDrawManager.DrawString(false, vScreen.x, vScreen.y, gColorManager.GetColor("null"), "%s", string(pBaseEntity->GetClientClass()->GetName()).substr(7).c_str());
			if (streql(pBaseEntity->GetClientClass()->GetName(), "CAK47"))
				gDrawManager.DrawString(false, vScreen.x, vScreen.y, gColorManager.GetColor("null"), "AK47");
			if (streql(pBaseEntity->GetClientClass()->GetName(), "CC4"))
				gDrawManager.DrawString(false, vScreen.x, vScreen.y, gColorManager.GetColor("null"), "C4");
		}

	}
}