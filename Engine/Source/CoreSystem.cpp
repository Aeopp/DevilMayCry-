#include "CoreSystem.h"
#include "GraphicSystem.h"
#include "InputSystem.h"
#include "TimeSystem.h"
#include "SceneSystem.h"
#include "ResourceSystem.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(CoreSystem)

CoreSystem::CoreSystem()
{
}

void CoreSystem::Free()
{
	m_pSceneSystem.reset();
	SceneSystem::DeleteInstance();

	m_pResourceSystem.reset();
	ResourceSystem::DeleteInstance();

	m_pGraphicSystem.reset();
	GraphicSystem::DeleteInstance();

	m_pInputSystem.reset();
	InputSystem::DeleteInstance();

	m_pTimeSystem.reset();
	TimeSystem::DeleteInstance();
}

HRESULT CoreSystem::ReadyEngine()
{
	m_pGraphicSystem = GraphicSystem::GetInstance();
	if (nullptr == m_pGraphicSystem.lock() || FAILED(m_pGraphicSystem.lock()->ReadyGraphicSystem()))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to ReadyEngine."));
		return E_FAIL;
	}

	m_pInputSystem = InputSystem::GetInstance();
	if (nullptr == m_pInputSystem.lock() || FAILED(m_pInputSystem.lock()->ReadyInputSystem()))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to ReadyEngine."));
		return E_FAIL;
	}

	m_pTimeSystem = TimeSystem::GetInstance();
	if (nullptr == m_pTimeSystem.lock() || FAILED(m_pTimeSystem.lock()->ReadyTimeSystem()))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to ReadyEngine."));
		return E_FAIL;
	}

	m_pSceneSystem = SceneSystem::GetInstance();
	if (nullptr == m_pSceneSystem.lock() || FAILED(m_pSceneSystem.lock()->ReadySceneSystem()))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to ReadyEngine."));
		return E_FAIL;
	}

	m_pResourceSystem = ResourceSystem::GetInstance();
	if (nullptr == m_pResourceSystem.lock() || FAILED(m_pResourceSystem.lock()->ReadyResourceSystem(m_pGraphicSystem.lock()->GetDevice())))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to ReadyEngine."));
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CoreSystem::UpdateEngine()
{
	if (FAILED(m_pInputSystem.lock()->UpdateInputSystem()))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to UpdateInputSystem."));
		return E_FAIL;
	}
	if (FAILED(m_pTimeSystem.lock()->UpdateTimeSystem()))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to UpdateTimeSystem."));
		return E_FAIL;
	}
	if (FAILED(m_pSceneSystem.lock()->UpdateSceneSystem(m_pTimeSystem.lock()->DeltaTime())))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to UpdateSceneSystem."));
		return E_FAIL;
	}
	if (FAILED(m_pSceneSystem.lock()->LateUpdateSceneSystem(m_pTimeSystem.lock()->DeltaTime())))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to LateUpdateSceneSystem."));
		return E_FAIL;
	}

	return S_OK;
}
