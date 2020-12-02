#include "WindowManager.hpp"



LRESULT CALLBACK WindowProc(
    _In_ HWND   hwnd,
    _In_ UINT   uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
)
{
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}




HWND WindowManager::create_window(const std::string& name, const vec2& pos, const vec2& size, HINSTANCE& hInst)
{
    WNDCLASSEX wcex;
    TCHAR ClassName[100] = { 'G','a','m','e',' ','E','n','g','i','n','e','\0' };  
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.hIcon = LoadIcon(nullptr,IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = ClassName;
    wcex.hIconSm = 0;
    RegisterClassEx(&wcex);
    HWND hWnd = CreateWindow(ClassName, name.c_str(), WS_OVERLAPPEDWINDOW, static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x), static_cast<int>(size.y),nullptr, nullptr, hInst, nullptr);
    if (!hWnd)
    {
        MessageBox(NULL, std::to_string(GetLastError()).c_str(), "Error", MB_OK);
    }
    UpdateWindow(hWnd);
    ShowWindow(hWnd, 1);    
    return hWnd;
}
