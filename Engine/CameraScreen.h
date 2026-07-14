#pragma once
#include "AUIScreen.h"
#include "Camera.h"

class CameraScreen : public AUIScreen {
public:
	CameraScreen(Camera* cam);
	~CameraScreen();

	// Inherited via AUIScreen
	Camera* cam;
	void draw() override;

};
