#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Vector3D.h"
#include "RenderTexture.h"
#include "UIManager.h"

class AppWindow : public Window, public InputListener
{
public:
    AppWindow();
    ~AppWindow();

    virtual void onCreate()     override;
    virtual void onUpdate()     override;
    virtual void onDestroy()    override;
    virtual void onFocus()      override;
    virtual void onKillFocus()  override;

    // Keyboard
    void onKeyDown(int key) override;
    void onKeyUp(int key)   override;

    // Mouse
    virtual void onMouseMove(const Point& delta_mouse_pos)  override;
    virtual void onLeftMouseDown(const Point& mouse_pos)    override;
    virtual void onLeftMouseUp(const Point& mouse_pos)      override;
    virtual void onRightMouseDown(const Point& mouse_pos)   override;
    virtual void onRightMouseUp(const Point& mouse_pos)     override;

private:
    SwapChain*      m_swap_chain  = nullptr;
    VertexBuffer*   m_cubeVB      = nullptr;
    IndexBuffer*    m_cubeIB      = nullptr;
    VertexShader*   m_vs          = nullptr;
    PixelShader*    m_ps          = nullptr;
    ConstantBuffer* m_cb          = nullptr;
    UIManager*      m_ui_manager  = nullptr;
    RenderTexture*  m_editor_rt   = nullptr;
    RenderTexture*  m_game_rt     = nullptr;

private:

    // Cube transform state
    float m_rot_x      = 0.0f;
    float m_rot_y      = 0.0f;
    float m_cube_scale = 1.0f;



    // Mouse button held state
    bool m_lmb_down = false;
    bool m_rmb_down = false;

    // Camera transform and movement state
    Vector3D m_cam_pos = Vector3D(0.0f, 0.0f, -2.0f);
    bool m_forward  = false;
    bool m_backward = false;
    bool m_right    = false;
    bool m_left     = false;
    bool m_up       = false;
    bool m_down     = false;
};