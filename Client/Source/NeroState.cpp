#include "stdafx.h"
#include "NeroState.h"
#include "Nero.h"
#include "NeroFSM.h"


#pragma region PARENT


NeroState::NeroState(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:FSMState(_pFSM,_nIndex)
	, m_pNero(_pNero)
{
	//애니메이터 가져올거면 여기서
}

NeroState::~NeroState()
{
}

HRESULT NeroState::StateEnter()
{
	FSMState::StateEnter();

	return S_OK;
}

HRESULT NeroState::StateExit()
{
	FSMState::StateExit();
	return S_OK;
}

HRESULT NeroState::StateUpdate(const float _fDeltaTime)
{
	FSMState::StateUpdate(_fDeltaTime);
	return S_OK;
}

HRESULT NeroState::KeyInput_Idle(const int _nIndex)
{
	UINT CurrIndex = m_pFSM->GetCurrentIndex();

	if (Input::GetKey(DIK_LSHIFT))
	{
		//락온
		if (Input::GetKey(DIK_W))
		{
			if (Input::GetMouse(DIM_L))
				m_pFSM->ChangeState(NeroFSM::SKILL_STREAK);
			else if (Input::GetMouse(DIM_R))
			{
				//총알 발사
			}
			else if (Input::GetMouse(DIM_M))
			{
				m_pFSM->ChangeState(NeroFSM::OVERTURE_SHOOT_UP);
			}
		}
		else if (Input::GetKey(DIK_S))
		{
			//락온한 상태로 뒤로
			if (Input::GetMouse(DIM_L))
				m_pFSM->ChangeState(NeroFSM::SKILL_FLOAT_GROUND);
			else if(Input::GetKey(DIK_W) && Input::GetMouse(DIM_L))
				m_pFSM->ChangeState(NeroFSM::SKILL_SHUFFLE);
			else if (Input::GetMouse(DIM_R))
			{
				//총알 발사
			}
			else if (Input::GetMouse(DIM_M))
			{
				m_pFSM->ChangeState(NeroFSM::OVERTURE_SHOOT_DOWN);
			}
		}
		else if (Input::GetKey(DIK_A))
		{
			if (Input::GetKey(DIK_SPACE))
			{
				//왼쪽 구르기
			}
		}
		else if (Input::GetKey(DIK_D))
		{
			if (Input::GetKey(DIK_SPACE))
			{
				// 오른쪽 구르기
			}
		}
	}

	else if (Input::GetKey(DIK_W))
	{
		m_pFSM->ChangeState(NeroFSM::RUNSTART);
	}
	else if (Input::GetKey(DIK_S))
	{
		//180도 회전
		m_pFSM->ChangeState(NeroFSM::RUNSTART_180);
	}
	else if (Input::GetKey(DIK_A))
	{
		//왼쪽으로 90도 회전
		m_pFSM->ChangeState(NeroFSM::RUNSTART_L);
	}
	else if (Input::GetKey(DIK_D))
	{
		//오른쪽으로 90도 회전
		m_pFSM->ChangeState(NeroFSM::RUNSTART_R);
	}
	else if (Input::GetMouseDown(DIM_L))
	{
		m_pFSM->ChangeState(NeroFSM::ATT1);
	}
	else if (Input::GetMouseDown(DIM_M))
	{
		m_pFSM->ChangeState(NeroFSM::OVERTURE_SHOOT);
	}
	else if (Input::GetKeyDown(DIK_SPACE))
	{
		//점프
	}
	return S_OK;
}

HRESULT NeroState::KeyInput_Run(const int _nIndex)
{
	UINT CurrIndex = m_pFSM->GetCurrentIndex();

	if (Input::GetMouseDown(DIM_L))
	{
		//달려가면서 공격하는걸로
		//m_pFSM->ChangeState(NeroFSM::ATT1_Run);
		
	}
	else if (Input::GetKeyDown(DIK_SPACE))
	{
		//앞으로 뛰는 점프
	}

	return S_OK;
}

HRESULT NeroState::PutWeapon()
{
	if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE_BATTLE);
	}
	return S_OK;
}

#pragma endregion

#pragma region IDLE



Idle::Idle(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM,_nIndex,_pNero)
{
}

Idle::~Idle()
{
}

Idle* Idle::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Idle(_pFSM,_nIndex,_pNero);
}

HRESULT Idle::StateEnter()
{
	NeroState::StateEnter();
	UINT CurAnimationIndex = m_pNero.lock()->Get_CurAnimationIndex();
	UINT PreAnimationIndex = m_pNero.lock()->Get_PreAnimationIndex();

	switch (CurAnimationIndex)
	{
	case Nero::ANI_IDLE_FROM_COMBOA1:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1_Loop", true, Nero::ANI_IDLE_FROM_COMBOA1_LOOP);
		break;
	case Nero::ANI_IDLE_FROM_COMBOA2:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA2_Loop", true, Nero::ANI_IDLE_FROM_COMBOA2_LOOP);
		break;
	case Nero::ANI_COMBOA3:
		//m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1_Loop", true, Nero::ANI_IDLE_FROM_COMBOA1_LOOP);
		break;
	case Nero::ANI_COMBOA4:
		//m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1_Loop", true, Nero::ANI_IDLE_FROM_COMBOA1_LOOP);
		break;
	case Nero::ANI_COMBOC1:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA2_Loop", true, Nero::ANI_IDLE_FROM_COMBOA2_LOOP);
		break;
	case Nero::ANI_COMBOC2:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA2_Loop", true, Nero::ANI_IDLE_FROM_COMBOA2_LOOP);
		break;
	case Nero::ANI_COMBOC3:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA2_Loop", true, Nero::ANI_IDLE_FROM_COMBOA2_LOOP);
		break;
	case Nero::ANI_COMBOC4:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA2_Loop", true, Nero::ANI_IDLE_FROM_COMBOA2_LOOP);
		break;
	case Nero::ANI_COMBOC_R:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA2_Loop", true, Nero::ANI_IDLE_FROM_COMBOA2_LOOP);
		break;
	case Nero::ANI_COMBOC_L:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA2_Loop", true, Nero::ANI_IDLE_FROM_COMBOA2_LOOP);
		break;
	case Nero::ANI_COMBOD1:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1_Loop", true, Nero::ANI_IDLE_FROM_COMBOA1_LOOP);
		break;
	case Nero::ANI_COMBOD2:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1_Loop", true, Nero::ANI_IDLE_FROM_COMBOA1_LOOP);
		break;
	case Nero::ANI_COMBOD3:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1_Loop", true, Nero::ANI_IDLE_FROM_COMBOA1_LOOP);
		break;
	case Nero::ANI_COMBOD4:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1_Loop", true, Nero::ANI_IDLE_FROM_COMBOA1_LOOP);
		break;
	case Nero::ANI_HR_GROUND:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1_Loop", true, Nero::ANI_IDLE_FROM_COMBOA1_LOOP);
		break;
	case Nero::ANI_HR_EX_FINISH:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1_Loop", true, Nero::ANI_IDLE_FROM_COMBOA1_LOOP);
		break;
	case Nero::ANI_STREAK_END:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1_Loop", true, Nero::ANI_IDLE_FROM_COMBOA1_LOOP);
		break;
	case Nero::ANI_STREAK_EX_ROLL_END:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1_Loop", true, Nero::ANI_IDLE_FROM_COMBOA1_LOOP);
		break;
	case Nero::ANI_HITFRONT:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1_Loop", true, Nero::ANI_IDLE_FROM_COMBOA1_LOOP);
		break;
	case Nero::ANI_SHUFFLE:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA2_Loop", true, Nero::ANI_IDLE_FROM_COMBOA2_LOOP);
		break;
	case Nero::ANI_SHUFFLE_EX:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA2_Loop", true, Nero::ANI_IDLE_FROM_COMBOA2_LOOP);
		break;
	case Nero::ANI_OVERTURE_SHOOT:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_OVERTURE_SHOOT_DOWN:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_OVERTURE_SHOOT_UP:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA2_Loop", true, Nero::ANI_IDLE_FROM_COMBOA2_LOOP);
		break;
	case Nero::ANI_JUMP_LANDING:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_JUMP_FRONT_LANDING:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_GT_BOMB:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_GT_CRASH_JUST:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_GT_CRASH_RELOAD:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_GT_RELOAD:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_GT_RELOAD_STYLISH:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_WIRE_SNATCH_PULL:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_WIRE_SNATCH_PULL_U:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_WIRE_SNATCH_PULL_D:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_EVADE_L:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_EVADE_R:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_STUN_END:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_RUNSTOP:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	case Nero::ANI_DASHSTOP:
		m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
		break;
	default:
		m_pNero.lock()->ChangeAnimation("Idle_Normal", true,Nero::ANI_IDLE_NORMAL);
		break;
	}

	
	return S_OK;
}

HRESULT Idle::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT Idle::StateUpdate(const float _fDeltaTime)
{
	NeroState::StateUpdate(_fDeltaTime);
	NeroState::KeyInput_Idle();

	return S_OK;
}

Idle_Start::Idle_Start(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Idle_Start::~Idle_Start()
{
}

Idle_Start* Idle_Start::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Idle_Start(_pFSM, _nIndex, _pNero);
}

HRESULT Idle_Start::StateEnter()
{
	NeroState::StateEnter();

	UINT CurAnimationIndex = m_pNero.lock()->Get_CurAnimationIndex();
	UINT PreAnimationIndex = m_pNero.lock()->Get_PreAnimationIndex();

	switch (CurAnimationIndex)
	{
	case Nero::ANI_COMBOA1:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1", false, Nero::ANI_IDLE_FROM_COMBOA1);
		break;
	case Nero::ANI_COMBOA2:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA2", false, Nero::ANI_IDLE_FROM_COMBOA2);
		break;
	case Nero::ANI_COMBOA3:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1", false, Nero::ANI_IDLE_FROM_COMBOA1);
		break;
	case Nero::ANI_COMBOA4:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1", false, Nero::ANI_IDLE_FROM_COMBOA1);
		break;
	default:
		m_pNero.lock()->ChangeAnimation("Idle_From_ComboA1", false, Nero::ANI_IDLE_FROM_COMBOA1);
		break;
	}
	return S_OK;
}

HRESULT Idle_Start::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT Idle_Start::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();
	UINT CurAnimationIndex = m_pNero.lock()->Get_CurAnimationIndex();
	UINT PreAnimationIndex = m_pNero.lock()->Get_PreAnimationIndex();

	if (Input::GetMouseDown(DIM_L))
	{
		switch (CurAnimationIndex)
		{
		case Nero::ANI_IDLE_FROM_COMBOA1:
			if (fCurrAnimationTime <= 0.5f)
			{
				// 레드퀸 콤보 땅바닥 찍는거 시작 
				m_pFSM->ChangeState(NeroFSM::ATT_COMBO_C1);
				return S_OK;
			}
			break;
		case Nero::ANI_IDLE_FROM_COMBOA2:
			if (fCurrAnimationTime <= 0.5f)
			{
				m_pFSM->ChangeState(NeroFSM::ATT_COMBO_D1);
				return S_OK;
			}
			break;

		default:
			break;
		}
	}

	else if (0.95f <= fCurrAnimationTime)
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}
	NeroState::KeyInput_Idle();
	return S_OK;
}

