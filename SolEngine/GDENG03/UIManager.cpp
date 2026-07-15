#include "UIManager.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_win32.h"
#include "IMGUI/imgui_impl_dx11.h"
#include <vector>

UIManager::UIManager(HWND hwnd, RenderSystem* system)
	: m_hwnd(hwnd)
	, m_system(system)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX11_Init(m_system->m_d3d_device, m_system->getImmediateDeviceContext()->m_device_context);

	initLogoTexture();
}

UIManager::~UIManager()
{
	if (m_logo_srv)
	{
		m_logo_srv->Release();
		m_logo_srv = nullptr;
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void UIManager::initLogoTexture()
{
	ID3D11Device* device = m_system->m_d3d_device;
	if (!device) return;

	// Try loading from file first if dlsu_logo.bmp or logo.bmp exists
	const char* bmp_paths[] = { "dlsu_logo.bmp", "logo.bmp" };
	for (const char* path : bmp_paths)
	{
		HBITMAP hBitmap = (HBITMAP)LoadImageA(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		if (hBitmap)
		{
			BITMAP bm;
			GetObject(hBitmap, sizeof(BITMAP), &bm);
			m_logo_w = bm.bmWidth;
			m_logo_h = bm.bmHeight;

			HDC hdc = GetDC(NULL);
			BITMAPINFO bmi = {};
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = bm.bmWidth;
			bmi.bmiHeader.biHeight = -bm.bmHeight; // top-down
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;
			bmi.bmiHeader.biCompression = BI_RGB;

			std::vector<unsigned char> pixels(bm.bmWidth * bm.bmHeight * 4);
			GetDIBits(hdc, hBitmap, 0, bm.bmHeight, pixels.data(), &bmi, DIB_RGB_COLORS);
			ReleaseDC(NULL, hdc);
			DeleteObject(hBitmap);

			// Convert BGRA to RGBA
			for (size_t i = 0; i < pixels.size(); i += 4)
			{
				unsigned char b = pixels[i];
				unsigned char r = pixels[i + 2];
				pixels[i] = r;
				pixels[i + 2] = b;
				pixels[i + 3] = 255;
			}

			D3D11_TEXTURE2D_DESC desc = {};
			desc.Width = bm.bmWidth;
			desc.Height = bm.bmHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

			D3D11_SUBRESOURCE_DATA initData = {};
			initData.pSysMem = pixels.data();
			initData.SysMemPitch = bm.bmWidth * 4;

			ID3D11Texture2D* texture = nullptr;
			if (SUCCEEDED(device->CreateTexture2D(&desc, &initData, &texture)))
			{
				device->CreateShaderResourceView(texture, nullptr, &m_logo_srv);
				texture->Release();
				return;
			}
		}
	}

	// If no BMP file found, generate a procedural DLSU stylized green/white/gold logo texture in memory
	m_logo_w = 128;
	m_logo_h = 128;
	std::vector<unsigned char> pixels(m_logo_w * m_logo_h * 4);
	for (int y = 0; y < m_logo_h; y++)
	{
		for (int x = 0; x < m_logo_w; x++)
		{
			int i = (y * m_logo_w + x) * 4;
			bool is_border = (x < 6 || x >= m_logo_w - 6 || y < 6 || y >= m_logo_h - 6);
			bool is_star = ((x - 64) * (x - 64) + (y - 64) * (y - 64) < 900);
			bool is_inner_ring = ((x - 64) * (x - 64) + (y - 64) * (y - 64) >= 750 && (x - 64) * (x - 64) + (y - 64) * (y - 64) < 1100);

			if (is_border || is_inner_ring)
			{
				pixels[i]     = 10;  // R (DLSU Green)
				pixels[i + 1] = 110; // G
				pixels[i + 2] = 50;  // B
				pixels[i + 3] = 255;
			}
			else if (is_star)
			{
				pixels[i]     = 240; // R (White/Gold center)
				pixels[i + 1] = 240; // G
				pixels[i + 2] = 230; // B
				pixels[i + 3] = 255;
			}
			else
			{
				pixels[i]     = 15;
				pixels[i + 1] = (unsigned char)(60 + (y * 40 / m_logo_h));
				pixels[i + 2] = 30;
				pixels[i + 3] = 255;
			}
		}
	}

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = m_logo_w;
	desc.Height = m_logo_h;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = pixels.data();
	initData.SysMemPitch = m_logo_w * 4;

	ID3D11Texture2D* texture = nullptr;
	if (SUCCEEDED(device->CreateTexture2D(&desc, &initData, &texture)))
	{
		device->CreateShaderResourceView(texture, nullptr, &m_logo_srv);
		texture->Release();
	}
}

void UIManager::update(ID3D11ShaderResourceView* editor_srv, ID3D11ShaderResourceView* game_srv)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// ---- Main Menu Bar ----
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				::PostMessage(m_hwnd, WM_CLOSE, 0, 0);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Objects"))
		{
			if (ImGui::MenuItem("Spawn Cube (Placeholder)")) {}
			if (ImGui::MenuItem("Spawn Sphere (Placeholder)")) {}
			if (ImGui::MenuItem("Spawn Plane (Placeholder)")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Particles"))
		{
			if (ImGui::MenuItem("Spawn Particle Emitter (Placeholder)")) {}
			if (ImGui::MenuItem("Particle Settings (Placeholder)")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::MenuItem("Editor Viewport", nullptr, &m_show_editor_view);
			ImGui::MenuItem("Game Viewport", nullptr, &m_show_game_view);
			ImGui::MenuItem("Credits", nullptr, &m_show_credits);
			ImGui::MenuItem("Color Picker (Placeholder)", nullptr, &m_show_color_picker);
			ImGui::MenuItem("Inspector", nullptr, &m_show_inspector);
			ImGui::MenuItem("Scene Hierarchy", nullptr, &m_show_hierarchy);
			ImGui::MenuItem("Library", nullptr, &m_show_library);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About"))
		{
			ImGui::MenuItem("Credits", nullptr, &m_show_credits);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGuiIO& io = ImGui::GetIO();
	float menu_h = 20.0f;
	float scr_w = io.DisplaySize.x;
	float scr_h = io.DisplaySize.y;
	float work_h = scr_h - menu_h;

	// ---- Scene Hierarchy Window (Unity Left Column) ----
	if (m_show_hierarchy)
	{
		ImGui::SetNextWindowPos(ImVec2(0, menu_h), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(scr_w * 0.20f, work_h * 0.70f), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Scene Hierarchy", &m_show_hierarchy))
		{
			ImGui::Text("SampleScene");
			ImGui::Separator();
			ImGui::Text("  [+] Directional Light");
			ImGui::Text("  [+] Ground/Plane");
			ImGui::Text("  [+] Cube (Right)");
			ImGui::Text("  [+] Player / Main Camera");
		}
		ImGui::End();
	}

	// ---- Editor Viewport Window (Unity Center-Left Area) ----
	if (m_show_editor_view)
	{
		ImGui::SetNextWindowPos(ImVec2(scr_w * 0.20f, menu_h), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(scr_w * 0.30f, work_h * 0.70f), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Editor Viewport", &m_show_editor_view, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
		{
			ImVec2 size = ImGui::GetContentRegionAvail();
			if (size.x < 64) size.x = 64;
			if (size.y < 64) size.y = 64;
			m_editor_size = size;

			if (editor_srv)
				ImGui::Image((void*)editor_srv, size);
			else
				ImGui::Text("Editor Viewport Texture Not Bound");
		}
		ImGui::End();
	}

	// ---- Game Viewport Window (Unity Center-Right Area) ----
	if (m_show_game_view)
	{
		ImGui::SetNextWindowPos(ImVec2(scr_w * 0.50f, menu_h), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(scr_w * 0.30f, work_h * 0.70f), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Game Viewport", &m_show_game_view, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
		{
			ImVec2 size = ImGui::GetContentRegionAvail();
			if (size.x < 64) size.x = 64;
			if (size.y < 64) size.y = 64;
			m_game_size = size;

			if (game_srv)
				ImGui::Image((void*)game_srv, size);
			else
				ImGui::Text("Game Viewport Texture Not Bound");
		}
		ImGui::End();
	}

	// ---- Inspector Window (Unity Far-Right Column) ----
	if (m_show_inspector)
	{
		ImGui::SetNextWindowPos(ImVec2(scr_w * 0.80f, menu_h), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(scr_w * 0.20f, work_h), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Inspector", &m_show_inspector))
		{
			ImGui::Text("GameObject: Cube (Right)");
			ImGui::Separator();
			ImGui::Text("Transform");
			ImGui::Text("  Position: (0.0, 0.5, 5.0)");
			ImGui::Text("  Rotation: (0.0, 0.0, 0.0)");
			ImGui::Text("  Scale:    (1.0, 1.0, 1.0)");
			ImGui::Separator();
			ImGui::Text("Mesh Renderer");
			ImGui::Text("  Material: Solid Blue");
		}
		ImGui::End();
	}

	// ---- Library / Project Window (Unity Bottom Area) ----
	if (m_show_library)
	{
		ImGui::SetNextWindowPos(ImVec2(0, menu_h + work_h * 0.70f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(scr_w * 0.80f, work_h * 0.30f), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Project / Library", &m_show_library))
		{
			ImGui::Columns(2, "LibraryColumns", true);
			ImGui::SetColumnWidth(0, scr_w * 0.18f);

			// Left pane: Folder tree
			ImGui::Text("Folders");
			ImGui::Separator();
			ImGui::Text("[-] Assets");
			ImGui::Text("    [+] Materials");
			ImGui::Text("    [+] Models");
			ImGui::Text("    [+] Scenes");
			ImGui::Text("    [+] Shaders");
			ImGui::Text("    [+] Textures");

			ImGui::NextColumn();

			// Right pane: Asset items
			ImGui::Text("Assets /");
			ImGui::Separator();
			ImGui::BulletText("SampleScene.unity");
			ImGui::BulletText("CubeMesh.obj");
			ImGui::BulletText("SolidBlue.mat");
			ImGui::BulletText("VertexShader.hlsl");
			ImGui::BulletText("PixelShader.hlsl");
			ImGui::Columns(1);
		}
		ImGui::End();
	}

	// ---- Credits Window ----
	if (m_show_credits)
	{
		ImGui::SetNextWindowPos(ImVec2(scr_w * 0.35f, menu_h + work_h * 0.20f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(scr_w * 0.30f, work_h * 0.45f), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Credits", &m_show_credits))
		{
			if (m_logo_srv)
			{
				ImGui::Image((void*)m_logo_srv, ImVec2((float)m_logo_w, (float)m_logo_h));
				ImGui::Separator();
			}
			ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.4f, 1.0f), "Overture");
			ImGui::Text("Developer: Sol Tabboga");
			ImGui::Text("Institution: De La Salle University (DLSU)");
			ImGui::Separator();
			ImGui::TextWrapped("Acknowledgments & Dedication:");
			ImGui::TextWrapped("Dedicated to GDENG03 and Pard Code for supporting codes");
		}
		ImGui::End();
	}

	// ---- Color Picker Placeholder UI ----
	if (m_show_color_picker)
	{
		ImGui::SetNextWindowPos(ImVec2(scr_w * 0.35f, menu_h + work_h * 0.25f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(scr_w * 0.30f, work_h * 0.40f), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Color Picker (Placeholder)", &m_show_color_picker))
		{
			ImGui::TextWrapped("Select Object/Light Color (Placeholder UI - No backend functionality):");
			static float color[4] = { 0.2f, 0.4f, 1.0f, 1.0f };
			ImGui::ColorEdit4("Color Edit", color);
			ImGui::Separator();
			ImGui::ColorPicker4("Color Wheel", color);
		}
		ImGui::End();
	}
}

void UIManager::render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
