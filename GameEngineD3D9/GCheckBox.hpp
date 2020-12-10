#pragma once
#include "GUI2D/GComponent.hpp"
class GCheckBox : public GComponent
{
public:
	GCheckBox() {};
	~GCheckBox() {};
	bool* value = nullptr;
	virtual void draw() override;
private:
	void change_state() { *value = !*value; };
};
