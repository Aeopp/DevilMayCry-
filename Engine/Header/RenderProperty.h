#ifndef __RENDERPROPERTY_H__
#define __RENDERPROPERTY_H__
#include "EngineTypedef.h"
#include "EngineDefine.h"
#include <set>

BEGIN(ENGINE)
struct RenderProperty
{
public : 
	enum class Order : uint8
	{
		GBuffer = 0u,
		ForwardAlphaBlend,
		AlphaBlendEffect,
		UI,
		
		Debug,
		DebugBone,
	};
	bool bRender = true;
	std::set<Order> RenderOrders{};
};

END
#endif // !__RENDERPROPERTY_H__