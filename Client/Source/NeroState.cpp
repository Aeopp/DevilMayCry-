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
	m_pNero.lock()->ChangeAnimation(Nero::IDLE, true);
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
	if (Input::GetKeyDown(DIK_W))
	{
		m_pFSM->ChangeState(NeroFSM::RUNSTART);
	}
	else if (Input::GetKeyDown(DIK_S))
	{
		//180도 회전
		m_pFSM->ChangeState(NeroFSM::RUNSTART_180);
		//m_pNero->lock()->ChangeAnimation(Nero::ru)
	}
	else if (Input::GetKeyDown(DIK_A))
	{
		//왼쪽으로 90도 회전
		m_pFSM->ChangeState(NeroFSM::RUNSTART_L);
	}
	else if (Input::GetKeyDown(DIK_D))
	{
		//오른쪽으로 90도 회전
		m_pFSM->ChangeState(NeroFSM::RUNSTART_R);
	}
	else if (Input::GetMouseDown(DIM_L))
	{
		m_pFSM->ChangeState(NeroFSM::ATT1);
	}
	//else if (m_pNero.lock()->IsAnimationEnd())
	//{
	//	//애니메이션이 끝나면 다리바꾸는 애니메이션 한번돌리고
	//	m_pFSM->ChangeState(NeroFSM::IDLE_SWITCH_LEG);
	//}
	return S_OK;
}
///////////////////////////////////////////////////////////////////

Idle_Switch_Leg::Idle_Switch_Leg(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
	//에베베베

}

Idle_Switch_Leg::~Idle_Switch_Leg()
{
}

Idle_Switch_Leg* Idle_Switch_Leg::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Idle_Switch_Leg(_pFSM, _nIndex, _pNero);
}

HRESULT Idle_Switch_Leg::StateEnter()
{
	m_pNero.lock()->ChangeAnimation(Nero::IDLE, false);
	return S_OK;
}

HRESULT Idle_Switch_Leg::StateExit()
{
	return S_OK;
}

HRESULT Idle_Switch_Leg::StateUpdate(const float _fDeltaTime)
{
	if (Input::GetKeyDown(DIK_W))
	{
		m_pFSM->ChangeState(NeroFSM::RUNSTART);
	}
	else if (Input::GetKeyDown(DIK_S))
	{
		//180도 회전
		m_pFSM->ChangeState(NeroFSM::RUNSTART_180);
		//m_pNero->lock()->ChangeAnimation(Nero::ru)
	}
	else if (Input::GetKeyDown(DIK_A))
	{
		//왼쪽으로 90도 회전
		m_pFSM->ChangeState(NeroFSM::RUNSTART_L);
	}
	else if (Input::GetKeyDown(DIK_D))
	{
		//오른쪽으로 90도 회전
		m_pFSM->ChangeState(NeroFSM::RUNSTART_R);
	}
	else if (Input::GetMouseDown(DIM_L))
	{
		m_pFSM->ChangeState(NeroFSM::ATT1);
	}
	else if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE);
	}

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
	m_pNero.lock()->ChangeAnimation(Nero::IDLE_BATTLE, true);
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
	if (Input::GetKeyDown(DIK_W))
	{
		m_pFSM->ChangeState(NeroFSM::RUNSTART);
	}
	else if (Input::GetKeyDown(DIK_S))
	{
		//180도 회전
		m_pFSM->ChangeState(NeroFSM::RUNSTART_180);
		//m_pNero->lock()->ChangeAnimation(Nero::ru)
	}
	else if (Input::GetKeyDown(DIK_A))
	{
		//왼쪽으로 90도 회전
		m_pFSM->ChangeState(NeroFSM::RUNSTART_L);
	}
	else if (Input::GetKeyDown(DIK_D))
	{
		//오른쪽으로 90도 회전
		m_pFSM->ChangeState(NeroFSM::RUNSTART_R);
	}
	else if (Input::GetMouseDown(DIM_L))
	{
		m_pFSM->ChangeState(NeroFSM::ATT1);
	}
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

Jump_Landing_Gun::Jump_Landing_Gun(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Jump_Landing_Gun::~Jump_Landing_Gun()
{
}

Jump_Landing_Gun* Jump_Landing_Gun::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Jump_Landing_Gun(_pFSM, _nIndex, _pNero);
}

