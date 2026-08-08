#include "DebugUI.h"
#include "UIManager.h"

DebugUI::DebugUI() {
	m_isActive = true;
	Debug::Log("DebugUI Created");
}

DebugUI::~DebugUI() {

}

void DebugUI::draw() {
	if (!m_isActive) return;

	if (ImGui::Begin("Debug Window",&m_isActive)) {
		if (ImGui::Button("Clear Logs")) {
			Debug::clearLogs();
		}
		ImGui::SameLine();
		ImGui::Text("FPS: %.1f", m_displayedFps);

		if (ImGui::BeginChild("Logs", ImVec2(0,0), true, ImGuiWindowFlags_HorizontalScrollbar)) {
			for (const auto& log : Debug::getLogs()) {
				ImVec4 color;
				if (log.level == LogLevel::Warning) color = ImVec4(1,0.8,0.2,1); 
				if (log.level == LogLevel::Error) color = ImVec4(1,0.3,0.3,1); 
				if (log.level == LogLevel::Info) color = ImVec4(1,1,1,1); 

				ImGui::PushStyleColor(ImGuiCol_Text, color);
				ImGui::TextUnformatted(log.msg.c_str());
				ImGui::PopStyleColor();
			}
		}
		
		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
			ImGui::SetScrollHereY(1);
		}

		ImGui::EndChild();
	}

	ImGui::End();
}
