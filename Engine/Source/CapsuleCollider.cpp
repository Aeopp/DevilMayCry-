#include "CapsuleCollider.h"
#include "PhysicsSystem.h"

USING(ENGINE)

CapsuleCollider::CapsuleCollider(std::weak_ptr<GameObject> const _pGameObject)
	: Collider(_pGameObject)
	, m_vCenter(0.f, 0.f, 0.f)
	, m_fRadius(0.5f)
	, m_fHalfHeight(0.5f)
{
}

void CapsuleCollider::Free()
{
	Collider::Free();
}

CapsuleCollider* CapsuleCollider::Create(std::weak_ptr<GameObject> const _pGameObject)
{
	CapsuleCollider* pInstance = new CapsuleCollider(_pGameObject);
	return pInstance;
}

HRESULT CapsuleCollider::ReadyCapsuleCollider()
{
	m_pMaterial = PhysicsSystem::GetInstance()->GetDefaultMaterial();
	m_pMaterial->acquireReference();
	//Create BoxShape

	m_pShape = PhysicsSystem::GetInstance()->GetPxPhysics()->createShape(PxCapsuleGeometry(m_fRadius, m_fHalfHeight), *m_pMaterial, true);

	return S_OK;
}