HRESULT Jump_Landing_Gun::StateEnter()
{
	return S_OK;
}

HRESULT Jump_Landing_Gun::StateExit()
{
	return S_OK;
}

HRESULT Jump_Landing_Gun::StateUpdate(const float _fDeltaTime)
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

Jump_Stinger::Jump_Stinger(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Jump_Stinger::~Jump_Stinger()
{
}

Jump_Stinger* Jump_Stinger::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Jump_Stinger(_pFSM, _nIndex, _pNero);
}

HRESULT Jump_Stinger::StateEnter()
{
	return S_OK;
}

HRESULT Jump_Stinger::StateExit()
{
	return S_OK;
}

HRESULT Jump_Stinger::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Jump_Stinger_Loop::Jump_Stinger_Loop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Jump_Stinger_Loop::~Jump_Stinger_Loop()
{
}

Jump_Stinger_Loop* Jump_Stinger_Loop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Jump_Stinger_Loop(_pFSM, _nIndex, _pNero);
}

HRESULT Jump_Stinger_Loop::StateEnter()
{
	return S_OK;
}

HRESULT Jump_Stinger_Loop::StateExit()
{
	return S_OK;
}

HRESULT Jump_Stinger_Loop::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Jump_Stinger_Landing::Jump_Stinger_Landing(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Jump_Stinger_Landing::~Jump_Stinger_Landing()
{
}

Jump_Stinger_Landing* Jump_Stinger_Landing::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Jump_Stinger_Landing(_pFSM, _nIndex, _pNero);
}

HRESULT Jump_Stinger_Landing::StateEnter()
{
	return S_OK;
}

HRESULT Jump_Stinger_Landing::StateExit()
{
	return S_OK;
}

HRESULT Jump_Stinger_Landing::StateUpdate(const float _fDeltaTime)
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
	m_pNero.lock()->ChangeAnimation(Nero::WALKSTARTLEFT, false);

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
	m_pNero.lock()->ChangeAnimation(Nero::WALKSTARTRIGHT, false);


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
	m_pNero.lock()->ChangeAnimation(Nero::WALKSTART180, false);

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
	m_pNero.lock()->ChangeAnimation(Nero::RUNLOOP, true);

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

	}
	else
	{
		m_pFSM->ChangeState(NeroFSM::RUNSTOP);
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
	m_pNero.lock()->ChangeAnimation(Nero::RUNSTARTFRONT, false);

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
	if (Input::GetKey(DIK_W) && m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::RUNLOOP);
	}
	else if(m_pNero.lock()->IsAnimationEnd())
		m_pFSM->ChangeState(NeroFSM::IDLE);

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
	m_pNero.lock()->ChangeAnimation(Nero::RUNSTOP, false);

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



GunWalkFront::GunWalkFront(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GunWalkFront::~GunWalkFront()
{
}

GunWalkFront* GunWalkFront::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new GunWalkFront(_pFSM, _nIndex, _pNero);
}

HRESULT GunWalkFront::StateEnter()
{
	return S_OK;
}

HRESULT GunWalkFront::StateExit()
{
	return S_OK;
}

HRESULT GunWalkFront::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

GunWalkLeft::GunWalkLeft(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GunWalkLeft::~GunWalkLeft()
{
}

GunWalkLeft* GunWalkLeft::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new GunWalkLeft(_pFSM, _nIndex, _pNero);
}

HRESULT GunWalkLeft::StateEnter()
{
	return S_OK;
}

HRESULT GunWalkLeft::StateExit()
{
	return S_OK;
}

HRESULT GunWalkLeft::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

GunWalkBack::GunWalkBack(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GunWalkBack::~GunWalkBack()
{
}

GunWalkBack* GunWalkBack::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new GunWalkBack(_pFSM, _nIndex, _pNero);
}

HRESULT GunWalkBack::StateEnter()
{
	return S_OK;
}

HRESULT GunWalkBack::StateExit()
{
	return S_OK;
}

HRESULT GunWalkBack::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

