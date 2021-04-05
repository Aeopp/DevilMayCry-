#ifndef _BONE_H_
#define _BONE_H_
#include "EngineDefine.h"
#include "EngineTypedef.h"
#include "FMath.hpp"
#include "AnimationBlendInfo.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <map>

BEGIN(ENGINE)
class ENGINE_DLL Bone
{
public:
	Bone* Parent{};
	std::vector<Bone*>    Childrens{};
	Matrix Final{ FMath::Identity() };
	Matrix Transform{ FMath::Identity() };
	Matrix OriginTransform{ FMath::Identity() };
	Matrix ToRoot{ FMath::Identity() };
	Matrix Offset{ FMath::Identity() };
	std::string Name{};

	void BoneMatrixUpdate(const Matrix& ParentToRoot,
		const double CurrentAnimationTime,

		const std::unordered_map<std::string,
		std::map<double, Vector3>>&ScaleTrack,

		const std::unordered_map<std::string,
		std::map<double, Quaternion>>&QuatTrack,

		const std::unordered_map<std::string,
		std::map<double, Vector3>>&PosTrack,

		const std::optional<AnimationBlendInfo>& IsAnimationBlend)&;
};
END

#endif // !_BONE_H_




