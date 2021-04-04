#include "stdafx.h"
#include "MapTool.h"

#include "TestObject.h"

#include <fstream>
#include <ostream>
#include <iostream>

/*
ImGui �ƴ� �Լ��� ����
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


HRESULT MapTool::LoadScene()
{
	Scene::LoadScene();

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0, 0, 0, 255);
	m_iPeekingCnt = 0;
	m_iTableID = 0;
	m_strSelectName = L"";
	m_fFOV = D3DXToRadian(90.f);
	UpdateProj();
	D3DXMatrixIdentity(&m_matCameraWorld);
	m_vCameraSpeed = { 5.f,3.f };
	m_vCameraPos = vZero;
	m_vRot = vZero;
	AddGameObject<TestObject>();


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

HRESULT MapTool::Update(const float _fDeltaTime)
{
	Scene::Update(_fDeltaTime);

	ShowMapTool();
	ShowPivotControl();
	HotKey();
	CameraControl(_fDeltaTime);
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
	auto convertToString = [](const std::wstring& w) {
		std::string Convert;
		Convert.assign(std::begin(w), std::end(w));
		return Convert;
	};

	using namespace rapidjson;
	std::filesystem::path TargetPath = "..\\..\\Resource\\SaveData\\PropsNameTable.json";
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
	auto convertToWstring = [](const std::string& s) {
		std::wstring Convert;
		Convert.assign(std::begin(s), std::end(s));
		return Convert;
	};

	using namespace rapidjson;
	std::ifstream Is{ "../../Resource/SaveData/PropsNameTable.json" };
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

	Value& TableVal = doc["NameTable"]; //���� ������Ʈ ���̺������� 
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

void MapTool::SaveObjInfo(const std::string& pPath)
{
}


void MapTool::CreateMeshNameTable(std::wstring strStartPath)
{
	WIN32_FIND_DATA tFindData;
	std::wstring tCurPath = strStartPath + L"/*.*";
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
	return;
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
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFOV, (float)g_nWndCX / g_nWndCY, 0.1f, 500.f);
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

	//if (Input::GetMouse(MOUSEBUTTON::DIM_R))
	//{
	//	if(Input::GetMouseMove(MOUSEAXIS::DIM_Y))
	//}
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
		ApplyPropsOption();
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
	//��ŷ Ÿ�Կ� �б� ������ ��ŷ ī��Ʈ ���� 
	if (m_ePeekType == ePeekingType::Single)
	{

	}
	else
	{

	}

}