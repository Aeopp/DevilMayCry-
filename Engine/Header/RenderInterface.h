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
	struct ShaderInfo
	{
		std::shared_ptr<ENGINE::Shader> ForwardAlphaBlendShader{};
		std::shared_ptr<ENGINE::Shader> GBufferShader{};
		std::shared_ptr<ENGINE::Shader> AlphaBlendEffectShader{};
		//std::shared_ptr<ENGINE::Shader> UIShader{};
	};
	struct UpdateInfo
	{
		Matrix World;
	};
	struct ImplementationInfo
	{
		ID3DXEffect* Fx{};
	};
	void Initialize(const RenderProperty&           _RenderProp)&;
	RenderProperty  GetRenderProp()const& { return  _RenderProperty;  };
	/// <��ü���� �ൿ�� �޶����ٸ� �ݵ�� ���� �׷��� �ʴٸ� Interface �Լ��� ȣ��...>
	virtual void    RenderForwardAlphaBlend();
	virtual void    RenderForwardAlphaBlendImplementation(const ImplementationInfo&_ImplInfo);

	virtual void    RenderGBuffer();
	virtual void    RenderGBufferImplementation(const ImplementationInfo& _ImplInfo);
	
	virtual void	RenderAlphaBlendEffect();
	virtual void    RenderAlphaBlendEffectImplementation(const ImplementationInfo& _ImplInfo);
	
	virtual void	RenderUI();
	virtual void    RenderUIImplementation(const ImplementationInfo& _ImplInfo);
	/// </summary>
public:
	///<��ü���� �غ��ؾ� �ϴ� �ൿ�� �ϸ� �׷��� �ʴٸ� RenderReadyUpdate ȣ��...>
	virtual void    RenderReady()      PURE;
protected:
	void RenderVariableBind(const UpdateInfo& CurUpdateInfo);
	UpdateInfo _UpdateInfo{};
	///  
public:
	ShaderInfo _ShaderInfo{};
	RenderProperty _RenderProperty{};
};
END

#endif // !_RENDERINTERFACE_H_




