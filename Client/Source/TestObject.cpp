#include "stdafx.h"
#include "..\Header\TestObject.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9mesh.h>
#include <fstream>
#include <ostream>
#include "RenderTarget.h"
#include "Shader.h"
#include <array>
#include "FMath.hpp"
#include "GraphicSystem.h"


D3DVIEWPORT9			oldviewport;
D3DVIEWPORT9			viewport;
D3DXVECTOR4				pixelsize(0, 0, 0, 1);
D3DXVECTOR4				texelsize(0, 0, 0, 1);
float					averageluminance = 0.1f;	// don't set this to zero!!!
float					adaptedluminance = 0.1f;	// don't set this to zero!!!
float					exposure = 0;
uint8_t					mousebuttons = 0;
int						currentafterimage = 0;
bool					drawafterimage = false;


LPD3DXMESH teapot = nullptr;
LPD3DXMESH skull = nullptr;
LPD3DXMESH knot = nullptr;
LPD3DXMESH skymesh = nullptr;
LPD3DXMESH mesh = nullptr;


LPDIRECT3DCUBETEXTURE9	environment = nullptr;		
// HDR environment
LPDIRECT3DCUBETEXTURE9	irradiance1 = nullptr;		
// preintegrated diffuse irradiance
LPDIRECT3DCUBETEXTURE9	irradiance2 = nullptr;		// preintegrated specular irradiance
IDirect3DTexture9* brdfLUT = nullptr;
// preintegrated BRDF lookup texture

std::shared_ptr<ENGINE::Shader> sky{};
std::shared_ptr<ENGINE::Shader> metaleffect{};
std::shared_ptr<ENGINE::Shader> insulatoreffect{};
std::shared_ptr<ENGINE::Shader>  effect{};
std::shared_ptr<ENGINE::Shader> measureeffect{};
std::shared_ptr<ENGINE::Shader> hdreffects{};
std::shared_ptr<ENGINE::Shader> screenquad{};
RenderTarget scenetarget{  };
RenderTarget avgluminance{  };
RenderTarget avglumsystemmem{  };
RenderTarget bloomresult{};
RenderTarget starresult{};

std::array<RenderTarget,5u> dsampletargets;
std::array<RenderTarget, 5u>blurtargets; 
std::array<std::array<RenderTarget, 2u>, 4u> startargets;
std::array<RenderTarget, 2u>		lensflaretargets; 
std::array<RenderTarget, 2u>		afterimagetargets;

