#include "Core.h"
#include "CoreSystem.h"

USING(ENGINE)

ENGINE_DLL HINSTANCE			g_hInstance;
ENGINE_DLL HWND					g_hWnd;
ENGINE_DLL int					g_nWndCX;
ENGINE_DLL int					g_nWndCY;
ENGINE_DLL WNDMODE				g_eWndMode;

std::weak_ptr<CoreSystem> Engine::m_pCoreSystem = CoreSystem::GetInstance();

HRESULT Engine::ReadyEngine()
{
	if (nullptr == m_pCoreSystem.lock() || m_pCoreSystem.expired())
		return E_FAIL;
	return m_pCoreSystem.lock()->ReadyEngine();
}

HRESULT Engine::UpdateEngine()
{
	if (nullptr == m_pCoreSystem.lock() || m_pCoreSystem.expired())
		return E_FAIL;
	return m_pCoreSystem.lock()->UpdateEngine();
}

HRESULT Engine::ReleaseEngine()
{
	CoreSystem::DeleteInstance();
	return S_OK;
}
