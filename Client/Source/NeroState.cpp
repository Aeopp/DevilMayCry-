#include "stdafx.h"
#include "NeroState.h"
#include "Nero.h"
#include "NeroFSM.h"

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


Idle::Idle(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM,_nIndex,_pNero)
{
}

Idle::~Idle()
{
}

Idle* Idle::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return nullptr;
}

HRESULT Idle::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Idle::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Idle::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
}
///////////////////////////////////////////////////////////////////

Idle_Switch_Leg::Idle_Switch_Leg(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Idle_Switch_Leg::~Idle_Switch_Leg()
{
}

Idle_Switch_Leg* Idle_Switch_Leg::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return nullptr;
}

HRESULT Idle_Switch_Leg::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Idle_Switch_Leg::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Idle_Switch_Leg::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Idle_Battle::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Idle_Battle::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Idle_Battle::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
}

Jump_Basic::Jump_Basic(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Jump_Basic::~Jump_Basic()
{
}

Jump_Basic* Jump_Basic::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return nullptr;
}

HRESULT Jump_Basic::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Jump_Basic::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Jump_Basic::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Jump_Fly_Loop::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Jump_Fly_Loop::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Jump_Fly_Loop::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Jump_Landing::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Jump_Landing::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Jump_Landing::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Jump_Landing_Gun::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Jump_Landing_Gun::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Jump_Landing_Gun::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Jump_Landing_High::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Jump_Landing_High::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Jump_Landing_High::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Jump_Twice::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Jump_Twice::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Jump_Twice::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Jump_Front::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Jump_Front::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Jump_Front::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Jump_Front_Landing::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Jump_Front_Landing::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Jump_Front_Landing::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Jump_Front_Twice::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Jump_Front_Twice::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Jump_Front_Twice::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Jump_Back::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Jump_Back::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Jump_Back::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Jump_Back_Twice::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Jump_Back_Twice::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Jump_Back_Twice::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Jump_Stinger::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Jump_Stinger::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Jump_Stinger::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Jump_Stinger_Loop::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Jump_Stinger_Loop::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Jump_Stinger_Loop::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Jump_Stinger_Landing::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Jump_Stinger_Landing::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Jump_Stinger_Landing::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT RunLoop::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT RunLoop::StateExit()
{
	return E_NOTIMPL;
}

HRESULT RunLoop::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT RunStartFront::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT RunStartFront::StateExit()
{
	return E_NOTIMPL;
}

HRESULT RunStartFront::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT RunStop::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT RunStop::StateExit()
{
	return E_NOTIMPL;
}

HRESULT RunStop::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT RunTurn::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT RunTurn::StateExit()
{
	return E_NOTIMPL;
}

HRESULT RunTurn::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT RunTurn_L::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT RunTurn_L::StateExit()
{
	return E_NOTIMPL;
}

HRESULT RunTurn_L::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
}

DashLoop::DashLoop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

DashLoop::~DashLoop()
{
}

DashLoop* DashLoop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return nullptr;
}

HRESULT DashLoop::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT DashLoop::StateExit()
{
	return E_NOTIMPL;
}

HRESULT DashLoop::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT DashStop::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT DashStop::StateExit()
{
	return E_NOTIMPL;
}

HRESULT DashStop::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT DashTurn::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT DashTurn::StateExit()
{
	return E_NOTIMPL;
}

HRESULT DashTurn::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
}

GunWalkFront::GunWalkFront(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GunWalkFront::~GunWalkFront()
{
}

GunWalkFront* GunWalkFront::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return nullptr;
}

HRESULT GunWalkFront::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT GunWalkFront::StateExit()
{
	return E_NOTIMPL;
}

HRESULT GunWalkFront::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT GunWalkLeft::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT GunWalkLeft::StateExit()
{
	return E_NOTIMPL;
}

HRESULT GunWalkLeft::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT GunWalkBack::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT GunWalkBack::StateExit()
{
	return E_NOTIMPL;
}

HRESULT GunWalkBack::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT GunWalkRight::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT GunWalkRight::StateExit()
{
	return E_NOTIMPL;
}

HRESULT GunWalkRight::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT GunWalkRightBack::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT GunWalkRightBack::StateExit()
{
	return E_NOTIMPL;
}

HRESULT GunWalkRightBack::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
}

WalkLoop::WalkLoop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

WalkLoop::~WalkLoop()
{
}

WalkLoop* WalkLoop::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return nullptr;
}

HRESULT WalkLoop::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT WalkLoop::StateExit()
{
	return E_NOTIMPL;
}

HRESULT WalkLoop::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT WalkStart::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT WalkStart::StateExit()
{
	return E_NOTIMPL;
}

HRESULT WalkStart::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT WalkStartRight::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT WalkStartRight::StateExit()
{
	return E_NOTIMPL;
}

HRESULT WalkStartRight::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT WalkStart180::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT WalkStart180::StateExit()
{
	return E_NOTIMPL;
}

HRESULT WalkStart180::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT WalkStartLeft::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT WalkStartLeft::StateExit()
{
	return E_NOTIMPL;
}

