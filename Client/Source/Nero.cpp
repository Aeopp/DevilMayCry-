#include "stdafx.h"
#include "Nero.h"
#include "Renderer.h"
#include "Subset.h"
#include "NeroFSM.h"
#include "RedQueen.h"

Nero::Nero()
	:m_iCurAnimationIndex(ANI_END)
	, m_iPreAnimationIndex(ANI_END)
	, m_iCurWeaponIndex(RQ)
	, m_iJumpDirIndex(Basic)
	, m_fRedQueenGage(0.f)
{
}
void Nero::Free()
{
	m_pFSM = nullptr;
}

void Nero::Set_RQ_State(UINT _StateIndex)
{
	m_pRedQueen.lock()->SetWeaponState(_StateIndex);
}

void Nero::Set_PlayingTime(float NewTime)
{
	m_pMesh->SetPlayingTime(NewTime);
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
	// 이값을 런타임에 바꾸면 렌더를 켜고 끌수 있음. 
	_InitRenderProp.bRender = true;
	// 넘겨준 패스에서는 렌더링 호출 보장 . 
	
	RenderInterface::Initialize(_InitRenderProp);


	m_pMesh = Resources::Load<SkeletonMesh>(L"..\\..\\Resource\\Mesh\\Dynamic\\Dante\\Player.fbx");


	m_pMesh->LoadAnimationFromDirectory(L"..\\..\\Resource\\Mesh\\Dynamic\\Dante\\Animation");
	m_pMesh->AnimationDataLoadFromJsonTable(L"..\\..\\Resource\\Mesh\\Dynamic\\Dante\\Player.Animation");

	m_pMesh->EnableToRootMatricies();
	m_pTransform.lock()->SetScale({ 0.03f,0.03f,0.03f });
	PushEditEntity(m_pMesh.get());
	
	PushEditEntity(m_pTransform.lock().get());

	//ENGINE::AnimNotify _Notify{};
	//
	////몇초에 이벤트를 발생 시킬지

	//_Notify.Event[0.5] = [this]() {  Log("0.5 Sec Call");  return true; };
	//_Notify.Event[0.9] = [this]() {  Log("0.9 Sec Call");  return false; };

	//내 클라에서의 SetAnimation
	//세팅할 애니메이션 이름 넘겨주고 나중에는 인덱스도 오버로딩
	//두번째인자 루프 할지 말지
	//세번째인자 _Notify 넘겨줌
	//m_pMesh->PlayAnimation("RunLoop", true , _Notify);
	//m_pMesh->PlayAnimation(IDLE, true, _Notify);
	//FSM 준비

	m_pRedQueen = AddGameObject<RedQueen>();
	m_pFSM.reset(NeroFSM::Create(static_pointer_cast<Nero>(m_pGameObject.lock())));

	m_iCurAnimationIndex = ANI_END;
	m_iPreAnimationIndex = ANI_END;

	m_nTag = 100;

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
	}
}


void Nero::Editor()
{
	GameObject::Editor();
	if (bEdit)
	{
		// 에디터 .... 
	}

}

float Nero::Get_PlayingTime()
{
	return m_pMesh->PlayingTime();
}

float Nero::Get_PlayingAccTime()
{
	return m_pMesh->PlayingAccTime();
}

optional<Matrix> Nero::Get_BoneMatrix_ByName(std::string _BoneName)
{
	return m_pMesh->GetNodeToRoot(_BoneName);
}

Matrix* Nero::Get_BoneMatrixPtr(std::string _BoneName)
{
	return m_pMesh->GetToRootMatrixPtr(_BoneName);;
}

void Nero::StopAnimation()
{
	m_pMesh->StopAnimation();
}

void Nero::ContinueAnimiation()
{
	m_pMesh->ContinueAnimation();
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

void Nero::ChangeAnimationIndex(const UINT AnimationIndex)
{
	m_iPreAnimationIndex = m_iCurAnimationIndex;
	m_iCurAnimationIndex = AnimationIndex;
}

void Nero::ChangeWeapon(UINT _iWeaponIndex)
{
	m_iCurWeaponIndex = _iWeaponIndex;
}
//if (Input::GetKeyDown(DIK_LCONTROL))
//	m_iCurWeaponIndex = m_iCurWeaponIndex == RQ ? Cbs : RQ;