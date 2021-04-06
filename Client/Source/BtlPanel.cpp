#include "stdafx.h"
#include "..\Header\BtlPanel.h"
#include "Transform.h"
#include "Subset.h"
#include "TextureType.h"
#include "Renderer.h"


void BtlPanel::Free()
{
	SafeDeleteArray(_UIDescs);
}

std::string BtlPanel::GetName()
{
	return "BtlPanel";
}

BtlPanel* BtlPanel::Create()
{
	return new BtlPanel{};
}


void BtlPanel::RenderUIImplementation(const ImplementationInfo& _ImplInfo)
{
	UI_DESC_ID CurID = DESC_END;
	Matrix ScreenMat;

	auto WeakSubset_Plane = _PlaneMesh->GetSubset(0u);

	if (auto SharedSubset = WeakSubset_Plane.lock();
		SharedSubset)
	{
		// RedOrb
		CurID = REDORB;
		if (_UIDescs[CurID].Using)
		{
			_ImplInfo.Fx->SetFloatArray("LightDirection", Vector3(-1.f, 0.f, 0.f), 3u);
			Create_ScreenMat(CurID, ScreenMat);
			_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);
			_ImplInfo.Fx->SetTexture("RedOrbALBMMap", _RedOrbALBMTex->GetTexture());
			_ImplInfo.Fx->SetTexture("RedOrbATOSMap", _RedOrbATOSTex->GetTexture());
			_ImplInfo.Fx->SetTexture("RedOrbNRMRMap", _RedOrbNRMRTex->GetTexture());

			_ImplInfo.Fx->BeginPass(0);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}
	}
};

void BtlPanel::RenderReady()
{

}

HRESULT BtlPanel::Ready()
{
	SetRenderEnable(true);

	ENGINE::RenderProperty _InitRenderProp;
	_InitRenderProp.bRender = true;
	_InitRenderProp._Order = ENGINE::RenderProperty::Order::UI;
	RenderInterface::Initialize(_InitRenderProp);

	_ShaderInfo.UIShader = Resources::Load<ENGINE::Shader>(L"..\\..\\Resource\\Shader\\UI\\BtlPanel.hlsl");

	//auto InitTransform = AddComponent<ENGINE::Transform>();
	//InitTransform.lock()->SetScale({ 0.01f, 0.01f, 0.01f });

	_PlaneMesh = Resources::Load<ENGINE::StaticMesh>(L"..\\..\\Resource\\Mesh\\Static\\mesh_primitive\\plane00.fbx");
	_RedOrbALBMTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\red_orb_albm.tga");
	_RedOrbATOSTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\red_orb_atos.tga");
	_RedOrbNRMRTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\red_orb_nrmr.tga");

	Init_UIDescs();
	
	return S_OK;
}

HRESULT BtlPanel::Awake()
{
	return S_OK;
}

HRESULT BtlPanel::Start()
{
	return S_OK;
}

UINT BtlPanel::Update(const float _fDeltaTime)
{
	Imgui_Modify_UIPosAndScale(REDORB);

	return 0;
}

UINT BtlPanel::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

void BtlPanel::Editor()
{
	GameObject::Editor();
}

void BtlPanel::OnEnable()
{

}

void BtlPanel::OnDisable()
{

}


void BtlPanel::Init_UIDescs()
{
	if (!_UIDescs)
		_UIDescs = new UI_DESC[DESC_END];

	// Using, Pos, Scale
	_UIDescs[REDORB] = { true, Vector2(1090.f, 50.f), Vector2(0.55f, 0.55f) };
}

void BtlPanel::Create_ScreenMat(UI_DESC_ID _ID, Matrix& _Out)
{
	D3DXMatrixIdentity(&_Out);

	_Out._11 = _UIDescs[_ID].Scale.x;
	_Out._22 = _UIDescs[_ID].Scale.y;
	_Out._33 = 1.f;
	_Out._41 = _UIDescs[_ID].Pos.x - (g_nWndCX >> 1);
	_Out._42 = -(_UIDescs[_ID].Pos.y - (g_nWndCY >> 1));
	_Out._43 = 0.02f;	// 0.01Àº ¾ÈµÊ ¤Ð¤Ð
}

void BtlPanel::Imgui_Modify_UIPosAndScale(UI_DESC_ID _ID)
{
	Vector2 Position = _UIDescs[_ID].Pos;
	ImGui::InputFloat2("Position", Position);
	_UIDescs[_ID].Pos = Position;

	Vector2 Scale = _UIDescs[_ID].Scale;
	ImGui::InputFloat2("Scale", Scale);
	_UIDescs[_ID].Scale = Scale;
}