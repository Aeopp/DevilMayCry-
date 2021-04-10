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
	//에베베
	return new TestAnimationObject{};
}


void TestAnimationObject::RenderGBufferImplementation(const ImplementationInfo& _ImplInfo)
{	
	const uint64 NumSubset = _SkeletonMesh->GetNumSubset();

	if (NumSubset > 0)
	{
		const auto& RenderInfo = GetRenderUpdateInfo();
		_ImplInfo.Fx->SetMatrix("World", &RenderInfo.World);
		_SkeletonMesh->BindVTF(_ImplInfo.Fx);
	}
	
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = _SkeletonMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			SharedSubset->BindProperty(TextureType::DIFFUSE, 0u, "ALBM0Map", _ImplInfo.Fx);
			SharedSubset->BindProperty(TextureType::NORMALS, 0u, "NRMR0Map", _ImplInfo.Fx);
			SharedSubset->Render(_ImplInfo.Fx);
		}
	};
}

void TestAnimationObject::RenderDebugImplementation(const ImplementationInfo& _ImplInfo)
{
	const auto& RenderInfo = GetRenderUpdateInfo();
	const uint64 NumSubset = _SkeletonMesh->GetNumSubset();
	
	if (NumSubset > 0)
	{
		const auto& RenderInfo = GetRenderUpdateInfo();
		_ImplInfo.Fx->SetMatrix("World", &RenderInfo.World);
		_SkeletonMesh->BindVTF(_ImplInfo.Fx);
	}
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = _SkeletonMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			SharedSubset->Render(_ImplInfo.Fx);
		}
	}
}

void TestAnimationObject::RenderDebugBoneImplementation(const ImplementationInfo& _ImplInfo)
{
	if (auto SpTransform = GetComponent<ENGINE::Transform>().lock();
		SpTransform)
	{
		const Matrix ScaleOffset = FMath::Scale({ 0.01,0.01 ,0.01 });
		const auto& RenderInfo = GetRenderUpdateInfo();
		_ImplInfo.Fx->SetMatrix("World", &RenderInfo.World);
		_SkeletonMesh->BoneDebugRender(SpTransform->GetWorldMatrix() ,_ImplInfo.Fx);
	}
}


