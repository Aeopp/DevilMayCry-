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
	// 달빛
	Moonlight = std::make_shared<FLight>
		(FLight(FLight::Type::Directional,
		{ 0,0,0,0 }, (const D3DXCOLOR&)Color::sRGBToLinear(250, 250, 250)));
	PointLights.resize(3u);

	PointLights[0] = std::make_shared<FLight>(
		FLight(
			FLight::Type::Point, { 1.5f,0.5f, 0.0f ,1 },
			{ 1,0,0,1 }));

	PointLights[1] = std::make_shared<FLight>(
		FLight(
			FLight::Type::Point, { -0.7f , 0.5f , 1.2f , 1.f },
			{ 0,1,0,1 }));

	PointLights[2] = std::make_shared<FLight>(
		FLight(
			FLight::Type::Point,
			{ 0.0f,0.5f,0.0f,1 },
			{ 0,0,1,1 }));

	// 그림자맵 512 로 생성
	Moonlight->CreateShadowMap(Device, 512);
	Moonlight->SetProjectionParameters(7.1f, 7.1f, -20.f, +20.f);

	PointLights[0]->CreateShadowMap(Device, 256);
	PointLights[1]->CreateShadowMap(Device, 256);
	PointLights[2]->CreateShadowMap(Device, 256);

	PointLights[0]->SetProjectionParameters(0, 0, 0.1f, 10.0f);
	PointLights[1]->SetProjectionParameters(0, 0, 0.1f, 10.0f);
	PointLights[2]->SetProjectionParameters(0, 0, 0.1f, 10.0f);
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

	Device->GetViewport(&_RenderInfo.Viewport);
}

void Renderer::ReadyFrustum()
{
	CameraFrustum = std::make_shared<Frustum>();
	CameraFrustum->Initialize(Device);
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
				for (const auto& _EntityRenderOrder: _SharedRenderEntity->GetRenderProp().RenderOrders)
				{
					RenderEntitys[_EntityRenderOrder].push_back(_SharedRenderEntity.get());
				}
			}
		}
	}
};

// 렌더 레디에서 절두체 준비 ..
HRESULT Renderer::Render()&
{
	RenderReady();
	RenderBegin();
	D3DXVECTOR4			pixelsize(1, 1, 1, 1);

	RenderShadowMaps();
		{
		// D3DVIEWPORT9 oldviewport;
		

		// STEP 0: render shadow maps
		
		{
		
		}

		// STEP 1: g-buffer pass
		{
			RenderGBuffer(_RenderInfo.ViewProjection);
		}
		Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// STEP 2: deferred shading
		pixelsize.x = 1.0f / (float)_RenderInfo.Viewport.Width;
		pixelsize.y = -1.0f / (float)_RenderInfo.Viewport.Height;

		Device->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1);
		Device->SetRenderState(D3DRS_ZENABLE, FALSE);

		DeferredShading(
			_RenderInfo.View, 
			_RenderInfo.ViewProjection, 
			_RenderInfo.ViewProjectionInverse, 
			Vector4{_RenderInfo.Eye.x,
			_RenderInfo.Eye.y ,
			_RenderInfo.Eye.z ,1.f} );

		// STEP 3: render sky
		Device->SetRenderTarget(0, BackBuffer);
		Device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
		BackBuffer->Release();

		auto screenquad = Shaders["ScreenQuad"]->GetEffect();

		screenquad->SetTechnique("screenquad");
		screenquad->Begin(NULL, 0);
		screenquad->BeginPass(0);
		{
			Device->SetTexture(0, sky);
			_Quad->Render(Device);
			//device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, DXScreenQuadVertices, 6 * sizeof(float));
		}
		screenquad->EndPass();
		screenquad->End();

		// STEP 4: tone mapping
		Device->SetRenderState(D3DRS_SRGBWRITEENABLE, TRUE);
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		auto tonemap = Shaders["ToneMap"]->GetEffect();

		auto scenetarget = RenderTargets["SceneTarget"]->GetTexture();

		tonemap->SetTechnique("tonemap");
		tonemap->SetVector("pixelSize", &pixelsize);

		tonemap->Begin(NULL, 0);
		tonemap->BeginPass(0);
		{
			Device->SetTexture(0, scenetarget);
			_Quad->Render(Device);
			// device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, DXScreenQuadVertices, 6 * sizeof(float));
		}
		tonemap->EndPass();
		tonemap->End();

		//// render text
		//viewport = oldviewport;

		//viewport.Width = 1024;
		//viewport.Height = 256;
		//viewport.X = viewport.Y = 10;

		//device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
		//device->SetViewport(&viewport);

		//screenquad->Begin(NULL, 0);
		//screenquad->BeginPass(0);
		//{
		//	// device->SetTexture(0, helptext);
		//	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, DXScreenQuadVertices, 6 * sizeof(float));
		//}
		//screenquad->EndPass();
		//screenquad->End();

		// reset states
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		Device->SetRenderState(D3DRS_ZENABLE, TRUE);
		Device->SetViewport(&_RenderInfo.Viewport);

		Device->EndScene();
	}

	// GraphicSystem::GetInstance()->Begin();
	// RenderImplementation();
	RenderTargetDebugRender();
	ImguiRender();
	// GraphicSystem::GetInstance()->End();
	RenderEnd();
	Device->Present(NULL, NULL, NULL, NULL);

	return S_OK;
}

