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
private : 
	Frustum CameraFrustum{};
	LPDIRECT3DDEVICE9	m_pDevice{ nullptr };
	std::map<RenderProperty::Order, std::vector<std::shared_ptr<RenderInterface>>> RenderEntitys{};
public :
	// ������Ʈ�� ���� ������ �����ִٸ� RenderInterface �������̽��� �˻��ϰ� ��ƼƼ�� �߰� .
	void Push(const std::weak_ptr<GameObject>& _RenderEntity)&;
public : 
	HRESULT Render()&;
	RenderInformation CurrentRenderInfo{};
	RenderInformation PrevRenderInfo{};

	//�ӽ�
	HRESULT Set_Info(const Matrix & _CameraView, const Matrix & _CameraProjection);
private:
	void RenderReady()&;
	void RenderReadyEntitys()&;
	void Culling()&;
	void FrustumCulling()&;
	void RenderEnd()&;
	void RenderEntityClear()&;
private:
	HRESULT RenderImplementation()&;
	HRESULT RenderForwardAlphaBlend()&;
	HRESULT ImguiRender()&;
};
END

#endif // _RENDERER_H_