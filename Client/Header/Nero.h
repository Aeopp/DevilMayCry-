#ifndef Player_h__
#define Player_h__

#pragma once
#include "GameObject.h"
#include "RenderInterface.h"
class NeroFSM;

class Nero :   public GameObject ,
	public ENGINE::RenderInterface

{
private:
	explicit Nero();
	virtual ~Nero() = default;
	// GameObject을(를) 통해 상속됨
	virtual void Free() override;
	virtual std::string GetName() override;
public:
	static Nero* Create();
public:
	virtual void RenderReady() override;
	virtual void RenderDebugImplementation(const ImplementationInfo& _ImplInfo)override;
	virtual void RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo)override;
	virtual void RenderDebugBoneImplementation(const ImplementationInfo& _ImplInfo)override;
	virtual void Editor()override;
public:
	float Get_PlayingTime();
public:
	virtual HRESULT Ready() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	std::shared_ptr<ENGINE::SkeletonMesh> m_pMesh;
	std::shared_ptr<NeroFSM> m_pFSM;

	// RenderInterface을(를) 통해 상속됨
public:
	enum Animation
	{
		IDLE,
		IDLE_SWITCH_LEG,
		IDLE_BATTLE,
		JUMP_BASIC,
		JUMP_FLY_LOOP,
		JUMP_LANDING,
		JUMP_LANDING_GUN,
		JUMP_LANDING_HIGH,
		JUMP_TWICE,
		JUMP_FRONT,
		JUMP_FRONT_LANDING,
		JUMP_FRONT_TWICE,
		JUMP_BACK,
		JUMP_BACK_TWICE,
		JUMP_STINGER,
		JUMP_STINGER_LOOP,
		JUMP_STINGER_LANDING,
		RUNLOOP,
		RUNSTARTFRONT,
		RUNSTOP,
		RUNTURN,
		RUNTURN_L,
		DASHLOOP,
		DASHSTOP,
		DASHTURN,
		GUNWALKFRONT,
		GUNWALKLEFT,
		GUNWALKBACK,
		GUNWALKRIGHT,
		GUNWALKRIGHTBACK,
		WALKLOOP,
		WALKSTART,
		WALKSTARTRIGHT,
		WALKSTART180,
		WALKSTARTLEFT,
		WALKSTOP,
		WALKTURN180,
		EVADE_L,
		EVADE_R,
		IDLE_TO_SHOOT,
		IDLE_TO_SHOOT_LEFT,
		IDLE_TO_SHOOT_RIGHT,
		SHOOT_TO_IDLE,
		HITFRONT,
		HITFRONT_BIG,
		HIT_AIR,
		WIND_PRESSURE_SMALL,
		WIND_PRESSURE_BIG,
		WIND_PRESSURE_SMALL_END,
		WIND_PRESSURE_BIG_END,
		KNOCKBACK_FRONT,
		KNOCKBACK_GET_UP,
		DIE,
		RESURRECTION,
		RESURRECTION_GETUP,
		STUNSTART,
		STUNLOOP,
		STUNEND,
		EARTHQUAKESTART,
		EARTHQUAKELOOP,
		EARTHQUAKEEND,
		WIRE_PULL,
		WIRE_PULL_UP,
		WIRE_PULL_DOWN,
		WIRE_FLY_START,
		WIRE_FLY_LOOP,
		WIRE_FLY_END,
		WIRE_PULL_AIR,
		WIRE_PULL_AIR_UP,
		WIRE_PULL_AIR_DOWN,
		WIRE_PULL_AIR_HARD,
		WIRE_PULL_AIR_HARD_UP,
		WIRE_PULL_AIR_HARD_DOWN,
		BT_ATT1,
		BT_ATT2,
		BT_ATT3,
		BT_ATT4,
		BT_ATT_COMBOB,
		BT_ATT_COMBOC_R_TO_L,
		BT_ATT_COMBOC_L_TO_R,
		BT_ATT_COMBOC_1,
		BT_ATT_COMBOC_2,
		BT_ATT_COMBOC_3,
		BT_ATT_COMBOC_4,
		BT_ATT_COMBOD_1,
		BT_ATT_COMBOD_2,
		BT_ATT_COMBOD_3,
		BT_ATT_COMBOD_4,
		BT_AIR_ATT1,
		BT_AIR_ATT2,
		BT_AIR_ATT3,
		BT_AIR_COMBOB,
		BT_ATT1_TO_RUNSTART,
		BT_ATT1_TO_RUNLOOP,
		BT_ATT1_TO_IDLE,
		BT_ATT1_TO_IDLE_END,
		BT_ATT2_TO_RUNSTART,
		BT_ATT2_TO_IDLE,
		BT_ATT1_DASH,
		BT_PARRYING,
		SKILL_E_START,
		SKILL_E_LOOP,
		SKILL_SPLIT,
		SKILL_SPLIT_LOOP,
		SKILL_SPLIT_LANDING,
		SKILL_FLOAT_GROUND,
		SKILL_FLOAT_GROUND_EX3,
		SKILL_FLOAT_GROUND_FINISH,
		SKILL_FLOAT_AIR,
		SKILL_FLOAT_AIR_EX3,
		SKILL_SHUFFLE,
		SKILL_CALIBER,
		SKILL_STREAK,
		SKILL_STREAK_EX3,
		GT_EQUIP,
		GT_CRUSH_RELOAD,
		GT_CRUSH_JUST,
		GT_BOMB,
		GT_PICKUP,
		GT_AIR_CRUSH_RELOAD,
		GT_AIR_CRUSH_JUST,
		GT_AIR_BOMB,
		GT_SPARK_BASIC,
		GT_SPARK_BASIC_UP,
		ANIMATION_END
	};
};


#endif // Player_h__
