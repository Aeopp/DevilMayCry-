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
	m_vecState.emplace_back(Idle_Switch_Leg::Create(this, IDLE_SWITCH_LEG, m_pNero));
	m_vecState.emplace_back(BT_Att1::Create(this, ATT1, m_pNero));
	m_vecState.emplace_back(BT_Att2::Create(this, ATT2, m_pNero));
	m_vecState.emplace_back(BT_Att3::Create(this, ATT3, m_pNero));
	m_vecState.emplace_back(BT_Att4::Create(this, ATT4, m_pNero));
	m_vecState.emplace_back(RunStartFront::Create(this, RUNSTART, m_pNero));
	m_vecState.emplace_back(RunStartLeft::Create(this, RUNSTART_L, m_pNero));
	m_vecState.emplace_back(RunStartRight::Create(this, RUNSTART_R, m_pNero));
	m_vecState.emplace_back(RunStart180::Create(this, RUNSTART_180, m_pNero));
	m_vecState.emplace_back(RunLoop::Create(this, RUNLOOP, m_pNero));
	m_vecState.emplace_back(RunStop::Create(this, RUNSTOP, m_pNero));
	m_vecState.emplace_back(DashLoop::Create(this, DASHLOOP, m_pNero));
	m_vecState.emplace_back(DashStop::Create(this, DASHSTOP, m_pNero));

	m_vecState.emplace_back(BT_Att_ComboC_R_to_L::Create(this, ATT_COMBO_C_R, m_pNero));
	m_vecState.emplace_back(BT_Att_ComboC_L_to_R::Create(this, ATT_COMBO_C_L, m_pNero));
	m_vecState.emplace_back(BT_Att_ComboC_1::Create(this, ATT_COMBO_C1, m_pNero));
	m_vecState.emplace_back(BT_Att_ComboC_2::Create(this, ATT_COMBO_C2, m_pNero));
	m_vecState.emplace_back(BT_Att_ComboC_3::Create(this, ATT_COMBO_C3, m_pNero));
	m_vecState.emplace_back(BT_Att_ComboC_4::Create(this, ATT_COMBO_C4, m_pNero));

	m_vecState.emplace_back(BT_Att_ComboD_1::Create(this, ATT_COMBO_D1, m_pNero));
	m_vecState.emplace_back(BT_Att_ComboD_2::Create(this, ATT_COMBO_D2, m_pNero));
	m_vecState.emplace_back(BT_Att_ComboD_3::Create(this, ATT_COMBO_D3, m_pNero));
	m_vecState.emplace_back(BT_Att_ComboD_4::Create(this, ATT_COMBO_D4, m_pNero));

	m_vecState.emplace_back(Skill_Streak::Create(this, SKILL_STREAK, m_pNero));
	m_vecState.emplace_back(Skill_Streak_Ex3::Create(this, SKILL_STREAK_EX3, m_pNero));

	m_vecState.emplace_back(Skill_Float_Ground::Create(this, SKILL_FLOAT_GROUND, m_pNero));
	m_vecState.emplace_back(Skill_Float_Ground_Ex3::Create(this, SKILL_FLOAT_GROUND_EX3, m_pNero));
	m_vecState.emplace_back(Skill_Float_Ground_Finish::Create(this, SKILL_FLOAT_GROUND_FINISH, m_pNero));

	return S_OK;						
}

HRESULT NeroFSM::UpdateFSM(const float _fDeltaTime)
{
	FSMBase::UpdateFSM(_fDeltaTime);
	return S_OK;
}
