#include "AboutUI.h"
#include "UIManager.h"

AboutUI::AboutUI() {

}

AboutUI::~AboutUI() {

}

void AboutUI::draw() {
	if (m_isActive) {
		if (ImGui::Begin("Credits", &m_isActive, ImGuiWindowFlags_NoCollapse)) {
			ImGui::Text("About");
			ImGui::Text("Scene Editor v0.1");
			ImGui::Text("Developed By: Nikos Bumanglag");
			ImGui::Text("Acknowledgements");

			static char ack_text[2048] =
				"Engine Structure and pipeline is based on PardCode's C++ 3D Game Tutorial Series:\n"
				"https://github.com/PardCode/CPP-3D-Game-Tutorial-Series\n"
				"Scene Editor UI built using Dear ImGui by Omar Cornut and contributors:\n"
				"https://github.com/ocornut/imgui\n";

			ImGui::InputTextMultiline(
				"acknowledgements",
				ack_text,
				IM_ARRAYSIZE(ack_text),
				ImVec2(-FLT_MIN, 180),
				ImGuiInputTextFlags_ReadOnly
			);

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Close").x) * 0.5f);
			if (ImGui::Button("Close")) {
				m_isActive = false;
			}
		}

		ImGui::End();

	}
}
