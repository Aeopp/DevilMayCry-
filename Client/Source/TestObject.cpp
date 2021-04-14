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


void TestObject::RenderReady()
{
	auto _WeakTransform = GetComponent<ENGINE::Transform>();
	if (auto _SpTransform = _WeakTransform.lock();
		_SpTransform)
	{
		_RenderProperty.bRender = true;
		_RenderUpdateInfo.World = _SpTransform->GetWorldMatrix();
	}
}

void TestObject::RenderInit()
{
	// ������ �����ؾ��ϴ� ������Ʈ��� (�������� ��� ���� ) �˸�.
	// ���� �������̽� ��ӹ��� �ʾҴٸ� Ű��������.
	SetRenderEnable(true);

	// ���� ���� �ʱ�ȭ ...
	ENGINE::RenderProperty _InitRenderProp;
	// �̰��� ��Ÿ�ӿ� �ٲٸ� ������ �Ѱ� ���� ����. 
	_InitRenderProp.bRender = true;
	// ���� �Ӽ� ��ü �ʱ�ȭ 
	// �̰��� ��Ÿ�ӿ� �ٲٸ� ������ �Ѱ� ���� ����. 
	_InitRenderProp.bRender = true;
	_InitRenderProp.RenderOrders[RenderProperty::Order::GBuffer] =
	{
		{"gbuffer_ds",
		[this](const DrawInfo& _Info)
			{
				RenderGBuffer(_Info);
			}
		},
	};
	_InitRenderProp.RenderOrders[RenderProperty::Order::Shadow]
		=
	{
		{"Shadow" ,
		[this](const DrawInfo& _Info)
		{
			RenderShadow(_Info);
		}
	} };

	_InitRenderProp.RenderOrders[RenderProperty::Order::Debug]
		=
	{
		{"Debug" ,
		[this](const DrawInfo& _Info)
		{
			RenderDebug(_Info);
		}
	} };

	RenderInterface::Initialize(_InitRenderProp);

	
		 // L"C:\\WorkingDirectory\\TestResource\\Map\\Building01.fbx"
	// ����ƽ �޽� �ε�
	_StaticMesh = Resources::Load<ENGINE::StaticMesh>(
		"C:\\WorkingDirectory\\TestResource\\Map\\Building01.fbx");

	PushEditEntity(_StaticMesh.get());
}


void TestObject::RenderGBuffer(const DrawInfo& _Info)
{
	const Matrix World = _RenderUpdateInfo.World;
	_Info.Fx->SetMatrix("matWorld", &World);
	const uint32 Numsubset =_StaticMesh->GetNumSubset();
	for (uint32 i = 0; i < Numsubset; ++i)
	{
		if (auto SpSubset = _StaticMesh->GetSubset(i).lock();
			SpSubset)
		{
			SpSubset->BindProperty(TextureType::DIFFUSE, 0, 0, _Info._Device);
			SpSubset->BindProperty(TextureType::NORMALS, 0, 1, _Info._Device);
			SpSubset->Render(_Info.Fx);
		};
	};
}
void TestObject::RenderShadow(const DrawInfo& _Info)
{
	const Matrix World = _RenderUpdateInfo.World;
	_Info.Fx->SetMatrix("matWorld", &World);
	const uint32 Numsubset = _StaticMesh->GetNumSubset();
	for (uint32 i = 0; i < Numsubset; ++i)
	{
		if (auto SpSubset = _StaticMesh->GetSubset(i).lock();
			SpSubset)
		{
			SpSubset->Render(_Info.Fx);
		};
	};
}


void TestObject::RenderDebug(const DrawInfo& _Info)
{
	const Matrix World = _RenderUpdateInfo.World;
	_Info.Fx->SetMatrix("World", &World);
	const uint32 Numsubset = _StaticMesh->GetNumSubset();
	for (uint32 i = 0; i < Numsubset; ++i)
	{
		if (auto SpSubset = _StaticMesh->GetSubset(i).lock();
			SpSubset)
		{
			SpSubset->Render(_Info.Fx);
		};
	};
};


HRESULT TestObject::Ready()
{
	
	// Ʈ������ �ʱ�ȭ .. 
	auto InitTransform = GetComponent<ENGINE::Transform>();
	InitTransform.lock()->SetScale({ 0.0005,0.0005,0.0005 });
	PushEditEntity(InitTransform.lock().get());
	RenderInit();
	// �������� ������ �ް���� ������Ʈ�� Raw �����ͷ� Ǫ��.

	return S_OK;
};

HRESULT TestObject::Awake()
{
	
	return S_OK;
}

HRESULT TestObject::Start()
{
	return S_OK;
}

UINT TestObject::Update(const float _fDeltaTime)
{

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
