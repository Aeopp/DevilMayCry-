#ifndef RedQueen_h__
#define RedQueen_h__

#include "GameObject.h"
#include "RenderInterface.h"
class Nero;
class RedQueen : public GameObject,
	public ENGINE::RenderInterface
{
private:
	explicit RedQueen();
	virtual ~RedQueen() = default;
	// GameObject을(를) 통해 상속됨
	virtual void Free() override;
public:
	static RedQueen* Create();
public:
	virtual HRESULT Ready() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual std::string GetName() override;
public:
	void	SetWeaponState(UINT _StateIndex) { m_iStateIndex = _StateIndex; }
public:
	// RenderInterface을(를) 통해 상속됨
	virtual void RenderReady() override;
	
	virtual void Editor()override;

private:
	std::weak_ptr<Nero>					m_pNero;
	std::shared_ptr<ENGINE::SkeletonMesh> m_pMesh;

	std::vector<Matrix*>					m_vecParentMat;
	UINT								m_iStateIndex = 0;
	Matrix* Test;
};



#endif // RedQueen_h__