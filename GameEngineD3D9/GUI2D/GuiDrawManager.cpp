#include "GuiDrawManager.hpp"

void GuiDrawManager::init(LPDIRECT3DDEVICE9 device)
{
	this->device = device;
	this->d3vb9 = std::unique_ptr<IDirect3DVertexBuffer9>(d3d9_helper::create_vertex_buffer(sizeof(CUSTOMVERTEX2D) * 128, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, device));
}

void GuiDrawManager::rect(const vec2& pos, const vec2& size, const Color& color)
{
	this->render_rect(pos, size, color, this->d3vb9.get(), this->device);
}

void GuiDrawManager::filled_rect(const vec2& pos, const vec2& size, const Color& color)
{
	this->render_filled_rect(pos, size, color, this->d3vb9.get(), this->device);
}
