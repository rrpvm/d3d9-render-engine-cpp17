#pragma once
#include "GUI2D/GuiDrawManager.hpp"
#include <malloc.h>
#include "GUI2D/GWindow.hpp"
#include <string>
#include "helpers/container.hpp"
#include "GChild.hpp"
#include "GCheckBox.hpp"
#include "GUI2D/GInput.hpp"
#define FVF2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
typedef struct GUIStyle {
	GUIStyle() {

	}
	~GUIStyle() {
		free(component_colors);
		free(component_margins);
		free(component_paddings);
		free(component_size);
	}
	GUIStyle(int components_num)
	{
		component_colors = reinterpret_cast<Color*>(malloc(sizeof(Color) * components_num));		
		component_margins = reinterpret_cast<vec2*>(malloc(sizeof(vec2) * components_num));
		component_paddings = reinterpret_cast<vec2*>(malloc(sizeof(vec2) * components_num));
		component_size = reinterpret_cast<vec2*>(malloc(sizeof(vec2) * components_num));
		new_line_space = 35.f;
		same_line_space = 100.0f;


		component_colors[WINDOW].set_color(44, 44, 44,255);
		component_colors[CHILD].set_color(164, 84, 84,255);
		component_colors[CHECKBOX].set_color(255, 255, 84,255);
		component_margins[CHILD] = vec2(10.f, 15.f);
		component_margins[CHECKBOX] = vec2(10.f, 15.f);
		component_size[CHECKBOX] = vec2(15.f, 10.f);
		

	}
	Color* component_colors;
	vec2* component_margins;
	vec2* component_paddings;
	vec2* component_size;
	float new_line_space;
	float same_line_space;
};

class Gui2D //contains all Windows. Windows->CHILDS->ETC ELEMENTS,GROUPS->ETC ELEMENTS(BUT EXCEPT GROUPS),ETC ELEMTNS(EXCEPT CHILDS AND GROUPS)
{
public:
	Gui2D() {
		g_style = std::unique_ptr<GUIStyle>(new GUIStyle(5));
		initialized = false;
		begin_called = false;
		end_called = true;
		windows_list.clear();
		child_list.clear();
		cb_list.clear();
		windows_list.reserve(10);
		child_list.reserve(10);
		cb_list.reserve(10);

	};
	~Gui2D() {
		for (auto& tmp : windows_list)delete tmp;
	};
	void init(LPDIRECT3DDEVICE9 device);
	void begin();
	void end();
	void deselect_window() { this->current_window = nullptr; };
	virtual void render_frame();
	GUIStyle* get_style();
	GWindow* add_window(const std::string& label, const vec2& pos, const vec2& size);
	GChild* add_child(const std::string& label, const vec2& size);



	helpers::container<GWindow*>& get_windows_list();
	helpers::container<GChild*>& get_child_list();
	helpers::container<GCheckBox*>& get_cb_list();
protected:
	IDirect3DDevice9* p_dev = nullptr;
	std::unique_ptr<GUIStyle> g_style = nullptr;
	GWindow* current_window = nullptr;
	GChild* current_child = nullptr;
	helpers::container<GWindow*>windows_list;
	helpers::container<GChild*>child_list;
	helpers::container<GCheckBox*>cb_list;
	void set_preset_to_2d_overlay();
	vec2 relative_pos;
private:
	bool initialized = false;
	bool begin_called = false;
	bool end_called = true;
};
inline std::unique_ptr<Gui2D>gui_2d(new Gui2D());
