#include "Transform.h"
#include "FMath.hpp"

USING(ENGINE)

Transform::Transform(std::weak_ptr<GameObject> const _pGameObject)
	: Component(_pGameObject)
	, m_vLocalScale(1.f, 1.f, 1.f)
	, m_vWorldScale(1.f, 1.f, 1.f)
	, m_vLocalRotation(0.f, 0.f, 0.f)
	, m_vWorldRotation(0.f, 0.f, 0.f)
	, m_vLocalPosition(0.f, 0.f, 0.f)
	, m_vWorldPosition(0.f, 0.f, 0.f)
	, m_bUpdate(false)
{
	m_matScale = m_matRotation = m_matTranslation = *D3DXMatrixIdentity(&m_matWorld);
	D3DXQuaternionRotationYawPitchRoll(&m_tLocalQuaternion, D3DXToRadian(m_vLocalRotation.y), D3DXToRadian(m_vLocalRotation.x), D3DXToRadian(m_vLocalRotation.z));
	m_tWorldQuaternion = m_tLocalQuaternion;
}

void Transform::Free()
{
	Component::Free();
}

Transform* Transform::Create(std::weak_ptr<GameObject> const _pGameObject)
{
	Transform* pInstance = new Transform(_pGameObject);
	return pInstance;
}

D3DXVECTOR3 Transform::GetScale(const SPACE _eSpace)
{
	if (SPACE::LOCAL == _eSpace)
		return m_vLocalScale;
	else
		return m_vWorldScale;
}

void Transform::SetScale(const D3DXVECTOR3& _vScale, const SPACE _eSpace)
{
	if (SPACE::LOCAL == _eSpace)
	{
		m_vLocalScale = _vScale;

		if (nullptr == m_pParent.lock())
			m_vWorldScale = _vScale;
		else
		{
			m_vWorldScale.x = _vScale.x * m_pParent.lock()->GetScale().x;
			m_vWorldScale.y = _vScale.y * m_pParent.lock()->GetScale().y;
			m_vWorldScale.z = _vScale.z * m_pParent.lock()->GetScale().z;
		}
	}
	else
	{
		m_vWorldScale = _vScale;
		if (nullptr == m_pParent.lock())
			m_vLocalScale = _vScale;
		else
		{
			m_vLocalScale.x = m_vWorldScale.x / m_pParent.lock()->GetScale().x;
			m_vLocalScale.y = m_vWorldScale.y / m_pParent.lock()->GetScale().y;
			m_vLocalScale.z = m_vWorldScale.z / m_pParent.lock()->GetScale().z;
		}
	}

	D3DXMatrixScaling(&m_matScale, m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);

	m_bUpdate = true;
}

D3DXVECTOR3 Transform::GetRotation(const SPACE _eSpace)
{
	if (SPACE::LOCAL == _eSpace)
		return m_vLocalRotation;
	else
		return m_vWorldRotation;
}

void Transform::SetRotation(const D3DXVECTOR3& _vRotation, const SPACE _eSpace)
{
	if (SPACE::LOCAL == _eSpace)
	{
		m_vLocalRotation = _vRotation;

		if (nullptr == m_pParent.lock())
			m_vWorldRotation = _vRotation;
		else
			m_vWorldRotation = _vRotation + m_pParent.lock()->GetRotation();
	}
	else
	{
		m_vWorldRotation = _vRotation;

		if (nullptr == m_pParent.lock())
			m_vLocalRotation = _vRotation;
		else
			m_vLocalRotation = m_vWorldRotation - m_pParent.lock()->GetRotation();
	}

	D3DXQuaternionRotationYawPitchRoll(&m_tLocalQuaternion, D3DXToRadian(m_vLocalRotation.y), D3DXToRadian(m_vLocalRotation.x), D3DXToRadian(m_vLocalRotation.z));
	D3DXQuaternionRotationYawPitchRoll(&m_tWorldQuaternion, D3DXToRadian(m_vWorldRotation.y), D3DXToRadian(m_vWorldRotation.x), D3DXToRadian(m_vWorldRotation.z));

	D3DXMatrixRotationQuaternion(&m_matRotation, &m_tLocalQuaternion);

	m_bUpdate = true;
}

