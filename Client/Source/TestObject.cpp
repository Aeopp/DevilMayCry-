#include "stdafx.h"
#include "..\Header\TestObject.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"


void TestObject::Free()
{
};

TestObject* TestObject::Create()
{
	return new TestObject{};
}

void TestObject::RenderForwardAlphaBlendImplementation(const ImplementationInfo&_ImplInfo)
{
	const uint64 NumSubset = _SkeletonMesh->GetNumSubset();
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = _SkeletonMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			const auto& VtxBufDesc = SharedSubset->GetVertexBufferDesc();
			auto Diffuse0 = SharedSubset->GetMaterial().
				GetTexture(TextureType::DIFFUSE, 0u);


			// TEST 
			_ImplInfo.Fx->SetFloatArray("LightDirection", Renderer::GetInstance()->TestDirectionLight,
				3u);
			// 


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

void TestObject::RenderReady()
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

HRESULT TestObject::Ready()
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
		L"..\\..\\Resource\\Mesh\\Static\\Tester.fbx"
	};

	_SkeletonMesh = Resources::Load<ENGINE::SkeletonMesh>(FbxPath);
	if (!_SkeletonMesh)
	{
		PRINT_LOG(L"Failed!", __FUNCTIONW__);
	}
	auto InitTransform = AddComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.1,0.1,0.1 });
	return S_OK;
};

HRESULT TestObject::Awake()
{
	return S_OK;
}

HRESULT TestObject::Start()
{
	return S_OK;
}

UINT TestObject::Update(const float _fDeltaTime)
{
	if  ( auto Sptransform = GetComponent<ENGINE::Transform>().lock();
		   Sptransform)
	{ 
		{
			Vector3 SliderPosition = Sptransform->GetPosition();
			ImGui::SliderFloat3("Position", SliderPosition, -10.f, 10.f);
			Sptransform->SetPosition(SliderPosition);
		}

		{
			float AllScale = Sptransform->GetScale().x;
			ImGui::SliderFloat("All Scale", &AllScale, 0.001f, 100.f);
			Sptransform->SetScale({AllScale,AllScale,AllScale });
		}

		ImGui::DragFloat3("LightDirection", Renderer::GetInstance()->TestDirectionLight);
		Renderer::GetInstance()->TestDirectionLight =
			FMath::Normalize(Renderer::GetInstance()->TestDirectionLight);
	}
	
	return 0;
}

UINT TestObject::LateUpdate(const float _fDeltaTime)
{
	return 0;
}


void TestObject::OnEnable()
{
}

void TestObject::OnDisable()
{
}
