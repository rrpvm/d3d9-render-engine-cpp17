#include "GChild.hpp"
#include "Gui2D.h"
void GChild::draw()
{
	gui_draw_manager->rect(position, size, gui_2d->get_style()->component_colors[CHILD]);
}
GComponent* GChild::add_child(int gui_comp, const vec2& size, const std::string& label)
{
    GComponent* ptr = nullptr;
    switch (gui_comp)
    {
    case CHECKBOX: {
        for (GCheckBox* cb : gui_2d->get_cb_list())
        {
            if (cb->label == label)return cb;
        }
        GCheckBox* tmp = new GCheckBox();
        tmp->label = label;
        tmp->position = this->relative_pos + this->position;
        tmp->size = size;
        gui_2d->get_cb_list().emplace_back(tmp);
        this->current_child = tmp;
        this->relative_pos.y += size.y;
        this->relative_pos.y += gui_2d->get_style()->component_margins[CHECKBOX].y;
        ptr = tmp;
        break;
    }
    }
    return ptr;
}
