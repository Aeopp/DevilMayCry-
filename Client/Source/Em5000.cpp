#include "stdafx.h"
#include "Em5000.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"
#include "TestObject.h"
#include <filesystem>

void Em5000::Free()
{
}

std::string Em5000::GetName()
{
	return "Em5000";
}

Em5000* Em5000::Create()
{
	return new Em5000{};
}




void Em5000::Fight(const float _fDeltaTime)
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
	if (m_bSideAttack == false)
	{
		m_fSideAttackTime += _fDeltaTime;
		if (m_fSideAttackTime >= 5.f)
		{
			m_bSideAttack = true;
			m_fSideAttackTime = 0.f;
		}
	}
	if (m_bTurn == false)
	{
		m_fTurnTime += _fDeltaTime;
		if (m_fTurnTime >= 5.f)
		{
			m_bTurn = true;
			m_fTurnTime = 0.f;
		}
	}

	Vector3	 vDir = m_pPlayerTrans.lock()->GetPosition() - m_pTransform.lock()->GetPosition();
	Vector3	 vLook = m_pTransform.lock()->GetLook();
	float	 fDir = D3DXVec3Length(&vDir);


	D3DXVec3Normalize(&vDir, &vDir);
	//���� ������ �����ϱ� ���� ���� ��� ��
	Vector3  vCross;
	D3DXVec3Cross(&vCross, &vLook, &vDir);
	//�� �� �����ϱ� ���� ���� ��� ��
	float fDot = D3DXVec3Dot(&vDir, &vLook);
	float fRadian = acosf(fDot);
	float fDegree = D3DXToDegree(fRadian);

	//�̵� ��ü�� �ֶ���
	if (fDir >= 8.f)
	{
		if (m_bMove && m_bIng == false)
		{
			m_bIng = true;
			m_bInteraction = true;
			Update_Angle();
			m_eState = Move_Start;
			return;
		}

	}
	//�÷��̾�� ���Ͷ� �����̴�
	else
	{
		//Move_Loop ���¿��� ����� ����.
		//���� ����� Move �ʱ�ȭ ����. ing�� ��� ������ false�� �ٲ�.
		if (m_bMove && m_bIng == true)
		{
			if (m_bAttack)
			{
				m_bMove = false;
				m_fMoveTime = 0.f;
				int iRandom = FMath::Random<int>(1, 2);
				if (fDegree >= 75.0f && fDegree <= 115.0f)
				{
					m_bIng = true;
					if (vCross.y > 0)
					{
						m_eState = Attack_Side_L;
						m_bMove = false;
						m_fMoveTime = 0.f;
						return;
					}
					else
					{
						m_eState = Attack_Side_R;
						m_bMove = false;
						m_fMoveTime = 0.f;
						return;
					}
				}

				if (iRandom == 1)
				{
					m_eState = Attack_Punch_Twice;
					return;
				}
				else
				{
					m_eState = Attack_Hack;
					return;
				}
			}
			//��������µ� ������ ��Ÿ���̾�. �ϴ� Idle�� �ٲ�
			else
			{
				m_bMove = false;
				m_fMoveTime = 0.f;
				m_eState = Idle;
				return;
			}
		}
	}


	if (m_bTurn && m_bIng == false)
	{
		m_bIng = true;
		Turn();
		return;
	}






}

