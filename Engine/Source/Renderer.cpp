#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "Renderer.h"
#include "GraphicSystem.h"
#include "FMath.hpp"
#include "Color.h"
#include "DxHelper.h"
#include "Resources.h"
#include "TimeSystem.h"
#include "Vertexs.h"
#include <filesystem>
#include <d3d9.h>
#include <d3dx9.h>

USING(ENGINE)
IMPLEMENT_SINGLETON(Renderer)

Renderer::Renderer(){}

void Renderer::Free()
{
	TestShaderRelease();
};


HRESULT Renderer::ReadyRenderSystem(LPDIRECT3DDEVICE9 const _pDevice)
{
	Device = _pDevice;
	SafeAddRef(Device);
	ReadyRenderTargets();
	ReadyShader("..\\..\\Resource\\Shader");
	ReadyLights();
	ReadyFrustum();
	ReadyQuad();

	TestShaderInit();
	return S_OK;
};

void Renderer::ReadyShader(const std::filesystem::path& TargetPath)
{
	std::filesystem::directory_iterator itr(TargetPath);

	for (const std::filesystem::directory_entry& entry :
		std::filesystem::recursive_directory_iterator(TargetPath))
	{
		const auto& CurPath = entry.path();
		if (CurPath.has_extension())
		{
			const std::string& ShaderKey = CurPath.stem().string();
			Shaders[ShaderKey] = Resources::Load<Shader>(CurPath);
		}
	}

}

void Renderer::ReadyLights()
{
	// ?޺?
	DirLights.resize(1u);
	DirLights[0] = std::make_shared<FLight>
		(FLight(FLight::Type::Directional,
		{ 0,0,0,0 }, (const D3DXCOLOR&)Color::sRGBToLinear(250, 250, 250)));
	DirLights[0]->CreateShadowMap(Device, 2048);
	DirLights[0]->GetPosition().x = -9.f;
	DirLights[0]->GetPosition().y = 105.f;
	DirLights[0]->GetPosition().z = -22.f;
	DirLights[0]->Direction.x = 71.f;
	DirLights[0]->Direction.y = -2.f;
	DirLights[0]->Direction.z = -83.f;

	DirLights[0]->SetProjectionParameters(60.f,60.f,-1.f,300.f);
	DirLights[0]->lightFlux = 25.f;
	DirLights[0]->lightIlluminance = 1.0409f;

	// PointLights.resize(1u);



	//PointLights[0] = std::make_shared<FLight>(
	//	FLight(
	//		FLight::Type::Point, { 1.5f,0.5f, 0.0f ,1 },
	//		{ 1,0,0,1 }));
	//PointLights[0]->SetPointRadius(7.1f);

	//PointLights[1] = std::make_shared<FLight>(
	//	FLight(
	//		FLight::Type::Point, { -0.7f , 0.5f , 1.2f , 1.f },
	//		{ 0,1,0,1 }));
	//PointLights[1]->SetPointRadius(7.1f);

	//PointLights[2] = std::make_shared<FLight>(
	//	FLight(
	//		FLight::Type::Point,
	//		{ 0.0f,0.5f,0.0f,1 },
	//		{ 0,0,1,1 }));
	//PointLights[2]->SetPointRadius(7.1f);

	// ?׸??ڸ? 512 ?? ????
	// DirLights[0]->CreateShadowMap(Device, 1024);
	// DirLights[0]->SetProjectionParameters(7.1f, 7.1f, -20.f, +20.f);

	// PointLights[0]->CreateShadowMap(Device, 512);
	/*PointLights[1]->CreateShadowMap(Device, 512);
	PointLights[2]->CreateShadowMap(Device, 512);*/
	
	// PointLights[0]->SetProjectionParameters(0, 0, 0.1f, 10.0f);
	/*PointLights[1]->SetProjectionParameters(0, 0, 0.1f, 10.0f);
	PointLights[2]->SetProjectionParameters(0, 0, 0.1f, 10.0f);*/
}

void Renderer::ReadyRenderTargets()
{
	static const Vector2 RenderTargetDebugRenderSize{ 80.f,80.f };

	 const  float InitX =
		g_nWndCX / -2.f + (RenderTargetDebugRenderSize.x);
	 const  float InitY =
		 g_nWndCY / +2.f - (RenderTargetDebugRenderSize.y);

	 const float YOffset = -RenderTargetDebugRenderSize.y *2.f ;
	 const float XOffset = RenderTargetDebugRenderSize.x * 2.f;
	 const float Interval = 5.f;

	{
		auto& SceneTarget = RenderTargets["SceneTarget"] = 
			std::make_shared<RenderTarget>();

		RenderTarget::Info InitInfo;
		InitInfo.Width = g_nWndCX;
		InitInfo.Height = g_nWndCY ;
		InitInfo.Levels = 1;
		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
		InitInfo.Format = D3DFMT_A16B16G16R16F;
		InitInfo._D3DPool = D3DPOOL_DEFAULT;
		SceneTarget->Initialize(InitInfo);
		SceneTarget->DebugBufferInitialize(
			{ InitX,InitY },
			RenderTargetDebugRenderSize);
	}

	{
		auto& ALBM = RenderTargets["ALBM"] = std::make_shared<RenderTarget>();

		RenderTarget::Info InitInfo;
		InitInfo.Width = g_nWndCX ;
		InitInfo.Height = g_nWndCY ;
		InitInfo.Levels = 1;
		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
		InitInfo.Format = D3DFMT_A8R8G8B8;
		InitInfo._D3DPool = D3DPOOL_DEFAULT;
		ALBM->Initialize(InitInfo);
		ALBM->DebugBufferInitialize(
			{ InitX,InitY + (YOffset  *1.f ) + Interval },
			RenderTargetDebugRenderSize);
	}

	{
		auto& NRMR = RenderTargets["NRMR"] = std::make_shared<RenderTarget>();


		RenderTarget::Info InitInfo;
		InitInfo.Width = g_nWndCX; 
		InitInfo.Height = g_nWndCY;
		InitInfo.Levels = 1;
		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
		InitInfo.Format = D3DFMT_A8R8G8B8;
		InitInfo._D3DPool = D3DPOOL_DEFAULT;
		NRMR->Initialize(InitInfo);
		NRMR->DebugBufferInitialize(
			{ InitX,InitY + (YOffset * 2.f) + Interval },
			RenderTargetDebugRenderSize);
	}

	{
		auto& Depth = RenderTargets["Depth"] = std::make_shared<RenderTarget>();

		RenderTarget::Info InitInfo;
		InitInfo.Width = g_nWndCX; 
		InitInfo.Height = g_nWndCY;
		InitInfo.Levels = 1;
		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
		InitInfo.Format = D3DFMT_R32F;
		InitInfo._D3DPool = D3DPOOL_DEFAULT;
		Depth->Initialize(InitInfo);
		Depth->DebugBufferInitialize(
			{ InitX,InitY + (YOffset * 3.f) + Interval },
			RenderTargetDebugRenderSize);
	}

}

