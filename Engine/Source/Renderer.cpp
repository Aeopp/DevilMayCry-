#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "Renderer.h"
#include "GraphicSystem.h"
#include "FMath.hpp"
#include "Color.h"
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

HRESULT DXGenTangentFrame(LPDIRECT3DDEVICE9 device, LPD3DXMESH mesh, LPD3DXMESH* newmesh)
{
	HRESULT hr;
	LPD3DXMESH clonedmesh = NULL;

	D3DVERTEXELEMENT9 decl[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		D3DDECL_END()
	};

	// it is safer to clone
	hr = mesh->CloneMesh(D3DXMESH_MANAGED, decl, device, &clonedmesh);

	if (FAILED(hr))
		return hr;

	if (*newmesh == mesh) {
		mesh->Release();
		*newmesh = NULL;
	}

	hr = D3DXComputeTangentFrameEx(clonedmesh, 
		D3DDECLUSAGE_TEXCOORD, 0,
		D3DDECLUSAGE_TANGENT, 0, 
		D3DDECLUSAGE_BINORMAL, 0, 
		D3DDECLUSAGE_NORMAL, 0,
		0, NULL, 0.01f, 0.25f, 0.01f, newmesh, NULL);

	clonedmesh->Release();
	return hr;
}


HRESULT Renderer::ReadyRenderSystem(LPDIRECT3DDEVICE9 const _pDevice)
{
	m_pDevice = _pDevice;
	SafeAddRef(m_pDevice);
	ReadyRenderTargets();
	ReadyShader("..\\..\\Resource\\Shader");
	ReadyLights();
	TestShaderInit();
	CameraFrustum = std::make_shared<Frustum>();
	CameraFrustum->Initialize(m_pDevice);
	_Quad = std::make_shared<Quad>();
	_Quad->Initialize(m_pDevice);

	camera.SetAspect((float)g_nWndCX/ (float)g_nWndCY);
	camera.SetFov(Math::DegreesToRadians(45));
	camera.SetClipPlanes(0.1f, 50);
	camera.SetZoomLimits(3, 10);
	camera.SetDistance(5);
	camera.SetPosition(0, 0.5f, 0);
	camera.SetOrientation(Math::DegreesToRadians(-135), Math::DegreesToRadians(30), 0);
	
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
	Moonlight->CreateShadowMap(m_pDevice, 512);
	Moonlight->SetProjectionParameters(7.1f, 7.1f, -20.f, +20.f);

	PointLights[0]->CreateShadowMap(m_pDevice, 256);
	PointLights[1]->CreateShadowMap(m_pDevice, 256);
	PointLights[2]->CreateShadowMap(m_pDevice, 256);

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
	static float time = 0;


	Math::Matrix	view, viewinv, proj;
	Math::Matrix	viewproj, viewprojinv;
	// Math::Vector4	eye;

	// NOTE: camera is right-handed
	camera.Animate(0.11f);
	camera.GetViewMatrix(view);
	camera.GetProjectionMatrix(proj);

	// camera.GetEyePosition((Math::Vector3&)eye);

	//Math::MatrixMultiply(viewproj, view, proj);
	//Math::MatrixInverse(viewprojinv, viewproj);


	// setup lights
	PointLights[0]->GetPosition().x = cosf(time * 0.5f) * 2;
	PointLights[0]->GetPosition().z = sinf(time * 0.5f) * cosf(time * 0.5f) * 2;

	PointLights[1]->GetPosition().x = cosf(1.5f * time) * 2;
	PointLights[1]->GetPosition().z = sinf(1 * time) * 2;

	PointLights[2]->GetPosition().x = cosf(0.75f * time) * 1.5f;
	PointLights[2]->GetPosition().z = sinf(1.5f * time) * 1.5f;

	
	std::memcpy(&view, &CurrentRenderInfo.View, sizeof(Matrix));
	std::memcpy(&viewinv, &CurrentRenderInfo.ViewInverse, sizeof(Matrix));
	std::memcpy(&proj, &CurrentRenderInfo.Projection, sizeof(Matrix));
	std::memcpy(&viewproj, &CurrentRenderInfo.ViewProjection, sizeof(Matrix));
	std::memcpy(&viewprojinv, &CurrentRenderInfo.ViewProjectionInverse, sizeof(Matrix));
	// eye = Math::Vector4{ CurrentRenderInfo.CameraLocation.x, CurrentRenderInfo.CameraLocation.y , CurrentRenderInfo.CameraLocation.z , 1.f };

	// calculate moonlight direction (let y stay in world space, so we can see shadow)
	static Math::Vector4 originmoondir = { -0.25f,-0.65f, -1, 0 };

	// Math::MatrixInverse(viewinv, view);
	ImGui::SliderFloat3("originmoondir ", (float*)&originmoondir, -1.f, 1.f);
	/*Math::Vec3TransformNormal((Math::Vector3&)moondir, (const Math::Vector3&)moondir, viewinv);
	Math::Vec3Normalize((Math::Vector3&)moondir, (const Math::Vector3&)moondir);*/

	Math::Vector4 moondir;

	D3DXVec4Normalize((Vector4*)&moondir, (Vector4*)&originmoondir);
// 	 moondir.y = 0.65f;
	Moonlight->SetPosition((const D3DXVECTOR4&)moondir);

	static bool bMoonLightTarget = false;
	ImGui::Checkbox("bMoonLightTarget", &bMoonLightTarget);
	if (bMoonLightTarget)
	{
		
		ImGui::SliderFloat3("MoonLightTarget", MoonLightTarget, -5.f, 5.f);
		Vector4 Targetdir  = MoonLightTarget - Vector4{ 0, 0, 0, 1 };
		D3DXVec4Normalize(&Targetdir, &Targetdir);
		moondir = { Targetdir.x , Targetdir.y ,Targetdir.z , 0.f };
		Moonlight->SetPosition((const D3DXVECTOR4&)moondir);
	}
	
	static bool TransformViewSpace = false;
	if (ImGui::Checkbox("Transform View Space", &TransformViewSpace))
	{
		
	}

	if (TransformViewSpace)
	{
		// 달빛을 카메라 공간으로 변환 . 
		D3DXVec4Transform((Vector4*)&moondir, (Vector4*)&moondir,
			&CurrentRenderInfo.ViewInverse);
		Moonlight->SetPosition((const D3DXVECTOR4&)moondir);
	}

	ImGui::Checkbox("bCustomEye", &bCurstomEye);
	if (bCurstomEye)
	{
		ImGui::SliderFloat3("CurstomEye", CurstomEye, -5.f, 5.f);
	}

	

	
	

	auto device = m_pDevice;
	D3DXVECTOR4			pixelsize(1, 1, 1, 1);

	if (SUCCEEDED(device->BeginScene())) {
		D3DVIEWPORT9 oldviewport;
		D3DVIEWPORT9 viewport;
		LPDIRECT3DSURFACE9 backbuffer = nullptr;

		device->GetRenderTarget(0, &backbuffer);
		device->GetViewport(&oldviewport);

		// STEP 0: render shadow maps
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		{
			RenderShadowMaps();
		}

		// STEP 1: g-buffer pass
		{
			RenderGBuffer(viewproj);
		}
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// STEP 2: deferred shading
		pixelsize.x = 1.0f / (float)oldviewport.Width;
		pixelsize.y = -1.0f / (float)oldviewport.Height;

		device->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1);
		device->SetRenderState(D3DRS_ZENABLE, FALSE);

		DeferredShading(view, proj, viewprojinv, 
			Vector4{CurrentRenderInfo.CameraLocation.x,
			CurrentRenderInfo.CameraLocation.y ,
			CurrentRenderInfo.CameraLocation.z ,1.f} );

		// STEP 3: render sky
		device->SetRenderTarget(0, backbuffer);
		device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);

		backbuffer->Release();

		auto screenquad = Shaders["ScreenQuad"]->GetEffect();

		screenquad->SetTechnique("screenquad");
		screenquad->Begin(NULL, 0);
		screenquad->BeginPass(0);
		{
			device->SetTexture(0, sky);
			_Quad->Render(m_pDevice); 
			//device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, DXScreenQuadVertices, 6 * sizeof(float));
		}
		screenquad->EndPass();
		screenquad->End();

		// STEP 4: tone mapping
		device->SetRenderState(D3DRS_SRGBWRITEENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		auto tonemap = Shaders["ToneMap"]->GetEffect();

		auto scenetarget = RenderTargets["SceneTarget"]->GetTexture();

		tonemap->SetTechnique("tonemap");
		tonemap->SetVector("pixelSize", &pixelsize);

		tonemap->Begin(NULL, 0);
		tonemap->BeginPass(0);
		{
			device->SetTexture(0, scenetarget);
			_Quad->Render(m_pDevice);  
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
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		device->SetRenderState(D3DRS_ZENABLE, TRUE);
		device->SetViewport(&oldviewport);

		device->EndScene();
	}

	time += TimeSystem::GetInstance()->DeltaTime();


	// GraphicSystem::GetInstance()->Begin();
	// RenderImplementation();
	RenderTargetDebugRender();
	ImguiRender();
	// GraphicSystem::GetInstance()->End();
	RenderEnd();
	device->Present(NULL, NULL, NULL, NULL);

	return S_OK;
}

