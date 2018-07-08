#include "CAutoStrafe.h"

void CAutoStrafe::Hook(CUserCmd* cmd)
{
	if (!Settings::AutoStrafe::Enabled)
		return;

	CSetupPlayer pLocal = gPlayers[me];
	CBaseEntity* eLocal = pLocal.BaseEnt();

	if (!eLocal)
		return;

	if (!(pLocal.GetFlags() & FL_ONGROUND))
	{
		if (cmd->mousedx > 0)
			cmd->sidemove = 400;
		else if (cmd->mousedx < 0)
			cmd->sidemove = -400;
	}
}