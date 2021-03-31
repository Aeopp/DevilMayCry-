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
	for (auto& [_CurRenderGroup, _GroupEntitys  ]: RenderEntitys)
	{
		for (auto& CurEntity : _GroupEntitys)
		{
			CurEntity->Render(this);
		}
	}
}
