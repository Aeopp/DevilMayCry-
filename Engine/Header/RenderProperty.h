#ifndef __RENDERPROPERTY_H__
#define __RENDERPROPERTY_H__
#include "EngineTypedef.h"
#include "EngineDefine.h"
#include <map>
#include <functional>
#include <any>

BEGIN(ENGINE)

struct ENGINE_DLL RenderInfo
{
	IDirect3DDevice9* _Device{};
	ID3DXEffect* Fx{};
	uint32 PassIndex{ 0u };
	std::any BySituation{};
};

struct ENGINE_DLL ShadowInfo
{
	Matrix ViewProjection;
};

struct ENGINE_DLL RenderProperty
{
public : 
	enum class Order : uint8
	{
		GBuffer = 0u,

		ForwardAlphaBlend,

		Shadow,

		AlphaBlendEffect,
		UI,

		Debug,

		DebugBone,
	};
	bool bRender = true;
	using CallType = std::function<void(const RenderInfo&)>;
	std::map<Order,std::map<std::string, CallType>> RenderOrders{};
};  

END
#endif // !__RENDERPROPERTY_H__