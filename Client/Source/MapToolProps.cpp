#include "stdafx.h"
#include "..\Header\MapToolProps.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"

MapToolProps* MapToolProps::Create()
{
	return new MapToolProps{};
}

void MapToolProps::Free()
{
}

HRESULT MapToolProps::Ready()
{
	m_iPropsID = -1;
	auto InitTransform = AddComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.01,0.01,0.01 });

	return S_OK;
}

HRESULT MapToolProps::Awake()
{
	return S_OK;
}

HRESULT MapToolProps::Start()
{
	SetRenderEnable(true);

	ENGINE::RenderProperty _InitRenderProp;
	_InitRenderProp.bRender = true;
	_InitRenderProp._Order = ENGINE::RenderProperty::Order::ForwardAlphaBlend;
	RenderInterface::Initialize(_InitRenderProp);

	_ShaderInfo.ForwardAlphaBlendShader = Resources::Load<ENGINE::Shader>(L"..\\..\\Resource\\Shader\\ForwardAlphaBlend.hlsl");
	_ShaderInfo.GBufferShader = Resources::Load<ENGINE::Shader>(L"..\\..\\Resource\\Shader\\GBuffer.hlsl");

	auto InitTransform = GetComponent<ENGINE::Transform>();


	// 경로값 확인 
	if (m_strFilePath.empty())
	{
		PRINT_LOG(L"Failed! m_strFilePath.empty()", __FUNCTIONW__);
		return E_FAIL;
	}
	_TCHAR test[MAX_PATH];
	lstrcpy(test, m_strFilePath.c_str());
	_StaticMesh = Resources::Load<ENGINE::StaticMesh>(test);

	ENGINE::AnimNotify _Notify{};

	_Notify.Event[0.5] = [this]() {  Log("0.5 !! ");  return true; };
	_Notify.Event[0.9] = [this]() {  Log("0.9 !! ");  return true; };

	PushEditEntity(_StaticMesh.get());
	PushEditEntity(InitTransform.lock().get());
	PushEditEntity(_ShaderInfo.ForwardAlphaBlendShader.get());
	PushEditEntity(_ShaderInfo.GBufferShader.get());

	return S_OK;
}

UINT MapToolProps::Update(const float _fDeltaTime)
{
	return 0;
}

UINT MapToolProps::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

void MapToolProps::OnEnable()
{
}

void MapToolProps::OnDisable()
{
}

void MapToolProps::RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = _StaticMesh->GetNumSubset();
	//_SkeletonMesh->BindVTF(_ImplInfo.Fx);
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = _StaticMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			_ImplInfo.Fx->SetFloatArray("LightDirection", Renderer::GetInstance()->TestDirectionLight, 3u);
			const auto& VtxBufDesc = SharedSubset->GetVertexBufferDesc();
			SharedSubset->BindProperty(TextureType::DIFFUSE, 0u, "ALBM0Map", _ImplInfo.Fx);
			SharedSubset->BindProperty(TextureType::NORMALS, 0u, "NRMR0Map", _ImplInfo.Fx);
			SharedSubset->Render(_ImplInfo.Fx);
		}
	}
}

void MapToolProps::RenderGBufferImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = _StaticMesh->GetNumSubset();
	//_SkeletonMesh->BindVTF(_ImplInfo.Fx);
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

void MapToolProps::RenderReady()
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
