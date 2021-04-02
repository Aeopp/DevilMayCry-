#ifndef _ANIMATIONINFORMATION_H_
#define _ANIMATIONINFORMATION_H_
#include "EngineDefine.h"
#include "EngineTypedef.h"
#include <string>

BEGIN(ENGINE)
class ENGINE_DLL AnimationInformation
{
	std::string Name{};
	double      Duration = 1.f;
	double      TickPerSecond = 30.f;
	double      TransitionTime = 0.25f;
	double      Acceleration = 1.f;
};
END
#endif // !_ANIMATIONINFORMATION_H_




