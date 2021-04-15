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
	// ������ �����ؾ��ϴ� ������Ʈ��� (�������� ��� ���� ) �˸�.
	// ���� �������̽� ��ӹ��� �ʾҴٸ� Ű��������.
	SetRenderEnable(true);

	// ���� ���� �ʱ�ȭ ...
	ENGINE::RenderProperty _InitRenderProp;
	// �̰��� ��Ÿ�ӿ� �ٲٸ� ������ �Ѱ� ���� ����. 
	_InitRenderProp.bRender = true;
	// �Ѱ��� �н������� ������ ȣ�� ���� . 
	/*_InitRenderProp.RenderOrders = 
	{ 
		RenderProperty::Order::ForwardAlphaBlend,
		RenderProperty::Order::Debug 
	};*/
	RenderInterface::Initialize(_InitRenderProp);
	// 


	// ������ �н��� ���̴� ��Ī . ���̴� ��Ī�� �ȵǸ� �������� ����.


	// ����ƽ �޽� �ε�
	m_pStaticMesh = Resources::Load<ENGINE::StaticMesh>(
		L"..\\..\\Resource\\Mesh\\Dynamic\\Monster\\Em0000\\Weapon\\Weapon.fbx");
	PushEditEntity(m_pStaticMesh.get());

	// Ʈ������ �ʱ�ȭ .. 
	auto InitTransform = GetComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.01f,0.01f,0.01f});
	PushEditEntity(InitTransform.lock().get());


	m_pTransform = GetComponent<ENGINE::Transform>();

	// �������� ������ �ް���� ������Ʈ�� Raw �����ͷ� Ǫ��.
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
	//////////////���� ���̱�////////////////////////////
	//�����տ��� �ٿ��µ� �޼յ� �� �°� ����.
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
