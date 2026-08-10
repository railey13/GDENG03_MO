#pragma once
#include "UI.h"
#include "Camera.h"

class CameraUI : public UI {
public:
	CameraUI();
	~CameraUI();

	// Inherited via UI
	void draw() override;

};
