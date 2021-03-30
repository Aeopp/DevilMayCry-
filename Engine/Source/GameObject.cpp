#include "GameObject.h"

USING(ENGINE)

GameObject::GameObject()
	: m_bActive(true)
{
}

void GameObject::Free()
{
	Object::Free();
}

bool GameObject::IsActive()
{
	return m_bActive;
}

void GameObject::SetActive(const bool _bActive)
{
	if (_bActive == m_bActive)
		return;

	m_bActive = _bActive;
}
