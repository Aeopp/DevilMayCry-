#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "Object.h"
#include "GameObject.h"
#include <map>
#include <memory>
#include <vector>
#include "RenderProperty.h"
#include "Frustum.h"
#include "RenderInformation.h"
#include "RenderInterface.h"
#include "ShaderTester.h"
#include "RenderTarget.h"
#include "Shader.h"

BEGIN(ENGINE)
class ENGINE_DLL Renderer final : public Object
{
	DECLARE_SINGLETON(Renderer) 
private : 
	explicit Renderer();
	virtual ~Renderer() = default;
	virtual void Free()	override ;
public :
	HRESULT ReadyRenderSystem(LPDIRECT3DDEVICE9 const _pDevice);
	void    ReadyRenderTargets();
private : 
	Frustum CameraFrustum{};
	LPDIRECT3DDEVICE9	m_pDevice{ nullptr };
	std::map<RenderProperty::Order, std::vector<RenderInterface*>> RenderEntitys{};
public :
	// 오브젝트의 렌더 세팅이 켜져있다면 RenderInterface 인터페이스를 검사하고 엔티티에 추가 .
	void Push(const std::weak_ptr<GameObject>& _RenderEntity)&;
public : 
	Vector3 TestDirectionLight{ 0,-1,0 };
	HRESULT Render()&;
	RenderInformation CurrentRenderInfo{};
	RenderInformation PrevRenderInfo{};
private:
	void RenderReady()&;
	void RenderReadyEntitys()&;
	void Culling()&;
	void FrustumCulling()&;
	void RenderEnd()&;
	void RenderEntityClear()&;
private:
	HRESULT RenderImplementation()&;
	HRESULT RenderGBuffer()&;
	HRESULT RenderForwardAlphaBlend()&;
	HRESULT RenderAlphaBlendEffect()&;
	HRESULT RenderDebug()&;
	HRESULT RenderDebugBone()&;
	HRESULT RenderUI()&;
	HRESULT ImguiRender()&;
private:
	void RenderTargetDebugRender()&;
private:
	std::shared_ptr<ENGINE::Shader> RTDebug{};
	D3DVIEWPORT9 OldViewport{};
	IDirect3DSurface9* BackBuffer{ nullptr };
private:
	RenderTarget SceneTarget{};
	RenderTarget ALBM{};
	RenderTarget NRMR{};
	RenderTarget Depth{};
	// ShaderTester _ShaderTester{};
};
END

#endif // _RENDERER_H_