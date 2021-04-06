#include "stdafx.h"
#include "..\Header\Eff_OvertureHand.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"


void Eff_OvertureHand::Free()
{
}
std::string Eff_OvertureHand::GetName()
{
	return "Eff_OvertureHand";
};

Eff_OvertureHand* Eff_OvertureHand::Create()
{
	return new Eff_OvertureHand{};
}


void Eff_OvertureHand::RenderAlphaBlendEffectImplementation(
	const ImplementationInfo& _ImplInfo)
{
	auto WeakSubset = _HandMesh->GetSubset(0u);

	if (auto SharedSubset = WeakSubset.lock();
		SharedSubset)
	{
		//_ImplInfo.Fx->SetFloatArray("LightDirection", Renderer::GetInstance()->TestDirectionLight, 3u);

		_ImplInfo.Fx->SetTexture("Color0Map", _GlowTex->GetTexture());
		_ImplInfo.Fx->SetTexture("Color1Map", _LightningColorTex->GetTexture());
		_ImplInfo.Fx->SetTexture("AlpMap", _LightningTex->GetTexture());
		_ImplInfo.Fx->SetTexture("NoiseMap", _NoiseTex->GetTexture());

		if (0.3f < _AccumulateTime)
		{
			_ImplInfo.Fx->SetFloat("_TexV", _RandTexV0);
			_ImplInfo.Fx->SetFloat("_SliceAmount", (_AccumulateTime - 0.3f) * 0.8f);

			_ImplInfo.Fx->BeginPass(1);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}

		if (0.2f > _AccumulateTime)
		{
			_ImplInfo.Fx->SetFloat("_TexV", _RandTexV1);
			_ImplInfo.Fx->SetFloat("_SliceAmount", 1.f - _AccumulateTime * 5.f);

			_ImplInfo.Fx->BeginPass(0);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}
		else if (0.6f > _AccumulateTime)
		{
			_ImplInfo.Fx->SetFloat("_TexV", _RandTexV1);
			_ImplInfo.Fx->SetFloat("_SliceAmount", _AccumulateTime);

			_ImplInfo.Fx->BeginPass(0);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}
	}
};

void Eff_OvertureHand::RenderReady()
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

HRESULT Eff_OvertureHand::Ready()
{
	SetRenderEnable(true);

	ENGINE::RenderProperty _InitRenderProp;
	_InitRenderProp.bRender = true;
	_InitRenderProp._Order = ENGINE::RenderProperty::Order::AlphaBlendEffect;
	RenderInterface::Initialize(_InitRenderProp);

	_ShaderInfo.AlphaBlendEffectShader = Resources::Load<ENGINE::Shader>(L"..\\..\\Resource\\Shader\\Effect\\OvertureHand.hlsl");

	auto InitTransform = AddComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.01f, 0.01f, 0.01f });

	_HandMesh = Resources::Load<ENGINE::StaticMesh>(L"..\\..\\Resource\\Mesh\\Static\\mesh_weapon\\wp00_010_0000.x");
	_LightningTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\Effect\\lightning.dds");
	_GlowTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\Effect\\tex_capcom_light_glow_0002_alpg.tga");
	_LightningColorTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\Effect\\lightning_alb.png");
	_NoiseTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\Effect\\noiseInput_ATOS.tga");
	//PushEditEntity(_HandMesh.get());
	//PushEditEntity(_LightningTex.get());
	//PushEditEntity(_GlowTex.get());
	//PushEditEntity(_LightningColorTex.get());
	//PushEditEntity(_NoiseTex.get());

	return S_OK;
};

HRESULT Eff_OvertureHand::Awake()
{
	return S_OK;
}

HRESULT Eff_OvertureHand::Start()
{
	return S_OK;
}

UINT Eff_OvertureHand::Update(const float _fDeltaTime)
{
	_AccumulateTime += _PlayingSpeed * _fDeltaTime;
	if (1.5f < _AccumulateTime)
	{
		_RandTexV0 = FMath::Random<float>(0.7f, 0.9f);
		_RandTexV1 = FMath::Random<float>(0.4f, 0.8f);
		_AccumulateTime = 0.f;
	}

	if (auto Sptransform = GetComponent<ENGINE::Transform>().lock();
		Sptransform)
	{
		{
			Vector3 SliderPosition = Sptransform->GetPosition();
			ImGui::SliderFloat3("Position", SliderPosition, -10.f, 10.f);
			Sptransform->SetPosition(SliderPosition);
		}

		{
			float AllScale = Sptransform->GetScale().x;
			ImGui::SliderFloat("All Scale", &AllScale, 0.001f, 100.f);
			Sptransform->SetScale({ AllScale,AllScale,AllScale });
		}
		static Vector3 Rotation{ 0,0,0 };
		if (ImGui::SliderAngle("Light Pitch", &Rotation.x) ||
			ImGui::SliderAngle("Light Yaw", &Rotation.y) ||
			ImGui::SliderAngle("Light Roll", &Rotation.z))
		{

			Renderer::GetInstance()->TestDirectionLight =
				FMath::Normalize(FMath::MulNormal(Vector3{ 0,0,1 }, FMath::Rotation(Rotation)));
		}
	}

	return 0;
}

UINT Eff_OvertureHand::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

void Eff_OvertureHand::Editor()
{
	GameObject::Editor();
}

void Eff_OvertureHand::OnEnable()
{

}

void Eff_OvertureHand::OnDisable()
{

}