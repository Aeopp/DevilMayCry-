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


void Em0000Weapon::RenderForwardAlphaBlendImplementation(
	const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = m_pStaticMesh->GetNumSubset();
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = m_pStaticMesh->GetSubset(SubsetIdx);
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
};

void Em0000Weapon::RenderDebugImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = m_pStaticMesh->GetNumSubset();
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = m_pStaticMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			SharedSubset->Render(_ImplInfo.Fx);
		}
	}
};

void Em0000Weapon::RenderReady()
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
	_InitRenderProp.RenderOrders = 
	{ 
		RenderProperty::Order::ForwardAlphaBlend,
		RenderProperty::Order::Debug 
	};
	RenderInterface::Initialize(_InitRenderProp);
	// 


	// ������ �н��� ���̴� ��Ī . ���̴� ��Ī�� �ȵǸ� �������� ����.
	_ShaderInfo.RegistShader(
		RenderProperty::Order::ForwardAlphaBlend,
		L"..\\..\\Resource\\Shader\\ForwardAlphaBlend.hlsl",{});
	_ShaderInfo.RegistShader(
			RenderProperty::Order::Debug,
		L"..\\..\\Resource\\Shader\\Debug.hlsl", {});

	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::ForwardAlphaBlend).get());
	PushEditEntity(_ShaderInfo.GetShader(RenderProperty::Order::Debug).get());
	// 

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
	m_pEm0000 = std::static_pointer_cast<Em0000>(FindGameObjectWithTag(Monster0000).lock());
	m_pEm0000Mesh = m_pEm0000.lock()->Get_Mesh();
	m_pEm0000Trasform = m_pEm0000.lock()->GetComponent<ENGINE::Transform>();

	
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
	m_ParentBone = m_pEm0000Mesh.lock()->GetNodeToRoot("R_WeaponHand");
	if (!m_ParentBone)
		return 0;
	m_ParentWorld = m_pEm0000Trasform.lock()->GetWorldMatrix();
	m_Result = (*m_ParentBone * m_ParentWorld);
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
