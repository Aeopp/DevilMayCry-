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
		GBufferSK,

		ForwardAlphaBlend,
		ForwardAlphaBlendSK,

		AlphaBlendEffect,
		UI,
		
		Debug,
		DebugSK,

		DebugBone,
	};
	bool bRender = true;
	std::set<Order> RenderOrders{};
};

END
#endif // !__RENDERPROPERTY_H__