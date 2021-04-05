#ifndef _NODE_H_
#define _NODE_H_
#include "EngineDefine.h"
#include "EngineTypedef.h"
#include "FMath.hpp"
#include "AnimationBlendInfo.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <map>
#include "AnimationTrack.h"

BEGIN(ENGINE)
class ENGINE_DLL Node
{
public:
	Node* Parent{};
	std::vector<Node*>    Childrens{};
	Matrix OriginTransform{ FMath::Identity() };
	Matrix Final{ FMath::Identity() };
	Matrix Transform{ FMath::Identity() };
	Matrix ToRoot{ FMath::Identity() };
	Matrix Offset{ FMath::Identity() };
	std::string Name{};
	int32 Index{ 0u };

	std::unordered_map<std::string,AnimationTrack> _AnimationTrack{};

	void NodeUpdate(
		const Matrix& ParentToRoot,
		const double CurrentAnimationTime,
		const std::string& AnimationName,
		const std::optional<AnimationBlendInfo>& IsAnimationBlend)&;

	void Editor()&;
};
END

#endif // !_NODE_H_




