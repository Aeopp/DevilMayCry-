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
	/* 파라미터는 쉐이더의 Create와 동기화 !*/
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

	/// <객체마다 행동이 달라진다면 반드시 정의 그렇지 않다면 Interface 함수를 호출...>
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
	///<객체마다 준비해야 하는 행동을 하며 그렇지 않다면 RenderReadyUpdate 호출...>
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




