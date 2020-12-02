#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <memory>
#include <cassert>
#include "math/vec2.hpp"
#include "helpers/Color.hpp"
#include "vec3.hpp"
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
struct CUSTOMVERTEX
{
	FLOAT x, y, z;    // from the D3DFVF_XYZRHW flag
	DWORD color;    // from the D3DFVF_DIFFUSE flag
};

class renderer
{
public:
	renderer() {
		d3d9_device = nullptr;
		vertex_buffer = nullptr;
		d3d9_object = std::unique_ptr<IDirect3D9>(Direct3DCreate9(D3D_SDK_VERSION));
	}
	~renderer() {

	}
	bool initialize(const HWND& hwnd, const vec2& size);
	void begin_frame();
	void end_frame();
	void set_frame_settings();
	void set_viewport();
	/*OOOOOOVERDRIIIIIVEEEEEEEEEEEEEE*/
	void draw_rectangle(const vec2& position, const vec2& size, const Color& color);
	void draw_rectangle(const vec2& position, const vec2& size, const Color& c1, const Color& c2, const Color& c3, const Color& c4);
	void draw_rectangle(const vec2& position, const vec2& size, const Color& c1, const Color& c2);
	void draw_rectangle(const vec3& position, const vec3& size, const Color& color);
	void draw_rectangle(const vec3& position, const vec3& size, const Color& c1, const Color& c2, const Color& c3, const Color& c4);
	void draw_rectangle(const vec3& position, const vec3& size, const Color& c1, const Color& c2);
	//\\<!TRIANGLE!>\\//
	void draw_triangle(const vec2&p1, const vec2& p2, const vec2& p3, const Color& color);
	void draw_triangle(const vec2&p1, const vec2& p2, const vec2& p3, const Color& color1, const Color& color2, const Color& color3);
	void draw_triangle(const vec2& p1, const vec2& p2, const vec2& p3, Color colors[3] );
	void draw_triangle(const std::pair<const vec2&, const Color&>&p1, const std::pair<const vec2&, const Color&>& p2, const std::pair<const vec2&, const Color&>& p3);
	void draw_triangle(const vec3& p1, const vec3& p2, const vec3& p3, const Color& color);
	void draw_triangle(const vec3& p1, const vec3& p2, const vec3& p3, const Color& color1, const Color& color2, const Color& color3);
	void draw_triangle(const vec3& p1, const vec3& p2, const vec3& p3, Color colors[3]);
	void draw_triangle(const std::pair<const vec3&, const Color&>& p1, const std::pair<const vec3&, const Color&>& p2, const std::pair<const vec3&, const Color&>& p3);

private:
	std::unique_ptr<IDirect3DDevice9>d3d9_device{ nullptr };
	std::unique_ptr<IDirect3D9>d3d9_object{ nullptr };
	std::unique_ptr<IDirect3DVertexBuffer9>vertex_buffer{ nullptr };	
};
inline std::unique_ptr<renderer>render(new renderer());