void Renderer::ReadyRenderInfo()
{
	Matrix CameraView, CameraProjection, Ortho;
	Device->GetTransform(D3DTS_VIEW, &CameraView);
	Device->GetTransform(D3DTS_PROJECTION, &CameraProjection);

	D3DXMatrixOrthoLH(&Ortho, g_nWndCX, g_nWndCY, 0.0f, 1.f);

	_RenderInfo.View = (CameraView);
	_RenderInfo.Projection = (CameraProjection);
	_RenderInfo.ViewInverse = FMath::Inverse(_RenderInfo.View);
	_RenderInfo.ProjectionInverse = FMath::Inverse(_RenderInfo.Projection);
	_RenderInfo.ViewProjection =
		CameraView * CameraProjection;
	_RenderInfo.ViewProjectionInverse = FMath::Inverse(_RenderInfo.ViewProjection);
	_RenderInfo.Eye =
	{ _RenderInfo.ViewInverse._41  , _RenderInfo.ViewInverse._42,_RenderInfo.ViewInverse._43,1.f };
	_RenderInfo.Ortho = Ortho;

	CameraFrustum->Make(_RenderInfo.ViewInverse, _RenderInfo.Projection);
	Device->GetViewport(&_RenderInfo.Viewport);
}

void Renderer::ReadyFrustum()
{
	CameraFrustum = std::make_shared<Frustum>();
	CameraFrustum->Initialize(Device);
	CurShadowFrustum = std::make_shared<Frustum>();
	CurShadowFrustum->Initialize(Device);
}

void Renderer::ReadyQuad()
{
	_Quad = std::make_shared<Quad>();
	_Quad->Initialize(Device);
}

void Renderer::Push(const std::weak_ptr<GameObject>& _RenderEntity)&
{
	if (auto _SharedObject = _RenderEntity.lock();
		_SharedObject)
	{
		if (_SharedObject->IsRenderEnable())
		{
			if (
				std::shared_ptr<RenderInterface> _SharedRenderEntity 
						= std::dynamic_pointer_cast<RenderInterface>(_SharedObject);
				_SharedRenderEntity)
			{
				if (false == _SharedRenderEntity->GetRenderProp().bRender)return;

				const auto& _EntityRenderProp = _SharedRenderEntity->GetRenderProp();

				for (const auto& [_EntityOrder,ShaderKeyCallMap] : _EntityRenderProp.RenderOrders)
				{
					for (const auto& [ShaderKey,Call]: ShaderKeyCallMap)
					{
						RenderEntitys[_EntityOrder][ShaderKey].push_back(
								RenderEntityType{ _SharedRenderEntity.get(), Call });
					}
				}
			}
		}
	}
};

// ???? ???𿡼? ????ü ?غ? ..
HRESULT Renderer::Render()&
{
	RenderReady();
	RenderBegin();
	//  ?????? ?н? 
	RenderShadowMaps();
	// ???? ?н?
	RenderGBuffer();
	// ???۵? ?????? .
	DeferredShading();
	// ?????۷? ???? . 
	Device->SetRenderTarget(0, BackBuffer);
	Device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	RenderSky();
	Tonemapping();
	AlphaBlendEffectRender();
	UIRender();


	ResetState();
	RenderTargetDebugRender();
	RenderDebug();
	RenderDebugBone();
	ImguiRender();
	GraphicSystem::GetInstance()->End();
	RenderEnd();
	Device->Present(NULL, NULL, NULL, NULL);
	BackBuffer->Release();

	return S_OK;
}

