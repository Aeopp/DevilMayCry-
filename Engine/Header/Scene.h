#ifndef __SCENE_H__
#define __SCENE_H__
#include "Object.h"
BEGIN(ENGINE)
class GameObject;
class ENGINE_DLL Scene abstract : public Object
{
private:
	enum OBJECTPOOLTYPE { POOL_STATIC, POOL_DYNAMIC, POOL_DESTROY, POOL_END };
	enum LOOPSTATE		{ LOOP_AWAKE, LOOP_START, LOOP_UPDATE, LOOP_END };
	enum ACTIVESTATE	{ ACTIVE, INACTIVE, ACTIVE_END };
private:
	typedef std::list<std::shared_ptr<GameObject>>	OBJECTPOOL;
	typedef std::list<std::weak_ptr<GameObject>>	LOOPPOOL;

	OBJECTPOOL			m_Pool[POOL_END];
	LOOPPOOL			m_Loop[ACTIVE_END][LOOP_END];
	LOOPPOOL::iterator	m_LoopIter;

protected:
	explicit Scene();
	virtual ~Scene() = default;
	// Object��(��) ���� ��ӵ�
	virtual void Free() PURE;
public:
	virtual HRESULT LoadScene()							PURE;
	virtual	HRESULT	Awake()								PURE;
	virtual HRESULT Start()								PURE;
	virtual HRESULT	Update(const float _fDeltaTime)		PURE;
	virtual HRESULT LateUpdate(const float _fDeltaTime)	PURE;
	
	void	Destroy();
public:
	void	TransferStatic(std::weak_ptr<Scene> const _pDestScene);
public:
	template <typename TYPE>
	std::weak_ptr<TYPE> AddGameObject();
public:
	void Destroy(std::weak_ptr<GameObject> const _pGameObject);
	void SetActive(std::weak_ptr<GameObject> const _pGameObject, const bool _bActive);
	void SetStatic(std::weak_ptr<GameObject> const _pGameObject, const bool _bStatic);
};
END
#include "Scene.hpp"
#endif // !__SCENE_H__
