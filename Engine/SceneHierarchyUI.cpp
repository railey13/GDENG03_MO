#include "SceneHierarchyUI.h"
#include "UIManager.h"
#include "AppWindow.h"

SceneHierarchyUI::SceneHierarchyUI() {
	m_isActive = true;
}

SceneHierarchyUI::~SceneHierarchyUI() {

}

void SceneHierarchyUI::draw() {
	if (!m_isActive) return;

	AppWindow* app = AppWindow::get();

	if (ImGui::Begin("Hierarchy Tree", &m_isActive, ImGuiWindowFlags_NoCollapse)) {

		auto objs = app->getGameObjects();

		// draw objects without parents first
		for (auto obj : objs) { 
			if (!obj->getParent()) {
				DrawGameObjectList(obj, app);
			}
		}

		ImGui::InvisibleButton("##HierarchyEmptySpace", ImGui::GetContentRegionAvail());

		// make the empty area a drag-drop target
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("GAMEOBJECT")) {
				GameObject* drag = *(GameObject**)payLoad->Data;

				if (drag && drag->getParent() != nullptr) {
					app->setPendingObjectParent({ drag, nullptr });
					app->getInvoker().executeCommand(static_cast<int>(Action::ParentAction));
				}
			}
			ImGui::EndDragDropTarget();
		}

		// unselect gameobject when clicked on empty area
		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) 
			&& !ImGui::IsAnyItemHovered()) {
			app->m_selectedGameObject = nullptr;
		}
	}

	ImGui::End();
	
}

void SceneHierarchyUI::DrawGameObjectList(GameObject* obj, AppWindow* app) {
	ImGui::PushID(obj);

	auto children = obj->getChildren();
	bool isSelected = (app->m_selectedGameObject == obj);

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (isSelected) flags = flags | ImGuiTreeNodeFlags_Selected;
	if (children.empty()) flags = flags | ImGuiTreeNodeFlags_Leaf; // no collapsing arrow set

	bool openChildList = ImGui::TreeNodeEx((void*)(intptr_t)obj, flags, "%s", obj->getName().c_str());

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
		app->m_selectedGameObject = obj;
	}

	if (ImGui::BeginPopupContextItem("ItemContexMenu")) {
		app->m_selectedGameObject = obj;

		if (ImGui::MenuItem("Delete")) {
			app->getInvoker().executeCommand((int)Action::DeleteSelectedObject);
		}
		ImGui::EndPopup();
	}

	// set the drag-drop source to be GameObject only
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
		ImGui::SetDragDropPayload("GAMEOBJECT", &obj, sizeof(GameObject*));
		ImGui::Text("%s", obj->getName().c_str());
		ImGui::EndDragDropSource();
	}

	// make this object a drag-drop target
	if (ImGui::BeginDragDropTarget()) { 
		if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("GAMEOBJECT")) {
			GameObject* drag = *(GameObject**)payLoad->Data;

			if (drag && drag != obj && !isDescendant(obj, drag)) {
				app->setPendingObjectParent({ drag, obj });
				app->getInvoker().executeCommand(static_cast<int>(Action::ParentAction));
			}
		}
		ImGui::EndDragDropTarget();
	}

	// repeat for further children
	if (openChildList) {
		for (auto c : children) {
			if (c) {
				DrawGameObjectList(c, app);
			}
		}
		ImGui::TreePop();
	}
	ImGui::PopID();
}

bool SceneHierarchyUI::isDescendant(GameObject* drag, GameObject* obj) {
	GameObject* current = drag;

	while (current) {
		if (current == obj) return true;
		current = current->getParent();
	}

	return false;
}
