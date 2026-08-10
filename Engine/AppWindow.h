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
#include "AGameObject.h"
#include "Cube.h"
#include "Plane.h"
#include "Sphere.h"

class SpawnObjectCommand;
class DeleteObjectCommand;
class CloseWindowCommand;

enum class Action {
	SpawnCube,
	SpawnSphere,
	SpawnPlane,
	SpawnCamera,
	DeleteSelectedObject,
	Undo,
	Redo,
	CloseWindow,
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

	AGameObject* SpawnGameObject(GAMEOBJECTS type);
	void RemoveObject(AGameObject* object);
public:
<<<<<<< Updated upstream
=======
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

>>>>>>> Stashed changes
	CommandInvoker& getInvoker() { return m_invoker; }
	const std::vector<AGameObject*>& getGameObjects() const { return m_objects; }

	AGameObject* m_selectedGameObject = nullptr;
private:
	SwapChainPtr m_swap_chain;
	RenderTexturePtr m_editor_rt;
	RenderTexturePtr m_game_rt;
<<<<<<< Updated upstream
=======
	PendingParent m_pendingParent;

	SceneState m_scene_state = SceneState::Edit;
	bool m_step_one_frame = false;
	int m_play_obj_count = 0;
>>>>>>> Stashed changes

	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	
	std::vector<AGameObject*> m_objects;
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