GunWalkRight::GunWalkRight(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GunWalkRight::~GunWalkRight()
{
}

GunWalkRight* GunWalkRight::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new GunWalkRight(_pFSM, _nIndex, _pNero);
}

HRESULT GunWalkRight::StateEnter()
{
	return S_OK;
}

HRESULT GunWalkRight::StateExit()
{
	return S_OK;
}

HRESULT GunWalkRight::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

GunWalkRightBack::GunWalkRightBack(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GunWalkRightBack::~GunWalkRightBack()
{
}

GunWalkRightBack* GunWalkRightBack::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new GunWalkRightBack(_pFSM, _nIndex, _pNero);
}

HRESULT GunWalkRightBack::StateEnter()
{
	return S_OK;
}

HRESULT GunWalkRightBack::StateExit()
{
	return S_OK;
}

HRESULT GunWalkRightBack::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

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

WalkStartRight::WalkStartRight(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

WalkStartRight::~WalkStartRight()
{
}

WalkStartRight* WalkStartRight::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new WalkStartRight(_pFSM, _nIndex, _pNero);
}

HRESULT WalkStartRight::StateEnter()
{
	return S_OK;
}

HRESULT WalkStartRight::StateExit()
{
	return S_OK;
}

HRESULT WalkStartRight::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

WalkStart180::WalkStart180(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

WalkStart180::~WalkStart180()
{
}

WalkStart180* WalkStart180::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return  new WalkStart180(_pFSM, _nIndex, _pNero);
}

HRESULT WalkStart180::StateEnter()
{
	return S_OK;
}

HRESULT WalkStart180::StateExit()
{
	return S_OK;
}

HRESULT WalkStart180::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

WalkStartLeft::WalkStartLeft(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

WalkStartLeft::~WalkStartLeft()
{
}

WalkStartLeft* WalkStartLeft::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new WalkStartLeft(_pFSM, _nIndex, _pNero);
}

HRESULT WalkStartLeft::StateEnter()
{
	return S_OK;
}

HRESULT WalkStartLeft::StateExit()
{
	return S_OK;
}

HRESULT WalkStartLeft::StateUpdate(const float _fDeltaTime)
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

WalkTurn180::WalkTurn180(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

WalkTurn180::~WalkTurn180()
{
}

WalkTurn180* WalkTurn180::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new WalkTurn180(_pFSM, _nIndex, _pNero);
}

HRESULT WalkTurn180::StateEnter()
{
	return S_OK;
}

HRESULT WalkTurn180::StateExit()
{
	return S_OK;
}

HRESULT WalkTurn180::StateUpdate(const float _fDeltaTime)
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

#pragma region SHOOT




Idle_To_Shoot::Idle_To_Shoot(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Idle_To_Shoot::~Idle_To_Shoot()
{
}

Idle_To_Shoot* Idle_To_Shoot::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Idle_To_Shoot(_pFSM, _nIndex, _pNero);
}

HRESULT Idle_To_Shoot::StateEnter()
{
	return S_OK;
}

HRESULT Idle_To_Shoot::StateExit()
{
	return S_OK;
}

HRESULT Idle_To_Shoot::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Idle_To_Shoot_Left::Idle_To_Shoot_Left(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Idle_To_Shoot_Left::~Idle_To_Shoot_Left()
{
}

Idle_To_Shoot_Left* Idle_To_Shoot_Left::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Idle_To_Shoot_Left(_pFSM, _nIndex, _pNero);
}

HRESULT Idle_To_Shoot_Left::StateEnter()
{
	return S_OK;
}

HRESULT Idle_To_Shoot_Left::StateExit()
{
	return S_OK;
}

HRESULT Idle_To_Shoot_Left::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Idle_To_Shoot_Right::Idle_To_Shoot_Right(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Idle_To_Shoot_Right::~Idle_To_Shoot_Right()
{
}

Idle_To_Shoot_Right* Idle_To_Shoot_Right::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Idle_To_Shoot_Right(_pFSM, _nIndex, _pNero);
}

HRESULT Idle_To_Shoot_Right::StateEnter()
{
	return S_OK;
}

HRESULT Idle_To_Shoot_Right::StateExit()
{
	return S_OK;
}

