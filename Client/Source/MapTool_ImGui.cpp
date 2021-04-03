#include "stdafx.h"
#include "MapTool.h"

#include <fstream>
#include <ostream>
#include <iostream>


/*
ImGui cpp
*/





bool MapTool::CheckWindow(const char* Text)
{
	return false;
}

void MapTool::ShowMapTool()
{
	//Window Setting
	ImGui::Begin("Hellow MapTool");
	ImGui::SetWindowPos(ImVec2(0, 10));
	ImGui::SetWindowSize(ImVec2(400, 600));

	m_bHoveredMaptool[(int)eWindowID::MapTool] = ImGui::IsWindowHovered();
	//NameTable
	NameTableGroup();

	if (m_bReadyNameTable)
	{
		//balse
		BaseMapCreateGroup();
		if (ImGui::CollapsingHeader("Props Tree"))
		{
			ImGui::Text("Test");
			ImGui::Text("Test");
			ImGui::Text("Test");
			ImGui::Text("Test");
			ImGui::Text("Test");
		}
		if (ImGui::CollapsingHeader("InterationObject Tree"))
		{
			ImGui::Text("Test");
			ImGui::Text("Test");
			ImGui::Text("Test");
			ImGui::Text("Test");
			ImGui::Text("Test");
			ImGui::Text("Test");
		}
		//PeekingOption
		PeekingOptionGroup();
		TransFormCtrlGroup();
		PropsOptionGroup();
		SaveFileGroup();
	}

	ImGui::End();
}

void MapTool::ShowPivotControl()
{

	ImGui::Begin("Hellow Pivot"); ImGui::SameLine();
	ImGui::SetWindowPos(ImVec2(g_nWndCX- 350.f, g_nWndCY - 200.f));
	ImGui::SetWindowSize(ImVec2(350, 200));

	m_bHoveredMaptool[(int)eWindowID::Pivot] = ImGui::IsWindowHovered();

	
	ImGui::Text("Drag Sensitivity"); ImGui::SameLine(); 
	HelpMarker("Edit value Drag Power");
	static float fSensitivety = 0.1f;
	ImGui::InputFloat("", &fSensitivety, 0.1f);

	ImGui::Text("Move Sensitivity"); ImGui::SameLine();
	HelpMarker("Also can be moved by keyboard\n"
		"x :left ,right z : up Down  y : home , end");
	ImGui::InputFloat(".", &m_fPivotMoveSpeed, 0.1f);


	ImGui::Text("Position"); ImGui::SameLine(); 
	HelpMarker(
		"Drag to edit value.\n"
		"Hold SHIFT/ALT for faster/slower edit.\n"
		"Double-click or CTRL+click to input value.");
	static vec3 TestPos = vZero;  // pivot pos Get 
	ImGui::DragFloat3("", (float*)&TestPos.x,fSensitivety);



	ImGui::End();
}



void MapTool::NameTableGroup()
{
	ImGui::Text("NameTable"); ImGui::SameLine();
	HelpMarker("Must be Ready Name Table"); ImGui::SameLine();
	ImGui::Text("NameTable Ready :"); ImGui::SameLine();

	ImVec4 fTextColor = m_bReadyNameTable ? ImVec4(0.0f, 0.0f, 1.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImGui::TextColored(fTextColor, m_bReadyNameTable ? "TRUE " : "FALSE");
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(6 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(6 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(6 / 7.0f, 0.8f, 0.8f));

	if (ImGui::Button("NewTable"))
		ImGui::OpenPopup("New Table?");
	ImGui::PopStyleColor(3);

	if (ImGui::BeginPopupModal("New Table?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Do you want to overwrite the data?\n");
		ImGui::PushStyleColor(ImGuiCol_Button,		   ImVec4(33/255.f, 33 / 255.f, 33 / 255.f,255));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,  ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f,255));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,   ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f,255));
		
		if (ImGui::Button("OK", ImVec2(100, 0)))
		{ 
			m_bReadyNameTable = NewFBXNameTable(L"../../Resource/SaveData/PropsNameTable.json");
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor(3);


		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 255));
		if (ImGui::Button("Cancel", ImVec2(100, 0)))
			ImGui::CloseCurrentPopup();
		ImGui::PopStyleColor(3);

		ImGui::EndPopup();
	}
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7 / 7.0f, 0.8f, 0.8f));
	if (ImGui::Button("LoadTable"))
	{
		m_bReadyNameTable = LoadFBXnametable(L"../../Resource/SaveData/PropsNameTable.json");
	}

	ImGui::PopStyleColor(3);
}

