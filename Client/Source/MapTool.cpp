#include "stdafx.h"
#include "MapTool.h"

#include <iostream>
#include <io.h>

using namespace std;

MapTool::MapTool()
	:m_iPeekCnt(0)
	, m_iPeekType(0)
	, m_fPivotMoveSpeed(0.1f)
	, m_bReadyNameTable(false)
{
	ZeroMemory(m_bPropsOption, sizeof(bool) * (int)ePropsOption::End);
}

void MapTool::Free()
{
	Scene::Free();
}

void MapTool::ApplyPropsOption()
{
	if (m_iPeekType == (int)ePeekingType::Single)
	{
		for (int i = 0; i < (int)ePropsOption::End; ++i)
		{
			//m_pSelectObj->SetOptino(i , m_bPropsOption[i]);
		}
	}
	else
	{
		//for (auto& pObj : m_pListMultiSelectObj)
		//{
		//	for (int i = 0; i << ePropsOption::End; ++i)
		//	{
		//		//pObj->SetOptino(i , m_bPropsOption[i]);
		//	}
		//}
	}
}

MapTool* MapTool::Create()
{
	MapTool* pInstance = new MapTool();
	return pInstance;
}


void MapTool::PivotControl()
{
	vec3 vPivotPos; // =  get Position

	if (Input::GetKeyDown(VK_LEFT)) // x Axis minus
		vPivotPos.x += -m_fPivotMoveSpeed;
	else if (Input::GetKeyDown(VK_RIGHT)) // x Axis plus 
		vPivotPos.x += m_fPivotMoveSpeed;
	else if (Input::GetKeyDown(VK_UP))  // z Axis plus
		vPivotPos.x += m_fPivotMoveSpeed;
	else if (Input::GetKeyDown(VK_DOWN)) // z Axis minus
		vPivotPos.x += m_fPivotMoveSpeed;
	else if (Input::GetKeyDown(VK_HOME))// y Axis plus
		vPivotPos.x += m_fPivotMoveSpeed;
	else if (Input::GetKeyDown(VK_END))	 // y Axis minus
		vPivotPos.x += m_fPivotMoveSpeed;
	//pivot->getTrans()->SetPos(vPos)
}

void MapTool::MouseInPut()
{
	//피킹 타입에 분기 때리고 피킹 카운트 제어 
	if (m_iPeekType == (int)ePeekingType::Single)
	{

	}
	else
	{

	}

}

void MapTool::CreateMeshNameTable(wstring strStartPath)
{
	WIN32_FIND_DATA tFindData;
	wstring tCurPath = strStartPath + L"/*.*";
	HANDLE hFind;
	hFind = FindFirstFile(tCurPath.c_str(), &tFindData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return;
	}
	BOOL bContinue = false;
	do
	{
		if (TEXT('.') == tFindData.cFileName[0])
		{
			bContinue = FindNextFile(hFind, &tFindData);
			continue;
		}
		else if (FILE_ATTRIBUTE_DIRECTORY == tFindData.dwFileAttributes)
		{
			CreateMeshNameTable(strStartPath + L"/" + tFindData.cFileName);
		}
		else
		{
			if (FILE_ATTRIBUTE_SYSTEM == tFindData.dwFileAttributes)
			{
				bContinue = FindNextFile(hFind, &tFindData);
				continue;
			};
			PATHINFO tInfo; 
			tInfo.sFileLocation = strStartPath;
			tInfo.sFileName = tFindData.cFileName;
			m_mapFBXNameTable.emplace(m_iTableID++, tInfo);
		}
		bContinue = FindNextFile(hFind, &tFindData);
	} while (bContinue);

	FindClose(hFind);
	return ;
}

void MapTool::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

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

	//NameTable
	NameTableGroup();
	//balse
	BaseMapCreateGroup();
	


	if(ImGui::CollapsingHeader("Props Tree"))
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


	
	ImGui::End();
}

