#include "SphereCollider.h"
#include "PhysicsSystem.h"

USING(ENGINE)

SphereCollider::SphereCollider(std::weak_ptr<GameObject> const _pGameObject)
	: Collider(_pGameObject)
	, m_vCenter(0.f, 0.f, 0.f)
	, m_fRadius(0.5f)
{
}

void SphereCollider::Free()
{
	Collider::Free();
}

SphereCollider* SphereCollider::Create(std::weak_ptr<GameObject> const _pGameObject)
{
	SphereCollider* pInstance = new SphereCollider(_pGameObject);
	return pInstance;
}

HRESULT SphereCollider::ReadySphereCollider()
{
	m_pMaterial = PhysicsSystem::GetInstance()->GetDefaultMaterial();
	m_pMaterial->acquireReference();
	//Create BoxShape
	m_pShape = PhysicsSystem::GetInstance()->GetPxPhysics()->createShape(PxSphereGeometry(m_fRadius), *m_pMaterial, true);
	//
	ReadyCollider();
	return S_OK;
}

D3DXVECTOR3 SphereCollider::GetCenter()
{
	return m_vCenter;
}

void SphereCollider::SetCenter(const D3DXVECTOR3 _vCenter)
{
	if (_vCenter == m_vCenter)
		return;

	m_vCenter = _vCenter;

	physx::PxTransform localPose = m_pShape->getLocalPose();

	localPose.p.x = _vCenter.x;
	localPose.p.y = _vCenter.y;
	localPose.p.z = _vCenter.z;

	m_pShape->setLocalPose(localPose);
}

float SphereCollider::GetRadius()
{
	return m_fRadius;
}

void SphereCollider::SetRadius(const float _fRadius)
{
	if (fabs(m_fRadius - _fRadius) < FLT_EPSILON)
		return;

	m_fRadius = _fRadius;

	PxSphereGeometry sphereGeometry;

	m_pShape->getSphereGeometry(sphereGeometry);

	sphereGeometry.radius = m_fRadius;

	m_pShape->setGeometry(sphereGeometry);
}
