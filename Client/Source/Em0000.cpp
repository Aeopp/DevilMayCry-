#include "stdafx.h"
#include "Em0000.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"
#include "TestObject.h"
#include <filesystem>

void Em0000::Free()
{
}

std::string Em0000::GetName()
{
	return "Em0000";
}

Em0000* Em0000::Create()
{
	return new Em0000{};
}

void Em0000::RenderDebugImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = m_pMesh->GetNumSubset();
	m_pMesh->BindVTF(_ImplInfo.Fx);
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = m_pMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			SharedSubset->Render(_ImplInfo.Fx);
		}
	}
}

void Em0000::RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = m_pMesh->GetNumSubset();
	m_pMesh->BindVTF(_ImplInfo.Fx);
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = m_pMesh->GetSubset(SubsetIdx);
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

void Em0000::RenderDebugBoneImplementation(const ImplementationInfo& _ImplInfo)
{
	if (auto SpTransform = GetComponent<ENGINE::Transform>().lock();
		SpTransform)
	{
		const Matrix ScaleOffset = FMath::Scale({ 0.01,0.01 ,0.01 });

		_ImplInfo.Fx->SetMatrix("ScaleOffset", &ScaleOffset);

		m_pMesh->BoneDebugRender(SpTransform->GetWorldMatrix(), _ImplInfo.Fx);
	}
}



void Em0000::Fight(const float _fDeltaTime)
{
	if (m_bMove == false)
	{
		m_fMoveTime += _fDeltaTime;
		if (m_fMoveTime >= 5.f)
		{
			m_bMove = true;
			m_fMoveTime = 0.f;
		}
	}
	if (m_bAttack == false)
	{
		m_fAttackTime += _fDeltaTime;
		if (m_fAttackTime >= 5.f)
		{
			m_bAttack = true;
			m_fAttackTime = 0.f;
		}
	}

	Vector3	 vDir = m_pPlayerTrans.lock()->GetPosition() - m_pTransform.lock()->GetPosition();
	float	 fDir = D3DXVec3Length(&vDir);
	

}

void Em0000::State_Change(const float _fDeltaTime)
{
	switch (m_eState)
	{
	case Em0000::Attack_1:
		break;
	case Em0000::Attack_2:
		break;
	case Em0000::Attack_Hard:
		break;
	case Em0000::Buster_End:
		break;
	case Em0000::Buster_Start:
		break;
	case Em0000::Dead:
		break;
	case Em0000::Guard_End:
		break;
	case Em0000::Guard_Hit1:
		break;
	case Em0000::Guard_Hit2:
		break;
	case Em0000::Guard_Loop:
		break;
	case Em0000::Guard_Start:
		break;
	case Em0000::Hit_Back:
		break;
	case Em0000::Hit_Front:
		break;
	case Em0000::Hit_L:
		break;
	case Em0000::Hit_R:
		break;
	case Em0000::Move_Front_End:
		break;
	case Em0000::Move_Front_Loop:
		break;
	case Em0000::Move_Front_Start:
		break;
	case Em0000::Step_Back:
		break;
	case Em0000::Stun_End:
		break;
	case Em0000::Stun_Start:
		break;
	case Em0000::idle:
		break;
	case Em0000::State_END:
		break;
	default:
		break;
	}
}

HRESULT Em0000::Ready()
{
	m_nTag = Monster0000;

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
		RenderProperty::Order::ForwardAlphaBlend,
		RenderProperty::Order::Debug ,
		RenderProperty::Order::DebugBone
	};
	RenderInterface::Initialize(_InitRenderProp);

	// ������ �н��� ���̴� ��Ī . ���̴� ��Ī�� �ȵǸ� �������� ����.
	_ShaderInfo.RegistShader(
		RenderProperty::Order::ForwardAlphaBlend,
		L"..\\..\\Resource\\Shader\\ForwardAlphaBlendSK.hlsl", {});
	_ShaderInfo.RegistShader(
		RenderProperty::Order::Debug,
		L"..\\..\\Resource\\Shader\\DebugSK.hlsl", {});
	_ShaderInfo.RegistShader(
		RenderProperty::Order::DebugBone,
		L"..\\..\\Resource\\Shader\\DebugBone.hlsl", {});

	// ..... 
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::ForwardAlphaBlend).get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::Debug).get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::DebugBone).get());

	// ���̷��� �޽� �ε� ... 
	Mesh::InitializeInfo _InitInfo{};
	// ���ؽ� ���� ������ CPU ������ �ʿ� �Ѱ� ? 
	_InitInfo.bLocalVertexLocationsStorage = false;
	// ��Ʈ ��� ���� ���� !!
	_InitInfo.bRootMotionScale = true;
	_InitInfo.bRootMotionRotation = true;
	_InitInfo.bRootMotionTransition = true;
	m_pMesh = Resources::Load<ENGINE::SkeletonMesh>(L"..\\..\\Resource\\Mesh\\Dynamic\\Monster\\Em0000\\Em0000.fbx", _InitInfo);

	// ����Ʈ �̸� ���� ���ϴ� �̸����� ��Ʈ��� �ѱ� . 
	// (�ʿ���� ��Ʈ��� ������ �̸��� "" ���� �Է�)
	// _SkeletonMesh->EnableRootMotion("�����Ϸ�Ʈ��Ƿ�Ʈ�̸�", "�����̼�...", "�̵�...");

	PushEditEntity(m_pMesh.get());
	// Prev VTF �ѱ� . (��Ǻ��� ���� ��Ű�� ������ �ʿ��� ���
	m_pMesh->EnablePrevVTF();
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


	//���� �ʱ���� idle
	m_pMesh->PlayAnimation("idle", true);

	//���� ȸ�� �⺻ �ӵ�
	m_fAngleSpeed = D3DXToRadian(100.f);
	return S_OK;
}

