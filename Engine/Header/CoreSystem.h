#ifndef __CORE_SYSTEM_H__
#define __CORE_SYSTEM_H__
#include "Object.h"
BEGIN(ENGINE)
class GraphicSystem;
class InputSystem;
class TimeSystem;
class SceneSystem;
class ResourceSystem;
class CoreSystem final : public Object
{
	DECLARE_SINGLETON(CoreSystem)
private:
	std::weak_ptr<GraphicSystem>		m_pGraphicSystem;
	std::weak_ptr<InputSystem>			m_pInputSystem;
	std::weak_ptr<TimeSystem>			m_pTimeSystem;
	std::weak_ptr<SceneSystem>			m_pSceneSystem;
	std::weak_ptr<ResourceSystem>		m_pResourceSystem;
private:
	explicit CoreSystem();
	virtual ~CoreSystem() = default;
	// Object��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	HRESULT ReadyEngine();
	HRESULT UpdateEngine();
};
END
#endif // !__CORE_SYSTEM_H__