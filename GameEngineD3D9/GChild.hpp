#pragma once
#include "GUI2D/GComponent.hpp"
class GChild : public  GParent
{
public:
	GChild() {};
	~GChild() {};
	virtual void draw() override;
	virtual GComponent* add_child(int gui_comp, const vec2& size, const std::string& label) override;
	//virtual GComponent* add_checkbox(const std::string& label, bool* value);
private:
};

