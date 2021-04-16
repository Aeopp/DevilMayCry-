#include "stdafx.h"
#include "Em5000.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"
#include "TestObject.h"
#include "Car.h"
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

void Em5000::RenderDebugImplementation(const ImplementationInfo& _ImplInfo)
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

void Em5000::RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo)
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

void Em5000::RenderDebugBoneImplementation(const ImplementationInfo& _ImplInfo)
{
	if (auto SpTransform = GetComponent<ENGINE::Transform>().lock();
		SpTransform)
	{
		const Matrix ScaleOffset = FMath::Scale({ 0.01,0.01 ,0.01 });

		_ImplInfo.Fx->SetMatrix("ScaleOffset", &ScaleOffset);

		m_pMesh->BoneDebugRender(SpTransform->GetWorldMatrix(), _ImplInfo.Fx);
	}
}



void Em5000::Fight(const float _fDeltaTime)
{
	Skill_CoolTime(_fDeltaTime);

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

	//�Ÿ��� �ֶ��� �̵� or ȸ���� ��.
	//�Ÿ��� ������ �������� ȸ���� ��ų �� ����
	if (fDir >= 10.f)
	{
		if (m_bThrow && m_bIng == false)
		{
			Turn_To_Car();
			m_bIng = true;
			return;
		}
		if (m_bTurn && m_bIng == false)
		{
			m_bIng = true;
			Turn();
			return;
		}

		if (m_bMove && m_bIng == false)
		{
			m_bIng = true;
			int iRandom = FMath::Random<int>(1, 7);

			if (iRandom == 1 || iRandom == 2)
				m_eState = Back_Jump;
			else
				m_eState = Move_Start;

			return;
		}
		if (m_bJumpAttack && fDir >= 15.f && fDir <= 20.f && m_bIng == false)
		{
			m_eState = Attack_Jump_Attack;
			return;
		}
	}
	else
	{
		if (m_bAttack && m_bIng == false)
		{
			m_bIng = true;
			int iRandom = FMath::Random<int>(1, 2);
			//���� �� ���Ұ� ���� or �����ʿ� ������ ����
			if (fDegree >= 65.0f && fDegree <= 115.0f)
			{
				if (vCross.y > 0)
				{
					m_eState = Attack_Side_L;
					m_fMoveTime = 0.f;
					return;
				}
				else
				{
					m_eState = Attack_Side_R;
					m_fMoveTime = 0.f;
					return;
				}
			}
			//���� �� ���Ұ� �ڿ� ������ �� ����
			if (fDegree <= 65.f && fDegree >= 0.f)
			{
				if (vCross.y > 0)
				{
					m_eState = Attack_Back_L;
					m_fMoveTime = 0.f;
					return;
				}
				else
				{
					m_eState = Attack_Back_R;
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
		int iRandom = FMath::Random<int>(1, 3);
		if (iRandom == 1)
		{
			if (m_bBackJump && m_bIng == false)
			{
				m_eState = Back_Jump;
				m_bIng = true;
			}
		}
	}


}

void Em5000::State_Change(const float _fDeltaTime)
{
	Vector3	 vDir = m_pPlayerTrans.lock()->GetPosition() - m_pTransform.lock()->GetPosition();
	Vector3	 vLook = m_pTransform.lock()->GetLook();
	float	 fDir = D3DXVec3Length(&vDir);


	Vector3 vCarDir = m_pTransform.lock()->GetPosition() - m_pCarTrans.lock()->GetPosition();
	float   fCarDir = D3DXVec3Length(&vCarDir);


	switch (m_eState)
	{
	case Em5000::Attack_Back_L:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Attack_Back_L", false, {}, 1.5f, 20.f);

			if (m_pMesh->CurPlayAnimInfo.Name == "Attack_Back_L" && m_pMesh->PlayingTime() >= 0.95f)
			{
				m_bIng = false;
				m_bAttack = false;
				m_eState = Idle;
			}
		}
		break;
	case Em5000::Attack_Back_R:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Attack_Back_R", false, {}, 1.5f, 20.f);

			if (m_pMesh->CurPlayAnimInfo.Name == "Attack_Back_R" && m_pMesh->PlayingTime() >= 0.95f)
			{
				m_bIng = false;
				m_bAttack = false;
				m_eState = Idle;
			}
		}
		break;
	case Em5000::Attack_Finish:
		break;
	case Em5000::Attack_Grap_Car:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Attack_Grap_Car", false, {}, 1.f, 10.f);


			if (m_pMesh->CurPlayAnimInfo.Name == "Attack_Grap_Car" && m_pMesh->PlayingTime() >= 0.65f
				&& m_pMesh->PlayingAccTime() <= 0.8f)
			{
				Update_Angle();
				m_bInteraction = true;
			}

			if (m_pMesh->CurPlayAnimInfo.Name == "Attack_Grap_Car" && m_pMesh->PlayingTime() >= 0.95f)
				m_eState = Attack_Throw_Car;
		}
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
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Attack_Jump_Attack", false, {}, 1.5f, 10.f);

			if (m_pMesh->PlayingTime() >= 0.1f && m_pMesh->PlayingTime() <= 0.4f)
			{
				Update_Angle();
				m_bInteraction = true;
			}

			if (m_pMesh->CurPlayAnimInfo.Name == "Attack_Jump_Attack" && m_pMesh->PlayingTime()>=0.98f)
			{
				m_bIng = false;
				m_bJumpAttack = false;
				m_eState = Idle;
			}
		}
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
		if (m_bIng == true)
		{
			Update_Angle();
			m_bInteraction = true;
			m_pMesh->PlayAnimation("Attack_Rush_Start", false, {}, 1.f, 50.f);
			
			if (m_pMesh->CurPlayAnimInfo.Name == "Attack_Rush_Start" && fDir <= 25.f)
				m_eState = Attack_Rush_End;


			if (m_pMesh->CurPlayAnimInfo.Name == "Attack_Rush_Start" && m_pMesh->PlayingTime() >= 0.95f)
				m_eState = Attack_Rush_Loop;
		}
		break;
	case Em5000::Attack_Rush_Loop:
		if (m_bIng == true)
		{
			Update_Angle();
			m_bInteraction = true;
			m_pMesh->PlayAnimation("Attack_Rush_Loop", false, {}, 1.f, 10.f);

			if (m_pMesh->CurPlayAnimInfo.Name == "Attack_Rush_Loop" && fDir <= 25.f)
				m_eState = Attack_Rush_End;
		}
		break;
	case Em5000::Attack_Rush_End:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Attack_Rush_End", false, {}, 1.f, 10.f);

			if (m_pMesh->CurPlayAnimInfo.Name == "Attack_Rush_End" && m_pMesh->PlayingTime()>= 0.95f)
			{
				m_bIng = false;
				m_bMove = false;
				m_bRushAttack = false;
				m_eState = Idle;
			}
		}
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
		if (m_bIng == true)
		{
			Update_Angle();
			m_bInteraction = true;
			m_pMesh->PlayAnimation("Attack_Throw_Car", false, {}, 1.f, 10.f);

			if (m_pMesh->CurPlayAnimInfo.Name == "Attack_Throw_Car" && m_pMesh->PlayingTime() >= 0.95f)
			{
				m_bIng = false;
				m_bThrow = false;
				m_eState = Idle;
			}
		}
		break;
	case Em5000::Back_Jump:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Back_Jump", false, {}, 1.f, 10.f);

			if (m_pMesh->PlayingTime()>=0.9f && m_pMesh->CurPlayAnimInfo.Name == "Back_Jump")
			{
				int iRandom = FMath::Random<int>(1, 2);
				
				if (iRandom == 1)
				{

					if (m_bRushAttack && fDir >= 15.f)
					{
						m_eState = Attack_Rush_Start;
						m_bMove = false;
						return;
					}
				}
				else
				{
					if (m_bJumpAttack && fDir >= 20.f &&fDir <= 25.f)
					{
						m_eState = Attack_Jump_Attack;
						m_bMove = false;
						return;
					}
				}
				m_bIng = false;
				m_bMove = false;
				m_bBackJump = false;
				m_eState = Idle;
			}
		}

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
			m_pMesh->PlayAnimation("Move_Loop", true, {}, 1.3f, 50.f);
			m_bInteraction = true;
			Update_Angle();


			if (fDir <= 30 && fDir >= 25.f && m_bJumpAttack)  
			{
				m_eState = Attack_Jump_Attack;
				m_bMove = false;
				return;
			}
			if (fDir <= 10.f)
			{
				m_bIng = false;
				m_bMove = false;
			}
		}
		break;
	case Em5000::Move_Start:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Move_Start", false, {}, 1.3f, 30.f);
			if(m_pMesh->CurPlayAnimInfo.Name == "Move_Start" && m_pMesh->PlayingTime()>= 0.2f)
			{
				Update_Angle();
				m_bInteraction = true;
			}
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
			m_pMesh->PlayAnimation("Move_Turn_L", false, {}, 1.2f, 20.f);

			if (m_pMesh->CurPlayAnimInfo.Name == "Move_Turn_L" && m_pMesh->PlayingTime() >= 0.2f && m_pMesh->PlayingTime() <= 0.6f)
			{
				m_bInteraction = true;
				Update_Angle();
			}
			if (m_pMesh->CurPlayAnimInfo.Name == "Move_Turn_L" && m_pMesh->PlayingTime() >= 0.95f)
			{
				m_eState = Idle;
				m_bIng = false;
				m_bTurn = false;
			}
		}
		break;
	case Em5000::Move_Turn_R:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Move_Turn_R", false, {}, 1.2f, 20.f);
			if (m_pMesh->CurPlayAnimInfo.Name == "Move_Turn_R" && m_pMesh->PlayingTime() >= 0.2f && m_pMesh->PlayingTime() <= 0.6f)
			{
				m_bInteraction = true;
				Update_Angle();
			}
			if (m_pMesh->CurPlayAnimInfo.Name == "Move_Turn_R" && m_pMesh->PlayingTime() >= 0.95f)
			{
				m_eState = Idle;
				m_bIng = false;
				m_bTurn = false;
			}
		}
		break;
	case Em5000::Car_Turn_L:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Move_Turn_L", false, {}, 1.2f, 20.f);
			if (m_pMesh->CurPlayAnimInfo.Name == "Move_Turn_L" && m_pMesh->PlayingTime() >= 0.2f && m_pMesh->PlayingTime() <= 0.6f)
			{
				m_bInteraction = true;
				Update_Angle_ToCar();
			}
			if (m_pMesh->CurPlayAnimInfo.Name == "Move_Turn_L" && m_pMesh->PlayingTime() >= 0.85f)
				m_eState = Car_Rush_Start;
		}
		break;
	case Em5000::Car_Turn_R:
		if (m_bIng == true)
		{
			m_pMesh->PlayAnimation("Move_Turn_R", false, {}, 1.2f, 20.f);
			if (m_pMesh->CurPlayAnimInfo.Name == "Move_Turn_R" && m_pMesh->PlayingTime() >= 0.2f && m_pMesh->PlayingTime() <= 0.6f)
			{
				m_bInteraction = true;
				Update_Angle_ToCar();
			}
			if (m_pMesh->CurPlayAnimInfo.Name == "Move_Turn_R" && m_pMesh->PlayingTime() >= 0.85f)
				m_eState = Car_Rush_Start;
		}
		break;
	case Em5000::Car_Rush_Start:
		if (m_bIng == true)
		{
			Update_Angle_ToCar();
			m_bInteraction = true;

			m_pMesh->PlayAnimation("Attack_Rush_Start", false, {}, 1.f, 50.f);
				
			if (fCarDir <= 8.5f)
			{
				m_eState = Attack_Grap_Car;
				return;
			}
			if (m_pMesh->CurPlayAnimInfo.Name == "Attack_Rush_Start" && m_pMesh->PlayingTime() >= 0.9f)
				m_eState = Car_Rush_Loop;
		}
		break;
	case Em5000::Car_Rush_Loop:
		if(m_bIng == true)
		{
			m_pMesh->PlayAnimation("Attack_Rush_Loop", true, {}, 1.f, 10.f);
			Update_Angle_ToCar();
			m_bInteraction = true;

			if (fCarDir <= 8.5f)
				m_eState = Attack_Grap_Car;

		}
		break;
	default:
		break;
	}

}

