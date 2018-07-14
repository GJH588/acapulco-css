#include "SDK.h"
#include "Hooks.h"
#include "CDrawManager.h"
#include "CGlobalVars.h"
#include "crc32.h"
#include "Menu.h"

// Features
#include "CBhop.h"
#include "CESP.h"
#include "CAutoStrafe.h"
#include "CRecorder.h"

CBhop* bhop;
CESP* esp;
CAutoStrafe* autoStrafe;
CRecorder* recorder;

CMenu* menu;

CreateMoveFn oCreateMove;
PaintTraverseFn oPaintTraverse;
FrameStageNotifyFn oFrameStageNotify;
KeyEventFn oKeyEvent;
EndSceneFn oEndScene;

CScreen gScreen;

bool init = false;
bool initImgui = false;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

HRESULT STDMETHODCALLTYPE hkEndScene(IDirect3DDevice9* vDevice)
{
	if (!initImgui)
	{
		ImGui::CreateContext();
		ImGui_ImplDX9_Init(hWindow, vDevice);

		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontDefault();

		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		
		io.Fonts->AddFontFromMemoryCompressedTTF(FontAwesome_data, FontAwesome_size, 13.0f, &icons_config, icons_ranges);

		ImGuiStyle& style = ImGui::GetStyle();

		ImVec4 mainColorHovered = ImVec4(0.211f + 0.1f, 0.211f + 0.1f, 0.211f + 0.1f, 1.f);
		ImVec4 mainColorActive = ImVec4(0.211f + 0.2f, 0.211f + 0.2f, 0.211f + 0.2f, 1.f);
		ImVec4 menubarColor = ImVec4(0.211f, 0.2119f, 0.211f, 1.f - 0.8f);
		ImVec4 frameBgColor = ImVec4(0.211f, 0.211f, 0.211f, 1.f + .1f);
		ImVec4 tooltipBgColor = ImVec4(0.211f, 0.211f, 0.211f, 1.f + .05f);

		style.Alpha = 0.75f;
		style.FramePadding = ImVec2(3.f, 0.f);
		style.GrabRounding = 0.f;
		style.WindowRounding = 0.0f;
		style.WindowPadding = ImVec2(3.f, 3.f);
		style.ScrollbarRounding = 0.f;
		style.FrameRounding = 0.f;
		style.ItemSpacing = ImVec2(3.0f, 3.0f);
		style.ItemInnerSpacing = ImVec2(3.0f, 3.0f);
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.TouchExtraPadding = ImVec2(0.f, 0.f);
		style.WindowBorderSize = 0.f;
		style.ChildBorderSize = 1.f;
		style.PopupBorderSize = 1.0f;
		style.FrameBorderSize = 0.f;
		style.ScrollbarSize = 3.f;

		style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0941176470588235f, 0.0941176470588235f, 0.0941176470588235f, 0.9411764705882353);
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(.0f, .0f, .0f, .0f);
		style.Colors[ImGuiCol_PopupBg] = tooltipBgColor;
		style.Colors[ImGuiCol_Border] = ImVec4(0.2117647058823529f, 0.2117647058823529f, 0.2117647058823529f, 1.f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = frameBgColor;
		style.Colors[ImGuiCol_FrameBgHovered] = mainColorHovered;
		style.Colors[ImGuiCol_FrameBgActive] = mainColorActive;
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2117647058823529f, 0.2117647058823529f, 0.2117647058823529f, 1.f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2117647058823529f, 0.2117647058823529f, 0.2117647058823529f, 1.f);
		style.Colors[ImGuiCol_MenuBarBg] = menubarColor;
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(frameBgColor.x + .05f, frameBgColor.y + .05f, frameBgColor.z + .05f, frameBgColor.w);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.2117647058823529f, 0.2117647058823529f, 0.2117647058823529f, 1.f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ScrollbarGrabActive] = mainColorActive;
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.2117647058823529f, 0.2117647058823529f, 0.2117647058823529f, 1.f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.5098f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.6274f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.2117647058823529f, 0.2117647058823529f, 0.2117647058823529f, 1.f);
		style.Colors[ImGuiCol_ButtonHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ButtonActive] = mainColorActive;
		style.Colors[ImGuiCol_Header] = ImVec4(0.2117647058823529f, 0.2117647058823529f, 0.2117647058823529f, 1.f);
		style.Colors[ImGuiCol_HeaderHovered] = mainColorHovered;
		style.Colors[ImGuiCol_HeaderActive] = mainColorActive;
		style.Colors[ImGuiCol_Column] = ImVec4(0.2117647058823529f, 0.2117647058823529f, 0.2117647058823529f, 1.f);
		style.Colors[ImGuiCol_ColumnHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.5098039215686275, 0, 0, 1); ///
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2117647058823529f, 0.2117647058823529f, 0.2117647058823529f, 1.f);
		style.Colors[ImGuiCol_ResizeGripHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ResizeGripActive] = mainColorActive;
		style.Colors[ImGuiCol_CloseButton] = ImVec4(0.2117647058823529f, 0.2117647058823529f, 0.2117647058823529f, 1.f);
		style.Colors[ImGuiCol_CloseButtonHovered] = mainColorHovered;
		style.Colors[ImGuiCol_CloseButtonActive] = mainColorActive;
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.2117647058823529f, 0.2117647058823529f, 0.2117647058823529f, 1.f);
		style.Colors[ImGuiCol_PlotLinesHovered] = mainColorHovered;
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.2117647058823529f, 0.2117647058823529f, 0.2117647058823529f, 1.f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = mainColorHovered;
		style.Colors[ImGuiCol_TextSelectedBg] = mainColorHovered;
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.0f, 0.0f, 0.0f, 0.75f);

		initImgui = true;
	}

	if (GetAsyncKeyState(VK_INSERT))
		Settings::Menu::Enabled = !Settings::Menu::Enabled;

	ImGui_ImplDX9_NewFrame();

	if (Settings::Menu::Enabled)
	{
		menu->Draw();
		
		if (Settings::Misc::Recorder)
		{
			recorder->Draw();
		}
	}

	ImGui::Render();

	return oEndScene(vDevice);
}

