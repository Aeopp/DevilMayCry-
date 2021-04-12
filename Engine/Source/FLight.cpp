#include "FLight.h"
#include "imgui.h"

USING(ENGINE)

D3DXVECTOR3 DXCubeForward[6] = {
	D3DXVECTOR3(1, 0, 0),	// +X
	D3DXVECTOR3(-1, 0, 0),	// -X
	D3DXVECTOR3(0, 1, 0),	// +Y
	D3DXVECTOR3(0, -1, 0),	// -Y
	D3DXVECTOR3(0, 0, 1),	// +Z
	D3DXVECTOR3(0, 0, -1),	// -Z
};


D3DXVECTOR3 DXCubeUp[6] = {
	D3DXVECTOR3(0, 1, 0),
	D3DXVECTOR3(0, 1, 0),
	D3DXVECTOR3(0, 0, -1),
	D3DXVECTOR3(0, 0, 1),
	D3DXVECTOR3(0, 1, 0),
	D3DXVECTOR3(0, 1, 0),
};



FLight::FLight(
	const FLight::Type type, 
	const D3DXVECTOR4& position, 
	const D3DXCOLOR& color,
	const float blurIntencity)
{
	this->_Type = type;
	this->Position = position;
	this->Color = color;

	Projparams = D3DXVECTOR4(0, 0, 0, 0);
	Spotdirection = D3DXVECTOR3(0, 0, 0);
	Spotparams = D3DXVECTOR2(0, 0);

	BlurIntencity = blurIntencity;
	Cubeshadowmap = nullptr;
	Blurredcubeshadowmap = nullptr;
	Shadowmap = nullptr;
	Blurredshadowmap = nullptr;
	Currentface = 0;
	ShadowMapSize = 0;
	Blurred = false;

	switch (type) {
	case Point:
		this->Position.w = 1;
		break;

	case Directional:
		this->Position.w = 0;
		break;

	case Spot:
		// haha :)
		this->Position.w = 0.75f;
		break;

	default:
		break;
	}
}

FLight::~FLight()
{
	if (Cubeshadowmap != nullptr)
		Cubeshadowmap->Release();

	if (Blurredcubeshadowmap != nullptr)
		Blurredcubeshadowmap->Release();

	if (Shadowmap != nullptr)
		Shadowmap->Release();

	if (Blurredshadowmap != nullptr)
		Blurredshadowmap->Release();
};

void FLight::Edit(const uint32 Idx)
{
	const std::string Id = "Light Index : " + std::to_string(Idx);
	if (ImGui::CollapsingHeader(Id.c_str()))
	{
		if (_Type == Directional)
		{
			ImGui::Text("ShadowMap %d", Idx);
			ImGui::Image(reinterpret_cast<void**>(Shadowmap), { 256,256 });
			ImGui::Text("BlurShadowMap %d", Idx);
			ImGui::Image(reinterpret_cast<void**>(Blurredshadowmap), { 256,256 });
		}
		else if (_Type == Point)
		{
			ImGui::Text("CubeShadowMap %d", Idx);
			ImGui::Image(reinterpret_cast<void**>(Cubeshadowmap), { 256,256 });
			ImGui::Text("CubeBlurShadowMap %d", Idx);
			ImGui::Image(reinterpret_cast<void**>(Blurredcubeshadowmap), { 256,256 });
		}

		EditImplementation(Idx);
	};
};

void FLight::EditImplementation(const uint32 Idx)
{
	ImGui::SliderFloat("BlurIntencity", &BlurIntencity,0.f,1000.f);
}

void FLight::CalculateViewProjection(D3DXMATRIX& out)
{
	if (_Type == Directional) {
		D3DXMATRIX proj;
		D3DXVECTOR3 eye = { Position.x , Position.y , Position.z };
		D3DXVECTOR3 look(0, 0, 0);
		D3DXVECTOR3 up(0, 1, 0);

		if (fabs(Position.y) > 0.999f)
			up = D3DXVECTOR3(1, 0, 0);

		D3DXMatrixLookAtLH(&out, &eye, &look, &up);
		D3DXMatrixOrthoLH(&proj, Projparams.x, Projparams.y, Projparams.z, Projparams.w);
		D3DXMatrixMultiply(&out, &out, &proj);
	}
	else if (_Type == Point) {
		D3DXMATRIX proj;
		D3DXVECTOR3 eye = { 
			Position.x , Position.y , Position.z };;

		D3DXMatrixPerspectiveFovLH(&proj, 
			D3DX_PI / 2.f, 1.f, Projparams.z, Projparams.w);
		const Vector3 At = eye + DXCubeForward[Currentface];

		D3DXMatrixLookAtLH(&out, &eye, &At, &DXCubeUp[Currentface]);
		D3DXMatrixMultiply(&out, &out, &proj);
	}
	else if (_Type == Spot) {
		// TODO:
	}
}

