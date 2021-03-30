#include "Component.h"
#include "GameObject.h"

USING(ENGINE)

Component::Component(std::weak_ptr<GameObject> const _pGameObject)
	: m_bActive(true)
	, m_pGameObject(_pGameObject)
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
