#include "stdafx.h"
#include "..\Header\TestObject.h"


HRESULT TestObject::Render()
{
	

	return S_OK;
}

void TestObject::Free()
{

};

TestObject* TestObject::Create()
{
	return new TestObject{};
}

HRESULT TestObject::Ready()
{

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
	ImGui::Begin("Test");

	

	ImGui::End();
	return 0;
}

UINT TestObject::LateUpdate(const float _fDeltaTime)
{
	return 0;
}


void TestObject::OnEnable()
{
}

void TestObject::OnDisable()
{
}