HRESULT Idle_To_Shoot_Right::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Shoot_To_Idle::Shoot_To_Idle(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Shoot_To_Idle::~Shoot_To_Idle()
{
}

Shoot_To_Idle* Shoot_To_Idle::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Shoot_To_Idle(_pFSM, _nIndex, _pNero);
}

HRESULT Shoot_To_Idle::StateEnter()
{
	return S_OK;
}

HRESULT Shoot_To_Idle::StateExit()
{
	return S_OK;
}

HRESULT Shoot_To_Idle::StateUpdate(const float _fDeltaTime)
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

Wire_Pull_Air_Hard::Wire_Pull_Air_Hard(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wire_Pull_Air_Hard::~Wire_Pull_Air_Hard()
{
}

Wire_Pull_Air_Hard* Wire_Pull_Air_Hard::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wire_Pull_Air_Hard(_pFSM, _nIndex, _pNero);
}

HRESULT Wire_Pull_Air_Hard::StateEnter()
{
	return S_OK;
}

HRESULT Wire_Pull_Air_Hard::StateExit()
{
	return S_OK;
}

HRESULT Wire_Pull_Air_Hard::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Wire_Pull_Air_Hard_Up::Wire_Pull_Air_Hard_Up(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wire_Pull_Air_Hard_Up::~Wire_Pull_Air_Hard_Up()
{
}

Wire_Pull_Air_Hard_Up* Wire_Pull_Air_Hard_Up::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wire_Pull_Air_Hard_Up(_pFSM, _nIndex, _pNero);
}

HRESULT Wire_Pull_Air_Hard_Up::StateEnter()
{
	return S_OK;
}

HRESULT Wire_Pull_Air_Hard_Up::StateExit()
{
	return S_OK;
}

HRESULT Wire_Pull_Air_Hard_Up::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Wire_Pull_Air_Hard_Down::Wire_Pull_Air_Hard_Down(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wire_Pull_Air_Hard_Down::~Wire_Pull_Air_Hard_Down()
{
}

Wire_Pull_Air_Hard_Down* Wire_Pull_Air_Hard_Down::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Wire_Pull_Air_Hard_Down(_pFSM, _nIndex, _pNero);
}

HRESULT Wire_Pull_Air_Hard_Down::StateEnter()
{
	return S_OK;
}

HRESULT Wire_Pull_Air_Hard_Down::StateExit()
{
	return S_OK;
}

