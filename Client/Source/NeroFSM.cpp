#include "stdafx.h"
#include "NeroFSM.h"
#include "Nero.h"
#include "NeroState.h"
NeroFSM::NeroFSM(weak_ptr<Nero> _pNero)
	:m_pNero(_pNero)
{
}

NeroFSM::~NeroFSM()
{

}

NeroFSM* NeroFSM::Create(weak_ptr<Nero> _pNero)
{
	NeroFSM* pInstance = new NeroFSM(_pNero);
	if (FAILED(pInstance->ReadyFSM()))
	{
		delete pInstance;
		pInstance = nullptr;
	}
	return pInstance;
}

HRESULT NeroFSM::ReadyFSM()
{
	FSMBase::ReadyFSM();
	
	ReserveContainer(STATE_END);

	m_vecState.emplace_back(Idle::Create(this, IDLE, m_pNero));
	m_vecState.emplace_back(Idle_Battle::Create(this, IDLE_BATTLE, m_pNero));
	m_vecState.emplace_back(RunStartFront::Create(this, RUNSTART, m_pNero));
	m_vecState.emplace_back(RunLoop::Create(this, RUNLOOP, m_pNero));
	m_vecState.emplace_back(RunStop::Create(this, RUNSTOP, m_pNero));
	return S_OK;						
}

HRESULT NeroFSM::UpdateFSM(const float _fDeltaTime)
{
	FSMBase::UpdateFSM(_fDeltaTime);
	return S_OK;
}