HRESULT WalkStartLeft::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT WalkStop::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT WalkStop::StateExit()
{
	return E_NOTIMPL;
}

HRESULT WalkStop::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT WalkTurn180::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT WalkTurn180::StateExit()
{
	return E_NOTIMPL;
}

HRESULT WalkTurn180::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
}

Evade_L::Evade_L(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Evade_L::~Evade_L()
{
}

Evade_L* Evade_L::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return nullptr;
}

HRESULT Evade_L::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Evade_L::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Evade_L::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Evade_R::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Evade_R::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Evade_R::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
}

Idle_To_Shoot::Idle_To_Shoot(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Idle_To_Shoot::~Idle_To_Shoot()
{
}

Idle_To_Shoot* Idle_To_Shoot::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return nullptr;
}

HRESULT Idle_To_Shoot::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Idle_To_Shoot::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Idle_To_Shoot::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Idle_To_Shoot_Left::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Idle_To_Shoot_Left::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Idle_To_Shoot_Left::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Idle_To_Shoot_Right::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Idle_To_Shoot_Right::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Idle_To_Shoot_Right::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Shoot_To_Idle::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Shoot_To_Idle::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Shoot_To_Idle::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
}

HitFront::HitFront(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

HitFront::~HitFront()
{
}

HitFront* HitFront::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return nullptr;
}

HRESULT HitFront::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT HitFront::StateExit()
{
	return E_NOTIMPL;
}

HRESULT HitFront::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT HitFront_Big::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT HitFront_Big::StateExit()
{
	return E_NOTIMPL;
}

HRESULT HitFront_Big::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Hit_Air::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Hit_Air::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Hit_Air::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Wind_Pressure_Small::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wind_Pressure_Small::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wind_Pressure_Small::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Wind_Pressure_Big::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wind_Pressure_Big::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wind_Pressure_Big::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Wind_Pressure_Small_End::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wind_Pressure_Small_End::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wind_Pressure_Small_End::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Wind_Pressure_Big_End::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wind_Pressure_Big_End::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wind_Pressure_Big_End::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT KnockBack_Front::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT KnockBack_Front::StateExit()
{
	return E_NOTIMPL;
}

HRESULT KnockBack_Front::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT KnockBack_Get_Up::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT KnockBack_Get_Up::StateExit()
{
	return E_NOTIMPL;
}

HRESULT KnockBack_Get_Up::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
}

Die::Die(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Die::~Die()
{
}

Die* Die::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return nullptr;
}

HRESULT Die::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Die::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Die::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Resurrection::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Resurrection::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Resurrection::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Resurrection_GetUp::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Resurrection_GetUp::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Resurrection_GetUp::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
}

StunStart::StunStart(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

StunStart::~StunStart()
{
}

StunStart* StunStart::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return nullptr;
}

HRESULT StunStart::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT StunStart::StateExit()
{
	return E_NOTIMPL;
}

HRESULT StunStart::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT StunLoop::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT StunLoop::StateExit()
{
	return E_NOTIMPL;
}

HRESULT StunLoop::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT StunEnd::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT StunEnd::StateExit()
{
	return E_NOTIMPL;
}

HRESULT StunEnd::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT EarthQuakeStart::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT EarthQuakeStart::StateExit()
{
	return E_NOTIMPL;
}

HRESULT EarthQuakeStart::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT EarthQuakeLoop::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT EarthQuakeLoop::StateExit()
{
	return E_NOTIMPL;
}

HRESULT EarthQuakeLoop::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT EarthQuakeEnd::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT EarthQuakeEnd::StateExit()
{
	return E_NOTIMPL;
}

HRESULT EarthQuakeEnd::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
}

Wire_Pull::Wire_Pull(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Wire_Pull::~Wire_Pull()
{
}

Wire_Pull* Wire_Pull::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return nullptr;
}

HRESULT Wire_Pull::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Wire_Pull_Up::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Up::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Up::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Wire_Pull_Down::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Down::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Down::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Wire_Fly_Start::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wire_Fly_Start::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wire_Fly_Start::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Wire_Fly_Loop::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wire_Fly_Loop::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wire_Fly_Loop::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Wire_Fly_End::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wire_Fly_End::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wire_Fly_End::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Wire_Pull_Air::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Air::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Air::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Wire_Pull_Air_Up::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Air_Up::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Air_Up::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Wire_Pull_Air_Down::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Air_Down::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Air_Down::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Wire_Pull_Air_Hard::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Air_Hard::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Air_Hard::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Wire_Pull_Air_Hard_Up::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Air_Hard_Up::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Air_Hard_Up::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Wire_Pull_Air_Hard_Down::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Air_Hard_Down::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Wire_Pull_Air_Hard_Down::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
}

BT_Att1::BT_Att1(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

BT_Att1::~BT_Att1()
{
}

BT_Att1* BT_Att1::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return nullptr;
}

