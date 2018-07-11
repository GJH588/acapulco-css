#include "CBhop.h"

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void CBhop::Hook(CUserCmd* cmd)
{
	if (!Settings::Bhop::Enabled)
		return;

	CSetupPlayer pLocal = gPlayers[LOCALPLAYER];
	CBaseEntity* eLocal = pLocal.BaseEnt();

	if (!eLocal)
		return;

	if (Settings::Bhop::BhopType == BHOP_DEFAULT)
	{
		if (cmd->buttons & IN_JUMP && !(pLocal.GetFlags() & FL_ONGROUND))
		cmd->buttons &= ~IN_JUMP;
	}
	else
	{
		static bool bFirstJump = false;
		static bool bFakeJump = false;

		if (cmd->buttons & IN_JUMP)
		{
			if (!bFirstJump)
			{
				bFirstJump = bFakeJump = true;
			}
			else if (!(pLocal.GetFlags() & FL_ONGROUND))
			{
				if (bFakeJump)
					bFakeJump = false;
				else
					cmd->buttons &= ~IN_JUMP;
			}
			else
			{
				bFakeJump = true;
			}
		}
		else
		{
			bFirstJump = false;
		}
	}
}