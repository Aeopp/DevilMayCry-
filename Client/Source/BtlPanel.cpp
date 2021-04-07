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

	_ImplInfo.Fx->SetTexture("NoiseMap", _NoiseTex->GetTexture());

	auto WeakSubset_Plane = _PlaneMesh->GetSubset(0u);
	if (auto SharedSubset = WeakSubset_Plane.lock();
		SharedSubset)
	{
		//
		CurID = REDORB;
		if (_UIDescs[CurID].Using)
		{
			_ImplInfo.Fx->SetFloatArray("LightDirection", Vector3(-1.f, 0.f, 0.f), 3u);

			_ImplInfo.Fx->SetTexture("RedOrbALBMMap", _RedOrbALBMTex->GetTexture());
			_ImplInfo.Fx->SetTexture("RedOrbATOSMap", _RedOrbATOSTex->GetTexture());
			_ImplInfo.Fx->SetTexture("RedOrbNRMRMap", _RedOrbNRMRTex->GetTexture());

			Create_ScreenMat(CurID, ScreenMat);
			_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);

			_ImplInfo.Fx->BeginPass(0);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}

		//
		CurID = TARGET_CURSOR;
		if (_UIDescs[CurID].Using)
		{
			_ImplInfo.Fx->SetTexture("TargetCursorMap", _TargetCursorTex->GetTexture());
			_ImplInfo.Fx->SetFloat("_AccumulationTexV", _AccumulateTime * 0.1f);

			for (int i = 0; i < 3; ++i)
			{
				Create_ScreenMat(CurID, ScreenMat, i);
				_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);

				_ImplInfo.Fx->BeginPass(1);
				SharedSubset->Render(_ImplInfo.Fx);
				_ImplInfo.Fx->EndPass();
			}
		}
	}

	auto WeakSubset_Pipe0 = _Pipe0Mesh->GetSubset(0u);
	if (auto SharedSubset = WeakSubset_Pipe0.lock();
		SharedSubset)
	{
		//
		CurID = TARGET_HP;
		if (_UIDescs[CurID].Using)
		{
			_ImplInfo.Fx->SetTexture("TargetHPMap", _EnemyHPTex->GetTexture());
			_ImplInfo.Fx->SetFloat("_AccumulationTexV", _AccumulateTime * 0.1f);

			Create_ScreenMat(CurID, ScreenMat);
			_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);

			_ImplInfo.Fx->BeginPass(2);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}
	}

	auto WeakSubset_Pipe1 = _Pipe1Mesh->GetSubset(0u);
	if (auto SharedSubset = WeakSubset_Pipe1.lock();
		SharedSubset)
	{
		//
		CurID = TARGET_HP;
		if (_UIDescs[CurID].Using)
		{
			_ImplInfo.Fx->SetFloat("_AccumulationTexV", _AccumulateTime * 0.5f);

			Create_ScreenMat(CurID, ScreenMat, 1);
			_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);

			_ImplInfo.Fx->BeginPass(3);
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

	_PlaneMesh = Resources::Load<ENGINE::StaticMesh>(L"..\\..\\Resource\\Mesh\\Static\\mesh_primitive\\plane00.fbx");
	_Pipe0Mesh = Resources::Load<ENGINE::StaticMesh>(L"..\\..\\Resource\\Mesh\\Static\\mesh_primitive\\pipe00.fbx");
	_Pipe1Mesh = Resources::Load<ENGINE::StaticMesh>(L"..\\..\\Resource\\Mesh\\Static\\mesh_primitive\\pipe01.fbx");

	_NoiseTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\Effect\\noiseInput_ATOS.tga");

	_RedOrbALBMTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\red_orb_albm.tga");
	_RedOrbATOSTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\red_orb_atos.tga");
	_RedOrbNRMRTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\red_orb_nrmr.tga");

	_TargetCursorTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\Cursor_MET.tga");

	_EnemyHPTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\Enemy_HP_Target_01_IM.tga");

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
	_AccumulateTime += _fDeltaTime;

	///////////////////////
	//임시로 마우스 위치를 타겟 위치로 지정
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	Vector2 ptToV2 = { static_cast<float>(pt.x), static_cast<float>(pt.y) };
	_UIDescs[TARGET_CURSOR].Pos = ptToV2;
	_UIDescs[TARGET_HP].Pos = ptToV2;
	///////////////////////
	// 
	//Imgui_Modify_UIPosAndScale(TARGET_HP);

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
	_UIDescs[TARGET_CURSOR] = { true, Vector2(640.f, 360.f), Vector2(0.3f, 0.3f) };
	_UIDescs[TARGET_HP] = { true, Vector2(640.f, 360.f), Vector2(0.46f, 0.46f) };
}

void BtlPanel::Create_ScreenMat(UI_DESC_ID _ID, Matrix& _Out, int _Opt/*= 0*/)
{
	Matrix RotMat;

	D3DXMatrixIdentity(&_Out);

	switch (_ID)
	{
	case TARGET_CURSOR:
		if (1 == _Opt)
		{
			_Out._11 = _UIDescs[_ID].Scale.x;
			_Out._22 = _UIDescs[_ID].Scale.y;
			_Out._33 = 1.f;
			D3DXMatrixRotationZ(&RotMat, D3DXToRadian(120.f));
			_Out *= RotMat;
			_Out._41 = (_UIDescs[_ID].Pos.x - 35.f) - (g_nWndCX >> 1);
			_Out._42 = -((_UIDescs[_ID].Pos.y + 20.f) - (g_nWndCY >> 1));
			_Out._43 = 0.02f;

		}
		else if (2 == _Opt)
		{
			_Out._11 = _UIDescs[_ID].Scale.x;
			_Out._22 = _UIDescs[_ID].Scale.y;
			_Out._33 = 1.f;
			D3DXMatrixRotationZ(&RotMat, D3DXToRadian(-120.f));
			_Out *= RotMat;
			_Out._41 = (_UIDescs[_ID].Pos.x + 35.f) - (g_nWndCX >> 1);
			_Out._42 = -((_UIDescs[_ID].Pos.y + 20.f) - (g_nWndCY >> 1));
			_Out._43 = 0.02f;
		}
		else
		{
			_Out._11 = _UIDescs[_ID].Scale.x;
			_Out._22 = _UIDescs[_ID].Scale.y;
			_Out._33 = 1.f;
			_Out._41 = _UIDescs[_ID].Pos.x - (g_nWndCX >> 1);
			_Out._42 = -((_UIDescs[_ID].Pos.y - 40.f) - (g_nWndCY >> 1));
			_Out._43 = 0.02f;
		}
		break;

	case TARGET_HP:
		if (1 == _Opt)
		{
			_Out._11 = _UIDescs[_ID].Scale.x * 0.9f;
			_Out._22 = _UIDescs[_ID].Scale.y * 0.9f;
			_Out._33 = 1.f;
			_Out._41 = _UIDescs[_ID].Pos.x - (g_nWndCX >> 1);
			_Out._42 = -(_UIDescs[_ID].Pos.y - (g_nWndCY >> 1));
			_Out._43 = 0.02f;
		}
		else
		{
			goto DEFAULT;
		}
		break;

	default: DEFAULT:
		_Out._11 = _UIDescs[_ID].Scale.x;
		_Out._22 = _UIDescs[_ID].Scale.y;
		_Out._33 = 1.f;
		_Out._41 = _UIDescs[_ID].Pos.x - (g_nWndCX >> 1);
		_Out._42 = -(_UIDescs[_ID].Pos.y - (g_nWndCY >> 1));
		_Out._43 = 0.02f;	// 0.01로는 안됨...
		break;
	}
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