void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active)
{
	oCreateMove(sequence_number, input_sample_frametime, active);

	if (!init)
	{
		gPlayerVars.FindOffsets();
		gDrawManager.Initialize();

		init = true;
	}

	CUserCmd* pCmd = pInput->GetUserCmd(sequence_number);
	//gMenu.Render();
	
	if (!(sequence_number != 0))
		return;
	
	// Hooks
	bhop->Hook(pCmd);
	autoStrafe->Hook(pCmd);
	recorder->Hook(pCmd);

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

		gDrawManager.DrawString(Font::Get().MenuText, 3, 3, Color(255, 255, 255, 255), "Acapulco CSS");

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
	return (int)oKeyEvent(eventcode, keynum, pszCurrentBinding);
}

bool PressedKeys[256] { false };
WNDPROC pOldWindowProc = nullptr;
HWND hWindow = nullptr;

LRESULT STDMETHODCALLTYPE hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		PressedKeys[VK_LBUTTON] = true;
		break;
	case WM_LBUTTONUP:
		PressedKeys[VK_LBUTTON] = false;
		break;
	case WM_RBUTTONDOWN:
		PressedKeys[VK_RBUTTON] = true;
		break;
	case WM_RBUTTONUP:
		PressedKeys[VK_RBUTTON] = false;
		break;
	case WM_KEYDOWN:
		PressedKeys[wParam] = true;
		break;
	case WM_KEYUP:
		PressedKeys[wParam] = false;
		break;
	default: 
		break;
	}

		static bool down = false;
		static bool clicked = false;

		if (PressedKeys[VK_INSERT])
		{
			clicked = false;
			down = true;
		}
		else if (!PressedKeys[VK_INSERT] && down)
		{
			clicked = true;
			down = false;
		}
		else
		{
			clicked = false;
			down = false;
		}

		if (clicked)
			Settings::Menu::Enabled = !Settings::Menu::Enabled;


	if (Settings::Menu::Enabled && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(pOldWindowProc, hWnd, uMsg, wParam, lParam);
}