Idle_Battle::Idle_Battle(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Idle_Battle::~Idle_Battle()
{
}

Idle_Battle* Idle_Battle::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Idle_Battle(_pFSM, _nIndex, _pNero);
}

HRESULT Idle_Battle::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("Idle_Battle", true, Nero::ANI_IDLE_BATTLE);
	return S_OK;
}

HRESULT Idle_Battle::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT Idle_Battle::StateUpdate(const float _fDeltaTime)
{
	//런스타트 배틀
	NeroState::StateUpdate(_fDeltaTime);
	NeroState::KeyInput_Idle();
	return S_OK;
}
#pragma endregion

#pragma region JUMP

Jump_Basic::Jump_Basic(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Jump_Basic::~Jump_Basic()
{
}

Jump_Basic* Jump_Basic::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Jump_Basic(_pFSM, _nIndex, _pNero);
}

HRESULT Jump_Basic::StateEnter()
{
	return S_OK;
}

HRESULT Jump_Basic::StateExit()
{
	return S_OK;
}

HRESULT Jump_Basic::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Jump_Fly_Loop::Jump_Fly_Loop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Jump_Fly_Loop::~Jump_Fly_Loop()
{
}

Jump_Fly_Loop* Jump_Fly_Loop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Jump_Fly_Loop(_pFSM, _nIndex, _pNero);
}

HRESULT Jump_Fly_Loop::StateEnter()
{
	return S_OK;
}

HRESULT Jump_Fly_Loop::StateExit()
{
	return S_OK;
}

HRESULT Jump_Fly_Loop::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Jump_Landing::Jump_Landing(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Jump_Landing::~Jump_Landing()
{
}

Jump_Landing* Jump_Landing::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Jump_Landing(_pFSM, _nIndex, _pNero);
}

HRESULT Jump_Landing::StateEnter()
{
	return S_OK;
}

HRESULT Jump_Landing::StateExit()
{
	return S_OK;
}

HRESULT Jump_Landing::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}


Jump_Landing_High::Jump_Landing_High(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Jump_Landing_High::~Jump_Landing_High()
{
}

Jump_Landing_High* Jump_Landing_High::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Jump_Landing_High(_pFSM, _nIndex, _pNero);
}

HRESULT Jump_Landing_High::StateEnter()
{
	return S_OK;
}

HRESULT Jump_Landing_High::StateExit()
{
	return S_OK;
}

HRESULT Jump_Landing_High::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Jump_Twice::Jump_Twice(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Jump_Twice::~Jump_Twice()
{
}

Jump_Twice* Jump_Twice::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Jump_Twice(_pFSM, _nIndex, _pNero);
}

HRESULT Jump_Twice::StateEnter()
{
	return S_OK;
}

HRESULT Jump_Twice::StateExit()
{
	return S_OK;
}

HRESULT Jump_Twice::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Jump_Front::Jump_Front(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Jump_Front::~Jump_Front()
{
}

Jump_Front* Jump_Front::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Jump_Front(_pFSM, _nIndex, _pNero);
}

HRESULT Jump_Front::StateEnter()
{
	return S_OK;
}

HRESULT Jump_Front::StateExit()
{
	return S_OK;
}

HRESULT Jump_Front::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Jump_Front_Landing::Jump_Front_Landing(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Jump_Front_Landing::~Jump_Front_Landing()
{
}

Jump_Front_Landing* Jump_Front_Landing::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Jump_Front_Landing(_pFSM, _nIndex, _pNero);
}

HRESULT Jump_Front_Landing::StateEnter()
{
	return S_OK;
}

HRESULT Jump_Front_Landing::StateExit()
{
	return S_OK;
}

HRESULT Jump_Front_Landing::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Jump_Front_Twice::Jump_Front_Twice(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Jump_Front_Twice::~Jump_Front_Twice()
{
}

Jump_Front_Twice* Jump_Front_Twice::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Jump_Front_Twice(_pFSM, _nIndex, _pNero);
}

HRESULT Jump_Front_Twice::StateEnter()
{
	return S_OK;
}

HRESULT Jump_Front_Twice::StateExit()
{
	return S_OK;
}

HRESULT Jump_Front_Twice::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Jump_Back::Jump_Back(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Jump_Back::~Jump_Back()
{
}

Jump_Back* Jump_Back::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Jump_Back(_pFSM, _nIndex, _pNero);
}

HRESULT Jump_Back::StateEnter()
{
	return S_OK;
}

HRESULT Jump_Back::StateExit()
{
	return S_OK;
}

HRESULT Jump_Back::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Jump_Back_Twice::Jump_Back_Twice(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Jump_Back_Twice::~Jump_Back_Twice()
{
}

Jump_Back_Twice* Jump_Back_Twice::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Jump_Back_Twice(_pFSM, _nIndex, _pNero);
}

HRESULT Jump_Back_Twice::StateEnter()
{
	return S_OK;
}

HRESULT Jump_Back_Twice::StateExit()
{
	return S_OK;
}

HRESULT Jump_Back_Twice::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

#pragma endregion JUMP

#pragma region RUN

RunStartLeft::RunStartLeft(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

RunStartLeft::~RunStartLeft()
{
}

RunStartLeft* RunStartLeft::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new RunStartLeft(_pFSM, _nIndex, _pNero);
}

HRESULT RunStartLeft::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("RunStart270", false,Nero::ANI_RUNSTART270);

	return S_OK;
}

HRESULT RunStartLeft::StateExit()
{
	//끝날때 회전
	NeroState::StateExit();
	return S_OK;
}

HRESULT RunStartLeft::StateUpdate(const float _fDeltaTime)
{
	if (Input::GetKey(DIK_A))
	{
		if (m_pNero.lock()->IsAnimationEnd())
		{
			m_pFSM->ChangeState(NeroFSM::RUNLOOP);
		}
	}
	else
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}
	return S_OK;
}

RunStartRight::RunStartRight(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

RunStartRight::~RunStartRight()
{
}

RunStartRight* RunStartRight::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new RunStartRight(_pFSM, _nIndex, _pNero);
}

HRESULT RunStartRight::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("RunStart90", false, Nero::ANI_RUNSTART90);


	return S_OK;
}

HRESULT RunStartRight::StateExit()
{
	//끝날때 회전
	NeroState::StateExit();

	return S_OK;
}

HRESULT RunStartRight::StateUpdate(const float _fDeltaTime)
{
	if (Input::GetKey(DIK_D))
	{
		if (m_pNero.lock()->IsAnimationEnd())
		{
			m_pFSM->ChangeState(NeroFSM::RUNLOOP);
		}
	}
	else
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}

	return S_OK;
}

RunStart180::RunStart180(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

RunStart180::~RunStart180()
{
}

RunStart180* RunStart180::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new RunStart180(_pFSM, _nIndex, _pNero);
}

HRESULT RunStart180::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("RunStart180", false,Nero::ANI_RUNSTART180);

	return S_OK;
}

HRESULT RunStart180::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT RunStart180::StateUpdate(const float _fDeltaTime)
{
	if (Input::GetKey(DIK_S))
	{
		if (m_pNero.lock()->IsAnimationEnd())
		{
			m_pFSM->ChangeState(NeroFSM::RUNLOOP);
		}
	}
	else
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}
	return S_OK;
}



RunLoop::RunLoop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

RunLoop::~RunLoop()
{
}

RunLoop* RunLoop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new RunLoop(_pFSM, _nIndex, _pNero);
}

HRESULT RunLoop::StateEnter()
{
	NeroState::StateEnter();
	UINT CurAnimationIndex = m_pNero.lock()->Get_CurAnimationIndex();
	UINT PreAnimationIndex = m_pNero.lock()->Get_PreAnimationIndex();

	switch (CurAnimationIndex)
	{
	case Nero::ANI_RUNSTART_FROM_COMBOA1:
		m_pNero.lock()->ChangeAnimation("RunLoop_From_ComboA1", true, Nero::ANI_RUNLOOP_FROM_COMBOA1);
		break;
	case Nero::ANI_RUNSTART_FROM_COMBOA2:
		m_pNero.lock()->ChangeAnimation("RunLoop_From_ComboA1", true, Nero::ANI_RUNLOOP_FROM_COMBOA1);
		break;
	default:
		m_pNero.lock()->ChangeAnimation("RunLoop", true, Nero::ANI_RUNLOOP);
		break;
	}

	

	return S_OK;
}

HRESULT RunLoop::StateExit()
{
	NeroState::StateExit();

	return S_OK;
}

HRESULT RunLoop::StateUpdate(const float _fDeltaTime)
{
	if (Input::GetKey(DIK_W) || Input::GetKey(DIK_S) || Input::GetKey(DIK_A) || Input::GetKey(DIK_D))
	{
		//키입력이 특정 시간이 넘었다
		//-> 그러면 대쉬 루프로 변환하고
		//칼들고 달리고있었으면 칼 집어넣고

	}
	else
	{
		//그냥 달리는거면 이거고
		m_pFSM->ChangeState(NeroFSM::RUNSTOP);
		//칼들고 달리는거면 멈추면서 칼만 넣는거

	}
	return S_OK;
}

RunStartFront::RunStartFront(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

RunStartFront::~RunStartFront()
{
}

RunStartFront* RunStartFront::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new RunStartFront(_pFSM, _nIndex, _pNero);
}

HRESULT RunStartFront::StateEnter()
{
	NeroState::StateEnter();
	//앞으로 달리는 애니메이션으로 세팅
	UINT CurAnimationIndex = m_pNero.lock()->Get_CurAnimationIndex();
	UINT PreAnimationIndex = m_pNero.lock()->Get_PreAnimationIndex();

	switch (CurAnimationIndex)
	{
	case Nero::ANI_IDLE_NORMAL:
		m_pNero.lock()->ChangeAnimation("RunStart0", false, Nero::ANI_RUNSTART0);
		break;
	case Nero::ANI_IDLE_BATTLE:
		m_pNero.lock()->ChangeAnimation("RunStart0", false,Nero::ANI_RUNSTART0);
		break;
	case Nero::ANI_IDLE_FROM_COMBOA1:
		m_pNero.lock()->ChangeAnimation("RunStart_From_ComboA1", false, Nero::ANI_RUNSTART_FROM_COMBOA1);
		break;
	case Nero::ANI_IDLE_FROM_COMBOA1_LOOP:
		m_pNero.lock()->ChangeAnimation("RunStart_From_ComboA1", false, Nero::ANI_RUNSTART_FROM_COMBOA1);
		break;
	case Nero::ANI_IDLE_FROM_COMBOA1_END:
		m_pNero.lock()->ChangeAnimation("RunStart_From_ComboA1", false, Nero::ANI_RUNSTART_FROM_COMBOA1);
		break;
	case Nero::ANI_IDLE_FROM_COMBOA2:
		m_pNero.lock()->ChangeAnimation("RunStart_From_ComboA2", false, Nero::ANI_RUNSTART_FROM_COMBOA2);
		break;
	case Nero::ANI_IDLE_FROM_COMBOA2_LOOP:
		m_pNero.lock()->ChangeAnimation("RunStart_From_ComboA2", false, Nero::ANI_RUNSTART_FROM_COMBOA2);
		break;												 
	case Nero::ANI_IDLE_FROM_COMBOA2_END:					 
		m_pNero.lock()->ChangeAnimation("RunStart_From_ComboA2", false, Nero::ANI_RUNSTART_FROM_COMBOA2);
		break;
		
	}
	

	return S_OK;
}

