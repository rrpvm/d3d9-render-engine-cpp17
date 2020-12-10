#pragma once
#include "../renderer.hpp"
class GuiDrawManager : protected renderer { // this class will draw every component
public:
	GuiDrawManager() {};
	~GuiDrawManager() {};
	void init(LPDIRECT3DDEVICE9 device);
	void rect(const vec2& pos, const vec2& size, const Color& color);
	void filled_rect(const vec2& pos, const vec2& size, const Color& color);
private:
	IDirect3DDevice9* device = nullptr;
	std::unique_ptr<IDirect3DVertexBuffer9>d3vb9 = nullptr;
};
inline std::unique_ptr< GuiDrawManager>gui_draw_manager(new GuiDrawManager());