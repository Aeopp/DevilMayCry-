#include "GraphicSystem.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(GraphicSystem)

GraphicSystem::GraphicSystem()
	: m_pSDK(nullptr)
	, m_pDevice(nullptr)
{
}

void GraphicSystem::Free()
{
	SafeRelease(m_pDevice);
	SafeRelease(m_pSDK);
}

HRESULT GraphicSystem::ReadyGraphicSystem()
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	if (nullptr == m_pSDK)
	{
		PRINT_LOG(TEXT("Warning"), TEXT("Failed to Direct3DCreate9"));
		return E_FAIL;
	}

	D3DCAPS9 tD3DCaps;
	ZeroMemory(&tD3DCaps, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &tD3DCaps)))
	{
		PRINT_LOG(TEXT("Warning"), TEXT("Failed to GetDeviceCaps"));
		SafeRelease(m_pSDK);
		return E_FAIL;
	}

	DWORD dwVertexProcessing = 0;
	if ((D3DDEVCAPS_HWTRANSFORMANDLIGHT & tD3DCaps.DevCaps))
		dwVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		dwVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	dwVertexProcessing |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS tD3DPP;
	ZeroMemory(&tD3DPP, sizeof(D3DPRESENT_PARAMETERS));

	tD3DPP.BackBufferWidth = g_nWndCX;
	tD3DPP.BackBufferHeight = g_nWndCY;
	tD3DPP.BackBufferFormat = D3DFMT_A8R8G8B8;
	tD3DPP.BackBufferCount = 1;

	tD3DPP.MultiSampleType = D3DMULTISAMPLE_NONE;
	tD3DPP.MultiSampleQuality = 0;

	tD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	tD3DPP.hDeviceWindow = g_hWnd;

	tD3DPP.Windowed = g_eWndMode;
	tD3DPP.EnableAutoDepthStencil = TRUE;
	tD3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;

	tD3DPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	tD3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, dwVertexProcessing, &tD3DPP, &m_pDevice)))
	{
		PRINT_LOG(TEXT("Warning"), TEXT("Failed to CreateDevice"));
		SafeRelease(m_pSDK);
		return E_FAIL;
	}
	return S_OK;
}

LPDIRECT3DDEVICE9 GraphicSystem::GetDevice()
{
	return m_pDevice;
}
