#include "MainMenuBarUI.h"
#include "UIManager.h"
#include "AppWindow.h"

#include "SceneHierarchyUI.h"
#include "InspectorUI.h"
#include "CameraUI.h"
#include "StressTestUI.h"
#include "../IMGUI/ImGuiFileDialog.h"
#include "SceneLoader.h"

MainMenuBarUI::MainMenuBarUI() {

}

MainMenuBarUI::~MainMenuBarUI() {

}

void MainMenuBarUI::draw() {
	UIManager* ui = UIManager::get();
	char fileName[128] = "";
	if (ImGui::BeginMainMenuBar()) {

		// --- File Menus ---
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Save Scene")) {
				IGFD::FileDialogConfig config;
				config.path = "Scenes";

				ImGuiFileDialog::Instance()->OpenDialog(
					"SaveSceneDlg",
					"Save Scene",
					".yml",
					config
				);
			}

			

			
			
			if (ImGui::MenuItem("Load Scene")) {
				IGFD::FileDialogConfig config;
				config.path = "Scenes";

				ImGuiFileDialog::Instance()->OpenDialog(
					"LoadSceneDlg",
					"Choose SceneFile",
					".yml",
					config
				);
			}
			ImGui::EndMenu();
		}
		if (ImGuiFileDialog::Instance()->Display("LoadSceneDlg")) 
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				//do stuff
				SceneLoader::get()->loadScene(filePathName.c_str());

			}
			// close
			ImGuiFileDialog::Instance()->Close();
		}
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
			if (ImGui::MenuItem("Pot")) {
				AppWindow::get()->getInvoker().executeCommand((int)Action::SpawnPot);
			}
			if (ImGui::MenuItem("Bunny")) {
				AppWindow::get()->getInvoker().executeCommand((int)Action::SpawnBunny);
			}
			if (ImGui::MenuItem("Armadillo")) {
				AppWindow::get()->getInvoker().executeCommand((int)Action::SpawnArmadillo);
			}
			if (ImGui::MenuItem("Lucy")) {
				AppWindow::get()->getInvoker().executeCommand((int)Action::SpawnLucy);
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

		// --- Centered Play / Pause / Step / Stop Buttons ---
		const float btnW    = 70.0f;
		const float spacing = ImGui::GetStyle().ItemSpacing.x;
		const float totalW  = btnW * 4.0f + spacing * 3.0f;
		const float barW    = ImGui::GetWindowWidth();
		ImGui::SetCursorPosX((barW - totalW) * 0.5f);

		const bool inEdit  = (m_scene_state == SceneState::Edit);
		const bool inPlay  = (m_scene_state == SceneState::Play);
		const bool inPause = (m_scene_state == SceneState::Pause);

		// 1. Play button (Green)
		if (inPlay) {
			ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.20f, 0.75f, 0.20f, 1.00f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.85f, 0.25f, 1.00f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.15f, 0.60f, 0.15f, 1.00f));
		} else {
			ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.13f, 0.55f, 0.13f, 1.00f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.72f, 0.18f, 1.00f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.10f, 0.42f, 0.10f, 1.00f));
		}
		if (ImGui::Button("  > Play", ImVec2(btnW, 0))) {
			AppWindow::get()->onPlay();
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		// 2. Pause button (Yellow / Orange)
		if (inPause) {
			ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.90f, 0.65f, 0.15f, 1.00f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.98f, 0.75f, 0.20f, 1.00f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.75f, 0.50f, 0.10f, 1.00f));
		} else {
			ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.70f, 0.50f, 0.10f, 1.00f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85f, 0.62f, 0.15f, 1.00f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.55f, 0.38f, 0.08f, 1.00f));
		}
		if (ImGui::Button(inPause ? " || Paused" : " || Pause", ImVec2(btnW, 0))) {
			if (inPause) AppWindow::get()->onPlay();
			else AppWindow::get()->onPause();
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		// 3. Step button (Blue, advances 1 frame)
		ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.15f, 0.45f, 0.75f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.20f, 0.58f, 0.90f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.10f, 0.35f, 0.60f, 1.00f));
		if (ImGui::Button(" |> Step", ImVec2(btnW, 0))) {
			AppWindow::get()->onStep();
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		// 4. Stop button (Red, disabled during Edit)
		if (inEdit) ImGui::BeginDisabled();
		ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.60f, 0.10f, 0.10f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.15f, 0.15f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.45f, 0.08f, 0.08f, 1.00f));
		if (ImGui::Button("  [] Stop", ImVec2(btnW, 0))) {
			AppWindow::get()->onStop();
		}
		ImGui::PopStyleColor(3);
		if (inEdit) ImGui::EndDisabled();

		ImGui::EndMainMenuBar();
	}

	if (ImGuiFileDialog::Instance()->Display("SaveSceneDlg"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName =
				ImGuiFileDialog::Instance()->GetFilePathName();
			SceneLoader::get()->SaveScene(filePathName);
			
		}

		ImGuiFileDialog::Instance()->Close();
	}
}