void Renderer::Editor()&
{
	if (ImGui::Begin("Render Editor"))
	{
		if (ImGui::CollapsingHeader("Lights"))
		{
			uint32 Idx = 0u;
			for (auto& _Light : PointLights)
			{
				_Light->Edit(Idx);
				++Idx;
			}
			Moonlight->Edit(Idx);
		}
		ImGui::End();
	}
}

void Renderer::RenderReady()&
{
	RenderReadyEntitys();
	ReadyRenderInfo();
	Culling();




	TestLightRotation();
	TestLightEdit();
}
void Renderer::RenderBegin()&
{
	GraphicSystem::GetInstance()->Begin();
	Device->GetRenderTarget(0, &BackBuffer);
}
// 등록코드수정 
void Renderer::RenderReadyEntitys()&
{
	for (auto& [_Order,RenderEntitys] : RenderEntitys)
	{
		for (auto& RenderEntity : RenderEntitys)
		{
			RenderEntity->RenderReady();
		}
	}
}

void Renderer::Culling()&
{
	FrustumCulling();
}

void Renderer::FrustumCulling()&
{
	CameraFrustum->Make(_RenderInfo.ViewInverse, _RenderInfo.Projection);
	// 절두체에서 검사해서 Entity 그룹에서 지우기 ....
}

void Renderer::RenderEnd()&
{
	_PrevRenderInfo = _RenderInfo;
	RenderEntityClear();
}

void Renderer::RenderEntityClear()&
{
	RenderEntitys.clear();
}

HRESULT Renderer::RenderImplementation()&
{
	//Device->GetRenderTarget(0u, &BackBuffer);
	//Device->GetViewport(&BackBufViewport);
	//{
	//	RenderShadows();
	//	RenderGBuffer();
	//    RenderDeferredShading();
	//	Device->SetRenderTarget(0u, BackBuffer);
	//	Device->Clear(
	//		0u, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0, 1.0f, 0);
	//	Device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	//	auto*const screenquad = Shaders["ScreenQuad"]->GetEffect();  
	//	screenquad->SetTechnique("screenquad");
	//	screenquad->Begin(NULL, 0);
	//	screenquad->BeginPass(0);
	//	{
	//		Device->SetTexture(0, sky);
	//		_Quad->Render(Device, 1.f, 1.f, screenquad);
	//	}
	//	screenquad->EndPass();
	//	screenquad->End();

	//	Device->SetRenderState(D3DRS_SRGBWRITEENABLE, TRUE);
	//	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//	auto* const tonemap = Shaders["ToneMap"]->GetEffect();
	//	tonemap->SetTechnique("tonemap");
	//	Vector4 pixelsize{};
	//	pixelsize.x = 1.0f / static_cast<float> (BackBufViewport.Width);
	//	pixelsize.y = -1.0f / static_cast<float> (BackBufViewport.Height);
	//	tonemap->SetVector("pixelSize", &pixelsize);

	//	tonemap->Begin(NULL, 0);
	//	tonemap->BeginPass(0);
	//	{
	//		Device->SetTexture(0, RenderTargets["SceneTarget"]->GetTexture());
	//		_Quad->Render(Device, 1.f, 1.f, tonemap);
	//	}
	//	tonemap->EndPass();
	//	tonemap->End();

	//	D3DVIEWPORT9 viewport = BackBufViewport;
	//	viewport.Width = 1024;
	//	viewport.Height = 256;
	//	viewport.X = viewport.Y = 10;


	//	Device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	//	Device->SetViewport(&viewport);


	//	/*screenquad->Begin(NULL, 0);
	//	screenquad->BeginPass(0);
	//	{
	//		Device->SetTexture()
	//	}*/


	//	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//	Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	//	Device->SetViewport(&BackBufViewport);
	//	/*RenderForwardAlphaBlend();
	//	RenderAlphaBlendEffect();
	//	RenderUI();
	//	RenderDebug();
	//	RenderDebugBone();*/
	//}
	//// Device->SetRenderTarget(0u, BackBuffer);
	//BackBuffer->Release();

	return S_OK;
}

