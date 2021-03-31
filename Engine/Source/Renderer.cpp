#include "Renderer.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(Renderer)

void Renderer::Free()
{

};

void Renderer::Push(const std::shared_ptr<GameObject>& _RenderEntity)&
{
	if (_RenderEntity)
	{
		const auto PushOrder = _RenderEntity->GetRenderProperty()._Order;
		RenderEntitys[PushOrder].push_back(_RenderEntity);
	}
};

void Renderer::Render()&
{
	// 그래픽 디바이스 시작

	for (auto& [_CurRenderGroup, _GroupEntitys  ]: RenderEntitys)
	{
		for (auto& CurEntity : _GroupEntitys)
		{
			CurEntity->Render(this);
		}
	}


	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());


	// 그래픽 디바이스 End



}
