#ifndef __TIME_SYSTEM_H__
#define __TIME_SYSTEM_H__
#include "Object.h"
BEGIN(ENGINE)
class TimeSystem final : public Object
{
	DECLARE_SINGLETON(TimeSystem)
private:
	float			m_fDeltaTime;
	LARGE_INTEGER	m_tCPUTick;
	LARGE_INTEGER	m_tStartFrame;
	LARGE_INTEGER	m_tEndFrame;
private:
	explicit TimeSystem();
	virtual ~TimeSystem() = default;
	// Object을(를) 통해 상속됨
	virtual void Free() override;
public:
	HRESULT ReadyTimeSystem();
	HRESULT UpdateTimeSystem();
private:
	void	UpdateDeltaTime();
public:
	float	DeltaTime();
};
END
#endif // !__TIME_SYSTEM_H__
