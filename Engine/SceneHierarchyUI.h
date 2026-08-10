#pragma once
#include "UI.h"

class AppWindow;

class SceneHierarchyUI : public UI{
public:
	SceneHierarchyUI();
	~SceneHierarchyUI();

	// Inherited via UI
	void draw() override;
private:
	void DrawGameObjectList(GameObject* obj, AppWindow* app);
	bool isDescendant(GameObject* drag, GameObject* obj);
private:
	bool m_isSelected = false;
};

