#ifndef Em0000_h__
#define Em0000_h__

#include "Monster.h"


class TestObject;
class Em0000 final : public Monster
{
private:
	enum Em0000_State
	{
		Attack_1,
		Attack_2,
		Attack_Hard,
		Buster_End,
		Buster_Start,
		Dead,
		Guard_End,
		Guard_Hit1,
		Guard_Hit2,
		Guard_Loop,
		Guard_Start,
		Hit_Back,
		Hit_Front,
		Hit_L,
		Hit_R,
		Move_Front_End,
		Move_Front_Loop,
		Move_Front_Start,
		Step_Back,
		Stun_End,
		Stun_Start,
		idle,
		State_END
	};



private:
	explicit Em0000() = default;
	virtual ~Em0000() = default;

	virtual void Free() override;
	virtual std::string GetName() override;

public:
	static Em0000* Create();
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
private:
	//���� ����
	Em0000_State	m_eState =State_END;		
	//TestPlayer �޾ƿ�.
	std::weak_ptr<ENGINE::Transform> m_pPlayerTrans;
	std::weak_ptr<TestObject>		 m_pPlayer;

	//���� �� �̵� ����
	bool		m_bMove = false;
	float		m_fMoveTime = 0.f;

	bool		m_bAttack = false;	
	float		m_fAttackTime = 0.f;

	//���� ���� �׽�Ʈ ��
	bool		m_bTest = false;

	//Rotate�Լ� ����
	bool		m_bRotate = false;
	float		m_fTest = 0.f;
};

#endif // Em0000_h__