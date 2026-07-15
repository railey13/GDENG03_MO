#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "IMGUI/imgui.h"
#include "Prerequisites.h"

class UIManager
{
public:
	UIManager(HWND hwnd, RenderSystem* system);
	~UIManager();

	void update(ID3D11ShaderResourceView* editor_srv = nullptr, ID3D11ShaderResourceView* game_srv = nullptr);
	void render();

	ImVec2 getEditorViewportSize() const { return m_editor_size; }
	ImVec2 getGameViewportSize() const { return m_game_size; }

private:
	void initLogoTexture();

private:
	RenderSystem* m_system = nullptr;
	HWND m_hwnd = nullptr;

	bool m_show_credits = true;
	bool m_show_inspector = true;
	bool m_show_hierarchy = true;
	bool m_show_color_picker = false;
	bool m_show_editor_view = true;
	bool m_show_game_view = true;
	bool m_show_library = true;

	ID3D11ShaderResourceView* m_logo_srv = nullptr;
	int m_logo_w = 128;
	int m_logo_h = 128;

	ImVec2 m_editor_size = ImVec2(1024, 768);
	ImVec2 m_game_size   = ImVec2(1024, 768);
};

