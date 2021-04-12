#include "framework.h"
#include "GameObject.h"
#include "Transform.h"

static int nCount = 0;

GameObject::GameObject()
{
	m_pTransform = new Transform();
	m_nIndex = nCount++;
}

GameObject::~GameObject()
{
	delete m_pTransform;
}

void GameObject::CreateRigidActor(PxShape* _pShape)
{
	m_pRigidActor = Physics::Instance.m_pPhysics->createRigidStatic(physx::PxTransform(0.f, 0.f, 0.f));

	m_pRigidActor->attachShape(*_pShape);

	m_pRigidActor->userData = this;

	Physics::Instance.m_pScene->addActor(*m_pRigidActor);
}

void GameObject::Update()
{
	PxVec3 p;
	PxQuat q;
	D3DXVECTOR3 vPos = m_pTransform->GetPosition();
	D3DXQUATERNION vQuat = m_pTransform->GetQuaternion();
	memcpy_s(&p, sizeof(D3DXVECTOR3), &vPos, sizeof(D3DXVECTOR3));
	memcpy_s(&q, sizeof(D3DXQUATERNION), &vQuat, sizeof(D3DXQUATERNION));


	PxTransform globalPose(p, q);

	m_pRigidActor->setGlobalPose(globalPose);
}
