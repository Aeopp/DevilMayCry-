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
	//������
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
	// ������ �����ؾ��ϴ� ������Ʈ��� (�������� ��� ���� ) �˸�.
	// ���� �������̽� ��ӹ��� �ʾҴٸ� Ű��������.
	SetRenderEnable(true);

	// ���� �Ӽ� ��ü �ʱ�ȭ 
	ENGINE::RenderProperty _InitRenderProp;
	// �̰��� ��Ÿ�ӿ� �ٲٸ� ������ �Ѱ� ���� ����. 
	_InitRenderProp.bRender = true;
	// �Ѱ��� �н������� ������ ȣ�� ���� . 
	_InitRenderProp.RenderOrders =
	{
		RenderProperty::Order::GBufferSK,
		RenderProperty::Order::ForwardAlphaBlendSK,
		RenderProperty::Order::DebugSK,
		RenderProperty::Order::DebugBone
	};
	RenderInterface::Initialize(_InitRenderProp);
	/// 

	//// ������ �н��� ���̴� ��Ī . ���̴� ��Ī�� �ȵǸ� �������� ����.
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

	// ���̷��� �޽� �ε� ... 
	Mesh::InitializeInfo _InitInfo{};
	// ���ؽ� ���� ������ CPU ������ �ʿ� �Ѱ� ? 
	_InitInfo.bLocalVertexLocationsStorage = false;
	// ��Ʈ ��� ���� ���� !!
	//_InitInfo.bRootMotionScale = false;
	//_InitInfo.bRootMotionRotation = false;
	//_InitInfo.bRootMotionTransition = false;
	_SkeletonMesh = Resources::Load<ENGINE::SkeletonMesh>
		(L"..\\..\\Resource\\TestDummy\\Em100\\Em100.fbx" , _InitInfo);

	// ����Ʈ �̸� ���� ���ϴ� �̸����� ��Ʈ��� �ѱ� . 
	// (�ʿ���� ��Ʈ��� ������ �̸��� "" ���� �Է�)
	// _SkeletonMesh->EnableRootMotion("�����Ϸ�Ʈ��Ƿ�Ʈ�̸�", "�����̼�...", "�̵�...");

	PushEditEntity(_SkeletonMesh.get());
	// Prev VTF �ѱ� . (��Ǻ��� ���� ��Ű�� ������ �ʿ��� ���
	_SkeletonMesh->EnablePrevVTF();
	// Prev VTF ���� . (��Ÿ�ӿ� ���� ������ ���� �Ѱ� ���� ���� ��������)
	// _SkeletonMesh->DisablePrevVTF();

	/* �ִϸ��̼ǿ� ���� �ݹ��� �ʿ��� ��� ����.
	return true �� ��� �ݹ� �ϰ��� ����
	return false �� ��� �ݹ� �ϰ� ���� ���� �������� �ٽ� �ݹ�.
	*/
	ENGINE::AnimNotify _Notify{};
	_Notify.Event[0.5] = [this]() {  Log("0.5 Sec Call");  return true; };
	_Notify.Event[0.9] = [this]() {  Log("0.9 Sec Call");  return false; };
	            // 

	// Ʈ������ �ʱ�ȭ�ϸ� Edit �� ������ ǥ�õǵ��� Ǫ�� . 
	auto InitTransform = GetComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.001,0.001,0.001 });
	PushEditEntity(InitTransform.lock().get());

	// �������� ������ �ް���� ������Ʈ�� Raw �����ͷ� Ǫ��.
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
	// ���� �����ϰ� ȸ���� �ǹ̰� ���� DeltaPos �� Ʈ���������� ���� . 
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
		// ������ .... 
	}
}


void TestAnimationObject::OnEnable()
{

}

void TestAnimationObject::OnDisable()
{

}