void FLight::CalculateScissorRect(RECT& out, const D3DXMATRIX& view, const D3DXMATRIX& proj, float radius, int32_t width, int32_t height)
{
	if (_Type != Point)
		return;

	D3DXVECTOR4 Q;
	D3DXVECTOR3 L, P1, P2;
	float u, v;

	out.left = 0;
	out.right = width;
	out.top = 0;
	out.bottom = height;

	D3DXVec3TransformCoord(&L, (D3DXVECTOR3*)&Position, &view);

	float D = 4 * (radius * radius * L.x * L.x - (L.x * L.x + L.z * L.z) * (radius * radius - L.z * L.z));

	if (D > 0.0f) {
		float Nx1 = (radius * L.x + sqrtf(D * 0.25f)) / (L.x * L.x + L.z * L.z);
		float Nx2 = (radius * L.x - sqrtf(D * 0.25f)) / (L.x * L.x + L.z * L.z);
		float Nz1 = (radius - Nx1 * L.x) / L.z;
		float Nz2 = (radius - Nx2 * L.x) / L.z;

		P1.x = L.x - Nx1 * radius;
		P1.y = 0.0f;
		P1.z = L.z - Nz1 * radius;

		P2.x = L.x - Nx2 * radius;
		P2.y = 0.0f;
		P2.z = L.z - Nz2 * radius;

		// left-handed: >
		if (P1.z < 0.0f && P2.z < 0.0f) {
			D3DXVec3Transform(&Q, &P1, &proj);

			Q /= Q.w;
			u = (Q.x * 0.5f + 0.5f) * width;

			D3DXVec3Transform(&Q, &P2, &proj);

			Q /= Q.w;
			v = (Q.x * 0.5f + 0.5f) * width;

			if (P2.x < L.x) {
				out.left = (LONG)max(v, 0.0f);
				out.right = (LONG)min(u, width);
			}
			else {
				out.left = (LONG)max(u, 0.0f);
				out.right = (LONG)min(v, width);
			}
		}
	}

	D = 4 * (radius * radius * L.y * L.y - (L.y * L.y + L.z * L.z) * (radius * radius - L.z * L.z));

	if (D >= 0.0f) {
		float Ny1 = (radius * L.y + sqrtf(D * 0.25f)) / (L.y * L.y + L.z * L.z);
		float Ny2 = (radius * L.y - sqrtf(D * 0.25f)) / (L.y * L.y + L.z * L.z);
		float Nz1 = (radius - Ny1 * L.y) / L.z;
		float Nz2 = (radius - Ny2 * L.y) / L.z;

		P1.x = 0.0f;
		P1.y = L.y - Ny1 * radius;
		P1.z = L.z - Nz1 * radius;

		P2.x = 0.0f;
		P2.y = L.y - Ny2 * radius;
		P2.z = L.z - Nz2 * radius;

		// left-handed: >
		if (P1.z < 0.0f && P2.z < 0.0f) {
			D3DXVec3Transform(&Q, &P1, &proj);

			Q /= Q.w;
			u = (Q.y * -0.5f + 0.5f) * height;

			D3DXVec3Transform(&Q, &P2, &proj);

			Q /= Q.w;
			v = (Q.y * -0.5f + 0.5f) * height;

			if (P2.y < L.y) {
				out.top = (LONG)max(u, 0.0f);
				out.bottom = (LONG)min(v, height);
			}
			else {
				out.top = (LONG)max(v, 0.0f);
				out.bottom = (LONG)min(u, height);
			}
		}
	}
}

