#include "stdafx.h"
#include "Nero.h"
#include "Renderer.h"
#include "Subset.h"
#include "NeroFSM.h"
Nero::Nero()
{
}
void Nero::Free()
{
	m_pFSM = nullptr;
}

std::string Nero::GetName()
{
	return "Nero";
}

Nero* Nero::Create()
{
	return new Nero;
}

HRESULT Nero::Ready()
{
	SetRenderEnable(true);

	ENGINE::RenderProperty _InitRenderProp;
	// �̰��� ��Ÿ�ӿ� �ٲٸ� ������ �Ѱ� ���� ����. 
	_InitRenderProp.bRender = true;
	// �Ѱ��� �н������� ������ ȣ�� ���� . 
	_InitRenderProp.RenderOrders =
	{
		RenderProperty::Order::ForwardAlphaBlend,
		RenderProperty::Order::Debug ,
		RenderProperty::Order::DebugBone
	};
	RenderInterface::Initialize(_InitRenderProp);

	// ������ �н��� ���̴� ��Ī . ���̴� ��Ī�� �ȵǸ� �������� ����.
	_ShaderInfo.RegistShader(
		RenderProperty::Order::ForwardAlphaBlend,
		L"..\\..\\Resource\\Shader\\ForwardAlphaBlendSK.hlsl", {});
	_ShaderInfo.RegistShader(
		RenderProperty::Order::Debug,
		L"..\\..\\Resource\\Shader\\DebugSK.hlsl", {});
	_ShaderInfo.RegistShader(
		RenderProperty::Order::DebugBone,
		L"..\\..\\Resource\\Shader\\DebugBone.hlsl", {});

	m_pMesh = Resources::Load<SkeletonMesh>(L"..\\..\\Resource\\Mesh\\Dynamic\\Player\\Player.fbx");
	m_pTransform.lock()->SetScale({ 0.001f,0.001f,0.001f });
	PushEditEntity(m_pMesh.get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::ForwardAlphaBlend).get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::Debug).get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::DebugBone).get());
	PushEditEntity(m_pTransform.lock().get());

	ENGINE::AnimNotify _Notify{};
	
	//���ʿ� �̺�Ʈ�� �߻� ��ų��

	_Notify.Event[0.5] = [this]() {  Log("0.5 Sec Call");  return true; };
	_Notify.Event[0.9] = [this]() {  Log("0.9 Sec Call");  return false; };

	//�� Ŭ�󿡼��� SetAnimation
	//������ �ִϸ��̼� �̸� �Ѱ��ְ� ���߿��� �ε����� �����ε�
	//�ι�°���� ���� ���� ����
	//����°���� _Notify �Ѱ���
	//m_pMesh->PlayAnimation("RunLoop", true , _Notify);
	//m_pMesh->PlayAnimation(IDLE, true, _Notify);
	//FSM �غ�
	m_pFSM.reset(NeroFSM::Create(static_pointer_cast<Nero>(m_pGameObject.lock())));

	
	return S_OK;
}


HRESULT Nero::Awake()
{
	m_pFSM->ChangeState(NeroFSM::IDLE);
	m_pMesh->PlayingTime();
	return S_OK;
}

HRESULT Nero::Start()
{
	return S_OK;
}

UINT Nero::Update(const float _fDeltaTime)
{
	if (nullptr != m_pFSM)
		m_pFSM->UpdateFSM(_fDeltaTime);

	auto [Scale,Rot,Pos] =m_pMesh->Update(_fDeltaTime);

	m_pTransform.lock()->SetPosition(m_pTransform.lock()->GetPosition() + Pos);

	return 0;
}

UINT Nero::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

void Nero::OnEnable()
{
}

void Nero::OnDisable()
{
}

void Nero::RenderReady()
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

void Nero::RenderDebugImplementation(const ImplementationInfo& _ImplInfo)
{
	//����׿� ����
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

void Nero::RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo)
{
	//������ ����
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

void Nero::RenderDebugBoneImplementation(const ImplementationInfo& _ImplInfo)
{
	//����� �� ���� ����ġ�� ��ü�� �׷���
	if (auto SpTransform = GetComponent<ENGINE::Transform>().lock();
		SpTransform)
	{
		m_pMesh->BoneDebugRender(SpTransform->GetWorldMatrix(), _ImplInfo.Fx);
	}
}

void Nero::Editor()
{
	GameObject::Editor();
	if (bEdit)
	{
		// ������ .... 
	}

}

float Nero::Get_PlayingTime()
{
	return m_pMesh->PlayingTime();
}

bool Nero::IsAnimationEnd()
{
	return m_pMesh->IsAnimationEnd();
}

void Nero::ChangeAnimation(const std::string& InitAnimName, const bool bLoop, const AnimNotify& _Notify)
{
	m_pMesh->PlayAnimation(InitAnimName, bLoop, _Notify);
}

void Nero::ChangeAnimation(const uint32 AnimationIndex, const bool bLoop, const AnimNotify& _Notify)
{
	m_pMesh->PlayAnimation(AnimationIndex, bLoop, _Notify);
}