void Renderer::RenderShadowMaps()
{
	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	auto shadowmap = Shaders["Shadow"]->GetEffect();
	auto Blur = Shaders["Blur"]->GetEffect();

	Moonlight->RenderShadowMap(Device, [&](FLight* light) {
		D3DXMATRIX viewproj;
		D3DXVECTOR4 clipplanes(light->GetNearPlane(), light->GetFarPlane(), 0, 0);

		light->CalculateViewProjection(viewproj);

		shadowmap->SetTechnique("variance");
		shadowmap->SetVector("clipPlanes", &clipplanes);
		shadowmap->SetBool("isPerspective", FALSE);

		Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
		RenderScene(shadowmap, viewproj);
		});

	Moonlight->BlurShadowMap(Device, [&](FLight* light) {
		D3DXVECTOR4 pixelsize(1.0f / light->GetShadowMapSize(),
			1.0f / light->GetShadowMapSize(), 0, 0);
		D3DXVECTOR4 TexelSize = 4.0f * pixelsize;	// make it more blurry
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

	// point lights
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	shadowmap->SetBool("isPerspective", TRUE);

	for (int i = 0; i < 3; ++i) {
		PointLights[i]->RenderShadowMap(Device, [&](FLight* light) {
			D3DXMATRIX viewproj;
			D3DXVECTOR4 clipplanes(light->GetNearPlane(), light->GetFarPlane(), 0, 0);

			light->CalculateViewProjection(viewproj);

			shadowmap->SetTechnique("variance");
			shadowmap->SetVector("lightPos", &light->GetPosition());
			shadowmap->SetVector("clipPlanes", &clipplanes);

			Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			RenderScene(shadowmap, viewproj);
			});
	}

	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
void Renderer::RenderGBuffer(const Matrix& ViewProjection)
{
	auto* const device = Device;

	// device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
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

	// auto viewproj2 = FMath::Transpose( (Matrix&)viewproj);
	auto gbuffer = Shaders["gbuffer_ds"]->GetEffect();
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	{
		gbuffer->SetTechnique("gbuffer");
		RenderScene(gbuffer, (const D3DXMATRIX&)ViewProjection);
	}
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	device->SetRenderTarget(1, NULL);
	device->SetRenderTarget(2, NULL);
}

void Renderer::DeferredShading(
	const Matrix& View,
	const Matrix& Projection,
	const Matrix& ViewProjectionInverse,
	const Vector4& Eye)
{
	RECT scissorrect;

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

	device->SetTexture(0, albedo);
	device->SetTexture(1, normals);
	device->SetTexture(2, depth);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	Vector4 pixelsize = { 1.0f / g_nWndCX,-1.0f / g_nWndCY ,0,0 };

	deferred->SetTechnique("deferred");
	deferred->SetMatrix("matViewProjInv", &ViewProjectionInverse);
	deferred->SetVector("pixelSize", &pixelsize);

	if (bCurstomEye)
	{
		deferred->SetVector("eyePos", &CurstomEye);
	}
	else
	{
		deferred->SetVector("eyePos", &Eye);
	}
	

	deferred->Begin(NULL, 0);
	deferred->BeginPass(0);
	{
		D3DXMATRIX lightviewproj;
		D3DXVECTOR4 clipplanes(0, 0, 0, 0);

		
		Moonlight->CalculateViewProjection(lightviewproj);

		{
			// directional light
			deferred->SetMatrix("lightViewProj", &lightviewproj);
			deferred->SetVector("lightColor", (D3DXVECTOR4*)&Moonlight->GetColor());
			deferred->SetVector("lightPos", &Moonlight->GetPosition());
			deferred->SetFloat("specularPower", 200.0f);
			static float sinAngularRadius = 0.0046251;
			static float cosAngularRadius = 0.9999893;
			ImGui::SliderFloat("sinAngularRadius", &sinAngularRadius, -1.f, 1.f);
			ImGui::SliderFloat("cosAngularRadius ", &cosAngularRadius, -1.f, 1.f);

			deferred->SetFloat("sinAngularRadius", sinAngularRadius );
			deferred->SetFloat("cosAngularRadius", cosAngularRadius );

			device->SetTexture(3, Moonlight->GetShadowMap());
			deferred->CommitChanges();
			_Quad->Render(Device);

			// device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, DXScreenQuadVertices, 6 * sizeof(float));
		}
		// 여기서부터 ..
		// point lights
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);

		for (int i = 0; i < 3; ++i) {
			clipplanes.x = PointLights[i]->GetNearPlane();
			clipplanes.y = PointLights[i]->GetFarPlane();
			float POINT_LIGHT_RADIUS = 7.f;

			 {
				/*Matrix viewtranspose , projtranspose;

				D3DXMatrixTranspose(&viewtranspose, (Matrix*)&view);
				D3DXMatrixTranspose(&projtranspose, (Matrix*)&proj);*/
				Math::Matrix viewtranspose, projtranspose ,view,proj; 
				std::memcpy(&view, &View, sizeof(Matrix));
				std::memcpy(&proj, &Projection, sizeof(Matrix)); 
				Math::MatrixTranspose(viewtranspose, view);   
				Math::MatrixTranspose(projtranspose, proj);

				PointLights[i]->CalculateScissorRect(
					scissorrect, (const D3DXMATRIX&)viewtranspose, (const D3DXMATRIX&)projtranspose,
					POINT_LIGHT_RADIUS, g_nWndCX, g_nWndCY);

				//Matrix dxview, dxproj;
				//std::memcpy(&dxview, &view, sizeof(Matrix));
				//std::memcpy(&dxproj, &proj, sizeof(Matrix));
				//Matrix dxviewproj = dxview* dxproj;
				//Vector3 pointposition;
				//std::memcpy(&pointposition, &PointLights[i]->GetPosition(), sizeof(Vector3));
				//Vector3 pointposition2 = pointposition + Vector3{ 1,0,0 } *POINT_LIGHT_RADIUS;
				//D3DXVec3TransformCoord(&pointposition, &pointposition, &dxviewproj);
				//D3DXVec3TransformCoord(&pointposition2, &pointposition2, &dxviewproj);

				//Matrix viewporttransform =
				//{
				//	g_nWndCX / 2.f , 0, 0, 0  ,
				//	0 ,          g_nWndCY / -2.f ,0,0,
				//		0,0,1.0f - 0.f ,0,
				//0 + g_nWndCX / 2.f , 0 + g_nWndCY / 2.f , 0.f,1 };

				//D3DXVec3TransformCoord(&pointposition, &pointposition, &viewporttransform);
				//D3DXVec3TransformCoord(&pointposition2, &pointposition2, &viewporttransform);
				//Vector3 distance = pointposition2 - pointposition;
				//const float _dist = D3DXVec3Length(&distance) / 2.f;
				//const Vector3 center = (pointposition + pointposition2) / 2.f;
				//
				//scissorrect.left = center.x - _dist;
				//scissorrect.right = center.x + _dist;
				//scissorrect.top = center.y  - _dist;
				//scissorrect.bottom = center.y + _dist;

				//device->SetScissorRect(&scissorrect);

				deferred->SetVector("clipPlanes", &clipplanes);
				deferred->SetVector("lightColor", (D3DXVECTOR4*)&PointLights[i]->GetColor());
				deferred->SetVector("lightPos", &PointLights[i]->GetPosition());
				deferred->SetFloat("specularPower", 80.0f);
				deferred->SetFloat("lightRadius", POINT_LIGHT_RADIUS);
				deferred->CommitChanges();

				device->SetTexture(4, PointLights[i]->GetCubeShadowMap());
				_Quad->Render(Device);				
				// device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, DXScreenQuadVertices, 6 * sizeof(float));
			}
		}

		device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	}
	deferred->EndPass();
	deferred->End();

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, FALSE);

	auto screenquad = Shaders["ScreenQuad"]->GetEffect();
	{
		// draw some outline around scissor rect
		float rectvertices[24];
		uint16_t rectindices[5] = { 0, 1, 2, 3, 0 };

		memcpy(rectvertices, DXScreenQuadVerticesFFP, 24 * sizeof(float));

		rectvertices[0] += scissorrect.left;
		rectvertices[1] += scissorrect.top;
		rectvertices[6] += scissorrect.left;
		rectvertices[7] += scissorrect.bottom;
		rectvertices[12] += scissorrect.right;
		rectvertices[13] += scissorrect.bottom;
		rectvertices[18] += scissorrect.right;
		rectvertices[19] += scissorrect.top;

		device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

		screenquad->SetTechnique("rect");
		screenquad->Begin(NULL, 0);
		screenquad->BeginPass(0);
		{
			device->DrawIndexedPrimitiveUP(
				D3DPT_LINESTRIP, 0, 4, 4, rectindices, D3DFMT_INDEX16, rectvertices, 6 * sizeof(float));
		}
		screenquad->EndPass();
		screenquad->End();

		device->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1);
	}
}
void Renderer::RenderScene(LPD3DXEFFECT effect, const D3DXMATRIX& viewproj)
{
	D3DXMATRIX	inv;
	D3DXMATRIX	world[4];
	D3DXVECTOR4	uv(1, 1, 1, 1);

	D3DXHANDLE	oldtech = effect->GetCurrentTechnique();
	D3DXHANDLE	tech = effect->GetTechniqueByName("gbuffer_tbn");

	// setup world matrices
	D3DXMatrixScaling(&world[0], 0.15f, 0.15f, 0.15f);
	D3DXMatrixScaling(&world[1], 0.15f, 0.15f, 0.15f);
	D3DXMatrixScaling(&world[2], 0.15f, 0.15f, 0.15f);
	D3DXMatrixScaling(&world[3], 5, 0.1f, 5);


	static Vector3 allrotationeuler{ 0,0,0 };
	ImGui::SliderFloat3("allrotationeuler", allrotationeuler, -360.f, 360.f);
	D3DXMATRIX Allrotation{};

	D3DXMatrixRotationYawPitchRoll(&Allrotation, 
		FMath::ToRadian( allrotationeuler.y  ) ,  
		FMath::ToRadian( allrotationeuler.x ) ,
		FMath::ToRadian( allrotationeuler.z  )  );

	world[0] *= Allrotation;
	world[1] *= Allrotation;
	world[2] *= Allrotation;
	world[3] *= Allrotation;

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
		skull->DrawSubset(0);

		// skull 2
		D3DXMatrixInverse(&inv, NULL, &world[1]);

		effect->SetMatrix("matWorld", &world[1]);
		effect->SetMatrix("matWorldInv", &inv);
		effect->CommitChanges();

		skull->DrawSubset(0);

		// skull 3
		D3DXMatrixInverse(&inv, NULL, &world[2]);

		effect->SetMatrix("matWorld", &world[2]);
		effect->SetMatrix("matWorldInv", &inv);
		effect->CommitChanges();
		skull->DrawSubset(0);

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
	uv = D3DXVECTOR4(2, 2, 0, 0);

	effect->SetMatrix("matWorldInv", &inv);
	effect->SetMatrix("matWorld", &world[3]);
	effect->SetVector("uv", &uv);

	effect->Begin(NULL, 0);
	effect->BeginPass(0);
	{
		Device->SetTexture(0, wood);
		Device->SetTexture(1, wood_normal);

		box->DrawSubset(0);
	}
	effect->EndPass();
	effect->End();

	if (tech)
		effect->SetTechnique(oldtech);
}
//
//HRESULT Renderer::RenderGBuffer()&
//{
//	/*Device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
//
//	Device->SetRenderTarget(0u, RenderTargets["ALBM"]->GetSurface());
//	Device->SetRenderTarget(1u, RenderTargets["NRMR"]->GetSurface());
//	Device->SetRenderTarget(2u, RenderTargets["Depth"]->GetSurface());
//
//	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
//	Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
//	Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
//
//	Device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//	Device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//	Device->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
//	Device->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
//	Device->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
//
//	Device->Clear(0u, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
//
//	auto GBufferRenderImplementation = [this](
//		const RenderProperty::Order _Order,
//		ID3DXEffect*const Fx
//		) 
//	{
//			if (auto _TargetGroup = RenderEntitys.find(_Order);
//				_TargetGroup != std::end(RenderEntitys))
//			{
//				RenderInterface::ImplementationInfo _ImplInfo{};
//				_ImplInfo.Fx = Fx;
//				_ImplInfo._Device = Device;
//				Fx->SetMatrix("ViewProjection", &CurrentRenderInfo.ViewProjection);
//
//				uint32 Passes = 0u;
//				Fx->Begin(&Passes, NULL);
//
//				for (uint32 i = 0; i < Passes; ++i)
//				{
//					Fx->BeginPass(i);
//					_ImplInfo.PassIndex = i;
//
//					for (auto& _RenderEntity : _TargetGroup->second)
//					{
//						if (_RenderEntity)
//						{
//							if (_RenderEntity->GetRenderProp().bRender)
//							{
//								_RenderEntity->RenderGBufferImplementation(_ImplInfo);
//							}
//						}
//					}
//
//					Fx->EndPass();
//				}
//
//				Fx->End();
//			};
//	};
//
//	
//	GBufferRenderImplementation(RenderProperty::Order::GBuffer, Shaders["GBuffer"]->GetEffect());
//	GBufferRenderImplementation(RenderProperty::Order::GBufferSK, Shaders["GBufferSK"]->GetEffect() );
//
//	Device->SetRenderTarget(1u, nullptr);
//	Device->SetRenderTarget(2u, nullptr);
//
//	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//
//	return S_OK;*/
//}


