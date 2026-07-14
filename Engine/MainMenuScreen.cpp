#include "MainMenuScreen.h"
#include "UIManager.h"
#include "AppWindow.h"

MainMenuScreen::MainMenuScreen() : AUIScreen(UINames::MAIN_MENU_BAR) {

}

MainMenuScreen::~MainMenuScreen() {

}

void MainMenuScreen::draw() {
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
			if (ImGui::MenuItem("Camera")) {
				AppWindow::get()->getInvoker().executeCommand((int)Action::SpawnCamera);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Panels")) {
			if (ImGui::MenuItem("Scene Hierarchy")) {
				UIManager::get()->setUIScreenActive(UINames::HIERARCHY_SCREEN, !UIManager::get()->isUIScreenActive(UINames::HIERARCHY_SCREEN));
			}
			if (ImGui::MenuItem("Inspector")) {
				UIManager::get()->setUIScreenActive(UINames::INSPECTOR_SCREEN, !UIManager::get()->isUIScreenActive(UINames::INSPECTOR_SCREEN));
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools")) {
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("About")) {
			UIManager::get()->setUIScreenActive(UINames::ABOUT_SCREEN, !UIManager::get()->isUIScreenActive(UINames::ABOUT_SCREEN));
		}
		if (ImGui::MenuItem("Camera")){
			UIManager::get()->setUIScreenActive(UINames::CAMERA_SCREEN, !UIManager::get()->isUIScreenActive(UINames::CAMERA_SCREEN));
		}
		ImGui::EndMainMenuBar();
	}
}
