#include "RenderInterface.h"
#include "Renderer.h"
#include "Resources.h"
#include "Shader.h"

USING(ENGINE)
void RenderInterface::Initialize(const RenderProperty& _RenderProp)&
{
	this->_RenderProperty = _RenderProp;
};

void  RenderInterface::RenderVariableBind(const UpdateInfo& CurUpdateInfo)
{
	_UpdateInfo = CurUpdateInfo;
}

void RenderInterface::RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo) {};

void RenderInterface::RenderForwardAlphaBlend()
{
	const auto& _CurRenderInfo = Renderer::GetInstance()->CurrentRenderInfo;

	if (_ShaderInfo.ForwardAlphaBlendShader)
	{
		auto Fx = _ShaderInfo.ForwardAlphaBlendShader->GetEffect();
		Fx->SetMatrix("World", &_UpdateInfo.World);
		Fx->SetMatrix("View", &_CurRenderInfo.CameraView);
		Fx->SetMatrix("Projection", &_CurRenderInfo.CameraProjection);
		/// <summary>
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
void RenderInterface::RenderGBufferImplementation(const ImplementationInfo& _ImplInfo) {};
void RenderInterface::RenderGBuffer()
{
	const auto& _CurRenderInfo = Renderer::GetInstance()->CurrentRenderInfo;

	if (_ShaderInfo.GBufferShader)
	{
		auto Fx = _ShaderInfo.GBufferShader->GetEffect();
		Fx->SetMatrix("World", &_UpdateInfo.World);
		Fx->SetMatrix("View", &_CurRenderInfo.CameraView);
		Fx->SetMatrix("Projection", &_CurRenderInfo.CameraProjection);
		/// <summary>
		UINT Passes{ 0u };
		if (FAILED(Fx->Begin(&Passes, NULL)))
		{
			PRINT_LOG(L"Failed!!",__FUNCTIONW__);
		}
		else
		{
			for (uint32 PassIdx = 0u; PassIdx < Passes; ++PassIdx)
			{
				Fx->BeginPass(PassIdx);
				ImplementationInfo _ImplInfo{};
				_ImplInfo.Fx = Fx;
				RenderGBufferImplementation(_ImplInfo);
				Fx->EndPass();
			}
			Fx->End();
		}
	};
};

void RenderInterface::RenderAlphaBlendEffectImplementation(const ImplementationInfo& _ImplInfo) {}

void RenderInterface::RenderAlphaBlendEffect()
{
	const auto& _CurRenderInfo = Renderer::GetInstance()->CurrentRenderInfo;

	if (_ShaderInfo.AlphaBlendEffectShader)
	{
		auto Fx = _ShaderInfo.AlphaBlendEffectShader->GetEffect();
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
			ImplementationInfo _ImplInfo{};
			_ImplInfo.Fx = Fx;
			RenderAlphaBlendEffectImplementation(_ImplInfo);
	
			Fx->End();
		}
	}
}

void RenderInterface::RenderUIImplementation(const ImplementationInfo& _ImplInfo) {}

void RenderInterface::RenderUI()
{
	const auto& _CurRenderInfo = Renderer::GetInstance()->CurrentRenderInfo;

	if (_ShaderInfo.UIShader)
	{
		auto Fx = _ShaderInfo.UIShader->GetEffect();
		Fx->SetMatrix("Ortho", &_CurRenderInfo.Ortho);

		UINT Passes{ 0u };
		if (FAILED(Fx->Begin(&Passes, NULL)))
		{
			PRINT_LOG(L"Failed!!", __FUNCTIONW__);
		}
		else
		{
			ImplementationInfo _ImplInfo{};
			_ImplInfo.Fx = Fx;
			RenderUIImplementation(_ImplInfo);

			Fx->End();
		}
	}
}