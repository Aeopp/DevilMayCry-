#ifndef __CAPSULE_COLLIDER_H__
#define __CAPSULE_COLLIDER_H__
#include "Collider.h"
BEGIN(ENGINE)
class CapsuleCollider final: public Collider
{
private:
	D3DXVECTOR3 m_vCenter;
	float		m_fRadius;
	float		m_fHalfHeight;
private:
	explicit CapsuleCollider(std::weak_ptr<GameObject> const _pGameObject);
	virtual ~CapsuleCollider() = default;
	// Collider을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CapsuleCollider* Create(std::weak_ptr<GameObject> const _pGameObject);
public:
	HRESULT ReadyCapsuleCollider();
};
END
#endif // !__CAPSULE_COLLIDER_H__
