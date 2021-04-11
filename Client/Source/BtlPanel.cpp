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

	_ImplInfo.Fx->SetMatrix("Perspective", &_PerspectiveProjMatrix);
	_ImplInfo.Fx->SetTexture("NoiseMap", _NoiseTex->GetTexture());
	_ImplInfo.Fx->SetFloatArray("LightDirection", _LightDir, 3u);

	auto WeakSubset = _PlaneMesh->GetSubset(0u);
	if (auto SharedSubset = WeakSubset.lock();
		SharedSubset)
	{
		//
		CurID = REDORB;
		if (_UIDescs[CurID].Using)
		{
			_ImplInfo.Fx->SetTexture("ALB0Map", _RedOrbALBMTex->GetTexture());
			_ImplInfo.Fx->SetTexture("ATOS0Map", _RedOrbATOSTex->GetTexture());
			_ImplInfo.Fx->SetTexture("NRMR0Map", _RedOrbNRMRTex->GetTexture());

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
			_ImplInfo.Fx->SetTexture("ATOS0Map", _TargetCursorTex->GetTexture());
			_ImplInfo.Fx->SetFloat("_AccumulationTexV", _AccumulateTime * 0.3f);

			for (int i = 0; i < 3; ++i)
			{
				Create_ScreenMat(CurID, ScreenMat, i);
				_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);

				_ImplInfo.Fx->BeginPass(1);
				SharedSubset->Render(_ImplInfo.Fx);
				_ImplInfo.Fx->EndPass();
			}
		}

		//
		CurID = BOSS_GUAGE;
		if (_UIDescs[CurID].Using)
		{
			_ImplInfo.Fx->SetTexture("ATOS0Map", _BossGaugeATOSTex->GetTexture());
			_ImplInfo.Fx->SetTexture("NRMR0Map", _BossGaugeNRMRTex->GetTexture());

			_ImplInfo.Fx->SetFloat("_HP_Degree", _TargetHP_Degree);
			_ImplInfo.Fx->SetFloat("_BossGaugeCurXPosOrtho", _BossGauge_CurXPosOrtho);

			Create_ScreenMat(CurID, ScreenMat);
			_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);

			_ImplInfo.Fx->BeginPass(3);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();

			_ImplInfo.Fx->BeginPass(4);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();

			_ImplInfo.Fx->BeginPass(5);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();

			_ImplInfo.Fx->BeginPass(6);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}

		//
		CurID = EX_GAUGE_BACK;
		if (_UIDescs[CurID].Using)
		{
			_ImplInfo.Fx->SetTexture("ALB0Map", _ExBackALBMTex->GetTexture());
			_ImplInfo.Fx->SetTexture("ATOS0Map", _ExBackATOSTex->GetTexture());
			_ImplInfo.Fx->SetTexture("NRMR0Map", _ExBackNRMRTex->GetTexture());

			Create_ScreenMat(CurID, ScreenMat);
			_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);

			_ImplInfo.Fx->BeginPass(8);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}
	}

	//
	CurID = TARGET_HP;
	if (_UIDescs[CurID].Using)
	{
		auto WeakSubset = _Pipe0Mesh->GetSubset(0u);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{

			_ImplInfo.Fx->SetTexture("ATOS0Map", _EnemyHPTex->GetTexture());
			_ImplInfo.Fx->SetFloat("_TotalAccumulateTime", _TotalAccumulateTime);
			
			_ImplInfo.Fx->SetFloat("_HP_Degree", _TargetHP_Degree);
			_ImplInfo.Fx->SetFloatArray("_HP_StartPt", _TargetHP_StartPtOrtho, 2u);
			_ImplInfo.Fx->SetFloatArray("_HP_Normal0", _TargetHP_Normal0, 2u);
			_ImplInfo.Fx->SetFloatArray("_HP_Normal1", _TargetHP_Normal1, 2u);

			Create_ScreenMat(CurID, ScreenMat);
			_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);

			_ImplInfo.Fx->BeginPass(2);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}
	}

	//
	CurID = HP_GLASS;
	if (_UIDescs[CurID].Using)
	{
		auto WeakSubset = _HPGlassMesh->GetSubset(0u);
		if (auto SharedSubset = WeakSubset.lock();
			SharedSubset)
		{
			_ImplInfo.Fx->SetTexture("ATOS0Map", _HPGlassATOSTex->GetTexture());
			_ImplInfo.Fx->SetTexture("NRMR0Map", _HPGlassNRMRTex->GetTexture());
			_ImplInfo.Fx->SetTexture("ALB0Map", _GlassTex->GetTexture());
			_ImplInfo.Fx->SetTexture("ALB1Map", _BloodTex->GetTexture());
			_ImplInfo.Fx->SetFloat("_HPGlassDirt", _HPGlassDirt);

			Create_ScreenMat(CurID, ScreenMat);
			_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);

			_ImplInfo.Fx->BeginPass(7);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}
	}

	//
	CurID = EX_GAUGE;
	if (_UIDescs[CurID].Using)
	{
		_ImplInfo.Fx->SetTexture("ALB0Map", _ExALBM1Tex->GetTexture());
		_ImplInfo.Fx->SetTexture("NRMR0Map", _ExNRMR1Tex->GetTexture());

		auto WeakSubset0 = _Ex0Mesh->GetSubset(1u);
		auto WeakSubset1 = _Ex1Mesh->GetSubset(1u);
		auto WeakSubset2 = _Ex2Mesh->GetSubset(1u);

		if (auto SharedSubset = WeakSubset2.lock();
			SharedSubset)
		{
			Create_ScreenMat(CurID, ScreenMat, 2);
			_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);
			_ImplInfo.Fx->BeginPass(9);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}
		if (auto SharedSubset = WeakSubset1.lock();
			SharedSubset)
		{
			Create_ScreenMat(CurID, ScreenMat, 1);
			_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);
			_ImplInfo.Fx->BeginPass(9);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}
		if (auto SharedSubset = WeakSubset0.lock();
			SharedSubset)
		{
			Create_ScreenMat(CurID, ScreenMat, 0);
			_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);
			_ImplInfo.Fx->BeginPass(9);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}		
		
		_ImplInfo.Fx->SetTexture("ALB0Map", _ExALBM0Tex->GetTexture());
		_ImplInfo.Fx->SetTexture("NRMR0Map", _ExNRMR0Tex->GetTexture());

		WeakSubset0 = _Ex0Mesh->GetSubset(0u);
		WeakSubset1 = _Ex1Mesh->GetSubset(0u);
		WeakSubset2 = _Ex2Mesh->GetSubset(0u);

		if (auto SharedSubset = WeakSubset2.lock();
			SharedSubset)
		{
			Create_ScreenMat(CurID, ScreenMat, 2);
			_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);
			_ImplInfo.Fx->BeginPass(9);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}
		if (auto SharedSubset = WeakSubset1.lock();
			SharedSubset)
		{
			Create_ScreenMat(CurID, ScreenMat, 1);
			_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);
			_ImplInfo.Fx->BeginPass(9);
			SharedSubset->Render(_ImplInfo.Fx);
			_ImplInfo.Fx->EndPass();
		}
		if (auto SharedSubset = WeakSubset0.lock();
			SharedSubset)
		{
			Create_ScreenMat(CurID, ScreenMat, 0);
			_ImplInfo.Fx->SetMatrix("ScreenMat", &ScreenMat);
			_ImplInfo.Fx->BeginPass(9);
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
	_InitRenderProp.RenderOrders =
	{
		ENGINE::RenderProperty::Order::UI,
		//ENGINE::RenderProperty::Order::Debug
	};
	RenderInterface::Initialize(_InitRenderProp);

	_ShaderInfo.RegistShader(ENGINE::RenderProperty::Order::UI,
		L"..\\..\\Resource\\Shader\\UI\\BtlPanel.hlsl", {});

	_PlaneMesh = Resources::Load<ENGINE::StaticMesh>(L"..\\..\\Resource\\Mesh\\Static\\mesh_primitive\\plane00.fbx");
	_Pipe0Mesh = Resources::Load<ENGINE::StaticMesh>(L"..\\..\\Resource\\Mesh\\Static\\mesh_primitive\\pipe00.fbx");

	_NoiseTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\Effect\\noiseInput_ATOS.tga");

	_RedOrbALBMTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\red_orb_albm.tga");
	_RedOrbATOSTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\red_orb_atos.tga");
	_RedOrbNRMRTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\red_orb_nrmr.tga");

	_TargetCursorTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\Cursor_MET.tga");
	_EnemyHPTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\Enemy_HP_Target_01_IM.tga");

	_BossGaugeATOSTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\BossGauge_ATOS.tga");
	_BossGaugeNRMRTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\BossGauge_NRMR.tga");
	
	_HPGlassMesh = Resources::Load<ENGINE::StaticMesh>(L"..\\..\\Resource\\Mesh\\Static\\UI\\hud_hp_glass.fbx");
	_HPGlassATOSTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\HUD_Nero_HP_Glass_ATOS.tga");
	_HPGlassNRMRTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\HUD_Nero_HP_Glass_NRMR.tga");
	_GlassTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\HP_IL_A_ALB.tga");
	_BloodTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\BloodStoneCH16.png");

	_Ex0Mesh = Resources::Load<ENGINE::StaticMesh>(L"..\\..\\Resource\\Mesh\\Static\\UI\\hud_ex_01.fbx");
	_Ex1Mesh = Resources::Load<ENGINE::StaticMesh>(L"..\\..\\Resource\\Mesh\\Static\\UI\\hud_ex_02.fbx");
	_Ex2Mesh = Resources::Load<ENGINE::StaticMesh>(L"..\\..\\Resource\\Mesh\\Static\\UI\\hud_ex_03.fbx");
	_ExBackALBMTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\EXBack_ALBM.tga");
	_ExBackATOSTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\EXBack_ATOS.tga");
	_ExBackNRMRTex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\EXBack_NRMR.tga");
	_ExALBM0Tex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\Ex_ALBM.tga");
	_ExNRMR0Tex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\Ex_NRMR.tga");
	_ExALBM1Tex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\wp00_001_ALBM.tga");
	_ExNRMR1Tex = Resources::Load<ENGINE::Texture>(L"..\\..\\Resource\\Texture\\UI\\wp00_001_NRMR.tga");

	D3DXMatrixPerspectiveFovLH(&_PerspectiveProjMatrix, D3DXToRadian(2.5f), (float)g_nWndCX / g_nWndCY, 0.1f, 1.f);
		 
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
	_TotalAccumulateTime += _fDeltaTime;

	////////////////////////////
	// 임시
	if (Input::GetKey(DIK_LEFTARROW))
	{
		_TargetHP_Degree += 150.f * _fDeltaTime;
		if (360.f < _TargetHP_Degree)
			_TargetHP_Degree = 360.f;
	}
	if (Input::GetKey(DIK_RIGHTARROW))
	{
		_TargetHP_Degree -= 150.f * _fDeltaTime;
		if (0.f > _TargetHP_Degree)
			_TargetHP_Degree = 0.f;
	}
	////////////////////////////

	//
	Update_TargetInfo();

	//
	float _BossGaugeOrthoOffsetToCenter = 0.344f; // 직접 수작업으로 찾아야 하나 ㅠㅠ
	// + 적 체력 받아와서 degree 같은 애들 갱신하자
	// 일단 임시
	_BossGauge_CurXPosOrtho = -_BossGaugeOrthoOffsetToCenter + ((360.f - _TargetHP_Degree) / 360.f * 2.f * _BossGaugeOrthoOffsetToCenter);
	//std::cout << _BossGauge_CurXPosOrtho << std::endl;

	//
	//Imgui_ModifyUI(EX_GAUGE);

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
	_UIDescs[REDORB] = { true, Vector3(1090.f, 50.f, 0.5f), Vector3(0.55f, 0.55f, 1.f) };
	_UIDescs[TARGET_CURSOR] = { true, Vector3(640.f, 360.f, 0.02f), Vector3(0.3f, 0.3f, 1.f) };
	_UIDescs[TARGET_HP] = { true, Vector3(640.f, 360.f, 0.5f), Vector3(0.46f, 0.46f, 1.f) };
	_UIDescs[BOSS_GUAGE] = { true, Vector3(640.f, 670.f, 0.5f), Vector3(4.7f, 5.f, 1.f) };
	_UIDescs[HP_GLASS] = { true, Vector3(240.f, 155.f, 0.2f), Vector3(0.5f, 0.5f, 1.f) };
	_UIDescs[EX_GAUGE_BACK] = { true, Vector3(80.f, 110.f, 0.5f), Vector3(1.8f, 1.8f, 1.f) };
	_UIDescs[EX_GAUGE] = { true, Vector3(-7.55f, 3.15f, 15.f), Vector3(0.01f, 0.01f, 0.01f) };
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
			_Out._33 = _UIDescs[_ID].Scale.z;
			D3DXMatrixRotationZ(&RotMat, D3DXToRadian(120.f));
			_Out *= RotMat;
			_Out._41 = (_UIDescs[_ID].Pos.x - 35.f) - (g_nWndCX >> 1);
			_Out._42 = -((_UIDescs[_ID].Pos.y + 20.f) - (g_nWndCY >> 1));
			_Out._43 = _UIDescs[_ID].Pos.z;

		}
		else if (2 == _Opt)
		{
			_Out._11 = _UIDescs[_ID].Scale.x;
			_Out._22 = _UIDescs[_ID].Scale.y;
			_Out._33 = _UIDescs[_ID].Scale.z;
			D3DXMatrixRotationZ(&RotMat, D3DXToRadian(-120.f));
			_Out *= RotMat;
			_Out._41 = (_UIDescs[_ID].Pos.x + 35.f) - (g_nWndCX >> 1);
			_Out._42 = -((_UIDescs[_ID].Pos.y + 20.f) - (g_nWndCY >> 1));
			_Out._43 = _UIDescs[_ID].Pos.z;
		}
		else
		{
			_Out._11 = _UIDescs[_ID].Scale.x;
			_Out._22 = _UIDescs[_ID].Scale.y;
			_Out._33 = _UIDescs[_ID].Scale.z;
			_Out._41 = _UIDescs[_ID].Pos.x - (g_nWndCX >> 1);
			_Out._42 = -((_UIDescs[_ID].Pos.y - 40.f) - (g_nWndCY >> 1));
			_Out._43 = _UIDescs[_ID].Pos.z;
		}
		break;

	case HP_GLASS:
		_Out._11 = _UIDescs[_ID].Scale.x;
		_Out._22 = _UIDescs[_ID].Scale.z * 0.00001f; // y z 축 잘못뽑음 ㅠㅠ
		_Out._33 = _UIDescs[_ID].Scale.y;
		D3DXMatrixRotationX(&RotMat, D3DXToRadian(-90.f));
		_Out *= RotMat;
		_Out._41 = _UIDescs[_ID].Pos.x - (g_nWndCX >> 1);
		_Out._42 = -(_UIDescs[_ID].Pos.y - (g_nWndCY >> 1));
		_Out._43 = _UIDescs[_ID].Pos.z;
		break;

	case EX_GAUGE_BACK:
		_Out._11 = _UIDescs[_ID].Scale.x;
		_Out._22 = _UIDescs[_ID].Scale.y;
		_Out._33 = _UIDescs[_ID].Scale.z;
		D3DXMatrixRotationZ(&RotMat, D3DXToRadian(-45.f));
		_Out *= RotMat;
		_Out._41 = _UIDescs[_ID].Pos.x - (g_nWndCX >> 1);
		_Out._42 = -(_UIDescs[_ID].Pos.y - (g_nWndCY >> 1));
		_Out._43 = _UIDescs[_ID].Pos.z;
		break;

	case EX_GAUGE:
		if (1 == _Opt)
		{
			_Out._11 = _UIDescs[_ID].Scale.x;
			_Out._22 = _UIDescs[_ID].Scale.y;
			_Out._33 = _UIDescs[_ID].Scale.z;
			D3DXMatrixRotationX(&RotMat, D3DXToRadian(35.f));
			_Out *= RotMat;
			D3DXMatrixRotationY(&RotMat, D3DXToRadian(-95.f));
			_Out *= RotMat;
			D3DXMatrixRotationZ(&RotMat, D3DXToRadian(-10.f));
			_Out *= RotMat;
			_Out._41 = -8.2f; //_UIDescs[_ID].Pos.x;
			_Out._42 = 3.65f; //_UIDescs[_ID].Pos.y;
			_Out._43 = 15.5f; //_UIDescs[_ID].Pos.z;
		}
		else if (2 == _Opt)
		{
			_Out._11 = _UIDescs[_ID].Scale.x;
			_Out._22 = _UIDescs[_ID].Scale.y;
			_Out._33 = _UIDescs[_ID].Scale.z;
			D3DXMatrixRotationX(&RotMat, D3DXToRadian(21.f));
			_Out *= RotMat;
			D3DXMatrixRotationY(&RotMat, D3DXToRadian(-91.f));
			_Out *= RotMat;
			D3DXMatrixRotationZ(&RotMat, D3DXToRadian(17.f));
			_Out *= RotMat;
			_Out._41 = -9.075f; //_UIDescs[_ID].Pos.x;
			_Out._42 = 4.1f; //_UIDescs[_ID].Pos.y;
			_Out._43 = 16.f; //_UIDescs[_ID].Pos.z;
		}
		else // 0
		{
			_Out._11 = _UIDescs[_ID].Scale.x;
			_Out._22 = _UIDescs[_ID].Scale.y;
			_Out._33 = _UIDescs[_ID].Scale.z;
			D3DXMatrixRotationX(&RotMat, D3DXToRadian(180.f));
			_Out *= RotMat;
			D3DXMatrixRotationY(&RotMat, D3DXToRadian(-100.f));
			_Out *= RotMat;
			D3DXMatrixRotationZ(&RotMat, D3DXToRadian(-80.f));
			_Out *= RotMat;
			_Out._41 = -7.55f; //_UIDescs[_ID].Pos.x;
			_Out._42 = 3.15f; //_UIDescs[_ID].Pos.y;
			_Out._43 = 15.f; //_UIDescs[_ID].Pos.z;
		}
		break;

	default:
		_Out._11 = _UIDescs[_ID].Scale.x;
		_Out._22 = _UIDescs[_ID].Scale.y;
		_Out._33 = _UIDescs[_ID].Scale.z;
		_Out._41 = _UIDescs[_ID].Pos.x - (g_nWndCX >> 1);
		_Out._42 = -(_UIDescs[_ID].Pos.y - (g_nWndCY >> 1));
		_Out._43 = _UIDescs[_ID].Pos.z;
		break;
	}
}