void Em5000::State_Change(const float _fDeltaTime)
{
	Vector3	 vDir = m_pPlayerTrans.lock()->GetPosition() - m_pTransform.lock()->GetPosition();
	Vector3	 vLook = m_pTransform.lock()->GetLook();
	float	 fDir = D3DXVec3Length(&vDir);


	switch (m_eState)
	{
	case Em5000::Attack_Back_L:
		break;
	case Em5000::Attack_Back_R:
		break;
	case Em5000::Attack_Finish:
		break;
	case Em5000::Attack_Grap_Car:
		break;
	case Em5000::Attack_Hack:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Attack_Hack", false, {}, 1.5f, 50.f);

			if (m_pMesh->CurPlayAnimInfo.Name == "Attack_Hack" && m_pMesh->PlayingTime() >= 0.95f)
			{
				m_bIng = false;
				m_bAttack = false;
				m_eState = Idle;
			}
		}
		break;
	case Em5000::Attack_Jump_Attack:
		break;
	case Em5000::Attack_Punch_Twice:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Attack_Punch_Twice", false, {}, 1.5f, 50.f);

			if (m_pMesh->CurPlayAnimInfo.Name == "Attack_Punch_Twice" && m_pMesh->PlayingTime() >= 0.95f)
			{
				m_bIng = false;
				m_bAttack = false;
				m_eState = Idle;
			}
		}
		break;
	case Em5000::Attack_Rush_Start:
		break;
	case Em5000::Attack_Rush_Loop:
		break;
	case Em5000::Attack_Rush_End:
		break;
	case Em5000::Attack_Side_L:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Attack_Side_L", false, {}, 1.f, 20.f);
			if (m_pMesh->IsAnimationEnd() && m_pMesh->CurPlayAnimInfo.Name == "Attack_Side_L")
			{
				m_bIng = false;
				m_bAttack = false;
				m_eState = Idle;	
			}
		}
		break;
	case Em5000::Attack_Side_R:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Attack_Side_R", false, {}, 1.f, 20.f);
			if (m_pMesh->IsAnimationEnd() && m_pMesh->CurPlayAnimInfo.Name == "Attack_Side_R")
			{
				m_bIng = false;
				m_bAttack = false;
				m_eState = Idle;
			}
		}
		break;
	case Em5000::Attack_Throw_Car:
		break;
	case Em5000::Back_Jump:
		break;
	case Em5000::Dead:
		break;
	case Em5000::Groggy_Dead:
		break;
	case Em5000::Groggy_Start:
		break;
	case Em5000::Groggy_Loop:
		break;
	case Em5000::Hit_Buster_Start:
		break;
	case Em5000::Hit_Buster_Swing_Start:
		break;
	case Em5000::Hit_Buster_Swing_Loop:
		break;
	case Em5000::Hit_Buster_Swing_Throw:
		break;
	case Em5000::Hit_Buster_Swing_End:
		break;
	case Em5000::Howling:
		break;
	case Em5000::Idle:
		m_pMesh->PlayAnimation("Idle", true, {}, 1.f, 50.f);
		break;
	case Em5000::Move_Loop:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Move_Loop", true, {}, 1.f, 50.f);
			m_bInteraction = true;
			Update_Angle();
		}
		break;
	case Em5000::Move_Start:
		if (m_bIng == true)
		{
			Update_Angle();
			m_bInteraction = true;
			m_pMesh->PlayAnimation("Move_Start", false, {}, 1.f, 50.f);
			if (m_pMesh->CurPlayAnimInfo.Name == "Move_Start" && m_pMesh->PlayingTime() >= 0.9f)
			{
				m_eState = Move_Loop;
			}
		}
		break;
	case Em5000::Tornado_End:
		break;
	case Em5000::Tornado_Start:
		break;
	case Em5000::Move_Turn_L_90:
		break;
	case Em5000::Move_Turn_L_180:
		break;
	case Em5000::Move_Turn_R_90:
		break;
	case Em5000::Move_Turn_R_180:
		break;
	case Em5000::Move_Turn_L:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Move_Turn_L", false, {}, 1.f, 50.f);

			if (m_pMesh->CurPlayAnimInfo.Name == "Move_Turn_L" && m_pMesh->PlayingTime() >= 0.2f && m_pMesh->PlayingTime() <= 0.8f)
			{
				m_bInteraction = true;
				Update_Angle();
			}
			if (m_pMesh->CurPlayAnimInfo.Name == "Move_Turn_L" && m_pMesh->PlayingTime() >= 0.95f)
			{
				m_eState = Idle;
				m_bIng = false;
				m_bTurn = false;
				m_bTurnEnd = true;
			}
		}
		break;
	case Em5000::Move_Turn_R:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Move_Turn_R", false, {}, 1.f, 50.f);
			if (m_pMesh->CurPlayAnimInfo.Name == "Move_Turn_R" && m_pMesh->PlayingTime() >= 0.2f && m_pMesh->PlayingTime() <= 0.8f)
			{
				m_bInteraction = true;
				Update_Angle();
			}
			if (m_pMesh->CurPlayAnimInfo.Name == "Move_Turn_R" && m_pMesh->PlayingTime() >= 0.95f)
			{
				m_eState = Idle;
				m_bIng = false;
				m_bTurn = false;
				m_bTurnEnd = true;
			}
		}
		break;
	case Em5000::State_END:
		break;
	default:
		break;
	}

}

HRESULT Em5000::Ready()
{
	//GameObject�� �޾ƿ����� ���� �±װ� �־����.
	m_nTag = Monster5000;

	// ������ �����ؾ��ϴ� ������Ʈ��� (�������� ��� ���� ) �˸�.
	// ���� �������̽� ��ӹ��� �ʾҴٸ� Ű��������.
	SetRenderEnable(true);

	// ���� �Ӽ� ��ü �ʱ�ȭ 
	ENGINE::RenderProperty _InitRenderProp;
	// �̰��� ��Ÿ�ӿ� �ٲٸ� ������ �Ѱ� ���� ����. 
	_InitRenderProp.bRender = true;
	// �Ѱ��� �н������� ������ ȣ�� ���� . 

	RenderInterface::Initialize(_InitRenderProp);


	// ���̷��� �޽� �ε� ... 
	Mesh::InitializeInfo _InitInfo{};
	// ���ؽ� ���� ������ CPU ������ �ʿ� �Ѱ� ? 
	_InitInfo.bLocalVertexLocationsStorage = false;
	// ��Ʈ ��� ���� ���� !!
	_InitInfo.bRootMotionScale = true;
	_InitInfo.bRootMotionRotation = true;
	_InitInfo.bRootMotionTransition = true;
	m_pMesh = Resources::Load<ENGINE::SkeletonMesh>(L"..\\..\\Resource\\Mesh\\Dynamic\\Monster\\Em5000\\Em5000.x", _InitInfo);

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
	InitTransform.lock()->SetScale({ 0.01,0.01,0.01 });
	PushEditEntity(InitTransform.lock().get());

	// �������� ������ �ް���� ������Ʈ�� Raw �����ͷ� Ǫ��.
	// 	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::ForwardAlphaBlend).get());


	//���� �ʱ���� idle
	m_pMesh->PlayAnimation("Idle", true);

	//���� ȸ�� �⺻ �ӵ�
	m_fAngleSpeed = D3DXToRadian(100.f);
	return S_OK;
}

