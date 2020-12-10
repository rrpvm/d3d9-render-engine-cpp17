#include "Gui2D.h"
#include "math/vec2.hpp"
void Gui2D::init(LPDIRECT3DDEVICE9 device)
{
    if (!initialized) {
        assert(device && "pointer to IDirect3DDevice9 == nullptr");

        this->p_dev = device;
        gui_draw_manager->init(device);
        initialized = true;
    }
}
void Gui2D::begin()
{
    set_preset_to_2d_overlay();
    begin_called = true;
    assert(end_called && "MISSED END()");
    end_called = false;
}
void Gui2D::end()
{
    assert(begin_called && "MISSED BEGIN()");
    begin_called = false;
    end_called = true;
    for (auto& wnd : windows_list)
    {
        wnd->draw();

    }
    for (auto& chld : child_list)
    {
        chld->draw();
    }
    for (auto& checkbx : cb_list)
    {
        checkbx->draw();
        checkbx->hover();
        if (checkbx->clicked())*checkbx->value = !*checkbx->value;
    }


}
bool del_me = true;
void Gui2D::render_frame()
{
    begin();
    GWindow* window = add_window("TEST", vec2(0, 0), vec2(640, 480));
    window->add_checkbox("TEST3", &del_me);
    GChild* child = add_child("TEST", vec2(100, 100));
    child->add_checkbox("BOX", &del_me);
    GChild* child2 = add_child("TEST2", vec2(100, 100));
    child2->add_checkbox("BOX2", &del_me);
    
    end();
}
GUIStyle* Gui2D::get_style()
{
    return g_style.get();
}
void Gui2D::set_preset_to_2d_overlay()
{
    this->p_dev->SetFVF(FVF2D);
    this->p_dev->SetStreamSource(0, nullptr, 0, 0);
    this->p_dev->SetRenderState(D3DRS_LIGHTING, FALSE);
    this->p_dev->SetRenderState(D3DRS_ZENABLE, FALSE);
    this->p_dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    this->p_dev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    this->p_dev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    this->p_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    this->p_dev->SetRenderState(D3DRS_POINTSIZE, 1.f);
}
GWindow* Gui2D::add_window(const std::string& label, const vec2& pos, const vec2& size)
{
    for (GWindow* wnds : this->windows_list)
    {
        if (wnds->label == label)return wnds;
    }
    GWindow* window = new GWindow();
    window->label = label;
    window->position = pos;
    window->size = size;
    windows_list.emplace_back(window);
    this->current_window = window;
    return window;
}
GChild* Gui2D::add_child(const std::string& label, const vec2& size)
{
    assert(this->current_window);
    for (GChild* chld : this->child_list)
    {
        if (chld->label == label)return chld;
    }
    GChild* child = new GChild();
    child->label = label;
    child->position = this->current_window->position + this->current_window->relative_pos;//OPERATOR  = GIVE A FUCK ABOUT ALL
    child->size = size;
    child_list.emplace_back(child);
    this->current_child = child;
    this->current_window->relative_pos.y += this->get_style()->component_margins[CHILD].y;
    this->current_window->relative_pos.y += size.y;
    return child;
}
helpers::container<GWindow*>& Gui2D::get_windows_list()
{
    return this->windows_list;
}
helpers::container<GChild*>& Gui2D::get_child_list()
{
    return this->child_list;
}
helpers::container<GCheckBox*>& Gui2D::get_cb_list()
{
    return this->cb_list;
}
