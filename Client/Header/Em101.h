#ifndef Em101_h__
#define Em101_h__

#include "Monster.h"

class TestObject;
class Em101 final : public Monster
{
private:
	enum Em101_State
	{
		Air_End,
		Air_Loop,
		Air_Start,
		Attack_A,
		Attack_D,
		Attack_Hard,
		Dead,
		Hit_Air,
		Hit_Back,
		Hit_End,
		Hit_Finish,
		Hit_Front,
		Hit_L,
		Hit_R,
		Walk_Front_End,
		Walk_Front_Loop,
		Walk_Front_Start,
		Walk_Left_End,
		Walk_Left_Loop,
		Walk_Left_Start,
		Walk_Right_Stop,
		Walk_Right_Loop,
		Walk_Right_Start,
		idle,
		State_END
	};



private:
	explicit Em101() = default;
	virtual ~Em101() = default;

	virtual void Free() override;
	virtual std::string GetName() override;

public:
	static Em101* Create();
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
	Em101_State	m_eState =State_END;		
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

#endif // Em101_h__