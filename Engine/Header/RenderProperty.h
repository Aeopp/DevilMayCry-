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
		NoAlphaBlend = 0u,
		AlphaBlend,
		AlphaBlendEffect,
		UI,
	};
	bool bRender = true;
	Order _Order{ Order::NoAlphaBlend };
};

END
#endif // !__RENDERPROPERTY_H__