void Renderer::Editor()&
{
	if (ImGui::Begin("Render Editor"))
	{
		if (ImGui::TreeNode("Lights"))
		{
			uint32 Idx = 0u;
			for (auto& _Light : PointLights)
			{
				_Light->Edit(Idx);
				++Idx;
			}
			Moonlight->Edit(Idx);
			ImGui::TreePop();
		}
	
		ImGui::End();
	}
	
}

void Renderer::RenderReady()&
{
	RenderReadyEntitys();

	Matrix CameraView, CameraProjection;
	m_pDevice->GetTransform(D3DTS_VIEW, &CameraView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &CameraProjection);
	
	Matrix Ortho;
	D3DXMatrixOrthoLH(&Ortho, g_nWndCX,g_nWndCY, 0.0f, 1.f);

	CurrentRenderInfo.View = (CameraView);
	CurrentRenderInfo.Projection = (CameraProjection);
	CurrentRenderInfo.ViewInverse = FMath::Inverse(CurrentRenderInfo.View);
	CurrentRenderInfo.ProjectionInverse =FMath::Inverse(CurrentRenderInfo.Projection);
	CurrentRenderInfo.ViewProjection = 
		CameraView * CameraProjection;
	CurrentRenderInfo.ViewProjectionInverse = FMath::Inverse(CurrentRenderInfo.ViewProjection);
	CurrentRenderInfo.CameraLocation =
	{ CurrentRenderInfo.ViewInverse._41  , CurrentRenderInfo.ViewInverse._42,CurrentRenderInfo.ViewInverse._43,1.f };
	CurrentRenderInfo.Ortho = Ortho;


	
	static float time = 0.0f;
	time += TimeSystem::GetInstance()->DeltaTime();
	//Vector4 moondir = { +0.25f,0.65f, -1,0 };
	//// 달빛을 카메라 공간으로 변환 . 
	//D3DXVec4Transform(&moondir, &moondir,
	//	&CurrentRenderInfo.ViewInverse);
	//Vector3 moondir3 = Vector3{ moondir.x , moondir.y, moondir.z };
	//D3DXVec3TransformNormal(&moondir3, &moondir3, &CurrentRenderInfo.ViewInverse);
	//D3DXVec3Normalize(&moondir3, &moondir3);
	//moondir3.y = 0.65f;
	//moondir = { moondir3.x,moondir3.y,moondir3.z ,0.0f };

	// Moonlight->SetPosition(moondir);

	PointLights[0]->GetPosition().x = std::cosf(time * 0.5f) * 2.f;

	PointLights[0]->GetPosition().z = std::sinf(time * 0.5f) *
		std::cosf(time * 0.5f) * 2.f;

	PointLights[1]->GetPosition().x = std::cosf(1.5f * time) * 2.f;
	PointLights[1]->GetPosition().z = std::sinf(1.f * time) * 2.f;

	PointLights[2]->GetPosition().x = std::cosf(0.75f * time) * 1.5f;
	PointLights[2]->GetPosition().z = std::sinf(1.5f * time) * 1.5f;

	camera.Update(TimeSystem::GetInstance()->DeltaTime());


	Culling();

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
	CameraFrustum->Make(CurrentRenderInfo.ViewInverse, CurrentRenderInfo.Projection);
	// 절두체에서 검사해서 Entity 그룹에서 지우기 ....
}