HRESULT Renderer::RenderDeferredShading()&
{
	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	Device->SetRenderState(D3DRS_ZENABLE, FALSE);

	Vector4 pixelsize{};
	pixelsize.x = 1.0f / static_cast<float> (_RenderInfo.Viewport.Width);
	pixelsize.y = -1.0f / static_cast<float> (_RenderInfo.Viewport.Height);

	Device->SetFVF(D3DFVF_XYZW |D3DFVF_TEX1);
	Device->SetRenderState(D3DRS_ZENABLE, FALSE);

	Device->SetRenderTarget(0u, RenderTargets["SceneTarget"]->GetSurface());
	Device->Clear(0,NULL,D3DCLEAR_TARGET, 0, 1.0f, 0);

	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	Device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, TRUE);

	for (int i = 1; i < 5; ++i) {
		Device->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		Device->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		Device->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		Device->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		Device->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	}

	Device->SetTexture(0, RenderTargets["ALBM"]->GetTexture());
	Device->SetTexture(1, RenderTargets["NRMR"]->GetTexture());
	Device->SetTexture(2, RenderTargets["Depth"]->GetTexture());

	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	auto* const Fx = Shaders["DeferredShading"]->GetEffect();
	Fx->SetTechnique("deferred");
	Fx->SetMatrix("matViewProjInv", (D3DXMATRIX*)&_RenderInfo.ViewProjectionInverse);
	Fx->SetVector("pixelSize", &pixelsize);

	if (bCurstomEye)
	{
		Fx->SetVector("eyePos",&CurstomEye);
	}
	else
	{
		Fx->SetVector("eyePos", (D3DXVECTOR4*)&_RenderInfo.Eye);
	}
	

	Fx->Begin(NULL, 0);
	Fx->BeginPass(0);

	Matrix  lightviewproj;
	Vector4 clipplanes(0, 0, 0, 0);

	Moonlight->CalculateViewProjection(lightviewproj);

	{
		// directional light
		Fx->SetMatrix("lightViewProj", &lightviewproj);
		Fx->SetVector("lightColor", (D3DXVECTOR4*)&Moonlight->GetColor());
		Fx->SetVector("lightPos", &Moonlight->GetPosition());
		Fx->SetFloat("specularPower", 200.0f);
		

		Device->SetTexture(3, Moonlight->GetShadowMap());
		Fx->CommitChanges();


		_Quad->Render(Device, 1.f, 1.f, Fx);
	}

	Device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	RECT scissorrect;
	static constexpr float POINT_LIGHT_RADIUS = 3.0f;
	for (int i = 0; i < 3; ++i)
	{
		clipplanes.x = PointLights[i]->GetNearPlane();
		clipplanes.y = PointLights[i]->GetFarPlane();

		PointLights[i]->CalculateScissorRect
		(
			scissorrect, (const D3DXMATRIX&)_RenderInfo.View,
			(const D3DXMATRIX&)_RenderInfo.Projection,
			POINT_LIGHT_RADIUS, g_nWndCX, g_nWndCY);

		Device->SetScissorRect(&scissorrect);

		Fx->SetVector("clipPlanes", &clipplanes);
		Fx->SetVector("lightColor", (D3DXVECTOR4*)&PointLights[i]->GetColor());
		Fx->SetVector("lightPos", &PointLights[i]->GetPosition());
		Fx->SetFloat("specularPower", 80.0f);
		Fx->SetFloat("lightRadius", POINT_LIGHT_RADIUS);
		Device->SetTexture(4, PointLights[i]->GetCubeShadowMap());
		Fx->CommitChanges();
		_Quad->Render(Device, 1.f, 1.f, Fx);
	}

	Device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	Fx->EndPass();
	Fx->End();

	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	Device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, FALSE);

	// draw some outline around scissor rect
	float rectvertices[24];
	uint16_t rectindices[5] = { 0, 1, 2, 3, 0 };

	memcpy(rectvertices, DXScreenQuadVerticesFFP, 24 * sizeof(float));

	rectvertices[0] += scissorrect.left;
	rectvertices[1] += scissorrect.top;
	rectvertices[6] += scissorrect.left;
	rectvertices[7] += scissorrect.bottom;
	rectvertices[12] += scissorrect.right;
	rectvertices[13] += scissorrect.bottom;
	rectvertices[18] += scissorrect.right;
	rectvertices[19] += scissorrect.top;

	Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	auto*const screenquad = Shaders["ScreenQuad"]->GetEffect();
	screenquad->SetTechnique("rect");
	screenquad->Begin(NULL, 0);
	screenquad->BeginPass(0);
	{
		Device->DrawIndexedPrimitiveUP(D3DPT_LINESTRIP, 0, 4, 4, rectindices, D3DFMT_INDEX16, rectvertices, 6 * sizeof(float));
	}
	screenquad->EndPass();
	screenquad->End();

	Device->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1);

	return S_OK;
}

