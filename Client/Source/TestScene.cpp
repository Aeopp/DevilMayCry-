#include "stdafx.h"
#include "..\Header\TestScene.h"
#include "Scene.h"
#include "TestObject.h"
#include "ShaderTester.h"
#include "TestAnimationObject.h"
#include <iostream>
#include "Camera.h"
using namespace std;

TestScene::TestScene()
{
}

void TestScene::Free()
{
	Scene::Free();
}

TestScene* TestScene::Create()
{
	TestScene* pInstance = new TestScene;
	return pInstance;
}

HRESULT TestScene::LoadScene()
{
	Scene::LoadScene();

	AddGameObject<Camera>();
	AddGameObject<TestObject>();
	
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>(); 
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();
	//AddGameObject<TestObject>();


	//AddGameObject<ShaderTester>();

	return S_OK;
}

HRESULT TestScene::Awake()
{
	Scene::Awake();
	return S_OK;
}

HRESULT TestScene::Start()
{
	Scene::Start();
	return S_OK;
}

HRESULT TestScene::Update(const float _fDeltaTime)
{
	Scene::Update(_fDeltaTime);
	return S_OK;
}

HRESULT TestScene::LateUpdate(const float _fDeltaTime)
{
	Scene::LateUpdate(_fDeltaTime);
	return S_OK;
}