D3DXVECTOR3 Transform::GetPosition(const SPACE _eSpace)
{
	if (SPACE::LOCAL == _eSpace)
		return m_vLocalPosition;
	else
		return m_vWorldPosition;
}

void Transform::SetPosition(const D3DXVECTOR3& _vPosition, const SPACE _eSpace)
{
	if (SPACE::LOCAL == _eSpace)
	{
		m_vLocalPosition = _vPosition;
		
		if (nullptr == m_pParent.lock())
			m_vWorldPosition = _vPosition;
		else
			m_vWorldPosition = _vPosition + m_pParent.lock()->GetPosition();
	}
	else
	{
		m_vWorldPosition = _vPosition;

		if (nullptr == m_pParent.lock())
			m_vLocalPosition = _vPosition;
		else
			m_vLocalPosition = m_vWorldPosition - m_pParent.lock()->GetPosition();
	}

	D3DXMatrixTranslation(&m_matTranslation, m_vLocalPosition.x, m_vLocalPosition.y, m_vLocalPosition.z);

	m_bUpdate = true;
}

D3DXVECTOR3 Transform::GetRight()
{
	D3DXVECTOR3 vRight(0.f, 0.f, 0.f);
	memcpy_s(&vRight, sizeof(D3DXVECTOR3), m_matWorld.m[0], sizeof(D3DXVECTOR3));
	return vRight;
}

D3DXVECTOR3 Transform::GetUp()
{
	D3DXVECTOR3 vUp(0.f, 0.f, 0.f);
	memcpy_s(&vUp, sizeof(D3DXVECTOR3), m_matWorld.m[1], sizeof(D3DXVECTOR3));
	return vUp;
}

D3DXVECTOR3 Transform::GetLook()
{
	D3DXVECTOR3 vLook(0.f, 0.f, 0.f);
	memcpy_s(&vLook, sizeof(D3DXVECTOR3), m_matWorld.m[2], sizeof(D3DXVECTOR3));
	return vLook;
}

D3DXMATRIX Transform::GetWorldMatrix()
{
	return FMath::WorldMatrix(m_vLocalScale, m_vLocalRotation, m_vLocalPosition);
}

void Transform::SetWorldMatirx(const D3DXMATRIX& _matWorld)
{
	m_matWorld = _matWorld;

	D3DXVECTOR3		vScale(0.f, 0.f, 0.f);
	D3DXQUATERNION	tQuaternion{};
	D3DXVECTOR3		vPosition(0.f, 0.f, 0.f);

	D3DXMatrixDecompose(&vScale, &tQuaternion, &vPosition, &m_matWorld);

	SetScale(vScale, SPACE::LOCAL);
	SetPosition(vPosition, SPACE::LOCAL);

	m_bUpdate = true;
}

std::weak_ptr<Transform> Transform::GetParent()
{
	return m_pParent;
}

void Transform::SetParent(std::weak_ptr<Transform> _pParent)
{
	if (_pParent.lock() == m_pParent.lock())
		return;

	m_pParent = _pParent;

	//새로 설정된 부모의 행렬에 맞게 Local 값 재설정.
	SetScale(m_vWorldScale);
	SetRotation(m_vWorldRotation);
	SetPosition(m_vWorldPosition);

	m_bUpdate = true;
}

std::string Transform::GetName()
{
	return "Transform";
}

void Transform::UpdateTransform()
{
	if (nullptr == m_pParent.lock())
		m_matWorld = m_matScale * m_matRotation * m_matTranslation;
	else
		m_matWorld = m_matScale * m_matRotation * m_matTranslation * m_pParent.lock()->GetWorldMatrix();
}
