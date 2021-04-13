#ifndef __RENDERPROPERTY_H__
#define __RENDERPROPERTY_H__
#include "EngineTypedef.h"
#include "EngineDefine.h"
#include <map>
#include <functional>

BEGIN(ENGINE)

struct ENGINE_DLL RenderInfo
{
	IDirect3DDevice9* _Device{};
	ID3DXEffect* Fx{};
	uint32 PassIndex{ 0u };
};

struct ENGINE_DLL RenderProperty
{
public : 
	enum class Order : uint8
	{
		GBuffer = 0u,
		GBufferSK,

		ForwardAlphaBlend,
		ForwardAlphaBlendSK,

		Shadow,
		ShadowSK,

		AlphaBlendEffect,
		UI,
		
		Debug,
		DebugSK,

		DebugBone,
	};
	bool bRender = true;
	using CallType = std::function<void(const RenderInfo&)>;
	std::map<Order,std::map<std::string, CallType>> RenderOrders{};
};  

END
#endif // !__RENDERPROPERTY_H__