void Renderer::RenderEnd()&
{
	RenderEntityClear();
}

void Renderer::RenderEntityClear()&
{
	RenderEntitys.clear();
}

HRESULT Renderer::RenderImplementation()&
{
	//m_pDevice->GetRenderTarget(0u, &BackBuffer);
	//m_pDevice->GetViewport(&BackBufViewport);
	//{
	//	RenderShadows();
	//	RenderGBuffer();
	//    RenderDeferredShading();
	//	m_pDevice->SetRenderTarget(0u, BackBuffer);
	//	m_pDevice->Clear(
	//		0u, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0, 1.0f, 0);
	//	m_pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	//	auto*const screenquad = Shaders["ScreenQuad"]->GetEffect();  
	//	screenquad->SetTechnique("screenquad");
	//	screenquad->Begin(NULL, 0);
	//	screenquad->BeginPass(0);
	//	{
	//		m_pDevice->SetTexture(0, sky);
	//		_Quad->Render(m_pDevice, 1.f, 1.f, screenquad);
	//	}
	//	screenquad->EndPass();
	//	screenquad->End();

	//	m_pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, TRUE);
	//	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//	auto* const tonemap = Shaders["ToneMap"]->GetEffect();
	//	tonemap->SetTechnique("tonemap");
	//	Vector4 pixelsize{};
	//	pixelsize.x = 1.0f / static_cast<float> (BackBufViewport.Width);
	//	pixelsize.y = -1.0f / static_cast<float> (BackBufViewport.Height);
	//	tonemap->SetVector("pixelSize", &pixelsize);

	//	tonemap->Begin(NULL, 0);
	//	tonemap->BeginPass(0);
	//	{
	//		m_pDevice->SetTexture(0, RenderTargets["SceneTarget"]->GetTexture());
	//		_Quad->Render(m_pDevice, 1.f, 1.f, tonemap);
	//	}
	//	tonemap->EndPass();
	//	tonemap->End();

	//	D3DVIEWPORT9 viewport = BackBufViewport;
	//	viewport.Width = 1024;
	//	viewport.Height = 256;
	//	viewport.X = viewport.Y = 10;


	//	m_pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	//	m_pDevice->SetViewport(&viewport);


	//	/*screenquad->Begin(NULL, 0);
	//	screenquad->BeginPass(0);
	//	{
	//		m_pDevice->SetTexture()
	//	}*/


	//	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//	m_pDevice->SetViewport(&BackBufViewport);
	//	/*RenderForwardAlphaBlend();
	//	RenderAlphaBlendEffect();
	//	RenderUI();
	//	RenderDebug();
	//	RenderDebugBone();*/
	//}
	//// m_pDevice->SetRenderTarget(0u, BackBuffer);
	//BackBuffer->Release();

	return S_OK;
}

