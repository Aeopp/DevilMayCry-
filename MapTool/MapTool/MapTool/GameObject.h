#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
class Transform;
#include "Physics.h"
class GameObject
{
public:
	std::string m_sMesh;
	Transform* m_pTransform;

	PxRigidActor* m_pRigidActor;
	
	int m_nIndex;
public:
	GameObject();
	~GameObject();

public:
	void CreateRigidActor(PxShape* _pTriangleMesh);

	void Update();
};

#endif // !__GAMEOBJECT_H__
