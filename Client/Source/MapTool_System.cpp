#include "stdafx.h"
#include "MapTool.h"

#include "TestObject.h"
#include "MapToolPivot.h"
#include <fstream>
#include <ostream>
#include <iostream>

#include <commdlg.h>

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
	ZeroMemory(m_bPropsOption, sizeof(bool) * (int)MapToolProps::ePropsOption::End);
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
//��ŷ�Լ����� ������ ������ �־������ UI Ŭ���� ������Ʈ ���� �ϸ� �� 
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

	std::filesystem::path rootPath = std::filesystem::absolute("./"); //�ַ�� ���丮 ���  �������� 

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
		m_strSelectFBXName = szPropsName;
		bool bFindID = false;
		m_iTableID = ERR_ID;

		//���̺��Ʈ���� value�� ������ Ű���� ã�� ...
		for (auto pair : m_mapFBXNameTable)
		{
			if (!lstrcmpW(m_strSelectFBXName.c_str(), pair.second.sFileName.c_str()))
			{
				m_iTableID = pair.first; // �����ؼ� �����Ҷ� �̰� m_iTableID�� ���ͷ� �־��� 
				bFindID = true;
				break;
			}
		}

		//Ȥ�� ���ҽ� ���� �߰��س��� ���̺� ��Ʈ�� �����ɷ� �ε�������� ��ã���� Ȯ�ο� 
		if (bFindID == false)
		{
			PRINT_LOG(L"File Name Not exist NameTable", __FUNCTIONW__);
			return;
		}
	}

	SetCurrentDirectory(rootPath.wstring().c_str()); // �����Ѱɷ� ��ü 
}

void MapTool::ClearMultiPeeking()
{
	if (!m_listMultiPeeking.empty())
	{
		for (auto& pObj : m_listMultiPeeking)
			pObj.lock()->m_bMultiPeekPeeking = false;

		m_listMultiPeeking.clear();
	}
}

HRESULT MapTool::LoadScene()
{
	Scene::LoadScene();
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0, 0, 0, 255);
	m_iPeekingCnt = 0;
	m_iTableID = 0;
	m_strSelectFBXName = L"";
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

	ShowMapTool(); //���� 
	ShowPivotOption(); // �Ǻ� 
	ShowCameraOption(); // ī�޶� ui ������ 
	HotKey(); // ����Ű ���� 
	CameraControl(_fDeltaTime);
	PivotControl(_fDeltaTime);
	return S_OK;
}

HRESULT MapTool::LateUpdate(const float _fDeltaTime)
{
	Scene::LateUpdate(_fDeltaTime);
	UpdateView(); //����� ��ġ�� �Է� 
	return S_OK;
}

