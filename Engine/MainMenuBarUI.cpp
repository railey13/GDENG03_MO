#include "MainMenuBarUI.h"
#include "UIManager.h"
#include "AppWindow.h"

#include "SceneHierarchyUI.h"
#include "InspectorUI.h"
#include "CameraUI.h"
#include "StressTestUI.h"

MainMenuBarUI::MainMenuBarUI() {

}

MainMenuBarUI::~MainMenuBarUI() {

}

void MainMenuBarUI::draw() {
	UIManager* ui = UIManager::get();

	if (ImGui::BeginMainMenuBar()) {

		// --- File Menus ---
		if (ImGui::BeginMenu("GameObjects")) {
			if (ImGui::MenuItem("Cube")) {
				AppWindow::get()->getInvoker().executeCommand((int)Action::SpawnCube);
			}
			if (ImGui::MenuItem("Sphere")) {
				AppWindow::get()->getInvoker().executeCommand((int)Action::SpawnSphere);
			}
			if (ImGui::MenuItem("Plane")) {
				AppWindow::get()->getInvoker().executeCommand((int)Action::SpawnPlane);
			}			
			if (ImGui::MenuItem("Capsule")) {
				AppWindow::get()->getInvoker().executeCommand((int)Action::SpawnCapsule);
			}
			if (ImGui::MenuItem("Camera")) {
				AppWindow::get()->getInvoker().executeCommand((int)Action::SpawnCamera);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Spawn 20 Cubes [Stack Test]")) {
				AppWindow::get()->spawnStressCubes(20, true);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Panels")) {
			if (ImGui::MenuItem("Scene Hierarchy")) {
				if (SceneHierarchyUI* hierarchy = ui->getUI<SceneHierarchyUI>()) {
					hierarchy->toggleActive();
				}
			}
			if (ImGui::MenuItem("Inspector")) {
				if (InspectorUI* inspector = ui->getUI<InspectorUI>()) {
					inspector->toggleActive();
				}
			}
			if (ImGui::MenuItem("Stress Test")) {
				if (StressTestUI* stress = ui->getUI<StressTestUI>()) {
					stress->toggleActive();
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Camera")){
			if (CameraUI* camera = ui->getUI<CameraUI>()) {
				camera->toggleActive();
			}
		}

		// --- Centered Play / Stop Buttons ---
		const float btnW    = 70.0f;
		const float spacing = ImGui::GetStyle().ItemSpacing.x;
		const float totalW  = btnW * 2.0f + spacing;
		const float barW    = ImGui::GetWindowWidth();
		ImGui::SetCursorPosX((barW - totalW) * 0.5f);

		const bool inPlay = (m_scene_state == SceneState::Play);

		// Play button (green, disabled during Play)
		if (inPlay) ImGui::BeginDisabled();
		ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.13f, 0.55f, 0.13f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.72f, 0.18f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.10f, 0.42f, 0.10f, 1.00f));
		if (ImGui::Button("  > Play", ImVec2(btnW, 0))) {
			AppWindow::get()->onPlay();
		}
		ImGui::PopStyleColor(3);
		if (inPlay) ImGui::EndDisabled();

		ImGui::SameLine();

		// Stop button (red, disabled during Edit)
		if (!inPlay) ImGui::BeginDisabled();
		ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.60f, 0.10f, 0.10f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.15f, 0.15f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.45f, 0.08f, 0.08f, 1.00f));
		if (ImGui::Button("  [] Stop", ImVec2(btnW, 0))) {
			AppWindow::get()->onStop();
		}
		ImGui::PopStyleColor(3);
		if (!inPlay) ImGui::EndDisabled();

		ImGui::EndMainMenuBar();
	}
}
