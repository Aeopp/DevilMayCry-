#include "stdafx.h"
#include "Buster_Arm.h"
#include "Nero.h"
#include "Renderer.h"
#include "Subset.h"

Buster_Arm::Buster_Arm()
	:m_bIsRender(false)
{
}

void Buster_Arm::Free()
{
}

Buster_Arm* Buster_Arm::Create()
{
	return new Buster_Arm;
}

HRESULT Buster_Arm::Ready()
{
	RenderInit();

	m_pTransform.lock()->SetScale({ 0.03f,0.03f,0.03f });
	
	PushEditEntity(m_pTransform.lock().get());
	SetActive(false);

	return S_OK;
}

HRESULT Buster_Arm::Awake()
{
	m_pNero = std::static_pointer_cast<Nero>(FindGameObjectWithTag(100).lock());
	

	return S_OK;
}

HRESULT Buster_Arm::Start()
{
	return S_OK;
}

UINT Buster_Arm::Update(const float _fDeltaTime)
{
	m_pMesh->Update(_fDeltaTime);

	float fCurAnimationTime = m_pMesh->PlayingTime();

	if (0.95 <= fCurAnimationTime)
	{
		SetActive(false);
	}

	return 0;
}

UINT Buster_Arm::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

void Buster_Arm::OnEnable()
{
	m_bIsRender = true;

	Matrix NeroWorld = m_pNero.lock()->Get_NeroWorldMatrix();
	std::optional<Matrix> R_HandLocal = m_pNero.lock()->Get_BoneMatrix_ByName("root");
	Matrix R_HandWorld = *R_HandLocal * NeroWorld;

	memcpy(NeroWorld.m[3], R_HandWorld.m[3], sizeof(Vector3));

	Vector3 PlayerLook = m_pNero.lock()->GetComponent<Transform>().lock()->GetLook();
	NeroWorld._41 += PlayerLook.x * -1.7;
	NeroWorld._43 += PlayerLook.z * -1.7;

	m_pTransform.lock()->SetWorldMatrix(NeroWorld);



}

void Buster_Arm::OnDisable()
{
	m_bIsRender = false;
	m_pMesh->SetPlayingTime(0);
}

void Buster_Arm::RenderInit()
{
	SetRenderEnable(true);

	ENGINE::RenderProperty _InitRenderProp;
	// 이값을 런타임에 바꾸면 렌더를 켜고 끌수 있음. 
	_InitRenderProp.bRender = true;
	_InitRenderProp.RenderOrders[RenderProperty::Order::GBuffer] =
	{
		{"gbuffer_dsSK",
		[this](const DrawInfo& _Info)
			{
				RenderGBufferSK(_Info);
			}
		},
	};
	_InitRenderProp.RenderOrders[RenderProperty::Order::Shadow]
		=
	{
		{"ShadowSK" ,
		[this](const DrawInfo& _Info)
		{
			RenderShadowSK(_Info);
		}
	} };
	_InitRenderProp.RenderOrders[RenderProperty::Order::DebugBone]
		=
	{
		{"DebugBone" ,
		[this](const DrawInfo& _Info)
		{
			RenderDebugBone(_Info);
		}
	} };
	_InitRenderProp.RenderOrders[RenderProperty::Order::Debug]
		=
	{
		{"DebugSK" ,
		[this](const DrawInfo& _Info)
		{
			RenderDebugSK(_Info);
		}
	} };
	RenderInterface::Initialize(_InitRenderProp);

	Mesh::InitializeInfo _InitInfo{};
	// 버텍스 정점 정보가 CPU 에서도 필요 한가 ? 
	_InitInfo.bLocalVertexLocationsStorage = false;
	m_pMesh = Resources::Load<SkeletonMesh>(L"..\\..\\Resource\\Mesh\\Dynamic\\Dante\\Buster_Arm\\BusterArm_Right.fbx", _InitInfo);

	m_pMesh->EnableToRootMatricies();
	PushEditEntity(m_pMesh.get());

}

