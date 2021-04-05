#include "stdafx.h"
//#include "ShaderTester.h"
//#include <d3d9.h>
//#include <d3dx9.h>
//#include <d3dx9mesh.h>
//#include <fstream>
//#include <ostream>
//#include "RenderTarget.h"
//#include "Shader.h"
//#include <array>
//#include "FMath.hpp"
//
//
//
//class Float16
//{
//private:
//	uint16_t bits;
//
//public:
//	Float16();
//	explicit Float16(float f);
//	explicit Float16(uint16_t s);
//
//	Float16& operator =(const Float16& other);
//	Float16& operator =(float f);
//	Float16& operator =(uint16_t s);
//	operator float() const;
//
//	inline operator uint16_t() const { return bits; }
//};
//float DXScreenQuadVertices[24] = {
//	-1, -1, 0, 1,	0, 1,
//	-1, 1, 0, 1,	0, 0,
//	1, -1, 0, 1,	1, 1,
//	1, 1, 0, 1,		1, 0
//};
//
//// --- Float16 impl -----------------------------------------------------------
//
//Float16::Float16()
//{
//	bits = 0;
//}
//
//Float16::Float16(float f)
//{
//	operator =(f);
//}
//
//Float16::Float16(uint16_t s)
//{
//	bits = s;
//}
//
//Float16& Float16::operator =(float f)
//{
//	uint32_t fp32 = *((uint32_t*)&f);
//	uint32_t signbit = (fp32 & 0x80000000) >> 16;
//	uint32_t mant = (fp32 & 0x007fffff);
//	int exp = ((fp32 & 0x7f800000) >> 23) - 112;
//
//	if (exp <= 0)
//		bits = 0;
//	else if (exp > 30)
//		bits = (uint16_t)(signbit | 0x7bff);
//	else
//		bits = (uint16_t)(signbit | (exp << 10) | (mant >> 13));
//
//	return *this;
//}
//
//Float16& Float16::operator =(uint16_t s)
//{
//	bits = s;
//	return *this;
//}
//
//Float16& Float16::operator =(const Float16& other)
//{
//	bits = other.bits;
//	return *this;
//}
//
//Float16::operator float() const
//{
//	uint32_t magic = 126 << 23;
//	uint32_t fp32 = (bits & 0x8000) << 16;
//	uint32_t mant = (bits & 0x000003ff);
//	int exp = (bits >> 10) & 0x0000001f;
//
//	if (exp == 0) {
//		fp32 = magic + mant;
//		(*(float*)&fp32) -= (*(float*)&magic);
//	}
//	else {
//		mant <<= 13;
//
//		if (exp == 31)
//			exp = 255;
//		else
//			exp += 127 - 15;
//
//		fp32 |= (exp << 23);
//		fp32 |= mant;
//	}
//
//	return *((float*)&fp32);
//}
//
//float DXScreenQuadVerticesFFP[24] = {
//	// NOTE: viewport must be added
//	-0.5f, -0.5f, 0, 1,		0, 1,
//	-0.5f, -0.5f, 0, 1,		0, 0,
//	-0.5f, -0.5f, 0, 1,		1, 1
//	- 0.5f, -0.5f, 0, 1,		1, 0,
//};
//
//D3DXVECTOR3 DXCubeForward[6] = {
//	D3DXVECTOR3(1, 0, 0),	// +X
//	D3DXVECTOR3(-1, 0, 0),	// -X
//	D3DXVECTOR3(0, 1, 0),	// +Y
//	D3DXVECTOR3(0, -1, 0),	// -Y
//	D3DXVECTOR3(0, 0, 1),	// +Z
//	D3DXVECTOR3(0, 0, -1),	// -Z
//};
//
//D3DXVECTOR3 DXCubeUp[6] = {
//	D3DXVECTOR3(0, 1, 0),
//	D3DXVECTOR3(0, 1, 0),
//	D3DXVECTOR3(0, 0, -1),
//	D3DXVECTOR3(0, 0, 1),
//	D3DXVECTOR3(0, 1, 0),
//	D3DXVECTOR3(0, 1, 0),
//};
//
//D3DVIEWPORT9			oldviewport;
//D3DVIEWPORT9			viewport;
//D3DXVECTOR4				pixelsize(0, 0, 0, 1);
//D3DXVECTOR4				texelsize(0, 0, 0, 1);
//float					averageluminance = 0.1f;
//// don't set this to zero!!!
//float					adaptedluminance = 0.1f;
//// don't set this to zero!!!
//float					exposure = 0;
//uint8_t					mousebuttons = 0;
//int						currentafterimage = 0;
//bool					drawafterimage = false;
//
//
//LPD3DXMESH teapot = nullptr;
//LPD3DXMESH skull = nullptr;
//LPD3DXMESH knot = nullptr;
//LPD3DXMESH skymesh = nullptr;
//LPD3DXMESH mesh = nullptr;
//
//
//LPDIRECT3DCUBETEXTURE9	environment = nullptr;
//// HDR environment
//LPDIRECT3DCUBETEXTURE9	irradiance1 = nullptr;
//// preintegrated diffuse irradiance
//LPDIRECT3DCUBETEXTURE9	irradiance2 = nullptr;		// preintegrated specular irradiance
//IDirect3DTexture9* brdfLUT = nullptr;
//// preintegrated BRDF lookup texture
//
//std::shared_ptr<ENGINE::Shader> skyeffect{};
//std::shared_ptr<ENGINE::Shader> metaleffect{};
//std::shared_ptr<ENGINE::Shader> insulatoreffect{};
//std::shared_ptr<ENGINE::Shader>  effect{};
//std::shared_ptr<ENGINE::Shader> measureeffect{};
//std::shared_ptr<ENGINE::Shader> hdreffects{};
//std::shared_ptr<ENGINE::Shader> screenquad{};
//
//RenderTarget scenetarget{  };
//RenderTarget avgluminance{  };
//RenderTarget avglumsystemmem{  };
//RenderTarget bloomresult{};
//RenderTarget starresult{};
//
//std::array<RenderTarget, 5u> dsampletargets;
//std::array<RenderTarget, 5u>blurtargets;
//std::array<std::array<RenderTarget, 2u>, 4u> startargets;
//std::array<RenderTarget, 2u>		lensflaretargets;
//std::array<RenderTarget, 2u>		afterimagetargets;
//
//
//
//static void RenderScene()
//{
//	Matrix Identity{};
//	Matrix world, worldinv;
//	Matrix rotation;
//	Vector4 silver = { 0.972f,0.96f,0.915f, 1.0f };
//
//	Vector4 insulator = FMath::Color::sRGBToLinear(255, 255, 255);
//
//	Matrix	view, proj;
//	Vector4	eye = { -3,0.5,-4,1 };
//	Vector3 eyeVec3 = Vector3{ eye.x , eye.y ,eye.z };
//	Vector3 At{ 0,0,-10.f };
//	Vector3 Up{ 0,1,0 };
//
//	D3DXMatrixLookAtRH(&view, &eyeVec3, &At, &Up);
//	D3DXMatrixPerspectiveFovRH(&proj,
//		FMath::ToRadian(45.f),
//		(float)g_nWndCX / (float)g_nWndCY, 0.1f, 50.f);
//	Matrix	skyview, viewproj;
//
//	Vector3	orient;
//
//	D3DXMatrixIdentity(&Identity);
//
//	IDirect3DDevice9* device = g_pDevice;
//	device->SetRenderTarget(0, scenetarget.GetSurface(0u));
//	// 감마보정을 비활성화 한다 . (기본값도 false 이긴 함)
//	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
//	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);
//	{
//		// render sky
//		skyview = view;
//		skyview._41 = skyview._42 = skyview._43 = 0;
//
//		viewproj = proj * view;
//
//		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
//		device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
//		device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
//
//		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
//		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//
//		skyeffect->GetEffect()->
//			SetMatrix("matViewProj", (D3DXMATRIX*)&viewproj);
//		skyeffect->GetEffect()->SetMatrix("matSkyRotation", &Identity);
//
//		skyeffect->GetEffect()->Begin(NULL, 0);
//		skyeffect->GetEffect()->BeginPass(0);
//		{
//			device->SetTexture(0, environment);
//			skymesh->DrawSubset(0);
//		}
//		skyeffect->GetEffect()->EndPass();
//		skyeffect->GetEffect()->End();
//
//		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
//
//		// render object
//		if (mesh == teapot) {
//			D3DXMatrixScaling(&world, 1.5f, 1.5f, 1.5f);
//		}
//		else if (mesh == knot) {
//			D3DXMatrixScaling(&world, 0.8f, 0.8f, 0.8f);
//		}
//		else if (mesh == skull) {
//			D3DXMatrixScaling(&world, 0.4f, 0.4f, 0.4f);
//			world._42 = -1.5f;
//		}
//
//
//		// render object
//		if (mesh == teapot) {
//			D3DXMatrixScaling(&world, 1.5f, 1.5f, 1.5f);
//		}
//		else if (mesh == knot) {
//			D3DXMatrixScaling(&world, 0.8f, 0.8f, 0.8f);
//		}
//		else if (mesh == skull) {
//			D3DXMatrixScaling(&world, 0.4f, 0.4f, 0.4f);
//			world._42 = -1.5f;
//		}
//
//		D3DXMatrixRotationYawPitchRoll(&rotation, orient.x, orient.y, 0);
//		D3DXMatrixMultiply(&world, &world, &rotation);
//		D3DXMatrixInverse(&worldinv, NULL, &world);
//		viewproj = proj * view;
//
//		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
//
//		device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//		device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//		device->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
//		device->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
//		device->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
//
//		device->SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//		device->SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//		device->SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
//		device->SetSamplerState(2, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
//		device->SetSamplerState(2, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
//
//		effect->GetEffect()->SetMatrix("matWorld", &world);
//		effect->GetEffect()->SetMatrix("matWorldInv", &worldinv);
//		effect->GetEffect()->SetMatrix("matViewProj", (D3DXMATRIX*)&viewproj);
//		effect->GetEffect()->SetVector("eyePos", (D3DXVECTOR4*)&eye);
//
//		if (effect == metaleffect) {
//			effect->GetEffect()->SetVector("baseColor", &silver);
//			effect->GetEffect()->SetFloat("roughness", 0.0f);
//
//			device->SetTexture(0, irradiance2);
//			device->SetTexture(1, brdfLUT);
//		}
//		else {
//			effect->GetEffect()->SetVector("baseColor", (D3DXVECTOR4*)&insulator);
//			effect->GetEffect()->SetFloat("roughness", 0.2f);
//
//			device->SetTexture(0, irradiance1);
//			device->SetTexture(1, irradiance2);
//			device->SetTexture(2, brdfLUT);
//		}
//
//		effect->GetEffect()->Begin(NULL, 0);
//		effect->GetEffect()->BeginPass(0);
//		{
//			mesh->DrawSubset(0);
//		}
//		effect->GetEffect()->EndPass();
//		effect->GetEffect()->End();
//
//		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//	}
//}
//static void MeasureLuminance()
//{
//	auto* device = g_pDevice;
//
//	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
//	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
//	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
//	device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
//	device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
//
//	for (int i = 0; i < 4; ++i) {
//		viewport.Width = 64 >> (i * 2);
//		viewport.Height = 64 >> (i * 2);
//
//		pixelsize.x = 1.0f / (float)viewport.Width;
//		pixelsize.y = -1.0f / (float)viewport.Height;
//
//		if (i == 0) {
//			texelsize.x = 1.0f / (float)oldviewport.Width;
//			texelsize.y = 1.0f / (float)oldviewport.Height;
//
//			measureeffect->GetEffect()->SetTechnique("avgluminital");
//			device->SetTexture(0, scenetarget.GetTexture());
//		}
//		else if (i == 3) {
//			texelsize.x = 1.0f / (float)(64 >> (2 * (i - 1)));
//			texelsize.y = 1.0f / (float)(64 >> (2 * (i - 1)));
//
//			measureeffect->GetEffect()->SetTechnique("avglumfinal");
//			device->SetTexture(0, avgluminance.GetTexture());
//		}
//		else {
//			texelsize.x = 1.0f / (float)(64 >> (2 * (i - 1)));
//			texelsize.y = 1.0f / (float)(64 >> (2 * (i - 1)));
//
//			measureeffect->GetEffect()->SetTechnique("avglumiterative");
//			device->SetTexture(0, avgluminance.GetTexture());
//		}
//
//		measureeffect->GetEffect()->SetInt("prevLevel", (i - 1) * 2);
//		measureeffect->GetEffect()->SetVector("pixelSize", &pixelsize);
//		measureeffect->GetEffect()->SetVector("texelSize", &texelsize);
//		device->SetRenderTarget(0, avgluminance.GetSurface(i));
//		device->SetViewport(&viewport);
//
//		measureeffect->GetEffect()->Begin(NULL, 0);
//		measureeffect->GetEffect()->BeginPass(0);
//		{
//			device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, DXScreenQuadVertices, 6 * sizeof(float));
//		}
//		measureeffect->GetEffect()->EndPass();
//		measureeffect->GetEffect()->End();
//	}
//
//	D3DLOCKED_RECT rect;
//
//	device->GetRenderTargetData(avgluminance.GetSurface(3), avglumsystemmem.GetSurface(0));
//	avglumsystemmem.GetTexture()->LockRect(0, &rect, NULL, D3DLOCK_READONLY);
//	{
//		averageluminance = ((Float16*)rect.pBits)->operator float();
//	}
//	avglumsystemmem.GetTexture()->UnlockRect(0);
//}
//
//void AdaptLuminance(float dt)
//{
//	adaptedluminance = adaptedluminance + (averageluminance - adaptedluminance) * (1.0f - powf(0.98f, 50.0f * dt));
//
//	// DICE's suggestion
//	float two_ad_EV = adaptedluminance * (100.0f / 12.5f);
//	exposure = 1.0f / (1.2f * two_ad_EV);
//}
//
//
//
//void ShaderTester::Free()
//{
//	scenetarget.Release();
//	avgluminance.Release();
//	avglumsystemmem.Release();
//	bloomresult.Release();
//	starresult.Release();
//
//	for (auto& DeleteTarget : dsampletargets)
//	{
//		DeleteTarget.Release();
//	}
//	for (auto& DeleteTarget : blurtargets)
//	{
//		DeleteTarget.Release();
//	}
//	for (auto& DeleteTargets : startargets)
//	{
//		for (auto& DeleteTarget : DeleteTargets)
//		{
//			DeleteTarget.Release();
//		}
//	}
//	for (auto& DeleteTarget : lensflaretargets)
//	{
//		DeleteTarget.Release();
//	}
//	for (auto& DeleteTarget : afterimagetargets)
//	{
//		DeleteTarget.Release();
//	}
//
//
//	if (teapot)
//		teapot->Release();
//	if (skull)
//		skull->Release();
//	if (knot)
//		knot->Release();
//	if (skymesh)
//		skymesh->Release();
//	if (environment)
//		environment->Release();
//	if (irradiance1)
//		irradiance1->Release();
//	if (irradiance2)
//		irradiance2->Release();
//	if (brdfLUT)
//		brdfLUT->Release();
//
//
//}
//
//void ShaderTester::RenderForwardAlphaBlend()
//{
//	// 메쉬 갖고와서 똑같은 함수 호출 !! 
//}
//
//void ShaderTester::RenderReady()
//{
//	// 메쉬 갖고 와서 똑같은 함수 호출 !! 
//}
//
//ShaderTester* ShaderTester::Create()
//{
//	return new ShaderTester{};
//}
//
//HRESULT ShaderTester::Ready()
//{
//	
//	auto _StaticMesh = Resources::Load<StaticMesh>(TEXT("../../Resource/TestMesh/Move.fbx"));
//
//	using namespace rapidjson;
//	std::filesystem::path TargetPath = "..\\..\\Resource\\Dummy\\TestJson.json";
//	StringBuffer StrBuf;
//	PrettyWriter<StringBuffer> Writer(StrBuf);
//	//  Cell Information Write...
//	Writer.StartObject();
//	Writer.Key("Test Json");
//	Writer.StartArray();
//	{
//		{
//			Writer.StartObject();
//			{
//				Writer.Key("Index");
//				Writer.Uint(777u);
//
//				Writer.Key("Name");
//				Writer.String("Nero");
//
//				Writer.Key("Value_1");
//				Writer.Double(123.456f);
//
//				Writer.Key("Value 2");
//				Writer.Double(999.999f);
//			}
//			Writer.EndObject();
//		}
//		Writer.EndArray();
//
//		Writer.EndObject();
//		TargetPath.replace_extension("json");
//		std::ofstream Of{ TargetPath };
//		Of << StrBuf.GetString();
//	};
//
//
//	auto* device = g_pDevice;
//
//
//
//	// load resources
//	if (FAILED(D3DXCreateCubeTextureFromFile(device, L"../../Media/Textures/grace.dds", &environment)))
//	{
//		PRINT_LOG(L"Load Fail!", L"Fail!");
//	}
//
//	if (FAILED(D3DXCreateCubeTextureFromFile(device, L"../../Media/Textures/grace_diff_irrad.dds", &irradiance1)))
//	{
//		PRINT_LOG(L"Load Fail!", L"Fail!");
//	};
//
//	if (FAILED(D3DXCreateCubeTextureFromFile(device, L"../../Media/Textures/grace_spec_irrad.dds", &irradiance2)))
//	{
//		PRINT_LOG(L"Load Fail!", L"Fail!");
//	}
//
//	if (FAILED(D3DXCreateTextureFromFile(device, L"../../Media/Textures/brdf.dds", &brdfLUT)))
//	{
//		PRINT_LOG(L"Load Fail!", L"Fail!");
//	};
//
//	if (FAILED(D3DXLoadMeshFromX(L"../../Media/MeshesDX/knot.x", D3DXMESH_MANAGED, device, NULL, NULL, NULL, NULL, &knot)))
//	{
//		PRINT_LOG(L"Load Fail!", L"Fail!");
//	}
//
//	if (FAILED(D3DXLoadMeshFromX(L"../../Media/MeshesDX/skullocc3.x", D3DXMESH_MANAGED, device, NULL, NULL, NULL, NULL, &skull)))
//	{
//		PRINT_LOG(L"Load Fail!", L"Fail!");
//	}
//
//	if (FAILED(D3DXLoadMeshFromX(L"../../Media/MeshesDX/teapot.x", D3DXMESH_MANAGED, device, NULL, NULL, NULL, NULL, &teapot)))
//	{
//		PRINT_LOG(L"Load Fail!", L"Fail!");
//	}
//
//	if (FAILED(D3DXLoadMeshFromX(L"../../Media/MeshesDX/sky.x", D3DXMESH_MANAGED, device, NULL, NULL, NULL, NULL, &skymesh)))
//	{
//		PRINT_LOG(L"Load Fail!", L"Fail!");
//	}
//
//
//
//
//	;
//	skyeffect = Resources::Load<Shader>(TEXT("../../Resource/TestDummy/ShadersDX/sky.fx"));
//	metaleffect = Resources::Load<Shader>(TEXT("../../Resource/TestDummy/ShadersDX/metal.fx"));
//	// insulator
//	effect = insulatoreffect = Resources::Load<Shader>(TEXT("../../Resource/TestDummy/ShadersDX/insulator.fx"));
//	// 측정 휘도 
//	measureeffect = Resources::Load<Shader>(TEXT("../../Resource/TestDummy/ShadersDX/measureluminance.fx"));
//	hdreffects = Resources::Load<Shader>(TEXT("../../Resource/TestDummy/ShadersDX/hdreffects.fx"));
//	screenquad = Resources::Load<Shader>(TEXT("../../Resource/TestDummy/ShadersDX/screenquad.fx"));
//
//
//	{
//		ENGINE::RenderTarget::Info InitInfo{};
//		InitInfo.Width = g_nWndCX;
//		InitInfo.Height = g_nWndCY;
//		InitInfo.Levels = 1;
//		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
//		InitInfo.Format = D3DFMT_A16B16G16R16F;
//		InitInfo._D3DPool = D3DPOOL_DEFAULT;
//
//		scenetarget.Initialize(InitInfo);
//	}
//
//	{
//		ENGINE::RenderTarget::Info InitInfo{};
//		InitInfo.Width = 64;
//		InitInfo.Height = 64;
//		InitInfo.Levels = 0;
//		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
//		InitInfo.Format = D3DFMT_R16F;
//		InitInfo._D3DPool = D3DPOOL_DEFAULT;
//
//		avgluminance.Initialize(InitInfo);
//	}
//
//	{
//		ENGINE::RenderTarget::Info InitInfo{};
//		InitInfo.Width = 1;
//		InitInfo.Height = 1;
//		InitInfo.Levels = 0;
//		InitInfo.Usages = 0;
//		InitInfo.Format = D3DFMT_R16F;
//		InitInfo._D3DPool = D3DPOOL_SYSTEMMEM;
//
//		avglumsystemmem.Initialize(InitInfo);
//	}
//
//	{
//		// 평균 휘도 
//		// 0 (초기화할때 획득), 2 , 4 , 6 밉맵 획득 .
//		const uint32 SurfaceCount = 4;
//		for (int i = 1; i < SurfaceCount; ++i)
//		{
//			avgluminance.InsertSurface(i * 2);
//		}
//	}
//
//	{
//		// 화면크기 절반으로 뭉개기 블룸 
//		ENGINE::RenderTarget::Info InitInfo{};
//		InitInfo.Width = g_nWndCX / 2;
//		InitInfo.Height = g_nWndCY / 2;
//		InitInfo.Levels = 1;
//		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
//		InitInfo.Format = D3DFMT_A16B16G16R16F;
//		InitInfo._D3DPool = D3DPOOL_DEFAULT;
//
//		bloomresult.Initialize(InitInfo);
//	}
//
//	{
//		// 스타 효과 화면크기 1/4 로 뭉개기
//		ENGINE::RenderTarget::Info InitInfo{};
//		InitInfo.Width = g_nWndCX / 4;
//		InitInfo.Height = g_nWndCY / 4;
//		InitInfo.Levels = 1;
//		InitInfo.Usages = D3DUSAGE_RENDERTARGET;
//		InitInfo.Format = D3DFMT_A16B16G16R16F;
//		InitInfo._D3DPool = D3DPOOL_DEFAULT;
//
//		starresult.Initialize(InitInfo);
//	}
//
//	for (int i = 0; i < 5; ++i)
//	{
//		{
//			ENGINE::RenderTarget::Info DS_InitInfo;
//			DS_InitInfo.Width = g_nWndCX / (2 << i);
//			DS_InitInfo.Height = g_nWndCY / (2 << i);
//			DS_InitInfo.Levels = 1;
//			DS_InitInfo.Usages = D3DUSAGE_RENDERTARGET;
//			DS_InitInfo.Format = D3DFMT_A16B16G16R16F;
//			DS_InitInfo._D3DPool = D3DPOOL_DEFAULT;
//
//			dsampletargets[i].Initialize(DS_InitInfo);
//
//		}
//
//		{
//			ENGINE::RenderTarget::Info BT_InitInfo;
//			BT_InitInfo.Width = g_nWndCX / (2 << i);
//			BT_InitInfo.Height = g_nWndCY / (2 << i);
//			BT_InitInfo.Levels = 1;
//			BT_InitInfo.Usages = D3DUSAGE_RENDERTARGET;
//			BT_InitInfo.Format = D3DFMT_A16B16G16R16F;
//			BT_InitInfo._D3DPool = D3DPOOL_DEFAULT;
//
//			blurtargets[i].Initialize(BT_InitInfo);
//
//		}
//	}
//
//	// 스타 블러
//	for (int i = 0; i < 4; ++i)
//	{
//		for (int j = 0; j < 2; ++j)
//		{
//			{
//				ENGINE::RenderTarget::Info InitInfo{};
//				InitInfo.Width = g_nWndCX / 4;
//				InitInfo.Height = g_nWndCY / 4;
//				InitInfo.Levels = 1;
//				InitInfo.Usages = D3DUSAGE_RENDERTARGET;
//				InitInfo.Format = D3DFMT_A16B16G16R16F;
//				InitInfo._D3DPool = D3DPOOL_DEFAULT;
//				startargets[i][j].Initialize(InitInfo);
//
//			}
//
//		}
//	}
//
//	for (int i = 0; i < 2; ++i)
//	{
//		{
//			ENGINE::RenderTarget::Info InitInfo{};
//			InitInfo.Width = g_nWndCX / 2;
//			InitInfo.Height = g_nWndCY / 2;
//			InitInfo.Levels = 1;
//			InitInfo.Format = D3DFMT_A16B16G16R16F;
//			InitInfo.Usages = D3DUSAGE_RENDERTARGET;
//			InitInfo._D3DPool = D3DPOOL_DEFAULT;
//
//
//			lensflaretargets[i].Initialize(InitInfo);
//
//		}
//
//		{
//			ENGINE::RenderTarget::Info InitInfo{};
//			InitInfo.Width = g_nWndCX / 2;
//			InitInfo.Height = g_nWndCY / 2;
//			InitInfo.Levels = 1;
//			InitInfo.Usages = D3DUSAGE_RENDERTARGET;
//			InitInfo.Format = D3DFMT_A16B16G16R16F;
//			InitInfo._D3DPool = D3DPOOL_DEFAULT;
//
//
//			afterimagetargets[i].Initialize(InitInfo);
//		}
//	}
//
//
//	effect = insulatoreffect;
//	mesh = skull;
//
//    return S_OK;
//}
//
//HRESULT ShaderTester::Awake()
//{
//    return E_NOTIMPL;
//}
//
//HRESULT ShaderTester::Start()
//{
//    return E_NOTIMPL;
//}
//
//Render()
//{
//	static float time = 0;
//	auto* device = g_pDevice;
//
//	LPDIRECT3DSURFACE9 backbuffer = nullptr;
//
//	device->GetRenderTarget(0, &backbuffer);
//	device->GetViewport(&oldviewport);
//
//	viewport = oldviewport;
//
//	// STEP 1: render scene
//	RenderScene();
//
//	device->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1);
//	device->SetRenderState(D3DRS_ZENABLE, FALSE);
//
//	// STEP 2:  평균 휘도 구하기
//	MeasureLuminance();
//	// 델타타임 구하기 HERE !! 
////	AdaptLuminance(1.f/300.f);
////	// STEP 3: extract high-luminance regions
////	BrightPass();
//
////	// STEP 4: downsample before blur
////	DownSample();
//
////	// STEP 5: render stars
////	Stars();
//
////	// STEP 6: render bloom
////	Bloom();
//
////	// STEP 7: lens flare
////	LensFlare();
//
////	// STEP 9: tonemap & final combine
////	device->SetRenderTarget(0, backbuffer);
////	backbuffer->Release();
//
////	ToneMap();
//
////	// render text
////	viewport.Width = 512;
////	viewport.Height = 512;
////	viewport.X = viewport.Y = 10;
//
////	if (drawtext) {
////		device->SetViewport(&viewport);
////		device->SetTexture(0, helptext);
//
////		device->SetFVF(D3DFVF_XYZW | D3DFVF_TEX1);
////		device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
////		device->SetRenderState(D3DRS_ZENABLE, FALSE);
////		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
////		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
////		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
////		screenquad->Begin(NULL, 0);
////		screenquad->BeginPass(0);
////		{
////			device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, DXScreenQuadVertices, 6 * sizeof(float));
////		}
////		screenquad->EndPass();
////		screenquad->End();
////	}
//
////	// reset states
////	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
////	device->SetRenderState(D3DRS_ZENABLE, TRUE);
////	device->SetViewport(&oldviewport);
//
////time += elapsedtime;
//}
//
//UINT ShaderTester::Update(const float _fDeltaTime)
//{
//	ImGui::Begin("sRGBToLinear");
//
//	for (uint32 i = 0; i < 256; ++i)
//	{
//		Vector4 insulator = FMath::Color::sRGBToLinear(i, i, i);
//		ImGui::Text("sRGB : %d -> (Linear Space) %f ", i, insulator.x);
//		ImGui::Separator();
//	}
//
//
//	ImGui::End();
//
//    return 0;
//}
//
//UINT ShaderTester::LateUpdate(const float _fDeltaTime)
//{
//    return 0;
//}
//
//void ShaderTester::OnEnable()
//{
//}
//
//void ShaderTester::OnDisable()
//{
//}
