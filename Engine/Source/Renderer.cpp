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
};


HRESULT Renderer::ReadyRenderSystem(LPDIRECT3DDEVICE9 const _pDevice) 
{
	m_pDevice = _pDevice;
	SafeAddRef(m_pDevice);
	CameraFrustum.Initialize(m_pDevice);


	return S_OK;
}

void Renderer::Push(const std::weak_ptr<GameObject>& _RenderEntity)&
{
	if (auto _SharedRenderEntity = _RenderEntity.lock();
		_SharedRenderEntity)
	{
		if (_SharedRenderEntity->GetRenderProperty().bRender)
		{
			const auto PushOrder = _SharedRenderEntity->GetRenderProperty()._Order;
			RenderEntitys[PushOrder].push_back(_SharedRenderEntity);
		}
	}
};

// 렌더 레디에서 절두체 준비 ..

HRESULT Renderer::Render()&
{
	GraphicSystem::GetInstance()->Begin();

	for (auto& [_CurRenderGroup, _GroupEntitys  ]: RenderEntitys)
	{
		for (auto& _Entity: _GroupEntitys)
		{
			_Entity->Render();
		}
	}

	ImGui::EndFrame();
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	GraphicSystem::GetInstance()->End();

	RenderEntitys.clear();

	return S_OK;
}
