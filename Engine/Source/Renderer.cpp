#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "Renderer.h"
#include "GraphicSystem.h"
#include "FMath.hpp"
#include <d3d9.h>
#include <d3dx9.h>
#include "Resources.h"
#include "TimeSystem.h"
#include "Vertexs.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(Renderer)

Renderer::Renderer(){}

void Renderer::Free()
{
	CameraFrustum.Release();
	SceneTarget.Release();
	ALBM.Release();
	NRMR.Release();
	Depth.Release();
	_Quad.Release();
	// _ShaderTester.Clear();
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
	CameraFrustum.Initialize(m_pDevice);
	
	_Quad.Initialize(m_pDevice);
	ReadyShader();
	// _ShaderTester.Initialize();


	// 테스트 쉐이더 ... 
	TestShaderInit();

	return S_OK;
}

void Renderer::ReadyShader()
{
	ForwardAlphaBlend = Resources::Load<Shader>(
		L"..\\..\\Resource\\Shader\\ForwardAlphaBlend.hlsl"
		);

	ForwardAlphaBlendSK = Resources::Load<Shader>(
		L"..\\..\\Resource\\Shader\\ForwardAlphaBlendSK.hlsl"
		);

	GBuffer = Resources::Load<Shader>(
		L"..\\..\\Resource\\Shader\\GBuffer.hlsl"
		);

	GBufferSK = Resources::Load<Shader>(
		L"..\\..\\Resource\\Shader\\GBufferSK.hlsl"
		);

	Debug = Resources::Load<Shader>(
		L"..\\..\\Resource\\Shader\\Debug.hlsl"
		);

	DebugSK = Resources::Load<Shader>(
		L"..\\..\\Resource\\Shader\\DebugSK.hlsl"
		);

	DebugBone = Resources::Load<Shader>(
		L"..\\..\\Resource\\Shader\\DebugBone.hlsl"
		);

	RTDebug = Resources::Load<ENGINE::Shader>(
		L"..\\..\\Resource\\Shader\\ScreenQuad.hlsl");

	// AlphaBlendEffect = ? ? ;
	// UI = ??  ;
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
		RenderTarget::Info InitInfo;
		InitInfo.Width = g_nWndCX;
		InitInfo.Height = g_nWndCY ;
		InitInfo.Levels = 1;
		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
		InitInfo.Format = D3DFMT_A16B16G16R16F;
		InitInfo._D3DPool = D3DPOOL_DEFAULT;
		SceneTarget.Initialize(InitInfo);
		SceneTarget.DebugBufferInitialize(
			{ InitX,InitY },
			RenderTargetDebugRenderSize);
	}

	{
		RenderTarget::Info InitInfo;
		InitInfo.Width = g_nWndCX ;
		InitInfo.Height = g_nWndCY ;
		InitInfo.Levels = 1;
		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
		InitInfo.Format = D3DFMT_A8R8G8B8;
		InitInfo._D3DPool = D3DPOOL_DEFAULT;
		ALBM.Initialize(InitInfo);
		ALBM.DebugBufferInitialize(
			{ InitX,InitY + (YOffset  *1.f ) + Interval },
			RenderTargetDebugRenderSize);
	}

	{
		RenderTarget::Info InitInfo;
		InitInfo.Width = g_nWndCX; 
		InitInfo.Height = g_nWndCY;
		InitInfo.Levels = 1;
		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
		InitInfo.Format = D3DFMT_A8R8G8B8;
		InitInfo._D3DPool = D3DPOOL_DEFAULT;
		NRMR.Initialize(InitInfo);
		NRMR.DebugBufferInitialize(
			{ InitX,InitY + (YOffset * 2.f) + Interval },
			RenderTargetDebugRenderSize);
	}

	{
		RenderTarget::Info InitInfo;
		InitInfo.Width = g_nWndCX; 
		InitInfo.Height = g_nWndCY;
		InitInfo.Levels = 1;
		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
		InitInfo.Format = D3DFMT_R32F;
		InitInfo._D3DPool = D3DPOOL_DEFAULT;
		Depth.Initialize(InitInfo);
		Depth.DebugBufferInitialize(
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
	// 기본 렌더...
	GraphicSystem::GetInstance()->Begin();
	// RenderImplementation();
	TestShaderRender(TimeSystem::GetInstance()->DeltaTime());
	// _ShaderTester.Render();
	RenderTargetDebugRender();
	ImguiRender();
	GraphicSystem::GetInstance()->End();
	RenderEnd();
	// 기본 렌더 ...


	return S_OK;
}

void Renderer::RenderReady()&
{
	RenderReadyEntitys();
	
	Matrix CameraView, CameraProjection;
	m_pDevice->GetTransform(D3DTS_VIEW, &CameraView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &CameraProjection);
	
	Matrix Ortho;
	D3DXMatrixOrthoLH(&Ortho, g_nWndCX,g_nWndCY, 0.0f, 1.f);
	CurrentRenderInfo.CameraView = CameraView;
	CurrentRenderInfo.CameraProjection = CameraProjection;
	CurrentRenderInfo.ViewInverse = FMath::Inverse(CameraView);
	CurrentRenderInfo.CameraLocation =
	{ CurrentRenderInfo.ViewInverse._41  , CurrentRenderInfo.ViewInverse._42,CurrentRenderInfo.ViewInverse._43,1.f };
	CurrentRenderInfo.Ortho = Ortho;
	
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
	CameraFrustum.Make(CurrentRenderInfo.ViewInverse, CurrentRenderInfo.CameraProjection);
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
	m_pDevice->GetRenderTarget(0u, &BackBuffer);
	m_pDevice->GetViewport(&OldViewport);
	{
		RenderGBuffer();
		RenderForwardAlphaBlend();
		RenderAlphaBlendEffect();
		RenderUI();
		RenderDebug();
		RenderDebugBone();
	}
	m_pDevice->SetRenderTarget(0u, BackBuffer);
	BackBuffer->Release();

	return S_OK;
}

HRESULT Renderer::RenderGBuffer()&
{
	m_pDevice->SetRenderTarget(0u, ALBM.GetSurface());
	m_pDevice->SetRenderTarget(1u, NRMR.GetSurface());
	m_pDevice->SetRenderTarget(2u, Depth.GetSurface());

	m_pDevice->Clear(0u, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	auto GBufferRenderImplementation = [this](
		const RenderProperty::Order _Order,
		ID3DXEffect*const Fx
		) 
	{
			if (auto _TargetGroup = RenderEntitys.find(_Order);
				_TargetGroup != std::end(RenderEntitys))
			{
				RenderInterface::ImplementationInfo _ImplInfo{};
				_ImplInfo.Fx = Fx;

				Fx->SetMatrix("View", &CurrentRenderInfo.CameraView);
				Fx->SetMatrix("Projection", &CurrentRenderInfo.CameraProjection);

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
								_RenderEntity->RenderGBufferImplementation(_ImplInfo);
							}
						}
					}

					Fx->EndPass();
				}

				Fx->End();
			};
	};

	GBufferRenderImplementation(RenderProperty::Order::GBuffer,GBuffer->GetEffect () );
	GBufferRenderImplementation(RenderProperty::Order::GBufferSK, GBufferSK->GetEffect());

	m_pDevice->SetRenderTarget(0u, nullptr);
	m_pDevice->SetRenderTarget(1u, nullptr);
	m_pDevice->SetRenderTarget(2u, nullptr);

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
			Fx->SetMatrix("View", &CurrentRenderInfo.CameraView);
			Fx->SetMatrix("Projection", &CurrentRenderInfo.CameraProjection);

			RenderInterface::ImplementationInfo _ImplInfo{};
			_ImplInfo.Fx = Fx;

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
	
	ForwardAlphaBlendImplementation(RenderProperty::Order::ForwardAlphaBlend, ForwardAlphaBlend->GetEffect());
	ForwardAlphaBlendImplementation(RenderProperty::Order::ForwardAlphaBlendSK, ForwardAlphaBlendSK->GetEffect () );

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
					&CurrentRenderInfo.CameraView);
				Fx->SetMatrix("Projection", &CurrentRenderInfo.CameraProjection);

				RenderInterface::ImplementationInfo _ImplInfo{};
				_ImplInfo.Fx = Fx;

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


		DebugRenderImplementation(RenderProperty::Order::Debug, Debug->GetEffect () );
		DebugRenderImplementation(RenderProperty::Order::DebugBone,DebugSK->GetEffect () );
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

			auto* const Fx = DebugBone->GetEffect();

			Fx->SetMatrix("View",
				&CurrentRenderInfo.CameraView);
			Fx->SetMatrix("Projection", &CurrentRenderInfo.CameraProjection);
			const Matrix ScaleOffset = FMath::Scale({ 0.01 ,0.01,0.01 });
			Fx->SetMatrix("ScaleOffset", &ScaleOffset);

			RenderInterface::ImplementationInfo _ImplInfo{};
			_ImplInfo.Fx = Fx;

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
								RenderDebugBoneImplementation(_ImplInfo);
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

	return S_OK;
}

