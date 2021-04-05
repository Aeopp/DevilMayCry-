#include "stdafx.h"
#include "Camera.h"

void Camera::Free()
{
}

Camera* Camera::Create()
{
    return new Camera{};
}

HRESULT Camera::Ready()
{
    m_vEye = Vector3(0.f, 0.f, -5.f);
    m_vAt = Vector3(0.f, 0.f, 0.f);
    m_vUp = Vector3(0.f, 1.f, 0.f);

    m_fFovY = D3DXToRadian(60.f);
    m_fAspect = float(g_nWndCX) / g_nWndCY;
    m_fNear = 1.f;
    m_fFar = 1000.f;


    D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
    Graphic::GetDevice()->SetTransform(D3DTS_VIEW, &m_matView);

    D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
    Graphic::GetDevice()->SetTransform(D3DTS_PROJECTION, &m_matProj);

    return S_OK;
}

HRESULT Camera::Awake()
{
    return S_OK;
}

HRESULT Camera::Start()
{
    return S_OK;
}

UINT Camera::Update(const float _fDeltaTime)
{
    //////////////////////////////////////////////////
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	Graphic::GetDevice()->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	Graphic::GetDevice()->SetTransform(D3DTS_PROJECTION, &m_matProj);
    //////////////////////////////////////////////////
    if (true == m_bFix)
    {
        Mouse_Fix();
        Move_Mouse(_fDeltaTime);
    }
    Move(_fDeltaTime);

    return 0;
}

UINT Camera::LateUpdate(const float _fDeltaTime)
{
    return 0;
}

void Camera::OnEnable()
{
}

void Camera::OnDisable()
{
}

void Camera::Move(const float& _fTimeDelta)
{
    Matrix      matCameraWorld;
    D3DXMatrixInverse(&matCameraWorld, NULL, &m_matView);
        
    if (Input::GetKey(DIK_W))
    {
        Vector3 vLook;
        memcpy(&vLook, &matCameraWorld.m[2][0], sizeof(Vector3));

        Vector3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * 40.f * _fTimeDelta;

        m_vEye += vLength;
        m_vAt += vLength;
    }

    if (Input::GetKey(DIK_S))
    {
        Vector3 vLook;
        memcpy(&vLook, &matCameraWorld.m[2][0], sizeof(Vector3));

		Vector3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * 40.f * _fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
    }

    if (Input::GetKey(DIK_A))
    {
        Vector3 vRight;
        memcpy(&vRight, &matCameraWorld.m[0][0], sizeof(Vector3));

		Vector3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * 20.f * _fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
    }

	if (Input::GetKey(DIK_D))
	{
		Vector3 vRight;
		memcpy(&vRight, &matCameraWorld.m[0][0], sizeof(Vector3));

		Vector3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * 20.f * _fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

    if (Input::GetKeyDown(DIK_LSHIFT))
    {
        if (true == m_bClick)
            return;

        m_bClick = true;

        if (true == m_bFix)
            m_bFix = false;
        else
            m_bFix = true;
    }
    else
        m_bClick = false;

    if (false == m_bFix)
        return;
 }

void Camera::Move_Mouse(const float& _fTimeDelta)
{
    Matrix  matCameraWorld;
    D3DXMatrixInverse(&matCameraWorld, NULL, &m_matView);

    long    dwMouseMove = 0;

    if (dwMouseMove = Input::GetMouseMove(DIM_Y))
    {
        Vector3  vRight;
        memcpy(&vRight, &matCameraWorld.m[0][0], sizeof(Vector3));

        Vector3 vLook = m_vAt - m_vEye;

        Matrix  matRot;
        D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove) / 10.f);
        D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

        m_vAt = m_vEye + vLook;
    }

    if (dwMouseMove = Input::GetMouseMove(DIM_X))
    {
        Vector3 vUp = Vector3(0.f, 1.f, 0.f);
        Vector3 vLook = m_vAt - m_vEye;

        Matrix matRot;
        D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove) / 10.f);
        D3DXVec3TransformNormal(&vLook, &vLook, &matRot);


        m_vAt = m_vEye + vLook;
    }
 }

void Camera::Mouse_Fix()
{
    POINT   ptMouse{ g_nWndCX >> 1, g_nWndCY >> 1 };

    ClientToScreen(g_hWnd, &ptMouse);
    SetCursorPos(ptMouse.x, ptMouse.y);
}
