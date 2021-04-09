#pragma once
#ifndef NeroFSM_h__
#define NeroFSM_h__

#include "FSMBase.h"
class Nero;
class NeroFSM :    public FSMBase
{
public:
	enum STATE
	{
		IDLE,
		IDLE_BATTLE,
		IDLE_SWITCH_LEG,
		ATT1,
		ATT2,
		ATT3,
		ATT4,
		RUNSTART,
		RUNSTART_L,
		RUNSTART_R,
		RUNSTART_180,
		RUNLOOP,
		RUNSTOP,
		DASHLOOP,
		DASHSTOP,
		ATT_COMBO_C_R,
		ATT_COMBO_C_L,
		ATT_COMBO_C1,
		ATT_COMBO_C2,
		ATT_COMBO_C3,
		ATT_COMBO_C4,
		ATT_COMBO_D1,
		ATT_COMBO_D2,
		ATT_COMBO_D3,
		ATT_COMBO_D4,
		SKILL_STREAK,
		SKILL_STREAK_EX3,
		SKILL_FLOAT_GROUND,
		SKILL_FLOAT_GROUND_EX3,
		SKILL_FLOAT_GROUND_FINISH,
		STATE_END

	};

private:
	weak_ptr<Nero>	m_pNero;

public:
	explicit NeroFSM(weak_ptr<Nero> _pNero);
	virtual ~NeroFSM();
public:
	static NeroFSM* Create(weak_ptr<Nero> _pNero);
public:
	// FSMBase을(를) 통해 상속됨
	virtual HRESULT ReadyFSM()							override;
	virtual HRESULT UpdateFSM(const float _fDeltaTime)	override;

};



#endif // NeroFSM_h__