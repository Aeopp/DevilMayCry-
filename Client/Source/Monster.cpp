#include "stdafx.h"
#include "Monster.h"

#include <iostream>


std::string Monster::GetName()
{
	return "Monster";
}


void Monster::RenderReady()
{
	auto _WeakTransform = GetComponent<ENGINE::Transform>();
	if (auto _SpTransform = _WeakTransform.lock();
		_SpTransform)
	{
		_RenderProperty.bRender = true;
		ENGINE::RenderInterface::UpdateInfo _UpdateInfo{};
		_UpdateInfo.World = _SpTransform->GetWorldMatrix();
		RenderVariableBind(_UpdateInfo);
	}
}