bool MapTool::NewFBXNameTable(const _TCHAR* pPath)
{
	// new �� �ʿ� ���ڳ�. �̰͵� ���丮 â���� �ҰŸ� .. 
	/*if(!std::filesystem::exists(pPath))
	{
		PRINT_LOG(L"Error Path", __FUNCTIONW__);
		return false;
	}*/

	m_mapFBXNameTable.clear();
	if (FAILED(CreateMeshNameTable(PROPSPATH)))
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

	//���̺� ���� üũ 
	if (!std::filesystem::exists(pPath))
	{
		PRINT_LOG(L"Error Path", __FUNCTIONW__);
		return false;
	}

	m_mapFBXNameTable.clear();
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

void MapTool::SaveProps()
{
	//Get Path
	TCHAR   szPropsPath[MAX_PATH] = L"";
	TCHAR   szPropsName[MAX_PATH] = L"";
	std::filesystem::path rootPath = std::filesystem::absolute("./"); //��Ʈ ��� ������ 
	OPENFILENAME open;
	memset(&open, 0, sizeof(OPENFILENAME));
	open.lStructSize = sizeof(OPENFILENAME);
	open.hwndOwner = g_hWnd;
	open.lpstrFilter = L"Json Files(*.json)\0*.json\0";
	open.nMaxFile = nFileNameMaxLen;
	open.nMaxFileTitle = nFileNameMaxLen;
	open.lpstrFile = szPropsPath;
	open.lpstrFileTitle = szPropsName;
	if (0 != GetSaveFileName(&open)) {}; //������ 
	SetCurrentDirectory(rootPath.wstring().c_str()); // open ���� ���丮��� ����Ǵϱ� ������ �̸� ���� �����Ѱ� ����

	// �� ©���ϱ� json ���� ��Ծ ���� �ٽ� ¥�ּ� 
	//Save json
	//m_mapObjDatas ���� ������ ID , list<GameOjbect >���� 
	// �� ��ȸ�ϸ鼭 ID ���� �ϰ� list ��ȸ�ϸ鼭 transform ���� ����  bool �� ���� 
	using namespace rapidjson;
	std::filesystem::path TargetPath = std::filesystem::relative(szPropsPath);;
	TargetPath += L".json";
	StringBuffer StrBuf;
	PrettyWriter<StringBuffer> js(StrBuf);

	js.StartObject();
	{
		js.Key("LoadingList");
		js.StartArray();
		{
			for (auto& pair : m_mapObjDatas) // pair second is list 
			{
				js.Key("ID");
				js.Uint(pair.first); // �������� Ű������ ���� �̸���ſ� ��� ���̺� Ž���� ���ڿ����� ���� �Ű��� 
				js.StartObject();
				{
					js.StartArray();
					{
						for (auto& pObj : pair.second) // list�� ���� Props ��ü 
						{
							js.Key("Scale"); 
							js.Double(pObj.lock()->Get_Trans().lock()->GetScale().x);

							js.Key("Rotate");
							js.StartArray();
							{
								js.Double(pObj.lock()->Get_Trans().lock()->GetRotation().x);
								js.Double(pObj.lock()->Get_Trans().lock()->GetRotation().y);
								js.Double(pObj.lock()->Get_Trans().lock()->GetRotation().z);
							}
							js.EndArray();
							js.Key("Position");
							js.StartArray();
							{
								js.Double(pObj.lock()->Get_Trans().lock()->GetPosition().x);
								js.Double(pObj.lock()->Get_Trans().lock()->GetPosition().y);
								js.Double(pObj.lock()->Get_Trans().lock()->GetPosition().z);
							}
							js.EndArray(); // end of positon
							// �⺻���� ���� ���� �� 
							js.Key("option");
							js.StartArray();
							for (int i = 0; i < (int)MapToolProps::ePropsOption::End; ++i)
								js.Bool(pObj.lock()->m_bOption[i]);
							js.EndArray();
						}
					}js.EndArray(); // end of id ojbect
				}js.EndObject();
			}
		}js.EndArray();
	}js.EndObject();
	std::ofstream Of{ TargetPath };
	Of << StrBuf.GetString();
}

HRESULT MapTool::LoadBaseMap()
{
	std::filesystem::path rootPath = std::filesystem::absolute("./"); //��Ʈ ��� ������ 
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
		m_pBaseMap = AddGameObject<MapToolProps>();
		m_pBaseMap.lock()->m_strFilePath = szPropsPath;
	}
	SetCurrentDirectory(rootPath.wstring().c_str()); // open ���� ���丮��� ����Ǵϱ� ������ �̸� ���� �����Ѱ� ����
	return S_OK;
}