void TestAnimationObject::RenderForwardAlphaBlendImplementation(
	const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = _SkeletonMesh->GetNumSubset();

	if (NumSubset > 0)
	{
		const auto& RenderInfo = GetRenderUpdateInfo();
		_ImplInfo.Fx->SetMatrix("World", &RenderInfo.World);
		_SkeletonMesh->BindVTF(_ImplInfo.Fx);
	}

	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = _SkeletonMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			if (SharedSubset->bRender)
			{
				const auto& VtxBufDesc = SharedSubset->GetVertexBufferDesc();
				SharedSubset->BindProperty(TextureType::DIFFUSE, 0u, "ALBM0Map", _ImplInfo.Fx);
				SharedSubset->BindProperty(TextureType::NORMALS, 0u, "NRMR0Map", _ImplInfo.Fx);
				SharedSubset->Render(_ImplInfo.Fx);
			}
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
	// 렌더를 수행해야하는 오브젝트라고 (렌더러에 등록 가능 ) 알림.
	// 렌더 인터페이스 상속받지 않았다면 키지마세요.
	SetRenderEnable(true);

	// 렌더 속성 전체 초기화 
	ENGINE::RenderProperty _InitRenderProp;
	// 이값을 런타임에 바꾸면 렌더를 켜고 끌수 있음. 
	_InitRenderProp.bRender = true;
	// 넘겨준 패스에서는 렌더링 호출 보장 . 
	_InitRenderProp.RenderOrders =
	{
		RenderProperty::Order::GBufferSK,
		RenderProperty::Order::ForwardAlphaBlendSK,
		RenderProperty::Order::DebugSK,
		RenderProperty::Order::DebugBone
	};
	RenderInterface::Initialize(_InitRenderProp);
	/// 

	//// 렌더링 패스와 쉐이더 매칭 . 쉐이더 매칭이 안되면 렌더링을 못함.
	//_ShaderInfo.RegistShader(
	//	RenderProperty::Order::ForwardAlphaBlend,
	//	L"..\\..\\Resource\\Shader\\ForwardAlphaBlendSK.hlsl", {});
	//_ShaderInfo.RegistShader(
	//	RenderProperty::Order::GBuffer,
	//	L"..\\..\\Resource\\Shader\\GBufferSK.hlsl", {});

	//_ShaderInfo.RegistShader(
	//	RenderProperty::Order::Debug,
	//	L"..\\..\\Resource\\Shader\\DebugSK.hlsl", {});
	//_ShaderInfo.RegistShader(
	//	RenderProperty::Order::DebugBone,
	//	L"..\\..\\Resource\\Shader\\DebugBone.hlsl", {});

	//// ..... 
	//PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::GBuffer).get());
	//PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::ForwardAlphaBlend).get());
	//PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::Debug).get());
	//PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::DebugBone).get());

	// 스켈레톤 메쉬 로딩 ... 
	Mesh::InitializeInfo _InitInfo{};
	// 버텍스 정점 정보가 CPU 에서도 필요 한가 ? 
	_InitInfo.bLocalVertexLocationsStorage = false;
	// 루트 모션 지원 해줘 !!
	//_InitInfo.bRootMotionScale = false;
	//_InitInfo.bRootMotionRotation = false;
	//_InitInfo.bRootMotionTransition = false;
	_SkeletonMesh = Resources::Load<ENGINE::SkeletonMesh>
		(L"..\\..\\Resource\\TestDummy\\Em100\\Em100.fbx" , _InitInfo);

	// 디폴트 이름 말고 원하는 이름으로 루트모션 켜기 . 
	// (필요없는 루트모션 정보는 이름을 "" 으로 입력)
	// _SkeletonMesh->EnableRootMotion("스케일루트모션루트이름", "로테이션...", "이동...");

	PushEditEntity(_SkeletonMesh.get());
	// Prev VTF 켜기 . (모션블러등 이전 스키닝 정보가 필요할 경우
	_SkeletonMesh->EnablePrevVTF();
	// Prev VTF 끄기 . (런타임에 끌수 있으며 자주 켜고 끄는 것은 좋지않음)
	// _SkeletonMesh->DisablePrevVTF();

	/* 애니메이션에 따라서 콜백이 필요한 경우 설정.
	return true 인 경우 콜백 하고나서 삭제
	return false 인 경우 콜백 하고 나서 다음 루프때도 다시 콜백.
	*/
	ENGINE::AnimNotify _Notify{};
	_Notify.Event[0.5] = [this]() {  Log("0.5 Sec Call");  return true; };
	_Notify.Event[0.9] = [this]() {  Log("0.9 Sec Call");  return false; };
	            // 

	// 트랜스폼 초기화하며 Edit 에 정보가 표시되도록 푸시 . 
	auto InitTransform = GetComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.001,0.001,0.001 });
	PushEditEntity(InitTransform.lock().get());

	// 에디터의 도움을 받고싶은 오브젝트들 Raw 포인터로 푸시.
	// 	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::ForwardAlphaBlend).get());

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
	// 현재 스케일과 회전은 의미가 없음 DeltaPos 로 트랜스폼에서 통제 . 
	auto [DeltaScale,DeltaQuat,DeltaPos ] = _SkeletonMesh->Update(_fDeltaTime);
	 Vector3 Axis = { 1,0,0 };

	 const float Length = FMath::Length(DeltaPos);

	//DeltaPos = FMath::RotationVecNormal(DeltaPos, Axis, FMath::ToRadian(90.f)) * Length;

	if (auto SpTransform = GetComponent<ENGINE::Transform>().lock();
		SpTransform)
	{
		SpTransform->SetPosition(SpTransform->GetPosition() + DeltaPos  * SpTransform->GetScale().x);
		// SpTransform->SetScale(SpTransform->GetScale() + DeltaScale * SpTransform->GetScale().x);
	}

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