void Renderer::Editor()&
{
	ImGui::Begin("Render Editor");
	{
		if (ImGui::CollapsingHeader("Add Light"))
		{
			static int32 ShadowMapSize = 0.0f;
			if (ImGui::InputInt("ShadowMapSize", &ShadowMapSize))
			{

			}

			if (ImGui::Button("Directional"))
			{
				auto _Insert = std::make_shared<FLight>(
					FLight(FLight::Type::Directional,
						{ 0,0,0,0 }, (const D3DXCOLOR&)Color::sRGBToLinear(250, 250, 250))
					);
				DirLights.push_back(_Insert);
				_Insert->CreateShadowMap(Device, ShadowMapSize);
				_Insert->SetProjectionParameters(7.1f, 7.1f, -20.f, +20.f);
			}

			if (ImGui::Button("Point"))
			{
				auto _Insert = std::make_shared<FLight>(
					FLight(
						FLight::Type::Point, { 1.5f,0.5f, 0.0f ,1 },
						{ 1,1,1,1 }));

				PointLights.push_back(_Insert);
				_Insert->CreateShadowMap(Device, ShadowMapSize);
				_Insert->SetProjectionParameters(0, 0, 0.1f, 10.0f);
			};
		};

		if (ImGui::CollapsingHeader("Remove"))
		{
			static int32 RemoveIndex = 0u;
			if (ImGui::InputInt("Index : %d", &RemoveIndex))
			{
				if (DirLights.empty() == false)
				{
					auto iter = DirLights.begin();
					std::advance(iter, RemoveIndex);
					if (iter != DirLights.end())
					{
						DirLights.erase(iter);
					}
				}

				if (PointLights.empty() == false)
				{
					auto iter = PointLights.begin();
					std::advance(iter, RemoveIndex);
					if (iter != PointLights.end())
					{
						PointLights.erase(iter);
					}
				}
			}
		}
		

		if (ImGui::CollapsingHeader("Lights"))
		{
			uint32 Idx = 0u;
			for (auto& _Light : PointLights)
			{
				_Light->Edit(Idx);
				++Idx;
			}
			for (auto& _Light : DirLights)
			{
				_Light->Edit(Idx);
				++Idx;
			}
		}
	}
	ImGui::End();
};

void Renderer::RenderReady()&
{
	RenderReadyEntitys();
	ReadyRenderInfo();
	// TestLightRotation();
};

void Renderer::RenderBegin()&
{
	GraphicSystem::GetInstance()->Begin();
	Device->GetRenderTarget(0, &BackBuffer);
}
// ?????ڵ????? 
void Renderer::RenderReadyEntitys()&
{
	for (auto& [_Order,RenderEntitys] : RenderEntitys)
	{
		for (auto& [ShaderKey,RenderEntityArr] : RenderEntitys)
		{
			for (auto& RenderEntity : RenderEntityArr)
			{
				auto& _RenderInterface = RenderEntity.first;
				_RenderInterface->RenderReady();
			}
		}
	}
}

void Renderer::ResetState()&
{
	Device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	Device->SetViewport(&_RenderInfo.Viewport);
};

void Renderer::RenderEnd()&
{
	_PrevRenderInfo = _RenderInfo;
	RenderEntityClear();
}

void Renderer::RenderEntityClear()&
{
	RenderEntitys.clear();
};

