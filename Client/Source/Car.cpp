#include "stdafx.h"
#include "..\Header\Car.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"
#include "Em5000.h"
#include "TestObject.h"

void Car::Free()
{

}

std::string Car::GetName()
{
	return "Car";
};

Car* Car::Create()
{
	return new Car{};
}


void Car::RenderForwardAlphaBlendImplementation(
	const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = _StaticMesh->GetNumSubset();
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = _StaticMesh->GetSubset(SubsetIdx);
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

void Car::RenderDebugImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = _StaticMesh->GetNumSubset();
	for (uint64 SubsetIdx = 0u; SubsetIdx < NumSubset; ++SubsetIdx)
	{
		auto WeakSubset = _StaticMesh->GetSubset(SubsetIdx);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			SharedSubset->Render(_ImplInfo.Fx);
		}
	}
};

void Car::RenderReady()
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

HRESULT Car::Ready()
{

	m_nTag = ThrowCar;


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
	_StaticMesh = Resources::Load<ENGINE::SkeletonMesh>(
		L"..\\..\\Resource\\Mesh\\Static\\Car\\CarC.fbx");
	PushEditEntity(_StaticMesh.get());

	// Ʈ������ �ʱ�ȭ .. 
	auto InitTransform = GetComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.01,0.01,0.01});
	InitTransform.lock()->SetPosition({ -40.f, 0.f, 0.f });
	PushEditEntity(InitTransform.lock().get());


	m_pTransform = GetComponent<ENGINE::Transform>();

	// �������� ������ �ް���� ������Ʈ�� Raw �����ͷ� Ǫ��.
	// PushEditEntity(_ShaderInfo.ForwardAlphaBlendShader.get());

	return S_OK;
};

HRESULT Car::Awake()
{
	m_pPlayer = std::static_pointer_cast<TestObject>(FindGameObjectWithTag(Player).lock());
	m_pPlayerTrans = m_pPlayer.lock()->GetComponent<ENGINE::Transform>();

	m_pMonster= std::static_pointer_cast<Em5000>(FindGameObjectWithTag(Monster5000).lock());
	m_pMonsterTrans = m_pMonster.lock()->GetComponent<ENGINE::Transform>();
	m_pMonsterMesh = m_pMonster.lock()->Get_Mesh();

	


	return S_OK;
}

HRESULT Car::Start()
{
	m_pParentBone = m_pMonsterMesh.lock()->GetToRootMatrixPtr("R_Hand");
	return S_OK;
}

UINT Car::Update(const float _fDeltaTime)
{
	//�� ¥�� �̷��� �ٲټ�
	if (m_pMonster.lock()->Get_State() == Em5000::Attack_Grap_Car)
	{
		if (m_pMonsterMesh.lock()->PlayingTime() >= 0.65f && m_bThrow == false)
		{
			// �տ� ���̱�
			m_ParentWorld = m_pMonsterTrans.lock()->GetWorldMatrix();
			m_Result = (*m_pParentBone * m_ParentWorld);
			m_pTransform.lock()->SetWorldMatrix(m_Result);
		}
	}
	if (m_pMonster.lock()->Get_State() == Em5000::Attack_Throw_Car)
	{

		// �տ� ���̱�
		if (m_pMonsterMesh.lock()->PlayingTime() <= 0.5f && m_bThrow == false)
		{
			m_ParentWorld = m_pMonsterTrans.lock()->GetWorldMatrix();
			m_Result = (*m_pParentBone * m_ParentWorld);
			m_pTransform.lock()->SetWorldMatrix(m_Result);
		}
	}

		

	Throw(_fDeltaTime);
	return 0;
}

UINT Car::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

void Car::Editor()
{
	GameObject::Editor();
}


void Car::OnEnable()
{

}

void Car::OnDisable()
{

}

void Car::Throw(const float _fDeltaTime)
{
	if(m_bThrow)
	{
		Vector3 vDir = m_vPlayerPos - m_pTransform.lock()->GetPosition();
		float	fDir = D3DXVec3Length(&vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		//�÷��̾��浹or�ٴ� �浹�Ǹ����� ���� �ٲ����
		if (fDir < 3.f)
		{
			m_fThrowTime = 0.f;
			m_pTransform.lock()->SetRotation({ 0.f,0.f,0.f });
		}
		else
		{
			m_pTransform.lock()->Rotate({ 5.f, 0.f,0.f });
			m_pTransform.lock()->Translate(vDir * 0.5f);
		}

		if (m_pMonster.lock()->Get_State() == Em5000::Attack_Throw_Car && m_pMonsterMesh.lock()->PlayingTime()>=0.9f)
			m_bThrow = false;

	}
	else
	{
		if (m_pMonster.lock()->Get_State() == Em5000::Attack_Throw_Car)
		{
			m_fThrowTime += _fDeltaTime;
			if (m_fThrowTime >= 1.f)
			{
				m_bThrow = true;
				m_vPlayerPos = m_pPlayerTrans.lock()->GetPosition();
			}

		}
	}
}

