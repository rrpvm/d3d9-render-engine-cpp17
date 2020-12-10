#pragma once
#include "GComponent.hpp"
#include "GuiDrawManager.hpp"

class GWindow : public GParent // base parent class for data in GUI
{
public:
	GWindow() {};
	~GWindow() {};
	virtual void draw() override;
	
};

