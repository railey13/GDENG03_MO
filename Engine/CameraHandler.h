#pragma once
#include "Camera.h"
#include "GameCamera.h"
#include "Matrix4x4.h"

class CameraHandler {
public:
	static CameraHandler* get();

	static void initialize();
	static void destroy();
private:
	CameraHandler();
	~CameraHandler();

	CameraHandler(CameraHandler const&);
	CameraHandler& operator=(CameraHandler const&) {};
	static CameraHandler* sharedInstance;
public:
	void setGameCamera(GameCamera* gameCamera);
	Camera* getSceneCamera();
	GameCamera* getGameCamera();
private:
	Camera* m_sceneCamera = nullptr;
	GameCamera* m_gameCamera = nullptr;
};