void FLight::CreateShadowMap(LPDIRECT3DDEVICE9 device, uint16_t size)
{
	ShadowMapSize = size;

	if (_Type == Directional) {
		device->CreateTexture(size, size, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G32R32F, D3DPOOL_DEFAULT, &Shadowmap, NULL);
		device->CreateTexture(size, size, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G32R32F, D3DPOOL_DEFAULT, &Blurredshadowmap, NULL);
	}
	else if (_Type == Point) {
		device->CreateCubeTexture(size, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G32R32F, D3DPOOL_DEFAULT, &Cubeshadowmap, NULL);
		device->CreateCubeTexture(size, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G32R32F, D3DPOOL_DEFAULT, &Blurredcubeshadowmap, NULL);
	}
}

void FLight::RenderShadowMap(
	LPDIRECT3DDEVICE9 _Device, 
	std::function<void(FLight*)> CallBack)
{
	D3DVIEWPORT9 OldViewPort;
	D3DVIEWPORT9 Viewport;

	LPDIRECT3DSURFACE9 OldSurface = NULL;
	LPDIRECT3DSURFACE9 Surface = NULL;

	_Device->GetRenderTarget(0, &OldSurface);
	_Device->GetViewport(&OldViewPort);

	if (_Type == Directional) {
		Shadowmap->GetSurfaceLevel(0, &Surface);

		Viewport.X = Viewport.Y = 0;
		Viewport.Width = Viewport.Height = ShadowMapSize;
		Viewport.MinZ = 0;
		Viewport.MaxZ = 1;

		_Device->SetRenderTarget(0, Surface);
		_Device->SetViewport(&Viewport);
		{
			CallBack(this);
		}
		Surface->Release();
	}
	else if (_Type == Point) {
		for (Currentface= 0; Currentface < 6; ++Currentface) {

			Cubeshadowmap->GetCubeMapSurface(
				(D3DCUBEMAP_FACES)Currentface, 0, &Surface);

			Viewport.X = Viewport.Y = 0;
			Viewport.Width = Viewport.Height = ShadowMapSize;
			Viewport.MinZ = 0;
			Viewport.MaxZ = 1;

			_Device->SetRenderTarget(0, Surface);
			_Device->SetViewport(&Viewport);
			{
				CallBack(this);
			}
			Surface->Release();
		}
	}

	Blurred = false;

	_Device->SetRenderTarget(0, OldSurface);
	_Device->SetViewport(&OldViewPort);
	OldSurface->Release();
}

void FLight::BlurShadowMap(LPDIRECT3DDEVICE9 device, std::function<void(FLight*)> callback)
{
	D3DVIEWPORT9 oldviewport;
	D3DVIEWPORT9 viewport;

	LPDIRECT3DSURFACE9 oldsurface = NULL;
	LPDIRECT3DSURFACE9 surface = NULL;

	device->GetRenderTarget(0, &oldsurface);
	device->GetViewport(&oldviewport);

	if (_Type == Directional) {
		Blurredshadowmap->GetSurfaceLevel(0, &surface);
		// µð·º¼Å³ã ¶óÀÌÆ®.

		viewport.X = viewport.Y = 0;
		viewport.Width = viewport.Height = ShadowMapSize;
		viewport.MinZ = 0;
		viewport.MaxZ = 1;

		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		device->SetRenderTarget(0, surface);
		device->SetViewport(&viewport);
		device->SetTexture(0, Shadowmap);
		{
			callback(this);
		}
		surface->Release();
	}
	else if(_Type == Point){
		for (Currentface = 0; Currentface < 6; ++Currentface) {
			/*Blurredcubeshadowmap->GetCubeMapSurface
				((D3DCUBEMAP_FACES)Currentface, 0, &surface);
			
			viewport.X = viewport.Y = 0;
			viewport.Width = viewport.Height = ShadowMapSize;
			viewport.MinZ = 0;
			viewport.MaxZ = 1;

			device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
			device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

			device->SetRenderTarget(0, surface);
			device->SetViewport(&viewport);
			device->SetTexture(0, Cubeshadowmap);
			
			callback(this);

			surface->Release();*/
		}
	}

	Blurred = true;

	device->SetRenderTarget(0, oldsurface);
	device->SetViewport(&oldviewport);

	oldsurface->Release();
}

void FLight::SetProjectionParameters(float xsize, float ysize, float znear, float zfar)
{
	Projparams.x = xsize;
	Projparams.y = ysize;
	Projparams.z = znear;
	Projparams.w = zfar;
}



void FLight::SetSpotParameters(const D3DXVECTOR3& dir, float inner, float outer)
{
	Spotdirection = dir;
	Spotparams.x = cosf(inner);
	Spotparams.y = cosf(outer);
}