HRESULT Wire_Pull_Air_Hard_Down::StateUpdate(const float _fDeltaTime)
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
	m_pNero.lock()->ChangeAnimation(Nero::BT_ATT1, false);

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
		else if (0.4f < fCurrAnimationTime && fCurrAnimationTime <= 0.8f)
		{
			// 레드퀸 콤보 땅바닥 찍는거 시작 
			m_pFSM->ChangeState(NeroFSM::ATT_COMBO_C1);
		}
	}
	else if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE_BATTLE);
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
	m_pNero.lock()->ChangeAnimation(Nero::BT_ATT2, false);

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
		else if (0.4f < fCurrAnimationTime && fCurrAnimationTime <= 0.8f)
		{
			// 레드퀸 콤보 앞으로 가면서 베는거
			m_pFSM->ChangeState(NeroFSM::ATT_COMBO_D1);
		}
	}
	else if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE_BATTLE);
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
	m_pNero.lock()->ChangeAnimation(Nero::BT_ATT3, false);
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
	else if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE_BATTLE);
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
	m_pNero.lock()->ChangeAnimation(Nero::BT_ATT4, false);

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
	else if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE_BATTLE);
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
	m_pNero.lock()->ChangeAnimation(Nero::BT_ATT_COMBOC_R_TO_L, false);

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
		m_pFSM->ChangeState(NeroFSM::IDLE_BATTLE);
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
	m_pNero.lock()->ChangeAnimation(Nero::BT_ATT_COMBOC_L_TO_R, false);

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
		m_pFSM->ChangeState(NeroFSM::IDLE_BATTLE);
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
	m_pNero.lock()->ChangeAnimation(Nero::BT_ATT_COMBOC_1, false);
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
		m_pFSM->ChangeState(NeroFSM::IDLE_BATTLE);
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
	m_pNero.lock()->ChangeAnimation(Nero::BT_ATT_COMBOC_2, false);

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
		m_pFSM->ChangeState(NeroFSM::IDLE_BATTLE);
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
	m_pNero.lock()->ChangeAnimation(Nero::BT_ATT_COMBOC_3, false);

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
		m_pFSM->ChangeState(NeroFSM::IDLE_BATTLE);
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
	m_pNero.lock()->ChangeAnimation(Nero::BT_ATT_COMBOC_4, false);
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
		m_pFSM->ChangeState(NeroFSM::IDLE_BATTLE);
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
	m_pNero.lock()->ChangeAnimation(Nero::BT_ATT_COMBOD_1, false);
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
		m_pFSM->ChangeState(NeroFSM::IDLE_BATTLE);
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
	m_pNero.lock()->ChangeAnimation(Nero::BT_ATT_COMBOD_2, false);
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
		m_pFSM->ChangeState(NeroFSM::IDLE_BATTLE);
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
	m_pNero.lock()->ChangeAnimation(Nero::BT_ATT_COMBOD_3, false);

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
		if (0.22f <= fCurrAnimationTime && fCurrAnimationTime <= 0.33f)
		{
			m_pFSM->ChangeState(NeroFSM::ATT_COMBO_D4);
		}
	}
	else if (m_pNero.lock()->IsAnimationEnd())
	{
		m_pFSM->ChangeState(NeroFSM::IDLE_BATTLE);
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
	m_pNero.lock()->ChangeAnimation(Nero::BT_ATT_COMBOD_4, false);

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
		m_pFSM->ChangeState(NeroFSM::IDLE_BATTLE);
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




Skill_E_Start::Skill_E_Start(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_E_Start::~Skill_E_Start()
{
}

Skill_E_Start* Skill_E_Start::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_E_Start(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_E_Start::StateEnter()
{
	return S_OK;
}

HRESULT Skill_E_Start::StateExit()
{
	return S_OK;
}

HRESULT Skill_E_Start::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

Skill_E_Loop::Skill_E_Loop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_E_Loop::~Skill_E_Loop()
{
}

Skill_E_Loop* Skill_E_Loop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new Skill_E_Loop(_pFSM, _nIndex, _pNero);
}

HRESULT Skill_E_Loop::StateEnter()
{
	return S_OK;
}

HRESULT Skill_E_Loop::StateExit()
{
	return S_OK;
}

HRESULT Skill_E_Loop::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

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
	return S_OK;
}

HRESULT Skill_Float_Ground::StateExit()
{
	return S_OK;
}

HRESULT Skill_Float_Ground::StateUpdate(const float _fDeltaTime)
{
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
	return S_OK;
}

HRESULT Skill_Shuffle::StateExit()
{
	return S_OK;
}

HRESULT Skill_Shuffle::StateUpdate(const float _fDeltaTime)
{
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
	return S_OK;
}

HRESULT Skill_Streak::StateExit()
{
	return S_OK;
}

HRESULT Skill_Streak::StateUpdate(const float _fDeltaTime)
{
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

GT_Spark_Basic::GT_Spark_Basic(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GT_Spark_Basic::~GT_Spark_Basic()
{
}

GT_Spark_Basic* GT_Spark_Basic::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new GT_Spark_Basic(_pFSM, _nIndex, _pNero);
}

HRESULT GT_Spark_Basic::StateEnter()
{
	return S_OK;
}

HRESULT GT_Spark_Basic::StateExit()
{
	return S_OK;
}

HRESULT GT_Spark_Basic::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

GT_Spark_Basic_Up::GT_Spark_Basic_Up(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GT_Spark_Basic_Up::~GT_Spark_Basic_Up()
{
}

GT_Spark_Basic_Up* GT_Spark_Basic_Up::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return new GT_Spark_Basic_Up(_pFSM, _nIndex, _pNero);
}

HRESULT GT_Spark_Basic_Up::StateEnter()
{
	return S_OK;
}

HRESULT GT_Spark_Basic_Up::StateExit()
{
	return S_OK;
}

HRESULT GT_Spark_Basic_Up::StateUpdate(const float _fDeltaTime)
{
	return S_OK;
}

#pragma endregion


