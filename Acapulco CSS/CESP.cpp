#include "CESP.h"

void CESP::DrawBone(CBaseEntity* pEntity, int* iBones, int count, Color dwCol)
{
	for (int i = 0; i < count; i++)
	{
		if (i == count - 1)
			continue;

		Vector vBone1;
		Vector vBone2;
			
		gDrawManager.GetBonePosition(pEntity, vBone1, iBones[i]);
		gDrawManager.GetBonePosition(pEntity, vBone2, iBones[i + 1]);

		Vector vScr1, vScr2;

		if (!gDrawManager.WorldToScreen(vBone1, vScr1) || !gDrawManager.WorldToScreen(vBone2, vScr2))
			continue;

		gDrawManager.DrawLine(vScr1.x, vScr1.y, vScr2.x, vScr2.y, dwCol);
	}
}

void CESP::DrawPlayers(int i)
{
	player_info_s pInfo;

	CSetupPlayer _pLocalBaseEntity = gPlayers[LOCALPLAYER];
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
			if (Settings::ESP::BoxesType == BOXES_DEFAULT)
			{
				gDrawManager.OutlineRect(vScreen.x - Width / 2, vScreenHead.y, Width, Height, Color(255, 255, 255, 255));
			}
			else
			{
				gDrawManager.OutlineRect(vScreen.x - Width / 2, vScreenHead.y, Width, Height, Color(255, 255, 255, 255));
				gDrawManager.OutlineRect(vScreen.x - Width / 2 - 1, vScreenHead.y - 1, Width + 2, Height + 2, Color(0, 0, 0, 255));
				gDrawManager.OutlineRect(vScreen.x - Width / 2 + 1, vScreenHead.y + 1, Width - 2, Height - 2, Color(0, 0, 0, 255));
			}
		}

		if (Settings::ESP::Name)
		{
			gDrawManager.DrawString(Font::Get().ESP, vScreen.x + (Width / 2) + 2, vScreen.y - Height + 6, Color(255, 255, 255, 255), "%s", pInfo.name);
			vScreen.y += gDrawManager.GetESPHeight();
		}

		if (Settings::ESP::Distance)
		{
			gDrawManager.DrawString(Font::Get().ESP, vScreen.x + (Width / 2) + 2, vScreen.y - Height + 6, Color(255, 255, 255, 255), "[%.0fm]", Distance);
			vScreen.y += gDrawManager.GetESPHeight();
		}

		if (Settings::ESP::Health)
		{
			gDrawManager.DrawString(Font::Get().ESP, vScreen.x + (Width / 2) + 2, vScreen.y - Height + 6, Color(255, 255, 255, 255), "%i HP", _pBaseEntity.GetHealth());
			vScreen.y += gDrawManager.GetESPHeight();
		}

		if (Settings::ESP::HealthBar)
		{
			gDrawManager.DrawHealthBar(vScreen.x, vScreenHead.y + Height + 1, _pBaseEntity.GetHealth(), Width + 5, 2, Color(0, 255, 0, 255));
		}

		if (Settings::ESP::Armor)
		{
			gDrawManager.DrawString(Font::Get().ESP, vScreen.x + (Width / 2) + 2, vScreen.y - Height + 6, Color(255, 255, 255, 255), "%i AP", _pBaseEntity.GetArmor());
			vScreen.y += gDrawManager.GetESPHeight();
		}

		if (Settings::ESP::HeadDot)
		{
			gDrawManager.DrawRect(vScreen.x - Width / 8, vScreenHead.y + 6, 3, 3, Color(255, 255, 255, 255));
		}

		if (Settings::ESP::Bones)
		{
			int iLeftArmBones[] = { 18, 17, 16, 14 };
			int iRightArmBones[] = { 31, 30, 29, 14 };

			int iHeadBones[] = { 14, 13, 0 };

			int iLeftLegBones[] = { 3, 2, 1, 0 };
			int iRightLegBones[] = { 7, 6, 5, 0 };

			DrawBone(pBaseEntity, iLeftArmBones, 4, Color(255, 255, 255, 255));
			DrawBone(pBaseEntity, iRightArmBones, 4, Color(255, 255, 255, 255));

			DrawBone(pBaseEntity, iHeadBones, 3, Color(255, 255, 255, 255));

			DrawBone(pBaseEntity, iLeftLegBones, 4, Color(255, 255, 255, 255));
			DrawBone(pBaseEntity, iRightLegBones, 4, Color(255, 255, 255, 255));
		}
	}
}

void CESP::DrawItems(int i)
{
	if (!Settings::ESP::Items)
		return;

	CBaseEntity* pLocalBaseEntity = gPlayers[LOCALPLAYER].BaseEnt();
	CBaseEntity* pBaseEntity = pEntList->GetClientEntity(i);

	if (!pBaseEntity)
		return;

	if (!pBaseEntity->IsDormant())
	{
		Vector vScreen;
		int iTeamNum;

		if (!gDrawManager.WorldToScreen(pBaseEntity->GetAbsOrigin(), vScreen))
			return;

		if (!(pEntList->GetClientEntityFromHandle(*MakePtr(int*, pBaseEntity, gPlayerVars.m_hOwnerEntity))))
		{
			if (strstr(pBaseEntity->GetClientClass()->GetName(), "CWeapon"))
				gDrawManager.DrawString(Font::Get().ESP, vScreen.x, vScreen.y, Color(255, 255, 255, 255), "%s", string(pBaseEntity->GetClientClass()->GetName()).substr(7).c_str());
			if (streql(pBaseEntity->GetClientClass()->GetName(), "CAK47"))
				gDrawManager.DrawString(Font::Get().ESP, vScreen.x, vScreen.y, Color(255, 255, 255, 255), "AK47");
			if (streql(pBaseEntity->GetClientClass()->GetName(), "CC4"))
				gDrawManager.DrawString(Font::Get().ESP, vScreen.x, vScreen.y, Color(255, 255, 255, 255), "C4");
		}

	}
}