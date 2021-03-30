#include "..\Header\TimeSystem.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(TimeSystem)

TimeSystem::TimeSystem()
	: m_fDeltaTime(0.f)
{
	ZeroMemory(&m_tCPUTick, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_tStartFrame, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_tEndFrame, sizeof(LARGE_INTEGER));
}

void TimeSystem::Free()
{
}

HRESULT TimeSystem::ReadyTimeSystem()
{
	QueryPerformanceCounter(&m_tStartFrame);
	QueryPerformanceCounter(&m_tEndFrame);
	QueryPerformanceFrequency(&m_tCPUTick);
	return S_OK;
}

HRESULT TimeSystem::UpdateTimeSystem()
{
	UpdateDeltaTime();
	return S_OK;
}

void TimeSystem::UpdateDeltaTime()
{
	QueryPerformanceFrequency(&m_tCPUTick);
	QueryPerformanceCounter(&m_tEndFrame);
	m_fDeltaTime = float(m_tEndFrame.QuadPart - m_tStartFrame.QuadPart) / m_tCPUTick.QuadPart;
	m_tStartFrame = m_tEndFrame;
}

float TimeSystem::DeltaTime()
{
	return m_fDeltaTime;
}

