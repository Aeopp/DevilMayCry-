#include "stdafx.h"
#include "..\Header\TestObject.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"


void TestObject::Free()
{
}
std::string TestObject::GetName()
{
	return "TestObject";
}
;

TestObject* TestObject::Create()
{
	return new TestObject{};
}


void TestObject::RenderForwardAlphaBlendImplementation(
	const ImplementationInfo&_ImplInfo)
{
	const uint64 NumSubset = _SkeletonMesh->GetNumSubset();
	_SkeletonMesh->BindVTF(_ImplInfo.Fx);
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = _SkeletonMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			_ImplInfo.Fx->SetFloatArray("LightDirection", Renderer::GetInstance()->TestDirectionLight,
				3u);

			const auto& VtxBufDesc = SharedSubset->GetVertexBufferDesc();

			auto& CurMat = SharedSubset->GetMaterial();

			auto ALBM0Map=CurMat.GetTexture(TextureType::DIFFUSE, 0u);
			if (ALBM0Map)
			{
				_ImplInfo.Fx->SetTexture("ALBMMap", 
					ALBM0Map->GetTexture());
			}

			auto NRMR0Map = CurMat.GetTexture(TextureType::NORMALS,
				0u);
			if (NRMR0Map)
			{
				_ImplInfo.Fx->SetTexture("NRMRMap",
					NRMR0Map->GetTexture());
			}

			_ImplInfo.Fx->CommitChanges();
			SharedSubset->Render();
			ImGui::Text("MaxBonesRefPerVtx : %d ", VtxBufDesc.nMaxBonesRefPerVtx);

			//g_pDevice->SetVertexDeclaration(VtxBufDesc.pVertexDecl);
			//g_pDevice->SetStreamSource(0u, SharedSubset->GetVertexBuffer(),
			//	0u,
			//	VtxBufDesc.nStride);
			//g_pDevice->SetIndices(SharedSubset->GetIndexBuffer());
			//
			//g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			//	0u, 0u, VtxBufDesc.nNumVertices, 0u, VtxBufDesc.nNumFaces);

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
	auto InitTransform = AddComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.1,0.1,0.1 });
	_SkeletonMesh = Resources::Load<ENGINE::SkeletonMesh>(FbxPath);
	_SkeletonMesh->EnablePrevVTF();
	if (!_SkeletonMesh)
	{
		PRINT_LOG(L"Failed!", __FUNCTIONW__);
	}
	ENGINE::AnimNotify A{};
	A.Event[0.5] = [this]() {  PRINT_LOG(L"0.5!", L"0.5!");
							  return true; };
	A.Event[0.9] = [this]() {  PRINT_LOG(L"0.9!", L"0.9!");
	return true; };

	/*A.Event[0.3] = [this]() {  PRINT_LOG(L"0.1!", L"0.1!");
	* // return false 하면 반복도미...
	return false; };*/
	_SkeletonMesh->PlayAnimation("Air_Belatos_Twohandedsword",
		true, 1.0f, 0.25f ,A);
	PushEditEntity(_SkeletonMesh.get());
	PushEditEntity(InitTransform.lock().get());

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
		static Vector3 Rotation{0,0,0};
		if (ImGui::SliderAngle("Light Pitch", &Rotation.x) ||
			ImGui::SliderAngle("Light Yaw", &Rotation.y) ||
			ImGui::SliderAngle("Light Roll", &Rotation.z))
		{
			
			Renderer::GetInstance()->TestDirectionLight =
				FMath::Normalize(FMath::MulNormal(Vector3{ 0,0,1 }, FMath::Rotation(Rotation)));
		}
	}

	_SkeletonMesh->Update(_fDeltaTime);

	return 0;
}

UINT TestObject::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

void TestObject::Editor()
{
	GameObject::Editor();
}


void TestObject::OnEnable()
{

}

void TestObject::OnDisable()
{

}
