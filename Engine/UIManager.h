#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "Window.h"
#include "DeviceContext.h"
#include "AUIScreen.h"
#include "Camera.h"

#include "../IMGUI/imgui.h"
#include "../IMGUI/backends/imgui_impl_dx11.h"
#include "../IMGUI/backends/imgui_impl_win32.h"

namespace UINames {
	const std::string MAIN_MENU_BAR = "MAIN_MENU_BAR";
	const std::string HIERARCHY_SCREEN = "HIERARCHY_SCREEN";
	const std::string INSPECTOR_SCREEN = "INSPECTOR_SCREEN";
	const std::string ABOUT_SCREEN = "ABOUT_SCREEN";
	const std::string CAMERA_SCREEN = "CAMERA_SCREEN";
	const std::string VIEWPORT_SCREEN = "VIEWPORT_SCREEN";
}

class ViewportScreen;

class UIManager {
public:
	static UIManager* get();

	static void initialize(HWND hwnd, Camera* cam);
	static void destroy();

	void drawAllUI();
	
	AUIScreen* getUIScreen(std::string name);

	void setUIScreenActive(std::string name, bool flag);

	bool isUIScreenActive(std::string name);

	void setViewportSRVs(ID3D11ShaderResourceView* editor_srv, ID3D11ShaderResourceView* game_srv);
	ImVec2 getEditorViewportSize() const;
	ImVec2 getGameViewportSize() const;
private:
	UIManager(HWND hwnd, Camera* cam);

	UIManager(UIManager const&) {};
	UIManager& operator=(UIManager const&) {};
	static UIManager* sharedInstance;

	~UIManager();
private:
	Camera* cam;
	std::vector<AUIScreen*> m_ui_list;
	std::unordered_map<std::string, AUIScreen*> m_ui_table;
	ViewportScreen* m_viewport_screen = nullptr;
};

