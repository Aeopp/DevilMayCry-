#ifndef _RENDERINTERFACE_H_
#define _RENDERINTERFACE_H_
#include "EngineTypedef.h"
#include "EngineDefine.h"
#include "RenderProperty.h"
#include <filesystem>
#include "Shader.h"
#include <map>

BEGIN(ENGINE)
class ENGINE_DLL RenderInterface
{
public:
	struct ENGINE_DLL ShaderInfo
	{
	public:
		void RegistShader(const RenderProperty::Order& RegistOrder,
	/* �Ķ���ʹ� ���̴��� Create�� ����ȭ !*/
						  const std::filesystem::path& LoadPath, 
						  const std::any& InitParams)&;
		std::shared_ptr<ENGINE::Shader> GetShader(const RenderProperty::Order& _RegistOrder)const&;
	private:
		std::map<RenderProperty::Order, std::shared_ptr<ENGINE::Shader>>_Shaders{};
	};
	struct ENGINE_DLL UpdateInfo
	{
		Matrix World;
	};
	struct ENGINE_DLL ImplementationInfo
	{
		ID3DXEffect* Fx{};
		uint32 PassIndex{ 0u };
	};
	void Initialize(
		const RenderProperty& _RenderProp)&;

	RenderProperty  GetRenderProp()const& 
	{ return  _RenderProperty;  };

	/// <��ü���� �ൿ�� �޶����ٸ� �ݵ�� ���� �׷��� �ʴٸ� Interface �Լ��� ȣ��...>
	// virtual void    RenderForwardAlphaBlend();
	virtual void    RenderForwardAlphaBlendImplementation(
		const ImplementationInfo&_ImplInfo);

	// virtual void    RenderGBuffer();
	virtual void    RenderGBufferImplementation(
		const ImplementationInfo& _ImplInfo);

	virtual void RenderShadowImplementation(
				const ImplementationInfo& _ImplInfo);
	
	// virtual void	RenderAlphaBlendEffect();
	virtual void    RenderAlphaBlendEffectImplementation(
		const ImplementationInfo& _ImplInfo);
	
	// virtual void	RenderUI();
	virtual void    RenderUIImplementation(
		const ImplementationInfo& _ImplInfo);

	// virtual void	RenderDebug();
	virtual void    RenderDebugImplementation(
		const ImplementationInfo& _ImplInfo);

	// virtual void	RenderDebugBone();
	virtual void    RenderDebugBoneImplementation(
		const ImplementationInfo& _ImplInfo);

	UpdateInfo GetRenderUpdateInfo()const& 
	{
		return _UpdateInfo; 
	};
	/// </summary>
public:
	///<��ü���� �غ��ؾ� �ϴ� �ൿ�� �ϸ� �׷��� �ʴٸ� RenderReadyUpdate ȣ��...>
	virtual void    RenderReady()      PURE;
protected:
	void RenderVariableBind(const UpdateInfo& CurUpdateInfo);
	UpdateInfo _UpdateInfo{};
	///  
public:
	ShaderInfo     _ShaderInfo{};
	RenderProperty _RenderProperty{};
};
END

#endif // !_RENDERINTERFACE_H_