void MapTool::BaseMapCreateGroup()
{
	if(ImGui::CollapsingHeader("==============BaseMapCreate=============", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("BaseMap"); ImGui::SameLine();
		HelpMarker("Write FBX Path but Only one file can be loaded");
		static char szBaseMapPath[MAX_PATH] = "";
		ImGui::InputText("`", szBaseMapPath, MAX_PATH, ImGuiInputTextFlags_NoUndoRedo); ImGui::SameLine();
		//ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
		if (ImGui::Button("Ins"))
		{
			//체크전에 로드 된거 있으면 릴리즈 
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3 / 7.0f, 0.8f, 0.8f));
		if (ImGui::Button("Del"))
		{

		}
		ImGui::PopStyleColor(3);
	}
}

void MapTool::PeekingOptionGroup()
{
	auto convertToString = [](const std::wstring& w) {
		std::string Convert;
		Convert.assign(std::begin(w), std::end(w));
		return Convert;
	};


	if (ImGui::CollapsingHeader("==============PeekingOption=============", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("WorkType");
		ImGui::RadioButton("Create", (int*)&m_eWorkType, (int)eWorkOption::Create); ImGui::SameLine();
		ImGui::RadioButton("Delete", (int*)&m_eWorkType, (int)eWorkOption::Delete); ImGui::SameLine();
		ImGui::RadioButton("Modyfy", (int*)&m_eWorkType, (int)eWorkOption::Modify); ImGui::SameLine();
		HelpMarker("Hot Key [R] [T] [Y] \n If you want to modify it, in the hotkey function");


		if (m_eWorkType == eWorkOption::Create)
		{
			if (m_strSelectName.empty())
			{
				ImGui::TextColored(ImVec4(1, 0, 0, 1),  "Click the FBX Name node "); 
				m_eCreateOption = eCreatePosition::End;
			}
			else
			{
				ImGui::Text("Create location");
				ImGui::RadioButton("PeekingPos", (int*)&m_eCreateOption, (int)eCreatePosition::PeekingPos);
				ImGui::SameLine();
				ImGui::RadioButton("PivotPos", (int*)&m_eCreateOption, (int)eCreatePosition::PivotPos);
				ImGui::SameLine();
				HelpMarker("When in creation mode, press the space bar to create an object");
			}
		}



		ImGui::Text("PeekingType");
		ImGui::RadioButton("Single", (int*)&m_ePeekType, (int)ePeekingType::Single);

		if (m_eWorkType != eWorkOption::Modify)
			m_ePeekType = ePeekingType::Single;
		else
		{
			ImGui::SameLine();
			ImGui::RadioButton("Multy", (int*)&m_ePeekType, (int)ePeekingType::Multi); ImGui::SameLine();
			HelpMarker("The number of selected objects is displayed.\n"
				"Multiple objects can be modified at the same time.");
		}

		if (m_ePeekType == ePeekingType::Single)
		{
			ImGui::Text("Peeking Object Name : "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", m_strSelectName.empty() ? "NULL" : convertToString(m_strSelectName).c_str());
		}
		else
		{
			ImGui::Text("Peeking Object Cnt: "); ImGui::SameLine();
			char szCnt[64];
			if (m_iPeekingCnt == 0)
				strcpy_s(szCnt, "ZERO");
			else
				_itoa_s(m_iPeekingCnt, szCnt, 10);

			ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", szCnt);
		}
	}
}

void MapTool::TransFormCtrlGroup()
{
	if (ImGui::CollapsingHeader("==============TransForm=============", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Drag Sensitivity"); ImGui::SameLine();
		HelpMarker("Edit value Drag Power");
		static float fSensitivety = 0.1f;
		static vec3 TestScale = vZero;  // obj pos Get 
		static vec3 TestRot = vZero;  // obj pos Get 
		static vec3 TestPos = vZero;  // obj pos Get 

		ImGui::InputFloat("", &fSensitivety, 0.1f);
		ImGui::SameLine();
		HelpMarker("Drag to edit value. Range 0~100 \n"
			"Hold SHIFT/ALT for faster/slower edit.\n"
			"Double-click or CTRL+click to input value.");
		fSensitivety = CLAMP(fSensitivety, 0, 100);
		ImGui::Text("Sacle    ");
		ImGui::SameLine();
		ImGui::DragFloat3("        ",(float*)&TestScale.x, fSensitivety);
		ImGui::Text("Rotate");
		ImGui::SameLine();
		ImGui::DragFloat3("     ",(float*)&TestRot.x, fSensitivety);
		ImGui::Text("Positon"); ImGui::SameLine();
		ImGui::DragFloat3("   ",(float*)&TestPos.x, fSensitivety);
	}
}

void MapTool::PropsOptionGroup()
{
	if (m_ePeekType !=  ePeekingType::Multi)
	{
		if (ImGui::CollapsingHeader("==============PropsOption=============", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Option");
			if (ImGui::Checkbox("Decoration", &m_bPropsOption[(int)ePropsOption::Decoration]))
				ApplyPropsOption();
			ImGui::SameLine();

			if (ImGui::Checkbox("Floating", &m_bPropsOption[(int)ePropsOption::Floating]))
				ApplyPropsOption();
			ImGui::SameLine();

			if (ImGui::Checkbox("Fixed", &m_bPropsOption[(int)ePropsOption::Fixed]))
				ApplyPropsOption();
		}
	}
}

void MapTool::SaveFileGroup()
{
	if (ImGui::CollapsingHeader("==============SaveFile=============", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextColored(ImVec4(0.3f, 0.f, 0.f, 1.f), "Loading List"); ImGui::SameLine();
		HelpMarker("ex) ***.json\n"
			"Saves the list of meshes used in the current scene. \n"
			"It is used to load the prototype when loading the scene.");

		static char szLoadingListName[MAX_PATH] = "";
		ImGui::InputText("FileName", szLoadingListName, MAX_PATH, ImGuiInputTextFlags_NoUndoRedo); ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7 / 7.0f, 0.8f, 0.8f));

		if (ImGui::Button("Save"))
			ImGui::OpenPopup("Save LoadingList");

		ImGui::PopStyleColor(3);

		if (ImGui::BeginPopupModal("Save LoadingList", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Do you want to Save data?\n");
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			if (ImGui::Button("OK", ImVec2(100, 0)))
			{
				SaveLoadingList("../../Resource/SaveData/SaveStage/" + std::string(szLoadingListName));
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopStyleColor(3);


			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			if (ImGui::Button("Cancel", ImVec2(100, 0)))
				ImGui::CloseCurrentPopup();
			ImGui::PopStyleColor(3);
			ImGui::EndPopup();
		}

		///////////////////////////////////////////

		ImGui::TextColored(ImVec4(0.1f, 0.f, 3.f, 1.f), "Object Info"); ImGui::SameLine();
		HelpMarker("ex) ***.json\n" "Saves the Size, Rotation, Position and Option values");


		static char szObjInfo [MAX_PATH] = "";
		ImGui::InputText("FileName", szObjInfo, MAX_PATH, ImGuiInputTextFlags_NoUndoRedo); ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7 / 7.0f, 0.8f, 0.8f));

		if (ImGui::Button(" Save "))
			ImGui::OpenPopup("Save ObjInfo");
		ImGui::PopStyleColor(3);

		if (ImGui::BeginPopupModal("Save ObjInfo", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Do you want to Save data?\n");
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			if (ImGui::Button("OK", ImVec2(100, 0)))
			{
				SaveLoadingList("../../Resource/SaveData/SaveStage/" + std::string(szObjInfo));
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopStyleColor(3);


			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			if (ImGui::Button("Cancel", ImVec2(100, 0)))
				ImGui::CloseCurrentPopup();
			ImGui::PopStyleColor(3);
			ImGui::EndPopup();
		}
	}
}

