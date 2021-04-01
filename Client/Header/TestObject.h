#ifndef __TEST_OBJECT_H__
#define __TEST_OBJECT_H__
#include "GameObject.h"

class TestObject : public ENGINE::GameObject
{
private:
	explicit TestObject() = default
	virtual ~TestObject() = default;
	// GameObject을(를) 통해 상속됨
	virtual void Free() override;
public:
	virtual HRESULT Ready() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT    Update(const float _fDeltaTime) override;
	virtual UINT    LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT	Render() override;
	virtual void	OnEnable() override;
	virtual void    OnDisable() override;
};
#endif //