#ifndef _RENDERINTERFACE_H_
#define _RENDERINTERFACE_H_
#include "EngineTypedef.h"
#include "EngineDefine.h"
#include "RenderProperty.h"
#include <filesystem>
#include "Shader.h"

BEGIN(ENGINE)
class ENGINE_DLL RenderInterface
{
public:
	struct InitializeInfo
	{
		std::shared_ptr<ENGINE::Shader> ForwardAlphaBlendShader{};
	};
	struct UpdateInfo
	{
		Matrix World;
		bool bRender = true;
	};
	struct ImplementationInfo
	{
		ID3DXEffect* Fx{};
	};
	void Initialize(const RenderProperty&        _RenderProp,
					const std::filesystem::path& ForwardAlphaBlendShaderFilePath)&;
	RenderProperty  GetRenderProp()const& { return  _RenderProperty;  };
	/// <객체마다 행동이 달라진다면 반드시 정의 그렇지 않다면 Interface 함수를 호출...>
	virtual void    RenderForwardAlphaBlend();
	virtual void    RenderForwardAlphaBlendImplementation(const ImplementationInfo&
															_ImplInfo)PURE;
	/// </summary>
public:
	///<객체마다 준비해야 하는 행동을 하며 그렇지 않다면 RenderReadyUpdate 호출...>
	virtual void    RenderReady()      PURE;
protected:
	void RenderVariableBind(const UpdateInfo& CurUpdateInfo);
	///  
protected:
	InitializeInfo _InitializeInfo{};
	UpdateInfo _UpdateInfo{};
private:
	RenderProperty _RenderProperty{};
};
END

#endif // !_RENDERINTERFACE_H_