HRESULT Renderer::RenderForwardAlphaBlend()&
{
	Device->SetRenderTarget(0u, BackBuffer);

	auto ForwardAlphaBlendImplementation = [this](const RenderProperty::Order& _Order , ID3DXEffect*const Fx) 
	{
		if (auto _TargetGroup = RenderEntitys.find(_Order);
			_TargetGroup != std::end(RenderEntitys))
		{
			Fx->SetMatrix("View", &_RenderInfo.View);
			Fx->SetMatrix("Projection", &_RenderInfo.Projection);

			RenderInterface::ImplementationInfo _ImplInfo{};
			_ImplInfo.Fx = Fx;
			_ImplInfo._Device = Device;

			uint32 Passes = 0u;
			Fx->Begin(&Passes, NULL);

			for (uint32 i = 0; i < Passes; ++i)
			{
				_ImplInfo.PassIndex = i;
				Fx->BeginPass(i);
				for (auto& _RenderEntity : _TargetGroup->second)
				{
					if (_RenderEntity)
					{
						if (_RenderEntity->GetRenderProp().bRender)
						{
							_RenderEntity->RenderForwardAlphaBlendImplementation(_ImplInfo);
						}
					}
				}
				Fx->EndPass();
			}
			Fx->End();
		}
	};
	
	ForwardAlphaBlendImplementation(RenderProperty::Order::ForwardAlphaBlend, Shaders["ForwardAlphaBlend"]->GetEffect());
	ForwardAlphaBlendImplementation(RenderProperty::Order::ForwardAlphaBlendSK, Shaders["ForwardAlphaBlendSK"]->GetEffect());

	return S_OK;
}

