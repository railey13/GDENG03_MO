#pragma once
#include <unordered_map>
#include <memory>

#include "UI.h"
#include "DeviceContext.h"
#include "Window.h"

class ViewportUI;

class UIManager {
public:
	static UIManager* get();

	static void initialize(HWND hwnd);
	static void destroy();
private:
	UIManager(HWND hwnd);

	UIManager(UIManager const&) {};
	UIManager& operator=(UIManager const&) {};
	static UIManager* sharedInstance;

	~UIManager();
private:
	void IMGUISetUp(HWND hwnd);
private:
	template<typename T>
	void registerUI() {
		static_assert(std::is_base_of <UI, T>::value, "T must be derive from UI Class");
		auto e = getUI<T>();
		if (!e) {
			auto id = typeid(T).hash_code();
			auto c = new T();
			registerUIInternal(c, id);
		}
	}

	void registerUIInternal(UI* ui, size_t id);
	UI* getUIInternal(size_t id) const;
public:
	template <typename T>
	T* getUI() const {
		static_assert(std::is_base_of<UI, T>::value, "T must be derive from UI Class");
		auto id = typeid(T).hash_code();
		return static_cast<T*>(getUIInternal(id));
	}

	void setViewportSRVs(ID3D11ShaderResourceView* editor_srv, ID3D11ShaderResourceView* game_srv);
	ImVec2 getEditorViewportSize() const;
	ImVec2 getGameViewportSize() const;
public:
	void draw();
private:
	std::unordered_map<size_t, std::unique_ptr<UI>> m_ui_table;
	friend class UI;
};

