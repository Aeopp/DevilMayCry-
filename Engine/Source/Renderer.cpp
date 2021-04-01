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

void Renderer::Push(const std::shared_ptr<GameObject>& _RenderEntity)&
{
	if (_RenderEntity)
	{
		const auto PushOrder = _RenderEntity->GetRenderProperty()._Order;
		RenderEntitys[PushOrder].push_back(_RenderEntity);
	}
};

HRESULT Renderer::Render()&
{
	GraphicSystem::GetInstance()->Begin();

	for (auto& [_CurRenderGroup, _GroupEntitys  ]: RenderEntitys)
	{
		for (auto& WeakCurEntity : _GroupEntitys)
		{
			if (auto CurEntity = WeakCurEntity.lock();
				CurEntity)
			{
				CurEntity->Render(this);
			}
		}
	}

	ImGui::EndFrame();
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	GraphicSystem::GetInstance()->End();

	return S_OK;
}
