#include "GCheckBox.hpp"
#include "Gui2D.h"
void GCheckBox::draw()
{
	if (*this->value)
		gui_draw_manager->filled_rect(position, size, gui_2d->get_style()->component_colors[CHECKBOX]);
	else gui_draw_manager->rect(position, size, gui_2d->get_style()->component_colors[CHECKBOX]);
}
