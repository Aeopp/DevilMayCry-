#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "Object.h"
#include "GameObject.h"
#include <map>
#include <memory>
#include <vector>
#include "RenderProperty.h"

BEGIN(ENGINE)
class Renderer final : public Object
{
	DECLARE_SINGLETON(Renderer) 
private : 
	explicit Renderer();
	virtual ~Renderer() = default;
	virtual void Free()	override ;
public:
private : 
	std::map<RenderProperty::Order, std::vector<std::weak_ptr<GameObject>>> RenderEntitys;
public :
	void Push(const std::shared_ptr<GameObject>& _RenderEntity)&;
public : 
	void Render()&;
public:

};
END

#endif // _RENDERER_H_