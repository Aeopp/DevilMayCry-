#include "stdafx.h"
#include "..\Header\TestAnimationObject.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"
#include <filesystem>


void TestAnimationObject::Free()
{
}
std::string TestAnimationObject::GetName()
{
	return "TestObject";
};

TestAnimationObject* TestAnimationObject::Create()
{
	return new TestAnimationObject{};
}


void TestAnimationObject::RenderForwardAlphaBlendImplementation(
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
			const auto& VtxBufDesc = SharedSubset->GetVertexBufferDesc();
			SharedSubset->BindProperty(TextureType::DIFFUSE, 0u, "ALBM0Map", _ImplInfo.Fx);
			SharedSubset->BindProperty(TextureType::NORMALS, 0u, "NRMR0Map", _ImplInfo.Fx);
			SharedSubset->Render(_ImplInfo.Fx);
		}
	}
};

void TestAnimationObject::RenderReady()
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

HRESULT TestAnimationObject::Ready()
{
	SetRenderEnable(true);
	ENGINE::RenderProperty _InitRenderProp;
	_InitRenderProp.bRender = true;
	_InitRenderProp._Order = ENGINE::RenderProperty::Order::ForwardAlphaBlend;
	RenderInterface::Initialize(_InitRenderProp);

	_ShaderInfo.ForwardAlphaBlendShader = Resources::Load<ENGINE::Shader>(L"..\\..\\Resource\\Shader\\ForwardAlphaBlendSK.hlsl");
	_SkeletonMesh = Resources::Load<ENGINE::SkeletonMesh>(L"..\\..\\Resource\\Mesh\\Dynamic\\Player.fbx");
	_SkeletonMesh->EnablePrevVTF();

	auto InitTransform = AddComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.001,0.001,0.001 });
	
	/* 애니메이션에 따라서 콜백이 필요한 경우 설정. 
		return true 인 경우 콜백 하고나서 삭제
		return false 인 경우 콜백 하고 나서 다음 루프때도 다시 콜백.
	 */
	ENGINE::AnimNotify _Notify{};
	_Notify.Event[0.5] = [this]() {  Log("0.5 Sec Call");  return true; };
	_Notify.Event[0.9] = [this]() {  Log("0.9 Sec Call");  return false; };

	// 에디터의 도움을 받고싶은 오브젝트들 Raw 포인터로 푸시.
	PushEditEntity(_SkeletonMesh.get());
	PushEditEntity(InitTransform.lock().get());
	PushEditEntity(_ShaderInfo.ForwardAlphaBlendShader.get());

	return S_OK;
};

HRESULT TestAnimationObject::Awake()
{
	return S_OK;
}

HRESULT TestAnimationObject::Start()
{
	return S_OK;
}

UINT TestAnimationObject::Update(const float _fDeltaTime)
{
	_SkeletonMesh->Update(_fDeltaTime);
	

	return 0;
}

UINT TestAnimationObject::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

void TestAnimationObject::Editor()
{
	GameObject::Editor();
	if (bEdit)
	{
		// 에디터 .... 
	}
}


void TestAnimationObject::OnEnable()
{

}

void TestAnimationObject::OnDisable()
{

}