void Renderer::RenderShadowMaps()
{
	// moonlight
	auto device = m_pDevice;

	auto shadowmap = Shaders["Shadow"]->GetEffect();
	auto blur = Shaders["Blur"]->GetEffect();

	Moonlight->RenderShadowMap(device, [&](FLight* light) {
		D3DXMATRIX viewproj;
		D3DXVECTOR4 clipplanes(light->GetNearPlane(), light->GetFarPlane(), 0, 0);

		light->CalculateViewProjection(viewproj);

		shadowmap->SetTechnique("variance");
		shadowmap->SetVector("clipPlanes", &clipplanes);
		shadowmap->SetBool("isPerspective", FALSE);

		device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
		RenderScene(shadowmap, viewproj);
		});

	Moonlight->BlurShadowMap(device, [&](FLight* light) {
		D3DXVECTOR4 pixelsize(1.0f / light->GetShadowMapSize(),
			1.0f / light->GetShadowMapSize(), 0, 0);
		D3DXVECTOR4 texelsize = 4.0f * pixelsize;	// make it more blurry

		device->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1);
		device->SetRenderState(D3DRS_ZENABLE, FALSE);

		blur->SetTechnique("boxblur3x3");
		blur->SetVector("pixelSize", &pixelsize);
		blur->SetVector("texelSize", &texelsize);

		blur->Begin(NULL, 0);
		blur->BeginPass(0);
		{
			_Quad->Render(m_pDevice);
			// device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, DXScreenQuadVertices, 6 * sizeof(float));
		}
		blur->EndPass();
		blur->End();

		device->SetRenderState(D3DRS_ZENABLE, TRUE);
		});

	// point lights
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	shadowmap->SetBool("isPerspective", TRUE);

	for (int i = 0; i < 3; ++i) {
		PointLights[i]->RenderShadowMap(device, [&](FLight* light) {
			D3DXMATRIX viewproj;
			D3DXVECTOR4 clipplanes(light->GetNearPlane(), light->GetFarPlane(), 0, 0);

			light->CalculateViewProjection(viewproj);

			shadowmap->SetTechnique("variance");
			shadowmap->SetVector("lightPos", &light->GetPosition());
			shadowmap->SetVector("clipPlanes", &clipplanes);

			device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			RenderScene(shadowmap, viewproj);
			});
	}

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
void Renderer::RenderGBuffer(const Math::Matrix& viewproj)
{
	auto* const device = m_pDevice;

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
		RenderScene(gbuffer, (const D3DXMATRIX&)viewproj);
	}
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	device->SetRenderTarget(1, NULL);
	device->SetRenderTarget(2, NULL);
}

