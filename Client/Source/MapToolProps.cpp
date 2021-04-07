#include "stdafx.h"
#include "..\Header\MapToolProps.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"
#include <iostream>
#include "StaticMesh.h"


std::string MapToolProps::GetName()
{
	return "MapProps";
}

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
	ZeroMemory(m_bOption, sizeof(bool) * (int)ePropsOption::End);
	auto InitTransform = AddComponent<ENGINE::Transform>();
	if (auto SpTransform = InitTransform.lock() ;
		SpTransform)
	{
		SpTransform->SetScale({ 0.01,0.01,0.01 });
		PushEditEntity(SpTransform.get());
	}

	SetRenderEnable(true);

	ENGINE::RenderProperty _InitRenderProp;
	_InitRenderProp.bRender = true;

	_InitRenderProp.RenderOrders =
	{
		RenderProperty::Order::ForwardAlphaBlend ,
		RenderProperty::Order::Debug
	};
	RenderInterface::Initialize(_InitRenderProp);

	_ShaderInfo.RegistShader(
		RenderProperty::Order::ForwardAlphaBlend,
		L"..\\..\\Resource\\Shader\\ForwardAlphaBlend.hlsl", {});
	_ShaderInfo.RegistShader(
		RenderProperty::Order::Debug,
		L"..\\..\\Resource\\Shader\\Debug.hlsl", {});
	if ( auto _Shader = _ShaderInfo.GetShader(RenderProperty::Order::ForwardAlphaBlend))
	{
		PushEditEntity(_Shader.get());
	}
	if (auto _Shader = _ShaderInfo.GetShader(RenderProperty::Order::Debug))
	{
		PushEditEntity(_Shader.get());
	};

	/*_StaticMesh = Resources::Load<ENGINE::StaticMesh>(
		L"..\\..\\Resource\\Mesh\\Static\\Sphere.fbx");*/

	if (_StaticMesh)
	{
		PushEditEntity(_StaticMesh.get());
	}
	
	return S_OK;
}

HRESULT MapToolProps::Awake()
{
	return S_OK;
}

HRESULT MapToolProps::Start()
{
	return S_OK;
}

UINT MapToolProps::Update(const float _fDeltaTime)
{
	return 0;
};

UINT MapToolProps::LateUpdate(const float _fDeltaTime)
{
	return 0;
};

void MapToolProps::OnEnable()
{

};

void MapToolProps::OnDisable()
{

};

void MapToolProps::RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo)
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

void MapToolProps::RenderDebugImplementation(const ImplementationInfo& _ImplInfo)
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
	};
}; 

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
