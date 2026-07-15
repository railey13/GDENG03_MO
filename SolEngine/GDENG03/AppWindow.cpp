#include "AppWindow.h"
#include "UIManager.h"
#include "IMGUI/imgui.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#define NOMINMAX
#include <Windows.h>
#include <chrono>



AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

// -----------------------------------------------------------------------
//  GPU data structures (must match VertexShader.hlsl)
// -----------------------------------------------------------------------
struct vertex
{
    Vector3D position;
    Vector3D position1;
    Vector3D color;
};

__declspec(align(16))
struct constant
{
    Matrix4x4    m_world;
    Matrix4x4    m_view;
    Matrix4x4    m_proj;
    Vector3D     m_color;
    unsigned int m_time;
};

// -----------------------------------------------------------------------
//  onCreate — build cube, compile shaders, create constant buffer
// -----------------------------------------------------------------------
void AppWindow::onCreate()
{
    InputSystem::get()->addListener(this);

    GraphicsEngine::get()->init();

    RECT rc = getClientWindowRect();
    m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

    // ---- Vertex shader ----
    void*  shader_byte_code = nullptr;
    size_t size_shader      = 0;

    if (!GraphicsEngine::get()->getRenderSystem()->compileVertexShaders(L"VertexShader.hlsl", "vsmain",
                                                     &shader_byte_code, &size_shader))
    {
        MessageBoxA(m_hwnd, "Failed to compile VertexShader.hlsl", "Shader Error", MB_OK | MB_ICONERROR);
        return;
    }

    m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
    if (!m_vs)
    {
        MessageBoxA(m_hwnd, "Failed to create vertex shader", "Shader Error", MB_OK | MB_ICONERROR);
        return;
    }

    // ---- Cube geometry ----
    //
    //   7 --- 6
    //  /|    /|
    // 4 --- 5 |
    // | 3 --| 2
    // |/    |/
    // 0 --- 1
    //
    vertex cube_verts[] =
    {
        // position                  position1 (unused)    color
        { Vector3D(-0.5f,-0.5f,-0.5f), Vector3D(0,0,0), Vector3D(1.0f, 0.2f, 0.2f) }, // 0 front-bottom-left
        { Vector3D( 0.5f,-0.5f,-0.5f), Vector3D(0,0,0), Vector3D(0.2f, 1.0f, 0.2f) }, // 1 front-bottom-right
        { Vector3D( 0.5f,-0.5f, 0.5f), Vector3D(0,0,0), Vector3D(0.2f, 0.2f, 1.0f) }, // 2 back-bottom-right
        { Vector3D(-0.5f,-0.5f, 0.5f), Vector3D(0,0,0), Vector3D(1.0f, 1.0f, 0.2f) }, // 3 back-bottom-left
        { Vector3D(-0.5f, 0.5f,-0.5f), Vector3D(0,0,0), Vector3D(0.2f, 1.0f, 1.0f) }, // 4 front-top-left
        { Vector3D( 0.5f, 0.5f,-0.5f), Vector3D(0,0,0), Vector3D(1.0f, 0.2f, 1.0f) }, // 5 front-top-right
        { Vector3D( 0.5f, 0.5f, 0.5f), Vector3D(0,0,0), Vector3D(1.0f, 1.0f, 1.0f) }, // 6 back-top-right
        { Vector3D(-0.5f, 0.5f, 0.5f), Vector3D(0,0,0), Vector3D(0.5f, 0.5f, 0.5f) }, // 7 back-top-left
    };

    // 12 triangles × 3 indices = 36 (CW winding)
    unsigned int cube_indices[] =
    {
        // Front  (-Z)
        0, 5, 4,    0, 1, 5,
        // Back   (+Z)
        2, 7, 6,    2, 3, 7,
        // Left   (-X)
        3, 4, 7,    3, 0, 4,
        // Right  (+X)
        1, 6, 5,    1, 2, 6,
        // Bottom (-Y)
        3, 1, 0,    3, 2, 1,
        // Top    (+Y)
        4, 6, 7,    4, 5, 6,
    };

    m_cubeVB = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(cube_verts, sizeof(vertex), ARRAYSIZE(cube_verts),
                   shader_byte_code, (UINT)size_shader);

    m_cubeIB = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(cube_indices, ARRAYSIZE(cube_indices));

    GraphicsEngine::get()->getRenderSystem()->releaseCompiledShaders();

    // ---- Pixel shader ----
    if (!GraphicsEngine::get()->getRenderSystem()->compilePixelShaders(L"PixelShader.hlsl", "psmain",
                                                    &shader_byte_code, &size_shader))
    {
        MessageBoxA(m_hwnd, "Failed to compile PixelShader.hlsl", "Shader Error", MB_OK | MB_ICONERROR);
        return;
    }

    m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
    if (!m_ps)
    {
        MessageBoxA(m_hwnd, "Failed to create pixel shader", "Shader Error", MB_OK | MB_ICONERROR);
        return;
    }
    GraphicsEngine::get()->getRenderSystem()->releaseCompiledShaders();

    // ---- Constant buffer ----
    constant cc = {};
    cc.m_time = 0;
    m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

    m_ui_manager = new UIManager(this->m_hwnd, GraphicsEngine::get()->getRenderSystem());
    m_editor_rt = GraphicsEngine::get()->getRenderSystem()->createRenderTexture(1024, 768);
    m_game_rt   = GraphicsEngine::get()->getRenderSystem()->createRenderTexture(1024, 768);
}