void Renderer::RenderShadowMaps()
{
	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	auto shadowmap = Shaders["Shadow"]->GetEffect();
	auto Blur = Shaders["Blur"]->GetEffect();

	for (auto& DirLight : DirLights)
	{
		DirLight->RenderShadowMap(Device, [&](FLight* light) {
			D3DXMATRIX  viewproj;
			D3DXVECTOR4 clipplanes(light->GetNearPlane(), light->GetFarPlane(), 0, 0);

			light->CalculateViewProjection(viewproj);

			shadowmap->SetTechnique("variance");
			shadowmap->SetVector("clipPlanes", &clipplanes);
			shadowmap->SetBool("isPerspective", FALSE);

			Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			RenderScene(shadowmap, viewproj);
			
			// ???? ???? ... 
			DrawInfo _DrawInfo{};
			_DrawInfo._Device = Device;
			CurShadowFrustum->Make(light->viewinv, light->proj); 
			_DrawInfo._Frustum = CurShadowFrustum.get();
			_DrawInfo.BySituation.reset();
			for (auto& [ShaderKey, EntityArr] : RenderEntitys[RenderProperty::Order::Shadow])
			{
				auto Fx = Shaders[ShaderKey]->GetEffect();
				Fx->SetMatrix("matViewProj", &viewproj);
				_DrawInfo.Fx = Fx;
				UINT Passes = 0u;
				Fx->Begin(&Passes, NULL);
				for (int32 i = 0; i < Passes; ++i)
				{
					_DrawInfo.PassIndex = i;
					Fx->BeginPass(i);
					for (auto& [_Entity, _Call] : EntityArr)
					{
						_Call(_DrawInfo);
					}
					Fx->EndPass();
				}
				Fx->End();
			}
			// ???? ???? ... 

			});
	};

	for (auto& DirLight : DirLights)
	{
		DirLight->BlurShadowMap(Device, [&](FLight* light) {
			D3DXVECTOR4 pixelsize(1.0f / light->GetShadowMapSize(),
				1.0f / light->GetShadowMapSize(), 0, 0);
			D3DXVECTOR4 TexelSize = DirLight->BlurIntencity * pixelsize;
			// make it more blurry
			Device->SetRenderState(D3DRS_ZENABLE, FALSE);
			Blur->SetTechnique("boxblur3x3");
			Blur->SetVector("pixelSize", &pixelsize);
			Blur->SetVector("texelSize", &TexelSize);

			Blur->Begin(NULL, 0);
			Blur->BeginPass(0);
			{
				_Quad->Render(Device);
			}
			Blur->EndPass();
			Blur->End();

			Device->SetRenderState(D3DRS_ZENABLE, TRUE);
		});
	}

	// point lights
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	shadowmap->SetBool("isPerspective", TRUE);

	for (auto& PointLight : PointLights)
	{
		Sphere PtlightSphere{};
		PtlightSphere.Center = (D3DXVECTOR3&)PointLight->GetPosition();
		PtlightSphere.Radius = PointLight->GetFarPlane();
		if (false == CameraFrustum->IsIn(PtlightSphere))continue;

		PointLight->RenderShadowMap(Device, [&](FLight* light) {
			D3DXMATRIX viewproj;
			D3DXVECTOR4 clipplanes(light->GetNearPlane(), light->GetFarPlane(), 0, 0);

			light->CalculateViewProjection(viewproj);

			shadowmap->SetTechnique("variance");
			shadowmap->SetVector("lightPos", &light->GetPosition());
			shadowmap->SetVector("clipPlanes", &clipplanes);

			Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			RenderScene(shadowmap, viewproj);

			CurShadowFrustum->Make(light->viewinv, light->proj);
			// ???? ???? ... 
			DrawInfo _DrawInfo{};
			_DrawInfo._Device = Device;
			_DrawInfo._Frustum = CurShadowFrustum.get();
			// ???????? ???? . ???? ȭ????
			for (auto& [ShaderKey, EntityArr] : RenderEntitys[RenderProperty::Order::Shadow])
			{
				auto Fx = Shaders[ShaderKey]->GetEffect();
				_DrawInfo.Fx = Fx;
				UINT Passes = 0u;
				Fx->SetMatrix("matViewProj", &viewproj);
				Fx->Begin(&Passes, NULL);
				for (int32 i = 0; i < Passes; ++i)
				{
					_DrawInfo.PassIndex = i;
					Fx->BeginPass(i);
					for (auto& [_Entity, _Call] : EntityArr)
					{
						_Call(_DrawInfo);
					}
					Fx->EndPass();
				}
				Fx->End();
			}
		});
	}

	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
void Renderer::RenderGBuffer()
{
	auto* const device = Device;
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);

	device->SetRenderTarget(0, RenderTargets["ALBM"]->GetSurface());
	device->SetRenderTarget(1, RenderTargets["NRMR"]->GetSurface());
	device->SetRenderTarget(2, RenderTargets["Depth"]->GetSurface());

	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	device->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	auto& GBufferGroup = RenderEntitys[RenderProperty::Order::GBuffer];
	DrawInfo _DrawInfo{};
	_DrawInfo.BySituation.reset();
	_DrawInfo._Device = Device;
	_DrawInfo._Frustum = CameraFrustum.get();
	for (auto&  [ ShaderKey , Entitys ] : GBufferGroup)
	{
		auto Fx = Shaders[ShaderKey]->GetEffect();
		Fx->SetMatrix("matViewProj", &_RenderInfo.ViewProjection);
		_DrawInfo.Fx = Fx;
		for (auto&  [ Entity ,Call ] : Entitys)
		{
			UINT Passes{ 0u }; 
			Fx->Begin(&Passes, NULL); 
			for (int32 i = 0; i < Passes; ++i)
			{
				_DrawInfo.PassIndex = i;
				Fx->BeginPass(i);
				{
					Call(_DrawInfo);
				}
				Fx->EndPass();
			}
			Fx->End();
		}
	}

	RenderScene(Shaders["gbuffer_ds"]->GetEffect() , _RenderInfo.ViewProjection);

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	device->SetRenderTarget(1, NULL);
	device->SetRenderTarget(2, NULL);
}

