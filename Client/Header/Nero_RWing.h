#pragma once

#ifndef Nero_RWing_h__
#define Nero_RWing_h__

#include "GameObject.h"
#include "RenderInterface.h"
class Nero;
class Nero_RWing :	public GameObject,
	public ENGINE::RenderInterface
{
private:
	explicit Nero_RWing();
	virtual ~Nero_RWing() = default;

	// GameObject을(를) 통해 상속됨
	virtual void Free() override;
public:
	static Nero_RWing* Create();
	virtual HRESULT Ready() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
public:
	// RenderInterface을(를) 통해 상속됨
	virtual void RenderReady() override;
	virtual void RenderDebugImplementation(const ImplementationInfo& _ImplInfo)override;
	virtual void RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo)override;
	virtual void RenderDebugBoneImplementation(const ImplementationInfo& _ImplInfo)override;
	virtual void Editor()override;
	virtual std::string GetName() override;
public:
	bool	Set_RenderState(bool _DrawOrNot) { m_bIsRender = _DrawOrNot; }
private:
	std::shared_ptr<ENGINE::SkeletonMesh> m_pMesh;
	std::weak_ptr<Nero>					m_pNero;


	Matrix*								m_pParentBoneMat;
	bool								m_bIsRender;
};



#endif // Nero_RWing_h__