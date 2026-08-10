#pragma once
#include "UI.h"
#include "Prerequisites.h"

class DebugUI : public UI {
public:
	DebugUI();
	~DebugUI();

	// Inherited via UI
	void draw() override;
private:
	f32 m_fpsUpdateTimer = 0.0f;
	f32 m_displayedFps = 0.0f;
	i32 m_frameCount = 0;
};