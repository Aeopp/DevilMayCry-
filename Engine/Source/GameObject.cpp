#include "GameObject.h"
#include "Scene.h"

USING(ENGINE)

GameObject::GameObject()
	: m_bActive(true)
	, m_pScene(nullptr)
	, m_nLoopIdx(0)
	, m_bStatic(false)
	, m_bDestroy(false)
{
}

void GameObject::Free()
{
	Object::Free();
}

void GameObject::Destroy(std::weak_ptr<GameObject> const _pGameObject)
{
	m_pScene->Destroy(_pGameObject);
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

	if (_bActive)
		OnEnable();
	else
		OnDisable();

	m_pScene->SetActive(m_pGameObject, _bActive);
}

bool GameObject::IsStatic()
{
	return m_bStatic;
}

void GameObject::SetStatic(const bool _bStatic)
{
	if (_bStatic == m_bStatic)
		return;
	m_bStatic = _bStatic;
}

void GameObject::SetScene(Scene* const _pScene)
{
	m_pScene = _pScene;
}

void GameObject::SetGameObject(std::weak_ptr<GameObject> _pGameObject)
{
	m_pGameObject = _pGameObject;
}

UINT GameObject::GetLoopIdx()
{
	return m_nLoopIdx;
}

void GameObject::SetLoopIdx(const UINT _nLoopIdx)
{
	m_nLoopIdx = _nLoopIdx;
}

void GameObject::OnEnable()
{
}

void GameObject::OnDisable()
{
}
