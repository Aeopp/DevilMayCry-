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
};

TestObject* TestObject::Create()
{
	return new TestObject{};
}


void TestObject::RenderForwardAlphaBlendImplementation(
	const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = _SkeletonMesh->GetNumSubset();
	_SkeletonMesh->BindVTF(_ImplInfo.Fx);
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = _SkeletonMesh->GetSubset(SubsetIdx);
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
};

void  TestObject::RenderGBufferImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = _SkeletonMesh->GetNumSubset();
	_SkeletonMesh->BindVTF(_ImplInfo.Fx);
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = _SkeletonMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			const auto& VtxBufDesc   = SharedSubset->GetVertexBufferDesc();
			SharedSubset->BindProperty(TextureType::DIFFUSE, 0u, "ALBM0Map", _ImplInfo.Fx);
			SharedSubset->BindProperty(TextureType::NORMALS, 0u, "NRMR0Map", _ImplInfo.Fx);
			SharedSubset->Render(_ImplInfo.Fx);
		}
	}
};


void TestObject::RenderReady()
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

HRESULT TestObject::Ready()
{
	SetRenderEnable(true);

	ENGINE::RenderProperty _InitRenderProp;
	_InitRenderProp.bRender = true;
	_InitRenderProp._Order = ENGINE::RenderProperty::Order::ForwardAlphaBlend;
	RenderInterface::Initialize(_InitRenderProp);

	_ShaderInfo.ForwardAlphaBlendShader = Resources::Load<ENGINE::Shader>(L"..\\..\\Resource\\Shader\\ForwardAlphaBlend.hlsl");
	_ShaderInfo.GBufferShader = Resources::Load<ENGINE::Shader>(L"..\\..\\Resource\\Shader\\GBuffer.hlsl");

	auto InitTransform = AddComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.1,0.1,0.1 });

	_SkeletonMesh = Resources::Load<ENGINE::SkeletonMesh>(L"..\\..\\Resource\\Mesh\\Static\\Bench_Ori.fbx");
	_SkeletonMesh->EnablePrevVTF();

	ENGINE::AnimNotify _Notify{};
	
	_Notify.Event[0.5] = [this]() {  Log("0.5 !! ");  return true; };
	_Notify.Event[0.9] = [this]() {  Log("0.9 !! ");  return true; };

	_SkeletonMesh->PlayAnimation("Air_Belatos_Twohandedsword",true,_Notify);
	PushEditEntity(_SkeletonMesh.get());
	PushEditEntity(InitTransform.lock().get());
	PushEditEntity(_ShaderInfo.ForwardAlphaBlendShader.get());
	PushEditEntity(_ShaderInfo.GBufferShader.get());

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