void Renderer::DeferredShading(const Math::Matrix& view, const Math::Matrix& proj, 
	const Math::Matrix& viewprojinv, 
	const Vector4& eye)
{
	RECT scissorrect;

	auto device = m_pDevice;

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
	deferred->SetMatrix("matViewProjInv", (D3DXMATRIX*)&viewprojinv);
	deferred->SetVector("pixelSize", &pixelsize);

	if (bCurstomEye)
	{
		deferred->SetVector("eyePos", &CurstomEye);
	}
	else
	{
		deferred->SetVector("eyePos", &eye);
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
			_Quad->Render(m_pDevice);

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
				Math::Matrix viewtranspose, projtranspose; 
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
				_Quad->Render(m_pDevice);				
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
		m_pDevice->SetTexture(0, marble);
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
		m_pDevice->SetTexture(0, wood);
		m_pDevice->SetTexture(1, wood_normal);

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
//	/*m_pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
//
//	m_pDevice->SetRenderTarget(0u, RenderTargets["ALBM"]->GetSurface());
//	m_pDevice->SetRenderTarget(1u, RenderTargets["NRMR"]->GetSurface());
//	m_pDevice->SetRenderTarget(2u, RenderTargets["Depth"]->GetSurface());
//
//	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
//	m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
//	m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
//
//	m_pDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//	m_pDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//	m_pDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
//	m_pDevice->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
//	m_pDevice->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
//
//	m_pDevice->Clear(0u, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
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
//				_ImplInfo._Device = m_pDevice;
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
//	m_pDevice->SetRenderTarget(1u, nullptr);
//	m_pDevice->SetRenderTarget(2u, nullptr);
//
//	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//
//	return S_OK;*/
//}


HRESULT Renderer::RenderDeferredShading()&
{
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	Vector4 pixelsize{};
	pixelsize.x = 1.0f / static_cast<float> (BackBufViewport.Width);
	pixelsize.y = -1.0f / static_cast<float> (BackBufViewport.Height);

	m_pDevice->SetFVF(D3DFVF_XYZW |D3DFVF_TEX1);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pDevice->SetRenderTarget(0u, RenderTargets["SceneTarget"]->GetSurface());
	m_pDevice->Clear(0,NULL,D3DCLEAR_TARGET, 0, 1.0f, 0);

	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	m_pDevice->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, TRUE);

	for (int i = 1; i < 5; ++i) {
		m_pDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		m_pDevice->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_pDevice->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	}

	m_pDevice->SetTexture(0, RenderTargets["ALBM"]->GetTexture());
	m_pDevice->SetTexture(1, RenderTargets["NRMR"]->GetTexture());
	m_pDevice->SetTexture(2, RenderTargets["Depth"]->GetTexture());

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	auto* const Fx = Shaders["DeferredShading"]->GetEffect();
	Fx->SetTechnique("deferred");
	Fx->SetMatrix("matViewProjInv", (D3DXMATRIX*)&CurrentRenderInfo.ViewProjectionInverse);
	Fx->SetVector("pixelSize", &pixelsize);

	if (bCurstomEye)
	{
		Fx->SetVector("eyePos",&CurstomEye);
	}
	else
	{
		Fx->SetVector("eyePos", (D3DXVECTOR4*)&CurrentRenderInfo.CameraLocation);
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
		

		m_pDevice->SetTexture(3, Moonlight->GetShadowMap());
		Fx->CommitChanges();


		_Quad->Render(m_pDevice, 1.f, 1.f, Fx);
	}

	m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	RECT scissorrect;
	static constexpr float POINT_LIGHT_RADIUS = 3.0f;
	for (int i = 0; i < 3; ++i)
	{
		clipplanes.x = PointLights[i]->GetNearPlane();
		clipplanes.y = PointLights[i]->GetFarPlane();

		PointLights[i]->CalculateScissorRect
		(
			scissorrect, (const D3DXMATRIX&)CurrentRenderInfo.View,
			(const D3DXMATRIX&)CurrentRenderInfo.Projection,
			POINT_LIGHT_RADIUS, g_nWndCX, g_nWndCY);

		m_pDevice->SetScissorRect(&scissorrect);

		Fx->SetVector("clipPlanes", &clipplanes);
		Fx->SetVector("lightColor", (D3DXVECTOR4*)&PointLights[i]->GetColor());
		Fx->SetVector("lightPos", &PointLights[i]->GetPosition());
		Fx->SetFloat("specularPower", 80.0f);
		Fx->SetFloat("lightRadius", POINT_LIGHT_RADIUS);
		m_pDevice->SetTexture(4, PointLights[i]->GetCubeShadowMap());
		Fx->CommitChanges();
		_Quad->Render(m_pDevice, 1.f, 1.f, Fx);
	}

	m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	Fx->EndPass();
	Fx->End();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, FALSE);

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

	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	auto*const screenquad = Shaders["ScreenQuad"]->GetEffect();
	screenquad->SetTechnique("rect");
	screenquad->Begin(NULL, 0);
	screenquad->BeginPass(0);
	{
		m_pDevice->DrawIndexedPrimitiveUP(D3DPT_LINESTRIP, 0, 4, 4, rectindices, D3DFMT_INDEX16, rectvertices, 6 * sizeof(float));
	}
	screenquad->EndPass();
	screenquad->End();

	m_pDevice->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1);

	return S_OK;
}