HRESULT Renderer::RenderAlphaBlendEffect()&
{
	//if (auto _TargetGroup = RenderEntitys.find(ENGINE::RenderProperty::Order::AlphaBlendEffect);
	//	_TargetGroup != std::end(RenderEntitys))
	//{
	//	Device->SetRenderTarget(0u, BackBuffer);

	//	for (auto& _RenderEntity : _TargetGroup->second)
	//	{
	//		if (_RenderEntity)
	//		{
	//			if (_RenderEntity->GetRenderProp().bRender)
	//			{
	//				_RenderEntity->RenderAlphaBlendEffect();
	//			}
	//		}
	//	}
	//}

	//return S_OK;

	return S_OK;
}

HRESULT Renderer::RenderDebug()&
{
	if (g_bDebugRender)
	{
		Device->SetRenderTarget(0u, BackBuffer);

		auto DebugRenderImplementation = [this](const RenderProperty::Order & _Order , ID3DXEffect*const Fx) 
		{
			if (auto _TargetGroup = RenderEntitys.find(_Order);
				_TargetGroup != std::end(RenderEntitys))
			{
				Fx->SetMatrix("View",
					&_RenderInfo.View);
				Fx->SetMatrix("Projection", &_RenderInfo.Projection);

				RenderInterface::ImplementationInfo _ImplInfo{};
				_ImplInfo.Fx = Fx;
				_ImplInfo._Device = Device;
				uint32 Passes = 0u;
				Fx->Begin(&Passes, NULL);

				for (uint32 i = 0; i < Passes; ++i)
				{
					_ImplInfo.PassIndex = i;
					Fx->BeginPass(i);

					for (auto& _RenderEntity : _TargetGroup->second)
					{
						if (_RenderEntity)
						{
							if (_RenderEntity->GetRenderProp().bRender)
							{
								_RenderEntity->
									RenderDebugImplementation(_ImplInfo);
							}
						}
					}

					Fx->EndPass();
				}

				Fx->End();
			}
		};

	
		DebugRenderImplementation(RenderProperty::Order::Debug, Shaders["Debug"]->GetEffect());
		DebugRenderImplementation(RenderProperty::Order::DebugBone, Shaders["DebugSK"]->GetEffect());
	}

	return S_OK;
}

