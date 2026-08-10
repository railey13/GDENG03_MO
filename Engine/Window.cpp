#include "Window.h"
#include "EngineTime.h"
#include "imgui.h"

#include <iostream>
#include <exception>

// for handling events in imgui
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd, ui32 msg, WPARAM wparam, LPARAM lparam) {
    // for processing inputs in imgui
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
        return true;
    }

    switch (msg) {
        case WM_SETFOCUS: {

            // Event fired when the window get focus
            Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

            if (window) window->onFocus();
            break;
        }
        case WM_KILLFOCUS: {

            // Event fired when the windowlost focus
            Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            window->onKillFocus();
            break;
        }
        case WM_DESTROY: {
            // event fired when the window will be destroyed
            // (updated code)
            Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            window->onDestroy();
            ::PostQuitMessage(0);
            break;
        }
        case WM_MOVE:
        case WM_SIZE: {
            Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if (window) {
                ui32 newWidth = LOWORD(lparam);
                ui32 newHeight = HIWORD(lparam);
                window->onResize(newWidth, newHeight);
            }
            break;
        }
        default: {
            return ::DefWindowProc(hwnd, msg, wparam, lparam);
        }
    }
    return 0;
}

Window::Window() {
    // starting up the WNDCLASSEX object and define the window attributes
    WNDCLASSEX wc;
    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = NULL;
    wc.lpszClassName = "MyWindowClass";
    wc.lpszMenuName = "";
    wc.style = NULL;
    wc.lpfnWndProc = &WndProc;

    // if the registration of class will fail, return false
    if (!::RegisterClassEx(&wc)) {
        throw std::exception("Window did not initiate successfully");
    }

    RECT rc = { 0, 0, Settings::WindowWidth, Settings::WindowHeight };
    ::AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    // create the window
    m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
        "MyWindowClass", "DirectX Application",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        NULL, NULL, NULL, NULL);

    // if window creation failed, return false
    if (!m_hwnd) {
        throw std::exception("Window did not initiate successfully");
    }

    // show the window
    ::ShowWindow(m_hwnd, SW_SHOW);
    ::UpdateWindow(m_hwnd);

    // set to true to indicate the window is running properly
    m_is_run = true;
}

bool Window::isRun() {
    if (m_is_run) this->broadcast();
    return m_is_run;
}

RECT Window::getClientWindowRect() {
    RECT rc;
    ::GetClientRect(this->m_hwnd, &rc);

    return rc;
}

void Window::onDestroy(){
    m_is_run = false;
}

void Window::onFocus() {

}

void Window::onKillFocus() {

}

Window::~Window() {

}

bool Window::broadcast() {
    EngineTime::LogFrameStart();

    if (!this->m_is_init) {
        SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
        this->onCreate();
        this->m_is_init = true;
    }

    this->onUpdate();

    MSG msg;

    while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }

    EngineTime::LimitFPS(Settings::FrameRateLimit);
    //EngineTime::UpdateFPSCounter();

    return true;
}