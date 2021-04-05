#ifndef _ANIMATIONINFORMATION_H_
#define _ANIMATIONINFORMATION_H_
#include "EngineDefine.h"
#include "EngineTypedef.h"
#include <string>
#include "AnimationTrack.h"

BEGIN(ENGINE)
struct ENGINE_DLL AnimationInformation
{
	std::string Name{};
	float       Duration = 1.f;
	float       TickPerSecond = 30.f;
	float       TransitionTime = 0.25f;
	void       SetAcceleration(const float NewAcceleration)
	{
		Acceleration = NewAcceleration;
	}
	float     CalcAcceleration() const 
	{
		return Acceleration * TickPerSecond;
	}
	float& RefOriginAcceleration()
	{
		return Acceleration;
	}
	float GetOriginAcceleration()const
	{
		return Acceleration;
	}
private:
	float      Acceleration = 1.f;
};
END
#endif // !_ANIMATIONINFORMATION_H_