HRESULT Renderer::RenderDebugBone()&
{
	if (g_bDebugRender)
	{
		if (auto _TargetGroup = RenderEntitys.find(ENGINE::RenderProperty::Order::DebugBone);
			_TargetGroup != std::end(RenderEntitys))
		{
			Device->SetRenderTarget(0u, BackBuffer);
			auto* const Fx = Shaders["DebugBone"]->GetEffect();

			Fx->SetMatrix("View",
				&_RenderInfo.View);
			Fx->SetMatrix("Projection", &_RenderInfo.Projection);
			const Matrix ScaleOffset = FMath::Scale({ 0.01 ,0.01,0.01 });
			Fx->SetMatrix("ScaleOffset", &ScaleOffset);

			RenderInterface::ImplementationInfo _ImplInfo{};
			_ImplInfo.Fx = Fx;
			_ImplInfo._Device = Device;
			uint32 Passes = 0u;
			Fx->Begin(&Passes, NULL);

			for (uint32 i = 0; i < Passes; ++i)
			{
				_ImplInfo.PassIndex = i;
				Fx->BeginPass(i);

				for (auto& _RenderEntity : _TargetGroup->second)
				{
					if (_RenderEntity)
					{
						if (_RenderEntity->GetRenderProp().bRender)
						{
							_RenderEntity->RenderDebugBoneImplementation(_ImplInfo);
						}
					}
				}

				Fx->EndPass();
			}

			Fx->End();
		}
	}

	return S_OK;
}

