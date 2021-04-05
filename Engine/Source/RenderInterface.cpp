#include "RenderInterface.h"
#include "Renderer.h"
#include "Resources.h"
#include "Shader.h"

USING(ENGINE)
void RenderInterface::Initialize(const RenderProperty& _RenderProp ,
								 const std::filesystem::path& ForwardAlphaBlendShaderFilePath)&
{
	this->_RenderProperty = _RenderProp;

	_InitializeInfo.ForwardAlphaBlendShader =
		Resources::Load<ENGINE::Shader>(ForwardAlphaBlendShaderFilePath);
};

void  RenderInterface::RenderVariableBind(const UpdateInfo& CurUpdateInfo)
{
	_UpdateInfo = CurUpdateInfo;
}

void RenderInterface::RenderForwardAlphaBlend()
{
	if (false ==_UpdateInfo.bRender)
	{
		return;
	};
	
	const auto& _CurRenderInfo = Renderer::GetInstance()->CurrentRenderInfo;
	_CurRenderInfo.CameraView;
	_CurRenderInfo.CameraProjection;
	
	if (_InitializeInfo.ForwardAlphaBlendShader)
	{
		auto Fx = _InitializeInfo.ForwardAlphaBlendShader->GetEffect();
		Fx->SetMatrix("World", &_UpdateInfo.World);
		Fx->SetMatrix("View", &_CurRenderInfo.CameraView);
		Fx->SetMatrix("Projection", &_CurRenderInfo.CameraProjection);

		UINT Passes{ 0u };
		if (FAILED(Fx->Begin(&Passes, NULL)))
		{
			PRINT_LOG(L"Failed!!", __FUNCTIONW__);
		}
		else
		{
			for (uint32 PassIdx = 0u; PassIdx < Passes; ++PassIdx)
			{
				Fx->BeginPass(PassIdx);
				ImplementationInfo _ImplInfo{};
				_ImplInfo.Fx = Fx;
				RenderForwardAlphaBlendImplementation(_ImplInfo);
				Fx->EndPass();
			}
			Fx->End();
		}
	};
};
