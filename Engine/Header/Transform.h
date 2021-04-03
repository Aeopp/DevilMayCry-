#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__
#include "Component.h"
BEGIN(ENGINE)
class ENGINE_DLL Transform final : public Component
{
public:
	enum class SPACE { LOCAL, WORLD };
private:
	D3DXVECTOR3		m_vLocalScale;		//부모에 상대적인 크기 값
	D3DXVECTOR3		m_vWorldScale;		//월드 상의 크기 값 == LocalScale + 부모의 WorldScale

	D3DXVECTOR3		m_vLocalRotation;	//부모에 상대적인 회전 값
	D3DXVECTOR3		m_vWorldRotation;
	D3DXQUATERNION	m_tLocalQuaternion;	//Yaw : y , Pitch : x , Roll : z
	D3DXQUATERNION	m_tWorldQuaternion;

	D3DXVECTOR3		m_vLocalPosition;	//부모에 상대적인 위치 값
	D3DXVECTOR3		m_vWorldPosition;

	D3DXMATRIX		m_matScale;
	D3DXMATRIX		m_matRotation;
	D3DXMATRIX		m_matTranslation;
	D3DXMATRIX		m_matWorld;

	std::weak_ptr<Transform>				m_pParent;
	//std::list<std::weak_ptr<Transform>>		m_vecChild;

	bool			m_bUpdate;
private:
	explicit Transform(std::weak_ptr<GameObject> const _pGameObject);
	virtual ~Transform() = default;
	// Component을(를) 통해 상속됨
	virtual void Free() override;
public:
	static Transform* Create(std::weak_ptr<GameObject> const _pGameObject);
public:
	D3DXVECTOR3		GetScale(const SPACE _eSpace = SPACE::WORLD);
	void			SetScale(const D3DXVECTOR3& _vScale, const SPACE _eSpace = SPACE::WORLD);

	D3DXVECTOR3		GetRotation(const SPACE _eSpace = SPACE::WORLD);
	void			SetRotation(const D3DXVECTOR3& _vRotation, const SPACE _eSpace = SPACE::WORLD);

	D3DXVECTOR3		GetPosition(const SPACE _eSpace = SPACE::WORLD);
	void			SetPosition(const D3DXVECTOR3& _vPosition, const SPACE _eSpace = SPACE::WORLD);

	D3DXVECTOR3		GetRight();
	D3DXVECTOR3		GetUp();
	D3DXVECTOR3		GetLook();

	D3DXMATRIX		GetWorldMatrix();
	void			SetWorldMatirx(const D3DXMATRIX& _matWorld);

	std::weak_ptr<Transform>	GetParent();
	void						SetParent(std::weak_ptr<Transform> _pParent);

	virtual std::string GetName()override;
private:
	void	UpdateTransform();
};
END
#endif // !__TRANSFORM_H__