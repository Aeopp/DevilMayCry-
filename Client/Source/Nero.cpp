#include "stdafx.h"
#include "Nero.h"
#include "Renderer.h"
#include "Subset.h"
#include "NeroFSM.h"
Nero::Nero()
	:m_iCurAnimationIndex(ANI_END)
	, m_iPreAnimationIndex(ANI_END)
	, m_iCurWeaponIndex(RQ)
	, m_fCbsGage(0.f)
	, m_bCbsCharge(false)
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

	m_pMesh = Resources::Load<SkeletonMesh>(L"..\\..\\Resource\\Mesh\\Dynamic\\Dante\\Player.fbx");
	m_pMesh->LoadAnimationFromDirectory(L"..\\..\\Resource\\Mesh\\Dynamic\\Dante\\Animation");
	m_pMesh->AnimationDataLoadFromJsonTable(L"..\\..\\Resource\\Mesh\\Dynamic\\Dante\\Player.Animation");
	m_pTransform.lock()->SetScale({ 0.03f,0.03f,0.03f });
	PushEditEntity(m_pMesh.get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::ForwardAlphaBlend).get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::Debug).get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::DebugBone).get());
	PushEditEntity(m_pTransform.lock().get());

	//ENGINE::AnimNotify _Notify{};
	//
	////���ʿ� �̺�Ʈ�� �߻� ��ų��

	//_Notify.Event[0.5] = [this]() {  Log("0.5 Sec Call");  return true; };
	//_Notify.Event[0.9] = [this]() {  Log("0.9 Sec Call");  return false; };

	//�� Ŭ�󿡼��� SetAnimation
	//������ �ִϸ��̼� �̸� �Ѱ��ְ� ���߿��� �ε����� �����ε�
	//�ι�°���� ���� ���� ����
	//����°���� _Notify �Ѱ���
	//m_pMesh->PlayAnimation("RunLoop", true , _Notify);
	//m_pMesh->PlayAnimation(IDLE, true, _Notify);
	//FSM �غ�
	m_pFSM.reset(NeroFSM::Create(static_pointer_cast<Nero>(m_pGameObject.lock())));
	

	m_iCurAnimationIndex = ANI_END;
	m_iPreAnimationIndex = ANI_END;
	return S_OK;
}


HRESULT Nero::Awake()
{
	m_pFSM->ChangeState(NeroFSM::IDLE);
	return S_OK;
}

HRESULT Nero::Start()
{
	return S_OK;
}

UINT Nero::Update(const float _fDeltaTime)
{
	//GameObject::Update(_fDeltaTime);
	if (Input::GetKeyDown(DIK_0))
		m_bDebugButton = !m_bDebugButton;

	if (nullptr != m_pFSM && m_bDebugButton)
		m_pFSM->UpdateFSM(_fDeltaTime);

	auto [Scale,Rot,Pos] =m_pMesh->Update(_fDeltaTime);

	m_pTransform.lock()->SetPosition(m_pTransform.lock()->GetPosition() + Pos * m_pTransform.lock()->GetScale().x);

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

void Nero::ChangeAnimation(const std::string& InitAnimName, const bool bLoop, const UINT AnimationIndex, const AnimNotify& _Notify)
{
	m_iPreAnimationIndex = m_iCurAnimationIndex;
	m_iCurAnimationIndex = AnimationIndex;
	m_pMesh->PlayAnimation(InitAnimName, bLoop, _Notify);
}

void Nero::ChangeWeapon(UINT _iWeaponIndex)
{
	m_iCurWeaponIndex = _iWeaponIndex;
}
void Nero::IncreaseCbsGage(float _fDeltaTime)
{
	if (m_bCbsCharge)
		return;
	m_fCbsGage += _fDeltaTime;
	if (1.f <= m_fCbsGage)
		m_bCbsCharge = true;
}
//if (Input::GetKeyDown(DIK_LCONTROL))
//	m_iCurWeaponIndex = m_iCurWeaponIndex == RQ ? Cbs : RQ;