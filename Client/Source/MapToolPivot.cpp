#include "stdafx.h"
#include "MapToolPivot.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"
void MapToolPivot::Free()
{
}


std::string MapToolPivot::GetName()
{
	return "Pivot";
}

MapToolPivot* MapToolPivot::Create()
{
    return new MapToolPivot{};
}

HRESULT MapToolPivot::Ready()
{
	// 렌더를 수행해야하는 오브젝트라고 (렌더러에 등록 가능 ) 알림.
	// 렌더 인터페이스 상속받지 않았다면 키지마세요.
	SetRenderEnable(true);

	// 렌더 정보 초기화 ...
	ENGINE::RenderProperty _InitRenderProp;
	// 이값을 런타임에 바꾸면 렌더를 켜고 끌수 있음. 
	_InitRenderProp.bRender = true;
	// 넘겨준 패스에서는 렌더링 호출 보장 . 
	_InitRenderProp.RenderOrders =
	{
		RenderProperty::Order::ForwardAlphaBlend,
		RenderProperty::Order::Debug
	};
	RenderInterface::Initialize(_InitRenderProp);
	// 


	// 렌더링 패스와 쉐이더 매칭 . 쉐이더 매칭이 안되면 렌더링을 못함.
	_ShaderInfo.RegistShader(
		RenderProperty::Order::ForwardAlphaBlend,
		L"..\\..\\Resource\\Shader\\ForwardAlphaBlend.hlsl", {});
	_ShaderInfo.RegistShader(
		RenderProperty::Order::Debug,
		L"..\\..\\Resource\\Shader\\Debug.hlsl", {});

	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::ForwardAlphaBlend).get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::Debug).get());
	// 

	// 스태틱 메쉬 로딩
	_StaticMesh = Resources::Load<ENGINE::StaticMesh>(L"../../Resource/Mesh/Maptool/Pivot2.FBX");
	PushEditEntity(_StaticMesh.get());

	// 트랜스폼 초기화 .. 
	auto InitTransform = AddComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.001,0.001,0.001 });
	PushEditEntity(InitTransform.lock().get());

	// 에디터의 도움을 받고싶은 오브젝트들 Raw 포인터로 푸시.
	// PushEditEntity(_ShaderInfo.ForwardAlphaBlendShader.get());


    return S_OK;
}

HRESULT MapToolPivot::Awake()
{
    return S_OK;
}

HRESULT MapToolPivot::Start()
{
    return S_OK;
}

UINT MapToolPivot::Update(const float _fDeltaTime)
{
	//_SkeletonMesh->Update(_fDeltaTime);
    return 0;
}

UINT MapToolPivot::LateUpdate(const float _fDeltaTime)
{
    return 0;
}

void MapToolPivot::OnEnable()
{
}

void MapToolPivot::OnDisable()
{
}

void MapToolPivot::Editor()
{
	GameObject::Editor();
	if (bEdit)
	{
		ImGui::Begin("Begin!!");
		{
			ImGui::Text("Flaot : %3.3f", 7.707f);
			ImGui::BulletText("Flaot : %3.3f", 7.707f);

			static bool bCheck = false;
			if (ImGui::Checkbox("Check !", &bCheck))
			{

			}

			static bool Radio1 = false;
			static bool Radio2 = false;


			static int e = 0;
			ImGui::RadioButton("radio a", &e, 0); 
			ImGui::SameLine();
			ImGui::RadioButton("radio b", &e, 1); 
			ImGui::Separator();
			ImGui::RadioButton("radio c", &e, 2);



			if (ImGui::TreeNode("Tree"))
			{
				ImGui::Text("Tree Text...");

				if (ImGui::TreeNode("Children"))
				{
					ImGui::Text("Children Text...");

					int curitem = 0;

					std::vector<std::string> FbxFileName;

					const char* itmenames[5] = { "Apple" , "Banana", "....", "......","Orange"};

					if (ImGui::ListBox("ListBox !!", &curitem, itmenames, 5))
					{
						int a = 0;
					}

					ImGui::TreePop();
				}
				ImGui::TreePop();
			}

			
		}
		ImGui::End();
	}

	
}

void MapToolPivot::RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = _StaticMesh->GetNumSubset();
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = _StaticMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			const auto& VtxBufDesc = SharedSubset->GetVertexBufferDesc();
			SharedSubset->BindProperty(TextureType::DIFFUSE, 0u, "ALBM0Map", _ImplInfo.Fx);
			SharedSubset->BindProperty(TextureType::NORMALS, 0u, "NRMR0Map", _ImplInfo.Fx);
			SharedSubset->Render(_ImplInfo.Fx);
		}
	}
}

void MapToolPivot::RenderDebugImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = _StaticMesh->GetNumSubset();
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = _StaticMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			SharedSubset->Render(_ImplInfo.Fx);
		}
	}
}


void MapToolPivot::RenderReady()
{
	auto _WeakTransform = GetComponent<ENGINE::Transform>();
	if (auto _SpTransform = _WeakTransform.lock();
		_SpTransform)
	{
		_RenderProperty.bRender = true;
		ENGINE::RenderInterface::UpdateInfo _UpdateInfo{};
		_UpdateInfo.World = _SpTransform->GetWorldMatrix();
		RenderVariableBind(_UpdateInfo);
	}
}
