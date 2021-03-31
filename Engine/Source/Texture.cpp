#include "Texture.h"

USING(ENGINE)

Texture::Texture(LPDIRECT3DDEVICE9 const _pDevice)
	: Resource(_pDevice)
	, m_pTexture(nullptr)
{
	ZeroMemory(&m_tDesc, sizeof(D3DXIMAGE_INFO));
}

Texture::Texture(const Texture& _rOther)
	: Resource(_rOther.m_pDevice)
	, m_pTexture(_rOther.m_pTexture)
{
	SafeAddRef(m_pTexture);
	memcpy_s(&m_tDesc, sizeof(D3DXIMAGE_INFO), &_rOther.m_tDesc, sizeof(D3DXIMAGE_INFO));
}

void Texture::Free()
{
	SafeRelease(m_pTexture);
	Resource::Free();
}

Texture* Texture::Create(LPDIRECT3DDEVICE9 const _pDevice, const TSTRING& _sFilePath ,
						const TextureInformation& InitTextureInfo)
{
	Texture* pInstance = new Texture(_pDevice);
	pInstance->_TextureInfo = InitTextureInfo;
	if (FAILED(pInstance->LoadTextureFromFile(_sFilePath)))
	{
		pInstance->Free();
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

Resource* Texture::Clone()
{
	Texture* pClone = new Texture(m_pDevice);
	return pClone;
}

HRESULT Texture::LoadTextureFromFile(const TSTRING& _sFilePath)
{
	if (FAILED(D3DXGetImageInfoFromFile(_sFilePath.c_str(), &m_tDesc)))
	{
		PRINT_LOG(TEXT("Warning"), TEXT("Failed to D3DXGetImageInfoFromFile"));
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFileEx(m_pDevice
		, _sFilePath.c_str()
		, m_tDesc.Width
		, m_tDesc.Height
		, m_tDesc.MipLevels
		, 0
		, m_tDesc.Format
		, D3DPOOL_MANAGED
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, 0
		, nullptr
		, nullptr
		, &m_pTexture)))
	{
		PRINT_LOG(TEXT("Warning"), TEXT("Failed to D3DXCreateTextureFromFileEx"));
		return E_FAIL;
	}
	return S_OK;
}

LPDIRECT3DTEXTURE9 Texture::GetTexture()
{
	return m_pTexture;
}

D3DXIMAGE_INFO Texture::GetDesc()
{
	return m_tDesc;
}