void MapTool::CreateProps()
{
	//
	if (m_bCreateLock)
	{
		PRINT_LOG(L"Create Lock", L"Check Setting");
		return;
	}
	//�߰� �� ���̺��� ��� �� �־ fbx �ε��ϰ� ���ְ� ���̵� �־��ְ� 
	if (m_eCreateOption == eCreatePosition::PeekingPos)
	{
		Vector3 vPeekPos = vZero;
		//��ŷ ���� �߰� �� ���� 
		// ex m_pbBase.lock()->��ŷ���� ��ȯ 
		//m_pCurSelectObj = AddGameObject<MapToolProps>();
		//m_pCurSelectObj.lock()->SetFBXPath(m_mapFBXNameTable[m_iTableID].sFileLocation);
		//m_pCurSelectObj.lock()->m_iPropsID = m_iTableID;	
		//m_pCurSelectObj.lock()->Get_Trans().lock()->SetPos(vPeekPos);
	}
	else if(m_eCreateOption == eCreatePosition::PivotPos)
	{
		m_pCurSelectObj = AddGameObject<MapToolProps>();
		//������ġ�� �Ǻ����� ./  �����Ѱ� �ٷ�  ���� �� �� �ֵ��� �ϴ°� ���ϰ���?  
		m_pCurSelectObj.lock()->m_strFilePath = m_strPeekingName = m_mapFBXNameTable[m_iTableID].sFileLocation;
		m_pCurSelectObj.lock()->m_iPropsID = m_iTableID;
		m_pCurSelectObj.lock()->Get_Trans().lock()->SetPosition(m_pPivot.lock()->Get_Trans().lock()->GetPosition());
	}
	else
	{
		PRINT_LOG(L"CreatOption Not Select", L"Check Setting");
		return;
	}

	//map�� �ߺ�Ȯ�� 
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
	//d2d �� ���� Ž�� ���� 
	if (std::filesystem::exists(strStartPath))
	{
		PRINT_LOG(L"Erorr Path Check plz", __FUNCTIONW__);
		return E_FAIL;
	}

	WIN32_FIND_DATA tFindData;
	std::wstring tCurPath = strStartPath + L"/*.*";
	int keyValue = 0;
	HANDLE hFind = FindFirstFile(tCurPath.c_str(), &tFindData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		PRINT_LOG(L"Error Path", __FUNCTIONW__);
		FindClose(hFind);
		return E_FAIL;
	}
	BOOL bContinue = false;
	do
	{
		if (TEXT('.') == tFindData.cFileName[0]) // . , .. �н� 
		{
			bContinue = FindNextFile(hFind, &tFindData);
			continue;
		}
		else if (FILE_ATTRIBUTE_DIRECTORY == tFindData.dwFileAttributes) // ���丮�� ��� 
		{
			CreateMeshNameTable(strStartPath + L'/' + tFindData.cFileName);
		}
		else
		{
			if (FILE_ATTRIBUTE_SYSTEM == tFindData.dwFileAttributes) // �ý��� ���� �н� 
			{
				bContinue = FindNextFile(hFind, &tFindData);
				continue;
			};

			//�̻��ϰ� ImGui ini ��  ������ ��   ���߿� fbx �� �������� ������ �ؽ��� �������� ������  tga �� ���� �� 
			std::wstring CheckExtension = tFindData.cFileName;
			if (std::wstring::npos != CheckExtension.find(L".tga") || std::wstring::npos != CheckExtension.find(L".ini")) 
			{
				bContinue = FindNextFile(hFind, &tFindData);
				continue;
			}

			PATHINFO tInfo;
			tInfo.sFileLocation = strStartPath +L"/" +  tFindData.cFileName;
			tInfo.sFileName = tFindData.cFileName;
			m_mapFBXNameTable.emplace(keyValue++, tInfo);
		}

		bContinue = FindNextFile(hFind, &tFindData);
	} while (bContinue);

	FindClose(hFind);
	return S_OK;
}

void MapTool::ApplyPropsOption()
{
	//check box ���� ������ ȣ�� �Ǵ� �Լ� �׳� bool �������� 
	if (m_ePeekType == ePeekingType::Single)
	{
		for (int i = 0; i < (int)MapToolProps::ePropsOption::End; ++i)
		{
			m_pCurSelectObj.lock()->m_bOption[i] = m_bPropsOption[i];
		}
	}
	else // Multi
	{
		for (auto& pObj : m_listMultiPeeking)
		{
			for (int i = 0; i < (int)MapToolProps::ePropsOption::End; ++i)
			{
				pObj.lock()->m_bOption[i] = m_bPropsOption[i];
			}
		}
	}
}

