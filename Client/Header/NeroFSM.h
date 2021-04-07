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
		RUNSTART,
		RUNLOOP,
		RUNSTOP,
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