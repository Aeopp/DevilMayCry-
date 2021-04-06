#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "Renderer.h"
#include "GraphicSystem.h"
#include "FMath.hpp"
#include <d3d9.h>
#include <d3dx9.h>
#include "Resources.h"


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
	// _ShaderTester.Clear();
};


HRESULT Renderer::ReadyRenderSystem(LPDIRECT3DDEVICE9 const _pDevice) 
{
	m_pDevice = _pDevice;
	SafeAddRef(m_pDevice);
	ReadyRenderTargets();
	CameraFrustum.Initialize(m_pDevice);
	RTDebug = Resources::Load<ENGINE::Shader>(L"..\\..\\Resource\\Shader\\ScreenQuad.hlsl");
	// _ShaderTester.Initialize();

	return S_OK;
}

void Renderer::ReadyRenderTargets()
{
	static const Vector2 RenderTargetDebugRenderSize{ 90.f,90.f };
	 const  float InitX =
		g_nWndCX / -2.f + (RenderTargetDebugRenderSize.x);
	 const  float InitY =
		 g_nWndCY / +2.f - (RenderTargetDebugRenderSize.y);

	{
		RenderTarget::Info InitInfo;
		InitInfo.Width = g_nWndCX;
		InitInfo.Height = g_nWndCY;
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
		InitInfo.Width = g_nWndCX;
		InitInfo.Height = g_nWndCY;
		InitInfo.Levels = 1;
		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
		InitInfo.Format = D3DFMT_A8R8G8B8;
		InitInfo._D3DPool = D3DPOOL_DEFAULT;
		ALBM.Initialize(InitInfo);
		ALBM.DebugBufferInitialize(
			{ InitX,InitY*3.f },
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
			{ InitX,InitY * 5.f }, 
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
			{ InitX,InitY * 7.f }, 
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
				const auto PassOrder = _SharedRenderEntity->GetRenderProp()._Order;
				RenderEntitys[PassOrder].push_back(_SharedRenderEntity);
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
	// _ShaderTester.Render();

	RenderTargetDebugRender();
	ImguiRender();
	GraphicSystem::GetInstance()->End();
	RenderEnd();

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
	CurrentRenderInfo.Ortho = Ortho;
	Culling();
}

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

	m_pDevice->Clear(0u,
		NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		0, 1.0f, 0);

	if (auto _TargetGroup = RenderEntitys.find(ENGINE::RenderProperty::Order::GBuffer);
		_TargetGroup != std::end(RenderEntitys))
	{
		for (auto& _RenderEntity : _TargetGroup->second)
		{
			if (_RenderEntity)
			{
				if (_RenderEntity->GetRenderProp().bRender)
				{
					_RenderEntity->RenderGBuffer();
				}
			}
		}
	}

	m_pDevice->SetRenderTarget(1u, nullptr);
	m_pDevice->SetRenderTarget(2u, nullptr);

	return S_OK;
}

HRESULT Renderer::RenderForwardAlphaBlend()&
{
	m_pDevice->SetRenderTarget(0u, BackBuffer);
	if (auto _TargetGroup = RenderEntitys.find(ENGINE::RenderProperty::Order::ForwardAlphaBlend);
		_TargetGroup !=    std::end(RenderEntitys))
	{
		for (auto& _RenderEntity: _TargetGroup->second)
		{
			if (_RenderEntity)
			{
				if (_RenderEntity->GetRenderProp().bRender)
				{
					_RenderEntity->RenderForwardAlphaBlend();
				}
			}
		}
	}

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
