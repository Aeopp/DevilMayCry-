#ifndef _ANIMATIONINFORMATION_H_
#define _ANIMATIONINFORMATION_H_
#include "EngineDefine.h"
#include "EngineTypedef.h"
#include <string>

BEGIN(ENGINE)
class ENGINE_DLL AnimationInformation
{
	std::string Name{};
<<<<<<< HEAD
	double      Duration = 1.f;
	double      TickPerSecond = 30.f;
	double      TransitionTime = 0.25f;
	double      Acceleration = 1.f;
=======
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
>>>>>>> a8ab4edb3eac470353904547d5f18b753d795ff0
};
END
#endif // !_ANIMATIONINFORMATION_H_