// -----------------------------------------------------------------------
//  onUpdate — scale from held buttons, build transforms, draw cube
// -----------------------------------------------------------------------
void AppWindow::onUpdate()
{
    static auto last_frame_time = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    float dt = std::chrono::duration<float>(now - last_frame_time).count();

    if (dt < (1.0f / 60.0f))
    {
        Sleep(1);
        return;
    }
    last_frame_time = now;

    // Scale cube with held left mouse button
    if (m_lmb_down)
    {
        m_cube_scale += 0.5f * dt;
        if (m_cube_scale > 5.0f) m_cube_scale = 5.0f;
    }

    DeviceContext* ctx = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();

    // ---- 1. Render Editor Viewport to m_editor_rt ----
    if (m_editor_rt)
    {
        ctx->setRenderTarget(m_editor_rt, 0.12f, 0.12f, 0.16f, 1.0f);
        ctx->setViewportSize(m_editor_rt->getWidth(), m_editor_rt->getHeight());

        Matrix4x4 world_cam;
        world_cam.setIdentity();
        Matrix4x4 rot_x; rot_x.setRotationX(m_rot_x);
        Matrix4x4 rot_y; rot_y.setRotationY(m_rot_y);
        world_cam *= rot_x;
        world_cam *= rot_y;

        Vector3D new_pos = m_cam_pos + world_cam.getZDirection() * (m_forward ? 5.0f * dt : 0.0f);
        new_pos = new_pos + world_cam.getZDirection() * (m_backward ? -5.0f * dt : 0.0f);
        new_pos = new_pos + world_cam.getXDirection() * (m_right ? 5.0f * dt : 0.0f);
        new_pos = new_pos + world_cam.getXDirection() * (m_left ? -5.0f * dt : 0.0f);
        new_pos = new_pos + world_cam.getYDirection() * (m_up ? 5.0f * dt : 0.0f);
        new_pos = new_pos + world_cam.getYDirection() * (m_down ? -5.0f * dt : 0.0f);
        m_cam_pos = new_pos;

        world_cam.setTranslation(m_cam_pos);
        Matrix4x4 m_view = world_cam;
        m_view.inverse();

        ImVec2 ed_size = m_ui_manager ? m_ui_manager->getEditorViewportSize() : ImVec2(1024, 768);
        float aspect = (ed_size.x > 0.0f && ed_size.y > 0.0f) ? (ed_size.x / ed_size.y) : ((float)m_editor_rt->getWidth() / (float)m_editor_rt->getHeight());
        Matrix4x4 m_proj;
        m_proj.setPerspectiveFovLH(1.57f, aspect, 0.1f, 100.0f);

        ctx->setVertexShader(m_vs);
        ctx->setPixelShader(m_ps);
        ctx->setConstantBuffer(m_vs, m_cb);
        ctx->setConstantBuffer(m_ps, m_cb);
        ctx->setVertexBuffer(m_cubeVB);
        ctx->setIndexBuffer(m_cubeIB);

        auto draw_object = [&](float sx, float sy, float sz,
                               float rx, float ry, float rz,
                               float px, float py, float pz,
                               const Vector3D& color)
        {
            Matrix4x4 sc, rotX, rotY, rotZ, tr, world;
            sc.setIdentity(); sc.setScale(Vector3D(sx, sy, sz));
            rotX.setIdentity(); rotX.setRotationX(rx);
            rotY.setIdentity(); rotY.setRotationY(ry);
            rotZ.setIdentity(); rotZ.setRotationZ(rz);
            tr.setIdentity(); tr.setTranslation(Vector3D(px, py, pz));
            world.setIdentity(); world *= sc; world *= rotX; world *= rotY; world *= rotZ; world *= tr;

            constant cc = {};
            cc.m_time  = (unsigned int)::GetTickCount64();
            cc.m_world = world;  cc.m_view = m_view;  cc.m_proj = m_proj;
            cc.m_color = color;
            m_cb->update(ctx, &cc);
            ctx->drawIndexedTriangleList(m_cubeIB->getSizeIndexList(), 0, 0);
        };

        draw_object(20.0f, 0.05f, 20.0f,  0, 0, 0,  0.0f, -0.025f, 5.0f,  Vector3D(1.0f, 1.0f, 1.0f));
        draw_object(1.0f * m_cube_scale, 1.0f * m_cube_scale, 1.0f * m_cube_scale,  0, 0, 0,  0.0f, 0.5f, 5.0f,  Vector3D(0.2f, 0.4f, 1.0f));
    }

    // ---- 2. Render Game Viewport to m_game_rt (Camera object's location) ----
    if (m_game_rt)
    {
        ctx->setRenderTarget(m_game_rt, 0.08f, 0.08f, 0.12f, 1.0f);
        ctx->setViewportSize(m_game_rt->getWidth(), m_game_rt->getHeight());

        Matrix4x4 game_cam;
        game_cam.setIdentity();
        Matrix4x4 rot_x; rot_x.setRotationX(0.35f);
        game_cam *= rot_x;
        game_cam.setTranslation(Vector3D(0.0f, 2.5f, 0.0f));
        Matrix4x4 m_view = game_cam;
        m_view.inverse();

        ImVec2 gm_size = m_ui_manager ? m_ui_manager->getGameViewportSize() : ImVec2(1024, 768);
        float aspect = (gm_size.x > 0.0f && gm_size.y > 0.0f) ? (gm_size.x / gm_size.y) : ((float)m_game_rt->getWidth() / (float)m_game_rt->getHeight());
        Matrix4x4 m_proj;
        m_proj.setPerspectiveFovLH(1.57f, aspect, 0.1f, 100.0f);

        ctx->setVertexShader(m_vs);
        ctx->setPixelShader(m_ps);
        ctx->setConstantBuffer(m_vs, m_cb);
        ctx->setConstantBuffer(m_ps, m_cb);
        ctx->setVertexBuffer(m_cubeVB);
        ctx->setIndexBuffer(m_cubeIB);

        auto draw_object = [&](float sx, float sy, float sz,
                               float rx, float ry, float rz,
                               float px, float py, float pz,
                               const Vector3D& color)
        {
            Matrix4x4 sc, rotX, rotY, rotZ, tr, world;
            sc.setIdentity(); sc.setScale(Vector3D(sx, sy, sz));
            rotX.setIdentity(); rotX.setRotationX(rx);
            rotY.setIdentity(); rotY.setRotationY(ry);
            rotZ.setIdentity(); rotZ.setRotationZ(rz);
            tr.setIdentity(); tr.setTranslation(Vector3D(px, py, pz));
            world.setIdentity(); world *= sc; world *= rotX; world *= rotY; world *= rotZ; world *= tr;

            constant cc = {};
            cc.m_time  = (unsigned int)::GetTickCount64();
            cc.m_world = world;  cc.m_view = m_view;  cc.m_proj = m_proj;
            cc.m_color = color;
            m_cb->update(ctx, &cc);
            ctx->drawIndexedTriangleList(m_cubeIB->getSizeIndexList(), 0, 0);
        };

        draw_object(20.0f, 0.05f, 20.0f,  0, 0, 0,  0.0f, -0.025f, 5.0f,  Vector3D(1.0f, 1.0f, 1.0f));
        draw_object(1.0f * m_cube_scale, 1.0f * m_cube_scale, 1.0f * m_cube_scale,  0, 0, 0,  0.0f, 0.5f, 5.0f,  Vector3D(0.2f, 0.4f, 1.0f));
    }

    // ---- 3. Render directly to main SwapChain back buffer & ImGui UI ----
    ctx->clearRenderTargetColor(this->m_swap_chain, 0.1f, 0.1f, 0.15f, 1.0f);
    RECT rc = getClientWindowRect();
    float width  = (float)(rc.right  - rc.left);
    float height = (float)(rc.bottom - rc.top);
    ctx->setViewportSize((UINT)width, (UINT)height);

    if (m_ui_manager)
    {
        m_ui_manager->update(m_editor_rt ? m_editor_rt->getShaderResourceView() : nullptr,
                             m_game_rt ? m_game_rt->getShaderResourceView() : nullptr);
        m_ui_manager->render();
    }

    m_swap_chain->present(false);
}