void BtlPanel::Update_TargetInfo()
{
	Matrix RotMat;

	///////////////////////
	// 
	//m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	//D3DXVec3TransformCoord(&vTargetPos, &vTargetPos, &matView);
	//m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	//D3DXVec3TransformCoord(&vTargetPos, &vTargetPos, &matProj);
	//vTargetPos.x = (vTargetPos.x * WINCX * 0.5f) + WINCX * 0.5f;
	//vTargetPos.y = (vTargetPos.y * WINCY * -0.5f) + WINCY * 0.5f;
	// 
	// 임시로 마우스 위치를 타겟 위치로 지정
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	_TargetPos = Vector3(static_cast<float>(pt.x), static_cast<float>(pt.y), _UIDescs[TARGET_CURSOR].Pos.z);
	_UIDescs[TARGET_CURSOR].Pos = _TargetPos;
	_UIDescs[TARGET_HP].Pos = _TargetPos;
	
	//Vector2 v = ScreenPosToOrtho(_TargetPos.x, _TargetPos.y);
	//std::cout << v.x << " " << v.y << std::endl;
	///////////////////////
	
	_TargetHP_StartPtOrtho = ScreenPosToOrtho(_TargetPos.x, _TargetPos.y);
	Vector2 Offset = Vector2(0.f, -100.f);	// EndPt offset (처음은 12시방향. 방향이 중요한거라 값의 크기는 관계 없는듯)

	Vector2 EndPtOrtho = ScreenPosToOrtho(_TargetPos.x + Offset.x, _TargetPos.y + Offset.y);
	Vector2 Dir = EndPtOrtho - _TargetHP_StartPtOrtho;
	_TargetHP_Normal0 = Vector2(-Dir.y, Dir.x);
	D3DXVec2Normalize(&_TargetHP_Normal0, &_TargetHP_Normal0);

	// offset 회전
	D3DXMatrixRotationZ(&RotMat, D3DXToRadian(_TargetHP_Degree));
	D3DXVec2TransformCoord(&Offset, &Offset, &RotMat);

	EndPtOrtho = ScreenPosToOrtho(_TargetPos.x + Offset.x, _TargetPos.y + Offset.y);
	Dir = EndPtOrtho - _TargetHP_StartPtOrtho;
	_TargetHP_Normal1 = Vector2(-Dir.y, Dir.x);
	D3DXVec2Normalize(&_TargetHP_Normal1, &_TargetHP_Normal1);
}