HRESULT Renderer::RenderShadowScene()
{
	auto GBufferRenderImplementation = [this](
		const RenderProperty::Order _Order,
		ID3DXEffect* const Fx
		)
	{
		if (auto _TargetGroup = RenderEntitys.find(_Order);
			_TargetGroup != std::end(RenderEntitys))
		{
			RenderInterface::ImplementationInfo _ImplInfo{};
			_ImplInfo.Fx = Fx;

			Fx->SetMatrix("View", &CurrentRenderInfo.CameraView);
			Fx->SetMatrix("Projection", &CurrentRenderInfo.CameraProjection);

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
							_RenderEntity->RenderGBufferImplementation(_ImplInfo);
						}
					}
				}

				Fx->EndPass();
			}

			Fx->End();
		};
	};


	GBufferRenderImplementation(RenderProperty::Order::GBuffer, GBuffer->GetEffect());
	GBufferRenderImplementation(RenderProperty::Order::GBufferSK, GBufferSK->GetEffect());

	return S_OK;
}

void Renderer::RenderTargetDebugRender()&
{
	if (RTDebug && g_bRenderTargetVisible)
	{
		auto* const RTFx = RTDebug->GetEffect();
		SceneTarget.DebugRender(RTFx);
		ALBM.DebugRender(RTFx);
		NRMR.DebugRender(RTFx);
		Depth.DebugRender(RTFx);
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

	if (FAILED(D3DXCreateTextureFromFileA(m_pDevice, "../../Media/Textures/wood2_normal.tga", &wood_normal)))
		return false;

	if (FAILED(D3DXCreateTextureFromFileA(m_pDevice, "../../Media/Textures/static_sky.jpg", &sky)))
		return false;

	ShadowMap = Resources::Load<Shader>
		(L"..\\..\\Resource\\Shader\\ShadowMap.hlsl");
	Blur = Resources::Load<Shader>
		(L"..\\..\\Resource\\Shader\\Blur.hlsl");

	// 달빛
	Moonlight = new FLight(FLight::Type::Directional,
		{ 0,0,0,0 }, (const D3DXCOLOR&)FMath::Color::sRGBToLinear(250, 250, 250));

	// 빨간색 포인트 라이트. 
	Pointlight[0] = new FLight(FLight::Type::Point, { 1.5f,0.5f, 0.0f ,1 },
		{ 1,0,0,1 });

	Pointlight[1] = new FLight(FLight::Type::Point, { -0.7f , 0.5f , 1.2f , 1.f },
		{ 0,1,0,1 });

	Pointlight[2] = new FLight(FLight::Type::Point, { 0.0f,0.5f,0.0f,1 },
		{ 0,0,1,1 });

	// 그림자맵 512 로 생성
	Moonlight->CreateShadowMap(m_pDevice, 512);
	Moonlight->SetProjectionParameters(7.1f, 7.1f, -5.f, +5.f);

	Pointlight[0]->CreateShadowMap(m_pDevice, 256);
	Pointlight[1]->CreateShadowMap(m_pDevice, 256);
	Pointlight[2]->CreateShadowMap(m_pDevice, 256);

	Pointlight[0]->SetProjectionParameters(0, 0, 0.1f, 10.0f);
	Pointlight[1]->SetProjectionParameters(0, 0, 0.1f, 10.0f);
	Pointlight[2]->SetProjectionParameters(0, 0, 0.1f, 10.0f);

}

void Renderer::TestShaderRelease()
{

}

void Renderer::TestShaderRender(const float elapsedtime)
{
	static float time = 0.0f;

	Vector4 moondir = { -0.25f,0.65f, -1,0 };
	
	// 달빛을 카메라 공간으로 변환 . 
	D3DXVec4Transform(&moondir, &moondir,
		&CurrentRenderInfo.ViewInverse);
	Vector3 moondir3 = Vector3{ moondir.x , moondir.y, moondir.z };
	D3DXVec3Normalize(&moondir3, &moondir3);
	moondir3.y = 0.65f;
	moondir = { moondir3 .x,moondir3 .y,moondir3 .z ,0.0f};

	Pointlight[0]->GetPosition().x = std::cosf(time * 0.5f) * 2.f;

	Pointlight[0]->GetPosition().z = std::sinf(time * 0.5f) *
		std::cosf(time * 0.5f) * 2.f;

	Pointlight[1]->GetPosition().x = std::cosf(1.5f * time) * 2.f;
	Pointlight[1]->GetPosition().z = std::sinf(1.f * time) * 2.f;

	Pointlight[2]->GetPosition().x = 
		std::cosf(0.75f * time) * 1.5f;
	Pointlight[2]->GetPosition().z = std::sinf(1.5f * time) * 1.5f;


	D3DVIEWPORT9 oldviewport;
	D3DVIEWPORT9 viewport;
	LPDIRECT3DSURFACE9 backbuffer = nullptr;

	m_pDevice->GetRenderTarget(0, &backbuffer);
	m_pDevice->GetViewport(&oldviewport);

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	RenderShadowMaps();
	RenderGBuffer();
};


void Renderer::RenderShadowMaps()
{
	Moonlight->RenderShadowMap(
		m_pDevice, [&](FLight* Light)
		{
			Matrix ViewProjection;
			//                  Near ~ Far
			const Vector4 clipplanes(
				Light->GetNearPlane(), Light->GetFarPlane(), 0, 0);
			// 광원기준 뷰투영 계산 . 
			Light->CalculateViewProjection(ViewProjection);

			auto * const Fx = ShadowMap->GetEffect();  

			if (FAILED(Fx->SetTechnique("Variance")))
			{
				PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
			}
			if (FAILED(Fx->SetVector("ClipPlanes",
				&clipplanes)))
			{
				PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
			}

			Fx->SetBool("IsPerspective", FALSE);

			m_pDevice->Clear(0, NULL,
				D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
				0, 1.0f, 0);
	  		RenderScene(ShadowMap, viewproj);
		});

	Moonlight->BlurShadowMap(
		m_pDevice, [&](FLight* Light)
		{
			const D3DXVECTOR4 PixelSize
				(1.0f / Light->GetShadowMapSize(),
		         1.0f / Light->GetShadowMapSize(),
			     0, 0);

			// 4.0f 계수로 블러의 강도를 통제한다. 
			const D3DXVECTOR4 TexelSize = 4.0f * PixelSize;

			m_pDevice->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1);
			// 전체화면 블러이므로 ZEnable 의미가 없음 . 
			m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

			auto* const Fx = Blur->GetEffect();

			Fx->SetTechnique("BoxBlur3x3");
			Fx->SetVector("PixelSize", &PixelSize);
			Fx->SetVector("TexelSize", &TexelSize);
			Fx->Begin(NULL, 0);
			Fx->BeginPass(0);

			{
				_Quad.Render(m_pDevice, 
					1.f,
					1.f,
					Fx);

				/*m_pDevice->DrawPrimitiveUP(
					D3DPT_TRIANGLESTRIP,
					2, 
					DXScreenQuadVertices, 6 * sizeof(float));*/
			}
			Fx->EndPass();
			Fx->End();

			m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		});

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	auto *const Fx = ShadowMap->GetEffect();
	Fx->SetBool("IsPerspective", TRUE);

	for (int i = 0; i < 3; ++i)
	{
		Pointlight[i]->RenderShadowMap(
			m_pDevice, [&](FLight* Light)
			{
				Matrix ViewProj;
				Vector4
					ClipPlanes(Light->GetNearPlane(),
						Light->GetFarPlane(), 0, 0);

				Light->CalculateViewProjection(ViewProj);

				Fx->SetTechnique("Variance");

				Fx->SetVector("LightPosition",
					&Light->GetPosition());
				Fx->SetVector("ClipPlanes", &ClipPlanes);

				m_pDevice->Clear(0, NULL,
					D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
					0, 1.0f, 0);

				// RenderScene(Fx, ViewProj);
			});
	};

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