HRESULT RunStartFront::StateExit()
{
	NeroState::StateExit();

	return S_OK;
}

HRESULT RunStartFront::StateUpdate(const float _fDeltaTime)
{
	// 시작하는 애니메니메이션이 끝났으면 Loop로 변환
	//if (Input::GetKey(DIK_W) && m_pNero.lock()->IsAnimationEnd())
	//{
	//	m_pFSM->ChangeState(NeroFSM::RUNLOOP);
	//}
	//else if(m_pNero.lock()->IsAnimationEnd())
	//	m_pFSM->ChangeState(NeroFSM::IDLE);
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();
	if (Input::GetKey(DIK_W))
	{
		if (0.98 <= fCurrAnimationTime)
		{
			m_pFSM->ChangeState(NeroFSM::RUNLOOP);
		}
	}
	else
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}

	return S_OK;
}

RunStop::RunStop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

RunStop::~RunStop()
{
}

RunStop* RunStop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new RunStop(_pFSM, _nIndex, _pNero);
}

HRESULT RunStop::StateEnter()
{
	NeroState::StateEnter();
	//상태에 따라서
	m_pNero.lock()->ChangeAnimation("RunStop", false,Nero::ANI_RUNSTOP);

	return S_OK;
}

HRESULT RunStop::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT RunStop::StateUpdate(const float _fDeltaTime)
{
	if (m_pNero.lock()->IsAnimationEnd())
		m_pFSM->ChangeState(NeroFSM::IDLE);

	return S_OK;
}

RunTurn::RunTurn(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

RunTurn::~RunTurn()
{
}

RunTurn* RunTurn::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new RunTurn(_pFSM, _nIndex, _pNero);
}

HRESULT RunTurn::StateEnter()
{
	return S_OK;
}

HRESULT RunTurn::StateExit()
{
	return S_OK;
}

HRESULT RunTurn::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

RunTurn_L::RunTurn_L(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

RunTurn_L::~RunTurn_L()
{
}

RunTurn_L* RunTurn_L::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new RunTurn_L(_pFSM, _nIndex, _pNero);
}

HRESULT RunTurn_L::StateEnter()
{
	return S_OK;
}

HRESULT RunTurn_L::StateExit()
{
	return S_OK;
}

HRESULT RunTurn_L::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}
#pragma endregion

#pragma region DASH



DashLoop::DashLoop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

DashLoop::~DashLoop()
{
}

DashLoop* DashLoop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new DashLoop(_pFSM, _nIndex, _pNero);
}

HRESULT DashLoop::StateEnter()
{
	return S_OK;
}

HRESULT DashLoop::StateExit()
{
	return S_OK;
}

HRESULT DashLoop::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

DashStop::DashStop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

DashStop::~DashStop()
{
}

DashStop* DashStop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new DashStop(_pFSM, _nIndex, _pNero);
}

HRESULT DashStop::StateEnter()
{
	return S_OK;
}

HRESULT DashStop::StateExit()
{
	return S_OK;
}

HRESULT DashStop::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

DashTurn::DashTurn(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

DashTurn::~DashTurn()
{
}

DashTurn* DashTurn::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new DashTurn(_pFSM, _nIndex, _pNero);
}

HRESULT DashTurn::StateEnter()
{
	return S_OK;
}

HRESULT DashTurn::StateExit()
{
	return S_OK;
}

HRESULT DashTurn::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

#pragma endregion

#pragma region GUN_WALK



#pragma endregion

#pragma region WALK




WalkLoop::WalkLoop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

WalkLoop::~WalkLoop()
{
}

WalkLoop* WalkLoop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new WalkLoop(_pFSM, _nIndex, _pNero);
}

HRESULT WalkLoop::StateEnter()
{
	return S_OK;
}

HRESULT WalkLoop::StateExit()
{
	return S_OK;
}

HRESULT WalkLoop::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

WalkStart::WalkStart(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

WalkStart::~WalkStart()
{
}

WalkStart* WalkStart::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new WalkStart(_pFSM, _nIndex, _pNero);
}

HRESULT WalkStart::StateEnter()
{
	return S_OK;
}

HRESULT WalkStart::StateExit()
{
	return S_OK;
}

HRESULT WalkStart::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}


WalkStop::WalkStop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

WalkStop::~WalkStop()
{
}

WalkStop* WalkStop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new WalkStop(_pFSM, _nIndex, _pNero);
}

HRESULT WalkStop::StateEnter()
{
	return S_OK;
}

HRESULT WalkStop::StateExit()
{
	return S_OK;
}

HRESULT WalkStop::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

#pragma endregion

#pragma region EVADE



Evade_L::Evade_L(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Evade_L::~Evade_L()
{
}

Evade_L* Evade_L::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Evade_L(_pFSM, _nIndex, _pNero);
}

HRESULT Evade_L::StateEnter()
{
	return S_OK;
}

HRESULT Evade_L::StateExit()
{
	return S_OK;
}

HRESULT Evade_L::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Evade_R::Evade_R(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Evade_R::~Evade_R()
{
}

Evade_R* Evade_R::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Evade_R(_pFSM, _nIndex, _pNero);
}

HRESULT Evade_R::StateEnter()
{
	return S_OK;
}

HRESULT Evade_R::StateExit()
{
	return S_OK;
}

HRESULT Evade_R::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}


#pragma endregion


#pragma region HIT




HitFront::HitFront(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

HitFront::~HitFront()
{
}

HitFront* HitFront::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new HitFront(_pFSM, _nIndex, _pNero);
}

HRESULT HitFront::StateEnter()
{
	return S_OK;
}

HRESULT HitFront::StateExit()
{
	return S_OK;
}

HRESULT HitFront::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

HitFront_Big::HitFront_Big(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

HitFront_Big::~HitFront_Big()
{
}

HitFront_Big* HitFront_Big::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new HitFront_Big(_pFSM, _nIndex, _pNero);
}

HRESULT HitFront_Big::StateEnter()
{
	return S_OK;
}

HRESULT HitFront_Big::StateExit()
{
	return S_OK;
}

HRESULT HitFront_Big::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Hit_Air::Hit_Air(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Hit_Air::~Hit_Air()
{
}

Hit_Air* Hit_Air::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Hit_Air(_pFSM, _nIndex, _pNero);
}

HRESULT Hit_Air::StateEnter()
{
	return S_OK;
}

HRESULT Hit_Air::StateExit()
{
	return S_OK;
}

HRESULT Hit_Air::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Wind_Pressure_Small::Wind_Pressure_Small(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wind_Pressure_Small::~Wind_Pressure_Small()
{
}

Wind_Pressure_Small* Wind_Pressure_Small::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wind_Pressure_Small(_pFSM, _nIndex, _pNero);
}

HRESULT Wind_Pressure_Small::StateEnter()
{
	return S_OK;
}

HRESULT Wind_Pressure_Small::StateExit()
{
	return S_OK;
}

HRESULT Wind_Pressure_Small::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Wind_Pressure_Big::Wind_Pressure_Big(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wind_Pressure_Big::~Wind_Pressure_Big()
{
}

Wind_Pressure_Big* Wind_Pressure_Big::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wind_Pressure_Big(_pFSM, _nIndex, _pNero);
}

HRESULT Wind_Pressure_Big::StateEnter()
{
	return S_OK;
}

HRESULT Wind_Pressure_Big::StateExit()
{
	return S_OK;
}

HRESULT Wind_Pressure_Big::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Wind_Pressure_Small_End::Wind_Pressure_Small_End(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wind_Pressure_Small_End::~Wind_Pressure_Small_End()
{
}

Wind_Pressure_Small_End* Wind_Pressure_Small_End::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wind_Pressure_Small_End(_pFSM, _nIndex, _pNero);
}

HRESULT Wind_Pressure_Small_End::StateEnter()
{
	return S_OK;
}

HRESULT Wind_Pressure_Small_End::StateExit()
{
	return S_OK;
}

HRESULT Wind_Pressure_Small_End::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Wind_Pressure_Big_End::Wind_Pressure_Big_End(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wind_Pressure_Big_End::~Wind_Pressure_Big_End()
{
}

Wind_Pressure_Big_End* Wind_Pressure_Big_End::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wind_Pressure_Big_End(_pFSM, _nIndex, _pNero);
}

HRESULT Wind_Pressure_Big_End::StateEnter()
{
	return S_OK;
}

HRESULT Wind_Pressure_Big_End::StateExit()
{
	return S_OK;
}

HRESULT Wind_Pressure_Big_End::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

KnockBack_Front::KnockBack_Front(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

KnockBack_Front::~KnockBack_Front()
{
}

KnockBack_Front* KnockBack_Front::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new KnockBack_Front(_pFSM, _nIndex, _pNero);
}

HRESULT KnockBack_Front::StateEnter()
{
	return S_OK;
}

HRESULT KnockBack_Front::StateExit()
{
	return S_OK;
}

HRESULT KnockBack_Front::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

KnockBack_Get_Up::KnockBack_Get_Up(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

KnockBack_Get_Up::~KnockBack_Get_Up()
{
}

KnockBack_Get_Up* KnockBack_Get_Up::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new KnockBack_Get_Up(_pFSM, _nIndex, _pNero);
}

HRESULT KnockBack_Get_Up::StateEnter()
{
	return S_OK;
}

HRESULT KnockBack_Get_Up::StateExit()
{
	return S_OK;
}

HRESULT KnockBack_Get_Up::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

#pragma endregion

#pragma region DIE




Die::Die(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Die::~Die()
{
}

Die* Die::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Die(_pFSM, _nIndex, _pNero);
}

HRESULT Die::StateEnter()
{
	return S_OK;
}

HRESULT Die::StateExit()
{
	return S_OK;
}

HRESULT Die::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Resurrection::Resurrection(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Resurrection::~Resurrection()
{
}

Resurrection* Resurrection::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Resurrection(_pFSM, _nIndex, _pNero);
}

HRESULT Resurrection::StateEnter()
{
	return S_OK;
}

HRESULT Resurrection::StateExit()
{
	return S_OK;
}

HRESULT Resurrection::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Resurrection_GetUp::Resurrection_GetUp(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Resurrection_GetUp::~Resurrection_GetUp()
{
}

Resurrection_GetUp* Resurrection_GetUp::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Resurrection_GetUp(_pFSM, _nIndex, _pNero);
}

HRESULT Resurrection_GetUp::StateEnter()
{
	return S_OK;
}

HRESULT Resurrection_GetUp::StateExit()
{
	return S_OK;
}

HRESULT Resurrection_GetUp::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

#pragma endregion

#pragma region STUN




StunStart::StunStart(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

StunStart::~StunStart()
{
}

StunStart* StunStart::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new StunStart(_pFSM, _nIndex, _pNero);
}

HRESULT StunStart::StateEnter()
{
	return S_OK;
}

HRESULT StunStart::StateExit()
{
	return S_OK;
}

HRESULT StunStart::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

StunLoop::StunLoop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

StunLoop::~StunLoop()
{
}

StunLoop* StunLoop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new StunLoop(_pFSM, _nIndex, _pNero);
}