void Renderer::DeferredShading()
{
	D3DXVECTOR4			pixelsize(1, 1, 1, 1);

	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// STEP 2: deferred shading
	pixelsize.x = 1.0f / (float)_RenderInfo.Viewport.Width;
	pixelsize.y = -1.0f / (float)_RenderInfo.Viewport.Height;
	// Device->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1);
	Device->SetRenderState(D3DRS_ZENABLE, FALSE);

	auto device = Device;

	auto scenesurface = RenderTargets["SceneTarget"]->GetSurface();
	auto albedo = RenderTargets["ALBM"]->GetTexture();
	auto normals= RenderTargets["NRMR"]->GetTexture();
	auto depth = RenderTargets["Depth"]->GetTexture();

	auto deferred = Shaders["DeferredShading"]->GetEffect();

	device->SetRenderTarget(0, scenesurface);
	device->Clear(0, NULL, D3DCLEAR_TARGET, 0, 1.0f, 0);

	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, TRUE);

	for (int i = 1; i < 5; ++i) {
		device->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		device->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		device->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	}

	for (int i = 3; i < 5; ++i)
	{
		device->SetSamplerState(i, D3DSAMP_BORDERCOLOR, 0xffffffff);
	}

	device->SetTexture(0, albedo);
	device->SetTexture(1, normals);
	device->SetTexture(2, depth);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	
	deferred->SetTechnique("deferred");
	deferred->SetMatrix("matViewProjInv", &_RenderInfo.ViewProjectionInverse);
	deferred->SetVector("pixelSize", &pixelsize);
	deferred->SetVector("eyePos", &_RenderInfo.Eye);
	

	deferred->Begin(NULL, 0);
	deferred->BeginPass(0);
	{
		D3DXMATRIX lightviewproj;
		D3DXVECTOR4 clipplanes(0, 0, 0, 0);

		for (auto& DirLight : DirLights)
		{
			DirLight->CalculateViewProjection(lightviewproj);
			{
				// directional light
				deferred->SetMatrix("lightViewProj", &lightviewproj);
				deferred->SetVector("lightColor", (D3DXVECTOR4*)&DirLight->GetColor());
				deferred->SetVector("lightPos", &DirLight->GetPosition());
				deferred->SetFloat("lightFlux", DirLight->lightFlux);
				deferred->SetFloat("lightIlluminance", DirLight->lightIlluminance);
				deferred->SetFloat("lightRadius", DirLight->GetPointRadius());
				deferred->SetFloat("specularPower", DirLight->specularPower);

				deferred->SetFloat("sinAngularRadius", DirLight->sinAngularRadius);
				deferred->SetFloat("cosAngularRadius", DirLight->cosAngularRadius);

				clipplanes.x = DirLight->GetNearPlane();
				clipplanes.y = DirLight->GetFarPlane();
				deferred->SetVector("clipPlanes", &clipplanes);
				device->SetTexture(3, DirLight->GetShadowMap());
				deferred->CommitChanges();
				_Quad->Render(Device);
			}
		}

		// ???⼭???? ..
		// point lights
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);

		for (auto& PointLight : PointLights)
		{
			Sphere PtLtSp{};

			PtLtSp.Center = (D3DXVECTOR3&)PointLight->GetPosition();
			PtLtSp.Radius = PointLight->GetPointRadius();
			if (false == CameraFrustum->IsIn(PtLtSp))continue;

			clipplanes.x = PointLight->GetNearPlane();
			clipplanes.y = PointLight->GetFarPlane();
			RECT scissorrect;
			PointLight->CalculateScissorRect(
				scissorrect, _RenderInfo.View, _RenderInfo.Projection,
				PointLight->GetPointRadius(), g_nWndCX, g_nWndCY);

			Device->SetScissorRect(&scissorrect);

			deferred->SetFloat("lightFlux", PointLight->lightFlux);
			deferred->SetFloat("lightIlluminance", PointLight->lightIlluminance);
			deferred->SetFloat("specularPower", PointLight->specularPower);
			deferred->SetFloat("sinAngularRadius", PointLight->sinAngularRadius);
			deferred->SetFloat("cosAngularRadius", PointLight->cosAngularRadius);

			deferred->SetVector("clipPlanes", &clipplanes);
			deferred->SetVector("lightColor", (D3DXVECTOR4*)&PointLight->GetColor());
			deferred->SetVector("lightPos", &PointLight->GetPosition());
			deferred->SetFloat("lightRadius", PointLight->GetPointRadius());
			device->SetTexture(4, PointLight->GetCubeShadowMap());
			deferred->CommitChanges();
			_Quad->Render(Device);
		}

		device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	}

	deferred->EndPass();
	deferred->End();

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, FALSE);

	if (g_bRenderPtLightScissorTest)
	{
		auto screenquad = Shaders["ScreenQuad"]->GetEffect();
		{
			device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
			screenquad->SetTechnique("rect");
			screenquad->Begin(NULL, 0);
			screenquad->BeginPass(0);
			for (auto& PtLt : PointLights)
			{
				// ??????Ʈ ?ܰ? ???? ... 
				float rectvertices[24];
				uint16_t rectindices[5] = { 0, 1, 2, 3, 0 };
				memcpy(rectvertices, DXScreenQuadVerticesFFP, 24 * sizeof(float));

				rectvertices[0] += PtLt->LastScissorRect.left;
				rectvertices[1] += PtLt->LastScissorRect.top;
				rectvertices[6] += PtLt->LastScissorRect.left;
				rectvertices[7] += PtLt->LastScissorRect.bottom;
				rectvertices[12] += PtLt->LastScissorRect.right;
				rectvertices[13] += PtLt->LastScissorRect.bottom;
				rectvertices[18] += PtLt->LastScissorRect.right;
				rectvertices[19] += PtLt->LastScissorRect.top;

				device->DrawIndexedPrimitiveUP(
					D3DPT_LINESTRIP, 0, 4, 4, rectindices, D3DFMT_INDEX16, rectvertices, 6 * sizeof(float));
			}
			screenquad->EndPass();
			screenquad->End();

			device->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1);
		}
	}

}
void Renderer::RenderScene(LPD3DXEFFECT effect, const D3DXMATRIX& viewproj)
{
	return;



	D3DXMATRIX	inv;
	D3DXMATRIX	world[4];
	D3DXVECTOR4	uv(1, 1, 1, 1);

	D3DXHANDLE	oldtech = effect->GetCurrentTechnique();
	D3DXHANDLE	tech = effect->GetTechniqueByName("gbuffer_tbn");

	// setup world matrices
	D3DXMatrixScaling(&world[0], 0.15f, 0.15f, 0.15f);
	D3DXMatrixScaling(&world[1], 0.15f, 0.15f, 0.15f);
	D3DXMatrixScaling(&world[2], 0.15f, 0.15f, 0.15f);
	D3DXMatrixScaling(&world[3], 75.f, 0.1f, 75.f);


	world[0]._41 = -1.5;
	world[0]._43 = 1.5;

	world[1]._41 = 1.5;
	world[1]._43 = 1.5;

	world[2]._41 = 0;
	world[2]._43 = -1;

	world[3]._42 = -0.05f;

	// render
	D3DXMatrixInverse(&inv, NULL, &world[0]);

	effect->SetMatrix("matWorld", &world[0]);
	effect->SetMatrix("matWorldInv", &inv);
	effect->SetMatrix("matViewProj", &viewproj);
	effect->SetVector("uv", &uv);

	effect->Begin(NULL, 0);
	effect->BeginPass(0);
	{
		// skull 1
		Device->SetTexture(0, marble);
		//skull->DrawSubset(0);

		//// skull 2
		//D3DXMatrixInverse(&inv, NULL, &world[1]);

		//effect->SetMatrix("matWorld", &world[1]);
		//effect->SetMatrix("matWorldInv", &inv);
		//effect->CommitChanges();

		//skull->DrawSubset(0);

		//// skull 3
		//D3DXMatrixInverse(&inv, NULL, &world[2]);

		//effect->SetMatrix("matWorld", &world[2]);
		//effect->SetMatrix("matWorldInv", &inv);
		//effect->CommitChanges();
		//skull->DrawSubset(0);

		Matrix targetscale,targettranslation ,targetworld,targetinverseworld;
 		D3DXMatrixScaling(&targetscale, 0.15f, 0.15f, 0.15f) ;
		D3DXMatrixTranslation(&targettranslation, 
			MoonLightTarget.x, MoonLightTarget.y, MoonLightTarget.z);
		targetworld = targetscale * targettranslation;
		D3DXMatrixInverse(&targetinverseworld,nullptr, &targetworld); 

		effect->SetMatrix("matWorld", &targetworld);
		effect->SetMatrix("matWorldInv", &targetinverseworld);
		effect->CommitChanges();
		skull->DrawSubset(0);
	}
	effect->EndPass();
	effect->End();

	// floor
	if (tech)
		effect->SetTechnique(tech);

	D3DXMatrixInverse(&inv, NULL, &world[3]);
	uv = D3DXVECTOR4(30, 30, 0, 0);

	effect->SetMatrix("matWorldInv", &inv);
	effect->SetMatrix("matWorld", &world[3]);
	effect->SetVector("uv", &uv);

	effect->Begin(NULL, 0);
	effect->BeginPass(0);
	{
		Device->SetTexture(0, wood);
		Device->SetTexture(1, wood_normal);

		box->DrawSubset(0);
		uv = Vector4(1, 1, 0, 0);
		effect->SetVector("uv", &uv);
	}
	effect->EndPass();
	effect->End();

	if (tech)
		effect->SetTechnique(oldtech);
};

