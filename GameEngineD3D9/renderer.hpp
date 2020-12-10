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
#include <string>
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define FVF_NORMAL_TEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
struct CUSTOMVERTEX
{
	FLOAT x, y, z;   
	DWORD color;    // from the D3DFVF_DIFFUSE flag
};
struct CUSTOMVERTEX2D
{
	FLOAT x, y, z, rhw;    // from the D3DFVF_XYZRHW flag
	DWORD color;    // from the D3DFVF_DIFFUSE flag
};
struct normal_textured_vertex
{
	FLOAT x, y, z; // position
	FLOAT nx, ny, nz; // normal vector
	FLOAT u, v; // texture coordinates
};
class d3d9_helper {
public:
	static IDirect3DVertexBuffer9* create_vertex_buffer(size_t length, DWORD type, LPDIRECT3DDEVICE9 dev);
};
class renderer//vec2 overrides useless
{
public:
	renderer() {
		d3d9_device = nullptr;
		vertex_buffer = nullptr;
		d3d9_object = std::unique_ptr<IDirect3D9>(Direct3DCreate9(D3D_SDK_VERSION));
		camera_position = { 0.0f, 0.0f, 10.0f };	
		current_poly_primitive = D3DPT_LINESTRIP;
	}
	~renderer() {
		/*release all stuff*/
		if(d3d9_device)d3d9_device->Release();
		if(vertex_buffer)vertex_buffer->Release();
		if(index_buffer)index_buffer->Release();
		if(d3d9_object)d3d9_object->Release();		
	}
	bool initialize(const HWND& hwnd, const vec2& size);
	void begin_frame();
	void end_frame();
	D3DMATERIAL9& init_mat(const ColorF& ambient);
private:
	void set_frame_settings();
	void set_viewport();
	/*OOOOOOVERDRIIIIIVEEEEEEEEEEEEEE*/
protected:
	void draw_rectangle_filled(const vec2& position, const vec2& size, const Color& color);
	void draw_rectangle_filled(const vec2& position, const vec2& size, const Color& c1, const Color& c2, const Color& c3, const Color& c4);
	void draw_rectangle_filled(const vec2& position, const vec2& size, const Color& c1, const Color& c2);
	void draw_rectangle_filled(const vec3& position, const vec3& size, const Color& color);
	void draw_rectangle_filled(const vec3& position, const vec3& size, const Color& c1, const Color& c2, const Color& c3, const Color& c4);
	void draw_rectangle_filled(const vec3& position, const vec3& size, const Color& c1, const Color& c2);
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
	//\\<!LINES!>\\//
	void draw_line(const vec2& position, const vec2& end_position, const Color& color);
	void draw_line(const vec3& position, const vec3& end_position, const Color& color, const Color& color2);
	//\\<!CIRCLE!>\\//
	void draw_circle(const vec3& pos, const float& radius,const int steps, const Color& color);
	//\\<!CUBE!>\\//
	void draw_cube(const vec3& pos, const Color& color);
	void draw_pyramide(const vec3& pos, const Color& color);
	//\\<POLYGONE\\//
	void draw_poly(vec3* vertices, int vertices_num,int poly_num, const Color& color);
	void set_poly_type(const D3DPRIMITIVETYPE& type);
	void set_poly_color(const Color& color);



	/*gui 2d drawing*/
	void render_filled_rect(const vec2& position, const vec2& size, const Color& color, IDirect3DVertexBuffer9* buffer = nullptr, LPDIRECT3DDEVICE9 dev = nullptr);
	void render_rect(const vec2& position, const vec2& size, const Color& color, IDirect3DVertexBuffer9* buffer = nullptr, LPDIRECT3DDEVICE9 dev = nullptr);

	void render_string(const vec2& pos, const std::string& message, const Color& color, bool outlined = false, bool centered = true, ID3DXFont* font);
	void render_string(const vec2& pos, const vec2& size, std::string message, const Color& color, bool outlined = false, bool centered = true, ID3DXFont* font);
	const vec2& calculate_txt_size(const std::string& message, const Color& color, bool outlined = false, bool centered = true, ID3DXFont* font);//move to engine
private:
	vec2 screen_resolution;
	D3DXVECTOR3 camera_position;
	D3DXMATRIX matrix_view;
	D3DXMATRIX matrix_projection;
	D3DPRIMITIVETYPE current_poly_primitive;//using by draw_poly()
	DWORD poly_color;//color wich used by draw_poly() without Color();
	const short camera_fov = 45;
	const float camera_max_view_distance = 200.f;
	const int max_vertex_count = 1024;
	std::unique_ptr<IDirect3DDevice9>d3d9_device{ nullptr };
	std::unique_ptr<IDirect3D9>d3d9_object{ nullptr };
	std::unique_ptr<IDirect3DVertexBuffer9>vertex_buffer{ nullptr };	
	std::unique_ptr<IDirect3DIndexBuffer9>index_buffer{ nullptr };	
	std::unique_ptr<ID3DXFont>d3d9font_tnr_normal;
};
inline std::unique_ptr<renderer>render(new renderer());
