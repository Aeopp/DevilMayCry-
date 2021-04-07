#include "stdafx.h"
#include "NeroFSM.h"

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
	return S_OK;
}

HRESULT NeroFSM::UpdateFSM(const float _fDeltaTime)
{
	FSMBase::UpdateFSM(_fDeltaTime);
	return S_OK;
}
