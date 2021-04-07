#pragma once
#ifndef NeroState_h__
#define NeroState_h__

#include "FSMState.h"
class Nero;

class NeroState :    public FSMState
{
protected:
	weak_ptr<Nero>	m_pNero;
	//weak_ptr<Animator>	m_pAnimator;

public:
	explicit NeroState(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
	virtual ~NeroState();

public:
	// FSMState을(를) 통해 상속됨
	virtual HRESULT StateEnter()							PURE;
	virtual HRESULT StateExit()								PURE;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	PURE;
};

class Idle : public NeroState
{
private:
	explicit Idle(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Idle();

public:
	static Idle* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Idle_Switch_Leg : public NeroState
{
private:
	explicit Idle_Switch_Leg(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Idle_Switch_Leg();

public:
	static Idle_Switch_Leg* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Idle_Battle : public NeroState
{
private:
	explicit Idle_Battle(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Idle_Battle();

public:
	static Idle_Battle* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Jump_Basic : public NeroState
{
private:
	explicit Jump_Basic(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Jump_Basic();

public:
	static Jump_Basic* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Jump_Fly_Loop : public NeroState
{
private:
	explicit Jump_Fly_Loop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Jump_Fly_Loop();

public:
	static Jump_Fly_Loop* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Jump_Landing : public NeroState
{
private:
	explicit Jump_Landing(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Jump_Landing();

public:
	static Jump_Landing* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};


class Jump_Landing_Gun : public NeroState
{
private:
	explicit Jump_Landing_Gun(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Jump_Landing_Gun();

public:
	static Jump_Landing_Gun* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Jump_Landing_High : public NeroState
{
private:
	explicit Jump_Landing_High(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Jump_Landing_High();

public:
	static Jump_Landing_High* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Jump_Twice : public NeroState
{
private:
	explicit Jump_Twice(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Jump_Twice();

public:
	static Jump_Twice* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Jump_Front : public NeroState
{
private:
	explicit Jump_Front(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Jump_Front();

public:
	static Jump_Front* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Jump_Front_Landing : public NeroState
{
private:
	explicit Jump_Front_Landing(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Jump_Front_Landing();

public:
	static Jump_Front_Landing* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Jump_Front_Twice : public NeroState
{
private:
	explicit Jump_Front_Twice(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Jump_Front_Twice();

public:
	static Jump_Front_Twice* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Jump_Back : public NeroState
{
private:
	explicit Jump_Back(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Jump_Back();

public:
	static Jump_Back* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Jump_Back_Twice : public NeroState
{
private:
	explicit Jump_Back_Twice(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Jump_Back_Twice();

public:
	static Jump_Back_Twice* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Jump_Stinger : public NeroState
{
private:
	explicit Jump_Stinger(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Jump_Stinger();

public:
	static Jump_Stinger* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Jump_Stinger_Loop : public NeroState
{
private:
	explicit Jump_Stinger_Loop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Jump_Stinger_Loop();

public:
	static Jump_Stinger_Loop* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Jump_Stinger_Landing : public NeroState
{
private:
	explicit Jump_Stinger_Landing(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Jump_Stinger_Landing();

public:
	static Jump_Stinger_Landing* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class RunLoop : public NeroState
{
private:
	explicit RunLoop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~RunLoop();

public:
	static RunLoop* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class RunStartFront : public NeroState
{
private:
	explicit RunStartFront(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~RunStartFront();

public:
	static RunStartFront* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class RunStop : public NeroState
{
private:
	explicit RunStop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~RunStop();

public:
	static RunStop* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class RunTurn : public NeroState
{
private:
	explicit RunTurn(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~RunTurn();

public:
	static RunTurn* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class RunTurn_L : public NeroState
{
private:
	explicit RunTurn_L(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~RunTurn_L();

public:
	static RunTurn_L* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class DashLoop : public NeroState
{
private:
	explicit DashLoop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~DashLoop();

public:
	static DashLoop* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class DashStop : public NeroState
{
private:
	explicit DashStop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~DashStop();

public:
	static DashStop* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class DashTurn : public NeroState
{
private:
	explicit DashTurn(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~DashTurn();

public:
	static DashTurn* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class GunWalkFront : public NeroState
{
private:
	explicit GunWalkFront(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~GunWalkFront();

public:
	static GunWalkFront* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class GunWalkLeft : public NeroState
{
private:
	explicit GunWalkLeft(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~GunWalkLeft();

public:
	static GunWalkLeft* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class GunWalkBack : public NeroState
{
private:
	explicit GunWalkBack(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~GunWalkBack();

public:
	static GunWalkBack* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class GunWalkRight : public NeroState
{
private:
	explicit GunWalkRight(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~GunWalkRight();

public:
	static GunWalkRight* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class GunWalkRightBack : public NeroState
{
private:
	explicit GunWalkRightBack(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~GunWalkRightBack();

public:
	static GunWalkRightBack* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class WalkLoop : public NeroState
{
private:
	explicit WalkLoop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~WalkLoop();

public:
	static WalkLoop* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class WalkStart : public NeroState
{
private:
	explicit WalkStart(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~WalkStart();

public:
	static WalkStart* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class WalkStartRight : public NeroState
{
private:
	explicit WalkStartRight(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~WalkStartRight();

public:
	static WalkStartRight* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class WalkStart180 : public NeroState
{
private:
	explicit WalkStart180(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~WalkStart180();

public:
	static WalkStart180* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class WalkStartLeft : public NeroState
{
private:
	explicit WalkStartLeft(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~WalkStartLeft();

public:
	static WalkStartLeft* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};


class WalkStop : public NeroState
{
private:
	explicit WalkStop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~WalkStop();

public:
	static WalkStop* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};


class WalkTurn180 : public NeroState
{
private:
	explicit WalkTurn180(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~WalkTurn180();

public:
	static WalkTurn180* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Evade_L : public NeroState
{
private:
	explicit Evade_L(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Evade_L();

public:
	static Evade_L* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Evade_R : public NeroState
{
private:
	explicit Evade_R(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Evade_R();

public:
	static Evade_R* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Idle_To_Shoot : public NeroState
{
private:
	explicit Idle_To_Shoot(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Idle_To_Shoot();

public:
	static Idle_To_Shoot* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Idle_To_Shoot_Left : public NeroState
{
private:
	explicit Idle_To_Shoot_Left(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Idle_To_Shoot_Left();

public:
	static Idle_To_Shoot_Left* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Idle_To_Shoot_Right : public NeroState
{
private:
	explicit Idle_To_Shoot_Right(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Idle_To_Shoot_Right();

public:
	static Idle_To_Shoot_Right* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Shoot_To_Idle : public NeroState
{
private:
	explicit Shoot_To_Idle(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Shoot_To_Idle();

public:
	static Shoot_To_Idle* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class HitFront : public NeroState
{
private:
	explicit HitFront(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~HitFront();

public:
	static HitFront* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class HitFront_Big : public NeroState
{
private:
	explicit HitFront_Big(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~HitFront_Big();

public:
	static HitFront_Big* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Hit_Air : public NeroState
{
private:
	explicit Hit_Air(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Hit_Air();

public:
	static Hit_Air* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wind_Pressure_Small : public NeroState
{
private:
	explicit Wind_Pressure_Small(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wind_Pressure_Small();

public:
	static Wind_Pressure_Small* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wind_Pressure_Big : public NeroState
{
private:
	explicit Wind_Pressure_Big(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wind_Pressure_Big();

public:
	static Wind_Pressure_Big* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wind_Pressure_Small_End : public NeroState
{
private:
	explicit Wind_Pressure_Small_End(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wind_Pressure_Small_End();

public:
	static Wind_Pressure_Small_End* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wind_Pressure_Big_End : public NeroState
{
private:
	explicit Wind_Pressure_Big_End(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wind_Pressure_Big_End();

public:
	static Wind_Pressure_Big_End* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class KnockBack_Front : public NeroState
{
private:
	explicit KnockBack_Front(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~KnockBack_Front();

public:
	static KnockBack_Front* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class KnockBack_Get_Up : public NeroState
{
private:
	explicit KnockBack_Get_Up(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~KnockBack_Get_Up();

public:
	static KnockBack_Get_Up* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Die : public NeroState
{
private:
	explicit Die(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Die();

public:
	static Die* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Resurrection : public NeroState
{
private:
	explicit Resurrection(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Resurrection();

public:
	static Resurrection* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Resurrection_GetUp : public NeroState
{
private:
	explicit Resurrection_GetUp(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Resurrection_GetUp();

public:
	static Resurrection_GetUp* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};


class StunStart : public NeroState
{
private:
	explicit StunStart(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~StunStart();

public:
	static StunStart* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class StunLoop : public NeroState
{
private:
	explicit StunLoop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~StunLoop();

public:
	static StunLoop* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class StunEnd : public NeroState
{
private:
	explicit StunEnd(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~StunEnd();

public:
	static StunEnd* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class EarthQuakeStart : public NeroState
{
private:
	explicit EarthQuakeStart(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~EarthQuakeStart();

public:
	static EarthQuakeStart* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class EarthQuakeLoop : public NeroState
{
private:
	explicit EarthQuakeLoop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~EarthQuakeLoop();

public:
	static EarthQuakeLoop* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class EarthQuakeEnd : public NeroState
{
private:
	explicit EarthQuakeEnd(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~EarthQuakeEnd();

public:
	static EarthQuakeEnd* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wire_Pull : public NeroState
{
private:
	explicit Wire_Pull(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wire_Pull();

public:
	static Wire_Pull* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wire_Pull_Up : public NeroState
{
private:
	explicit Wire_Pull_Up(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wire_Pull_Up();

public:
	static Wire_Pull_Up* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wire_Pull_Down : public NeroState
{
private:
	explicit Wire_Pull_Down(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wire_Pull_Down();

public:
	static Wire_Pull_Down* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wire_Fly_Start : public NeroState
{
private:
	explicit Wire_Fly_Start(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wire_Fly_Start();

public:
	static Wire_Fly_Start* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wire_Fly_Loop : public NeroState
{
private:
	explicit Wire_Fly_Loop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wire_Fly_Loop();

public:
	static Wire_Fly_Loop* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wire_Fly_End : public NeroState
{
private:
	explicit Wire_Fly_End(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wire_Fly_End();

public:
	static Wire_Fly_End* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wire_Pull_Air : public NeroState
{
private:
	explicit Wire_Pull_Air(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wire_Pull_Air();

public:
	static Wire_Pull_Air* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wire_Pull_Air_Up : public NeroState
{
private:
	explicit Wire_Pull_Air_Up(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wire_Pull_Air_Up();

public:
	static Wire_Pull_Air_Up* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wire_Pull_Air_Down : public NeroState
{
private:
	explicit Wire_Pull_Air_Down(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wire_Pull_Air_Down();

public:
	static Wire_Pull_Air_Down* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wire_Pull_Air_Hard : public NeroState
{
private:
	explicit Wire_Pull_Air_Hard(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wire_Pull_Air_Hard();

public:
	static Wire_Pull_Air_Hard* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wire_Pull_Air_Hard_Up : public NeroState
{
private:
	explicit Wire_Pull_Air_Hard_Up(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wire_Pull_Air_Hard_Up();

public:
	static Wire_Pull_Air_Hard_Up* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Wire_Pull_Air_Hard_Down : public NeroState
{
private:
	explicit Wire_Pull_Air_Hard_Down(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Wire_Pull_Air_Hard_Down();

public:
	static Wire_Pull_Air_Hard_Down* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att1 : public NeroState
{
private:
	explicit BT_Att1(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att1();

public:
	static BT_Att1* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att2 : public NeroState
{
private:
	explicit BT_Att2(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att2();

public:
	static BT_Att2* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att3 : public NeroState
{
private:
	explicit BT_Att3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att3();

public:
	static BT_Att3* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att4 : public NeroState
{
private:
	explicit BT_Att4(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att4();

public:
	static BT_Att4* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att_ComboB : public NeroState
{
private:
	explicit BT_Att_ComboB(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att_ComboB();

public:
	static BT_Att_ComboB* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att_ComboC_R_to_L : public NeroState
{
private:
	explicit BT_Att_ComboC_R_to_L(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att_ComboC_R_to_L();

public:
	static BT_Att_ComboC_R_to_L* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att_ComboC_L_to_R : public NeroState
{
private:
	explicit BT_Att_ComboC_L_to_R(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att_ComboC_L_to_R();

public:
	static BT_Att_ComboC_L_to_R* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att_ComboC_1 : public NeroState
{
private:
	explicit BT_Att_ComboC_1(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att_ComboC_1();

public:
	static BT_Att_ComboC_1* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att_ComboC_2 : public NeroState
{
private:
	explicit BT_Att_ComboC_2(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att_ComboC_2();

public:
	static BT_Att_ComboC_2* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att_ComboC_3 : public NeroState
{
private:
	explicit BT_Att_ComboC_3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att_ComboC_3();

public:
	static BT_Att_ComboC_3* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att_ComboC_4 : public NeroState
{
private:
	explicit BT_Att_ComboC_4(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att_ComboC_4();

public:
	static BT_Att_ComboC_4* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att_ComboD_1 : public NeroState
{
private:
	explicit BT_Att_ComboD_1(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att_ComboD_1();

public:
	static BT_Att_ComboD_1* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att_ComboD_2 : public NeroState
{
private:
	explicit BT_Att_ComboD_2(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att_ComboD_2();

public:
	static BT_Att_ComboD_2* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att_ComboD_3 : public NeroState
{
private:
	explicit BT_Att_ComboD_3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att_ComboD_3();

public:
	static BT_Att_ComboD_3* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att_ComboD_4 : public NeroState
{
private:
	explicit BT_Att_ComboD_4(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att_ComboD_4();

public:
	static BT_Att_ComboD_4* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Air_Att1 : public NeroState
{
private:
	explicit BT_Air_Att1(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Air_Att1();

public:
	static BT_Air_Att1* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Air_Att2 : public NeroState
{
private:
	explicit BT_Air_Att2(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Air_Att2();

public:
	static BT_Air_Att2* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Air_Att3 : public NeroState
{
private:
	explicit BT_Air_Att3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Air_Att3();

public:
	static BT_Air_Att3* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Air_ComboB : public NeroState
{
private:
	explicit BT_Air_ComboB(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Air_ComboB();

public:
	static BT_Air_ComboB* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att1_To_RunStart : public NeroState
{
private:
	explicit BT_Att1_To_RunStart(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att1_To_RunStart();

public:
	static BT_Att1_To_RunStart* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att1_To_RunLoop : public NeroState
{
private:
	explicit BT_Att1_To_RunLoop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att1_To_RunLoop();

public:
	static BT_Att1_To_RunLoop* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att1_To_Idle : public NeroState
{
private:
	explicit BT_Att1_To_Idle(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att1_To_Idle();

public:
	static BT_Att1_To_Idle* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att1_To_Idle_End : public NeroState
{
private:
	explicit BT_Att1_To_Idle_End(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att1_To_Idle_End();

public:
	static BT_Att1_To_Idle_End* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att2_To_RunStart : public NeroState
{
private:
	explicit BT_Att2_To_RunStart(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att2_To_RunStart();

public:
	static BT_Att2_To_RunStart* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att2_To_Idle : public NeroState
{
private:
	explicit BT_Att2_To_Idle(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att2_To_Idle();

public:
	static BT_Att2_To_Idle* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Att1_Dash : public NeroState
{
private:
	explicit BT_Att1_Dash(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Att1_Dash();

public:
	static BT_Att1_Dash* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class BT_Parrying : public NeroState
{
private:
	explicit BT_Parrying(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~BT_Parrying();

public:
	static BT_Parrying* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Skill_E_Start : public NeroState
{
private:
	explicit Skill_E_Start(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Skill_E_Start();

public:
	static Skill_E_Start* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Skill_E_Loop : public NeroState
{
private:
	explicit Skill_E_Loop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Skill_E_Loop();

public:
	static Skill_E_Loop* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Skill_Split : public NeroState
{
private:
	explicit Skill_Split(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Skill_Split();

public:
	static Skill_Split* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Skill_Split_Loop : public NeroState
{
private:
	explicit Skill_Split_Loop(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Skill_Split_Loop();

public:
	static Skill_Split_Loop* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Skill_Split_Landing : public NeroState
{
private:
	explicit Skill_Split_Landing(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Skill_Split_Landing();

public:
	static Skill_Split_Landing* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Skill_Float_Ground : public NeroState
{
private:
	explicit Skill_Float_Ground(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Skill_Float_Ground();

public:
	static Skill_Float_Ground* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Skill_Float_Ground_Ex3 : public NeroState
{
private:
	explicit Skill_Float_Ground_Ex3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Skill_Float_Ground_Ex3();

public:
	static Skill_Float_Ground_Ex3* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Skill_Float_Ground_Finish : public NeroState
{
private:
	explicit Skill_Float_Ground_Finish(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Skill_Float_Ground_Finish();

public:
	static Skill_Float_Ground_Finish* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Skill_Float_Air : public NeroState
{
private:
	explicit Skill_Float_Air(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Skill_Float_Air();

public:
	static Skill_Float_Air* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Skill_Float_Air_Ex3 : public NeroState
{
private:
	explicit Skill_Float_Air_Ex3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Skill_Float_Air_Ex3();

public:
	static Skill_Float_Air_Ex3* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Skill_Shuffle : public NeroState
{
private:
	explicit Skill_Shuffle(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Skill_Shuffle();

public:
	static Skill_Shuffle* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Skill_Caliber : public NeroState
{
private:
	explicit Skill_Caliber(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Skill_Caliber();

public:
	static Skill_Caliber* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Skill_Streak : public NeroState
{
private:
	explicit Skill_Streak(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Skill_Streak();

public:
	static Skill_Streak* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class Skill_Streak_Ex3 : public NeroState
{
private:
	explicit Skill_Streak_Ex3(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~Skill_Streak_Ex3();

public:
	static Skill_Streak_Ex3* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};


class GT_Equip : public NeroState
{
private:
	explicit GT_Equip(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~GT_Equip();

public:
	static GT_Equip* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class GT_Crush_Reload : public NeroState
{
private:
	explicit GT_Crush_Reload(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~GT_Crush_Reload();

public:
	static GT_Crush_Reload* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};


class GT_Crush_Just : public NeroState
{
private:
	explicit GT_Crush_Just(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~GT_Crush_Just();

public:
	static GT_Crush_Just* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class GT_Bomb : public NeroState
{
private:
	explicit GT_Bomb(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~GT_Bomb();

public:
	static GT_Bomb* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class GT_PickUp : public NeroState
{
private:
	explicit GT_PickUp(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~GT_PickUp();

public:
	static GT_PickUp* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class GT_Air_Crush_Reload : public NeroState
{
private:
	explicit GT_Air_Crush_Reload(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~GT_Air_Crush_Reload();

public:
	static GT_Air_Crush_Reload* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};


class GT_Air_Crush_Just : public NeroState
{
private:
	explicit GT_Air_Crush_Just(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~GT_Air_Crush_Just();

public:
	static GT_Air_Crush_Just* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};


class GT_Air_Bomb : public NeroState
{
private:
	explicit GT_Air_Bomb(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~GT_Air_Bomb();

public:
	static GT_Air_Bomb* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class GT_Spark_Basic : public NeroState
{
private:
	explicit GT_Spark_Basic(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~GT_Spark_Basic();

public:
	static GT_Spark_Basic* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};

class GT_Spark_Basic_Up : public NeroState
{
private:
	explicit GT_Spark_Basic_Up(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);
public:
	virtual ~GT_Spark_Basic_Up();

public:
	static GT_Spark_Basic_Up* Create(FSMBase* const _pFSM, const UINT _nIndex, weak_ptr<Nero> _pNero);

public:
	virtual HRESULT StateEnter()							override;
	virtual HRESULT StateExit()								override;
	virtual HRESULT StateUpdate(const float _fDeltaTime)	override;
};





#endif // NeroState_h__