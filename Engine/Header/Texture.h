#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#include "Resource.h"
#include "EngineTypedef.h"
#include "TextureInformation.hpp"

BEGIN(ENGINE)
class ENGINE_DLL Texture final : public Resource
{
private:
	LPDIRECT3DTEXTURE9	m_pTexture;
	D3DXIMAGE_INFO		m_tDesc;
private:
	explicit Texture(LPDIRECT3DDEVICE9 const _pDevice);
	explicit Texture(const Texture& _rOther);
	
	// Resource을(를) 통해 상속됨
	virtual void Free() override;
public:
	virtual	~Texture() = default;
public:
	static Texture*   Create(LPDIRECT3DDEVICE9 const _pDevice, const TSTRING& _sFilePath ,
							const TextureInformation & InitTextureInfo = TextureInformation{});
	// Resource을(를) 통해 상속됨
	virtual Resource* Clone() override;
private:
	HRESULT LoadTextureFromFile(const TSTRING& _sFilePath);
public:
	TextureInformation _TextureInfo{};
	LPDIRECT3DTEXTURE9	GetTexture();
	D3DXIMAGE_INFO		GetDesc();
};
END
#endif // !__TEXTURE_H__