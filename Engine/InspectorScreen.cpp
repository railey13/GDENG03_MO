#include "InspectorScreen.h"
#include "UIManager.h"
#include "AppWindow.h"
#include "AGameObject.h"

InspectorScreen::InspectorScreen() : AUIScreen(UINames::INSPECTOR_SCREEN){
	m_isActive = true;
}

InspectorScreen::~InspectorScreen() {

}

void InspectorScreen::draw() {
	AGameObject* obj = AppWindow::get()->m_selectedGameObject;

	if (m_isActive) {
		ImGuiIO& io = ImGui::GetIO();
		float menu_h = 20.0f;
		float scr_w = io.DisplaySize.x;
		float scr_h = io.DisplaySize.y;
		float work_h = scr_h - menu_h;

		ImGui::SetNextWindowPos(ImVec2(scr_w * 0.80f, menu_h), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(scr_w * 0.20f, work_h), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Inspector", &m_isActive, ImGuiWindowFlags_NoCollapse)) {
			if (obj) {
				// GameObject Name
				{
					strncpy_s(m_nameBuffer, obj->m_name.c_str(), sizeof(m_nameBuffer) - 1);
					m_nameBuffer[sizeof(m_nameBuffer) - 1] = '\0';

					if (ImGui::InputText("Name", m_nameBuffer, sizeof(m_nameBuffer))) {
						if (m_nameBuffer[0] == '\0') {
							obj->m_name = "GameObject";
						}
						else {
							obj->m_name = m_nameBuffer;
						}
					}
				}
				// GameObject Transform
				{
					Vector3D pos = obj->m_position;
					Vector3D rot = obj->m_rotation;
					Vector3D scale = obj->m_scale;

					ImGui::Text("Transform");
					if (ImGui::DragFloat3("Position", &pos.m_x, m_transform_speed)) {
						obj->m_position = pos;
					}
					if (ImGui::DragFloat3("Rotation", &rot.m_x, m_transform_speed)) {
						obj->m_rotation = rot;
					}
					if (ImGui::DragFloat3("Scale", &scale.m_x, m_transform_speed)) {
						obj->m_scale = scale;
					}

				}

			}		
		}

		ImGui::End();
	}
}
