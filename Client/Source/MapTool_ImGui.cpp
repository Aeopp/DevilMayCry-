#include "stdafx.h"
#include "MapTool.h"
#include "MapToolProps.h"
#include <fstream>
#include <ostream>
#include <iostream>
#include <commdlg.h>
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
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1 / 7.0f, 0.8f, 0.8f));
		if (ImGui::Button("Props Select",ImVec2(400,20)))
		{
			SelectFile();
		}
		ImGui::PopStyleColor(3);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1 / 7.0f, 0.8f, 0.8f));
		if(ImGui::Button("InterationObject Select", ImVec2(400, 20)))
		{
	
		}
		ImGui::PopStyleColor(3);


		//PeekingOption
		PeekingOptionGroup();
		TransFormCtrlGroup();
		PropsOptionGroup();
		SaveFileGroup();
	}

	ImGui::End();
}

void MapTool::ShowPivotOption()
{
	if (m_pPivot.expired())
	{
		ImGui::End();
		return;
	}
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

	 vec3 vPivotPos = m_pPivot.lock()->Get_Trans().lock()->GetPosition();  // pivot pos Get 
	 if (ImGui::DragFloat3("", (float*)&vPivotPos.x, fSensitivety))
		 std::cout << vPivotPos.x << std::endl;
	m_pPivot.lock()->Get_Trans().lock()->SetPosition(vPivotPos);

	ImGui::End();
}

void MapTool::ShowCameraOption()
{
	ImGui::Begin("Hellow Camera"); 
	ImGui::SetWindowPos(ImVec2(g_nWndCX - 350.f, g_nWndCY -300.f));
	ImGui::SetWindowSize(ImVec2(350.f, 100.f));
	m_bHoveredMaptool[(int)eWindowID::Camera] = ImGui::IsWindowHovered();

	ImGui::DragFloat2("MoveSpeed", m_vCameraSpeed, 1.f, 0.f, 200.f); ImGui::SameLine();
	HelpMarker("Move Key WSAD , 1 parameter x,z Axis Speed  2 y Axis Speed");

	ImGui::DragFloat("AngleSpeed", &m_fCameraAngSpeed, 1.f, 0.f, 200.f); ImGui::SameLine();
	HelpMarker("Press Mouse RBnt + Mouse Move  -> Angle Control" );

	if (ImGui::DragFloat("FOV", &m_fFOV, 0.1f, 10.f, 120.f))
		UpdateProj();
	

	ImGui::End();
}

void MapTool::NameTableGroup()
{
	ImGui::Text("NameTable"); ImGui::SameLine();
	HelpMarker("Must be Exist NameTable"); ImGui::SameLine();
	ImGui::Text("NameTable Exist :"); ImGui::SameLine();

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
		ImGui::Text("Need BaseMap"); ImGui::SameLine();
		HelpMarker("Only one file can be loaded"); ImGui::SameLine();
		if (m_pBaseMap.expired())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
			if (ImGui::Button("Load", ImVec2(150, 20)))
			{
				TCHAR   szPropsPath[MAX_PATH] = L"";
				OPENFILENAME open;
				memset(&open, 0, sizeof(OPENFILENAME));
				open.lStructSize = sizeof(OPENFILENAME);
				open.hwndOwner = g_hWnd;
				open.lpstrFilter = L"All Files(*.*)\0*.*\0";
				open.nMaxFile = nFileNameMaxLen;
				open.lpstrFile = szPropsPath;
				if (0 != GetOpenFileName(&open))
				{
					LoadBaseMap(szPropsPath);
				}
			}
			ImGui::PopStyleColor(3);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3 / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3 / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3 / 7.0f, 0.8f, 0.8f));
			if (ImGui::Button("Delete", ImVec2(150, 20)))
			{
				m_pBaseMap.lock()->Destroy(ConvertGameObjPtr(m_pBaseMap));
				m_pBaseMap.reset();
			}
			ImGui::PopStyleColor(3);
		}

	}
}

