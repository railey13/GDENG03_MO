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
#include "SceneState.h"
#include "SceneLoader.h"

#include "../IMGUI/imgui.h"
#include "../IMGUI/backends/imgui_impl_dx11.h"
#include "../IMGUI/backends/imgui_impl_win32.h"

#include "vector"
#include "GameObject.h"

class SpawnObjectCommand;
class DeleteObjectCommand;
class SaveTransformCommand;
class CloseWindowCommand;
class SceneLoader;


enum class Action {
	SpawnCube,
	SpawnSphere,
	SpawnPlane,
	SpawnCapsule,
	SpawnCamera,
	SpawnPot,
	SpawnArmadillo,
	SpawnBunny,
	SpawnLucy,
	DeleteSelectedObject,
	ParentAction,
	Undo,
	Redo,
	CloseWindow,
	SaveTransform,
	UndoTransform
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

	// Stress Test
	void spawnStressCubes(int count, bool withRb);
	bool isStressActive() const { return m_stress_active; }
	void setStressActive(bool active);
	float getStressRate() const { return m_stress_rate; }
	void setStressRate(float rate) { m_stress_rate = rate; }
	bool getStressWithRb() const { return m_stress_with_rb; }
	void setStressWithRb(bool withRb) { m_stress_with_rb = withRb; }
	bool getStressAutoStop() const { return m_stress_auto_stop; }
	void setStressAutoStop(bool autoStop) { m_stress_auto_stop = autoStop; }
	float getStressStopFps() const { return m_stress_stop_fps; }
	void setStressStopFps(float stopFps) { m_stress_stop_fps = stopFps; }

	float getStressFps() const { return m_fps; }
	float getStressElapsed() const { return m_stress_elapsed; }
	int getStressPeakObjs() const { return m_stress_peak_objs; }
	float getStressMinFps() const { return m_stress_min_fps; }
	float getStressLastDur() const { return m_stress_last_dur; }
	int getStressLastObjs() const { return m_stress_last_objs; }

	// Play / Pause / Step / Stop
	void onPlay();
	void onPause();
	void onStep();
	void onStop();
	bool isPaused() const { return m_scene_state == SceneState::Pause; }
	SceneState getSceneState() const { return m_scene_state; }
	CommandInvoker& getInvoker() { return m_invoker; }
	const std::vector<GameObject*>& getGameObjects() const { return m_objects; }

	GameObject* m_selectedGameObject = nullptr;
private:

	SwapChainPtr m_swap_chain;
	RenderTexturePtr m_editor_rt;
	RenderTexturePtr m_game_rt;
	PendingParent m_pendingParent;

	SceneState m_scene_state = SceneState::Edit;
	bool m_step_one_frame = false;
	int m_play_obj_count = 0;

	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	
	std::vector<GameObject*> m_objects;



	CommandInvoker m_invoker;
public:

private:
	void* vs_byte_code = nullptr;
	size_t vs_size = 0;
	void* ps_byte_code = nullptr;
	size_t ps_size = 0;

	bool gamecamera = false;

	// Stress Test State
	bool  m_stress_active    = false;
	float m_stress_rate      = 10.0f;
	bool  m_stress_with_rb   = true;
	float m_stress_timer     = 0.0f;
	float m_stress_elapsed   = 0.0f;
	bool  m_stress_auto_stop = false;
	float m_stress_stop_fps  = 30.0f;

	float m_fps              = 60.0f;
	int   m_stress_peak_objs = 0;
	float m_stress_min_fps   = 9999.0f;
	float m_stress_last_dur  = 0.0f;
	int   m_stress_last_objs = 0;

	//SceneLoader
	bool sceneLoading = false;
private:
	friend class SpawnObjectCommand;
	friend class DeleteObjectCommand;
	friend class SaveTransformCommand;
	friend class CloseWindowCommand;
	friend class SceneLoader;
	
};

