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
	CameraFrustum = std::make_shared<Frustum>();
	CameraFrustum->Initialize(m_pDevice);
	_Quad = std::make_shared<Quad>();
	_Quad->Initialize(m_pDevice);
	ReadyShader("..\\..\\Resource\\Shader");
	ReadyLights();
	TestShaderInit();

	return S_OK;
};

void Renderer::ReadyShader(const std::filesystem::path& TargetPath)
{
	std::filesystem::directory_iterator itr(TargetPath);
	while (itr != std::filesystem::end(itr)) {
		const std::filesystem::directory_entry& entry = *itr;
		const std::string& ShaderKey = TargetPath.stem().string();
		Shaders[ShaderKey] = Resources::Load<Shader>(entry.path());
		itr++;
	}
	// AlphaBlendEffect = ? ? ;
	// UI = ??  ;
}

void Renderer::ReadyLights()
{
	// 달빛
	Moonlight = std::make_shared<FLight>(FLight(FLight::Type::Directional,
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
	Moonlight->SetProjectionParameters(7.1f, 7.1f, -5.f, +5.f);

	PointLights[0]->CreateShadowMap(m_pDevice, 256);
	PointLights[1]->CreateShadowMap(m_pDevice, 256);
	PointLights[2]->CreateShadowMap(m_pDevice, 256);

	PointLights[0]->SetProjectionParameters
	(0, 0, 0.1f, 10.0f);
	PointLights[1]->SetProjectionParameters
	(0, 0, 0.1f, 10.0f);
	PointLights[2]->SetProjectionParameters
	(0, 0, 0.1f, 10.0f);
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
		auto& SceneTarget = RenderTargets["SceneTarget"] = std::make_shared<RenderTarget>();

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
	GraphicSystem::GetInstance()->Begin();
	RenderImplementation();
	RenderTargetDebugRender();
	ImguiRender();
	GraphicSystem::GetInstance()->End();
	RenderEnd();

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
	CurrentRenderInfo.View = CameraView;
	CurrentRenderInfo.ViewInverse = FMath::Inverse(CurrentRenderInfo.View);
	CurrentRenderInfo.Projection= CameraProjection;
	CurrentRenderInfo.ProjectionInverse =
		FMath::Inverse(CurrentRenderInfo.Projection);
	CurrentRenderInfo.ViewProjection = 
		CameraView * CameraProjection;
	CurrentRenderInfo.ViewProjectionInverse = FMath::Inverse(CurrentRenderInfo.ViewProjection);
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
	m_pDevice->GetRenderTarget(0u, &BackBuffer);
	m_pDevice->GetViewport(&BackBufViewport);
	{
		static float time = 0.0f;

		Vector4 moondir = { -0.25f,0.65f, -1,0 };

		// 달빛을 카메라 공간으로 변환 . 
		D3DXVec4Transform(&moondir, &moondir,
			&CurrentRenderInfo.ViewInverse);
		Vector3 moondir3 = Vector3{ moondir.x , moondir.y, moondir.z };
		D3DXVec3Normalize(&moondir3, &moondir3);
		moondir3.y = 0.65f;
		moondir = { moondir3.x,moondir3.y,moondir3.z ,0.0f };

		PointLights[0]->GetPosition().x = std::cosf(time * 0.5f) * 2.f;

		PointLights[0]->GetPosition().z = std::sinf(time * 0.5f) *
			std::cosf(time * 0.5f) * 2.f;

		PointLights[1]->GetPosition().x = std::cosf(1.5f * time) * 2.f;
		PointLights[1]->GetPosition().z = std::sinf(1.f * time) * 2.f;

		PointLights[2]->GetPosition().x =
			std::cosf(0.75f * time) * 1.5f;
		PointLights[2]->GetPosition().z = std::sinf(1.5f * time) * 1.5f;

		RenderShadows();
		RenderGBuffer();
		RenderDeferredShading();
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
	m_pDevice->SetRenderTarget(0u, RenderTargets["ALBM"]->GetSurface());
	m_pDevice->SetRenderTarget(1u, RenderTargets["NRMR"]->GetSurface());
	m_pDevice->SetRenderTarget(2u, RenderTargets["Depth"]->GetSurface());

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

				Fx->SetMatrix("View", &CurrentRenderInfo.View);
				Fx->SetMatrix("Projection", &CurrentRenderInfo.Projection);

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

	
	GBufferRenderImplementation(RenderProperty::Order::GBuffer, Shaders["GBuffer"]->GetEffect());
	GBufferRenderImplementation(RenderProperty::Order::GBufferSK, Shaders["GBufferSK"]->GetEffect() );

	m_pDevice->SetRenderTarget(0u, nullptr);
	m_pDevice->SetRenderTarget(1u, nullptr);
	m_pDevice->SetRenderTarget(2u, nullptr);

	return S_OK;
}

HRESULT Renderer::RenderDeferredShading()&
{
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	const float PixelSizeX = 1.0f / static_cast<float> (BackBufViewport.Width);
	const float PixelSizeY = -1.0f / static_cast<float> (BackBufViewport.Height);

	m_pDevice->SetRenderTarget(0u, RenderTargets["SceneTarget"]->GetSurface());
	m_pDevice->Clear(0,NULL,D3DCLEAR_TARGET, 0, 1.0f, 0);

	auto* const Fx = Shaders["DeferredShading"]->GetEffect();
	Fx->SetTechnique("DeferredShading");

	Fx->SetTexture("ALBM", 
		RenderTargets["ALBM"]->GetTexture());
	Fx->SetTexture("NRMR", RenderTargets["NRMR"]->GetTexture());
	Fx->SetTexture("Depth", RenderTargets["Depth"]->GetTexture());

	Fx->SetMatrix("ViewProjectionInverse", 
		&CurrentRenderInfo.ViewProjectionInverse);
	const Vector4 PixelSize = { PixelSizeX  ,PixelSizeY , 0.0f ,0.0f  };
	Fx->SetVector("PixelSize", &PixelSize);
	Fx->SetVector("EyePosition", &CurrentRenderInfo.CameraLocation);
	Fx->SetMatrix("ViewProjectionInverse", &CurrentRenderInfo.ViewProjectionInverse);

	Fx->Begin(NULL, 0);
	Fx->BeginPass(0);
	{
		Matrix LightViewProjection;
		Vector4 Clipplanes(0, 0, 0, 0);

		Moonlight->CalculateViewProjection(LightViewProjection);

		Fx->SetMatrix("LightViewProjection", &LightViewProjection);
		Fx->SetVector("LightColor", (D3DXVECTOR4*)&Moonlight->GetColor());
		Fx->SetVector("LightPosition", &Moonlight->GetPosition());
		Fx->SetFloat("SpecularPower", 200.0f);
		Fx->SetTexture("Shadow", Moonlight->GetShadowMap());
		Fx->CommitChanges();

		_Quad->Render(m_pDevice, 1.f, 1.f, Fx);
	}

	// 포인트 라이트 
	m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);

	for (int i = 0; i < 3; ++i)
	{
		Clipplanes.x = PointLights[i]->GetNearPlane();

	}

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

			Matrix ViewProjection;
			Light->CalculateViewProjection(ViewProjection);
			
			const Vector4 ClipPlanes(
				Light->GetNearPlane(),
				Light->GetFarPlane(),
				0, 0);

			if (FAILED(Fx->SetTechnique("Variance")))
			{
				PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
			}
			if (FAILED(Fx->SetVector("LightPosition", &Light->GetPosition())))
			{
				PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
			}
			if (FAILED(Fx->SetMatrix("ViewProjection", &ViewProjection)))
			{
				PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
			}
			if (FAILED(Fx->SetVector("ClipPlanes", &ClipPlanes)))
			{
				PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
			}
			if (FAILED(Fx->SetBool("IsPerspective", Light->IsPerspective())))
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
	if ( g_bRenderTargetVisible)
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


HRESULT Renderer::RenderShadows()
{
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	Moonlight->RenderShadowMap(
		m_pDevice,[this](FLight* Light)
		{
			RenderShadowScene(Light);
		});

	Moonlight->BlurShadowMap(
		m_pDevice, [this](FLight* Light)
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

			auto* const Fx = Shaders["Blur"]->GetEffect();

			Fx->SetTechnique("BoxBlur3x3");
			Fx->SetVector("PixelSize", &PixelSize);
			Fx->SetVector("TexelSize", &TexelSize);
			Fx->Begin(NULL, 0);
			Fx->BeginPass(0);

			{
				_Quad->Render(m_pDevice, 
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
	for (int i = 0; i < 3; ++i)
	{
		PointLights[i]->RenderShadowMap(
			m_pDevice, [this](FLight* Light)
			{
				RenderShadowScene(Light);
			});
	};

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}
