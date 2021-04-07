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
};


#endif // Player_h__
