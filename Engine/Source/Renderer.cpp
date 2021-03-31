#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "Renderer.h"
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
	// 그래픽 디바이스 시작
	// Clear 하고 BeginScene gkrl

	m_pDevice->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		0xff000000, 1.f, 0);
	m_pDevice->BeginScene();

	for (uint8 i = 0; i < 8u; ++i)
	{
		m_pDevice->SetSamplerState(i, D3DSAMPLERSTATETYPE::D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		m_pDevice->SetSamplerState(i, D3DSAMPLERSTATETYPE::D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		m_pDevice->SetSamplerState(i, D3DSAMPLERSTATETYPE::D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
		// m_pDevice->SetSamplerState(i, D3DSAMPLERSTATETYPE::D3DSAMP_MAXANISOTROPY, GetCaps().MaxAnisotropy);
	}


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


	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, NULL, nullptr);

	// End Scene 하고 Present 하기

	return S_OK;
}
