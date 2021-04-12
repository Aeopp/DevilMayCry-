#include "stdafx.h"
#include "TestMap.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"

TestMap::TestMap()
	: m_pMesh(nullptr)
{
}

void TestMap::Free()
{
	GameObject::Free();
}

TestMap* TestMap::Create()
{
	TestMap* pInstance = new TestMap;
	return pInstance;
}

void TestMap::RenderReady()
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

void TestMap::RenderDebugImplementation(const ImplementationInfo& _ImplInfo)
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

void TestMap::RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo)
{
	const uint64 NumSubset = m_pMesh->GetNumSubset();
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

HRESULT TestMap::Ready()
{
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
		L"..\\..\\Resource\\Shader\\ForwardAlphaBlend.hlsl", {});
	_ShaderInfo.RegistShader(
		RenderProperty::Order::Debug,
		L"..\\..\\Resource\\Shader\\Debug.hlsl", {});


	Mesh::InitializeInfo _InitInfo{};
	_InitInfo.bLocalVertexLocationsStorage = true;
	m_pMesh = Resources::Load<StaticMesh>(TEXT("..\\..\\Resource\\Mesh\\Static\\Map\\S1_HM.fbx"), _InitInfo);

	return S_OK;
}

HRESULT TestMap::Awake()
{
	auto pMeshCollider = AddComponent<MeshCollider>();
	pMeshCollider.lock()->ReadyMeshCollider(m_pMesh->GetVerticesPointer(), m_pMesh->GetNumVertices(), m_pMesh->GetIndicesPointer(), m_pMesh->GetNumIndices());

	m_pTransform.lock()->SetRotation(D3DXVECTOR3(0.01f, 0.01f, 0.01f));
	return S_OK;
}

HRESULT TestMap::Start()
{
	return S_OK;
}

UINT TestMap::Update(const float _fDeltaTime)
{
	GameObject::Update(_fDeltaTime);
	return 0;
}


UINT TestMap::LateUpdate(const float _fDeltaTime)
{
	return 0;
}


void TestMap::OnEnable()
{
}

void TestMap::OnDisable()
{
}
