#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "Renderer.h"
#include "GraphicSystem.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(Renderer)

Renderer::Renderer()
{

}

void Renderer::Free()
{

};


HRESULT Renderer::ReadyRenderSystem(LPDIRECT3DDEVICE9 const _pDevice) 
{
	m_pDevice = _pDevice;
	SafeAddRef(m_pDevice);
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

HRESULT Renderer::Render()&
{
	GraphicSystem::GetInstance()->Begin();

	for (auto& [_CurRenderGroup, _GroupEntitys  ]: RenderEntitys)
	{
		for (auto& _Entity: _GroupEntitys)
		{
			_Entity->Render(this);
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
