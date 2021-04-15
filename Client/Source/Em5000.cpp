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
	//왼쪽 오른쪽 구분하기 위한 외적 결과 값
	Vector3  vCross;
	D3DXVec3Cross(&vCross, &vLook, &vDir);
	//앞 뒤 구분하기 위한 내적 결과 값
	float fDot = D3DXVec3Dot(&vDir, &vLook);
	float fRadian = acosf(fDot);
	float fDegree = D3DXToDegree(fRadian);

	//이동 자체는 멀때만
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
	//플레이어랑 몬스터랑 근접이다
	else
	{
		//Move_Loop 상태에서 가까워 졌다.
		//이전 모션인 Move 초기화 해줌. ing는 모션 끝날때 false로 바뀜.
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
			//가까워졌는데 공격이 쿨타임이야. 일단 Idle로 바꿔
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
	//GameObject를 받아오려면 각자 태그가 있어야함.
	m_nTag = Monster5000;

	// 렌더를 수행해야하는 오브젝트라고 (렌더러에 등록 가능 ) 알림.
	// 렌더 인터페이스 상속받지 않았다면 키지마세요.
	SetRenderEnable(true);

	// 렌더 속성 전체 초기화 
	ENGINE::RenderProperty _InitRenderProp;
	// 이값을 런타임에 바꾸면 렌더를 켜고 끌수 있음. 
	_InitRenderProp.bRender = true;
	// 넘겨준 패스에서는 렌더링 호출 보장 . 

	RenderInterface::Initialize(_InitRenderProp);


	// 스켈레톤 메쉬 로딩 ... 
	Mesh::InitializeInfo _InitInfo{};
	// 버텍스 정점 정보가 CPU 에서도 필요 한가 ? 
	_InitInfo.bLocalVertexLocationsStorage = false;
	// 루트 모션 지원 해줘 !!
	_InitInfo.bRootMotionScale = true;
	_InitInfo.bRootMotionRotation = true;
	_InitInfo.bRootMotionTransition = true;
	m_pMesh = Resources::Load<ENGINE::SkeletonMesh>(L"..\\..\\Resource\\Mesh\\Dynamic\\Monster\\Em5000\\Em5000.x", _InitInfo);

	// 디폴트 이름 말고 원하는 이름으로 루트모션 켜기 . 
	// (필요없는 루트모션 정보는 이름을 "" 으로 입력)
	// _SkeletonMesh->EnableRootMotion("스케일루트모션루트이름", "로테이션...", "이동...");

	PushEditEntity(m_pMesh.get());
	// Prev VTF 켜기 . (모션블러등 이전 스키닝 정보가 필요할 경우
	m_pMesh->EnablePrevVTF();
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
	InitTransform.lock()->SetScale({ 0.01,0.01,0.01 });
	PushEditEntity(InitTransform.lock().get());

	// 에디터의 도움을 받고싶은 오브젝트들 Raw 포인터로 푸시.
	// 	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::ForwardAlphaBlend).get());


	//몬스터 초기상태 idle
	m_pMesh->PlayAnimation("Idle", true);

	//몬스터 회전 기본 속도
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
	// 현재 스케일과 회전은 의미가 없음 DeltaPos 로 트랜스폼에서 통제 . 
	auto [DeltaScale, DeltaQuat, DeltaPos] = m_pMesh->Update(_fDeltaTime);
	Vector3 Axis = { 1,0,0 };

	//ENGINE::AnimNotify _Notify{};
	////return true 면 이제 호출 안함, false면 저 루프 돌떄 계속 호출.
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
	//플레이어가 사라졌는지 판단
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
	//왼쪽 오른쪽 구분하기 위한 외적 결과 값
	Vector3  vCross;
	D3DXVec3Cross(&vCross, &vLook, &vDir);
	//앞 뒤 구분하기 위한 내적 결과 값
	float fDot = D3DXVec3Dot(&vDir, &vLook);


	//
	//fDot> 0 //플레이어가 앞에 있다.
	//fDot< 0 //플레이어가 뒤에 있다.
	//vCross.y> 0 //플레이어가 왼쪽에 있다
	//vCross.y< 0 // 플레이어가 오른쪽에 있다

	//플레이어가 앞에 있어요
	if (fDot > 0)
	{
		//플레이어가 왼쪽에 있어요
		if (vCross.y > 0)
			m_eState = Move_Turn_L;
		//플레이어가 오른쪽에 있어요
		else
			m_eState = Move_Turn_R;
	}
	//플레이어가 뒤에 있어요
	else
	{
		if (vCross.y > 0)
			m_eState = Move_Turn_L;
		//플레이어가 오른쪽에 있어요
		else
			m_eState = Move_Turn_R;
	}


}
