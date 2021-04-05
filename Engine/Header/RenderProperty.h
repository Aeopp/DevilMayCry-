#ifndef __RENDERPROPERTY_H__
#define __RENDERPROPERTY_H__
#include "EngineTypedef.h"
#include "EngineDefine.h"
BEGIN(ENGINE)
struct RenderProperty
{
public : 
	enum class Order : uint8
	{
		DefaultDeferred = 0u,
		ForwardAlphaBlend,
		AlphaBlendEffect,
		UI,
	};
	bool bRender = true;
	Order _Order{ Order::DefaultDeferred };
};

END
#endif // !__RENDERPROPERTY_H__