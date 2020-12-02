#pragma once
#include <Windows.h>
#include <string>
#include "math/vec2.hpp"
class WindowManager
{
public:
	WindowManager() {

	};
	~WindowManager() {

	};
	HWND create_window(const std::string& name, const vec2& pos, const vec2& size, HINSTANCE&hInst);
};

