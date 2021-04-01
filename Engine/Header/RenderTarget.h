#ifndef _RENDERTARGET_H_
#define _RENDERTARGET_H_
#include <vector>
#include "EngineDefine.h"
#include "EngineTypedef.h"
BEGIN(ENGINE)
class ENGINE_DLL RenderTarget
{
public:
	struct Info
	{
		uint32 Width;
		uint32 Height;
		uint32 Levels;
		long Usages;
		D3DPOOL _D3DPool;
		D3DFORMAT Format;
	};
	void Initialize(const Info& InitInfo)&;
	void Release()&;

	IDirect3DTexture9* GetTexture()const& { return TargetTexture;  };
	IDirect3DSurface9* GetSurface(const size_t Index)const&;
	void InsertSurface(const UINT Level)&;
	Info GetInfo()& { return _RenderTargetInfo;  };
private:
	std::vector<IDirect3DSurface9*> Surfaces{};
	IDirect3DTexture9* TargetTexture{ nullptr };
	Info _RenderTargetInfo{};
};
END

#endif // !_RENDERTARGET_H_

