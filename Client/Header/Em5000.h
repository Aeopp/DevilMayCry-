#ifndef Em5000_h__
#define Em5000_h__

#include "Monster.h"


class TestObject;
class Em5000 final : public Monster
{
private:
	enum Em5000_State
	{
		Attack_Back_L,
		Attack_Back_R,
		Attack_Finish,
		Attack_Grap_Car,
		Attack_Hack,
		Attack_Jump_Attack,
		Attack_Punch_Twice,
		Attack_Rush_Start,
		Attack_Rush_Loop,
		Attack_Rush_End,
		Attack_Side_L,
		Attack_Side_R,
		Attack_Throw_Car,
		Back_Jump,
		Dead,
		Groggy_Dead,
		Groggy_Start,
		Groggy_Loop,
		Hit_Buster_Start,
		Hit_Buster_Swing_Start,
		Hit_Buster_Swing_Loop,
		Hit_Buster_Swing_Throw,
		Hit_Buster_Swing_End,
		Howling,
		Idle,
		Move_Loop,
		Move_Start,
		Tornado_End,
		Tornado_Start,
		Move_Turn_L_90,
		Move_Turn_L_180,
		Move_Turn_R_90,
		Move_Turn_R_180,
		Move_Turn_L,
		Move_Turn_R,
		State_END
	};
private:
	explicit Em5000() = default;
	virtual ~Em5000() = default;

	virtual void Free() override;
	virtual std::string GetName() override;

public:
	static Em5000* Create();
public:
	virtual void RenderDebugImplementation(const ImplementationInfo& _ImplInfo)override;
	virtual void RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo) override;
	virtual void RenderDebugBoneImplementation(const ImplementationInfo& _ImplInfo) override;


	virtual void Fight(const float _fDeltaTime)override;
	virtual void State_Change(const float _fDeltaTime)override;

public:
	virtual HRESULT Ready() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual void Editor() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
public:
	virtual void Rotate(const float _fDeltaTime) override;
	virtual void Update_Angle() override;

	void	Turn();
private:
	//���� ����
	Em5000_State	m_eState =State_END;		
	//TestPlayer �޾ƿ�.
	std::weak_ptr<ENGINE::Transform> m_pPlayerTrans;
	std::weak_ptr<TestObject>		 m_pPlayer;

	//���� �� �̵� ����
	bool		m_bMove = false;
	float		m_fMoveTime = 0.f;

	bool		m_bAttack = false;	
	float		m_fAttackTime = 0.f;

	bool		m_bSideAttack = false;
	float		m_fSideAttackTime = 0.f;

	//������
	bool		m_bTurn = false;
	bool		m_bTurnEnd = false;
	float		m_fTurnTime = 0.f;


	//���� ���� �׽�Ʈ ��
	bool		m_bTest = false;
};

#endif // Em5000_h__