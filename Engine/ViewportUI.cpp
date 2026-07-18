#include "ViewportUI.h"
#include "UIManager.h"
#include "CameraHandler.h"

ViewportUI::ViewportUI() {
	m_isActive = true;
}

ViewportUI::~ViewportUI() {
}

void ViewportUI::setViewportSRVs(ID3D11ShaderResourceView* editor_srv, ID3D11ShaderResourceView* game_srv) {
	m_editor_srv = editor_srv;
	m_game_srv = game_srv;
}

ImVec2 ViewportUI::getEditorViewportSize() const {
	return m_editor_size;
}

ImVec2 ViewportUI::getGameViewportSize() const {
	return m_game_size;
}

void ViewportUI::draw() {
	if (!m_isActive) return;

	ImGuiIO& io = ImGui::GetIO();
	float menu_h = 20.0f;
	float scr_w = io.DisplaySize.x;
	float scr_h = io.DisplaySize.y;
	float work_h = scr_h - menu_h;

	// ---- Editor Viewport Window ----
	if (m_show_editor_view) {
		ImGui::SetNextWindowPos(ImVec2(scr_w * 0.20f, menu_h), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(scr_w * 0.35f, work_h * 0.70f), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Editor Viewport", &m_show_editor_view, ImGuiWindowFlags_NoCollapse)) {
			Camera* scene = CameraHandler::get()->getSceneCamera();
			GameCamera* game = CameraHandler::get()->getGameCamera();

			if (game && scene) {
				if (ImGui::Button("Align with View")) {
					game->setPosition(scene->getPosition());
					game->setRotation(scene->getRotation());
				}
				ImGui::Separator();
			}

			ImVec2 size = ImGui::GetContentRegionAvail();
			if (size.x < 64) size.x = 64;
			if (size.y < 64) size.y = 64;
			m_editor_size = size;

			if (m_editor_srv)
				ImGui::Image((void*)m_editor_srv, size);
			else
				ImGui::Text("Editor Viewport Texture Not Bound");
		}
		ImGui::End();
	}

	// ---- Game Viewport Window ----
	if (m_show_game_view) {
		ImGui::SetNextWindowPos(ImVec2(scr_w * 0.55f, menu_h), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(scr_w * 0.25f, work_h * 0.70f), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Game Viewport", &m_show_game_view, ImGuiWindowFlags_NoCollapse)) {
			ImVec2 size = ImGui::GetContentRegionAvail();
			if (size.x < 64) size.x = 64;
			if (size.y < 64) size.y = 64;
			m_game_size = size;

			if (m_game_srv)
				ImGui::Image((void*)m_game_srv, size);
			else
				ImGui::Text("Game Viewport Texture Not Bound");
		}
		ImGui::End();
	}
}