void MapTool::PeekingOptionGroup()
{
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
				ImGui::TextColored(ImVec4(0, 0, 0, 1), "Select Props : %s \tID: %d", convertToString(m_strSelectName).c_str(), m_iTableID);
				ImGui::Text("Create location");
				if (ImGui::RadioButton("PeekingPos", (int*)&m_eCreateOption, (int)eCreatePosition::PeekingPos))
				ImGui::SameLine();
				if (ImGui::RadioButton("PivotPos", (int*)&m_eCreateOption, (int)eCreatePosition::PivotPos))
				ImGui::SameLine();
				HelpMarker("When in creation mode, press the space bar to create an object");
			}
		}

		if (m_eCreateOption == eCreatePosition::PeekingPos)
		{
			if (m_pBaseMap.expired())
			{
				m_bCreateLock = false;
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "Not Exist Base Map");
			}
			else
			{
				m_bCreateLock = true;
			}
		}
		else
		{
			m_bCreateLock = true;
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
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", m_strPeekingName.empty() ? "NULL" : convertToString(m_strPeekingName).c_str());
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
	if (m_pCurSelectObj.expired())
		return;

	if (ImGui::CollapsingHeader("==============TransForm=============", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Drag Sensitivity"); ImGui::SameLine();
		HelpMarker("Edit value Drag Power");
		static float fSensitivety = 0.1f;
		 vec3 TestScale = m_pCurSelectObj.lock()->Get_Trans().lock()->GetScale();  // obj pos Get 
		 vec3 TestRot = m_pCurSelectObj.lock()->Get_Trans().lock()->GetRotation(); // obj pos Get 
		 vec3 TestPos = m_pCurSelectObj.lock()->Get_Trans().lock()->GetPosition();  // obj pos Get 

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


#pragma region Loading List 
		ImGui::TextColored(ImVec4(0.3f, 0.f, 0.f, 1.f), "Loading List"); ImGui::SameLine();
		HelpMarker("ex) ***.json\n"
			"Saves the list of meshes used in the current scene. \n"
			"It is used to load the prototype when loading the scene."); ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7 / 7.0f, 0.8f, 0.8f));

		if (ImGui::Button("Save"))
			ImGui::OpenPopup("Save LoadingList");

		ImGui::PopStyleColor(3);

		static char szLoadingListName[MAX_PATH] = "";
		ImGui::InputText("FileName", szLoadingListName, MAX_PATH, ImGuiInputTextFlags_NoUndoRedo); 


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
#pragma endregion


#pragma region PropsSave


		ImGui::TextColored(ImVec4(0.1f, 0.f, 3.f, 1.f), "Object Info"); ImGui::SameLine();
		HelpMarker("ex) ***.json\n" "Saves the Scale, Rotation, Position and Option values");  ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7 / 7.0f, 0.8f, 0.8f));
		if (ImGui::Button(" Save "))
			ImGui::OpenPopup("Save ObjInfo");
		ImGui::SameLine();
		ImGui::PopStyleColor(3);


		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3/ 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3 / 7.0f, 0.8f, 0.8f));
		if (ImGui::Button(" Load "))
			ImGui::OpenPopup("Load ObjInfo");
		ImGui::PopStyleColor(3);

		static char szSavePropsPath[MAX_PATH] = "";
		ImGui::InputText("FileName", szSavePropsPath, MAX_PATH, ImGuiInputTextFlags_NoUndoRedo); ImGui::SameLine();

		//Popup
		if (ImGui::BeginPopupModal("Save ObjInfo", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Do you want to Save data?\n");
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			
			if (ImGui::Button("OK", ImVec2(100, 0)))
			{
				SaveProps("../../Resource/SaveData/SaveStage/" + std::string(szSavePropsPath));
				strcpy_s(szSavePropsPath, "");
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


		if (ImGui::BeginPopupModal("Load ObjInfo", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Do you want to Load data?\n");
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(33 / 255.f, 33 / 255.f, 33 / 255.f, 1.f));
			if (ImGui::Button("OK", ImVec2(100, 0)))
			{
				SaveLoadingList("../../Resource/SaveData/SaveStage/" + std::string(szSavePropsPath));
				strcpy_s(szSavePropsPath, "");
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


#pragma endregion

	}
}

