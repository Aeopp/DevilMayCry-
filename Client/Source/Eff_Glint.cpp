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
		for (UINT i = 0u; i < 3u; ++i)
		{
			if (_SliceAmount[i] < 1.f)
			{
				_ImplInfo.Fx->SetMatrix("World", &_WorldMatrix[i]);
				_ImplInfo.Fx->SetFloat("_SliceAmount", _SliceAmount[i]);

				_ImplInfo.Fx->SetTexture("BaseMap", _GlintTex->GetTexture());
				_ImplInfo.Fx->BeginPass(0);
				SharedSubset->Render(_ImplInfo.Fx);
				_ImplInfo.Fx->EndPass();
			}
		}
	}
}

HRESULT Eff_Glint::Ready()
{
	//
	_PlayingSpeed = 3.5f;

	//
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

	auto InitTransform = GetComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 1.f, 1.f, 1.f });

	_PlaneMesh = Resources::Load<ENGINE::StaticMesh>(L"..\\..\\Resource\\Mesh\\Static\\mesh_primitive\\plane00.fbx");
	_GlintTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\Light\\tex_capcom_light_glint_0017_alpg.tga");

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

	if (1.5f < _AccumulateTime)
	{
		for (UINT i = 0u; i < 3u; ++i)
		{
			_SliceAmount[i] = 1.f;
			_Scale[i] = 0.1f;
			D3DXMatrixIdentity(&_WorldMatrix[i]);
		}
		_Interval = 0.f;

		_AccumulateTime = 0.f;
	}

	if (auto Sptransform = GetComponent<ENGINE::Transform>().lock();
		Sptransform)
	{
		float ScaleSpeed = 0.018f;
		for (UINT i = 0u; i < 3u; ++i)
		{
			_SliceAmount[i] = _AccumulateTime - _Interval > 1.f ? 1.f : _AccumulateTime - _Interval;
			if (0.f > _SliceAmount[i])
			{
				_SliceAmount[i] = 1.f;
				_Scale[i] = 0.f;
			}
			else
			{
				_Scale[i] = 0.1f + ScaleSpeed * _SliceAmount[0];
				if (1.f < _Scale[i])
					_Scale[i] = 1.f;

				Matrix TempMat, BillMat;
				D3DXMatrixIdentity(&_WorldMatrix[i]);
				float Scale = Sptransform->GetScale().x * _Scale[i] * 0.1f;
				float Rot = 0.f;
				Vector3 Pos = Sptransform->GetPosition();
				if (1u == i)
				{
					Rot = D3DXToRadian(35.f);
					D3DXMatrixScaling(&TempMat, Scale * 1.8f, Scale * _Aspect, 1.f);
					Pos += Vector3(0.01f, 0.01f, 0.01f);
				}
				else if (2u == i)
				{
					Rot = D3DXToRadian(-35.f);
					D3DXMatrixScaling(&TempMat, Scale * 0.5f, Scale * _Aspect, 1.f);
					Pos -= Vector3(0.01f, 0.01f, 0.01f);
				}
				else // 0
				{
					Rot = 0.f;
					D3DXMatrixScaling(&TempMat, Scale, Scale * _Aspect, 1.f);
				}
				_WorldMatrix[i] *= TempMat;
				D3DXMatrixRotationZ(&TempMat, Rot);
				_WorldMatrix[i] *= TempMat;
				TempMat = Renderer::GetInstance()->CurrentRenderInfo.CameraView;
				D3DXMatrixIdentity(&BillMat);
				memcpy(&BillMat.m[0][0], &TempMat.m[0][0], sizeof(Vector3));
				memcpy(&BillMat.m[1][0], &TempMat.m[1][0], sizeof(Vector3));
				memcpy(&BillMat.m[2][0], &TempMat.m[2][0], sizeof(Vector3));
				D3DXMatrixInverse(&BillMat, 0, &BillMat);
				_WorldMatrix[i] *= BillMat;
				D3DXMatrixTranslation(&TempMat, Pos.x, Pos.y, Pos.z);
				_WorldMatrix[i] *= TempMat;
			}

			if (0u == i)
				_Interval = 0.2f;
			else if (1u == i)
				_Interval = 0.4f;
			else
				_Interval = 0.f;
		}

		//
		{
			Vector3 SliderPosition = Sptransform->GetPosition();
			ImGui::SliderFloat3("Position", SliderPosition, -10.f, 10.f);
			Sptransform->SetPosition(SliderPosition);
		}

		{
			float Scale = Sptransform->GetScale().x;
			ImGui::SliderFloat("Scale", &Scale, 0.1f, 10.f);
			Sptransform->SetScale({ Scale, Scale, Scale });	// x¸¸ À¯È¿
		}

		{
			float Aspect = _Aspect;
			ImGui::SliderFloat("Aspect", &Aspect, 0.1f, 1.f);
			_Aspect = Aspect;
		}

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