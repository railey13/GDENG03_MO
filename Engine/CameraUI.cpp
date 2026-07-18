#include "CameraUI.h"
#include "UIManager.h"
#include "CameraHandler.h"
#include "AppWindow.h"
#include "Camera.h"

CameraUI::CameraUI() {

}

CameraUI::~CameraUI() {

}

void CameraUI::draw() {
	if (m_isActive) {
		if (ImGui::Begin("Camera Settings", &m_isActive, ImGuiWindowFlags_NoCollapse)) {
			ImGui::Text("Coordinates");
			//static float position[3] = { cam->getPosition().m_x,cam->getPosition().m_y, cam->getPosition().m_z};
			//ImGui::DragFloat3("Game View Camera", position, 0.5f);
			//Scene View
			//ImGui::DragFloat3("Scene View Camera", , 0.5f);

			
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Align Scene View with Game View").x) * 0.5f);
			if (ImGui::Button("Align Scene View with Game View")) {
				//
				Camera* viewCam = CameraHandler::get()->getSceneCamera();
				Vector3D camPos = viewCam->getPosition();
				CameraHandler::get()->getGameCamera()->getTransform()->m_position = camPos;
				CameraHandler::get()->getGameCamera()->getTransform()->m_rotation = viewCam->getRotation();
			}

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Align to View: Current Selected GameObject").x) * 0.5f);
			if (ImGui::Button("Align to View: Current Selected GameObject")) {
				//
				Camera* viewCam = CameraHandler::get()->getSceneCamera();
				Vector3D camPos = viewCam->getPosition();
				AppWindow::get()->m_selectedGameObject->getTransform()->m_position = camPos;
				AppWindow::get()->m_selectedGameObject->getTransform()->m_rotation = viewCam->getRotation();
			}

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Close").x) * 0.5f);
			if (ImGui::Button("Close")) {
				m_isActive = false;
			}
		}

		ImGui::End();

	}
}
