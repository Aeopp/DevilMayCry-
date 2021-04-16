#ifndef __CAR_H__
#define __CAR_H__
#include "GameObject.h"
#include "RenderInterface.h"

class Em5000;
class TestObject;
class Car : public ENGINE::GameObject ,
				   public ENGINE::RenderInterface				
{
private:
	std::shared_ptr<ENGINE::StaticMesh> _StaticMesh{};
private:
	explicit Car() = default;
	virtual ~Car() = default;
	// GameObject을(를) 통해 상속됨
	virtual void Free() override;
	virtual std::string GetName() override;
public:
	static Car* Create();
public:
	virtual void    RenderForwardAlphaBlendImplementation(const ImplementationInfo&_ImplInfo)override;
	virtual void    RenderDebugImplementation(const ImplementationInfo& _ImplInfo)override;
	virtual void    RenderReady()                          override;
public:
	virtual HRESULT Ready() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT    Update(const float _fDeltaTime) override;
	virtual UINT    LateUpdate(const float _fDeltaTime) override;
	virtual void    Editor()override;
	virtual void	OnEnable() override;
	virtual void    OnDisable() override;


public:
	void	Throw(const float _fDeltaTime);


	std::weak_ptr<ENGINE::Transform>	m_pMonsterTrans;
	std::weak_ptr<Em5000>				m_pMonster;
	std::weak_ptr<ENGINE::SkeletonMesh> m_pMonsterMesh;

	std::weak_ptr<ENGINE::Transform> m_pPlayerTrans;
	std::weak_ptr<TestObject>		 m_pPlayer;


	std::optional<Matrix>				  m_ParentBone;
	Matrix*								  m_pParentBone;
	Matrix								  m_ParentWorld;
	Matrix								  m_Result;


	Vector3 m_vDir;
	float	m_fDir = 0.f;
	float	m_fThrowTime = 0.f;
	bool	m_bThrow = false;
	bool	m_bBone = false;
	Vector3	m_vPlayerPos;


};
#endif //