void Em5000::Skill_CoolTime(const float _fDeltaTime)
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
	if (m_bRushAttack == false)
	{
		m_fRushAttackTime += _fDeltaTime;
		if (m_fRushAttackTime >= 10.f)
		{
			m_bRushAttack = true;
			m_fRushAttackTime = 0.f;
		}
	}
	if (m_bJumpAttack == false)
	{
		m_fJumpAttackTime += _fDeltaTime;
		if (m_fJumpAttackTime >= 15.f)
		{
			m_bJumpAttack = true;
			m_fJumpAttackTime = 0.f;
		}
	}
	if (m_bThrow == false)
	{
		m_fThrowTime += _fDeltaTime;
		if (m_fThrowTime >= 4.f)
		{
			m_bThrow = true;
			m_fThrowTime = 0.f;
		}
	}
	if (m_bBackJump == false)
	{
		m_fBackJumpTime += _fDeltaTime;
		if (m_fBackJumpTime >= 10.f)
		{
			m_bBackJump = true;
			m_fBackJumpTime = 0.f;
		}
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

	m_pMesh->EnableToRootMatricies();
	return S_OK;
}

HRESULT Em5000::Awake()
{
	m_pPlayer = std::static_pointer_cast<TestObject>(FindGameObjectWithTag(Player).lock());
	m_pPlayerTrans = m_pPlayer.lock()->GetComponent<ENGINE::Transform>();

	m_pCar = std::static_pointer_cast<Car>(FindGameObjectWithTag(ThrowCar).lock());
	m_pCarTrans = m_pCar.lock()->GetComponent<ENGINE::Transform>();

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


void Em5000::Turn_To_Car()
{
	Vector3	 vDir2 = m_pCarTrans.lock()->GetPosition() - m_pTransform.lock()->GetPosition();
	Vector3	 vLook2 = m_pTransform.lock()->GetLook();
	float	 fDir2 = D3DXVec3Length(&vDir2);

	D3DXVec3Normalize(&vDir2, &vDir2);
	Vector3  vCross2;
	D3DXVec3Cross(&vCross2, &vLook2, &vDir2);
	float fDot2 = D3DXVec3Dot(&vDir2, &vLook2);

	if (fDot2 > 0)
	{
		if (vCross2.y > 0)
			m_eState = Car_Turn_L;
		else
			m_eState = Car_Turn_R;
	}
	else
	{
		if (vCross2.y > 0)
			m_eState = Car_Turn_L;
		else
			m_eState = Car_Turn_R;
	}
}

void Em5000::Update_Angle_ToCar()
{
	Vector3 vCarPos = m_pCarTrans.lock()->GetPosition();
	Vector3 vMyPos = m_pTransform.lock()->GetPosition();

	Vector3 vDir = vCarPos - vMyPos;
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
