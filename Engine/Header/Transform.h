#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__
#include "Component.h"
BEGIN(ENGINE)
class Transform final : public Component
{
private:
	D3DXVECTOR3	m_vLocalScale;
	D3DXVECTOR3	m_vWorldScale;
	D3DXVECTOR3	m_vLocalRotation;
	D3DXVECTOR3 m_vWorldRotation;
	D3DXVECTOR3	m_vLocalPosition;
	D3DXVECTOR3	m_vWorldPosition;
private:
	explicit Transform(std::weak_ptr<GameObject> const _pGameObject);
	virtual ~Transform() = default;
	// Component을(를) 통해 상속됨
	virtual void Free() override;
public:
	static Transform* Create(std::weak_ptr<GameObject> const _pGameObject);
};
END
#endif // !__TRANSFORM_H__