#include "CreateMove.h"
#include "Player Manager.h"

CCreateMove gCreateMove;
Vector hitpos;

void CCreateMove::Invoke( CUserCmd* cmd )
{	
}

void CCreateMove::Triggerbot(CUserCmd* cmd, CSetupPlayer pMe)
{
	Ray_t ray;
	trace_t trace;
	CBaseEntity* eMe = pMe.BaseEnt();
	if (!eMe)
		return;
	Vector angles;
	Vector eyepos = eMe->GetEyePosition();
	AngleVectors(cmd->viewangles, &angles);
	angles = angles * 8192 + eyepos;
	CTraceFilter filter;
	filter.pSkip1 = eMe;
	ray.Init(eyepos, angles);
	g_pTrace->TraceRay(ray, 0x4600400B, &filter, &trace);
	CBaseEntity* target = trace.m_pEnt;
	if (!target)
		return;
	player_info_t info;
	if (!g_pEngine->GetPlayerInfo(target->GetIndex(), &info))
		return;
	if (pMe.GetTeam() == gPlayers[target->GetIndex()].GetTeam())
		return;
	cmd->buttons |= IN_ATTACK;
	
}