#ifndef __SPHERE_COLLIDER_H__
#define __SPHERE_COLLIDER_H__
#include "Collider.h"
BEGIN(ENGINE)
class ENGINE_DLL SphereCollider : public Collider
{
private:
	D3DXVECTOR3	m_vCenter;
	float		m_fRadius;
private:
	explicit SphereCollider(std::weak_ptr<GameObject> const _pGameObject);
	virtual ~SphereCollider() = default;
	// Collider을(를) 통해 상속됨
	virtual void Free() override;
public:
	static SphereCollider* Create(std::weak_ptr<GameObject> const _pGameObject);
public:
	HRESULT ReadySphereCollider();
public:
	D3DXVECTOR3	GetCenter();
	void		SetCenter(const D3DXVECTOR3 _vCenter);

	float		GetRadius();
	void		SetRadius(const float _fRadius);
};
END
#endif // !__SPHERE_COLLIDER_H__
