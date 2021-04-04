#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "Renderer.h"
#include "GraphicSystem.h"
#include "FMath.hpp"
#include <d3d9.h>
#include <d3dx9.h>


USING(ENGINE)
IMPLEMENT_SINGLETON(Renderer)

Renderer::Renderer()
{

}

void Renderer::Free()
{
	CameraFrustum.Release();
	_ShaderTester.Clear();
};


HRESULT Renderer::ReadyRenderSystem(LPDIRECT3DDEVICE9 const _pDevice) 
{
	m_pDevice = _pDevice;
	SafeAddRef(m_pDevice);
	CameraFrustum.Initialize(m_pDevice);
	_ShaderTester.Initialize();

	return S_OK;
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
	
	/*RenderReady();
	GraphicSystem::GetInstance()->Begin();
	RenderImplementation();*/
	_ShaderTester.Render();

	ImguiRender();
	//GraphicSystem::GetInstance()->End();
	//RenderEnd();

	return S_OK;
}

void Renderer::RenderReady()&
{
	RenderReadyEntitys();

	 // 테스트를 위해 임시로 카메라와 투영 설정...
	Matrix CameraView, CameraProjection;
	{
		// 카메라 .. 
		{
			const Vector3 Eye{ 0,0,-10 };
			const Vector3 At{ 0,0,0 };
			const Vector3 WorldUp = { 0,1,0 };
			D3DXMatrixLookAtLH(&CameraView, &Eye, &At, &WorldUp);
		}
		// 프로젝션
		{
			const float FovY = FMath::ToRadian(45.f);
			const float Aspect = static_cast<float>(g_nWndCX)/ static_cast<float>(g_nWndCY);
			const float NearPlane = 0.1f;
			const float FarPlane = 100.f;
			D3DXMatrixPerspectiveFovLH(&CameraProjection, FovY, Aspect,NearPlane,FarPlane);
		}
	}
	
	/*Matrix CameraView, CameraProjection;
	m_pDevice->GetTransform(D3DTS_VIEW, &CameraView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &CameraProjection);*/
	CurrentRenderInfo.Make(CameraView, CameraProjection);
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
	RenderForwardAlphaBlend();


	return S_OK;
}

HRESULT Renderer::RenderForwardAlphaBlend()&
{
	if (auto _TargetGroup = RenderEntitys.find(ENGINE::RenderProperty::Order::ForwardAlphaBlend);
		_TargetGroup !=    std::end(RenderEntitys))
	{
		for (auto& _RenderEntity: _TargetGroup->second)
		{
			if (_RenderEntity)
			{
				_RenderEntity->RenderForwardAlphaBlend();
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
