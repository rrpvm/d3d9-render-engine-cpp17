#pragma once
#include "../math/vec2.hpp"
#include "GRenderable.hpp"
#include <string>
enum gui_components {
	NONE = -1,
	WINDOW,
	CHILD,
	GROUP,
	CHECKBOX,
	SLIDER,
};

class GComponent : public GRenderable {//base class for all Gui objects
public:
	GComponent() {
		type = NONE;
	};
	~GComponent() {};
	int type;
	std::string label;
	virtual void draw()override {};
	virtual bool hover() noexcept;
	virtual bool clicked() noexcept;
/*	virtual bool dragged() noexcept = 0;
	virtual void onhover() noexcept = 0;*/
private:
	bool _hovered_ = false;
	bool _clicked_ = false;
};
class GParent : public GComponent {
public:
	GParent() {};
	~GParent() {};
	vec2 relative_pos;
	virtual GComponent* add_child(int gui_comp,const vec2& size, const std::string&label);
	virtual GComponent* add_checkbox(const std::string& label, bool* value);
	GComponent* current_child = nullptr;
private:
};