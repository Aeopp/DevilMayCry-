#include "stdafx.h"
#include "..\Header\Em0000_Weapon.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"
#include "Em0000.h"



void Em0000Weapon::Free()
{

}

std::string Em0000Weapon::GetName()
{
	return "Em0000Weapon";
};

Em0000Weapon* Em0000Weapon::Create()
{
	return new Em0000Weapon{};
}


void Em0000Weapon::RenderReady()
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

HRESULT Em0000Weapon::Ready()
{
	// 렌더를 수행해야하는 오브젝트라고 (렌더러에 등록 가능 ) 알림.
	// 렌더 인터페이스 상속받지 않았다면 키지마세요.
	SetRenderEnable(true);

	// 렌더 정보 초기화 ...
	ENGINE::RenderProperty _InitRenderProp;
	// 이값을 런타임에 바꾸면 렌더를 켜고 끌수 있음. 
	_InitRenderProp.bRender = true;
	// 넘겨준 패스에서는 렌더링 호출 보장 . 
	/*_InitRenderProp.RenderOrders = 
	{ 
		RenderProperty::Order::ForwardAlphaBlend,
		RenderProperty::Order::Debug 
	};*/
	RenderInterface::Initialize(_InitRenderProp);
	// 


	// 렌더링 패스와 쉐이더 매칭 . 쉐이더 매칭이 안되면 렌더링을 못함.


	// 스태틱 메쉬 로딩
	m_pStaticMesh = Resources::Load<ENGINE::StaticMesh>(
		L"..\\..\\Resource\\Mesh\\Dynamic\\Monster\\Em0000\\Weapon\\Weapon.fbx");
	PushEditEntity(m_pStaticMesh.get());

	// 트랜스폼 초기화 .. 
	auto InitTransform = GetComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.01f,0.01f,0.01f});
	PushEditEntity(InitTransform.lock().get());


	m_pTransform = GetComponent<ENGINE::Transform>();

	// 에디터의 도움을 받고싶은 오브젝트들 Raw 포인터로 푸시.
	// PushEditEntity(_ShaderInfo.ForwardAlphaBlendShader.get());

	return S_OK;
};

HRESULT Em0000Weapon::Awake()
{
	m_pParentBone = m_pEm0000Mesh.lock()->GetToRootMatrixPtr("R_WeaponHand");

	return S_OK;
}

HRESULT Em0000Weapon::Start()
{
	return S_OK;
}

UINT Em0000Weapon::Update(const float _fDeltaTime)
{
	//////////////무기 붙이기////////////////////////////
	//오른손에만 붙였는데 왼손도 딱 맞게 붙음.
	//m_ParentBone = m_pEm0000Mesh.lock()->GetNodeToRoot("R_WeaponHand");
	//if (!m_ParentBone)
	//	return 0;
	m_ParentWorld = m_pEm0000Trasform.lock()->GetWorldMatrix();
	m_Result = (*m_pParentBone * m_ParentWorld);
	m_pTransform.lock()->SetWorldMatrix(m_Result);
	//////////////////////////////////////////////////
	

	return 0;
}

UINT Em0000Weapon::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

void Em0000Weapon::Editor()
{
	GameObject::Editor();
}


void Em0000Weapon::OnEnable()
{

}

void Em0000Weapon::OnDisable()
{

}

void Em0000Weapon::SetMesh(std::weak_ptr<ENGINE::SkeletonMesh> _pMesh)
{
	m_pEm0000Mesh = _pMesh;
}

void Em0000Weapon::SetOwner(std::weak_ptr<Em0000> _pOwner)
{
	m_pEm0000 = _pOwner;
	m_pEm0000Trasform = m_pEm0000.lock()->GetComponent<Transform>();
}
