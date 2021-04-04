#include "stdafx.h"

#include "..\Header\TestScene.h"
#include "Scene.h"
#include "TestObject.h"
#include "ShaderTester.h"

#include <iostream>
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

	AddGameObject<TestObject>();
	AddGameObject<TestObject>().lock()->GetComponent<ENGINE::Transform>().lock()->SetPosition({ 1,0,0 });
	AddGameObject<TestObject>().lock()->GetComponent<ENGINE::Transform>().lock()->SetPosition({ -1,0,0 });



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
	

	cout << "SceneUpdate" << endl;
	return S_OK;
}

HRESULT TestScene::LateUpdate(const float _fDeltaTime)
{
	Scene::LateUpdate(_fDeltaTime);
	return S_OK;
}
