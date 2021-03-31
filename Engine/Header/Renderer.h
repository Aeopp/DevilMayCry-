#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "Object.h"
#include "GameObject.h"
#include <map>
#include <memory>
#include <vector>
#include "RenderProperty.h"

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
	LPDIRECT3DDEVICE9	m_pDevice{ nullptr };
	std::map<RenderProperty::Order, std::vector<std::weak_ptr<GameObject>>> RenderEntitys{};
public :
	void Push(const std::shared_ptr<GameObject>& _RenderEntity)&;
public : 
	HRESULT Render()&;
public:

};
END

#endif // _RENDERER_H_