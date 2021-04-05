#include "stdafx.h"
#include "MapTool.h"

#include "TestObject.h"
#include "MapToolPivot.h"
#include <fstream>
#include <ostream>
#include <iostream>

#include <commdlg.h>

/*
ImGui 아닌 함수들 모음
*/
//#include "Renderer.h"

using namespace std;

MapTool::MapTool()
	:m_iPeekingCnt(0)
	, m_fPivotMoveSpeed(0.1f)
	, m_bReadyNameTable(false)
{
	ZeroMemory(m_bPropsOption, sizeof(bool) * (int)ePropsOption::End);
	ZeroMemory(m_bHoveredMaptool, sizeof(bool) * (int)eWindowID::End);
}

void MapTool::Free()
{
	Scene::Free();
}

MapTool* MapTool::Create()
{
	MapTool* pInstance = new MapTool();
	return pInstance;
}

bool MapTool::IsHoverUIWindow()
{
	for (int i = 0; i < (int)eWindowID::End; ++i)
		if (m_bHoveredMaptool[i])
			return true;
	return false;
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


void MapTool::SelectFile()
{
	TCHAR   szPropsPath[MAX_PATH] = L"";
	TCHAR   szPropsName[MAX_PATH] = L"";

	OPENFILENAME open;
	memset(&open, 0, sizeof(OPENFILENAME));
	open.lStructSize = sizeof(OPENFILENAME);
	open.hwndOwner = g_hWnd;
	open.lpstrFilter = L"All Files(*.*)\0*.*\0";
	open.nMaxFile = nFileNameMaxLen;
	open.nMaxFileTitle = nFileNameMaxLen;
	open.lpstrFile = szPropsPath;
	open.lpstrFileTitle = szPropsName;
	if (0 != GetOpenFileName(&open))
	{
		m_strSelectName = szPropsName;
		//check
		bool bFindID = false;
		m_iTableID = ERR_ID;

		//테이블시트에서 id 찾고 
		for (auto pair : m_mapFBXNameTable)
		{
			if (!lstrcmpW(m_strSelectName.c_str(), pair.second.sFileName.c_str()))
			{
				m_iTableID = pair.first;
				bFindID = true;
				break;
			}
		}
		if (bFindID == false)
		{
			PRINT_LOG(L"File Name Not exist NameTable", __FUNCTIONW__);
			return;
		}
	}
}

HRESULT MapTool::LoadScene()
{
	Scene::LoadScene();
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0, 0, 0, 255);
	m_iPeekingCnt = 0;
	m_iTableID = 0;
	m_strSelectName = L"";
	m_fFOV = 90.f;
	UpdateProj();
	D3DXMatrixIdentity(&m_matCameraWorld);
	m_vCameraSpeed = { 20.f,10.f };
	m_vCameraPos = vZero;
	m_vRot = vZero;
	m_fCameraAngSpeed = 100.f;
	m_fPivotMoveSpeed = 10.f;
	m_pPivot =  AddGameObject<MapToolPivot>();
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

std::wstring MapTool::convertToWstring(const std::string& s)
{
	std::wstring Convert;
	Convert.assign(std::begin(s), std::end(s));
	return Convert;
}

std::string MapTool::convertToString(const std::wstring& s)
{
	std::string Convert;
	Convert.assign(std::begin(s), std::end(s));
	return Convert;
}
HRESULT MapTool::Update(const float _fDeltaTime)
{
	Scene::Update(_fDeltaTime);

	ShowMapTool();
	ShowPivotOption();
	ShowCameraOption();
	HotKey();
	CameraControl(_fDeltaTime);
	PivotControl(_fDeltaTime);
	return S_OK;
}

HRESULT MapTool::LateUpdate(const float _fDeltaTime)
{
	Scene::LateUpdate(_fDeltaTime);
	UpdateView();
	return S_OK;
}

bool MapTool::NewFBXNameTable(const _TCHAR* pPath)
{
	if (FAILED(CreateMeshNameTable(TEXT(PROPSPATH))))
	{
		PRINT_LOG(L"Failed!", __FUNCTIONW__);
		return false;
	}

	using namespace rapidjson;
	std::filesystem::path TargetPath = pPath;
	StringBuffer StrBuf;
	PrettyWriter<StringBuffer> js(StrBuf);
	UINT iID = 0;
	js.StartObject();
	js.Key("NameTable");
	js.StartArray();
	{
		for (auto& pair : m_mapFBXNameTable)
		{
			js.StartObject();
			{
				js.Key("ID");
				js.Uint(pair.first);
				js.Key("FileLocation");
				js.String(convertToString(pair.second.sFileLocation).c_str());
				js.Key("FileName");
				js.String(convertToString(pair.second.sFileName).c_str());
			}
			js.EndObject();
		}
	}
	js.EndArray();
	js.EndObject();
	std::ofstream Of{ TargetPath };
	Of << StrBuf.GetString();

	return true;
}

bool MapTool::LoadFBXnametable(const _TCHAR* pPath)
{
	using namespace rapidjson;
	std::ifstream Is{ pPath };
	if (!Is.is_open())
	{
		PRINT_LOG(L"Error Open", __FUNCTIONW__);
		return false;
	}
	IStreamWrapper Isw(Is);
	Document doc;
	doc.ParseStream(Isw);
	if (doc.HasParseError())
	{
		PRINT_LOG(L"Error Parse", __FUNCTIONW__);
		return false;
	}

	Value& TableVal = doc["NameTable"]; //단일 오브젝트 세이브파일임 
	const auto& TableArr = TableVal.GetArray();
	PATHINFO tInfo;
	for (auto TableIter = TableArr.Begin(); TableIter != TableArr.End(); ++TableIter)
	{
		tInfo.sFileLocation = convertToWstring(TableIter->FindMember("FileLocation")->value.GetString());
		tInfo.sFileName = convertToWstring(TableIter->FindMember("FileName")->value.GetString());
		m_mapFBXNameTable.emplace(TableIter->FindMember("ID")->value.GetUint(), tInfo);
	}

	return true;
}

void MapTool::SaveLoadingList(const std::string& pPath)
{

}
void MapTool::SaveProps(const std::string& pPath)
{
}


HRESULT MapTool::LoadBaseMap(std::wstring strFilePath)
{
	m_pBaseMap = AddGameObject<MapToolProps>();
	m_pBaseMap.lock()->SetFBXPath(strFilePath);
	return S_OK;
}

void MapTool::AddProps()
{
	if (m_bCreateLock)
	{
		PRINT_LOG(L"Create Lock", L"Check Setting");
		return;
	}

	//추가 후 테이블에서 경로 값 넣어서 fbx 로드하게 해주고 아이디 넣어주고 
	if (m_eCreateOption == eCreatePosition::PeekingPos)
	{
		//피킹 지점 추가 후 삭제
		//m_pCurSelectObj = AddGameObject<MapToolProps>();
		//m_pCurSelectObj.lock()->SetFBXPath(m_mapFBXNameTable[m_iTableID].sFileLocation);
		//m_pCurSelectObj.lock()->m_iPropsID = m_iTableID;
	}
	else if(m_eCreateOption == eCreatePosition::PivotPos)
	{
		//생성위치를 피봇으로 
		m_pCurSelectObj = AddGameObject<MapToolProps>();
		m_pCurSelectObj.lock()->SetFBXPath(m_mapFBXNameTable[m_iTableID].sFileLocation);
		m_pCurSelectObj.lock()->m_iPropsID = m_iTableID;
		m_pCurSelectObj.lock()->Get_Trans().lock()->SetPosition(m_pPivot.lock()->Get_Trans().lock()->GetPosition());
	}
	else
	{
		PRINT_LOG(L"CreatOption Not Select", L"Check Setting");
		return;
	}

	//map에 중복확인 
	auto iterFind = m_mapObjDatas.find(m_iTableID);

	if (iterFind == m_mapObjDatas.end())
	{
		iterFind = m_mapObjDatas.emplace(m_iTableID, std::list<std::weak_ptr<MapToolProps>>()).first;
		iterFind->second.emplace_back(m_pCurSelectObj);
	}
	else
		iterFind->second.emplace_back(m_pCurSelectObj);
}


HRESULT MapTool::CreateMeshNameTable(std::wstring strStartPath)
{
	WIN32_FIND_DATA tFindData;
	std::wstring tCurPath = strStartPath + L"/*.*";
	HANDLE hFind;
	hFind = FindFirstFile(tCurPath.c_str(), &tFindData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return E_FAIL;
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

			// TGA  제외 
			std::wstring CheckExtension = tFindData.cFileName;
			if (std::wstring::npos != CheckExtension.find(L".tga") || std::wstring::npos != CheckExtension.find(L".ini")) // 나중에 텍스쳐 경로 지정되면 삭제  ini 이 계속 들어가있음 가끔 
			{
				bContinue = FindNextFile(hFind, &tFindData);
				continue;
			}
			
			//

			PATHINFO tInfo;

			tInfo.sFileLocation = strStartPath +L'/' +  tFindData.cFileName;
			tInfo.sFileName = tFindData.cFileName;
			m_mapFBXNameTable.emplace(m_iTableID++, tInfo);
		}
		bContinue = FindNextFile(hFind, &tFindData);
	} while (bContinue);

	FindClose(hFind);
	return S_OK;
}

