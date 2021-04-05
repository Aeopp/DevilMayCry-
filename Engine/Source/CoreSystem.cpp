#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "CoreSystem.h"
#include "GraphicSystem.h"
#include "InputSystem.h"
#include "TimeSystem.h"
#include "SceneSystem.h"
#include "ResourceSystem.h"
#include "Renderer.h"
#include "IconsFontAwesome5.h"


USING(ENGINE)
IMPLEMENT_SINGLETON(CoreSystem)

static  void  SetupImGuiStyle(bool bStyleDark_, float alpha_)
{
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0f;
		style.FrameRounding = 3.0f;
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);

		if (bStyleDark_)
		{
			for (int i = 0; i <= ImGuiCol_COUNT; i++)
			{
				ImVec4& col = style.Colors[i];
				float H, S, V;
				ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

				if (S < 0.1f)
				{
					V = 1.0f - V;
				}
				ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
				if (col.w < 1.00f)
				{
					col.w *= alpha_;
				}
			}
		}
		else
		{
			for (int i = 0; i <= ImGuiCol_COUNT; i++)
			{
				ImVec4& col = style.Colors[i];
				if (col.w < 1.00f)
				{
					col.x *= alpha_;
					col.y *= alpha_;
					col.z *= alpha_;
					col.w *= alpha_;
				}
			}
		}
}




CoreSystem::CoreSystem()
{

}

void CoreSystem::Free()
{
	m_pSceneSystem.reset();
	SceneSystem::DeleteInstance();

	m_pResourceSystem.reset();
	ResourceSystem::DeleteInstance();

	m_pGraphicSystem.reset();
	GraphicSystem::DeleteInstance();

	m_pInputSystem.reset();
	InputSystem::DeleteInstance();

	m_pTimeSystem.reset();
	TimeSystem::DeleteInstance();


	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}

HRESULT CoreSystem::ReadyEngine(const bool bWindowed,
								const bool bMultiSample)
{
	m_pGraphicSystem = GraphicSystem::GetInstance();
	if (nullptr == m_pGraphicSystem.lock() || FAILED(m_pGraphicSystem.lock()->ReadyGraphicSystem(
		bWindowed,
		bMultiSample)))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to ReadyEngine."));
		return E_FAIL;
	}

	m_pInputSystem = InputSystem::GetInstance();
	if (nullptr == m_pInputSystem.lock() || FAILED(m_pInputSystem.lock()->ReadyInputSystem()))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to ReadyEngine."));
		return E_FAIL;
	}

	m_pTimeSystem = TimeSystem::GetInstance();
	if (nullptr == m_pTimeSystem.lock() || FAILED(m_pTimeSystem.lock()->ReadyTimeSystem()))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to ReadyEngine."));
		return E_FAIL;
	}

	m_pSceneSystem = SceneSystem::GetInstance();
	if (nullptr == m_pSceneSystem.lock() || FAILED(m_pSceneSystem.lock()->ReadySceneSystem()))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to ReadyEngine."));
		return E_FAIL;
	}

	m_pResourceSystem = ResourceSystem::GetInstance();
	if (nullptr == m_pResourceSystem.lock() || FAILED(m_pResourceSystem.lock()->ReadyResourceSystem(m_pGraphicSystem.lock()->GetDevice())))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to ReadyEngine."));
		return E_FAIL;
	}

	m_pRenderer = Renderer::GetInstance();
	if (nullptr == m_pRenderer.lock() || FAILED(m_pRenderer.lock()->ReadyRenderSystem(m_pGraphicSystem.lock()->GetDevice())))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to ReadyEngine."));
		return E_FAIL;
	}

	g_bDebugMode = false;
	g_bEditMode  = false;
	g_bCollisionVisible = false;
	g_bRenderTargetVisible = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	ImGui::StyleColorsDark();
	SetupImGuiStyle(true, 0.4f);
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(m_pGraphicSystem.lock()->GetDevice());

	//
	//io.Fonts->AddFontDefault();

	//// merge in icons from Font Awesome
	//static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	//ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	//io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 16.0f, &icons_config, icons_ranges);
	//// use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid

	//// in an imgui window somewhere...
	//ImGui::Text(ICON_FA_PAINT_BRUSH "  Paint");    // use string literal concatenation
	//// outputs a paint brush icon and 'Paint' as a string.

	io.Fonts->AddFontFromFileTTF("..\\..\\Resource\\Font\\Roboto\\Roboto-Regular.ttf", 18.0f);
	io.Fonts->AddFontFromFileTTF("..\\..\\Resource\\Font\\Roboto\\Roboto-Regular.ttf", 10);
	io.Fonts->AddFontFromFileTTF("..\\..\\Resource\\Font\\Roboto\\Roboto-Regular.ttf", 14);
	io.Fonts->AddFontFromFileTTF("..\\..\\Resource\\Font\\Roboto\\Roboto-Regular.ttf", 18);

	return S_OK;
}

static void GlobalVariableEditor()
{
	ImGui::Begin("System");
	{
		ImGui::Checkbox("bDebug", &g_bDebugMode);
		if (g_bDebugMode)
		{
			ImGui::Checkbox("bEditor", &g_bEditMode);
			ImGui::Checkbox("bCollisionVisible", &g_bCollisionVisible);
			ImGui::Checkbox("bRenderTargetVisible", &g_bRenderTargetVisible);
		}
	}
	ImGui::End();
}

HRESULT CoreSystem::UpdateEngine()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
	GlobalVariableEditor();

	if (FAILED(m_pInputSystem.lock()->UpdateInputSystem()))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to UpdateInputSystem."));
		return E_FAIL;
	}
	if (FAILED(m_pTimeSystem.lock()->UpdateTimeSystem()))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to UpdateTimeSystem."));
		return E_FAIL;
	}
	if (FAILED(m_pSceneSystem.lock()->UpdateSceneSystem(m_pTimeSystem.lock()->DeltaTime())))
	{
		PRINT_LOG(TEXT("Error"), TEXT("Failed to UpdateSceneSystem."));
		return E_FAIL;
	}
	if (FAILED(m_pSceneSystem.lock()->LateUpdateSceneSystem(m_pTimeSystem.lock()->DeltaTime())))
	{
		PRINT_LOG(TEXT("Error"),TEXT("Failed to LateUpdateSceneSystem."));
		return E_FAIL;
	}
	if (g_bDebugMode)
	{
		if (g_bEditMode)
		{
			ImGui::Begin("Object Editor");
			m_pSceneSystem.lock()->EditUpdateSceneSystem();
			ImGui::End();

			ImGui::Begin("Log");
			for (const auto& CurLog : g_Logs)
			{
				ImGui::Text(CurLog.c_str());
			}
			ImGui::End();
		}
		g_Logs.clear();
	}
	

	if (FAILED(m_pRenderer.lock()->Render()))
	{
		PRINT_LOG(TEXT("Error"),TEXT("Failed to Renderer Render."));
		return E_FAIL;
	}

	
	return S_OK;
}