Vector2 BtlPanel::ScreenPosToOrtho(float _ScreenPosX, float _ScreenPosY)
{
	Vector2 Ret = Vector2(_ScreenPosX - (g_nWndCX >> 1), -(_ScreenPosY - (g_nWndCY >> 1)));	
	D3DXVec2TransformCoord(&Ret, &Ret, &Renderer::GetInstance()->CurrentRenderInfo.Ortho);
	return Ret;
}

void BtlPanel::Imgui_ModifyUI(UI_DESC_ID _ID)
{
	Vector3 Position = _UIDescs[_ID].Pos;
	ImGui::InputFloat3("Position", Position);
	_UIDescs[_ID].Pos = Position;

	Vector3 Scale = _UIDescs[_ID].Scale;
	ImGui::InputFloat3("Scale", Scale);
	_UIDescs[_ID].Scale = Scale;

	Vector3 Rot = _Rot;
	ImGui::InputFloat3("Rot", Rot);
	_Rot = Rot;
	
	Vector3 Vector = _LightDir;
	ImGui::SliderFloat3("LightVec", Vector, -1.f, 1.f);
	_LightDir = Vector;

	float HPGlassDirt = _HPGlassDirt;
	ImGui::SliderFloat("GlassDirt", &HPGlassDirt, 0.f, 1.f);
	_HPGlassDirt = HPGlassDirt;
}