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
	/// <��ü���� �ൿ�� �޶����ٸ� �ݵ�� ���� �׷��� �ʴٸ� Interface �Լ��� ȣ��...>
	virtual void    RenderForwardAlphaBlend();
	virtual void    RenderForwardAlphaBlendImplementation(const ImplementationInfo&
															_ImplInfo)PURE;
	/// </summary>
public:
	///<��ü���� �غ��ؾ� �ϴ� �ൿ�� �ϸ� �׷��� �ʴٸ� RenderReadyUpdate ȣ��...>
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




