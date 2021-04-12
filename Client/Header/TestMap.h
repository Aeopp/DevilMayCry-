#ifndef __TEST_MAP_H__
#define __TEST_MAP_H__
#include "GameObject.h"
#include "RenderInterface.h"

class TestMap : public GameObject, public ENGINE::RenderInterface
{
private:
	std::shared_ptr<ENGINE::StaticMesh> m_pMesh;

private:
	explicit TestMap();
	virtual ~TestMap() = default;
	// GameObject을(를) 통해 상속됨
	virtual void Free() override;
public:
	static TestMap* Create();
public:
	virtual void    RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo)override;
	virtual void    RenderDebugImplementation(const ImplementationInfo& _ImplInfo)override;
	virtual void    RenderReady()override;
public:
	virtual HRESULT Ready() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
};

#endif // !__TEST_MAP_H__