void MapTool::UpdateProj()
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(m_fFOV), (float)g_nWndCX / g_nWndCY, 0.1f, 500.f);
	g_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void MapTool::UpdateView()
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

void MapTool::CameraControl(const float& _fDeltaTime)
{
	if (IsHoverUIWindow())
		return;

	Vector3 vLook(0.f, 0.f, 0.f);
	memcpy_s(&vLook, sizeof(Vector3), m_matCameraWorld.m[2], sizeof(Vector3));
	Vector3 vRight(0.f, 0.f, 0.f);
	memcpy_s(&vRight, sizeof(Vector3), m_matCameraWorld.m[0], sizeof(Vector3));
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
		m_vCameraPos += Vector3(0, -1, 0) * m_vCameraSpeed.y * _fDeltaTime;
	if (Input::GetKey(DIK_C))
		m_vCameraPos += Vector3(0, 1, 0) * m_vCameraSpeed.y * _fDeltaTime;

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
	if (Input::GetKeyDown(DIK_SPACE) && m_eWorkType == eWorkOption::Create && m_eCreateOption == eCreatePosition::PivotPos)
		CreateProps();
}

void MapTool::PivotControl(const float& fDeltaTime)
{
	Vector3 vPivotPos   = m_pPivot.lock()->Get_Trans().lock()->GetPosition();  
	Vector3 vLook(0.f, 0.f, 0.f);
	memcpy_s(&vLook, sizeof(Vector3), m_matCameraWorld.m[2], sizeof(Vector3));
	Vector3 vRight(0.f, 0.f, 0.f);
	memcpy_s(&vRight, sizeof(Vector3), m_matCameraWorld.m[0], sizeof(Vector3));
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vLook *= fDeltaTime;
	vRight *= fDeltaTime;
	// ī�޶� �� ����Ʈ ������ �����Ϸ��� y�� ���� �̻��ϰ� ������ y �� ���� 
	vLook.y = 0;
	vRight.y = 0;

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


void MapTool::MousePeeking()
{
	// ��ŷ��ü�� ���� �ȵ����� �뷫������ Ʋ�� �̷�  

	if (Input::GetMouse(MOUSEBUTTON::DIM_L)) // Ŭ���� 
	{
		switch (m_eWorkType) // Create Delete Modify �� ���� �۵� 
		{
		case MapTool::eWorkOption::Create:
			//if(m_pBaseMap.lock() -> ��ŷ gȣ�� )
			//{
			//	AddProps();
			//}
			break;
		case MapTool::eWorkOption::Delete:
			for (auto& pair : m_mapObjDatas)
			{
				for (auto& pObj : pair.second)
				{
					//if (pObj.lock()->��ŷ ȣ��)
					//{
					//	//pObj.lock()->Destroy()
						//�ɽ��� ������ ��Ƴ���  �� ������ 
					//	//ConvertGameObjPtr(x) static_cast<std::weak_ptr<GameObject>>(x) 
					//}
				}
			}
			break;
		case MapTool::eWorkOption::Modify:
			for (auto& pair : m_mapObjDatas)
			{
				for (auto& pObj : pair.second)
				{
					//if(pObj.lock()-> ��ŷ ȣ�� ) // ��ŷ���� 
					{
						if (m_ePeekType == ePeekingType::Single)
						{
							m_pCurSelectObj = pObj;
							//UI�� �ɼ��� ��ŷ�� ������Ʈ�ɷ� ����ȭ 
							for (int i = 0; i < (int)MapToolProps::ePropsOption::End; ++i)
								m_bPropsOption[i] = pObj.lock()->m_bOption[i];
						}
						else  // multi
						{
							if (pObj.lock()->m_bMultiPeekPeeking == false)
							{
								pObj.lock()->m_bMultiPeekPeeking = true;
							}
						}
					}
				}
			}
			break;
		case MapTool::eWorkOption::End:
			break;
		}
	}
}

