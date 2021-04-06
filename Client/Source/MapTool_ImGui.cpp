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
		if (ImGui::Button("Props Select",ImVec2(400,20)))
			SelectFile();
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
	m_pPivot.lock()->Get_Trans().lock()->SetPosition(vPivotPos);

	ImGui::End();
}

void MapTool::ShowCameraOption()
{
	ImGui::Begin("Hellow Camera"); 
	ImGui::SetWindowPos(ImVec2(g_nWndCX - 350.f, g_nWndCY -320.f));
	ImGui::SetWindowSize(ImVec2(350.f, 120.f));
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

	ImVec4 fTextColor = m_bReadyNameTable ? ImVec4(1.0f, 0.0f, 1.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImGui::TextColored(fTextColor, m_bReadyNameTable ? "TRUE " : "FALSE");
	ImGui::SameLine();

	if (ImGui::Button("New"))
		ImGui::OpenPopup("New Table?");

	if (ImGui::BeginPopupModal("New Table?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Do you want to overwrite the data?\n");
		
		if (ImGui::Button("OK", ImVec2(100, 0)))
		{ 
			m_bReadyNameTable = NewFBXNameTable(L"../../SaveFile/SaveData/PropsNameTable.json");
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(100, 0)))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		m_bReadyNameTable = LoadFBXnametable(L"../../SaveFile/SaveData/PropsNameTable.json");
	}
}



void MapTool::BaseMapCreateGroup()
{
	if(ImGui::CollapsingHeader("==============BaseMapCreate=============", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Need BaseMap"); ImGui::SameLine();
		HelpMarker("Only one file can be loaded"); ImGui::SameLine();
		if (m_pBaseMap.expired())
		{
			if (ImGui::Button("Load BaseMap"))
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
		}
		else
		{
			if (ImGui::Button("Delete", ImVec2(150, 20)))
			{
				m_pBaseMap.lock()->Destroy(ConvertGameObjPtr(m_pBaseMap));
				m_pBaseMap.reset();
			}
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
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "Select Props : %s \tID: %d", convertToString(m_strSelectName).c_str(), m_iTableID);
				ImGui::Text("Create location");
				ImGui::RadioButton("PeekingPos", (int*)&m_eCreateOption, (int)eCreatePosition::PeekingPos);ImGui::SameLine();
				ImGui::RadioButton("PivotPos", (int*)&m_eCreateOption, (int)eCreatePosition::PivotPos); ImGui::SameLine();
				HelpMarker("When in creation mode,If PivotPos Mode press the space bar to create an object");
			}
		}


		if (m_eCreateOption == eCreatePosition::PeekingPos)
		{
			if (m_pBaseMap.expired())
			{
				m_bCreateLock = true;
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "Not Exist Base Map");
			}
			else
			{
				m_bCreateLock = false;
			}
		}
		else
		{
			m_bCreateLock = false;
		}


		/// <summary>
		/// Peeking Type
		/// </summary>
		ImGui::Text("PeekingType");
		ImGui::RadioButton("Single", (int*)&m_ePeekType, (int)ePeekingType::Single);

		if (m_eWorkType != eWorkOption::Modify)
		{
			//Modity 아닐시 멀티파킹 아니면 그냥 계속 멀티피킹 리스트 초기화하는걸로
			ClearMultiPeeking();
			m_ePeekType = ePeekingType::Single;
		}
		else
		{
			ImGui::SameLine();
			ImGui::RadioButton("Multy", (int*)&m_ePeekType, (int)ePeekingType::Multi); ImGui::SameLine();
			HelpMarker("The number of selected objects is displayed.\n"
				"Multiple objects can be modified at the same time.");
		}

		/// <summary>
		/// 피킹 타입 설정 후 검사 및 초기화 처리 피킹된 오브젝트 간단하게 표시 
		/// </summary>
		/// 
		if (m_ePeekType == ePeekingType::Single)
		{
			ClearMultiPeeking();//싱글타입으로 교체시
			ImGui::Text("Peeking Object Name : "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", m_strPeekingName.empty() ? "NULL" : convertToString(m_strPeekingName).c_str());
		}
		else //멀티 피킹시 
		{
			//현재 피킹 구현이 안되있음으로 막음 
			PRINT_LOG(L"Waring", L"Not Ready peeking ");
			m_ePeekType = ePeekingType::Single;
			return;
			/////////

			//멀티 피킹 카운팅 표시 
			ImGui::Text("Peeking Object Cnt : "); 
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "%d", m_listMultiPeeking.size());
		}
	}
}