HRESULT BT_Att1::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att1::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att1::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att2::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att2::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att2::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att3::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att3::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att3::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att4::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att4::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att4::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att_ComboB::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboB::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboB::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att_ComboC_R_to_L::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboC_R_to_L::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboC_R_to_L::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att_ComboC_L_to_R::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboC_L_to_R::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboC_L_to_R::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att_ComboC_1::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboC_1::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboC_1::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att_ComboC_2::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboC_2::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboC_2::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att_ComboC_3::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboC_3::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboC_3::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att_ComboC_4::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboC_4::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboC_4::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att_ComboD_1::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboD_1::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboD_1::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att_ComboD_2::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboD_2::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboD_2::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att_ComboD_3::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboD_3::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboD_3::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att_ComboD_4::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboD_4::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att_ComboD_4::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Air_Att1::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Air_Att1::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Air_Att1::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Air_Att2::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Air_Att2::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Air_Att2::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Air_Att3::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Air_Att3::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Air_Att3::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Air_ComboB::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Air_ComboB::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Air_ComboB::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att1_To_RunStart::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att1_To_RunStart::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att1_To_RunStart::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att1_To_RunLoop::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att1_To_RunLoop::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att1_To_RunLoop::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att1_To_Idle::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att1_To_Idle::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att1_To_Idle::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att1_To_Idle_End::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att1_To_Idle_End::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att1_To_Idle_End::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att2_To_RunStart::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att2_To_RunStart::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att2_To_RunStart::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att2_To_Idle::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att2_To_Idle::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att2_To_Idle::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Att1_Dash::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Att1_Dash::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Att1_Dash::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT BT_Parrying::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT BT_Parrying::StateExit()
{
	return E_NOTIMPL;
}

HRESULT BT_Parrying::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
}

Skill_E_Start::Skill_E_Start(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

Skill_E_Start::~Skill_E_Start()
{
}

Skill_E_Start* Skill_E_Start::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return nullptr;
}

HRESULT Skill_E_Start::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Skill_E_Start::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Skill_E_Start::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Skill_E_Loop::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Skill_E_Loop::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Skill_E_Loop::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Skill_Split::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Skill_Split::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Skill_Split::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Skill_Split_Loop::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Skill_Split_Loop::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Skill_Split_Loop::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Skill_Split_Landing::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Skill_Split_Landing::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Skill_Split_Landing::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Skill_Float_Ground::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Skill_Float_Ground::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Skill_Float_Ground::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Skill_Float_Ground_Ex3::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Skill_Float_Ground_Ex3::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Skill_Float_Ground_Ex3::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Skill_Float_Ground_Finish::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Skill_Float_Ground_Finish::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Skill_Float_Ground_Finish::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Skill_Float_Air::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Skill_Float_Air::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Skill_Float_Air::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Skill_Float_Air_Ex3::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Skill_Float_Air_Ex3::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Skill_Float_Air_Ex3::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Skill_Shuffle::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Skill_Shuffle::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Skill_Shuffle::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Skill_Caliber::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Skill_Caliber::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Skill_Caliber::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Skill_Streak::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Skill_Streak::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Skill_Streak::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT Skill_Streak_Ex3::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT Skill_Streak_Ex3::StateExit()
{
	return E_NOTIMPL;
}

HRESULT Skill_Streak_Ex3::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
}

GT_Equip::GT_Equip(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
	:NeroState(_pFSM, _nIndex, _pNero)
{
}

GT_Equip::~GT_Equip()
{
}

GT_Equip* GT_Equip::Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero)
{
	return nullptr;
}

HRESULT GT_Equip::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT GT_Equip::StateExit()
{
	return E_NOTIMPL;
}

HRESULT GT_Equip::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT GT_Crush_Reload::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT GT_Crush_Reload::StateExit()
{
	return E_NOTIMPL;
}

HRESULT GT_Crush_Reload::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT GT_Crush_Just::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT GT_Crush_Just::StateExit()
{
	return E_NOTIMPL;
}

HRESULT GT_Crush_Just::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT GT_Bomb::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT GT_Bomb::StateExit()
{
	return E_NOTIMPL;
}

HRESULT GT_Bomb::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT GT_PickUp::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT GT_PickUp::StateExit()
{
	return E_NOTIMPL;
}

HRESULT GT_PickUp::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT GT_Air_Crush_Reload::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT GT_Air_Crush_Reload::StateExit()
{
	return E_NOTIMPL;
}

HRESULT GT_Air_Crush_Reload::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT GT_Air_Crush_Just::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT GT_Air_Crush_Just::StateExit()
{
	return E_NOTIMPL;
}

HRESULT GT_Air_Crush_Just::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT GT_Air_Bomb::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT GT_Air_Bomb::StateExit()
{
	return E_NOTIMPL;
}

HRESULT GT_Air_Bomb::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT GT_Spark_Basic::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT GT_Spark_Basic::StateExit()
{
	return E_NOTIMPL;
}

HRESULT GT_Spark_Basic::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
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
	return nullptr;
}

HRESULT GT_Spark_Basic_Up::StateEnter()
{
	return E_NOTIMPL;
}

HRESULT GT_Spark_Basic_Up::StateExit()
{
	return E_NOTIMPL;
}

HRESULT GT_Spark_Basic_Up::StateUpdate(const float _fDeltaTime)
{
	return E_NOTIMPL;
}
