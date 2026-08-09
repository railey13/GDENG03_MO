#include "MainMenuBarUI.h"
#include "UIManager.h"
#include "AppWindow.h"

#include "SceneHierarchyUI.h"
#include "InspectorUI.h"
#include "CameraUI.h"

MainMenuBarUI::MainMenuBarUI() {

}

MainMenuBarUI::~MainMenuBarUI() {

}

void MainMenuBarUI::draw() {
	UIManager* ui = UIManager::get();

	if (ImGui::BeginMainMenuBar()) {
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
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Camera")){
			if (CameraUI* camera = ui->getUI<CameraUI>()) {
				camera->toggleActive();
			}

		}
		ImGui::EndMainMenuBar();
	}
}
