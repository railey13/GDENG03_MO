#pragma once
#include "UI.h"

class SceneHierarchyUI : public UI{
public:
	SceneHierarchyUI();
	~SceneHierarchyUI();

	// Inherited via UI
	void draw() override;
private:
	bool m_isSelected = false;
};

