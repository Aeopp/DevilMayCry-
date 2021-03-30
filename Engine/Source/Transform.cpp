#include "Transform.h"

USING(ENGINE)

Transform::Transform(GameObject* const _pGameObject)
	: Component(_pGameObject)
{
}

void Transform::Free()
{
	Component::Free();
}

Transform* Transform::Create(GameObject* const _pGameObject)
{
	Transform* pInstance = new Transform(_pGameObject);
	return pInstance;
}
