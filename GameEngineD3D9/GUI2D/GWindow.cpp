#include "GWindow.hpp"
#include "../Gui2D.h"
void GWindow::draw()
{
	gui_draw_manager->rect(position, size, gui_2d->get_style()->component_colors[WINDOW]);
}
