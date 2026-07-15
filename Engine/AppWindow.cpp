#include "AppWindow.h"
#include "Vector3D.h"
#include "iostream"
#include "../Settings.h"

#include "UIManager.h"
#include "SpawnObjectCommand.h"
#include "DeleteObjectCommand.h"
#include "CloseWindowCommand.h"

#include "Camera.h"
#include "GameCamera.h"
#include "CameraHandler.h"

AppWindow* AppWindow::sharedInstance = NULL;

AppWindow* AppWindow::get() {
	return sharedInstance;
}

void AppWindow::initialize() {
	if (sharedInstance != NULL) throw std::exception("App Window already exists");

	sharedInstance = new AppWindow();
}

void AppWindow::destroy() {
	if (sharedInstance == NULL) return;

	delete sharedInstance;
}

void AppWindow::createGraphicsWindow() {
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, Settings::WindowWidth, Settings::WindowHeight);

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"Engine/VertexShader.hlsl", "vsmain", &vs_byte_code, &vs_size);
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(vs_byte_code, vs_size);

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"Engine/PixelShader.hlsl", "psmain", &ps_byte_code, &ps_size);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(ps_byte_code, ps_size);

	CameraHandler::initialize();

	UIManager::initialize(m_hwnd, CameraHandler::get()->getSceneCamera());
}

AppWindow::AppWindow() {
	
}

AppWindow::~AppWindow() {
	sharedInstance = nullptr;
}

void AppWindow::onCreate() {
	/*Window::onCreate();*/
	InputSystem::get()->addListener(this);

	m_invoker.bindCommand((int)Action::SpawnCube, [this]() { return new SpawnObjectCommand(this, GAMEOBJECTS::CUBE); });
	m_invoker.bindCommand((int)Action::SpawnSphere, [this]() { return new SpawnObjectCommand(this, GAMEOBJECTS::SPHERE); });
	m_invoker.bindCommand((int)Action::SpawnPlane, [this]() { return new SpawnObjectCommand(this, GAMEOBJECTS::PLANE); });
	m_invoker.bindCommand((int)Action::SpawnCamera , [this]() { return new SpawnObjectCommand(this, GAMEOBJECTS::GAME_CAMERA); });

	m_invoker.bindCommand((int)Action::DeleteSelectedObject, [this]() {
		return new DeleteObjectCommand(this, m_selectedGameObject);
	});

	m_invoker.bindCommand((int)Action::CloseWindow, [this]() { return new CloseWindowCommand(this); });
}

void AppWindow::onUpdate() {
	/*Window::onUpdate();*/
	//screen
	InputSystem::get()->update();
	GraphicsEngine* graphEngine = GraphicsEngine::get();

	f32 deltaTime = EngineTime::getDeltaTime();

	Camera* sceneCamera = CameraHandler::get()->getSceneCamera();
	GameCamera* gameCamera = CameraHandler::get()->getGameCamera();

	sceneCamera->update(deltaTime);

	graphEngine->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	graphEngine->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);
	
	graphEngine->getRenderSystem()->getImmediateDeviceContext()->ClearRenderTargetColor(this->m_swap_chain, 0.55f, 0.68f, 0.76f, 1);

	graphEngine->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(m_window_width, m_window_height);

	for (auto obj : m_objects) {
		obj->update(deltaTime);
		if (camera_flag == 0) {
			obj->draw(m_vs, m_ps, sceneCamera->getViewMatrix(), sceneCamera->getProjectionMatrix());
		}
		if (camera_flag == 1) {
			obj->draw(m_vs, m_ps, gameCamera->getViewMatrix(), gameCamera->getProjectionMatrix());
		}
	}

	UIManager::get()->drawAllUI();

	m_swap_chain->present(false);
}

void AppWindow::onDestroy() {
	Window::onDestroy();

	InputSystem::get()->removeListener(this);
	m_objects.clear();

	GraphicsEngine::get()->destroy();
}

void AppWindow::onFocus() {
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus() {	
	InputSystem::get()->removeListener(this);
}

void AppWindow::onResize(ui32 width, ui32 height) {
	if (width == 0 || height == 0) return;

	m_window_width = width;
	m_window_height = height;

	if (m_swap_chain) {
		m_swap_chain->resize(m_window_width, m_window_height);
	}

	CameraHandler::get()->getSceneCamera()->setAspect((f32)width, (f32)height);
}

void AppWindow::onKeyDown(i32 key) {

}

void AppWindow::onKeyUp(i32 key) {
	// return if any input field is highlighted
	if (ImGui::GetIO().WantCaptureKeyboard) return;
	// temporary inputs to test textures
	switch (key) {
		case '0': 
			m_selectedGameObject->setTexture(GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/white.png"));
			break;
		case '1':
			m_selectedGameObject->setTexture(GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/CartethyiaPuppet.gif"));
			break;
		case 90: m_invoker.undo();
			break;
		case 89: m_invoker.redo();
			break;
		case VK_DELETE: 
			if (m_selectedGameObject) {
				m_invoker.executeCommand((int)Action::DeleteSelectedObject);
			}
			break;
		case VK_LEFT: camera_flag = 0;
			break;
		case VK_RIGHT: if(CameraHandler::get()->getGameCamera()) camera_flag = 1;
			break;
		default: break;
	}
}

void AppWindow::onMouseMove(const Point& mouse_pos) {

}

void AppWindow::onLeftMouseDown(const Point& mouse_pos) {

}

void AppWindow::onLeftMouseUp(const Point& mouse_pos) {

}

void AppWindow::onRightMouseDown(const Point& mouse_pos) {
	ImGui::SetWindowFocus(nullptr);
}

void AppWindow::onRightMouseUp(const Point& mouse_pos) {

}

void AppWindow::DestroyObject() {
	if (m_objects.empty()) return;
	if (m_objects.back() == CameraHandler::get()->getGameCamera()) {
		camera_flag = 0;
		gamecamera = false;
	}
	delete m_objects.back();
	m_objects.pop_back();
}

void AppWindow::DestroyAllObjects() {
	if (m_objects.empty()) return;
	while (!m_objects.empty()) {
		delete m_objects.back();
		m_objects.pop_back();
	}
}

AGameObject* AppWindow::SpawnGameObject(GAMEOBJECTS type) {
	AGameObject* obj = nullptr;
	switch (type) {
		case GAMEOBJECTS::CUBE:
			obj = new Cube(vs_byte_code, vs_size);
			break;
		case GAMEOBJECTS::SPHERE:
			obj = new Sphere(vs_byte_code, vs_size);
			break;
		case GAMEOBJECTS::PLANE:
			obj = new Plane(vs_byte_code, vs_size);
			break;
		case GAMEOBJECTS::GAME_CAMERA:
			if (gamecamera) return nullptr;
			obj = new GameCamera(vs_byte_code, vs_size);
			obj->setScale(Vector3D(0.4f, 0.5f, 1));
			gamecamera = true;
			break;
		default: break;
	}

	f32 spawnDistance = 1.0f;

	Vector3D spawnPos = CameraHandler::get()->getSceneCamera()->getPosition() + CameraHandler::get()->getSceneCamera()->getForwardDirection() * spawnDistance;

	obj->setPosition(spawnPos);

	m_objects.push_back(obj);
	return obj;
}

void AppWindow::RemoveObject(AGameObject* object) {
	auto it = std::find(m_objects.begin(), m_objects.end(), object);

	if (it != m_objects.end()) {
		m_objects.erase(it);
	}

	if (object == CameraHandler::get()->getGameCamera()) {
		camera_flag = 0;
		gamecamera = false;
	}
}



