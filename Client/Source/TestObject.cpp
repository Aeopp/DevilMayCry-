#include "..\Header\TestObject.h"
#include "StaticMesh.h"

void TestObject::Free()
{
}

HRESULT TestObject::Ready()
{
	/*auto _StaticMesh = AddComponent<ENGINE::StaticMesh>();
	if (auto _SharedStaticMesh = _StaticMesh.lock();
		_SharedStaticMesh)
	{
		_SharedStaticMesh->LoadMeshFromFile()
	}*/

	return S_OK;
}

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

HRESULT TestObject::Render()
{
	return S_OK;
}

void TestObject::OnEnable()
{
}

void TestObject::OnDisable()
{
}