//
//
//HRESULT Renderer::RenderDeferredShading()&
//{
//	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//	Device->SetRenderState(D3DRS_ZENABLE, FALSE);
//
//	Vector4 pixelsize{};
//	pixelsize.x = 1.0f / static_cast<float> (_RenderInfo.Viewport.Width);
//	pixelsize.y = -1.0f / static_cast<float> (_RenderInfo.Viewport.Height);
//
//	Device->SetFVF(D3DFVF_XYZW |D3DFVF_TEX1);
//	Device->SetRenderState(D3DRS_ZENABLE, FALSE);
//
//	Device->SetRenderTarget(0u, RenderTargets["SceneTarget"]->GetSurface());
//	Device->Clear(0,NULL,D3DCLEAR_TARGET, 0, 1.0f, 0);
//
//	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
//	Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
//	Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
//	Device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, TRUE);
//
//	for (int i = 1; i < 5; ++i) {
//		Device->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//		Device->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//		Device->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
//		Device->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
//		Device->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
//	}
//
//	Device->SetTexture(0, RenderTargets["ALBM"]->GetTexture());
//	Device->SetTexture(1, RenderTargets["NRMR"]->GetTexture());
//	Device->SetTexture(2, RenderTargets["Depth"]->GetTexture());
//
//	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//
//	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
//	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
//
//	auto* const Fx = Shaders["DeferredShading"]->GetEffect();
//	Fx->SetTechnique("deferred");
//	Fx->SetMatrix("matViewProjInv", (D3DXMATRIX*)&_RenderInfo.ViewProjectionInverse);
//	Fx->SetVector("pixelSize", &pixelsize);
//	Fx->SetVector("eyePos", (D3DXVECTOR4*)&_RenderInfo.Eye);
//
//
//	Fx->Begin(NULL, 0);
//	Fx->BeginPass(0);
//
//	Matrix  lightviewproj;
//	Vector4 clipplanes(0, 0, 0, 0);
//
//	Moonlight->CalculateViewProjection(lightviewproj);
//
//	{
//		// directional light
//		Fx->SetMatrix("lightViewProj", &lightviewproj);
//		Fx->SetVector("lightColor", (D3DXVECTOR4*)&Moonlight->GetColor());
//		Fx->SetVector("lightPos", &Moonlight->GetPosition());
//		Fx->SetFloat("specularPower", 200.0f);
//		Fx->SetFloat("lightFlux", Moonlight->lightFlux);
//		Fx->SetFloat("lightIlluminance", Moonlight->lightIlluminance);
//		Fx->SetFloat("specularPower",Moonlight->specularPower);
//		Fx->SetFloat("sinAngularRadius",Moonlight->sinAngularRadius);
//		Fx->SetFloat("cosAngularRadius", Moonlight->cosAngularRadius);
//		
//		Device->SetTexture(3, Moonlight->GetShadowMap());
//		Fx->CommitChanges();
//
//
//		_Quad->Render(Device, 1.f, 1.f, Fx);
//	}
//
//	Device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
//	RECT scissorrect;
//	
//	for (int i = 0; i < 3; ++i)
//	{
//		clipplanes.x = PointLights[i]->GetNearPlane();
//		clipplanes.y = PointLights[i]->GetFarPlane();
//
//		PointLights[i]->CalculateScissorRect
//		(
//			scissorrect, (const D3DXMATRIX&)_RenderInfo.View,
//			(const D3DXMATRIX&)_RenderInfo.Projection,
//			PointLights[i]->GetPointRadius(), g_nWndCX, g_nWndCY);
//
//		Device->SetScissorRect(&scissorrect);
//
//		Fx->SetVector("clipPlanes", &clipplanes);
//		Fx->SetVector("lightColor", (D3DXVECTOR4*)&PointLights[i]->GetColor());
//		Fx->SetVector("lightPos", &PointLights[i]->GetPosition());
//		Fx->SetFloat("specularPower", PointLights[0]->specularPower);
//		Fx->SetFloat("lightRadius", PointLights[i]->GetPointRadius());
//		Fx->SetFloat("lightFlux",PointLights[0]->lightFlux);
//		Fx->SetFloat("lightIlluminance", PointLights[0]->lightIlluminance);
//		Fx->SetFloat("sinAngularRadius", PointLights[0]->sinAngularRadius);
//		Fx->SetFloat("cosAngularRadius", PointLights[0]->cosAngularRadius);
//		
//		Device->SetTexture(4, PointLights[i]->GetCubeShadowMap());
//		Fx->CommitChanges();
//		_Quad->Render(Device, 1.f, 1.f, Fx);
//	}
//
//	Device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
//
//	Fx->EndPass();
//	Fx->End();
//
//	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
//	Device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, FALSE);
//
//	// draw some outline around scissor rect
//	float rectvertices[24];
//	uint16_t rectindices[5] = { 0, 1, 2, 3, 0 };
//
//	memcpy(rectvertices, DXScreenQuadVerticesFFP, 24 * sizeof(float));
//
//	rectvertices[0] += scissorrect.left;
//	rectvertices[1] += scissorrect.top;
//	rectvertices[6] += scissorrect.left;
//	rectvertices[7] += scissorrect.bottom;
//	rectvertices[12] += scissorrect.right;
//	rectvertices[13] += scissorrect.bottom;
//	rectvertices[18] += scissorrect.right;
//	rectvertices[19] += scissorrect.top;
//
//	Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
//	auto*const screenquad = Shaders["ScreenQuad"]->GetEffect();
//	screenquad->SetTechnique("rect");
//	screenquad->Begin(NULL, 0);
//	screenquad->BeginPass(0);
//	{
//		Device->DrawIndexedPrimitiveUP(D3DPT_LINESTRIP, 0, 4, 4, rectindices, D3DFMT_INDEX16, rectvertices, 6 * sizeof(float));
//	}
//	screenquad->EndPass();
//	screenquad->End();
//
//	Device->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1);
//
//	return S_OK;
//};

