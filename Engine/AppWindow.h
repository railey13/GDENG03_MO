#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "EngineTime.h"
#include "InputSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "CommandInvoker.h"

#include "../IMGUI/imgui.h"
#include "../IMGUI/backends/imgui_impl_dx11.h"
#include "../IMGUI/backends/imgui_impl_win32.h"

#include "vector"
#include "GameObject.h"

class SpawnObjectCommand;
class DeleteObjectCommand;
class CloseWindowCommand;

enum class Action {
	SpawnCube,
	SpawnSphere,
	SpawnPlane,
	SpawnCapsule,
	SpawnCamera,
	DeleteSelectedObject,
	ParentAction,
	Undo,
	Redo,
	CloseWindow,
};


struct PendingParent {
	GameObject* child = nullptr;
	GameObject* newParent = nullptr;
};

class AppWindow: public Window, public InputListener{
public:
	static AppWindow* get();

	static void initialize();
	static void destroy();
public:
	void createGraphicsWindow();
private:
	AppWindow(AppWindow const&);
	AppWindow& operator=(AppWindow const&) {};
	static AppWindow* sharedInstance;
public:
	AppWindow();
	~AppWindow();

	// Inherited via Window
	void onCreate() override;
	void onUpdate() override;
	void onDestroy() override;
	void onFocus() override;
	void onKillFocus() override;
	void onResize(ui32 width, ui32 height) override;
	// Inherited via InputListener
	void onKeyDown(i32 key) override;
	void onKeyUp(i32 key) override;
	// Inherited via InputListener
	void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;
private:
	void DestroyObject();
	void DestroyAllObjects();

	GameObject* SpawnGameObject(GameObjectTypes type);
	void RemoveObject(GameObject* object);
public:
	void setPendingObjectParent(PendingParent pendingParent);

	CommandInvoker& getInvoker() { return m_invoker; }
	const std::vector<GameObject*>& getGameObjects() const { return m_objects; }

	GameObject* m_selectedGameObject = nullptr;
private:
	SwapChainPtr m_swap_chain;
	RenderTexturePtr m_editor_rt;
	RenderTexturePtr m_game_rt;
	PendingParent m_pendingParent;

	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	
	std::vector<GameObject*> m_objects;
	CommandInvoker m_invoker;
private:
	void* vs_byte_code = nullptr;
	size_t vs_size = 0;
	void* ps_byte_code = nullptr;
	size_t ps_size = 0;

	int camera_flag = 0;
	bool gamecamera = false;
private:
	friend class SpawnObjectCommand;
	friend class DeleteObjectCommand;
	friend class CloseWindowCommand;
};

