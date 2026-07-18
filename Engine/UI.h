#pragma once
#include <string>
#include "../IMGUI/imgui.h"
#include "../IMGUI/backends/imgui_impl_dx11.h"
#include "../IMGUI/backends/imgui_impl_win32.h"

class UIManager;

class UI {
public:
	UI();
	virtual ~UI();
public:
	virtual void draw() = 0;
public:
	void toggleActive() { m_isActive = !m_isActive; }
	bool isActive() { return m_isActive; }
protected:
	bool m_isActive = false;
private:
	size_t m_typeId = 0;

	friend class UIManager;
};