HRESULT Renderer::RenderUI()&
{
	/*Device->SetRenderTarget(0u, BackBuffer);
	if (auto _TargetGroup = RenderEntitys.find(ENGINE::RenderProperty::Order::UI);
		_TargetGroup != std::end(RenderEntitys))
	{
		for (auto& _RenderEntity : _TargetGroup->second)
		{
			if (_RenderEntity)
			{
				if (_RenderEntity->GetRenderProp().bRender)
				{
					_RenderEntity->RenderUI();
				}
			}
		}
	}*/

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
}

HRESULT Renderer::RenderShadowScene(FLight*const Light)
{
	auto ShadowSceneRenderImplementation = [this,&Light](
		const RenderProperty::Order _Order,
		ID3DXEffect* const Fx)
	{
		if (auto _TargetGroup = RenderEntitys.find(_Order);
			_TargetGroup != std::end(RenderEntitys))
		{
			RenderInterface::ImplementationInfo _ImplInfo{};
			_ImplInfo.Fx = Fx;
			_ImplInfo._Device = Device;
			Matrix matViewProj;
			Light->CalculateViewProjection(matViewProj);
			
			D3DXVECTOR4 clipplanes(
				Light->GetNearPlane(),
				Light->GetFarPlane(),
				0, 0);

			if (FAILED(Fx->SetTechnique("variance")))
			{
				PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
			}
			if (FAILED(Fx->SetVector("clipPlanes", &clipplanes)))
			{
				PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
			}
			if (FAILED(Fx->SetBool("isPerspective", Light->IsPerspective())))
			{
				PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
			}
			if (FAILED(Fx->SetVector("lightPos", &Light->GetPosition())))
			{
				PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
			}
			if (FAILED(Fx->SetMatrix("matViewProj", &matViewProj)))
			{
				PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
			}

			uint32 Passes = 0u;
			Fx->Begin(&Passes, NULL);

			for (uint32 i = 0; i < Passes; ++i)
			{
				Fx->BeginPass(i);
				_ImplInfo.PassIndex = i;

				for (auto& _RenderEntity : _TargetGroup->second)
				{
					if (_RenderEntity)
					{
						if (_RenderEntity->GetRenderProp().bRender)
						{
							_RenderEntity->RenderShadowImplementation(_ImplInfo);
						}
					}
				}

				Fx->EndPass();
			}

			Fx->End();
		};
	};

	Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	ShadowSceneRenderImplementation(RenderProperty::Order::Shadow, Shaders["Shadow"]->GetEffect());
	ShadowSceneRenderImplementation(RenderProperty::Order::ShadowSK, Shaders["ShadowSK"]->GetEffect());

	return S_OK;
}

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

	/*if (FAILED(D3DXCreateTextureFromFileA(Device, "../../Media/Textures/brick_normal.jpg", &wood_normal)))
		return false;*/
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

// 포인트 라이트 회전 !
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

void Renderer::TestLightEdit()
{
	static Math::Vector4 originmoondir = { -0.25f,-0.65f, -1, 0 };
	ImGui::SliderFloat3("originmoondir ", (float*)&originmoondir, -1.f, 1.f);
	Math::Vector4 moondir;

	D3DXVec4Normalize((Vector4*)&moondir, (Vector4*)&originmoondir);
	Moonlight->SetPosition((const D3DXVECTOR4&)moondir);

	static bool bMoonLightTarget = false;
	ImGui::Checkbox("bMoonLightTarget", &bMoonLightTarget);
	if (bMoonLightTarget)
	{
		ImGui::SliderFloat3("MoonLightTarget", MoonLightTarget, -5.f, 5.f);
		Vector4 Targetdir = MoonLightTarget - Vector4{ 0, 0, 0, 1 };
		D3DXVec4Normalize(&Targetdir, &Targetdir);
		moondir = { Targetdir.x , Targetdir.y ,Targetdir.z , 0.f };
		Moonlight->SetPosition((const D3DXVECTOR4&)moondir);
	}

	static bool TransformViewSpace = false;
	ImGui::Checkbox("Transform View Space", &TransformViewSpace);
	if (TransformViewSpace)
	{
		// 달빛을 카메라 공간으로 변환 . 
		D3DXVec4Transform((Vector4*)&moondir, (Vector4*)&moondir,
			&_RenderInfo.ViewInverse);
		Moonlight->SetPosition((const D3DXVECTOR4&)moondir);
	}

	ImGui::Checkbox("bCustomEye", &bCurstomEye);
	if (bCurstomEye)
	{
		ImGui::SliderFloat3("CurstomEye", CurstomEye, -5.f, 5.f);
	};
};

