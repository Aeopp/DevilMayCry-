#include "FLight.h"
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



FLight::FLight(const FLight::Type type, const D3DXVECTOR4& position, const D3DXCOLOR& color)
{
	this->type = type;
	this->position = position;
	this->color = color;

	projparams = D3DXVECTOR4(0, 0, 0, 0);
	spotdirection = D3DXVECTOR3(0, 0, 0);
	spotparams = D3DXVECTOR2(0, 0);

	cubeshadowmap = nullptr;
	blurredcubeshadowmap = nullptr;
	shadowmap = nullptr;
	blurredshadowmap = nullptr;
	currentface = 0;
	shadowmapsize = 0;
	blurred = false;

	switch (type) {
	case Point:
		this->position.w = 1;
		break;

	case Directional:
		this->position.w = 0;
		break;

	case Spot:
		// haha :)
		this->position.w = 0.75f;
		break;

	default:
		break;
	}
}

FLight::~FLight()
{
	if (cubeshadowmap != nullptr)
		cubeshadowmap->Release();

	if (blurredcubeshadowmap != nullptr)
		blurredcubeshadowmap->Release();

	if (shadowmap != nullptr)
		shadowmap->Release();

	if (blurredshadowmap != nullptr)
		blurredshadowmap->Release();
}

void FLight::CalculateViewProjection(D3DXMATRIX& out)
{
	if (type == Directional) {
		D3DXMATRIX proj;
		D3DXVECTOR3 eye = { position.x , position.y , position.z };
		D3DXVECTOR3 look(0, 0, 0);
		D3DXVECTOR3 up(0, 1, 0);

		if (fabs(position.y) > 0.999f)
			up = D3DXVECTOR3(1, 0, 0);

		D3DXMatrixLookAtLH(&out, &eye, &look, &up);
		D3DXMatrixOrthoLH(&proj, projparams.x, projparams.y, projparams.z, projparams.w);
		D3DXMatrixMultiply(&out, &out, &proj);
	}
	else if (type == Point) {
		D3DXMATRIX proj;
		D3DXVECTOR3 eye = { position.x , position.y , position.z };;

		D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 2, 1, projparams.z, projparams.w);
		D3DXMatrixLookAtLH(&out, &eye, &(eye + 
			DXCubeForward[currentface]), &DXCubeUp[currentface]);
		D3DXMatrixMultiply(&out, &out, &proj);
	}
	else if (type == Spot) {
		// TODO:
	}
}

void FLight::CalculateScissorRect(RECT& out, const D3DXMATRIX& view, const D3DXMATRIX& proj, float radius, int32_t width, int32_t height)
{
	if (type != Point)
		return;

	D3DXVECTOR4 Q;
	D3DXVECTOR3 L, P1, P2;
	float u, v;

	out.left = 0;
	out.right = width;
	out.top = 0;
	out.bottom = height;

	D3DXVec3TransformCoord(&L, (D3DXVECTOR3*)&position, &view);

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
	shadowmapsize = size;

	if (type == Directional) {
		device->CreateTexture(size, size, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G32R32F, D3DPOOL_DEFAULT, &shadowmap, NULL);
		device->CreateTexture(size, size, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G32R32F, D3DPOOL_DEFAULT, &blurredshadowmap, NULL);
	}
	else if (type == Point) {
		device->CreateCubeTexture(size, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G32R32F, D3DPOOL_DEFAULT, &cubeshadowmap, NULL);
		device->CreateCubeTexture(size, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G32R32F, D3DPOOL_DEFAULT, &blurredcubeshadowmap, NULL);
	}
}

void FLight::RenderShadowMap(LPDIRECT3DDEVICE9 device, std::function<void(FLight*)> callback)
{
	D3DVIEWPORT9 oldviewport;
	D3DVIEWPORT9 viewport;

	LPDIRECT3DSURFACE9 oldsurface = NULL;
	LPDIRECT3DSURFACE9 surface = NULL;

	device->GetRenderTarget(0, &oldsurface);
	device->GetViewport(&oldviewport);

	if (type == Directional) {
		shadowmap->GetSurfaceLevel(0, &surface);

		viewport.X = viewport.Y = 0;
		viewport.Width = viewport.Height = shadowmapsize;
		viewport.MinZ = 0;
		viewport.MaxZ = 1;

		device->SetRenderTarget(0, surface);
		device->SetViewport(&viewport);
		{
			callback(this);
		}
		surface->Release();
	}
	else if (type == Point) {
		for (currentface = 0; currentface < 6; ++currentface) {
			cubeshadowmap->GetCubeMapSurface((D3DCUBEMAP_FACES)currentface, 0, &surface);

			viewport.X = viewport.Y = 0;
			viewport.Width = viewport.Height = shadowmapsize;
			viewport.MinZ = 0;
			viewport.MaxZ = 1;

			device->SetRenderTarget(0, surface);
			device->SetViewport(&viewport);
			{
				callback(this);
			}
			surface->Release();
		}
	}

	blurred = false;

	device->SetRenderTarget(0, oldsurface);
	device->SetViewport(&oldviewport);

	oldsurface->Release();
}

void FLight::BlurShadowMap(LPDIRECT3DDEVICE9 device, std::function<void(FLight*)> callback)
{
	D3DVIEWPORT9 oldviewport;
	D3DVIEWPORT9 viewport;

	LPDIRECT3DSURFACE9 oldsurface = NULL;
	LPDIRECT3DSURFACE9 surface = NULL;

	device->GetRenderTarget(0, &oldsurface);
	device->GetViewport(&oldviewport);

	if (type == Directional) {
		blurredshadowmap->GetSurfaceLevel(0, &surface);
		// µð·º¼Å³ã ¶óÀÌÆ®.

		viewport.X = viewport.Y = 0;
		viewport.Width = viewport.Height = shadowmapsize;
		viewport.MinZ = 0;
		viewport.MaxZ = 1;

		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		device->SetRenderTarget(0, surface);
		device->SetViewport(&viewport);
		device->SetTexture(0, shadowmap);
		{
			callback(this);
		}
		surface->Release();
	}
	else {
		// TODO:
	}

	blurred = true;

	device->SetRenderTarget(0, oldsurface);
	device->SetViewport(&oldviewport);

	oldsurface->Release();
}

void FLight::SetProjectionParameters(float xsize, float ysize, float znear, float zfar)
{
	projparams.x = xsize;
	projparams.y = ysize;
	projparams.z = znear;
	projparams.w = zfar;
}



void FLight::SetSpotParameters(const D3DXVECTOR3& dir, float inner, float outer)
{
	spotdirection = dir;
	spotparams.x = cosf(inner);
	spotparams.y = cosf(outer);
}

