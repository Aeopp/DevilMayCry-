#include "..\Header\TimeSystem.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(TimeSystem)

TimeSystem::TimeSystem()
	: m_fDeltaTime(0.f) ,  
	m_fAccDeltaTime  (0.0) , 
	m_uiFrameRate(0u),
	m_uiUpdateCount(0u)
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

void TimeSystem::Editor()
{
	ImGui::Text("Frame Rate : %d ", m_uiFrameRate);
	ImGui::Text("Delta Time : %2.6f ", m_fDeltaTime);
	ImGui::Text("Accumulate Time : %10.1f ", m_fAccTime);
}

void TimeSystem::UpdateDeltaTime()
{
	QueryPerformanceFrequency(&m_tCPUTick);
	QueryPerformanceCounter(&m_tEndFrame);
	m_fDeltaTime = float(m_tEndFrame.QuadPart - m_tStartFrame.QuadPart) / m_tCPUTick.QuadPart;
	m_fAccDeltaTime += m_fDeltaTime;
	m_fAccTime += m_fDeltaTime;
	++m_uiUpdateCount;
	if (m_fAccDeltaTime > 1.0)
	{
		m_fAccDeltaTime -= 1.0;
		m_uiFrameRate = m_uiUpdateCount;
		m_uiUpdateCount = 0u;
	}
	m_tStartFrame = m_tEndFrame;
}

float TimeSystem::AccTime()
{
	return m_fAccTime;
}

float TimeSystem::DeltaTime()
{
	return m_fDeltaTime;
}