HRESULT Em5000::Awake()
{
	m_pPlayer = std::static_pointer_cast<TestObject>(FindGameObjectWithTag(Player).lock());
	m_pPlayerTrans = m_pPlayer.lock()->GetComponent<ENGINE::Transform>();


	return S_OK;
}

HRESULT Em5000::Start()
{
	return S_OK;
}

UINT Em5000::Update(const float _fDeltaTime)
{
	GameObject::Update(_fDeltaTime);
	// ���� �����ϰ� ȸ���� �ǹ̰� ���� DeltaPos �� Ʈ���������� ���� . 
	auto [DeltaScale, DeltaQuat, DeltaPos] = m_pMesh->Update(_fDeltaTime);
	Vector3 Axis = { 1,0,0 };

	//ENGINE::AnimNotify _Notify{};
	////return true �� ���� ȣ�� ����, false�� �� ���� ���� ��� ȣ��.
	//_Notify.Event[0.5] = [this]() {  AttackStart();  return false; };

	const float Length = FMath::Length(DeltaPos);


	//DeltaPos = FMath::RotationVecNormal(DeltaPos, Axis, FMath::ToRadian(90.f)) * Length;
	if (auto SpTransform = GetComponent<ENGINE::Transform>().lock();
		SpTransform)
	{	

		 
		 D3DXQUATERNION tTemp;
		 D3DXQuaternionRotationYawPitchRoll(&tTemp, D3DXToRadian(0.f), D3DXToRadian(90.f), D3DXToRadian(0.f));

		 //std::cout << DeltaQuat.x << "\t" << DeltaQuat.z << std::endl;
		 D3DXQUATERNION tTest = { DeltaQuat.x, DeltaQuat.z, DeltaQuat.y, DeltaQuat.w };
		 D3DXQUATERNION tResult = SpTransform->GetQuaternion() * DeltaQuat;
		 SpTransform->SetQuaternion(tResult);

		 D3DXMATRIX matRot;
		 D3DXQUATERNION tQuat = m_pTransform.lock()->GetQuaternion();
		 D3DXMatrixRotationQuaternion(&matRot, &tQuat);
		 D3DXVec3TransformNormal(&DeltaPos, &DeltaPos, &matRot);

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

	Rotate(_fDeltaTime);

	return 0;
}

UINT Em5000::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

void Em5000::Editor()
{
	GameObject::Editor();
	if (bEdit)
	{

	}
}

void Em5000::OnEnable()
{
}

void Em5000::OnDisable()
{
}

void Em5000::Rotate(const float _fDeltaTime)
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

void Em5000::Update_Angle()
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

void Em5000::Turn()
{
	Vector3	 vDir = m_pPlayerTrans.lock()->GetPosition() - m_pTransform.lock()->GetPosition();
	Vector3	 vLook = m_pTransform.lock()->GetLook();
	float	 fDir = D3DXVec3Length(&vDir);

	D3DXVec3Normalize(&vDir, &vDir);
	//���� ������ �����ϱ� ���� ���� ��� ��
	Vector3  vCross;
	D3DXVec3Cross(&vCross, &vLook, &vDir);
	//�� �� �����ϱ� ���� ���� ��� ��
	float fDot = D3DXVec3Dot(&vDir, &vLook);


	//
	//fDot> 0 //�÷��̾ �տ� �ִ�.
	//fDot< 0 //�÷��̾ �ڿ� �ִ�.
	//vCross.y> 0 //�÷��̾ ���ʿ� �ִ�
	//vCross.y< 0 // �÷��̾ �����ʿ� �ִ�

	//�÷��̾ �տ� �־��
	if (fDot > 0)
	{
		//�÷��̾ ���ʿ� �־��
		if (vCross.y > 0)
			m_eState = Move_Turn_L;
		//�÷��̾ �����ʿ� �־��
		else
			m_eState = Move_Turn_R;
	}
	//�÷��̾ �ڿ� �־��
	else
	{
		if (vCross.y > 0)
			m_eState = Move_Turn_L;
		//�÷��̾ �����ʿ� �־��
		else
			m_eState = Move_Turn_R;
	}


}
