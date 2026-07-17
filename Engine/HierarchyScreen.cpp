#include "HierarchyScreen.h"
#include "UIManager.h"
#include "AppWindow.h"

HierarchyScreen::HierarchyScreen() : AUIScreen(UINames::HIERARCHY_SCREEN) {
	m_isActive = true;
}

HierarchyScreen::~HierarchyScreen() {

}

void HierarchyScreen::draw() {
	if (m_isActive) {
		ImGuiIO& io = ImGui::GetIO();
		float menu_h = 20.0f;
		float scr_w = io.DisplaySize.x;
		float scr_h = io.DisplaySize.y;
		float work_h = scr_h - menu_h;

		ImGui::SetNextWindowPos(ImVec2(0.0f, menu_h), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(scr_w * 0.20f, work_h * 0.70f), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Hierarchy Tree", &m_isActive, ImGuiWindowFlags_NoCollapse)) {
			const std::vector<AGameObject*> objects = AppWindow::get()->getGameObjects();
			for (int i = 0; i < (int)objects.size(); i++) {
				AGameObject* obj = objects[i];
				ImGui::PushID(i); 

				bool isSelected = (AppWindow::get()->m_selectedGameObject == obj);
				if (ImGui::Selectable(obj->m_name.c_str(), isSelected)) {
					AppWindow::get()->m_selectedGameObject = obj;
				}

				ImGui::PopID();
			}
		}

		ImGui::End();
	}
}
