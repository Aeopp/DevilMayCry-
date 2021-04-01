#ifndef __CORE_H__
#define __CORE_H__
#include "EngineStdafx.h"
BEGIN(ENGINE)
class CoreSystem;
class ENGINE_DLL Engine abstract
{
private:
	static std::weak_ptr<CoreSystem> m_pCoreSystem;
public:
	static HRESULT ReadyEngine(const bool bWindowed);
	static HRESULT UpdateEngine();
	static HRESULT ReleaseEngine();
};
END
#endif // !__CORE_H__
