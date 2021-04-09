#include "stdafx.h"
#include "..\Header\TestScene.h"
#include "Scene.h"
#include "TestObject.h"
#include "ShaderTester.h"
#include "TestAnimationObject.h"
#include "Eff_OvertureHand.h"
#include "Eff_Glint.h"
#include "BtlPanel.h"
#include "Camera.h"
#include "Nero.h"
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

	AddGameObject<Camera>();

	//AddGameObject<ShaderTester>();

	// AddGameObject<TestObject>();
	AddGameObject<TestAnimationObject>();
	//AddGameObject<Nero>();
	/*AddGameObject<TestAnimationObject>();
	AddGameObject<TestAnimationObject>();
	AddGameObject<TestAnimationObject>();
	AddGameObject<TestAnimationObject>();
	AddGameObject<TestAnimationObject>();
	AddGameObject<TestAnimationObject>();
	AddGameObject<TestAnimationObject>();
	AddGameObject<TestAnimationObject>();
	AddGameObject<TestAnimationObject>();*/

	/*AddGameObject<Eff_OvertureHand>();
	AddGameObject<BtlPanel>();
	AddGameObject<Eff_Glint>();*/

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

	//cout << "SceneUpdate" << endl;

	return S_OK;
}

HRESULT TestScene::LateUpdate(const float _fDeltaTime)
{
	Scene::LateUpdate(_fDeltaTime);
	return S_OK;
}
