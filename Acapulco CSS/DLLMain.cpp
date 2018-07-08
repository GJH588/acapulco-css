#include "SDK.h"
#include "VMT Hook.h"
#include "Hooks.h"
#include "Menu.h"
#include "CGlobalVars.h"

CInput* pInput;
CEntList* pEntList;
EngineClient* pEngine;
IPanel* pIPanel;
ISurface* pSurface;
CHLClient* pClient;
CTrace* pTrace;
CPlayers gPlayers;

CPlayerVariables gPlayerVars;

CreateInterface_t EngineFactory = NULL;
CreateInterface_t ClientFactory = NULL;
CreateInterface_t VGUIFactory = NULL;
CreateInterface_t VGUI2Factory = NULL;

extern CreateMoveFn oCreateMove;
extern PaintTraverseFn oPaintTraverse;
extern FrameStageNotifyFn oFrameStageNotify;
extern KeyEventFn oKeyEvent;

CUserCmd* __fastcall hkGetUserCmd(CInput* input, int, int sqnum)
{
	CUserCmd* cmdlist = *(CUserCmd**)((DWORD)input + 0xC4);
	CUserCmd* cmd = &cmdlist[sqnum % 90];
	cmd->random_seed = 150;

	return cmd;
}

DWORD WINAPI dwMainThread(LPVOID lpArguments)
{
	if (!pClient)
	{
		ClientFactory = (CreateInterfaceFn)GetProcAddress(gUtils.GetModuleHandleSafe("client.dll"), "CreateInterface");
		pClient = (CHLClient*)ClientFactory("VClient017", NULL);

		pEntList = (CEntList*)ClientFactory("VClientEntityList003", NULL);
		EngineFactory = (CreateInterfaceFn)GetProcAddress(gUtils.GetModuleHandleSafe("engine.dll"), "CreateInterface");
		pTrace = (CTrace*)EngineFactory("EngineTraceClient003", 0);

		pEngine = (EngineClient*)EngineFactory("VEngineClient013", NULL);
		VGUIFactory = (CreateInterfaceFn)GetProcAddress(gUtils.GetModuleHandleSafe("vguimatsurface.dll"), "CreateInterface");

		pSurface = (ISurface*)VGUIFactory("VGUI_Surface030", NULL);
		VGUI2Factory = (CreateInterfaceFn)GetProcAddress(gUtils.GetModuleHandleSafe("vgui2.dll"), "CreateInterface");

		if (pClient)
		{
			CVMTHookManager* g_pClientHook = NULL;
			g_pClientHook = new CVMTHookManager((PDWORD*)pClient);

			oKeyEvent = (KeyEventFn)g_pClientHook->dwHookMethod((DWORD)hkKeyEvent, 20);
			oCreateMove = (CreateMoveFn)g_pClientHook->dwHookMethod((DWORD)hkCreateMove, 21);
			oFrameStageNotify = (FrameStageNotifyFn)g_pClientHook->dwHookMethod((DWORD)hkFrameStageNotify, 35);

			DWORD dwInputPointer = gUtils.FindPattern((DWORD)oCreateMove, 0x100, (byte*)"\x8B\x0D", "xx");
			if (dwInputPointer)
			{
				dwInputPointer += 0x2;
				pInput = **(CInput***)dwInputPointer;
			}

			CVMTHookManager* inputhook = NULL;
			inputhook = new CVMTHookManager((PDWORD*)pInput);
		}

		if (!pIPanel)
		{
			pIPanel = (IPanel*)VGUI2Factory("VGUI_Panel009", NULL);

			if (pIPanel)
			{
				CVMTHookManager* g_pPanelHook = new CVMTHookManager;

				if (g_pPanelHook->bInitialize((PDWORD*)pIPanel))
				{
					oPaintTraverse = (PaintTraverseFn)g_pPanelHook->dwHookMethod((DWORD)hkPaintTraverse, 41);
				}
			}
		}
	}
	
	pEngine->ClientCmd_Unrestricted("clear; echo Injected; showconsole");
	
	return 0;
}

int WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		gUtils.BaseUponModule((HMODULE)hInstance);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)dwMainThread, NULL, 0, NULL);
	}
	return true;
}
