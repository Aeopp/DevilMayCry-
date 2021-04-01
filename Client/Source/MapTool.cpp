#include "stdafx.h"
#include "MapTool.h"

#include <iostream>

using namespace std;

MapTool::MapTool()
	:m_iPeekCnt(0)
	,m_iPeekType(0)
	,m_fPivotMoveSpeed(0.1f)
{
}

void MapTool::Free()
{
	Scene::Free();
}
MapTool* MapTool::Create()
{
	MapTool* pInstance = new MapTool();
	return pInstance;
}


void MapTool::PivotControl()
{
	vec3 vPivotPos; // =  get Position

	if (Input::GetKeyDown(VK_LEFT)) // x Axis minus
		vPivotPos.x += -m_fPivotMoveSpeed;
	else if (Input::GetKeyDown(VK_RIGHT)) // x Axis plus 
		vPivotPos.x += m_fPivotMoveSpeed;
	else if (Input::GetKeyDown(VK_UP))  // z Axis plus
		vPivotPos.x += m_fPivotMoveSpeed;
	else if (Input::GetKeyDown(VK_DOWN)) // z Axis minus
		vPivotPos.x += m_fPivotMoveSpeed;
	else if (Input::GetKeyDown(VK_HOME))// y Axis plus
		vPivotPos.x += m_fPivotMoveSpeed;
	else if (Input::GetKeyDown(VK_END))	 // y Axis minus
		vPivotPos.x += m_fPivotMoveSpeed;
	//pivot->getTrans()->SetPos(vPos)
}

void MapTool::MouseInPut()
{
	//피킹 타입에 분기 때리고 피킹 카운트 제어 
	if (m_iPeekType == (int)ePeekingType::Single)
	{

	}
	else
	{

	}

}

void MapTool::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void MapTool::ShowMapTool()
{
	ImGui::Begin("Hellow MapTool");
	ImGui::SetWindowPos(ImVec2(0, 10));
	ImGui::SetWindowSize(ImVec2(400, 600));
	ImGui::Text("BaseMap"); ImGui::SameLine();
	HelpMarker("Write FBX Path");
	static char szBaseMapPath[MAX_PATH] = "";
	ImGui::InputText("", szBaseMapPath,MAX_PATH); ImGui::SameLine();
	//ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
	if (ImGui::Button("Load"))
	{
		//체크전에 로드 된거 있으면 릴리즈 
	}
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3 / 7.0f, 0.8f, 0.8f));
	if (ImGui::Button("Delete"))
	{

	}
	ImGui::PopStyleColor(3);
	if(ImGui::CollapsingHeader("Props Tree"))
	{
		ImGui::Text("Test");
		ImGui::Text("Test");
		ImGui::Text("Test");
		ImGui::Text("Test");
		ImGui::Text("Test");
	}
	if (ImGui::CollapsingHeader("InterationObject Tree"))
	{
		ImGui::Text("Test");
		ImGui::Text("Test");
		ImGui::Text("Test");
		ImGui::Text("Test");
		ImGui::Text("Test");
		ImGui::Text("Test");
	}
	ImGui::Text("PeekingType");
	ImGui::RadioButton("Single", &m_iPeekType, 0); ImGui::SameLine();
	ImGui::RadioButton("Multi", &m_iPeekType, 1);

	static char szObjName[64] = "TestName";

	if (m_iPeekType == (int)ePeekingType::Single)
	{
		ImGui::Text("Peeking Object Name : %s", szObjName);
	}
	else
	{
		ImGui::Text("Peeking Object Cnt: %d", m_iPeekCnt);
	}



	ImGui::Text("---------Transform---------\n\n");
	ImGui::Text("Drag Sensitivity"); ImGui::SameLine();
	HelpMarker("Edit value Drag Power"); ImGui::SameLine();
	HelpMarker(
		"Drag to edit value.\n"
		"Hold SHIFT/ALT for faster/slower edit.\n"
		"Double-click or CTRL+click to input value.");
	static float fSensitivety = 0.1f;
	static vec3 TestScale = vZero;  // obj pos Get 
	static vec3 TestRot = vZero;  // obj pos Get 
	static vec3 TestPos = vZero;  // obj pos Get 

	ImGui::InputFloat("", &fSensitivety, 0.1f);
	ImGui::Text("Sacle");
	ImGui::SameLine();
	ImGui::DragFloat3(".", (float*)&TestScale.x, fSensitivety);
	ImGui::Text("Rotate");
	ImGui::SameLine();
	ImGui::DragFloat3(" ", (float*)&TestRot.x, fSensitivety);
	ImGui::Text("Positon"); ImGui::SameLine(); 
	ImGui::DragFloat3("   ", (float*)&TestPos.x, fSensitivety);


	ImGui::Text("Option");

	ImGui::Checkbox("Decoration", &m_bPropsOptino[ePropsOption::Decoration]); ImGui::SameLine();
	ImGui::Checkbox("Floating", &m_bPropsOptino[ePropsOption::Floating]); ImGui::SameLine();
	ImGui::Checkbox("Fixed", &m_bPropsOptino[ePropsOption::Fixed]); ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));


	if (ImGui::Button("Apply"))
	{

	}

	ImGui::PopStyleColor(3);
	//ImGui::PopID();

	ImGui::End();
}

void MapTool::ShowPivotControl()
{

	ImGui::Begin("Hellow Pivot"); ImGui::SameLine();
	ImGui::SetWindowPos(ImVec2(g_nWndCX- 350, g_nWndCY - 200));
	ImGui::SetWindowSize(ImVec2(350, 200));
	
	ImGui::Text("Drag Sensitivity"); ImGui::SameLine(); 
	HelpMarker("Edit value Drag Power");
	static float fSensitivety = 0.1f;
	ImGui::InputFloat("", &fSensitivety, 0.1f);

	ImGui::Text("Move Sensitivity"); ImGui::SameLine();
	HelpMarker("Also can be moved by keyboard\n"
		"x :left ,right z : up Down  y : home , end");
	ImGui::InputFloat(".", &m_fPivotMoveSpeed, 0.1f);


	ImGui::Text("Position"); ImGui::SameLine(); 
	HelpMarker(
		"Drag to edit value.\n"
		"Hold SHIFT/ALT for faster/slower edit.\n"
		"Double-click or CTRL+click to input value.");
	static vec3 TestPos = vZero;  // pivot pos Get 
	ImGui::DragFloat3("", (float*)&TestPos.x,fSensitivety);

	ImGui::End();
}



HRESULT MapTool::LoadScene()
{
	Scene::LoadScene();

	return S_OK;
}

HRESULT MapTool::Awake()
{
	Scene::Awake();

	return S_OK;
}

HRESULT MapTool::Start()
{
	Scene::Start();
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0,0,0,255);
	m_iPeekCnt = 0;
	ZeroMemory(m_bPropsOptino, sizeof(bool) * ePropsOption::End);
	return S_OK;
}

HRESULT MapTool::Update(const float _fDeltaTime)
{
	Scene::Update(_fDeltaTime);

	ShowMapTool();
	ShowPivotControl();
	return S_OK;
}

HRESULT MapTool::LateUpdate(const float _fDeltaTime)
{
	Scene::LateUpdate(_fDeltaTime);

	return S_OK;
}
