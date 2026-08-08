#include "UIManager.h"

#include "GraphicsEngine.h"
#include "DeviceContext.h"

#include "SceneHierarchyUI.h"
#include "InspectorUI.h"
#include "MainMenuBarUI.h"
#include "CameraUI.h"
#include "ViewportUI.h"
#include "DebugUI.h"

UIManager* UIManager::sharedInstance = NULL;

UIManager* UIManager::get() {
	return sharedInstance;
}

void UIManager::initialize(HWND hwnd) {
	if (sharedInstance != NULL) throw std::exception("UIManager already exists");

	sharedInstance = new UIManager(hwnd);
}

void UIManager::destroy() {

}

UIManager::UIManager(HWND hwnd) {
	IMGUISetUp(hwnd);
	registerUI<SceneHierarchyUI>();
	registerUI<InspectorUI>();
	registerUI<MainMenuBarUI>();
	registerUI<CameraUI>();
	registerUI<ViewportUI>();
	registerUI<DebugUI>();
}

UIManager::~UIManager() {
	m_ui_table.clear();
}

void UIManager::IMGUISetUp(HWND hwnd) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsDark();

	GraphicsEngine* graphEngine = GraphicsEngine::get();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(graphEngine->getRenderSystem()->getD11Device(),
		graphEngine->getRenderSystem()->getImmediateDeviceContext()->getContext());
}

void UIManager::registerUIInternal(UI* ui, size_t id) {
	auto compPtr = std::unique_ptr<UI>(ui);
	m_ui_table.emplace(id, std::move(compPtr));
	ui->m_typeId = id;
}

UI* UIManager::getUIInternal(size_t id) const{
	auto it = m_ui_table.find(id);

	if (it == m_ui_table.end()) return nullptr;

	return it->second.get();
}

void UIManager::setViewportSRVs(ID3D11ShaderResourceView* editor_srv, ID3D11ShaderResourceView* game_srv){
	if (ViewportUI* port = getUI<ViewportUI>()) {
		port->setViewportSRVs(editor_srv, game_srv);
	}
}

ImVec2 UIManager::getEditorViewportSize() const {
	if (ViewportUI* port = getUI<ViewportUI>()) {
		return port->getEditorViewportSize();
	}
	return ImVec2(1024, 768);
}

ImVec2 UIManager::getGameViewportSize() const {
	if (ViewportUI* port = getUI<ViewportUI>()) {
		return port->getGameViewportSize();
	}
	return ImVec2(1024, 768);
}

void UIManager::draw() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (auto& [type, ui] : m_ui_table) {
		ui->draw();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
