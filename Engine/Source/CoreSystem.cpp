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


USING(ENGINE)
IMPLEMENT_SINGLETON(CoreSystem)

static void SetupImGuiStyle()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_::ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_::ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
};

	/*ImGuiIoRef.Fonts->AddFontFromFileTTF("..\\..\\..\\Resource\\Font\\Ruda\\static\\Ruda-Bold.ttf", 12);
	ImGuiIoRef.Fonts->AddFontFromFileTTF(("..\\..\\..\\Resource\\Font\\Ruda\\static\\Ruda-Bold.ttf"), 10);
	ImGuiIoRef.Fonts->AddFontFromFileTTF(("..\\..\\..\\Resource\\Font\\Ruda\\static\\Ruda-Bold.ttf"), 14);
	ImGuiIoRef.Fonts->AddFontFromFileTTF(("..\\..\\..\\Resource\\Font\\Ruda\\static\\Ruda-Bold.ttf"), 18);
	*/


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

HRESULT CoreSystem::ReadyEngine()
{
	m_pGraphicSystem = GraphicSystem::GetInstance();
	if (nullptr == m_pGraphicSystem.lock() || FAILED(m_pGraphicSystem.lock()->ReadyGraphicSystem()))
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

	bDebugMode = false;
	bEditMode  = false;
	bDebugCollision = false;
	bDebugRenderTargetRender = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	ImGui::StyleColorsDark();
	SetupImGuiStyle();
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(m_pGraphicSystem.lock()->GetDevice());

	return S_OK;
}

HRESULT CoreSystem::UpdateEngine()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();

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
	if (FAILED(m_pRenderer.lock()->Render()))
	{
		PRINT_LOG(TEXT("Error"),TEXT("Failed to Renderer Render."));
		return E_FAIL;
	}

	return S_OK;
}