void Buster_Arm::RenderGBufferSK(const DrawInfo& _Info)
{
	const Matrix World = _RenderUpdateInfo.World;
	_Info.Fx->SetMatrix("matWorld", &World);
	const uint32 Numsubset = m_pMesh->GetNumSubset();
	if (Numsubset > 0)
	{
		m_pMesh->BindVTF(_Info.Fx);
	};
	for (uint32 i = 0; i < Numsubset; ++i)
	{
		if (false == _Info._Frustum->IsIn(_RenderUpdateInfo.SubsetCullingSphere[i]))
		{
			continue;
		}
		if (auto SpSubset = m_pMesh->GetSubset(i).lock();
			SpSubset)
		{
			SpSubset->BindProperty(TextureType::DIFFUSE, 0, 0, _Info._Device);
			SpSubset->BindProperty(TextureType::NORMALS, 0, 1, _Info._Device);
			SpSubset->Render(_Info.Fx);
		};
	};
}

void Buster_Arm::RenderShadowSK(const DrawInfo& _Info)
{
	const Matrix World = _RenderUpdateInfo.World;
	_Info.Fx->SetMatrix("matWorld", &World);
	const uint32 Numsubset = m_pMesh->GetNumSubset();
	if (Numsubset > 0)
	{
		m_pMesh->BindVTF(_Info.Fx);
	};
	for (uint32 i = 0; i < Numsubset; ++i)
	{
		if (false == _Info._Frustum->IsIn(_RenderUpdateInfo.SubsetCullingSphere[i]))
		{
			continue;
		}
		if (auto SpSubset = m_pMesh->GetSubset(i).lock();
			SpSubset)
		{
			SpSubset->Render(_Info.Fx);
		};
	};
}

void Buster_Arm::RenderDebugBone(const DrawInfo& _Info)
{
	const Matrix ScaleOffset = FMath::Scale({ 0.01,0.01 ,0.01 });
	m_pMesh->BoneDebugRender(_RenderUpdateInfo.World, _Info.Fx);
}

void Buster_Arm::RenderDebugSK(const DrawInfo& _Info)
{
	const Matrix World = _RenderUpdateInfo.World;
	_Info.Fx->SetMatrix("World", &World);
	const uint32 Numsubset = m_pMesh->GetNumSubset();

	if (Numsubset > 0)
	{
		m_pMesh->BindVTF(_Info.Fx);
	};
	for (uint32 i = 0; i < Numsubset; ++i)
	{
		if (false == _Info._Frustum->IsIn(_RenderUpdateInfo.SubsetCullingSphere[i]))
		{
			continue;
		}
		if (auto SpSubset = m_pMesh->GetSubset(i).lock();
			SpSubset)
		{
			SpSubset->Render(_Info.Fx);
		};
	};
}

void Buster_Arm::ChangeAnimation(const std::string& InitAnimName, const bool bLoop, const AnimNotify& _Notify)
{
	m_pMesh->PlayAnimation(InitAnimName, bLoop, _Notify);
}

std::string Buster_Arm::GetName()
{
	return "Buster_Arm";
}

float Buster_Arm::Get_PlayingTime()
{
	return m_pMesh->PlayingTime();
}

float Buster_Arm::Get_PlayingAccTime()
{
	return m_pMesh->PlayingAccTime();
}

void Buster_Arm::RenderReady()
{
	auto _WeakTransform = GetComponent<ENGINE::Transform>();
	if (auto _SpTransform = _WeakTransform.lock();
		_SpTransform)
	{
		const Vector3 Scale = _SpTransform->GetScale();
		_RenderProperty.bRender = true;
		_RenderUpdateInfo.World = _SpTransform->GetWorldMatrix();
		if (m_pMesh)
		{
			const uint32  Numsubset = m_pMesh->GetNumSubset();
			_RenderUpdateInfo.SubsetCullingSphere.resize(Numsubset);

			for (uint32 i = 0; i < Numsubset; ++i)
			{
				const auto& _Subset = m_pMesh->GetSubset(i);
				const auto& _CurBS = _Subset.lock()->GetVertexBufferDesc().BoundingSphere;

				_RenderUpdateInfo.SubsetCullingSphere[i] = _CurBS.Transform(_RenderUpdateInfo.World, Scale.x);
			}
		}
	}
};


void Buster_Arm::Editor()
{
	GameObject::Editor();
	if (bEdit)
	{
		// 에디터 .... 
	}
}