void MapTool::ShowPivotControl()
{

	ImGui::Begin("Hellow Pivot"); ImGui::SameLine();
	ImGui::SetWindowPos(ImVec2(g_nWndCX- 350.f, g_nWndCY - 200.f));
	ImGui::SetWindowSize(ImVec2(350, 200));
	
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



HRESULT MapTool::LoadScene()
{
	Scene::LoadScene();

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0, 0, 0, 255);

	m_iPeekCnt = 0;
	m_iTableID = 0;
	CreateMeshNameTable(L"../../Resource/Map");
	m_mapFBXNameTable;
	return S_OK;
}

HRESULT MapTool::Awake()
{
	Scene::Awake();

	return S_OK;
}

HRESULT MapTool::Start()
{
	Scene::Start();

	return S_OK;
}

void MapTool::HotKey()
{
	if (Input::GetKey(DIK_R))
		m_eWorkType = eWorkOption::Create;
	if (Input::GetKey(DIK_T))
		m_eWorkType = eWorkOption::Delete;
	if (Input::GetKey(DIK_Y))
		m_eWorkType = eWorkOption::Modify;
	if (Input::GetKey(DIK_SPACE))
		ApplyPropsOption();
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
			m_bReadyNameTable = NewFBXNameTable(L"../../Resource/SaveData/PropsNameTable.tsv");
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
		LoadFBXnametable(L"../../Resource/SaveData/PropsNameTable.tsv");
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
		ImGui::InputText("    ", szBaseMapPath, MAX_PATH, ImGuiInputTextFlags_NoUndoRedo); ImGui::SameLine();
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
	if(ImGui::CollapsingHeader("==============PeekingOption=============",ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("WorkType");
		ImGui::RadioButton("Create", (int*)&m_eWorkType, (int)eWorkOption::Create); ImGui::SameLine();
		ImGui::RadioButton("Delete", (int*)&m_eWorkType, (int)eWorkOption::Delete); ImGui::SameLine();
		ImGui::RadioButton("Modyfy", (int*)&m_eWorkType, (int)eWorkOption::Modify); ImGui::SameLine();
		HelpMarker("Hot Key [R] [T] [Y] \n If you want to modify it, in the hotkey function");


		if (m_eWorkType == eWorkOption::Create)
		{
			ImGui::Text("Create location");
			ImGui::RadioButton("PeekingPos", (int*)&m_eCreateOption, (int)eCreatePosition::PeekingPos);
			ImGui::SameLine();
			ImGui::RadioButton("PivotPos", (int*)&m_eCreateOption, (int)eCreatePosition::PivotPos);
		}

		ImGui::Text("PeekingType");
		ImGui::RadioButton("Single", &m_iPeekType, (int)ePeekingType::Single);
		ImGui::SameLine();
		ImGui::RadioButton("Multi", &m_iPeekType, (int)ePeekingType::Multi);

		if (m_eWorkType != eWorkOption::Modify)
			m_iPeekType = (int)ePeekingType::Single;
	

		static char szObjName[64] = "Peeking obj Fbx Name";

		if (m_iPeekType == (int)ePeekingType::Single)
		{
			ImGui::Text("Peeking Object Name : "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", szObjName);
		}
		else
		{
			ImGui::Text("Peeking Object Cnt: %d", m_iPeekCnt);
		}
	}
}

void MapTool::TransFormCtrlGroup()
{
	if(ImGui::CollapsingHeader("==============TransForm=============", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Drag Sensitivity"); ImGui::SameLine();
		HelpMarker("Edit value Drag Power"); ImGui::SameLine();
		HelpMarker("Drag to edit value.\n"
			"Hold SHIFT/ALT for faster/slower edit.\n"
			"Double-click or CTRL+click to input value.");
		static float fSensitivety = 0.1f;
		static vec3 TestScale = vZero;  // obj pos Get 
		static vec3 TestRot = vZero;  // obj pos Get 
		static vec3 TestPos = vZero;  // obj pos Get 

		ImGui::InputFloat("", &fSensitivety, 0.1f);
		fSensitivety = CLAMP(fSensitivety, 0, 100);
		ImGui::Text("Sacle");
		ImGui::SameLine();
		ImGui::DragFloat3(".", (float*)&TestScale.x, fSensitivety);
		ImGui::Text("Rotate");
		ImGui::SameLine();
		ImGui::DragFloat3(" ", (float*)&TestRot.x, fSensitivety);
		ImGui::Text("Positon"); ImGui::SameLine();
		ImGui::DragFloat3("   ", (float*)&TestPos.x, fSensitivety);
	}
}

void MapTool::PropsOptionGroup()
{
	if (m_iPeekType != (int)ePeekingType::Multi)
	{
		static bool bOpen = true;
		if(ImGui::CollapsingHeader("==============PropsOption=============",ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Option");
			ImGui::Checkbox("Decoration", &m_bPropsOption[(int)ePropsOption::Decoration]); ImGui::SameLine();
			ImGui::Checkbox("Floating", &m_bPropsOption[(int)ePropsOption::Floating]); ImGui::SameLine();
			ImGui::Checkbox("Fixed", &m_bPropsOption[(int)ePropsOption::Fixed]); ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));

			if (ImGui::Button("Apply"))
			{
				ApplyPropsOption();
			}
			ImGui::PopStyleColor(3); ImGui::SameLine();
			HelpMarker("Hotkey Space");
		}
	}
}

bool MapTool::NewFBXNameTable(const _TCHAR* pPath)
{
	/// <summary>
	/// 식별번호 \t 파일경로 \t 파일명
	/// </summary>
	FILE* pFile = nullptr;
	errno_t err;

	TCHAR szWord[MAX_PATH + 1] = TEXT("");
	err = _tfopen_s(&pFile, pPath, TEXT("w,ccs=UTF-8")); // 한글도 가능 
	if (pFile == nullptr)
	{
		pFile = nullptr;
		PRINT_LOG(TEXT("Faild"), __FUNCTIONW__);
		return false;
	}
	CreateMeshNameTable(pPath);

	for (size_t i = 0; i < m_mapFBXNameTable.size(); ++i)
	{

		_ftprintf(pFile, L"%d\t", (int)i);
		lstrcpy(szWord, m_mapFBXNameTable[i].sFileLocation.c_str());
		_ftprintf(pFile, L"%s\t", szWord);
		lstrcpy(szWord, m_mapFBXNameTable[i].sFileName.c_str());
		_ftprintf(pFile, L"%s\n", szWord);
	}
	if (!err && pFile != NULL)
		fclose(pFile);
	return true;
}

bool MapTool::LoadFBXnametable(const _TCHAR* pPath)
{

	FILE* pFile = nullptr;
	errno_t err;
	PATHINFO tTemp;
	TCHAR szWord[MAX_PATH + 1] = TEXT("");
	err = _tfopen_s(&pFile, pPath, TEXT("r,ccs=UTF-8")); // 한글도 가능 
	if (pFile == nullptr)
	{
		pFile = nullptr;
		PRINT_LOG(TEXT("Faild"), __FUNCTIONW__);
		return false;
	}
	while (EOF != _ftscanf_s(pFile, TEXT("%s"), szWord, MAX_PATH))
	{
		int  i = _ttoi(szWord);
		_ftscanf_s(pFile, TEXT("%s"), szWord, MAX_PATH);
		m_mapFBXNameTable.emplace(szWord);
	}

	return false;
}

HRESULT MapTool::Update(const float _fDeltaTime)
{
	Scene::Update(_fDeltaTime);

	ShowMapTool();
	ShowPivotControl();
	HotKey();

	return S_OK;
}

HRESULT MapTool::LateUpdate(const float _fDeltaTime)
{
	Scene::LateUpdate(_fDeltaTime);

	return S_OK;
}
