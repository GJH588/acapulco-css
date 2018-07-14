#include "Menu.h"
#include "CRecorder.h"

const char* BoxesType[] = { "Defualt", "Outline" };
const char* BhopType[] = { "Defualt", "SMAC" };

void BtnNormal()
{
	auto& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Button] = ImVec4(0.10, 0.10, 0.10, .98f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.f);
}

void BtnActive()
{
	auto& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Button] = ImVec4(0.5098f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.5098f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.5098f, 0.00f, 0.00f, 1.00f);
}

void EspTab()
{
	ImGui::Checkbox("Enabled", &Settings::ESP::Enabled);
	ImGui::Checkbox("Boxes", &Settings::ESP::Boxes);
	ImGui::Checkbox("Name", &Settings::ESP::Name);
	ImGui::Checkbox("Health", &Settings::ESP::Health);
	ImGui::Checkbox("Health Bar", &Settings::ESP::HealthBar);
	ImGui::Checkbox("Distance", &Settings::ESP::Distance);
	ImGui::Checkbox("Bones", &Settings::ESP::Bones);
	ImGui::Checkbox("Head Dot", &Settings::ESP::HeadDot);
	ImGui::Checkbox("Items", &Settings::ESP::Items);
	ImGui::Checkbox("Draw Team", &Settings::ESP::DrawTeam);
}

void OtherTab()
{
	ImGui::Checkbox("Bhop", &Settings::Bhop::Enabled);
	ImGui::Checkbox("Auto Strafe", &Settings::AutoStrafe::Enabled);
	ImGui::Checkbox("Anti SS", &Settings::Misc::AntiSS);
	ImGui::Checkbox("Recorder", &Settings::Misc::Recorder);
}

void OptionsTab()
{
	ImGui::Columns(2, NULL, true);
	{
		ImGui::Text("Boxes");
		ImGui::Text("Bhop");
	}
	ImGui::NextColumn();
	{
		ImGui::PushItemWidth(-1);
		ImGui::Combo("##BOXESTYPE", (int*)&Settings::ESP::BoxesType, BoxesType, IM_ARRAYSIZE(BoxesType));
		ImGui::Combo("##BHOPTYPE", (int*)&Settings::Bhop::BhopType, BhopType, IM_ARRAYSIZE(BhopType));
		ImGui::PopItemWidth();
	}
	ImGui::Columns(1);
}

void CMenu::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(200, 100));
	ImGui::Begin(ICON_FA_SHIELD " Acapulco CSS", &Settings::Menu::Enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::BeginChild("##TABCHILD", ImVec2(56, -1), true);
		{
			if (Settings::Menu::Tab == 0) BtnActive(); else BtnNormal();
			if (ImGui::Button(ICON_FA_EYE, ImVec2(-1, 16))) { Settings::Menu::Tab = 0; }

			if (Settings::Menu::Tab == 1) BtnActive(); else BtnNormal();
			if (ImGui::Button(ICON_FA_CROSSHAIRS, ImVec2(-1, 16))) { Settings::Menu::Tab = 1; }

			if (Settings::Menu::Tab == 2) BtnActive(); else BtnNormal();
			if (ImGui::Button(ICON_FA_PAINT_BRUSH, ImVec2(-1, 16))) { Settings::Menu::Tab = 2; }

			if (Settings::Menu::Tab == 3) BtnActive(); else BtnNormal();
			if (ImGui::Button(ICON_FA_COG, ImVec2(-1, 16))) { Settings::Menu::Tab = 3; }

			BtnNormal();
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("##FEATURESCHILD", ImVec2(-1, -1), true);
		{
			if (Settings::Menu::Tab == 0) { EspTab(); }
			if (Settings::Menu::Tab == 2) { OtherTab(); }
			if (Settings::Menu::Tab == 3) { OptionsTab(); }
		}
		ImGui::EndChild();
	}
	ImGui::End();
}