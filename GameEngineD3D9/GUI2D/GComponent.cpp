#include "GComponent.hpp"
#include "../GChild.hpp"
#include "../Gui2D.h"
GComponent* GParent::add_child(int gui_comp, const vec2& size, const std::string& label)
{
    switch (gui_comp)
    {
    case CHILD: {
        for (GChild* chld : gui_2d->get_child_list())
        {
            if (chld->label == label)return chld;
        }
        GChild* child = new GChild();
        child->label = label;
        child->position = this->relative_pos + this->position;
        child->size = size;
        this->current_child = child;
        gui_2d->get_child_list().emplace_back(child);
        this->relative_pos.y += size.y;
        this->relative_pos.y += gui_2d->get_style()->component_margins[CHILD].y;
        return child;
      //  break;
    }
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
        return tmp;
       // break;
    }
    }
    return nullptr;
}
GComponent* GParent::add_checkbox(const std::string& label, bool* value)
{
    GCheckBox* cb = reinterpret_cast<GCheckBox*>(this->add_child(CHECKBOX, gui_2d->get_style()->component_size[CHECKBOX], label));
    assert(cb);
    cb->value = value;
    return cb;
}
inline bool GComponent::hover() noexcept
{
    if (g_input->g_mouse.x >= this->position.x && g_input->g_mouse.x <= this->position.x + this->size.x)
        if (g_input->g_mouse.y >= this->position.y && g_input->g_mouse.y <= this->position.y + this->size.y) {
            _hovered_ = true;  return true;
        }
    return false;
}
inline bool GComponent::clicked() noexcept
{
    if (this->_hovered_ && g_input->click_per_frame) {
        this->_clicked_ = true;
        return true;
    }
    return false;
}
