#pragma once
#include "GameObject.h"
class Camera : public GameObject
{
private:
	explicit Camera() = default;
	virtual ~Camera() = default;

	virtual void Free() override;

public:
	static Camera* Create();
	virtual HRESULT Ready() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;


public:
	void	Move(const float& _fTimeDelta);
	void	Move_Mouse(const float& _fTimeDelta);
	void	Mouse_Fix();


private:
	Vector3	m_vEye, m_vAt, m_vUp;
	float	m_fFovY, m_fAspect, m_fNear, m_fFar;
	Matrix	m_matView, m_matProj;

	bool	m_bFix = false;
	bool	m_bClick = false;

	

};

