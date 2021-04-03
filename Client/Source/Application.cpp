#include "stdafx.h"
#include "Application.h"

#include "TestScene.h"
#include "MapTool.h"
Application::Application()
{
}

HRESULT Application::ReadyApplication(const bool bWindowed)
{
	if (FAILED(Engine::ReadyEngine(bWindowed)))
		return E_FAIL;

	//SceneManager::LoadScene(MapTool::Create());
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
