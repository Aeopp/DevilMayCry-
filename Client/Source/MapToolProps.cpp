#include "stdafx.h"
#include "..\Header\MapToolProps.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"

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

	return S_OK;
}

HRESULT MapToolProps::Awake()
{
	return S_OK;
}

HRESULT MapToolProps::Start()
{

	ENGINE::RenderProperty _InitRenderProp;
	_InitRenderProp.bRender = true;
	_InitRenderProp._Order = ENGINE::RenderProperty::Order::ForwardAlphaBlend;
	SetRenderEnable(true);


	const std::filesystem::path ShaderPath
	{
		L"..\\..\\Resource\\Shader\\ForwardAlphaBlend.hlsl"
	};

	RenderInterface::Initialize(_InitRenderProp, ShaderPath);


	//세터로 받은 fbx 검사 
	if (m_strFilePath.empty())
	{
		PRINT_LOG(L"Failed! m_strFilePath.empty()", __FUNCTIONW__);
		return E_FAIL;
	}
	const std::filesystem::path FbxPath {m_strFilePath};

	_StaticMesh = Resources::Load<ENGINE::StaticMesh>(FbxPath);
	if (!_StaticMesh)
	{
		PRINT_LOG(L"Failed!", __FUNCTIONW__);
	}
	auto InitTransform = AddComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.01f,0.01f,0.01f });

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
	for (UINT SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = _StaticMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			const auto& VtxBufDesc = SharedSubset->GetVertexBufferDesc();
			auto Diffuse0 = SharedSubset->GetMaterial().
				GetTexture(TextureType::DIFFUSE, 0u);

			if (Diffuse0)
			{
				_ImplInfo.Fx->SetTexture("ALBMMap", Diffuse0->GetTexture());
			}

			g_pDevice->SetVertexDeclaration(VtxBufDesc.pVertexDecl);
			g_pDevice->SetStreamSource(0u, SharedSubset->GetVertexBuffer(),
				0u,
				VtxBufDesc.nStride);
			g_pDevice->SetIndices(SharedSubset->GetIndexBuffer());
			_ImplInfo.Fx->CommitChanges();
			g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				0u, 0u, VtxBufDesc.nNumVertices, 0u, VtxBufDesc.nNumFaces);
		}
	}
}

void MapToolProps::RenderReady()
{
	auto _WeakTransform = GetComponent<ENGINE::Transform>();
	if (auto _SpTransform = _WeakTransform.lock();
		_SpTransform)
	{
		ENGINE::RenderInterface::UpdateInfo _UpdateInfo{};
		_UpdateInfo.bRender = true;
		_UpdateInfo.World = _SpTransform->GetWorldMatrix();
		RenderVariableBind(_UpdateInfo);
	}
}
