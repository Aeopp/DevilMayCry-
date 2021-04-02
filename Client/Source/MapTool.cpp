#include "stdafx.h"
#include "MapTool.h"

#include <iostream>
#include <io.h>

using namespace std;

MapTool::MapTool()
	:m_iPeekCnt(0)
	, m_iPeekType(0)
	, m_fPivotMoveSpeed(0.1f)
	, m_eWorkType(eWorkOption::e::End)
	, m_bReadyNameTable(false)
{
	ZeroMemory(m_bPropsOption, sizeof(bool) * ePropsOption::End);
}

void MapTool::Free()
{
	Scene::Free();
}
void  NewFBXNameTable(const _TCHAR* pPath)
{
	//OBJDATA tTemp;
	//FILE* pFile = nullptr;
	//TCHAR szWord[MAX_PATH + 1] = TEXT("");
	//if (0 != _tfopen_s(&pFile, pFilePath, TEXT("r,ccs=UTF-8")))
	//{
	//	MSG_BOX(L"failed Load");
	//	return E_FAIL;
	//}
	//_int iDeco, iFloat, iStat;
	//eItemCategory::eItemCategory eTag;
	//int i = 0;
	//_fgetts(szWord, MAX_PATH, pFile);
	//while (EOF != _ftscanf_s(pFile, TEXT("%s"), szWord, MAX_PATH))
	//{
	//	if (isdigit(szWord[0]))
	//	{
	//		tTemp.iItemID = _ttoi(szWord);
	//		_ftscanf_s(pFile, TEXT("%s"), szWord, MAX_PATH);
	//		tTemp.wstrTag = wstring(szWord);
	//		_ftscanf_s(pFile, TEXT("%f %f %f %d %d %d %d %f")
	//			, &tTemp.fSacle, &tTemp.fSize, &tTemp.fWeight
	//			, &iDeco, &iFloat, &iStat, &eTag, &tTemp.fInfoScale);
	//		tTemp.bDecoration = (i != iDeco);
	//		tTemp.bFloating = (i != iFloat);
	//		tTemp.bStationary = (i != iStat);
	//		tTemp.eCategory = eTag;
	//		_ftscanf_s(pFile, TEXT("%s\n"), szWord, MAX_PATH);
	//		tTemp.wstrName = wstring(szWord);
	//		m_vecDataTble.emplace_back(tTemp);
	//	}
	//}
	//fclose(pFile);


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


	ImGui::Begin("Hellow MapTool");
	ImGui::SetWindowPos(ImVec2(0, 10));
	ImGui::SetWindowSize(ImVec2(400, 600));



	ImGui::Text("NameTable"); ImGui::SameLine();
	HelpMarker("Must be Ready Name Table");ImGui::SameLine();
	ImGui::Text("NameTable Ready :"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), m_bReadyNameTable ? "TRUE " : "FALSE");
	ImGui::Text("BaseMap"); ImGui::SameLine();
	HelpMarker("Write FBX Path");
	static char szBaseMapPath[MAX_PATH] = "";
	ImGui::InputText("", szBaseMapPath,MAX_PATH); ImGui::SameLine();
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

	ImGui::Text("WorkType");
	ImGui::RadioButton("Create", (int*)&m_eWorkType, 0); ImGui::SameLine();
	ImGui::RadioButton("Delete", (int*)&m_eWorkType, 1); ImGui::SameLine();
	ImGui::RadioButton("Modyfy", (int*)&m_eWorkType, 2);



	ImGui::Text("PeekingType");
	ImGui::RadioButton("Single", &m_iPeekType, 0); 
	if (m_eWorkType == eWorkOption::Modify)
	{
		ImGui::SameLine();
		ImGui::RadioButton("Multi", &m_iPeekType, 1);
	}
	else
	{
		m_iPeekType = (int)ePeekingType::Single;
	}

	static char szObjName[64] = "TestName";

	if (m_iPeekType == (int)ePeekingType::Single)
	{
		ImGui::Text("Peeking Object Name : %s", szObjName);
	}
	else
	{
		ImGui::Text("Peeking Object Cnt: %d", m_iPeekCnt);
	}



	ImGui::Text("---------Transform---------\n\n");
	ImGui::Text("Drag Sensitivity"); ImGui::SameLine();
	HelpMarker("Edit value Drag Power"); ImGui::SameLine();
	HelpMarker(
		"Drag to edit value.\n"
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

	if (m_iPeekType != (int)ePeekingType::Multi)
	{
		ImGui::Text("Option");
		ImGui::Checkbox("Decoration", &m_bPropsOption[ePropsOption::Decoration]); ImGui::SameLine();
		ImGui::Checkbox("Floating", &m_bPropsOption[ePropsOption::Floating]); ImGui::SameLine();
		ImGui::Checkbox("Fixed", &m_bPropsOption[ePropsOption::Fixed]); ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));

		if (ImGui::Button("Apply"))
		{
			if (m_iPeekType == (int)ePeekingType::Single)
			{
				for (int i = 0; i << ePropsOption::End; ++i)
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
	}

	ImGui::Text("---------Save---------");
	static char  szSaveFileName[64];
	ImGui::InputText("_", szSaveFileName, 64);

	if (ImGui::Button("Save"))
		ImGui::OpenPopup("Check");

	if (ImGui::BeginPopup("Check"))
	{

		ImGui::SetWindowPos(ImVec2(g_nWndCX / 2.f - (200 / 2.f), g_nWndCY / 2.f - (100 / 2)));
		ImGui::SetWindowSize(ImVec2(200, 100));
		ImGui::Text("you want to save the current data?");

		if (ImGui::Button("Yes"))
		{
			//save Call
		}
		ImGui::SameLine(50);
		if (ImGui::Button("No"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor(3);


	
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