HRESULT TestObject::Render()
{
	Matrix Identity{};
	Matrix world, worldinv;
	Matrix rotation;
	Vector4 silver = {0.972f,0.96f,0.915f, 1.0f};

	Vector4 insulator = FMath::Color::sRGBToLinear(255, 255, 255);

	Matrix	view, proj;
	Matrix	skyview, viewproj;
	Vector4	eye;
	Vector3	orient;

	D3DXMatrixIdentity(&Identity);

	IDirect3DDevice9* device = GraphicSystem::GetInstance()->GetDevice();
	device->SetRenderTarget(0, scenetarget.GetSurface(0u ));
	// 감마보정을 비활성화 한다 . (기본값도 false 이긴 함)
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);
	{
		// render sky
		skyview = view;
		skyview._41 = skyview._42 = skyview._43 = 0;

		Math::MatrixMultiply(viewproj, skyview, proj);

		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		skyeffect->SetMatrix("matViewProj", (D3DXMATRIX*)&viewproj);
		skyeffect->SetMatrix("matSkyRotation", &identity);

		skyeffect->Begin(NULL, 0);
		skyeffect->BeginPass(0);
		{
			device->SetTexture(0, environment);
			skymesh->DrawSubset(0);
		}
		skyeffect->EndPass();
		skyeffect->End();

		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// render object
		if (mesh == teapot) {
			D3DXMatrixScaling(&world, 1.5f, 1.5f, 1.5f);
		}
		else if (mesh == knot) {
			D3DXMatrixScaling(&world, 0.8f, 0.8f, 0.8f);
		}
		else if (mesh == skull) {
			D3DXMatrixScaling(&world, 0.4f, 0.4f, 0.4f);
			world._42 = -1.5f;
		}

		D3DXMatrixRotationYawPitchRoll(&rotation, orient.x, orient.y, 0);
		D3DXMatrixMultiply(&world, &world, &rotation);
		D3DXMatrixInverse(&worldinv, NULL, &world);

		Math::MatrixMultiply(viewproj, view, proj);

		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		device->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		device->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		device->SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		device->SetSamplerState(2, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		device->SetSamplerState(2, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		effect->SetMatrix("matWorld", &world);
		effect->SetMatrix("matWorldInv", &worldinv);
		effect->SetMatrix("matViewProj", (D3DXMATRIX*)&viewproj);
		effect->SetVector("eyePos", (D3DXVECTOR4*)&eye);

		if (effect == metaleffect) {
			effect->SetVector("baseColor", &silver);
			effect->SetFloat("roughness", 0.0f);

			device->SetTexture(0, irradiance2);
			device->SetTexture(1, brdfLUT);
		}
		else {
			effect->SetVector("baseColor", (D3DXVECTOR4*)&insulator);
			effect->SetFloat("roughness", 0.2f);

			device->SetTexture(0, irradiance1);
			device->SetTexture(1, irradiance2);
			device->SetTexture(2, brdfLUT);
		}

		effect->Begin(NULL, 0);
		effect->BeginPass(0);
		{
			mesh->DrawSubset(0);
		}
		effect->EndPass();
		effect->End();

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	return S_OK;
}

void TestObject::Free()
{
	scenetarget.Release();
	avgluminance.Release();
	avglumsystemmem.Release();
	bloomresult.Release();
	starresult.Release();

	for (auto& DeleteTarget : dsampletargets)
	{
		DeleteTarget.Release();
	}
	for (auto& DeleteTarget : blurtargets)
	{
		DeleteTarget.Release();
	}
	for (auto& DeleteTargets : startargets)
	{
		for (auto& DeleteTarget : DeleteTargets)
		{
			DeleteTarget.Release();
		}
	}
	for (auto& DeleteTarget : lensflaretargets)
	{
		DeleteTarget.Release();
	}
	for (auto& DeleteTarget : afterimagetargets)
	{
		DeleteTarget.Release();
	}


	if (teapot)
		teapot->Release();

	teapot = nullptr;
	skull = nullptr;
	knot = nullptr;
	skymesh = nullptr;
	mesh = nullptr;
	environment = nullptr;
	irradiance1 = nullptr;
	irradiance2 = nullptr;
	brdfLUT = nullptr;
};

TestObject* TestObject::Create()
{
	return new TestObject{};
}

HRESULT TestObject::Ready()
{
	auto _StaticMesh = Resources::Load<StaticMesh>(TEXT("../../Resource/TestMesh/Move.fbx"));

	using namespace rapidjson;
	std::filesystem::path TargetPath = "..\\..\\Resource\\Dummy\\TestJson.json";
	StringBuffer StrBuf;
	PrettyWriter<StringBuffer> Writer(StrBuf);
	//  Cell Information Write...
	Writer.StartObject();
	Writer.Key("Test Json");
	Writer.StartArray();
	{
		{
			Writer.StartObject();
			{
				Writer.Key("Index");
				Writer.Uint(777u);

				Writer.Key("Name");
				Writer.String("Nero");

				Writer.Key("Value_1");
				Writer.Double(123.456f);

				Writer.Key("Value 2");
				Writer.Double(999.999f);
			}
			Writer.EndObject();
		}
		Writer.EndArray();

		Writer.EndObject();
		TargetPath.replace_extension("json");
		std::ofstream Of{ TargetPath };
		Of << StrBuf.GetString();
	};




	;
	sky = Resources::Load<Shader>(TEXT("../../Resource/TestDummy/ShadersDX/sky.fx"));
	metaleffect = Resources::Load<Shader>(TEXT("../../Resource/TestDummy/ShadersDX/metal.fx"));
	// insulator
	effect = insulatoreffect = Resources::Load<Shader>(TEXT("../../Resource/TestDummy/ShadersDX/insulator.fx"));
	// 측정 휘도 
	measureeffect = Resources::Load<Shader>(TEXT("../../Resource/TestDummy/ShadersDX/measureluminance.fx"));
	hdreffects = Resources::Load<Shader>(TEXT("../../Resource/TestDummy/ShadersDX/hdreffects.fx"));
	screenquad = Resources::Load<Shader>(TEXT("../../Resource/TestDummy/ShadersDX/screenquad.fx"));

	{
		ENGINE::RenderTarget::Info InitInfo{};
		InitInfo.Width = g_nWndCX;
		InitInfo.Height = g_nWndCY;
		InitInfo.Levels = 1;
		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
		InitInfo.Format = D3DFMT_A16B16G16R16F;
		InitInfo._D3DPool = D3DPOOL_DEFAULT;

		scenetarget.Initialize(InitInfo);
	}

	{
		ENGINE::RenderTarget::Info InitInfo{};
		InitInfo.Width = 64;
		InitInfo.Height = 64;
		InitInfo.Levels = 0;
		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
		InitInfo.Format = D3DFMT_R16F;
		InitInfo._D3DPool = D3DPOOL_DEFAULT;

		avgluminance.Initialize(InitInfo);
	}

	{
		ENGINE::RenderTarget::Info InitInfo{};
		InitInfo.Width = 1;
		InitInfo.Height = 1;
		InitInfo.Levels = 0;
		InitInfo.Usages = 0;
		InitInfo.Format = D3DFMT_R16F;
		InitInfo._D3DPool = D3DPOOL_SYSTEMMEM;

		avglumsystemmem.Initialize(InitInfo);
	}

	{
		// 평균 휘도 
		// 0 (초기화할때 획득), 2 , 4 , 6 밉맵 획득 .
		const uint32 SurfaceCount = 4;
		for (int i = 1; i < SurfaceCount; ++i)
		{
			avgluminance.InsertSurface(i * 2);
		}
	}

	{
		// 화면크기 절반으로 뭉개기 블룸 
		ENGINE::RenderTarget::Info InitInfo{};
		InitInfo.Width = g_nWndCX / 2;
		InitInfo.Height = g_nWndCY / 2;
		InitInfo.Levels = 1;
		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
		InitInfo.Format = D3DFMT_A16B16G16R16F;
		InitInfo._D3DPool = D3DPOOL_DEFAULT;

		bloomresult.Initialize(InitInfo);
	}

	{
		// 스타 효과 화면크기 1/4 로 뭉개기
		ENGINE::RenderTarget::Info InitInfo{};
		InitInfo.Width = g_nWndCX / 4;
		InitInfo.Height = g_nWndCY / 4;
		InitInfo.Levels = 1;
		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
		InitInfo.Format = D3DFMT_A16B16G16R16F;
		InitInfo._D3DPool = D3DPOOL_DEFAULT;

		starresult.Initialize(InitInfo);
	}

	for (int i = 0; i < 5; ++i)
	{
		{
			ENGINE::RenderTarget::Info DS_InitInfo;
			DS_InitInfo.Width = g_nWndCX / (2 << i);
			DS_InitInfo.Height = g_nWndCY / (2 << i);
			DS_InitInfo.Levels = 1;
			DS_InitInfo.Usages = D3DUSAGE_RENDERTARGET;
			DS_InitInfo.Format = D3DFMT_A16B16G16R16F;
			DS_InitInfo._D3DPool = D3DPOOL_DEFAULT;

			dsampletargets[i].Initialize(DS_InitInfo);
			
		}

		{
			ENGINE::RenderTarget::Info BT_InitInfo;
			BT_InitInfo.Width = g_nWndCX / (2 << i);
			BT_InitInfo.Height = g_nWndCY / (2 << i);
			BT_InitInfo.Levels = 1;
			BT_InitInfo.Usages = D3DUSAGE_RENDERTARGET;
			BT_InitInfo.Format = D3DFMT_A16B16G16R16F;
			BT_InitInfo._D3DPool = D3DPOOL_DEFAULT;

			blurtargets[i].Initialize(BT_InitInfo);
			
		}
	}

	// 스타 블러
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			{
				ENGINE::RenderTarget::Info InitInfo{};
				InitInfo.Width = g_nWndCX / 4;
				InitInfo.Height = g_nWndCY / 4;
				InitInfo.Levels = 1;
				InitInfo.Usages  = D3DUSAGE_RENDERTARGET; 
				InitInfo.Format = D3DFMT_A16B16G16R16F;
				InitInfo._D3DPool = D3DPOOL_DEFAULT;
				startargets[i][j].Initialize(InitInfo);

			}
			
		}
	}

	for (int i = 0; i < 2; ++i)
	{
		{
			ENGINE::RenderTarget::Info InitInfo{};
			InitInfo.Width = g_nWndCX / 2;
			InitInfo.Height = g_nWndCY / 2;
			InitInfo.Levels = 1;
			InitInfo.Format = D3DFMT_A16B16G16R16F;
			InitInfo.Usages = D3DUSAGE_RENDERTARGET;
			InitInfo._D3DPool = D3DPOOL_DEFAULT;

			
			lensflaretargets[i].Initialize(InitInfo);
	
		}

		{
			ENGINE::RenderTarget::Info InitInfo{};
			InitInfo.Width = g_nWndCX / 2;
			InitInfo.Height = g_nWndCY / 2;
			InitInfo.Levels = 1;
			InitInfo.Usages = D3DUSAGE_RENDERTARGET;
			InitInfo.Format = D3DFMT_A16B16G16R16F;
			InitInfo._D3DPool = D3DPOOL_DEFAULT;

			
			afterimagetargets[i].Initialize(InitInfo);
		}
	}


	effect = insulatoreffect;

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
	ImGui::Begin("sRGBToLinear");

	for (uint32 i = 0; i < 256; ++i)
	{
		Vector4 insulator = FMath::Color::sRGBToLinear(i, i, i);
		ImGui::Text("sRGB : %d -> (Linear Space) %f ", i, insulator.x);
		ImGui::Separator();
	}
	

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