void MapTool::ApplyPropsOption()
{
	if (m_ePeekType == ePeekingType::Single)
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

void MapTool::UpdateProj()
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(m_fFOV), (float)g_nWndCX / g_nWndCY, 0.1f, 500.f);
	g_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void MapTool::UpdateView()
{
	static bool t = true;

	if (Input::GetKey(DIK_F1))
		t = !t;

	if (t)
	{

	Matrix a;
	D3DXMatrixIdentity(&m_matCameraWorld);
	m_matCameraWorld *= (*D3DXMatrixRotationX(&a, D3DXToRadian(m_vRot.x)));
	m_matCameraWorld *= (*D3DXMatrixRotationY(&a, D3DXToRadian(m_vRot.y)));
	m_matCameraWorld *= (*D3DXMatrixRotationZ(&a, D3DXToRadian(m_vRot.z)));
	m_matCameraWorld *= (*D3DXMatrixTranslation(&a, m_vCameraPos.x,m_vCameraPos.y,m_vCameraPos.z));
	D3DXMatrixInverse(&m_matView, NULL, &m_matCameraWorld);
	g_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	}
	else
	{
		D3DXVECTOR3 vLook(0.f, 0.f, 0.f);
		memcpy_s(&vLook, sizeof(D3DXVECTOR3), m_matCameraWorld.m[2], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vLook, &vLook);

		Vector3 vAt = m_vCameraPos + vLook;
		Vector3 vUp(0, 1, 0);
		D3DXMatrixLookAtLH(&m_matView, &m_vCameraPos, &vAt, &vUp);
		g_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	}


}

