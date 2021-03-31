#include "stdafx.h"
#include "Application.h"

#include "TestScene.h"

Application::Application()
{
}

HRESULT Application::ReadyApplication()
{
	g_eWndMode = WNDMODE::WND_WINDOW;
	if (FAILED(Engine::ReadyEngine()))
		return E_FAIL;

	SceneManager::LoadScene(TestScene::Create());
	return S_OK;
}

HRESULT Application::UpdateApplication()
{
	if (FAILED(Engine::UpdateEngine()))
		return E_FAIL;
	return S_OK;
}

HRESULT Application::ReleaseApplication()
{
	if (FAILED(Engine::ReleaseEngine()))
		return E_FAIL;
	return S_OK;
}