HRESULT StunLoop::StateEnter()
{
	return S_OK;
}

HRESULT StunLoop::StateExit()
{
	return S_OK;
}

HRESULT StunLoop::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

StunEnd::StunEnd(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

StunEnd::~StunEnd()
{
}

StunEnd* StunEnd::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new StunEnd(_pFSM, _nIndex, _pNero);
}

HRESULT StunEnd::StateEnter()
{
	return S_OK;
}

HRESULT StunEnd::StateExit()
{
	return S_OK;
}

HRESULT StunEnd::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

EarthQuakeStart::EarthQuakeStart(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

EarthQuakeStart::~EarthQuakeStart()
{
}

EarthQuakeStart* EarthQuakeStart::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new EarthQuakeStart(_pFSM, _nIndex, _pNero);
}

HRESULT EarthQuakeStart::StateEnter()
{
	return S_OK;
}

HRESULT EarthQuakeStart::StateExit()
{
	return S_OK;
}

HRESULT EarthQuakeStart::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

EarthQuakeLoop::EarthQuakeLoop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

EarthQuakeLoop::~EarthQuakeLoop()
{
}

EarthQuakeLoop* EarthQuakeLoop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new EarthQuakeLoop(_pFSM, _nIndex, _pNero);
}

HRESULT EarthQuakeLoop::StateEnter()
{
	return S_OK;
}

HRESULT EarthQuakeLoop::StateExit()
{
	return S_OK;
}

HRESULT EarthQuakeLoop::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

EarthQuakeEnd::EarthQuakeEnd(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

EarthQuakeEnd::~EarthQuakeEnd()
{
}

EarthQuakeEnd* EarthQuakeEnd::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new EarthQuakeEnd(_pFSM, _nIndex, _pNero);
}

HRESULT EarthQuakeEnd::StateEnter()
{
	return S_OK;
}

HRESULT EarthQuakeEnd::StateExit()
{
	return S_OK;
}

HRESULT EarthQuakeEnd::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

#pragma endregion

#pragma region WIRE




Wire_Pull::Wire_Pull(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wire_Pull::~Wire_Pull()
{
}

Wire_Pull* Wire_Pull::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wire_Pull(_pFSM, _nIndex, _pNero);
}

HRESULT Wire_Pull::StateEnter()
{
	return S_OK;
}

HRESULT Wire_Pull::StateExit()
{
	return S_OK;
}

HRESULT Wire_Pull::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Wire_Pull_Up::Wire_Pull_Up(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wire_Pull_Up::~Wire_Pull_Up()
{
}

Wire_Pull_Up* Wire_Pull_Up::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wire_Pull_Up(_pFSM, _nIndex, _pNero);
}

HRESULT Wire_Pull_Up::StateEnter()
{
	return S_OK;
}

HRESULT Wire_Pull_Up::StateExit()
{
	return S_OK;
}

HRESULT Wire_Pull_Up::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Wire_Pull_Down::Wire_Pull_Down(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wire_Pull_Down::~Wire_Pull_Down()
{
}

Wire_Pull_Down* Wire_Pull_Down::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wire_Pull_Down(_pFSM, _nIndex, _pNero);
}

HRESULT Wire_Pull_Down::StateEnter()
{
	return S_OK;
}

HRESULT Wire_Pull_Down::StateExit()
{
	return S_OK;
}

HRESULT Wire_Pull_Down::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Wire_Fly_Start::Wire_Fly_Start(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wire_Fly_Start::~Wire_Fly_Start()
{
}

Wire_Fly_Start* Wire_Fly_Start::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wire_Fly_Start(_pFSM, _nIndex, _pNero);
}

HRESULT Wire_Fly_Start::StateEnter()
{
	return S_OK;
}

HRESULT Wire_Fly_Start::StateExit()
{
	return S_OK;
}

HRESULT Wire_Fly_Start::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Wire_Fly_Loop::Wire_Fly_Loop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wire_Fly_Loop::~Wire_Fly_Loop()
{
}

Wire_Fly_Loop* Wire_Fly_Loop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wire_Fly_Loop(_pFSM, _nIndex, _pNero);
}

HRESULT Wire_Fly_Loop::StateEnter()
{
	return S_OK;
}

HRESULT Wire_Fly_Loop::StateExit()
{
	return S_OK;
}

HRESULT Wire_Fly_Loop::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Wire_Fly_End::Wire_Fly_End(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wire_Fly_End::~Wire_Fly_End()
{
}

Wire_Fly_End* Wire_Fly_End::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wire_Fly_End(_pFSM, _nIndex, _pNero);
}

HRESULT Wire_Fly_End::StateEnter()
{
	return S_OK;
}

HRESULT Wire_Fly_End::StateExit()
{
	return S_OK;
}

HRESULT Wire_Fly_End::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Wire_Pull_Air::Wire_Pull_Air(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wire_Pull_Air::~Wire_Pull_Air()
{
}

Wire_Pull_Air* Wire_Pull_Air::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wire_Pull_Air(_pFSM, _nIndex, _pNero);
}

HRESULT Wire_Pull_Air::StateEnter()
{
	return S_OK;
}

HRESULT Wire_Pull_Air::StateExit()
{
	return S_OK;
}

HRESULT Wire_Pull_Air::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Wire_Pull_Air_Up::Wire_Pull_Air_Up(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wire_Pull_Air_Up::~Wire_Pull_Air_Up()
{
}

Wire_Pull_Air_Up* Wire_Pull_Air_Up::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wire_Pull_Air_Up(_pFSM, _nIndex, _pNero);
}

HRESULT Wire_Pull_Air_Up::StateEnter()
{
	return S_OK;
}

HRESULT Wire_Pull_Air_Up::StateExit()
{
	return S_OK;
}

HRESULT Wire_Pull_Air_Up::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Wire_Pull_Air_Down::Wire_Pull_Air_Down(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wire_Pull_Air_Down::~Wire_Pull_Air_Down()
{
}

Wire_Pull_Air_Down* Wire_Pull_Air_Down::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wire_Pull_Air_Down(_pFSM, _nIndex, _pNero);
}

HRESULT Wire_Pull_Air_Down::StateEnter()
{
	return S_OK;
}

HRESULT Wire_Pull_Air_Down::StateExit()
{
	return S_OK;
}

HRESULT Wire_Pull_Air_Down::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}


#pragma endregion

#pragma region ATT



BT_Att1::BT_Att1(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att1::~BT_Att1()
{
}

BT_Att1* BT_Att1::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att1(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att1::StateEnter()
{
	NeroState::StateEnter();
	//달리고있었으면 대쉬 ComboA로 가야됨
	m_pNero.lock()->ChangeAnimation("ComboA1", false,Nero::ANI_COMBOA1);

	return S_OK;
}

HRESULT BT_Att1::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT BT_Att1::StateUpdate(const float _fDeltaTime)
{
	//현재 애니메이션 프레임 위치
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();
	if(Input::GetMouseDown(DIM_L))
	{
		if (0.22f <= fCurrAnimationTime && fCurrAnimationTime <= 0.35f)
		{
			m_pFSM->ChangeState(NeroFSM::ATT2);
		}

		//else if (0.4f < fCurrAnimationTime && fCurrAnimationTime <= 0.8f)
		//{
		//	// 레드퀸 콤보 땅바닥 찍는거 시작 
		//	m_pFSM->ChangeState(NeroFSM::ATT_COMBO_C1);
		//}
	}
	else if (0.37f <= fCurrAnimationTime)
	{
		m_pFSM->ChangeState(NeroFSM::IDLE_START);
	}
	return S_OK;
}

BT_Att2::BT_Att2(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att2::~BT_Att2()
{
}

BT_Att2* BT_Att2::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att2(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att2::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("ComboA2", false,Nero::ANI_COMBOA2);

	return S_OK;
}

HRESULT BT_Att2::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT BT_Att2::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();
	if (Input::GetMouseDown(DIM_L))
	{
		if (0.22f <= fCurrAnimationTime && fCurrAnimationTime <= 0.35f)
		{
			m_pFSM->ChangeState(NeroFSM::ATT3);
		}
		//else if (0.4f < fCurrAnimationTime && fCurrAnimationTime <= 0.8f)
		//{
		//	// 레드퀸 콤보 앞으로 가면서 베는거
		//	m_pFSM->ChangeState(NeroFSM::ATT_COMBO_D1);
		//}
	}
	else if (0.56f <= fCurrAnimationTime)
	{
		m_pFSM->ChangeState(NeroFSM::IDLE_START);
	}

	return S_OK;
}

BT_Att3::BT_Att3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att3::~BT_Att3()
{
}

BT_Att3* BT_Att3::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att3(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att3::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("ComboA3", false, Nero::ANI_COMBOA3);
	return S_OK;
}

HRESULT BT_Att3::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT BT_Att3::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();
	if (Input::GetMouseDown(DIM_L))
	{
		if (0.22f <= fCurrAnimationTime && fCurrAnimationTime <= 0.5f)
		{
			m_pFSM->ChangeState(NeroFSM::ATT4);
		}
		else if (0.5f < fCurrAnimationTime && 0.8f <= fCurrAnimationTime)
		{
			// 레드퀸 콤보B인가 땅바닥 쎄게 찍는거
		}
	}
	//else if (m_pNero.lock()->IsAnimationEnd())
	else if (0.97 <= fCurrAnimationTime)
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}
	return S_OK;
}

BT_Att4::BT_Att4(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att4::~BT_Att4()
{
}

BT_Att4* BT_Att4::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att4(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att4::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("ComboA4", false, Nero::ANI_COMBOA4);

	return S_OK;
}

HRESULT BT_Att4::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT BT_Att4::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();
	if (Input::GetMouseDown(DIM_L))
	{

	}
	//else if (m_pNero.lock()->IsAnimationEnd())
	else if (0.97 <= fCurrAnimationTime)
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}

	return S_OK;
}

BT_Att_ComboB::BT_Att_ComboB(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att_ComboB::~BT_Att_ComboB()
{
}

BT_Att_ComboB* BT_Att_ComboB::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att_ComboB(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att_ComboB::StateEnter()
{
	return S_OK;
}

HRESULT BT_Att_ComboB::StateExit()
{
	return S_OK;
}

HRESULT BT_Att_ComboB::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

BT_Att_ComboC_R_to_L::BT_Att_ComboC_R_to_L(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att_ComboC_R_to_L::~BT_Att_ComboC_R_to_L()
{
}

BT_Att_ComboC_R_to_L* BT_Att_ComboC_R_to_L::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att_ComboC_R_to_L(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att_ComboC_R_to_L::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("ComboC_R", false, Nero::ANI_COMBOC_R);

	return S_OK;
}

HRESULT BT_Att_ComboC_R_to_L::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT BT_Att_ComboC_R_to_L::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();
	if (Input::GetMouseDown(DIM_L))
	{
		if (0.22f <= fCurrAnimationTime && fCurrAnimationTime <= 0.35f)
		{
			m_pFSM->ChangeState(NeroFSM::ATT_COMBO_C4);
		}
	}
	else if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}

	return S_OK;
}

BT_Att_ComboC_L_to_R::BT_Att_ComboC_L_to_R(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att_ComboC_L_to_R::~BT_Att_ComboC_L_to_R()
{
}

BT_Att_ComboC_L_to_R* BT_Att_ComboC_L_to_R::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att_ComboC_L_to_R(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att_ComboC_L_to_R::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("ComboC_L", false, Nero::ANI_COMBOC_L);

	return S_OK;
}

HRESULT BT_Att_ComboC_L_to_R::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT BT_Att_ComboC_L_to_R::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();
	if (Input::GetMouseDown(DIM_L))
	{
		if (0.22f <= fCurrAnimationTime && fCurrAnimationTime <= 0.35f)
		{
			m_pFSM->ChangeState(NeroFSM::ATT_COMBO_C3);
		}
	}
	else if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}

	return S_OK;
}

