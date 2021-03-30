#ifndef __SHADER_H__
#define __SHADER_H__
#include "Resource.h"
BEGIN(ENGINE)
class ENGINE_DLL Shader final: public Resource
{
private:
	LPD3DXEFFECT	m_pEffect;
	LPD3DXBUFFER	m_pErrMsg;
private:
	explicit Shader(LPDIRECT3DDEVICE9 const _pDevice);
	explicit Shader(const Shader& _rOther);
	virtual ~Shader() = default;
	// Resource을(를) 통해 상속됨
	virtual void Free() override;
public:
	static Shader* Create(LPDIRECT3DDEVICE9 const _pDevice, const TSTRING& _sFilePath);
	// Resource을(를) 통해 상속됨
	virtual Resource* Clone() override;
private:
	HRESULT CreateEffectFromFile(const TSTRING _sFilePath);
public:
	LPD3DXEFFECT GetEffect();
};
END
#endif // !__SHADER_H__