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
	m_vecState.emplace_back(Idle_Start::Create(this, IDLE_START, m_pNero));
	m_vecState.emplace_back(Idle_Battle::Create(this, IDLE_BATTLE, m_pNero));
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
	m_vecState.emplace_back(Skill_Streak_Loop::Create(this, SKILL_STREAK_LOOP, m_pNero));
	m_vecState.emplace_back(Skill_Streak_End::Create(this, SKILL_STREAK_END, m_pNero));
	m_vecState.emplace_back(Skill_Streak_Ex3::Create(this, SKILL_STREAK_EX3, m_pNero));
	m_vecState.emplace_back(Skill_Streak_Ex3_Rush::Create(this, SKILL_STREAK_EX3_RUSH, m_pNero));
	m_vecState.emplace_back(Skill_Streak_Ex3_Roll_Loop::Create(this, SKILL_STREAK_EX3_ROLL_LOOP, m_pNero));
	m_vecState.emplace_back(Skill_Streak_Ex3_Roll_End::Create(this, SKILL_STREAK_EX3_ROLL_END, m_pNero));

	m_vecState.emplace_back(Skill_Float_Ground::Create(this, SKILL_FLOAT_GROUND, m_pNero));
	m_vecState.emplace_back(Skill_Float_Ground_Ex3::Create(this, SKILL_FLOAT_GROUND_EX3, m_pNero));
	m_vecState.emplace_back(Skill_Float_Ground_Finish::Create(this, SKILL_FLOAT_GROUND_FINISH, m_pNero));

	m_vecState.emplace_back(Skill_Shuffle::Create(this, SKILL_SHUFFLE, m_pNero));

#pragma region GT

	m_vecState.emplace_back(Overture_Shoot::Create(this, OVERTURE_SHOOT, m_pNero));
	m_vecState.emplace_back(Overture_Shoot_Down::Create(this, OVERTURE_SHOOT_DOWN, m_pNero));
	m_vecState.emplace_back(Overture_Shoot_Up::Create(this, OVERTURE_SHOOT_UP, m_pNero));

#pragma endregion

#pragma region Cbs

	m_vecState.emplace_back(Cbs_Idle::Create(this, CBS_IDLE, m_pNero));
	m_vecState.emplace_back(Cbs_ComboA1::Create(this, CBS_COMBOA1, m_pNero));
	m_vecState.emplace_back(Cbs_ComboA2::Create(this, CBS_COMBOA2, m_pNero));
	m_vecState.emplace_back(Cbs_ComboA3::Create(this, CBS_COMBOA3, m_pNero));
	m_vecState.emplace_back(Cbs_ComboA4::Create(this, CBS_COMBOA4, m_pNero));
	m_vecState.emplace_back(Cbs_ComboA5::Create(this, CBS_COMBOA5, m_pNero));
	m_vecState.emplace_back(Cbs_SKill_IceAge_Start::Create(this, CBS_ICEAGE_START, m_pNero));
	m_vecState.emplace_back(Cbs_SKill_IceAge_Loop::Create(this, CBS_ICEAGE_LOOP, m_pNero));
	m_vecState.emplace_back(Cbs_SKill_IceAge_End::Create(this, CBS_ICEAGE_END, m_pNero));
	m_vecState.emplace_back(Cbs_SKill_Revolver_Start::Create(this, CBS_REVOLVER_START, m_pNero));
	m_vecState.emplace_back(Cbs_SKill_Revolver_Loop::Create(this, CBS_REVOLVER_LOOP, m_pNero));
	m_vecState.emplace_back(Cbs_SKill_Revolver_End::Create(this, CBS_REVOLVER_END, m_pNero));
	m_vecState.emplace_back(Cbs_SKill_Swing::Create(this, CBS_SWING, m_pNero));
	
	m_vecState.emplace_back(Middle_Cbs_BiAttack::Create(this, MIDDLE_CBS_BI_ATTACK, m_pNero));
	m_vecState.emplace_back(Middle_Cbs_BlitzAttack::Create(this, MIDDLE_CBS_BLITZATTACK, m_pNero));
	m_vecState.emplace_back(Middle_Cbs_Satellite::Create(this, MIDDLE_CBS_SATELLITE, m_pNero));
	m_vecState.emplace_back(Middle_Cbs_Satellite_Air::Create(this, MIDDLE_CBS_SATELLITE_AIR, m_pNero));
	m_vecState.emplace_back(Middle_Cbs_Strike::Create(this, MIDDLE_CBS_STRIKE, m_pNero));
	m_vecState.emplace_back(Middle_Cbs_Strike_Air_Down::Create(this, MIDDLE_CBS_STRIKE_DOWN, m_pNero));
	m_vecState.emplace_back(Middle_Cbs_Strike_Up::Create(this, MIDDLE_CBS_STRIKE_UP, m_pNero));
	m_vecState.emplace_back(Middle_Cbs_Strike_Air::Create(this, MIDDLE_CBS_STRIKE_AIR, m_pNero));
	m_vecState.emplace_back(Middle_Cbs_Strike_Air_Down::Create(this, MIDDLE_CBS_STRIKE_AIR_DOWN, m_pNero));
	m_vecState.emplace_back(Middle_Cbs_Strike_Air_Up::Create(this, MIDDLE_CBS_STRIKE_AIR_UP, m_pNero));
	m_vecState.emplace_back(Middle_Cbs_ThunderBullet::Create(this, MIDDLE_CBS_THUNDERBULLET, m_pNero));

	m_vecState.emplace_back(Pole_ComboA1::Create(this, POLE_COMBOA1, m_pNero));
	m_vecState.emplace_back(Pole_ComboA2::Create(this, POLE_COMBOA2, m_pNero));
	m_vecState.emplace_back(Pole_ComboA3::Create(this, POLE_COMBOA3, m_pNero));
	m_vecState.emplace_back(Pole_ComboB1::Create(this, POLE_COMBOB1, m_pNero));
	m_vecState.emplace_back(Pole_ComboB2::Create(this, POLE_COMBOB2, m_pNero));
	m_vecState.emplace_back(Pole_ComboB3::Create(this, POLE_COMBOB3, m_pNero));
	m_vecState.emplace_back(Pole_ComboB4::Create(this, POLE_COMBOB4, m_pNero));
	m_vecState.emplace_back(Pole_WhirlWind_Start::Create(this, POLE_WHIRLWIND_START, m_pNero));
	m_vecState.emplace_back(Pole_WhirlWind_Loop::Create(this, POLE_WHIRLWIND_LOOP, m_pNero));
	m_vecState.emplace_back(Pole_WhirlWind_End::Create(this, POLE_WHIRLWIND_END, m_pNero));
#pragma endregion
	return S_OK;						
}

HRESULT NeroFSM::UpdateFSM(const float _fDeltaTime)
{
	FSMBase::UpdateFSM(_fDeltaTime);
	return S_OK;
}