BT_Att_ComboC_1::BT_Att_ComboC_1(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att_ComboC_1::~BT_Att_ComboC_1()
{
}

BT_Att_ComboC_1* BT_Att_ComboC_1::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att_ComboC_1(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att_ComboC_1::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("ComboC1", false, Nero::ANI_COMBOC1);
	return S_OK;
}

HRESULT BT_Att_ComboC_1::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT BT_Att_ComboC_1::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();
	if (Input::GetMouseDown(DIM_L))
	{
		if (0.4f <= fCurrAnimationTime && fCurrAnimationTime <= 0.5f)
		{
			m_pFSM->ChangeState(NeroFSM::ATT_COMBO_C2);
		}
	}
	else if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}

	return S_OK;
}

BT_Att_ComboC_2::BT_Att_ComboC_2(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att_ComboC_2::~BT_Att_ComboC_2()
{
}

BT_Att_ComboC_2* BT_Att_ComboC_2::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att_ComboC_2(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att_ComboC_2::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("ComboC2", false, Nero::ANI_COMBOC2);

	return S_OK;
}

HRESULT BT_Att_ComboC_2::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT BT_Att_ComboC_2::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();
	if (Input::GetMouseDown(DIM_L))
	{
		if (0.22f <= fCurrAnimationTime && fCurrAnimationTime <= 0.35f)
		{
			m_pFSM->ChangeState(NeroFSM::ATT_COMBO_C_L);
		}
	}
	else if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}
	return S_OK;
}

BT_Att_ComboC_3::BT_Att_ComboC_3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att_ComboC_3::~BT_Att_ComboC_3()
{
}

BT_Att_ComboC_3* BT_Att_ComboC_3::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att_ComboC_3(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att_ComboC_3::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("ComboC3", false, Nero::ANI_COMBOC3);

	return S_OK;
}

HRESULT BT_Att_ComboC_3::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT BT_Att_ComboC_3::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();
	if (Input::GetMouseDown(DIM_L))
	{
		if (0.22f <= fCurrAnimationTime && fCurrAnimationTime <= 0.35f)
		{
			m_pFSM->ChangeState(NeroFSM::ATT_COMBO_C_R);
		}
	}
	else if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}
	return S_OK;
}

BT_Att_ComboC_4::BT_Att_ComboC_4(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att_ComboC_4::~BT_Att_ComboC_4()
{
}

BT_Att_ComboC_4* BT_Att_ComboC_4::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att_ComboC_4(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att_ComboC_4::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("ComboC4", false, Nero::ANI_COMBOC4);
	return S_OK;
}

HRESULT BT_Att_ComboC_4::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT BT_Att_ComboC_4::StateUpdate(const float _fDeltaTime)
{
	if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}
	return S_OK;
}

BT_Att_ComboD_1::BT_Att_ComboD_1(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att_ComboD_1::~BT_Att_ComboD_1()
{
}

BT_Att_ComboD_1* BT_Att_ComboD_1::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att_ComboD_1(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att_ComboD_1::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("ComboD1", false, Nero::ANI_COMBOD1);
	return S_OK;
}

HRESULT BT_Att_ComboD_1::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT BT_Att_ComboD_1::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();
	if (Input::GetMouseDown(DIM_L))
	{
		if (0.15f <= fCurrAnimationTime && fCurrAnimationTime <= 0.3f)
		{
			m_pFSM->ChangeState(NeroFSM::ATT_COMBO_D2);
		}
	}
	else if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}

	return S_OK;
}

BT_Att_ComboD_2::BT_Att_ComboD_2(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att_ComboD_2::~BT_Att_ComboD_2()
{
}

BT_Att_ComboD_2* BT_Att_ComboD_2::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att_ComboD_2(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att_ComboD_2::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("ComboD2", false, Nero::ANI_COMBOD2);
	return S_OK;
}

HRESULT BT_Att_ComboD_2::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT BT_Att_ComboD_2::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();
	if (Input::GetMouseDown(DIM_L))
	{
		if (0.14f <= fCurrAnimationTime && fCurrAnimationTime <= 0.22f)
		{
			m_pFSM->ChangeState(NeroFSM::ATT_COMBO_D3);
		}
	}
	else if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}

	return S_OK;
}

BT_Att_ComboD_3::BT_Att_ComboD_3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att_ComboD_3::~BT_Att_ComboD_3()
{
}

BT_Att_ComboD_3* BT_Att_ComboD_3::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att_ComboD_3(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att_ComboD_3::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("ComboD3", false, Nero::ANI_COMBOD3);

	return S_OK;
}

HRESULT BT_Att_ComboD_3::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT BT_Att_ComboD_3::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();
	if (Input::GetMouseDown(DIM_L))
	{
		if (0.19f <= fCurrAnimationTime && fCurrAnimationTime <= 0.27f)
		{
			m_pFSM->ChangeState(NeroFSM::ATT_COMBO_D4);
		}
	}
	else if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}

	return S_OK;
}

BT_Att_ComboD_4::BT_Att_ComboD_4(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att_ComboD_4::~BT_Att_ComboD_4()
{
}

BT_Att_ComboD_4* BT_Att_ComboD_4::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att_ComboD_4(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att_ComboD_4::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("ComboD4", false, Nero::ANI_COMBOD4);

	return S_OK;
}

HRESULT BT_Att_ComboD_4::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT BT_Att_ComboD_4::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();
	if (Input::GetMouseDown(DIM_L))
	{

	}
	else if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}
	return S_OK;
}

