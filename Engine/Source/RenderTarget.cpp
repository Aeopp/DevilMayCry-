#include "RenderTarget.h"
#include "GraphicSystem.h"

USING(ENGINE)

void RenderTarget::Release ()&
{
	if (TargetTexture)
		TargetTexture->Release();

	for (auto& DeleteSurface : Surfaces)
	{
		if (DeleteSurface)
		{
			DeleteSurface->Release();
		}
	}
}
void RenderTarget::Initialize(const Info& InitInfo)&
{
	auto _Device = GraphicSystem::GetInstance()->GetDevice();
	
	if (FAILED(_Device->CreateTexture(
		InitInfo.Width, 
		InitInfo.Height, 
		InitInfo.Levels, 
		InitInfo.Usages, 
		InitInfo.Format,
		InitInfo._D3DPool, &TargetTexture, NULL)))
	{
		PRINT_LOG(L"Failed!", L"Failed Render Target Texture !");
	}
	else
	{
		_RenderTargetInfo = InitInfo;
		Surfaces.resize(1);
		if (FAILED(TargetTexture->GetSurfaceLevel(0, &(Surfaces[0]))))
		{
			PRINT_LOG(L"Failed!", L"Failed Render Target Surfaces[0]!");
		}
	}
}
IDirect3DSurface9* RenderTarget::GetSurface(const uint64 Index)const&
{
	return Surfaces[Index];
}
void RenderTarget::InsertSurface(const UINT Level)&
{
	IDirect3DSurface9* InsertSurface{ nullptr };

	if (FAILED(TargetTexture->GetSurfaceLevel(Level,  &InsertSurface)))
	{
		PRINT_LOG(L"Failed!", L"Failed Render Target Surface !");
	}
	else
	{
		Surfaces.push_back(InsertSurface);
	}
}
