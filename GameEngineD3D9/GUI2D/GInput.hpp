#pragma once
#include <memory>
#include "../math/vec2.hpp"
class GInput
{
public:
	GInput() {};
	~GInput() {};
	vec2 g_mouse;
	float one_frame_time = 0.250f;//250 ms
	bool click_per_frame;
	bool last_tick_lkm_pressed;
private:
};
inline std::unique_ptr<GInput>g_input(new GInput());