void MapTool::CameraControl(const float& _fDeltaTime)
{
	if (IsHoverUIWindow())
		return;

	D3DXVECTOR3 vLook(0.f, 0.f, 0.f);
	memcpy_s(&vLook, sizeof(D3DXVECTOR3), m_matCameraWorld.m[2], sizeof(D3DXVECTOR3));
	D3DXVECTOR3 vRight(0.f, 0.f, 0.f);
	memcpy_s(&vRight, sizeof(D3DXVECTOR3), m_matCameraWorld.m[0], sizeof(D3DXVECTOR3));
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vLook*= _fDeltaTime;
	vRight*= _fDeltaTime;

	if (Input::GetKey(DIK_W))
		m_vCameraPos += vLook * m_vCameraSpeed.x;
	if (Input::GetKey(DIK_S))
		m_vCameraPos += -vLook * m_vCameraSpeed.x;
	if (Input::GetKey(DIK_A))
		m_vCameraPos += -vRight * m_vCameraSpeed.x;
	if (Input::GetKey(DIK_D))
		m_vCameraPos += vRight * m_vCameraSpeed.x;
	if (Input::GetKey(DIK_Z))
		m_vCameraPos += vec3(0, -1, 0) * m_vCameraSpeed.y * _fDeltaTime;
	if (Input::GetKey(DIK_C))
		m_vCameraPos += vec3(0, 1, 0) * m_vCameraSpeed.y * _fDeltaTime;

	if (Input::GetMouse(MOUSEBUTTON::DIM_R))
	{
		long	dwMouseMove = 0;

		if (dwMouseMove = Input::GetMouseMove(MOUSEAXIS::DIM_Y))
			m_vRot.x += dwMouseMove * m_fCameraAngSpeed * _fDeltaTime;
		if (dwMouseMove = Input::GetMouseMove(MOUSEAXIS::DIM_X))
			m_vRot.y += dwMouseMove * m_fCameraAngSpeed * _fDeltaTime;

		POINT	ptMouse{ g_nWndCX >> 1, g_nWndCY >> 1 };

		ClientToScreen(g_hWnd, &ptMouse);
		SetCursorPos(ptMouse.x, ptMouse.y);
	}
}

