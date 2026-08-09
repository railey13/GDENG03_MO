#pragma once
#include "UI.h"
#include "SceneState.h"

class MainMenuBarUI : public UI{
public:
	MainMenuBarUI();
	~MainMenuBarUI();
	
	// Inherited via UI
	void draw() override;
	void setSceneState(SceneState state) { m_scene_state = state; }
private:
	SceneState m_scene_state = SceneState::Edit;
	friend class SpawnObjectCommand;
};

