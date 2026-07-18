#include "SceneHierarchyUI.h"
#include "UIManager.h"
#include "AppWindow.h"

SceneHierarchyUI::SceneHierarchyUI() {
	m_isActive = true;
}

SceneHierarchyUI::~SceneHierarchyUI() {

}

void SceneHierarchyUI::draw() {
	if (m_isActive) {
		ImGuiIO& io = ImGui::GetIO();
		float menu_h = 20.0f;
		float scr_w = io.DisplaySize.x;
		float scr_h = io.DisplaySize.y;
		float work_h = scr_h - menu_h;

		ImGui::SetNextWindowPos(ImVec2(0.0f, menu_h), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(scr_w * 0.20f, work_h * 0.70f), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Hierarchy Tree", &m_isActive, ImGuiWindowFlags_NoCollapse)) {
			const std::vector<GameObject*> objects = AppWindow::get()->getGameObjects();
			for (int i = 0; i < (int)objects.size(); i++) {
				GameObject* obj = objects[i];
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