HRESULT Renderer::RenderDebug()&
{
	if (g_bDebugRender == false)return S_OK;

	DrawInfo _DrawInfo;
	_DrawInfo._Device = Device;
	_DrawInfo.BySituation = {};
	_DrawInfo._Frustum = CameraFrustum.get();
	for (auto& [ShaderKey, _EntityArr] : RenderEntitys[RenderProperty::Order::Debug])
	{
		auto Fx = Shaders[ShaderKey]->GetEffect();
		auto DebugColor = Vector4(0.7f, 0.0f, 0.3f, 0.5f);
		Fx->SetVector("DebugColor", &DebugColor); 
		Fx->SetMatrix("ViewProjection", &_RenderInfo.ViewProjection);
		_DrawInfo.Fx = Fx;
		UINT Passes{ 0u };
		Fx->Begin(&Passes, NULL);
		for (uint32 i = 0; i < Passes; ++i)
		{
			_DrawInfo.PassIndex = i;
			Fx->BeginPass(i);
			for (auto& [_Entity,Call]: _EntityArr)
			{
				Call(_DrawInfo);
			}
			Fx->EndPass();
		}
		Fx->End(); 
	}
	return S_OK;
};


HRESULT Renderer::RenderDebugBone()&
{
	if (g_bDebugBoneToRoot)return S_OK;

	auto& _Order = RenderEntitys[RenderProperty::Order::DebugBone];
	DrawInfo _DrawInfo{};
	_DrawInfo._Device = Device;
	_DrawInfo.BySituation.reset();
	_DrawInfo._Frustum = CameraFrustum.get();
	for (auto& [ShaderKey, _EntityArr] : _Order)
	{
		auto Fx = Shaders[ShaderKey]->GetEffect();
		_DrawInfo.Fx = Fx;
		Vector4 DebugColor {0.3f,0.7f,0.1f,0.5f};
		const Matrix ScaleOffset = FMath::Scale({ 0.01f,0.01f,0.01f });
		const Matrix ViewProjection = _RenderInfo.ViewProjection;
		Fx->SetVector("DebugColor", &DebugColor);
		Fx->SetMatrix("ScaleOffset", &ScaleOffset);
		Fx->SetMatrix("ViewProjection", &ViewProjection);
		UINT Passes = 0u;
		Fx->Begin(&Passes, NULL);
		for (int32 i = 0; i < Passes; ++i)
		{
			Fx->BeginPass(i);
			_DrawInfo.PassIndex = i;
			for (auto& [Entity ,Call]: _EntityArr)
			{
				Call(_DrawInfo);
			}
			Fx->EndPass();
		}
		Fx->End();
	}

	return S_OK;
}


HRESULT Renderer::ImguiRender()&
{
	ImGui::EndFrame();
	Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	Device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	Device->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
};


HRESULT Renderer::RenderTargetDebugRender()&
{
	if (g_bRenderTargetVisible)
	{
		auto RTDebugIter = Shaders.find("RTDebug");
		if (RTDebugIter != std::end(Shaders))
		{
			auto RTDebug = RTDebugIter->second;
			auto* const RTFx = RTDebug->GetEffect();
			for (auto& [RTName, RT] : RenderTargets)
			{
				RT->DebugRender(RTFx);
			};
		}
	}

	return S_OK;
}

