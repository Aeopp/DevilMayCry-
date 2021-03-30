#include "Component.h"
#include "GameObject.h"

USING(ENGINE)

Component::Component(GameObject* const _pGameObject)
	: m_bActive(true)
{
}

void Component::Free()
{
	Object::Free();
}

bool Component::IsActive()
{
	return m_bActive;
}

void Component::SetActive(const bool _bActive)
{
	m_bActive = _bActive;
}
