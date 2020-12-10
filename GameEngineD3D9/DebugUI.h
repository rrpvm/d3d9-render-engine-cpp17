#pragma once
#include "Gui2D.h"
class DebugUI : protected Gui2D
{
public:
	DebugUI() {};
	~DebugUI() {};
private:
};

inline std::unique_ptr<DebugUI>debug_ui(new DebugUI());