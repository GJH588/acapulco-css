#include "CBhop.h"

void CBhop::Hook(CUserCmd* cmd)
{
	if (!Settings::Bhop::Enabled)
		return;

	CSetupPlayer pLocal = gPlayers[me];
	CBaseEntity* eLocal = pLocal.BaseEnt();

	if (!eLocal)
		return;

	if (cmd->buttons & IN_JUMP && !(pLocal.GetFlags() & FL_ONGROUND))
		cmd->buttons &= ~IN_JUMP;
}