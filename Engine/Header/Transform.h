#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__
#include "Component.h"
BEGIN(ENGINE)
class Transform final : public Component
{
private:
	explicit Transform(std::weak_ptr<GameObject> const _pGameObject);
	virtual ~Transform() = default;
	// Component��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static Transform* Create(std::weak_ptr<GameObject> const _pGameObject);
};
END
#endif // !__TRANSFORM_H__