#pragma once
#include <Windows.h>
#include <string>
#include "math/vec2.hpp"
#include <memory>
#include "GUI2D/GInput.hpp"
class WindowManager
{
public:
	WindowManager() {

	};
	~WindowManager() {

	};
	HWND create_window(const std::string& name, const vec2& pos, const vec2& size, HINSTANCE&hInst);
	vec2 mouse_pos;
};

inline std::unique_ptr<WindowManager>wnd_manager(new WindowManager());