HRESULT Renderer::RenderSky()&
{
	auto screenquad = Shaders["ScreenQuad"]->GetEffect();
	screenquad->SetTechnique("screenquad");
	screenquad->Begin(NULL, 0);
	screenquad->BeginPass(0);
	Device->SetTexture(0, sky);
	_Quad->Render(Device);
	screenquad->EndPass();
	screenquad->End();

	return S_OK;
}

HRESULT Renderer::Tonemapping()&
{
	//                     ???????? ???? . 
	Device->SetRenderState(D3DRS_SRGBWRITEENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	auto tonemap = Shaders["ToneMap"]->GetEffect();

	auto scenetarget = RenderTargets["SceneTarget"]->GetTexture();
	D3DXVECTOR4			pixelsize(1, 1, 1, 1);
	pixelsize.x = 1.0f / (float)_RenderInfo.Viewport.Width;
	pixelsize.y = -1.0f / (float)_RenderInfo.Viewport.Height;
	tonemap->SetTechnique("tonemap");
	tonemap->SetVector("pixelSize", &pixelsize);

	tonemap->Begin(NULL, 0);
	tonemap->BeginPass(0);
	Device->SetTexture(0, scenetarget);
	_Quad->Render(Device);
	tonemap->EndPass();
	tonemap->End();

	return S_OK;
}
HRESULT Renderer::AlphaBlendEffectRender()&
{
	auto& _Group = RenderEntitys[RenderProperty::Order::AlphaBlendEffect];
	DrawInfo _DrawInfo{};
	_DrawInfo.BySituation.reset();
	_DrawInfo._Device = Device;
	_DrawInfo._Frustum = CameraFrustum.get();
	for (auto& [ShaderKey, Entitys] : _Group)
	{
		auto Fx = Shaders[ShaderKey]->GetEffect();
		Fx->SetMatrix("ViewProjection", &_RenderInfo.ViewProjection);
		
		_DrawInfo.Fx = Fx;
		for (auto& [Entity, Call] : Entitys)
		{
			UINT Passes{ 0u };
			Fx->Begin(&Passes, NULL);
			for (int32 i = 0; i < Passes; ++i)
			{
				_DrawInfo.PassIndex = i;
				Fx->BeginPass(i);
				{
					Call(_DrawInfo);
				}
				Fx->EndPass();
			}
			Fx->End();
		}
	}
	return S_OK;
}
HRESULT Renderer::UIRender()&
{
	auto& _Group = RenderEntitys[RenderProperty::Order::UI];
	DrawInfo _DrawInfo{};
	_DrawInfo.BySituation.reset();
	_DrawInfo._Device = Device;
	for (auto& [ShaderKey, Entitys] : _Group)
	{
		auto Fx = Shaders[ShaderKey]->GetEffect();
		Fx->SetMatrix("Ortho", &_RenderInfo.Ortho);
		_DrawInfo.Fx = Fx;
		for (auto& [Entity, Call] : Entitys)
		{
			UINT Passes{ 0u };
			Fx->Begin(&Passes, NULL);
			Call(_DrawInfo);
			Fx->End();
		}
	}

	return S_OK;
}
;


bool Renderer::TestShaderInit()
{
	//// Shader Test ... 
	if (FAILED(D3DXLoadMeshFromX(L"../../Media/MeshesDX/box.x", D3DXMESH_MANAGED, Device, NULL, NULL, NULL, NULL, &box)))
		return false;

	if (FAILED(DxHelper::DXGenTangentFrame(Device, box, &box)))
		return false;

	if (FAILED(D3DXLoadMeshFromX(L"../../Media/MeshesDX/skullocc3.x", D3DXMESH_MANAGED, Device, NULL, NULL, NULL, NULL, &skull)))
		return false;

	if (FAILED(D3DXCreateTextureFromFileA(Device, "../../Media/Textures/marble.dds", &marble)))
		return false;

	if (FAILED(D3DXCreateTextureFromFileA(Device, "../../Media/Textures/wood2.jpg", &wood)))
		return false;

	if (FAILED(D3DXCreateTextureFromFileA(Device, "../../Media/Textures/wood2_normal.tga", &wood_normal)))
		return false;

	if (FAILED(D3DXCreateTextureFromFileA(Device, "../../Media/Textures/static_sky.jpg", &sky)))
		return false;
}

void Renderer::TestShaderRelease()
{
	if (wood_normal)
		wood_normal->Release();
	if (sky)
		sky->Release();
	if (wood)
		wood->Release();
	if (marble)
		marble->Release();
	if (skull)
		skull->Release();
	if (box)
		box->Release();
}

// ????Ʈ ????Ʈ ȸ?? !
void Renderer::TestLightRotation()
{
	static float time = 0;
	PointLights[0]->GetPosition().x = cosf(time * 0.5f) * 2;
	PointLights[0]->GetPosition().z = sinf(time * 0.5f) * cosf(time * 0.5f) * 2;

	PointLights[1]->GetPosition().x = cosf(1.5f * time) * 2;
	PointLights[1]->GetPosition().z = sinf(1 * time) * 2;

	PointLights[2]->GetPosition().x = cosf(0.75f * time) * 1.5f;
	PointLights[2]->GetPosition().z = sinf(1.5f * time) * 1.5f;

	time += TimeSystem::GetInstance()->DeltaTime();
};