void MapTool::HotKey()
{
	if (Input::GetKey(DIK_R))
		m_eWorkType = eWorkOption::Create;
	if (Input::GetKey(DIK_T))
		m_eWorkType = eWorkOption::Delete;
	if (Input::GetKey(DIK_Y))
		m_eWorkType = eWorkOption::Modify;
	if (Input::GetKey(DIK_SPACE) && m_eWorkType == eWorkOption::Create && m_eCreateOption == eCreatePosition::PivotPos)
		AddProps();
}



void MapTool::PivotControl(const float& fDeltaTime)
{
	vec3 vPivotPos   = m_pPivot.lock()->Get_Trans().lock()->GetPosition();  
	D3DXVECTOR3 vLook(0.f, 0.f, 0.f);
	memcpy_s(&vLook, sizeof(D3DXVECTOR3), m_matCameraWorld.m[2], sizeof(D3DXVECTOR3));
	D3DXVECTOR3 vRight(0.f, 0.f, 0.f);
	memcpy_s(&vRight, sizeof(D3DXVECTOR3), m_matCameraWorld.m[0], sizeof(D3DXVECTOR3));
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vLook *= fDeltaTime;
	vRight *= fDeltaTime;

	if (Input::GetKey(DIK_LEFT)) // x Axis minus
	{
		vPivotPos += vRight *  -m_fPivotMoveSpeed;
		std::cout << vPivotPos.x << std::endl;
	}
	 if (Input::GetKey(DIK_RIGHT)) // x Axis plus 
		vPivotPos+= vRight * m_fPivotMoveSpeed;
	 if (Input::GetKey(DIK_UP))  // z Axis plus
		vPivotPos += vLook * m_fPivotMoveSpeed;
	 if (Input::GetKey(DIK_DOWN)) // z Axis minus
		vPivotPos += vLook *  -m_fPivotMoveSpeed;
	 if (Input::GetKey(DIK_HOME))// y Axis plus
		vPivotPos += Vector3(0,1,0) * m_fPivotMoveSpeed * fDeltaTime;
	 if (Input::GetKey(DIK_END))	 // y Axis minus
		vPivotPos+= Vector3(0, -1, 0) * m_fPivotMoveSpeed  * fDeltaTime;

	m_pPivot.lock()->Get_Trans().lock()->SetPosition(vPivotPos);
}


void MapTool::MouseInPut()
{
	//피킹 타입에 분기 때리고 피킹 카운트 제어 
	if (m_ePeekType == ePeekingType::Single)
	{

	}
	else
	{

	}
}

bool MapTool::ObjKeyFinder(const _TCHAR* pTag)
{

	bool bFindID = false;
	int  iTableID = -1;
	for (auto pair : m_mapFBXNameTable)
	{
		if (!lstrcmpW(m_strSelectName.c_str(), pair.second.sFileName.c_str()))
		{
			bFindID = true;
			break;
		}
	}
	if (bFindID == false)
	{
		PRINT_LOG(L"File Name Not exist NameTable", __FUNCTIONW__);
		return bFindID;
	}

	return false;
}