#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#include "Resource.h"
BEGIN(ENGINE)
class ENGINE_DLL Texture final : public Resource
{
private:
	LPDIRECT3DTEXTURE9	m_pTexture;
	D3DXIMAGE_INFO		m_tInfo;
	TEXTUREDESC			m_tDesc;
private:
	explicit Texture(LPDIRECT3DDEVICE9 const _pDevice);
	explicit Texture(const Texture& _rOther);
	
	// Resource을(를) 통해 상속됨
	virtual void Free() override;
public:
	virtual	~Texture() = default;
public:
	static Texture*   Create(LPDIRECT3DDEVICE9 const _pDevice, const std::filesystem::path _Path);
	// Resource을(를) 통해 상속됨
	virtual Resource* Clone() override;
private:
	HRESULT LoadTextureFromFile(const std::filesystem::path _Path);
public:
	LPDIRECT3DTEXTURE9	GetTexture();
	D3DXIMAGE_INFO		GetInfo();

	TEXTUREDESC			GetDesc();
	void				SetDesc(const TEXTUREDESC & _tDesc);
};
END
#endif // !__TEXTURE_H__