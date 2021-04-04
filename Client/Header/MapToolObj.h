#ifndef __MAPTOOLOBJ_H__
#define __MAPTOOLOBJ_H__
#include "GameObject.h"
#include "RenderInterface.h"
class MapToolObj : public ENGINE::GameObject , 
					public ENGINE::RenderInterface
{

public:
	bool							m_bRender = true;
	std::weak_ptr<Transform>		Get_Trans() { return GetComponent<ENGINE::Transform>(); };
private:
	std::shared_ptr<ENGINE::StaticMesh> _StaticMesh{};
private:
	explicit MapToolObj() = default;
	virtual ~MapToolObj() = default;
	virtual void Free() override;
public:
	static MapToolObj* Create();
public:
	virtual HRESULT Ready() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	// RenderInterface을(를) 통해 상속됨
	virtual void RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo) override;
	virtual void RenderReady() override;
};


#endif // !__MAPTOOLOBJ_H__