//트렌스폼 수정관련 기능 그룹 
void MapTool::TransFormCtrlGroup()
{

	//싱글 피킹상태일떄 피킹대상이 없는 경우 or 멀티 피킹상태시 멀티피킹 갯수가 0 
	if (m_ePeekType == ePeekingType::Single &&  m_pCurSelectObj.expired()
		|| m_ePeekType == ePeekingType::Multi && m_listMultiPeeking.empty())
		return;

	if (ImGui::CollapsingHeader("==============TransForm=============", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Drag Sensitivity"); ImGui::SameLine();
		HelpMarker("Edit value Drag Power");
		static float fSensitivety = 0.001f;

		// 시작

		if (m_ePeekType == ePeekingType::Single)
		{
			float fScale = m_pCurSelectObj.lock()->Get_Trans().lock()->GetScale().x;  // obj pos Get 
			vec3 vRot = m_pCurSelectObj.lock()->Get_Trans().lock()->GetRotation(); // obj pos Get 
			vec3 vPos = m_pCurSelectObj.lock()->Get_Trans().lock()->GetPosition();  // obj pos Get 

			ImGui::InputFloat("", &fSensitivety, 0.001f); ImGui::SameLine();
			HelpMarker("Drag to edit value. Range 0~100 \n"
				"Hold SHIFT/ALT for faster/slower edit.\n"
				"Double-click or CTRL+click to input value.");

			fSensitivety = CLAMP(fSensitivety, 0, 100);

			ImGui::Text("Sacle    ");
			ImGui::SameLine();
			ImGui::DragFloat("        ", &fScale, fSensitivety);
			ImGui::Text("Rotate");
			ImGui::SameLine();
			ImGui::DragFloat3("     ", (float*)&vRot.x, fSensitivety);
			ImGui::Text("Positon"); ImGui::SameLine();
			ImGui::DragFloat3("   ", (float*)&vPos.x, fSensitivety);

			m_pCurSelectObj.lock()->Get_Trans().lock()->SetScale(Vector3(fScale, fScale, fScale));
			m_pCurSelectObj.lock()->Get_Trans().lock()->SetRotation(vRot);
			m_pCurSelectObj.lock()->Get_Trans().lock()->SetPosition(vPos);
		}
		else
		{
			//멀티 피킹 일떄는 
			//드래그  누적갑으로 
			float fScale = 0.f;
			vec3 vRot = vZero;// = vector(0,0,0)
			vec3 vPos = vZero;
			ImGui::InputFloat("", &fSensitivety, 0.001f); 

			ImGui::Text("Sacle    ");
			ImGui::SameLine();
			ImGui::DragFloat("        ", &fScale, fSensitivety);
			ImGui::Text("Rotate");
			ImGui::SameLine();
			ImGui::DragFloat3("     ", (float*)&vRot.x, fSensitivety);
			ImGui::Text("Positon"); ImGui::SameLine();
			ImGui::DragFloat3("   ", (float*)&vPos.x, fSensitivety);

			//0 상태에서 인풋값들 누적하는걸로 

			//for (auto& pObj : m_listMultiPeeking)
			//{
			//	//Trnasform add 함수 필요 
			//	pObj.lock()->Get_Trans().lock()->se

			//}
		}
	}
}

void MapTool::PropsOptionGroup()
{
	if (m_ePeekType !=  ePeekingType::Multi)
	{
		if (ImGui::CollapsingHeader("==============PropsOption=============", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Option");
			if (ImGui::Checkbox("Decoration", &m_bPropsOption[(int)MapToolProps::ePropsOption::Decoration]))
				ApplyPropsOption();
			ImGui::SameLine();

			if (ImGui::Checkbox("Floating", &m_bPropsOption[(int)MapToolProps::ePropsOption::Floating]))
				ApplyPropsOption();
			ImGui::SameLine();

			if (ImGui::Checkbox("Fixed", &m_bPropsOption[(int)MapToolProps::ePropsOption::Fixed]))
				ApplyPropsOption();
		}
	}
}

void MapTool::SaveFileGroup()
{
	if (ImGui::CollapsingHeader("==============SaveFile=============", ImGuiTreeNodeFlags_DefaultOpen))
	{


#pragma region Loading List 
		ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Loading List"); ImGui::SameLine();
		HelpMarker("ex) ***.json\n"
			"Saves the list of meshes used in the current scene. \n"
			"It is used to load the prototype when loading the scene."); ImGui::SameLine();

		if (ImGui::Button("Save"))
			ImGui::OpenPopup("Save LoadingList");

		if (ImGui::BeginPopupModal("Save LoadingList", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Do you want to Save data?\n");
			if (ImGui::Button("OK", ImVec2(100, 0)))
			{
				SaveLoadingList();
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(100, 0)))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
#pragma endregion


#pragma region PropsSave
		ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Object Info"); ImGui::SameLine();
		HelpMarker("ex) ***.json\n" "Saves the Scale, Rotation, Position and Option values");  ImGui::SameLine();


		if (ImGui::Button(" Save "))
			ImGui::OpenPopup("Save ObjInfo");
		ImGui::SameLine();


		if (ImGui::Button(" Load "))
			ImGui::OpenPopup("Load ObjInfo");

		static char szSavePropsPath[MAX_PATH] = "";
		//Popup
		if (ImGui::BeginPopupModal("Save ObjInfo", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Do you want to Save data?\n");
			if (ImGui::Button("OK", ImVec2(100, 0)))
			{
				SaveProps("../../Resource/SaveData/SaveStage/" + std::string(szSavePropsPath));
				strcpy_s(szSavePropsPath, "");
				ImGui::CloseCurrentPopup();
			}


			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(100, 0)))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}


		if (ImGui::BeginPopupModal("Load ObjInfo", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Do you want to Load data?\n");
			if (ImGui::Button("OK", ImVec2(100, 0)))
			{
				//SaveLoadingList("../../Resource/SaveData/SaveStage/" + std::string(szSavePropsPath));
				strcpy_s(szSavePropsPath, "");
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(100, 0)))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}


#pragma endregion

	}
}

