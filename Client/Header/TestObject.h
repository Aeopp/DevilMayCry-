#ifndef __TEST_OBJECT_H__
#define __TEST_OBJECT_H__
#include "GameObject.h"
#include "RenderInterface.h"

class TestObject : public ENGINE::GameObject ,
				   public ENGINE::RenderInterface				
{
private:
	std::shared_ptr<ENGINE::StaticMesh> _StaticMesh{};
private:
	explicit TestObject() = default;
	virtual ~TestObject() = default;
	// GameObject을(를) 통해 상속됨
	virtual void Free() override;
	virtual std::string GetName() override;
public:
	static TestObject* Create();
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


	
};
#endif //