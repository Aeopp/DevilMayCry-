#include "stdafx.h"
#include "MapToolPivot.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"
void MapToolPivot::Free()
{
}

std::string MapToolPivot::GetName()
{
	return std::string();
}

MapToolPivot* MapToolPivot::Create()
{
    return new MapToolPivot{};
}

HRESULT MapToolPivot::Ready()
{
	SetRenderEnable(true);

	ENGINE::RenderProperty _InitRenderProp;
	_InitRenderProp.bRender = true;
	_InitRenderProp._Order = ENGINE::RenderProperty::Order::ForwardAlphaBlend;
	RenderInterface::Initialize(_InitRenderProp);

	_ShaderInfo.ForwardAlphaBlendShader = Resources::Load<ENGINE::Shader>(L"..\\..\\Resource\\Shader\\ForwardAlphaBlend.hlsl");
	_ShaderInfo.GBufferShader = Resources::Load<ENGINE::Shader>(L"..\\..\\Resource\\Shader\\GBuffer.hlsl");

	auto InitTransform = AddComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.01,0.01,0.01 });

	_StaticMesh = Resources::Load<ENGINE::StaticMesh>(L"..\\..\\Resource\\Mesh\\MapTool\\Pivot2.fbx");
	//_SkeletonMesh->EnablePrevVTF();

	ENGINE::AnimNotify _Notify{};

	_Notify.Event[0.5] = [this]() {  Log("0.5 !! ");  return true; };
	_Notify.Event[0.9] = [this]() {  Log("0.9 !! ");  return true; };

	//_SkeletonMesh->PlayAnimation("Air_Belatos_Twohandedsword", true, _Notify);
	PushEditEntity(_StaticMesh.get());
	PushEditEntity(InitTransform.lock().get());
	PushEditEntity(_ShaderInfo.ForwardAlphaBlendShader.get());
	PushEditEntity(_ShaderInfo.GBufferShader.get());
	
	
	/////
    return S_OK;
}

HRESULT MapToolPivot::Awake()
{
    return S_OK;
}

HRESULT MapToolPivot::Start()
{
    return S_OK;
}

UINT MapToolPivot::Update(const float _fDeltaTime)
{
	//_SkeletonMesh->Update(_fDeltaTime);
    return 0;
}

UINT MapToolPivot::LateUpdate(const float _fDeltaTime)
{
    return 0;
}

void MapToolPivot::OnEnable()
{
}

void MapToolPivot::OnDisable()
{
}

void MapToolPivot::RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = _StaticMesh->GetNumSubset();
	//_SkeletonMesh->BindVTF(_ImplInfo.Fx);
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = _StaticMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			_ImplInfo.Fx->SetFloatArray("LightDirection", Renderer::GetInstance()->TestDirectionLight, 3u);
			const auto& VtxBufDesc = SharedSubset->GetVertexBufferDesc();
			SharedSubset->BindProperty(TextureType::DIFFUSE, 0u, "ALBM0Map", _ImplInfo.Fx);
			SharedSubset->BindProperty(TextureType::NORMALS, 0u, "NRMR0Map", _ImplInfo.Fx);
			SharedSubset->Render(_ImplInfo.Fx);
		}
	}
}

void MapToolPivot::RenderGBufferImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = _StaticMesh->GetNumSubset();
	//_SkeletonMesh->BindVTF(_ImplInfo.Fx);
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = _StaticMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			const auto& VtxBufDesc = SharedSubset->GetVertexBufferDesc();
			SharedSubset->BindProperty(TextureType::DIFFUSE, 0u, "ALBM0Map", _ImplInfo.Fx);
			SharedSubset->BindProperty(TextureType::NORMALS, 0u, "NRMR0Map", _ImplInfo.Fx);
			SharedSubset->Render(_ImplInfo.Fx);
		}
	}
}

void MapToolPivot::RenderReady()
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
