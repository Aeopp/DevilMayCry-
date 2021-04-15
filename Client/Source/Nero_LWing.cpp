#include "stdafx.h"
#include "Nero_LWing.h"
#include "Renderer.h"
#include "Subset.h"
#include "Nero.h"
Nero_LWing::Nero_LWing()
	:m_bIsRender(true)
{
}

void Nero_LWing::Free()
{
}

Nero_LWing* Nero_LWing::Create()
{
	return new Nero_LWing;
}

HRESULT Nero_LWing::Ready()
{
	SetRenderEnable(true);

	ENGINE::RenderProperty _InitRenderProp;
	// 이값을 런타임에 바꾸면 렌더를 켜고 끌수 있음. 
	_InitRenderProp.bRender = m_bIsRender;
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

	m_pMesh = Resources::Load<SkeletonMesh>(L"..\\..\\Resource\\Mesh\\Dynamic\\Dante\\Wing\\Wing_Left.fbx");
	m_pMesh->EnableToRootMatricies();

	m_pTransform.lock()->SetScale({ 0.001f,0.001f,0.001f });

	PushEditEntity(m_pMesh.get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::ForwardAlphaBlend).get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::Debug).get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::DebugBone).get());
	PushEditEntity(m_pTransform.lock().get());

	return S_OK;
}

HRESULT Nero_LWing::Awake()
{
	m_pNero = std::static_pointer_cast<Nero>(FindGameObjectWithTag(100).lock());
	m_pParentBoneMat = m_pNero.lock()->Get_BoneMatrixPtr("L_Shoulder");
	return S_OK;
}

HRESULT Nero_LWing::Start()
{
	return S_OK;
}

UINT Nero_LWing::Update(const float _fDeltaTime)
{
	m_pMesh->GetRootNode()->NodeUpdate(FMath::Identity(), 0.f, "", {});
	m_pMesh->UpdateToRootMatricies();
	m_pMesh->VTFUpdate();

	return 0;
}

UINT Nero_LWing::LateUpdate(const float _fDeltaTime)
{
	Matrix								ParentWorldMatrix, FinalWorld;

	ParentWorldMatrix = m_pNero.lock()->Get_NeroWorldMatrix();

	if (nullptr != m_pParentBoneMat)
	{
		FinalWorld =  *m_pParentBoneMat * ParentWorldMatrix;
		m_pTransform.lock()->SetWorldMatrix(FinalWorld);
	}

	//m_pTransform.lock()->UpdateTransform();

	return 0;
}

void Nero_LWing::OnEnable()
{
	m_bIsRender = true;
}

void Nero_LWing::OnDisable()
{
	m_bIsRender = false;
}

void Nero_LWing::RenderReady()
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

void Nero_LWing::RenderDebugImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = m_pMesh->GetNumSubset();
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

void Nero_LWing::RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo)
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

void Nero_LWing::RenderDebugBoneImplementation(const ImplementationInfo& _ImplInfo)
{
}

void Nero_LWing::Editor()
{
	GameObject::Editor();
	if (bEdit)
	{
		static char BoneName[MAX_PATH] = "";
		ImGui::InputText("BoneName", BoneName, MAX_PATH);
		if (ImGui::Button("ChangeBone"))
		{
			m_pParentBoneMat = m_pNero.lock()->Get_BoneMatrixPtr(BoneName);
		}
	}
}

std::string Nero_LWing::GetName()
{
	return "Nero_LWing";
}
