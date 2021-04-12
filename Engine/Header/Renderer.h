#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "Object.h"
#include "GameObject.h"
#include <map>
#include "FLight.h"
#include <memory>
#include <vector>
#include "RenderProperty.h"
#include "Frustum.h"
#include "RenderInformation.h"
#include "RenderInterface.h"
#include "RenderTarget.h"
#include "Shader.h"
#include "Quad.h"
#include "basiccamera.h"

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
private:
	void    ReadyRenderTargets();
	void	ReadyShader(const std::filesystem::path & TargetPath);
	void    ReadyLights();
	void    ReadyRenderInfo();
	void    ReadyFrustum();
	void    ReadyQuad();	
public :
	// 오브젝트의 렌더 세팅이 켜져있다면 RenderInterface 인터페이스를 검사하고 엔티티에 추가 .
	void Push(const std::weak_ptr<GameObject>& _RenderEntity)&;
public : 
	HRESULT Render()&;
	void    Editor()&;
private:
	void RenderReady()&;
	void RenderReadyEntitys()&;
	void Culling()&;
	void FrustumCulling()&;
	void RenderEnd()&;
	void RenderEntityClear()&;
private:
	HRESULT RenderImplementation()&;
	// HRESULT RenderGBuffer()&;
	HRESULT RenderDeferredShading()&;
	HRESULT RenderForwardAlphaBlend()&;
	// HRESULT RenderShadows();
	HRESULT RenderAlphaBlendEffect()&;
	HRESULT RenderDebug()&;
	HRESULT RenderDebugBone()&;
	HRESULT RenderUI()&;
	HRESULT ImguiRender()&;
	HRESULT RenderShadowScene(FLight*const  Light);
	HRESULT RenderTargetDebugRender()&;
public:
	bool    bEdit = false;
	RenderInformation _RenderInfo{};
	RenderInformation _PrevRenderInfo{};
private:
	D3DVIEWPORT9       BackBufViewport{};
	IDirect3DSurface9* BackBuffer{ nullptr };
	std::shared_ptr<Frustum> CameraFrustum{};
	LPDIRECT3DDEVICE9	m_pDevice{ nullptr };
	std::map<RenderProperty::Order, std::vector<RenderInterface*>> RenderEntitys{};
	std::shared_ptr<Quad> _Quad;
	std::map<std::string, std::shared_ptr<ENGINE::Shader>> Shaders{};
	std::map<std::string, std::shared_ptr<RenderTarget>>   RenderTargets{};

	std::shared_ptr<FLight> Moonlight{};
	std::vector<std::shared_ptr<FLight>> PointLights{};

	// 쉐이더 테스트 시작 ....
	bool TestShaderInit();
	void TestShaderRelease();
	
	bool bCurstomEye = false;
	void RenderShadowMaps();
	void RenderGBuffer(
		const Matrix & ViewProjection);
	void DeferredShading(
		const Matrix & View, 
		const Matrix & Projection, 
		const Matrix & ViewProjectionInverse, 
		const Vector4 & Eye);
	void RenderScene(LPD3DXEFFECT effect, const D3DXMATRIX & viewproj);

	
	LPDIRECT3DTEXTURE9	marble = nullptr;
	LPDIRECT3DTEXTURE9	wood = nullptr;
	LPDIRECT3DTEXTURE9	wood_normal = nullptr;
	LPDIRECT3DTEXTURE9	sky = nullptr;
	LPD3DXMESH			skull = nullptr;
	LPD3DXMESH			box = nullptr;

	Vector4 MoonLightTarget{ 0,0,0 , 1 };
	Vector4 CurstomEye= { 0,0,0,1 };
	float DXScreenQuadVerticesFFP[24] = {
		// NOTE: viewport must be added
		-0.5f, -0.5f, 0, 1,		0, 1,
		-0.5f, -0.5f, 0, 1,		0, 0,
		-0.5f, -0.5f, 0, 1,		1, 1
		- 0.5f, -0.5f, 0, 1,		1, 0,
	};

};
END

#endif // _RENDERER_H_