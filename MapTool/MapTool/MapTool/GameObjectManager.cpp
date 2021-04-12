#include "framework.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Transform.h"

GameObjectManager GameObjectManager::Instance;

GameObjectManager::GameObjectManager()
	: m_pSelected(nullptr)
{
	m_nCurrItem = -1;
}

GameObjectManager::~GameObjectManager()
{
	for (auto rPair : m_Container)
	{
		for (UINT i = 0; i < rPair.second.size(); ++i)
		{
			delete rPair.second[i];
		}
		rPair.second.clear();
		rPair.second.shrink_to_fit();
	}
	m_Container.clear();
}

void GameObjectManager::Update()
{
	if (Input::GetMouseDown(DIM_R))
	{
		GameObject* pPicking = Physics::Instance.RayCast();
		if (nullptr != pPicking)
		{
			m_pSelected = pPicking;

			for (UINT i = 0; i < m_Container[m_pSelected->m_sMesh].size(); ++i)
			{
				if (m_pSelected == m_Container[m_pSelected->m_sMesh][i])
				{
					m_nCurrItem = m_pSelected->m_nListIdx;
					break;
				}
			}
		}
		//ResourceManager::Instance.RayCast();
	}

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(51.f / 255.f, 51.f / 255.f, 51.f / 255.f, 1.f));

	ImGui::Begin("GameObject",nullptr,ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);
	ShowMenuBar();
	ShowGameObjectList();
	ImGui::PopStyleColor();
	ImGui::End();
}

void GameObjectManager::Render()
{
	for (auto& rPair : m_Container)
	{
		for (UINT i = 0; i < rPair.second.size(); ++i)
		{
			rPair.second[i]->Update();
			D3DXMATRIX matWorld = rPair.second[i]->m_pTransform->GetWorldMatrix();
			g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
			ResourceManager::Instance.Render(rPair.first);
		}
	}
}

void GameObjectManager::ShowMenuBar()
{
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(64.f / 255.f, 64.f / 255.f, 64.f / 255.f, 1.f));
	ImGui::BeginMenuBar();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ItemSpacing = ImVec2(0.f, 0.f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.f, 0.f, 0.f, 0.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(103.f / 255.f, 103.f / 255.f, 103.f / 255.f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.f, 0.f, 0.f, 0.f);
	style.Colors[ImGuiCol_Text] = ImVec4(194.f / 255.f, 194.f / 255.f, 194.f / 255.f, 1.f);

	ImGui::Button("SAVE");
	if (ImGui::IsItemClicked())
	{
		
	}

	ImGui::Button("LOAD");
	if (ImGui::IsItemClicked())
	{

	}

	ImGui::Button("DELETE");
	if (ImGui::IsItemClicked())
	{
		DeleteGameObject();
	}

	ImGui::PopStyleColor();
	ImGui::EndMenuBar();
}

void GameObjectManager::ShowGameObjectList()
{
	int nItemCount = 0;
	for (auto& rPair : m_Container)
	{
		nItemCount += rPair.second.size();
	}

	char** ppIndex;
	ppIndex = new char* [nItemCount];

	int nIndex = 0;
	for (auto& rPair : m_Container)
	{
		for (int i = 0; i < rPair.second.size(); ++i)
		{
			ppIndex[i] = new char[MAX_PATH];
			char szIndex[10] = {};
			_itoa_s(rPair.second[i]->m_nIndex, szIndex, 10, 10);

			std::string sTemp = rPair.first;
			sTemp += " / ";
			sTemp += szIndex;

			strcpy_s(ppIndex[i], sizeof(char) * MAX_PATH, sTemp.c_str());

			rPair.second[i]->m_nListIdx = nIndex++;
		}
	}

	if (ImGui::ListBox("##GameObjectList", &m_nCurrItem, ppIndex, nItemCount, nItemCount))
	{
		if (-1 != m_nCurrItem)
		{
			std::string sSelected = ppIndex[m_nCurrItem];
			size_t nToken = sSelected.find('/');

			std::string sKey = sSelected.substr(0, nToken - 1);
			std::string sIndex = sSelected.substr(nToken + 2, sSelected.length() - 1);

			m_pSelected = m_Container[sKey][atoi(sIndex.c_str())];
		}
	}
	//if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
	//{
	//	if (-1 != m_nCurrItem && m_pSelected)
	//	{
	//		Camera::SetCameraFocus(rPair.second[m_nCurrItem]->m_pTransform->GetPosition());
	//	}
	//}


	for (int i = 0; i < nItemCount; ++i)
	{
		delete[] ppIndex[i];
	}
	delete[] ppIndex;
	
	/*bool bSel = false;

	static int nSel = m_nCurrItem;
	for (auto& rPair : m_Container)
	{
		char** ppIndex;
		ppIndex = new char* [rPair.second.size()];

		ImGui::Text(rPair.first.c_str());
		for (int i = 0; i < rPair.second.size(); ++i)
		{
			ppIndex[i] = new char[10];
			_itoa_s(rPair.second[i]->m_nIndex, ppIndex[i], 10, 10);
		}
		std::string sLabel = "##" + rPair.first;

		if (bSel)
			nSel = -1;
		if (bSel = ImGui::ListBox(sLabel.c_str(), &nSel, ppIndex, rPair.second.size(), rPair.second.size()))
		{
			m_nCurrItem = nSel;
			if (-1 != m_nCurrItem && m_nCurrItem < rPair.second.size())
				m_pSelected = rPair.second[m_nCurrItem];

	
		}
		if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
		{
			if (-1 != m_nCurrItem && m_pSelected)
			{
				Camera::SetCameraFocus(rPair.second[m_nCurrItem]->m_pTransform->GetPosition());
			}
		}
		for (int i = 0; i < rPair.second.size(); ++i)
		{
			delete[] ppIndex[i];
		}
		delete[] ppIndex;
	}*/
}

void GameObjectManager::AddGameObject(std::string _sMeshID, PxShape* _pShape)
{
	//게임 오브젝트 동적 생성 및 초기화.
	GameObject* pInstance = new GameObject;

	pInstance->m_sMesh = _sMeshID;
	pInstance->CreateRigidActor(_pShape);
	//
	auto iterFind = m_Container.find(_sMeshID);

	if (m_Container.end() == iterFind)
	{
		iterFind = m_Container.emplace(_sMeshID, std::vector<GameObject*>()).first;
	}

	iterFind->second.push_back(pInstance);
}

void GameObjectManager::DeleteGameObject()
{
	if (nullptr == m_pSelected)
		return;

	std::string sKey = m_pSelected->m_sMesh;

	for (auto iter = m_Container[sKey].begin(); iter != m_Container[sKey].end(); ++iter)
	{
		if (*iter == m_pSelected)
		{
			m_Container[sKey].erase(iter);
			m_pSelected = nullptr;
			m_nCurrItem = -1;
			break;
		}
	}
}