BT_Air_Att1::BT_Air_Att1(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Air_Att1::~BT_Air_Att1()
{
}

BT_Air_Att1* BT_Air_Att1::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Air_Att1(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Air_Att1::StateEnter()
{
	return S_OK;
}

HRESULT BT_Air_Att1::StateExit()
{
	return S_OK;
}

HRESULT BT_Air_Att1::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

BT_Air_Att2::BT_Air_Att2(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Air_Att2::~BT_Air_Att2()
{
}

BT_Air_Att2* BT_Air_Att2::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Air_Att2(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Air_Att2::StateEnter()
{
	return S_OK;
}

HRESULT BT_Air_Att2::StateExit()
{
	return S_OK;
}

HRESULT BT_Air_Att2::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

BT_Air_Att3::BT_Air_Att3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Air_Att3::~BT_Air_Att3()
{
}

BT_Air_Att3* BT_Air_Att3::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Air_Att3(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Air_Att3::StateEnter()
{
	return S_OK;
}

HRESULT BT_Air_Att3::StateExit()
{
	return S_OK;
}

HRESULT BT_Air_Att3::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

BT_Air_ComboB::BT_Air_ComboB(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Air_ComboB::~BT_Air_ComboB()
{
}

BT_Air_ComboB* BT_Air_ComboB::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Air_ComboB(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Air_ComboB::StateEnter()
{
	return S_OK;
}

HRESULT BT_Air_ComboB::StateExit()
{
	return S_OK;
}

HRESULT BT_Air_ComboB::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

BT_Att1_To_RunStart::BT_Att1_To_RunStart(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att1_To_RunStart::~BT_Att1_To_RunStart()
{
}

BT_Att1_To_RunStart* BT_Att1_To_RunStart::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att1_To_RunStart(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att1_To_RunStart::StateEnter()
{
	return S_OK;
}

HRESULT BT_Att1_To_RunStart::StateExit()
{
	return S_OK;
}

HRESULT BT_Att1_To_RunStart::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

BT_Att1_To_RunLoop::BT_Att1_To_RunLoop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att1_To_RunLoop::~BT_Att1_To_RunLoop()
{
}

BT_Att1_To_RunLoop* BT_Att1_To_RunLoop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att1_To_RunLoop(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att1_To_RunLoop::StateEnter()
{
	return S_OK;
}

HRESULT BT_Att1_To_RunLoop::StateExit()
{
	return S_OK;
}

HRESULT BT_Att1_To_RunLoop::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

BT_Att1_To_Idle::BT_Att1_To_Idle(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att1_To_Idle::~BT_Att1_To_Idle()
{
}

BT_Att1_To_Idle* BT_Att1_To_Idle::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att1_To_Idle(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att1_To_Idle::StateEnter()
{
	return S_OK;
}

HRESULT BT_Att1_To_Idle::StateExit()
{
	return S_OK;
}

HRESULT BT_Att1_To_Idle::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

BT_Att1_To_Idle_End::BT_Att1_To_Idle_End(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att1_To_Idle_End::~BT_Att1_To_Idle_End()
{
}

BT_Att1_To_Idle_End* BT_Att1_To_Idle_End::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att1_To_Idle_End(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att1_To_Idle_End::StateEnter()
{
	return S_OK;
}

HRESULT BT_Att1_To_Idle_End::StateExit()
{
	return S_OK;
}

HRESULT BT_Att1_To_Idle_End::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

BT_Att2_To_RunStart::BT_Att2_To_RunStart(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att2_To_RunStart::~BT_Att2_To_RunStart()
{
}

BT_Att2_To_RunStart* BT_Att2_To_RunStart::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att2_To_RunStart(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att2_To_RunStart::StateEnter()
{
	return S_OK;
}

HRESULT BT_Att2_To_RunStart::StateExit()
{
	return S_OK;
}

HRESULT BT_Att2_To_RunStart::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

BT_Att2_To_Idle::BT_Att2_To_Idle(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att2_To_Idle::~BT_Att2_To_Idle()
{
}

BT_Att2_To_Idle* BT_Att2_To_Idle::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att2_To_Idle(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att2_To_Idle::StateEnter()
{
	return S_OK;
}

HRESULT BT_Att2_To_Idle::StateExit()
{
	return S_OK;
}

HRESULT BT_Att2_To_Idle::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

BT_Att1_Dash::BT_Att1_Dash(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att1_Dash::~BT_Att1_Dash()
{
}

BT_Att1_Dash* BT_Att1_Dash::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Att1_Dash(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Att1_Dash::StateEnter()
{
	return S_OK;
}

HRESULT BT_Att1_Dash::StateExit()
{
	return S_OK;
}

HRESULT BT_Att1_Dash::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

BT_Parrying::BT_Parrying(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Parrying::~BT_Parrying()
{
}

BT_Parrying* BT_Parrying::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new BT_Parrying(_pFSM, _nIndex, _pNero);
}

HRESULT BT_Parrying::StateEnter()
{
	return S_OK;
}

HRESULT BT_Parrying::StateExit()
{
	return S_OK;
}

HRESULT BT_Parrying::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

#pragma endregion


#pragma region SKILL

Skill_Split::Skill_Split(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_Split::~Skill_Split()
{
}

Skill_Split* Skill_Split::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Split(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Split::StateEnter()
{
	return S_OK;
}

HRESULT Skill_Split::StateExit()
{
	return S_OK;
}

HRESULT Skill_Split::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Skill_Split_Loop::Skill_Split_Loop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_Split_Loop::~Skill_Split_Loop()
{
}

Skill_Split_Loop* Skill_Split_Loop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Split_Loop(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Split_Loop::StateEnter()
{
	return S_OK;
}

HRESULT Skill_Split_Loop::StateExit()
{
	return S_OK;
}

HRESULT Skill_Split_Loop::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Skill_Split_Landing::Skill_Split_Landing(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_Split_Landing::~Skill_Split_Landing()
{
}

Skill_Split_Landing* Skill_Split_Landing::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Split_Landing(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Split_Landing::StateEnter()
{
	return S_OK;
}

HRESULT Skill_Split_Landing::StateExit()
{
	return S_OK;
}

HRESULT Skill_Split_Landing::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Skill_Float_Ground::Skill_Float_Ground(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_Float_Ground::~Skill_Float_Ground()
{
}

Skill_Float_Ground* Skill_Float_Ground::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Float_Ground(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Float_Ground::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("Hr_Ground", false, Nero::ANI_HR_GROUND);

	return S_OK;
}

HRESULT Skill_Float_Ground::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT Skill_Float_Ground::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();

	//if (m_pNero.lock()->IsAnimationEnd())
	if(0.97 <= fCurrAnimationTime)
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}
	return S_OK;
}

Skill_Float_Ground_Ex3::Skill_Float_Ground_Ex3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_Float_Ground_Ex3::~Skill_Float_Ground_Ex3()
{
}

Skill_Float_Ground_Ex3* Skill_Float_Ground_Ex3::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Float_Ground_Ex3(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Float_Ground_Ex3::StateEnter()
{
	return S_OK;
}

HRESULT Skill_Float_Ground_Ex3::StateExit()
{
	return S_OK;
}

HRESULT Skill_Float_Ground_Ex3::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Skill_Float_Ground_Finish::Skill_Float_Ground_Finish(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_Float_Ground_Finish::~Skill_Float_Ground_Finish()
{
}

Skill_Float_Ground_Finish* Skill_Float_Ground_Finish::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Float_Ground_Finish(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Float_Ground_Finish::StateEnter()
{
	return S_OK;
}

HRESULT Skill_Float_Ground_Finish::StateExit()
{
	return S_OK;
}

HRESULT Skill_Float_Ground_Finish::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Skill_Float_Air::Skill_Float_Air(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_Float_Air::~Skill_Float_Air()
{
}

Skill_Float_Air* Skill_Float_Air::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Float_Air(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Float_Air::StateEnter()
{
	return S_OK;
}

HRESULT Skill_Float_Air::StateExit()
{
	return S_OK;
}

HRESULT Skill_Float_Air::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Skill_Float_Air_Ex3::Skill_Float_Air_Ex3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_Float_Air_Ex3::~Skill_Float_Air_Ex3()
{
}

Skill_Float_Air_Ex3* Skill_Float_Air_Ex3::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Float_Air_Ex3(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Float_Air_Ex3::StateEnter()
{
	return S_OK;
}

HRESULT Skill_Float_Air_Ex3::StateExit()
{
	return S_OK;
}

HRESULT Skill_Float_Air_Ex3::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Skill_Shuffle::Skill_Shuffle(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_Shuffle::~Skill_Shuffle()
{
}

Skill_Shuffle* Skill_Shuffle::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Shuffle(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Shuffle::StateEnter()
{
	NeroState::StateEnter();
	//EX게이지에 따라서 분기
	//게이지없을때
	m_pNero.lock()->ChangeAnimation("Shuffle", false, Nero::ANI_SHUFFLE);
	//게이지 있을때
	//m_pNero.lock()->ChangeAnimation("Shuffle_Ex", false, Nero::ANI_SHUFFLE_EX);
	return S_OK;
}

HRESULT Skill_Shuffle::StateExit()
{
	return S_OK;
}

HRESULT Skill_Shuffle::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();

	if(0.97 <= fCurrAnimationTime)
		m_pFSM->ChangeState(NeroFSM::IDLE);
	return S_OK;
}

Skill_Caliber::Skill_Caliber(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_Caliber::~Skill_Caliber()
{
}

Skill_Caliber* Skill_Caliber::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Caliber(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Caliber::StateEnter()
{
	return S_OK;
}

HRESULT Skill_Caliber::StateExit()
{
	return S_OK;
}

HRESULT Skill_Caliber::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Skill_Streak::Skill_Streak(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_Streak::~Skill_Streak()
{
}

Skill_Streak* Skill_Streak::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Streak(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Streak::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("Streak_Start", false, Nero::ANI_STREAK_START);

	return S_OK;
}

HRESULT Skill_Streak::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT Skill_Streak::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();

	if (0.97 <= fCurrAnimationTime)
	{
		m_pFSM->ChangeState(NeroFSM::SKILL_STREAK_LOOP);
	}
	return S_OK;
}

Skill_Streak_Ex3::Skill_Streak_Ex3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_Streak_Ex3::~Skill_Streak_Ex3()
{
}

Skill_Streak_Ex3* Skill_Streak_Ex3::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Streak_Ex3(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Streak_Ex3::StateEnter()
{
	return S_OK;
}

HRESULT Skill_Streak_Ex3::StateExit()
{
	return S_OK;
}

HRESULT Skill_Streak_Ex3::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}
#pragma endregion

#pragma region GT


GT_Equip::GT_Equip(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GT_Equip::~GT_Equip()
{
}

GT_Equip* GT_Equip::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new GT_Equip(_pFSM, _nIndex, _pNero);
}

HRESULT GT_Equip::StateEnter()
{
	return S_OK;
}

HRESULT GT_Equip::StateExit()
{
	return S_OK;
}

HRESULT GT_Equip::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

GT_Crush_Reload::GT_Crush_Reload(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GT_Crush_Reload::~GT_Crush_Reload()
{
}

GT_Crush_Reload* GT_Crush_Reload::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new GT_Crush_Reload(_pFSM, _nIndex, _pNero);
}

HRESULT GT_Crush_Reload::StateEnter()
{
	return S_OK;
}

HRESULT GT_Crush_Reload::StateExit()
{
	return S_OK;
}

HRESULT GT_Crush_Reload::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

GT_Crush_Just::GT_Crush_Just(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GT_Crush_Just::~GT_Crush_Just()
{
}

GT_Crush_Just* GT_Crush_Just::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new GT_Crush_Just(_pFSM, _nIndex, _pNero);
}

HRESULT GT_Crush_Just::StateEnter()
{
	return S_OK;
}

HRESULT GT_Crush_Just::StateExit()
{
	return S_OK;
}

HRESULT GT_Crush_Just::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

GT_Bomb::GT_Bomb(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GT_Bomb::~GT_Bomb()
{
}

GT_Bomb* GT_Bomb::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new GT_Bomb(_pFSM, _nIndex, _pNero);
}

HRESULT GT_Bomb::StateEnter()
{
	return S_OK;
}

HRESULT GT_Bomb::StateExit()
{
	return S_OK;
}

HRESULT GT_Bomb::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

GT_PickUp::GT_PickUp(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GT_PickUp::~GT_PickUp()
{
}

GT_PickUp* GT_PickUp::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new GT_PickUp(_pFSM, _nIndex, _pNero);
}

HRESULT GT_PickUp::StateEnter()
{
	return S_OK;
}

HRESULT GT_PickUp::StateExit()
{
	return S_OK;
}

HRESULT GT_PickUp::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

GT_Air_Crush_Reload::GT_Air_Crush_Reload(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GT_Air_Crush_Reload::~GT_Air_Crush_Reload()
{
}

GT_Air_Crush_Reload* GT_Air_Crush_Reload::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new GT_Air_Crush_Reload(_pFSM, _nIndex, _pNero);
}

HRESULT GT_Air_Crush_Reload::StateEnter()
{
	return S_OK;
}

HRESULT GT_Air_Crush_Reload::StateExit()
{
	return S_OK;
}

HRESULT GT_Air_Crush_Reload::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

GT_Air_Crush_Just::GT_Air_Crush_Just(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GT_Air_Crush_Just::~GT_Air_Crush_Just()
{
}

GT_Air_Crush_Just* GT_Air_Crush_Just::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new GT_Air_Crush_Just(_pFSM, _nIndex, _pNero);
}

HRESULT GT_Air_Crush_Just::StateEnter()
{
	return S_OK;
}

HRESULT GT_Air_Crush_Just::StateExit()
{
	return S_OK;
}

HRESULT GT_Air_Crush_Just::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

GT_Air_Bomb::GT_Air_Bomb(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GT_Air_Bomb::~GT_Air_Bomb()
{
}

GT_Air_Bomb* GT_Air_Bomb::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new GT_Air_Bomb(_pFSM, _nIndex, _pNero);
}

HRESULT GT_Air_Bomb::StateEnter()
{
	return S_OK;
}

HRESULT GT_Air_Bomb::StateExit()
{
	return S_OK;
}

HRESULT GT_Air_Bomb::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Overture_Shoot::Overture_Shoot(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Overture_Shoot::~Overture_Shoot()
{
}

Overture_Shoot* Overture_Shoot::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Overture_Shoot(_pFSM, _nIndex, _pNero);
}

HRESULT Overture_Shoot::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("Overture_Shoot", false, Nero::ANI_OVERTURE_SHOOT);

	return S_OK;
}

HRESULT Overture_Shoot::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT Overture_Shoot::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();

	if(0.97 <= fCurrAnimationTime)
		m_pFSM->ChangeState(NeroFSM::IDLE);

	return S_OK;
}

Overture_Shoot_Up::Overture_Shoot_Up(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Overture_Shoot_Up::~Overture_Shoot_Up()
{
}

Overture_Shoot_Up* Overture_Shoot_Up::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Overture_Shoot_Up(_pFSM, _nIndex, _pNero);
}

HRESULT Overture_Shoot_Up::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("Overture_Shoot_Up", false, Nero::ANI_OVERTURE_SHOOT_UP);

	return S_OK;
}

HRESULT Overture_Shoot_Up::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT Overture_Shoot_Up::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();

	if(0.97 <= fCurrAnimationTime)
		m_pFSM->ChangeState(NeroFSM::IDLE);

	return S_OK;
}

#pragma endregion

Skill_Streak_Loop::Skill_Streak_Loop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero), m_fLoopTime(0.f)
{

}

Skill_Streak_Loop::~Skill_Streak_Loop()
{
}

Skill_Streak_Loop* Skill_Streak_Loop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Streak_Loop(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Streak_Loop::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("Streak_Loop", false, Nero::ANI_STREAK_LOOP);
	m_fLoopTime = 1.f;
	return S_OK;
}

HRESULT Skill_Streak_Loop::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT Skill_Streak_Loop::StateUpdate(const float _fDeltaTime)
{
	m_fLoopTime -= _fDeltaTime;
	if (m_fLoopTime < 0.f)
		m_pFSM->ChangeState(NeroFSM::SKILL_STREAK_END);
	return S_OK;
}

Skill_Streak_End::Skill_Streak_End(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_Streak_End::~Skill_Streak_End()
{
}

Skill_Streak_End* Skill_Streak_End::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Streak_End(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Streak_End::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("Streak_End", false, Nero::ANI_STREAK_END);
	return S_OK;
}

HRESULT Skill_Streak_End::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT Skill_Streak_End::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();

	if (0.97 <= fCurrAnimationTime)
		m_pFSM->ChangeState(NeroFSM::IDLE);
	return S_OK;
}

Skill_Streak_Ex3_Rush::Skill_Streak_Ex3_Rush(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_Streak_Ex3_Rush::~Skill_Streak_Ex3_Rush()
{
}

Skill_Streak_Ex3_Rush* Skill_Streak_Ex3_Rush::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Streak_Ex3_Rush(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Streak_Ex3_Rush::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("Streak_Ex_Start", false, Nero::ANI_STREAK_EX_START);

	return S_OK;
}

HRESULT Skill_Streak_Ex3_Rush::StateExit()
{
	NeroState::StateExit();

	return S_OK;
}

HRESULT Skill_Streak_Ex3_Rush::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();

	if(0.97 <= fCurrAnimationTime)
		m_pFSM->ChangeState(NeroFSM::SKILL_STREAK_EX3_RUSH);

	return S_OK;
}

Skill_Streak_Ex3_Roll_Loop::Skill_Streak_Ex3_Roll_Loop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero), m_iLoopCount(0)
{
}

Skill_Streak_Ex3_Roll_Loop::~Skill_Streak_Ex3_Roll_Loop()
{
}

Skill_Streak_Ex3_Roll_Loop* Skill_Streak_Ex3_Roll_Loop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Streak_Ex3_Roll_Loop(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Streak_Ex3_Roll_Loop::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("Streak_Ex_Roll_Start", false, Nero::ANI_STREAK_END);
	m_iLoopCount = 2;

	return S_OK;
}

HRESULT Skill_Streak_Ex3_Roll_Loop::StateExit()
{
	NeroState::StateExit();

	return S_OK;
}

HRESULT Skill_Streak_Ex3_Roll_Loop::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();

	if (m_pNero.lock()->IsAnimationEnd())
		--m_iLoopCount;

	if (m_iLoopCount <= 0)
	{
		if(0.97 <= fCurrAnimationTime)
			m_pFSM->ChangeState(NeroFSM::SKILL_STREAK_EX3_ROLL_END);
	}

	return S_OK;
}

Skill_Streak_Ex3_Roll_End::Skill_Streak_Ex3_Roll_End(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_Streak_Ex3_Roll_End::~Skill_Streak_Ex3_Roll_End()
{
}

Skill_Streak_Ex3_Roll_End* Skill_Streak_Ex3_Roll_End::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_Streak_Ex3_Roll_End(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_Streak_Ex3_Roll_End::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("Streak_Ex_Roll_End", false, Nero::ANI_STREAK_END);


	return S_OK;
}

HRESULT Skill_Streak_Ex3_Roll_End::StateExit()
{
	NeroState::StateExit();

	return S_OK;
}

HRESULT Skill_Streak_Ex3_Roll_End::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();

	if(0.97 <= fCurrAnimationTime)
		m_pFSM->ChangeState(NeroFSM::IDLE);
	return S_OK;
}

Overture_Shoot_Down::Overture_Shoot_Down(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Overture_Shoot_Down::~Overture_Shoot_Down()
{
}

Overture_Shoot_Down* Overture_Shoot_Down::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Overture_Shoot_Down(_pFSM, _nIndex, _pNero);
}

HRESULT Overture_Shoot_Down::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("Overture_Shoot_Down", false, Nero::ANI_OVERTURE_SHOOT_DOWN);


	return S_OK;
}

HRESULT Overture_Shoot_Down::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT Overture_Shoot_Down::StateUpdate(const float _fDeltaTime)
{
	float fCurrAnimationTime = m_pNero.lock()->Get_PlayingTime();

	if(0.97 <= fCurrAnimationTime)
		m_pFSM->ChangeState(NeroFSM::IDLE);

	return S_OK;
}

Cbs_Idle::Cbs_Idle(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Cbs_Idle::~Cbs_Idle()
{
}

Cbs_Idle* Cbs_Idle::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Cbs_Idle(_pFSM, _nIndex, _pNero);
}

HRESULT Cbs_Idle::StateEnter()
{
	NeroState::StateEnter();
	m_pNero.lock()->ChangeAnimation("Cbs_Idle", false, Nero::ANI_CBS_IDLE);
	return S_OK;
}

HRESULT Cbs_Idle::StateExit()
{
	NeroState::StateExit();
	return S_OK;
}

HRESULT Cbs_Idle::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Cbs_ComboA1::Cbs_ComboA1(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Cbs_ComboA1::~Cbs_ComboA1()
{
}

Cbs_ComboA1* Cbs_ComboA1::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Cbs_ComboA1(_pFSM, _nIndex, _pNero);
}

HRESULT Cbs_ComboA1::StateEnter()
{
	return S_OK;
}

HRESULT Cbs_ComboA1::StateExit()
{
	return S_OK;
}

HRESULT Cbs_ComboA1::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Cbs_ComboA2::Cbs_ComboA2(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Cbs_ComboA2::~Cbs_ComboA2()
{
}

Cbs_ComboA2* Cbs_ComboA2::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Cbs_ComboA2(_pFSM, _nIndex, _pNero);
}

HRESULT Cbs_ComboA2::StateEnter()
{
	return S_OK;
}

HRESULT Cbs_ComboA2::StateExit()
{
	return S_OK;
}

HRESULT Cbs_ComboA2::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Cbs_ComboA3::Cbs_ComboA3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Cbs_ComboA3::~Cbs_ComboA3()
{
}

Cbs_ComboA3* Cbs_ComboA3::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Cbs_ComboA3(_pFSM, _nIndex, _pNero);
}

HRESULT Cbs_ComboA3::StateEnter()
{
	return S_OK;
}

HRESULT Cbs_ComboA3::StateExit()
{
	return S_OK;
}

HRESULT Cbs_ComboA3::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Cbs_ComboA4::Cbs_ComboA4(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Cbs_ComboA4::~Cbs_ComboA4()
{
}

Cbs_ComboA4* Cbs_ComboA4::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Cbs_ComboA4(_pFSM, _nIndex, _pNero);
}

HRESULT Cbs_ComboA4::StateEnter()
{
	return S_OK;
}

HRESULT Cbs_ComboA4::StateExit()
{
	return S_OK;
}

HRESULT Cbs_ComboA4::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Cbs_ComboA5::Cbs_ComboA5(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Cbs_ComboA5::~Cbs_ComboA5()
{
}

Cbs_ComboA5* Cbs_ComboA5::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Cbs_ComboA5(_pFSM, _nIndex, _pNero);
}

HRESULT Cbs_ComboA5::StateEnter()
{
	return S_OK;
}

HRESULT Cbs_ComboA5::StateExit()
{
	return S_OK;
}

HRESULT Cbs_ComboA5::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Cbs_SKill_Crystal::Cbs_SKill_Crystal(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Cbs_SKill_Crystal::~Cbs_SKill_Crystal()
{
}

Cbs_SKill_Crystal* Cbs_SKill_Crystal::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Cbs_SKill_Crystal(_pFSM, _nIndex, _pNero);
}

HRESULT Cbs_SKill_Crystal::StateEnter()
{
	return S_OK;
}

HRESULT Cbs_SKill_Crystal::StateExit()
{
	return S_OK;
}

HRESULT Cbs_SKill_Crystal::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Cbs_SKill_IceAge_Start::Cbs_SKill_IceAge_Start(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Cbs_SKill_IceAge_Start::~Cbs_SKill_IceAge_Start()
{
}

Cbs_SKill_IceAge_Start* Cbs_SKill_IceAge_Start::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Cbs_SKill_IceAge_Start(_pFSM, _nIndex, _pNero);
}

HRESULT Cbs_SKill_IceAge_Start::StateEnter()
{
	return S_OK;
}

HRESULT Cbs_SKill_IceAge_Start::StateExit()
{
	return S_OK;
}

HRESULT Cbs_SKill_IceAge_Start::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Cbs_SKill_IceAge_Loop::Cbs_SKill_IceAge_Loop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Cbs_SKill_IceAge_Loop::~Cbs_SKill_IceAge_Loop()
{
}

Cbs_SKill_IceAge_Loop* Cbs_SKill_IceAge_Loop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Cbs_SKill_IceAge_Loop(_pFSM, _nIndex, _pNero);
}

HRESULT Cbs_SKill_IceAge_Loop::StateEnter()
{
	return S_OK;
}

HRESULT Cbs_SKill_IceAge_Loop::StateExit()
{
	return S_OK;
}

HRESULT Cbs_SKill_IceAge_Loop::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Cbs_SKill_IceAge_End::Cbs_SKill_IceAge_End(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Cbs_SKill_IceAge_End::~Cbs_SKill_IceAge_End()
{
}

Cbs_SKill_IceAge_End* Cbs_SKill_IceAge_End::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Cbs_SKill_IceAge_End(_pFSM, _nIndex, _pNero);
}

HRESULT Cbs_SKill_IceAge_End::StateEnter()
{
	return S_OK;
}

HRESULT Cbs_SKill_IceAge_End::StateExit()
{
	return S_OK;
}

HRESULT Cbs_SKill_IceAge_End::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Cbs_SKill_Revolver_Start::Cbs_SKill_Revolver_Start(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Cbs_SKill_Revolver_Start::~Cbs_SKill_Revolver_Start()
{
}

Cbs_SKill_Revolver_Start* Cbs_SKill_Revolver_Start::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Cbs_SKill_Revolver_Start(_pFSM, _nIndex, _pNero);
}

HRESULT Cbs_SKill_Revolver_Start::StateEnter()
{
	return S_OK;
}

HRESULT Cbs_SKill_Revolver_Start::StateExit()
{
	return S_OK;
}

HRESULT Cbs_SKill_Revolver_Start::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Cbs_SKill_Revolver_Loop::Cbs_SKill_Revolver_Loop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Cbs_SKill_Revolver_Loop::~Cbs_SKill_Revolver_Loop()
{
}

Cbs_SKill_Revolver_Loop* Cbs_SKill_Revolver_Loop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Cbs_SKill_Revolver_Loop(_pFSM, _nIndex, _pNero);
}

HRESULT Cbs_SKill_Revolver_Loop::StateEnter()
{
	return S_OK;
}

HRESULT Cbs_SKill_Revolver_Loop::StateExit()
{
	return S_OK;
}

HRESULT Cbs_SKill_Revolver_Loop::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Cbs_SKill_Revolver_End::Cbs_SKill_Revolver_End(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Cbs_SKill_Revolver_End::~Cbs_SKill_Revolver_End()
{
}

Cbs_SKill_Revolver_End* Cbs_SKill_Revolver_End::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Cbs_SKill_Revolver_End(_pFSM, _nIndex, _pNero);
}

HRESULT Cbs_SKill_Revolver_End::StateEnter()
{
	return S_OK;
}

HRESULT Cbs_SKill_Revolver_End::StateExit()
{
	return S_OK;
}

HRESULT Cbs_SKill_Revolver_End::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Cbs_SKill_Swing::Cbs_SKill_Swing(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Cbs_SKill_Swing::~Cbs_SKill_Swing()
{
}

Cbs_SKill_Swing* Cbs_SKill_Swing::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Cbs_SKill_Swing(_pFSM, _nIndex, _pNero);
}

HRESULT Cbs_SKill_Swing::StateEnter()
{
	return S_OK;
}

HRESULT Cbs_SKill_Swing::StateExit()
{
	return S_OK;
}

HRESULT Cbs_SKill_Swing::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Middle_Cbs_BiAttack::Middle_Cbs_BiAttack(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)

{
}

Middle_Cbs_BiAttack::~Middle_Cbs_BiAttack()
{
}

Middle_Cbs_BiAttack* Middle_Cbs_BiAttack::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Middle_Cbs_BiAttack(_pFSM, _nIndex, _pNero);
}

HRESULT Middle_Cbs_BiAttack::StateEnter()
{
	return S_OK;
}

HRESULT Middle_Cbs_BiAttack::StateExit()
{
	return S_OK;
}

HRESULT Middle_Cbs_BiAttack::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Middle_Cbs_BlitzAttack::Middle_Cbs_BlitzAttack(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Middle_Cbs_BlitzAttack::~Middle_Cbs_BlitzAttack()
{
}

Middle_Cbs_BlitzAttack* Middle_Cbs_BlitzAttack::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Middle_Cbs_BlitzAttack(_pFSM, _nIndex, _pNero);
}

HRESULT Middle_Cbs_BlitzAttack::StateEnter()
{
	return S_OK;
}

HRESULT Middle_Cbs_BlitzAttack::StateExit()
{
	return S_OK;
}

HRESULT Middle_Cbs_BlitzAttack::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Middle_Cbs_Satellite::Middle_Cbs_Satellite(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Middle_Cbs_Satellite::~Middle_Cbs_Satellite()
{
}

Middle_Cbs_Satellite* Middle_Cbs_Satellite::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Middle_Cbs_Satellite(_pFSM, _nIndex, _pNero);
}

HRESULT Middle_Cbs_Satellite::StateEnter()
{
	return S_OK;
}

HRESULT Middle_Cbs_Satellite::StateExit()
{
	return S_OK;
}

HRESULT Middle_Cbs_Satellite::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Middle_Cbs_Satellite_Air::Middle_Cbs_Satellite_Air(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Middle_Cbs_Satellite_Air::~Middle_Cbs_Satellite_Air()
{
}

Middle_Cbs_Satellite_Air* Middle_Cbs_Satellite_Air::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Middle_Cbs_Satellite_Air(_pFSM, _nIndex, _pNero);
}

HRESULT Middle_Cbs_Satellite_Air::StateEnter()
{
	return S_OK;
}

HRESULT Middle_Cbs_Satellite_Air::StateExit()
{
	return S_OK;
}

HRESULT Middle_Cbs_Satellite_Air::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Middle_Cbs_Strike::Middle_Cbs_Strike(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Middle_Cbs_Strike::~Middle_Cbs_Strike()
{
}

Middle_Cbs_Strike* Middle_Cbs_Strike::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Middle_Cbs_Strike(_pFSM, _nIndex, _pNero);
}

HRESULT Middle_Cbs_Strike::StateEnter()
{
	return S_OK;
}

HRESULT Middle_Cbs_Strike::StateExit()
{
	return S_OK;
}

HRESULT Middle_Cbs_Strike::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Middle_Cbs_Strike_Air::Middle_Cbs_Strike_Air(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Middle_Cbs_Strike_Air::~Middle_Cbs_Strike_Air()
{
}

Middle_Cbs_Strike_Air* Middle_Cbs_Strike_Air::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Middle_Cbs_Strike_Air(_pFSM, _nIndex, _pNero);
}

HRESULT Middle_Cbs_Strike_Air::StateEnter()
{
	return S_OK;
}

HRESULT Middle_Cbs_Strike_Air::StateExit()
{
	return S_OK;
}

HRESULT Middle_Cbs_Strike_Air::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Middle_Cbs_Strike_Air_Down::Middle_Cbs_Strike_Air_Down(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Middle_Cbs_Strike_Air_Down::~Middle_Cbs_Strike_Air_Down()
{
}

Middle_Cbs_Strike_Air_Down* Middle_Cbs_Strike_Air_Down::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Middle_Cbs_Strike_Air_Down(_pFSM, _nIndex, _pNero);
}

HRESULT Middle_Cbs_Strike_Air_Down::StateEnter()
{
	return S_OK;
}

HRESULT Middle_Cbs_Strike_Air_Down::StateExit()
{
	return S_OK;
}

HRESULT Middle_Cbs_Strike_Air_Down::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Middle_Cbs_Strike_Air_Up::Middle_Cbs_Strike_Air_Up(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Middle_Cbs_Strike_Air_Up::~Middle_Cbs_Strike_Air_Up()
{
}

Middle_Cbs_Strike_Air_Up* Middle_Cbs_Strike_Air_Up::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Middle_Cbs_Strike_Air_Up(_pFSM, _nIndex, _pNero);
}

HRESULT Middle_Cbs_Strike_Air_Up::StateEnter()
{
	return S_OK;
}

HRESULT Middle_Cbs_Strike_Air_Up::StateExit()
{
	return S_OK;
}

HRESULT Middle_Cbs_Strike_Air_Up::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Middle_Cbs_Strike_Down::Middle_Cbs_Strike_Down(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Middle_Cbs_Strike_Down::~Middle_Cbs_Strike_Down()
{
}

Middle_Cbs_Strike_Down* Middle_Cbs_Strike_Down::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Middle_Cbs_Strike_Down(_pFSM, _nIndex, _pNero);
}

HRESULT Middle_Cbs_Strike_Down::StateEnter()
{
	return S_OK;
}

HRESULT Middle_Cbs_Strike_Down::StateExit()
{
	return S_OK;
}

HRESULT Middle_Cbs_Strike_Down::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Middle_Cbs_Strike_Up::Middle_Cbs_Strike_Up(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Middle_Cbs_Strike_Up::~Middle_Cbs_Strike_Up()
{
}

Middle_Cbs_Strike_Up* Middle_Cbs_Strike_Up::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Middle_Cbs_Strike_Up(_pFSM, _nIndex, _pNero);
}

HRESULT Middle_Cbs_Strike_Up::StateEnter()
{
	return S_OK;
}

HRESULT Middle_Cbs_Strike_Up::StateExit()
{
	return S_OK;
}

HRESULT Middle_Cbs_Strike_Up::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Middle_Cbs_ThunderBullet::Middle_Cbs_ThunderBullet(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Middle_Cbs_ThunderBullet::~Middle_Cbs_ThunderBullet()
{
}

Middle_Cbs_ThunderBullet* Middle_Cbs_ThunderBullet::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Middle_Cbs_ThunderBullet(_pFSM, _nIndex, _pNero);
}

HRESULT Middle_Cbs_ThunderBullet::StateEnter()
{
	return S_OK;
}

HRESULT Middle_Cbs_ThunderBullet::StateExit()
{
	return S_OK;
}

HRESULT Middle_Cbs_ThunderBullet::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Pole_ComboA2::Pole_ComboA2(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Pole_ComboA2::~Pole_ComboA2()
{
}

Pole_ComboA2* Pole_ComboA2::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Pole_ComboA2(_pFSM, _nIndex, _pNero);
}

HRESULT Pole_ComboA2::StateEnter()
{
	return S_OK;
}

HRESULT Pole_ComboA2::StateExit()
{
	return S_OK;
}

HRESULT Pole_ComboA2::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Pole_ComboA3::Pole_ComboA3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Pole_ComboA3::~Pole_ComboA3()
{
}

Pole_ComboA3* Pole_ComboA3::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Pole_ComboA3(_pFSM, _nIndex, _pNero);
}

HRESULT Pole_ComboA3::StateEnter()
{
	return S_OK;
}

HRESULT Pole_ComboA3::StateExit()
{
	return S_OK;
}

HRESULT Pole_ComboA3::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Pole_ComboB1::Pole_ComboB1(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Pole_ComboB1::~Pole_ComboB1()
{
}

Pole_ComboB1* Pole_ComboB1::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Pole_ComboB1(_pFSM, _nIndex, _pNero);
}

HRESULT Pole_ComboB1::StateEnter()
{
	return S_OK;
}

HRESULT Pole_ComboB1::StateExit()
{
	return S_OK;
}

HRESULT Pole_ComboB1::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Pole_ComboB2::Pole_ComboB2(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)

{
}

Pole_ComboB2::~Pole_ComboB2()
{
}

Pole_ComboB2* Pole_ComboB2::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Pole_ComboB2(_pFSM, _nIndex, _pNero);
}

HRESULT Pole_ComboB2::StateEnter()
{
	return S_OK;
}

HRESULT Pole_ComboB2::StateExit()
{
	return S_OK;
}

HRESULT Pole_ComboB2::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Pole_ComboB3::Pole_ComboB3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Pole_ComboB3::~Pole_ComboB3()
{
}

Pole_ComboB3* Pole_ComboB3::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Pole_ComboB3(_pFSM, _nIndex, _pNero);
}

HRESULT Pole_ComboB3::StateEnter()
{
	return S_OK;
}

HRESULT Pole_ComboB3::StateExit()
{
	return S_OK;
}

HRESULT Pole_ComboB3::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Pole_ComboB4::Pole_ComboB4(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Pole_ComboB4::~Pole_ComboB4()
{
}

Pole_ComboB4* Pole_ComboB4::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Pole_ComboB4(_pFSM, _nIndex, _pNero);
}

HRESULT Pole_ComboB4::StateEnter()
{
	return S_OK;
}

HRESULT Pole_ComboB4::StateExit()
{
	return S_OK;
}

HRESULT Pole_ComboB4::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Pole_WhirlWind_Start::Pole_WhirlWind_Start(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Pole_WhirlWind_Start::~Pole_WhirlWind_Start()
{
}

Pole_WhirlWind_Start* Pole_WhirlWind_Start::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Pole_WhirlWind_Start(_pFSM, _nIndex, _pNero);
}

HRESULT Pole_WhirlWind_Start::StateEnter()
{
	return S_OK;
}

HRESULT Pole_WhirlWind_Start::StateExit()
{
	return S_OK;
}

HRESULT Pole_WhirlWind_Start::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Pole_WhirlWind_Loop::Pole_WhirlWind_Loop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Pole_WhirlWind_Loop::~Pole_WhirlWind_Loop()
{
}

Pole_WhirlWind_Loop* Pole_WhirlWind_Loop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Pole_WhirlWind_Loop(_pFSM, _nIndex, _pNero);
}

HRESULT Pole_WhirlWind_Loop::StateEnter()
{
	return S_OK;
}

HRESULT Pole_WhirlWind_Loop::StateExit()
{
	return S_OK;
}

HRESULT Pole_WhirlWind_Loop::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Pole_WhirlWind_End::Pole_WhirlWind_End(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Pole_WhirlWind_End::~Pole_WhirlWind_End()
{
}

Pole_WhirlWind_End* Pole_WhirlWind_End::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Pole_WhirlWind_End(_pFSM, _nIndex, _pNero);
}

HRESULT Pole_WhirlWind_End::StateEnter()
{
	return S_OK;
}

HRESULT Pole_WhirlWind_End::StateExit()
{
	return S_OK;
}

HRESULT Pole_WhirlWind_End::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}
