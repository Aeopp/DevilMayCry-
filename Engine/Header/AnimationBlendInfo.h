#ifndef _ANIMATIONBLENDINFO_H_
#define _ANIMATIONBLENDINFO_H_
#include "EngineDefine.h"
#include "EngineTypedef.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <map>

BEGIN(ENGINE)
class ENGINE_DLL AnimationBlendInfo
{
public:
	uint32 PrevAnimationIdx{ 0u };

	const double PrevAnimationWeight = 0.0;
	const double AnimationTime{ 0.0 };

	const std::unordered_map<std::string,
		std::map<double, Vector3>>&ScaleTrack;

	const std::unordered_map<std::string,
		std::map<double, Quaternion>>&QuatTrack;

	const std::unordered_map<std::string, std::map<double, Vector3>>& PosTrack;
};
END

#endif // !ANIMATIONBLENDINFO




