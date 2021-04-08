#include "stdafx.h"
#include "..\Header\Eff_Glint.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"


void Eff_Glint::Free()
{
}

std::string Eff_Glint::GetName()
{
	return "Eff_Glint";
}

Eff_Glint* Eff_Glint::Create()
{
	return new Eff_Glint{};
}


void Eff_Glint::RenderAlphaBlendEffectImplementation(
	const ImplementationInfo& _ImplInfo)
{
	auto WeakSubset_Plane = _PlaneMesh->GetSubset(0u);
	if (auto SharedSubset = WeakSubset_Plane.lock();
		SharedSubset)
	{
		if (_SliceAmount < 1.f)
		{
			_ImplInfo.Fx->SetFloat("_SliceAmount", _SliceAmount);

			_ImplInfo.Fx->SetTexture("BaseMap", _GlintTex0->GetTexture());
			_ImplInfo.Fx->BeginPass(0);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();

			_ImplInfo.Fx->SetTexture("BaseMap", _GlintTex1->GetTexture());
			_ImplInfo.Fx->BeginPass(0);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}
	}

	for (UINT SubsetIdx = 0u; SubsetIdx < 2; ++SubsetIdx)
	{
		auto WeakSubset_Sphere = _SphereMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset_Sphere.lock();
			SharedSubset)
		{
			_ImplInfo.Fx->SetFloat("_SliceAmount", _SliceAmount);
			_ImplInfo.Fx->SetMatrix("_ScaleMatrix", &_SphereScaleMatrix);

			_ImplInfo.Fx->BeginPass(1);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}
	}
}

void Eff_Glint::RenderReady()
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

HRESULT Eff_Glint::Ready()
{
	SetRenderEnable(true);

	ENGINE::RenderProperty _InitRenderProp;
	_InitRenderProp.bRender = true;
	_InitRenderProp.RenderOrders =
	{
		ENGINE::RenderProperty::Order::AlphaBlendEffect,
		//ENGINE::RenderProperty::Order::Debug
	};
	RenderInterface::Initialize(_InitRenderProp);

	_ShaderInfo.RegistShader(ENGINE::RenderProperty::Order::AlphaBlendEffect,
		L"..\\..\\Resource\\Shader\\Effect\\Glint.hlsl", {});

	auto InitTransform = AddComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.001f, 0.001f, 0.001f });

	_PlaneMesh = Resources::Load<ENGINE::StaticMesh>(L"..\\..\\Resource\\Mesh\\Static\\mesh_primitive\\plane00.fbx");
	_SphereMesh = Resources::Load<ENGINE::StaticMesh>(L"..\\..\\Resource\\Mesh\\Static\\mesh_primitive\\sphere00.fbx");

	_GlintTex0 = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\Light\\tex_capcom_light_glint_0003_alpg.tga");
	_GlintTex1 = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\Light\\tex_capcom_light_glint_0005_alpg.tga");

	D3DXMatrixScaling(&_SphereScaleMatrix, 0.55f, 0.55f, 0.55f);

	return S_OK;
}

HRESULT Eff_Glint::Awake()
{
	return S_OK;
}

HRESULT Eff_Glint::Start()
{
	return S_OK;
}

UINT Eff_Glint::Update(const float _fDeltaTime)
{
	_AccumulateTime += _PlayingSpeed * _fDeltaTime;
	_SliceAmount = _AccumulateTime > 1.f ? 1.f : _AccumulateTime;
	_AllScale = 0.001f + 0.045f * _SliceAmount;
	if (0.01f < _AllScale)
		_AllScale = 0.01f;

	if (1.5f < _AccumulateTime)
	{
		_GlintTexIdx = FMath::Random<UINT>(0, 2);
		_SliceAmount = 0.f;
		_AllScale = 0.001f;
		_AccumulateTime = 0.f;
	}

	if (auto Sptransform = GetComponent<ENGINE::Transform>().lock();
		Sptransform)
	{
		Sptransform->SetScale({ _AllScale, _AllScale, _AllScale });
		//{
		//	Vector3 SliderPosition = Sptransform->GetPosition();
		//	ImGui::SliderFloat3("Position", SliderPosition, -10.f, 10.f);
		//	Sptransform->SetPosition(SliderPosition);
		//}

		//{
		//	float AllScale = Sptransform->GetScale().x;
		//	ImGui::SliderFloat("All Scale", &AllScale, 0.001f, 0.1f);
		//	Sptransform->SetScale({ AllScale,AllScale,AllScale });
		//}

		//static Vector3 Rotation{ 0,0,0 };
		//if (ImGui::SliderAngle("Pitch", &Rotation.x) ||
		//	ImGui::SliderAngle("Yaw", &Rotation.y) ||
		//	ImGui::SliderAngle("PitchRoll", &Rotation.z))
		//{
		//	Sptransform->SetRotation(Rotation);
		//}
	}

	return 0;
}

UINT Eff_Glint::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

void Eff_Glint::Editor()
{
	GameObject::Editor();
}

void Eff_Glint::OnEnable()
{

}

void Eff_Glint::OnDisable()
{

}