// -----------------------------------------------------------------------
//  onDestroy
// -----------------------------------------------------------------------
void AppWindow::onDestroy()
{
    Window::onDestroy();

    if (m_cubeVB)    { delete m_cubeVB;     m_cubeVB    = nullptr; }
    if (m_cubeIB)    { delete m_cubeIB;     m_cubeIB    = nullptr; }
    if (m_cb)        { delete m_cb;         m_cb        = nullptr; }
    if (m_vs)        { delete m_vs;         m_vs        = nullptr; }
    if (m_ps)        { delete m_ps;         m_ps        = nullptr; }
    if (m_swap_chain){ delete m_swap_chain; m_swap_chain = nullptr; }
    if (m_ui_manager){ delete m_ui_manager; m_ui_manager = nullptr; }
    if (m_editor_rt) { delete m_editor_rt;  m_editor_rt  = nullptr; }
    if (m_game_rt)   { delete m_game_rt;    m_game_rt    = nullptr; }

    GraphicsEngine::get()->release();
}

// -----------------------------------------------------------------------
//  Focus
// -----------------------------------------------------------------------
void AppWindow::onFocus()
{
    InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
    InputSystem::get()->removeListener(this);
}

// -----------------------------------------------------------------------
//  Keyboard
// -----------------------------------------------------------------------
void AppWindow::onKeyDown(int key)
{
    if (ImGui::GetCurrentContext() && ImGui::GetIO().WantCaptureKeyboard) return;
    if (key == 'W') m_forward = true;
    if (key == 'S') m_backward = true;
    if (key == 'A') m_left = true;
    if (key == 'D') m_right = true;
    if (key == 'Q') m_down = true;
    if (key == 'E') m_up = true;
}

void AppWindow::onKeyUp(int key)
{
    if (key == 'W') m_forward = false;
    if (key == 'S') m_backward = false;
    if (key == 'A') m_left = false;
    if (key == 'D') m_right = false;
    if (key == 'Q') m_down = false;
    if (key == 'E') m_up = false;

    if (key == VK_ESCAPE)
        m_is_run = false;
}

// -----------------------------------------------------------------------
//  Mouse
// -----------------------------------------------------------------------
void AppWindow::onMouseMove(const Point& delta_mouse_pos)
{
    if (!m_rmb_down) return; // Only rotate camera when holding Right-Click!

    // Accumulate rotation: horizontal delta → Y-axis, vertical delta → X-axis
    const float sensitivity = 0.003f;
    m_rot_y += delta_mouse_pos.m_x * sensitivity;
    m_rot_x += delta_mouse_pos.m_y * sensitivity;
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
    if (ImGui::GetCurrentContext() && ImGui::GetIO().WantCaptureMouse) return;
    m_lmb_down = true;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
    m_lmb_down = false;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
    m_rmb_down = true;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
    m_rmb_down = false;
}
