#include "stdafx.h"
#include "..\Header\Effect.h"
//#include "Transform.h"
//#include "Subset.h"
//#include "TextureType.h"
//#include "Renderer.h"


void Effect::Free()
{

}

std::string Effect::GetName()
{
	return "Effect";
}


void Effect::RenderReady()
{
	auto _WeakTransform = GetComponent<ENGINE::Transform>();
	if (auto _SpTransform = _WeakTransform.lock();
		_SpTransform)
	{
		_RenderProperty.bRender = true;
		ENGINE::RenderInterface::UpdateInfo _UpdateInfo{};
		_UpdateInfo.World = _SpTransform->GetWorldMatrix();
		// RenderVariableBind(_UpdateInfo);
	}
}

HRESULT Effect::Ready()
{
	SetRenderEnable(true);

	ENGINE::RenderProperty _InitRenderProp;
	_InitRenderProp.bRender = true;
	//_InitRenderProp.RenderOrders =
	//{
	//	ENGINE::RenderProperty::Order::AlphaBlendEffect,
	//	//ENGINE::RenderProperty::Order::Debug
	//};
	RenderInterface::Initialize(_InitRenderProp);

	// 자식 Ready에서 Add Transform
	//auto InitTransform = AddComponent<ENGINE::Transform>();
	//InitTransform.lock()->SetScale({ 0.001f, 0.001f, 0.001f });

	return S_OK;
}