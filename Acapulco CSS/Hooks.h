#pragma once
#include "SDK.h"

enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,
	FRAME_START,

	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,

	FRAME_RENDER_START,
	FRAME_RENDER_END
};

typedef void* (__stdcall* CreateMoveFn)(int, float, bool);
typedef void* (__stdcall* PaintTraverseFn)(int, bool, bool);
typedef void* (__stdcall* KeyEventFn)(int, int, const char *);
typedef void* (__stdcall* FrameStageNotifyFn)(ClientFrameStage_t);

void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active);
void __stdcall hkPaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce);
void __stdcall hkFrameStageNotify(ClientFrameStage_t curStage);
int __stdcall hkKeyEvent(int eventcode, int keynum, const char *pszCurrentBinding);