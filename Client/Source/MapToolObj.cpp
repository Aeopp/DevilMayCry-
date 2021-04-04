#include "stdafx.h"
#include "..\Header\MapToolObj.h"
#include "Subset.h"
#include "TextureType.h"

void MapToolObj::Free()
{
}

MapToolObj* MapToolObj::Create()
{
    return new MapToolObj{};
}

HRESULT MapToolObj::Ready()
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

	const std::filesystem::path FbxPath
	{
		L"../../Resource/MaptoolUtill/Pivot2.fbx"
	};
	_StaticMesh = Resources::Load<ENGINE::StaticMesh>(FbxPath);
	if (!_StaticMesh)
	{
		PRINT_LOG(L"Failed!", __FUNCTIONW__);
	}
	auto InitTransform = AddComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.1,0.1,0.1 });

    return S_OK;
}

HRESULT MapToolObj::Awake()
{
    return S_OK;
}

HRESULT MapToolObj::Start()
{
    return S_OK;
}

UINT MapToolObj::Update(const float _fDeltaTime)
{
    return 0;
}

UINT MapToolObj::LateUpdate(const float _fDeltaTime)
{
    return 0;
}

void MapToolObj::OnEnable()
{
}

void MapToolObj::OnDisable()
{
}

void MapToolObj::RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = _StaticMesh->GetNumSubset();
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
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

void MapToolObj::RenderReady()
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
