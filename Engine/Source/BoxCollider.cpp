#include "BoxCollider.h"
#include "Physics.h"

USING(ENGINE)

BoxCollider::BoxCollider(std::weak_ptr<GameObject> const _pGameObject)
    : Collider(_pGameObject)
    , m_vCenter(0.f, 0.f, 0.f)
    , m_vSize(1.f, 1.f, 1.f)
{
}

void BoxCollider::Free()
{
    Collider::Free();
}

BoxCollider* BoxCollider::Create(std::weak_ptr<GameObject> const _pGameObject)
{
	BoxCollider* pInstance = new BoxCollider(_pGameObject);

	//if (FAILED(pInstance->ReadyBoxCollider()))
	//{
	//	delete pInstance;
	//	return nullptr;
	//}

	return pInstance;
}

HRESULT BoxCollider::ReadyBoxCollider()
{
	physx::PxVec3 vHalfExtent(0.f, 0.f, 0.f);
	memcpy_s(&vHalfExtent, sizeof(D3DXVECTOR3), &m_vSize, sizeof(D3DXVECTOR3));
	vHalfExtent = vHalfExtent * 0.5f;

	m_pMaterial = Physics::GetDefaultMaterial();
	m_pMaterial->acquireReference();
	//Create BoxShape
	m_pShape = Physics::GetPxPhysics()->createShape(physx::PxBoxGeometry(vHalfExtent), *m_pMaterial, true);
	//
	ReadyCollider();
	return S_OK;
}

D3DXVECTOR3 BoxCollider::GetCenter()
{
	return m_vCenter;
}

void BoxCollider::SetCenter(const D3DXVECTOR3 _vCenter)
{
	m_vCenter = _vCenter;

	physx::PxTransform localPose = m_pShape->getLocalPose();

	localPose.p.x = _vCenter.x;
	localPose.p.y = _vCenter.y;
	localPose.p.z = _vCenter.z;

	m_pShape->setLocalPose(localPose);
}

D3DXVECTOR3 BoxCollider::GetSize()
{
	return m_vSize;
}

void BoxCollider::SetSize(const D3DXVECTOR3 _vSize)
{
	m_vSize = _vSize;

	physx::PxBoxGeometry boxGeometry;

	m_pShape->getBoxGeometry(boxGeometry);

	boxGeometry.halfExtents.x = m_vSize.x * 0.5f;
	boxGeometry.halfExtents.y = m_vSize.y * 0.5f;
	boxGeometry.halfExtents.z = m_vSize.z * 0.5f;

	m_pShape->setGeometry(boxGeometry);
}
