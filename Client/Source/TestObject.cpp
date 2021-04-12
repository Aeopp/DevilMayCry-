#include "stdafx.h"
#include "..\Header\TestObject.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"


void TestObject::Free()
{

}

std::string TestObject::GetName()
{
	return "TestObject";
};

TestObject* TestObject::Create()
{
	return new TestObject{};
}


void TestObject::RenderForwardAlphaBlendImplementation(
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

void TestObject::RenderDebugImplementation(const ImplementationInfo& _ImplInfo)
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

void TestObject::RenderReady()
{
	auto _WeakTransform = GetComponent<ENGINE::Transform>();
	if (auto _SpTransform = _WeakTransform.lock();
		_SpTransform)
	{
		_RenderProperty.bRender = true;
		ENGINE::RenderInterface::UpdateInfo _UpdateInfo{};
		_UpdateInfo.World = _SpTransform->GetRenderMatrix();
		RenderVariableBind(_UpdateInfo);
	}
}

HRESULT TestObject::Ready()
{
	m_nTag = 1;


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
	_StaticMesh = Resources::Load<ENGINE::StaticMesh>(
		L"..\\..\\Resource\\Mesh\\Static\\Sphere.fbx");
	PushEditEntity(_StaticMesh.get());

	// Ʈ������ �ʱ�ȭ .. 
	auto InitTransform = GetComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.01,0.01,0.01});
	PushEditEntity(InitTransform.lock().get());


	m_pTransform = GetComponent<ENGINE::Transform>();

	// �������� ������ �ް���� ������Ʈ�� Raw �����ͷ� Ǫ��.
	// PushEditEntity(_ShaderInfo.ForwardAlphaBlendShader.get());


	return S_OK;
};

HRESULT TestObject::Awake()
{
	auto pBoxCollider = AddComponent<BoxCollider>();
	pBoxCollider.lock()->ReadyBoxCollider();
	pBoxCollider.lock()->SetRigid(true);
	return S_OK;
}

HRESULT TestObject::Start()
{
	return S_OK;
}

UINT TestObject::Update(const float _fDeltaTime)
{
	Vector3 vDir = m_pTransform.lock()->GetLook();

	D3DXVec3Normalize(&vDir, &vDir);
	if (Input::GetKey(DIK_UP))
		m_pTransform.lock()->Translate(vDir * _fDeltaTime * 10.f);
	if (Input::GetKey(DIK_DOWN))
		m_pTransform.lock()->Translate(-vDir * _fDeltaTime * 10.f);
	if (Input::GetKey(DIK_LEFT))
		m_pTransform.lock()->Rotate({ 0.f, D3DXToRadian(180 * -_fDeltaTime * 50.f), 0.f });
	if (Input::GetKey(DIK_RIGHT))
		m_pTransform.lock()->Rotate({ 0.f, D3DXToRadian(180 * _fDeltaTime * 50.f), 0.f });
		

	
	
	return 0;
}

UINT TestObject::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

void TestObject::Editor()
{
	GameObject::Editor();
}


void TestObject::OnEnable()
{

}

void TestObject::OnDisable()
{

}
