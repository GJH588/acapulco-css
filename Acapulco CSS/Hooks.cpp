#include "SDK.h"
#include "Hooks.h"
#include "CDrawManager.h"
#include "CColorManager.h"
#include "CGlobalVars.h"
#include "Menu.h"
#include "crc32.h"

// Features
#include "CBhop.h"
#include "CESP.h"
#include "CAutoStrafe.h"

CAutoStrafe* autoStrafe;
CBhop* bhop;
CESP* esp;

CreateMoveFn oCreateMove;
PaintTraverseFn oPaintTraverse;
FrameStageNotifyFn oFrameStageNotify;
KeyEventFn oKeyEvent;

CScreen gScreen;

bool init = false;

void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active)
{
	oCreateMove(sequence_number, input_sample_frametime, active);

	if (!init)
	{
		gPlayerVars.FindOffsets();
		gDrawManager.Initialize();
		gColorManager.Initialize();

		init = true;
	}

	CUserCmd* pCmd = pInput->GetUserCmd(sequence_number);
	gMenu.Render();
	
	if (!(sequence_number != 0))
		return;
	
	// Hooks
	bhop->Hook(pCmd);
	autoStrafe->Hook(pCmd);

	CVerifiedUserCmd* verifiedlist = *(CVerifiedUserCmd**)((DWORD)pInput + 0xC8);
	CVerifiedUserCmd* verified = &verifiedlist[sequence_number % 90];
	verified->m_cmd = *pCmd;
	verified->m_crc = gCRC.CRC32_ProcessSingleBuffer(pCmd, sizeof(pCmd));
}

void __stdcall hkPaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	oPaintTraverse(vguiPanel, forceRepaint, allowForce);

	static unsigned int vguiMatSystemTopPanel;

	if (!vguiMatSystemTopPanel)
	{
		const char* szName = pIPanel->GetName(vguiPanel);
		if (szName[0] == 'M' && szName[3] == 'S')
			vguiMatSystemTopPanel = vguiPanel;
	}

	if (vguiMatSystemTopPanel == vguiPanel)
	{
		if (pEngine->IsDrawingLoadingImage() || !pEngine->IsInGame() || !pEngine->IsConnected() || pEngine->Con_IsVisible() || (pEngine->IsTakingScreenshot() && Settings::Misc::AntiSS))
			return;

		if (gMenu.bActive)
			gMenu.DrawMenu();

		gDrawManager.DrawString(true, 3, 3, COLORCODE(255, 255, 255, 255), "Acapulco CSS");

		for (int i = 0; i < pEntList->GetHighestEntityIndex(); i++)
		{
			esp->DrawPlayers(i);
			esp->DrawItems(i);
		}
	}
}

void __stdcall hkFrameStageNotify(ClientFrameStage_t curStage)
{
	oFrameStageNotify(curStage);
}

int __stdcall hkKeyEvent(int eventcode, int keynum, const char *pszCurrentBinding)
{
	if(eventcode == 1)
	{
		if(keynum == 72)
		{
			gMenu.bActive = !gMenu.bActive;
		}

		if(gMenu.bActive)
		{
			if(keynum == 88 || keynum == 112)
			{
				if(gMenu.iIndex > 0) gMenu.iIndex--;
				else gMenu.iIndex = gMenu.iItems - 1;
				return 0;
			}
			else if(keynum == 90 || keynum == 113)
			{

				if(gMenu.iIndex < gMenu.iItems - 1 ) gMenu.iIndex++;
				else gMenu.iIndex = 0;
				return 0;

			}
			else if(keynum == 89 || keynum == 107)
			{

				if(gMenu.pMenu[gMenu.iIndex].value )
				{
					gMenu.pMenu[gMenu.iIndex].value[0] -= gMenu.pMenu[gMenu.iIndex].flStep;
					if(gMenu.pMenu[gMenu.iIndex].value[0] < gMenu.pMenu[gMenu.iIndex].flMin)
						gMenu.pMenu[gMenu.iIndex].value[0] = gMenu.pMenu[gMenu.iIndex].flMax;
				}
				return 0;
			}
			else if(keynum == 91 || keynum == 108)
			{
				if(gMenu.pMenu[gMenu.iIndex].value )
				{
					gMenu.pMenu[gMenu.iIndex].value[0] += gMenu.pMenu[gMenu.iIndex].flStep;
					if(gMenu.pMenu[gMenu.iIndex].value[0] > gMenu.pMenu[gMenu.iIndex].flMax)
						gMenu.pMenu[gMenu.iIndex].value[0] = gMenu.pMenu[gMenu.iIndex].flMin;
				}
				return 0;
			}
		}
	}
	return (int)oKeyEvent(eventcode, keynum, pszCurrentBinding);
}