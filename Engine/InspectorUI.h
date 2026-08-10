#pragma once
#include "UI.h"
#include "Prerequisites.h"

class InspectorUI : public UI{
public:
	InspectorUI();
	~InspectorUI();

	// Inherited via UI
	void draw() override;
private:
	char m_nameBuffer[128] = "";
	f32 m_transform_speed = 0.05f;
	
};

