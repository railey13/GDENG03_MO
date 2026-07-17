#pragma once
#include "AUIScreen.h"
#include <d3d11.h>
#include "../IMGUI/imgui.h"

class ViewportScreen : public AUIScreen {
public:
	ViewportScreen();
	~ViewportScreen();

	void draw() override;

	void setViewportSRVs(ID3D11ShaderResourceView* editor_srv, ID3D11ShaderResourceView* game_srv);

	ImVec2 getEditorViewportSize() const;
	ImVec2 getGameViewportSize() const;

private:
	ID3D11ShaderResourceView* m_editor_srv = nullptr;
	ID3D11ShaderResourceView* m_game_srv = nullptr;

	ImVec2 m_editor_size = ImVec2(1024, 768);
	ImVec2 m_game_size   = ImVec2(1024, 768);
	bool m_show_editor_view = true;
	bool m_show_game_view = true;
};
