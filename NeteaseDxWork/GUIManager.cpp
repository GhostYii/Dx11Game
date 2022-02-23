#pragma once
#include "GUIManager.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"

GUIManager::GUIManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

GUIManager::~GUIManager()
{
	ImGui::DestroyContext();
}

void GUIManager::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();
}

void GUIManager::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
