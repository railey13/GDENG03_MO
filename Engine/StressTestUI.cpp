#include "StressTestUI.h"
#include "AppWindow.h"
#include "UIManager.h"
#include <cmath>

StressTestUI::StressTestUI() {
	m_isActive = false;
}

StressTestUI::~StressTestUI() {

}

void StressTestUI::draw() {
	if (!m_isActive) return;

	AppWindow* app = AppWindow::get();
	if (!app) return;

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.35f, io.DisplaySize.y * 0.20f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(320.0f, 0.0f), ImGuiCond_FirstUseEver);

	bool running = app->isStressActive();

	if (running) {
		float pulse = 0.55f + 0.15f * sinf((float)ImGui::GetTime() * 4.0f);
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(pulse, 0.08f, 0.08f, 1.0f));
	} else {
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.22f, 0.22f, 0.22f, 1.0f));
	}

	if (ImGui::Begin("Stress Test", &m_isActive)) {
		// Status row
		if (running)
			ImGui::TextColored(ImVec4(1.0f, 0.30f, 0.30f, 1.0f), "  RUNNING");
		else
			ImGui::TextColored(ImVec4(0.55f, 0.75f, 0.55f, 1.0f), "  IDLE");

		ImGui::SameLine(0.0f, 12.0f);

		// Start/Stop Toggle Button
		if (!running) {
			ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.15f, 0.42f, 0.18f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.22f, 0.58f, 0.26f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.10f, 0.32f, 0.13f, 1.0f));
			if (ImGui::Button("  Start Stress Test ", ImVec2(-1, 0))) {
				app->setStressActive(true);
			}
			ImGui::PopStyleColor(3);
		} else {
			ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.55f, 0.10f, 0.10f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.75f, 0.18f, 0.18f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.42f, 0.07f, 0.07f, 1.0f));
			if (ImGui::Button("  Stop Stress Test  ", ImVec2(-1, 0))) {
				app->setStressActive(false);
			}
			ImGui::PopStyleColor(3);
		}

		ImGui::Spacing();
		// Batch spawn button
		ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.20f, 0.35f, 0.55f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.28f, 0.48f, 0.72f, 1.0f));
		if (ImGui::Button("Spawn 20 Cubes [Stack Test]", ImVec2(-1, 0))) {
			app->spawnStressCubes(20, app->getStressWithRb());
		}
		ImGui::PopStyleColor(2);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Configuration
		ImGui::Text("Configuration");
		float rate = app->getStressRate();
		ImGui::Text("Spawn Rate:");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(-1);
		if (ImGui::SliderFloat("##rate", &rate, 1.0f, 60.0f, "%.1f obj/s")) {
			app->setStressRate(rate);
		}

		bool withRb = app->getStressWithRb();
		if (ImGui::Checkbox("Spawn with Rigidbody", &withRb)) {
			app->setStressWithRb(withRb);
		}

		bool autoStop = app->getStressAutoStop();
		if (ImGui::Checkbox("Auto-stop below FPS:", &autoStop)) {
			app->setStressAutoStop(autoStop);
		}
		ImGui::SameLine();
		float stopFps = app->getStressStopFps();
		ImGui::SetNextItemWidth(-1);
		ImGui::BeginDisabled(!autoStop);
		if (ImGui::SliderFloat("##stopfps", &stopFps, 5.0f, 60.0f, "%.0f FPS")) {
			app->setStressStopFps(stopFps);
		}
		ImGui::EndDisabled();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Telemetry
		ImGui::Text("Live Telemetry");
		float fps = app->getStressFps();
		ImVec4 fps_color = (fps >= 45.0f) ? ImVec4(0.30f, 0.90f, 0.35f, 1.0f)
		                 : (fps >= 30.0f) ? ImVec4(0.95f, 0.80f, 0.10f, 1.0f)
		                 :                  ImVec4(1.00f, 0.25f, 0.25f, 1.0f);

		ImGui::Text("FPS:");
		ImGui::SameLine(90.0f);
		ImGui::TextColored(fps_color, "%.1f", fps);

		ImGui::Text("Objects:");
		ImGui::SameLine(90.0f);
		ImGui::Text("%d", (int)app->getGameObjects().size());

		if (running) {
			ImGui::Text("Elapsed:");
			ImGui::SameLine(90.0f);
			ImGui::Text("%.1f s", app->getStressElapsed());
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Last Run
		ImGui::Text("Last Run Results");
		if (app->getStressLastDur() > 0.0f) {
			ImGui::Text("Duration:   %.1f s", app->getStressLastDur());
			ImGui::Text("Peak Objs:  %d", app->getStressLastObjs());
			ImGui::Text("Lowest FPS: %.1f", app->getStressMinFps());
		} else {
			ImGui::TextDisabled("  (no run recorded yet)");
		}
	}
	ImGui::End();
	ImGui::PopStyleColor();
}
