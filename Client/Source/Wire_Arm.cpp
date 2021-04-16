#include "stdafx.h"
#include "Wire_Arm.h"
#include "Nero.h"
#include "Renderer.h"
#include "Subset.h"

Wire_Arm::Wire_Arm()
	:m_bIsRender(false)
{
}

void Wire_Arm::Free()
{
}

Wire_Arm* Wire_Arm::Create()
{
	return new Wire_Arm;
}

HRESULT Wire_Arm::Ready()
{
	SetRenderEnable(true);

	ENGINE::RenderProperty _InitRenderProp;
	// 이값을 런타임에 바꾸면 렌더를 켜고 끌수 있음. 
	_InitRenderProp.bRender = true;
	// 넘겨준 패스에서는 렌더링 호출 보장 . 
	_InitRenderProp.RenderOrders =
	{
		RenderProperty::Order::ForwardAlphaBlend,
		RenderProperty::Order::Debug ,
		RenderProperty::Order::DebugBone
	};
	RenderInterface::Initialize(_InitRenderProp);

	// 렌더링 패스와 쉐이더 매칭 . 쉐이더 매칭이 안되면 렌더링을 못함.
	_ShaderInfo.RegistShader(
		RenderProperty::Order::ForwardAlphaBlend,
		L"..\\..\\Resource\\Shader\\ForwardAlphaBlendSK.hlsl", {});
	_ShaderInfo.RegistShader(
		RenderProperty::Order::Debug,
		L"..\\..\\Resource\\Shader\\DebugSK.hlsl", {});
	_ShaderInfo.RegistShader(
		RenderProperty::Order::DebugBone,
		L"..\\..\\Resource\\Shader\\DebugBone.hlsl", {});

	m_pMesh = Resources::Load<SkeletonMesh>(L"..\\..\\Resource\\Mesh\\Dynamic\\Dante\\Wire_Arm\\Wire_Arm.fbx");
	m_pMesh->AnimationDataLoadFromJsonTable(L"..\\..\\Resource\\Mesh\\Dynamic\\Dante\\Wire_Arm\\Wire_Arm.Animation");
	m_pMesh->EnableToRootMatricies();
	m_pTransform.lock()->SetScale({ 0.03f,0.03f,0.03f });
	PushEditEntity(m_pMesh.get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::ForwardAlphaBlend).get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::Debug).get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::DebugBone).get());
	PushEditEntity(m_pTransform.lock().get());
	SetActive(false);

	return S_OK;
}

HRESULT Wire_Arm::Awake()
{
	m_pNero = std::static_pointer_cast<Nero>(FindGameObjectWithTag(100).lock());
	

	return S_OK;
}

HRESULT Wire_Arm::Start()
{
	return S_OK;
}

UINT Wire_Arm::Update(const float _fDeltaTime)
{
	m_pMesh->Update(_fDeltaTime);

	float fCurAnimationTime = m_pMesh->PlayingTime();

	if (0.95 <= fCurAnimationTime)
	{
		SetActive(false);
	}

	return 0;
}

UINT Wire_Arm::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

void Wire_Arm::OnEnable()
{
	m_bIsRender = true;

	Matrix NeroWorld = m_pNero.lock()->Get_NeroWorldMatrix();
	std::optional<Matrix> R_HandLocal = m_pNero.lock()->Get_BoneMatrix_ByName("R_Hand");
	Matrix R_HandWorld = *R_HandLocal * NeroWorld;

	memcpy(NeroWorld.m[3], R_HandWorld.m[3],sizeof(Vector3));

	Vector3 PlayeUp = m_pNero.lock()->GetComponent<Transform>().lock()->GetUp();
	NeroWorld._42 += PlayeUp.y * -1.7;

	m_pTransform.lock()->SetWorldMatrix(NeroWorld);
}

void Wire_Arm::OnDisable()
{
	m_bIsRender = false;
	m_pMesh->SetPlayingTime(0);
}

void Wire_Arm::ChangeAnimation(const std::string& InitAnimName, const bool bLoop, const AnimNotify& _Notify)
{
	m_pMesh->PlayAnimation(InitAnimName, bLoop, _Notify);
}

std::string Wire_Arm::GetName()
{
	return "Wire_Arm";
}

float Wire_Arm::Get_PlayingTime()
{
	return m_pMesh->PlayingTime();
}

float Wire_Arm::Get_PlayingAccTime()
{
	return m_pMesh->PlayingAccTime();
}

void Wire_Arm::RenderReady()
{
	auto _WeakTransform = GetComponent<ENGINE::Transform>();
	if (auto _SpTransform = _WeakTransform.lock();
		_SpTransform)
	{
		_RenderProperty.bRender = m_bIsRender;
		ENGINE::RenderInterface::UpdateInfo _UpdateInfo{};
		_UpdateInfo.World = _SpTransform->GetWorldMatrix();
		RenderVariableBind(_UpdateInfo);
	}
}

void Wire_Arm::RenderDebugImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = m_pMesh->GetNumSubset();
	m_pMesh->BindVTF(_ImplInfo.Fx);
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = m_pMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			SharedSubset->Render(_ImplInfo.Fx);
		}
	}
}

void Wire_Arm::RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = m_pMesh->GetNumSubset();
	m_pMesh->BindVTF(_ImplInfo.Fx);
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = m_pMesh->GetSubset(SubsetIdx);
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

void Wire_Arm::RenderDebugBoneImplementation(const ImplementationInfo& _ImplInfo)
{
}

void Wire_Arm::Editor()
{
	GameObject::Editor();
	if (bEdit)
	{
		// 에디터 .... 
	}
}
