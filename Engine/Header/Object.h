#ifndef __OBJECT_H__
#define __OBJECT_H__
#include "EngineStdafx.h"
BEGIN(ENGINE)
class ENGINE_DLL Object abstract
{
	friend Deleter<Object>;
protected:
	explicit Object();
	virtual ~Object() = default;
	virtual void Free()	PURE;
};
END
#endif // !__OBJECT_H__