HRESULT Renderer::RenderForwardAlphaBlend()&
{
	m_pDevice->SetRenderTarget(0u, BackBuffer);

	auto ForwardAlphaBlendImplementation = [this](const RenderProperty::Order& _Order , ID3DXEffect*const Fx) 
	{
		if (auto _TargetGroup = RenderEntitys.find(_Order);
			_TargetGroup != std::end(RenderEntitys))
		{
			Fx->SetMatrix("View", &CurrentRenderInfo.View);
			Fx->SetMatrix("Projection", &CurrentRenderInfo.Projection);

			RenderInterface::ImplementationInfo _ImplInfo{};
			_ImplInfo.Fx = Fx;
			_ImplInfo._Device = m_pDevice;

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
	//	m_pDevice->SetRenderTarget(0u, BackBuffer);

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
		m_pDevice->SetRenderTarget(0u, BackBuffer);

		auto DebugRenderImplementation = [this](const RenderProperty::Order & _Order , ID3DXEffect*const Fx) 
		{
			if (auto _TargetGroup = RenderEntitys.find(_Order);
				_TargetGroup != std::end(RenderEntitys))
			{
				Fx->SetMatrix("View",
					&CurrentRenderInfo.View);
				Fx->SetMatrix("Projection", &CurrentRenderInfo.Projection);

				RenderInterface::ImplementationInfo _ImplInfo{};
				_ImplInfo.Fx = Fx;
				_ImplInfo._Device = m_pDevice;
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
			m_pDevice->SetRenderTarget(0u, BackBuffer);
			auto* const Fx = Shaders["DebugBone"]->GetEffect();

			Fx->SetMatrix("View",
				&CurrentRenderInfo.View);
			Fx->SetMatrix("Projection", &CurrentRenderInfo.Projection);
			const Matrix ScaleOffset = FMath::Scale({ 0.01 ,0.01,0.01 });
			Fx->SetMatrix("ScaleOffset", &ScaleOffset);

			RenderInterface::ImplementationInfo _ImplInfo{};
			_ImplInfo.Fx = Fx;
			_ImplInfo._Device = m_pDevice;
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
	/*m_pDevice->SetRenderTarget(0u, BackBuffer);
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
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

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
			_ImplInfo._Device = m_pDevice;
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

	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	ShadowSceneRenderImplementation(RenderProperty::Order::Shadow, Shaders["Shadow"]->GetEffect());
	ShadowSceneRenderImplementation(RenderProperty::Order::ShadowSK, Shaders["ShadowSK"]->GetEffect());

	return S_OK;
}

void Renderer::RenderTargetDebugRender()&
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
}

bool Renderer::TestShaderInit()
{
	//// Shader Test ... 
	if (FAILED(D3DXLoadMeshFromX(L"../../Media/MeshesDX/box.x", D3DXMESH_MANAGED, m_pDevice, NULL, NULL, NULL, NULL, &box)))
		return false;

	if (FAILED(DXGenTangentFrame(m_pDevice, box, &box)))
		return false;

	if (FAILED(D3DXLoadMeshFromX(L"../../Media/MeshesDX/skullocc3.x", D3DXMESH_MANAGED, m_pDevice, NULL, NULL, NULL, NULL, &skull)))
		return false;

	if (FAILED(D3DXCreateTextureFromFileA(m_pDevice, "../../Media/Textures/marble.dds", &marble)))
		return false;

	if (FAILED(D3DXCreateTextureFromFileA(m_pDevice, "../../Media/Textures/wood2.jpg", &wood)))
		return false;

	/*if (FAILED(D3DXCreateTextureFromFileA(m_pDevice, "../../Media/Textures/brick_normal.jpg", &wood_normal)))
		return false;*/
	if (FAILED(D3DXCreateTextureFromFileA(m_pDevice, "../../Media/Textures/wood2_normal.tga", &wood_normal)))
		return false;

	if (FAILED(D3DXCreateTextureFromFileA(m_pDevice, "../../Media/Textures/static_sky.jpg", &sky)))
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
};
