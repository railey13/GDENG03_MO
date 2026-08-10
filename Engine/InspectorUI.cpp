#include "InspectorUI.h"
#include "UIManager.h"
#include "AppWindow.h"
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "TextureComponent.h"
#include "PhysicsSystem.h"
#include "../IMGUI/ImGuiFileDialog.h"
#include "Texture.h"

InspectorUI::InspectorUI() {
	m_isActive = true;
}

InspectorUI::~InspectorUI() {

}

void InspectorUI::draw() {
	GameObject* obj = AppWindow::get()->m_selectedGameObject;

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
					strncpy_s(m_nameBuffer, obj->getName().c_str(), sizeof(m_nameBuffer) - 1);
					m_nameBuffer[sizeof(m_nameBuffer) - 1] = '\0';

					if (ImGui::InputText("Name", m_nameBuffer, sizeof(m_nameBuffer))) {
						if (m_nameBuffer[0] == '\0') {
							obj->setName("GameObject");
						}
						else {
							obj->setName(m_nameBuffer);
						}
					}
				}
				// GameObject Transform
				{
					Vector3D pos = obj->getTransform()->getPosition();
					Vector3D rot = obj->getTransform()->getRotation();
					Vector3D scale = obj->getTransform()->getScale();

					ImGui::Text("Transform");
					if (ImGui::DragFloat3("Position", &pos.m_x, m_transform_speed)) {
						obj->getTransform()->setPosition(pos);
					}
					if (ImGui::DragFloat3("Rotation", &rot.m_x, m_transform_speed)) {
						obj->getTransform()->setRotation(rot);
					}
					if (ImGui::DragFloat3("Scale", &scale.m_x, m_transform_speed)) {
						obj->getTransform()->setScale(scale);
					}

					if (ImGui::Button("Confirm Changes")) {
						//Call command stack here
						AppWindow::get()->getInvoker().executeCommand((int)Action::SaveTransform);
						

						Debug::Log("TRANSFORM FOR " + obj->getName() +  " SAVED");

					}
				}

				// Physics Component
				{
					PhysicsComponent* rb = obj->getComponent<PhysicsComponent>();

					ImGui::Separator();
					if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen)) {
						if (!rb) {
							// No physics — offer to add one
							ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.13f, 0.45f, 0.65f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.60f, 0.85f, 1.0f));
							if (ImGui::Button("+ Add Box Collider", ImVec2(-1, 0))) {
								PhysicsComponent* newRb = PhysicsSystem::get()->createComponent(
									obj, PhysicsComponent::BodyType::DYNAMIC);
								newRb->addBoxColliderFromScale();
								newRb->enableGravity(true);
							}
							ImGui::PopStyleColor(2);
						}
						else {
							// Body type dropdown
							const char* bodyTypes[] = { "Static", "Kinematic", "Dynamic" };
							int currentType = static_cast<int>(rb->getBodyType());
							if (ImGui::Combo("Body Type", &currentType, bodyTypes, 3)) {
								rb->setBodyType(static_cast<PhysicsComponent::BodyType>(currentType));
							}

							// Live velocity readouts (read-only)
							Vector3D linVel = rb->getLinearVelocity();
							Vector3D angVel = rb->getAngularVelocity();
							ImGui::Text("Linear Vel:  (%.2f, %.2f, %.2f)", linVel.m_x, linVel.m_y, linVel.m_z);
							ImGui::Text("Angular Vel: (%.2f, %.2f, %.2f)", angVel.m_x, angVel.m_y, angVel.m_z);

							ImGui::Spacing();
							// Remove physics
							ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.55f, 0.10f, 0.10f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.75f, 0.15f, 0.15f, 1.0f));
							if (ImGui::Button("Remove Physics", ImVec2(-1, 0))) {
								rb->release(); // removes from component map, destructor untracks
							}
							ImGui::PopStyleColor(2);
						}
					}
				}

				// Texture Component
				{
					TextureComponent* tex = obj->getComponent<TextureComponent>();
					if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen)) {
						if (!tex) {
							if (ImGui::Button("+ Add Texture Component"))
								obj->createComponent<TextureComponent>();
						}
						else {
							ImGui::Text("Current Texture: ");
							ImGui::SameLine();
							ImGui::Image(tex->getTexture()->getSRV(), ImVec2(32, 32));

							if (ImGui::Button("Change Texture")) {
								IGFD::FileDialogConfig config;
								config.path = "Assets/Textures";

								ImGuiFileDialog::Instance()->OpenDialog(
									"ChooseFileDlgKey",
									"Choose Texture",
									".png,.jpg,.gif",
									config
								);
							}

							if (ImGui::Button("Remove Texture Component")) {
								obj->deleteComponent<TextureComponent>();
							}
						}
					}

				}
			}		
		}
		ImGui::End();

		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) // => will show a dialog
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				//std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
				//std::string filter = ImGuiFileDialog::Instance()->GetCurrentFilter();
				// here convert from string because a string was passed as a userDatas, but it can be what you want
				//std::string userDatas;
				//if (ImGuiFileDialog::Instance()->GetUserDatas())
				//	userDatas = std::string((const char*)ImGuiFileDialog::Instance()->GetUserDatas());
				//auto selection = ImGuiFileDialog::Instance()->GetSelection(); // multiselection

				// action

				TextureComponent* tex = obj->getComponent<TextureComponent>();
				if (tex && !filePathName.empty()) {
					tex->setTexturePath(filePathName);
				}

			}
			// close
			ImGuiFileDialog::Instance()->Close();
		}
	}
}
