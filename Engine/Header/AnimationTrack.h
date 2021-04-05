#ifndef _ANIMATIONTRACK_H_
#define _ANIMATIONTRACK_H_
#include "EngineDefine.h"
#include "EngineTypedef.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <map>

BEGIN(ENGINE)
class ENGINE_DLL AnimationTrack
{
	std::vector       <std::unordered_map<std::string,
		std::map<double, Vector3>>>   ScaleTimeLine{};

	std::vector       <std::unordered_map<std::string,
		std::map<double, Quaternion>>>QuatTimeLine{};

	std::vector       <std::unordered_map<std::string,
		std::map<double, Vector3>>>   PosTimeLine{};
};
END

#endif // !_ANIMATIONTRACK_H_