HRESULT Em0000::Awake()
{
	m_pPlayer = std::static_pointer_cast<TestObject>(FindGameObjectWithTag(Player).lock());
	m_pPlayerTrans = m_pPlayer.lock()->GetComponent<ENGINE::Transform>();


	return S_OK;
}

HRESULT Em0000::Start()
{
	return S_OK;
}

UINT Em0000::Update(const float _fDeltaTime)
{
	GameObject::Update(_fDeltaTime);
	// ���� �����ϰ� ȸ���� �ǹ̰� ���� DeltaPos �� Ʈ���������� ���� . 
	auto [DeltaScale, DeltaQuat, DeltaPos] = m_pMesh->Update(_fDeltaTime);
	Vector3 Axis = { 1,0,0 };
	

	//ENGINE::AnimNotify _Notify{};
	////return true �� ���� ȣ�� ����, false�� �� ���� ���� ��� ȣ��.
	//_Notify.Event[0.5] = [this]() {  AttackStart();  return false; };

	const float Length = FMath::Length(DeltaPos);
	D3DXMATRIX matRot;
	D3DXQUATERNION tQuat = m_pTransform.lock()->GetQuaternion();
	D3DXMatrixRotationQuaternion(&matRot, &tQuat);

	D3DXVec3TransformNormal(&DeltaPos, &DeltaPos, &matRot);
	//DeltaPos = FMath::RotationVecNormal(DeltaPos, Axis, FMath::ToRadian(90.f)) * Length;

	if (auto SpTransform = GetComponent<ENGINE::Transform>().lock();
		SpTransform)
	{
		SpTransform->SetPosition(SpTransform->GetPosition() + DeltaPos * SpTransform->GetScale().x);
	}
	//�÷��̾ ��������� �Ǵ�
	/*if (false == m_pPlayer.expired())
	{
		std::cout << "Player Dead" << std::endl;
	}*/


	

	if (Input::GetKeyDown(DIK_SPACE))
	{
		if (m_bTest == true)
			m_bTest = false;
		else
			m_bTest = true;
	}

	if (m_bTest == true)
	{
		Fight(_fDeltaTime);
		State_Change(_fDeltaTime);
	}

	/*if (Input::GetKeyDown(DIK_T))
		Update_Angle();

	if (Input::GetKeyDown(DIK_Y))
	{
		if (m_bInteraction == true)
			m_bInteraction = false;
		else
			m_bInteraction = true;
	}*/


	Rotate(_fDeltaTime);


	return 0;
}

UINT Em0000::LateUpdate(const float _fDeltaTime)
{
	
	return 0;

}

void Em0000::Editor()
{
	GameObject::Editor();
	if (bEdit)
	{

	}

}

void Em0000::OnEnable()
{
}

void Em0000::OnDisable()
{
}

void Em0000::Rotate(const float _fDeltaTime)
{
	if (m_bInteraction == false)
		return;

	if (fabs(m_fRadian) <= fabs(m_fAccuangle))
	{
		float fAdd = m_fRadian - m_fAccuangle;

		m_pTransform.lock()->Rotate({ 0.f, -D3DXToDegree(fAdd), 0.f });
		
		m_bInteraction = false;
		
		return;
	}
	m_pTransform.lock()->Rotate({ 0.f, -D3DXToDegree(m_fAngleSpeed * _fDeltaTime), 0.f });

	m_fAccuangle += m_fAngleSpeed * _fDeltaTime;
}

void Em0000::Update_Angle()
{
	Vector3 vPlayerPos = m_pPlayerTrans.lock()->GetPosition();
	Vector3 vMyPos = m_pTransform.lock()->GetPosition();

	Vector3 vDir = vPlayerPos - vMyPos;
	vDir.y = 0;
	D3DXVec3Normalize(&vDir, &vDir);

	Vector3 vLook = -m_pTransform.lock()->GetLook();

	float fDot = D3DXVec3Dot(&vDir, &vLook);
	float fRadian = acosf(fDot);

	Vector3	vCross;
	D3DXVec3Cross(&vCross, &vLook, &vDir);
	
	if (vCross.y > 0)
		fRadian *= -1;

	m_fRadian = fRadian;
	m_fAccuangle = 0.f;

	if (m_fRadian > 0)
		m_fAngleSpeed = fabs(m_fAngleSpeed);
	else
		m_fAngleSpeed = -fabs(m_fAngleSpeed);
}
