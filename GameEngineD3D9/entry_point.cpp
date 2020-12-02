#include <Windows.h>
#include "WindowManager.hpp"
#include "renderer.hpp"
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow){
	WindowManager wm;
	MSG         msg;
	HWND hWnd = wm.create_window("Qwerty", vec2(CW_USEDEFAULT, CW_USEDEFAULT), vec2(1280, 720), hInstance);
	render->initialize(hWnd, vec2(1280, 720));

	while (true)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Translate the message and dispatch it to WindowProc()
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		render->begin_frame();
		render->end_frame();
	}
}