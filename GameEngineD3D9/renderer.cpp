#include "renderer.hpp"
#include <d3d9types.h>
#include <d3dx9math.h>
#include "helpers/container.hpp"
bool renderer::initialize(const HWND& hwnd, const vec2& size)
{
    assert(this->d3d9_object.get());
    this->screen_resolution = size;
    D3DPRESENT_PARAMETERS d3dpp{};
    d3dpp.BackBufferWidth = (int)size.x;
    d3dpp.BackBufferHeight = (int)size.y;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality = 0;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.Windowed = true;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.Flags = 0;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    IDirect3DDevice9* d3d9_tmp = nullptr;
    IDirect3DVertexBuffer9* vertex_buffer_tmp = nullptr;
    IDirect3DIndexBuffer9* index_buffer_tmp = nullptr;
    HRESULT hr = this->d3d9_object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3d9_tmp);
    assert(d3d9_tmp);
    this->d3d9_device = std::unique_ptr<IDirect3DDevice9>(d3d9_tmp);
    this->d3d9_device->CreateVertexBuffer(max_vertex_count * sizeof(CUSTOMVERTEX), 0, CUSTOMFVF, D3DPOOL_MANAGED, &vertex_buffer_tmp, nullptr);
    this->d3d9_device->CreateIndexBuffer(256 * sizeof(short), 0,D3DFMT_INDEX16,D3DPOOL_MANAGED,&index_buffer_tmp, NULL);
    this->vertex_buffer = std::unique_ptr<IDirect3DVertexBuffer9>(vertex_buffer_tmp);
    this->index_buffer = std::unique_ptr<IDirect3DIndexBuffer9>(index_buffer_tmp);
    assert(vertex_buffer_tmp); 
    assert(index_buffer_tmp);
    return SUCCEEDED(hr);
}
void renderer::begin_frame()
{
    assert(this->d3d9_device.get()); 
    this->d3d9_device->BeginScene();
    this->d3d9_device->Clear(0l, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    d3d9_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    this->set_frame_settings();
    this->set_viewport();  
}
void renderer::end_frame()
{
    assert(this->d3d9_device.get());
    this->d3d9_device->EndScene();
    this->d3d9_device->Present(NULL, NULL, NULL, NULL);
}
void renderer::set_frame_settings()
{
    this->d3d9_device->SetFVF(CUSTOMFVF);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    d3d9_device->SetRenderState(D3DRS_LIGHTING, FALSE);
    d3d9_device->SetRenderState(D3DRS_ZENABLE, TRUE);
    this->d3d9_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  // this->d3d9_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    this->d3d9_device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
}
void renderer::set_viewport()
{
    D3DXMatrixLookAtLH(&this->matrix_view,
        &D3DXVECTOR3(this->camera_position.x, this->camera_position.y, this->camera_position.z),    // the camera position
        &D3DXVECTOR3(0.0f, 0.0f, 0.0f),    // the look-at position
        &D3DXVECTOR3(0.0f, 1.0f, 1.0f));    // the up direction
    d3d9_device->SetTransform(D3DTS_VIEW, &this->matrix_view);
    D3DXMatrixPerspectiveFovLH(&this->matrix_projection,
        D3DXToRadian(this->camera_fov),    // the horizontal field of view
        (FLOAT)this->screen_resolution.x / (FLOAT)this->screen_resolution.y, // aspect ratio
        1.0f,    // the near view-plane
        this->camera_max_view_distance);    // the far view-plane
    d3d9_device->SetTransform(D3DTS_PROJECTION, &matrix_projection);    // set the projection*/
}
void renderer::draw_rectangle(const vec2& position, const vec2& size, const Color& color)
{
    const DWORD d_color = color.to_dword();
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[4];
    verticies[0] = { position.x, position.y, 0.f,  d_color };
    verticies[1] = { position.x +size.x, position.y, 0.f,  d_color };
    verticies[2] = { position.x, position.y + size.y, 0.f,  d_color };
    verticies[3] = { position.x+size.x, position.y + size.y, 0.f,  d_color };

    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 4);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_rectangle(const vec2& position, const vec2& size, const Color& c1, const Color& c2, const Color& c3, const Color& c4)
{
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[4];
    verticies[0] = { position.x, position.y, 0.f,  c1.to_dword() };
    verticies[1] = { position.x + size.x, position.y, 0.f,  c2.to_dword() };
    verticies[2] = { position.x, position.y + size.y, 0.f,  c3.to_dword() };
    verticies[3] = { position.x + size.x, position.y + size.y, 0.f,  c4.to_dword() };

    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 4);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_rectangle(const vec2& position, const vec2& size, const Color& c1, const Color& c2)
{
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[4];
    verticies[0] = { position.x, position.y, 0.f, c2.to_dword() };
    verticies[1] = { position.x + size.x, position.y, 0.f,c2.to_dword() };
    verticies[2] = { position.x, position.y + size.y, 0.f, c1.to_dword() };
    verticies[3] = { position.x + size.x, position.y + size.y, 0.f, c1.to_dword() };

    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 4);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_rectangle(const vec3& position, const vec3& size, const Color& color)
{
    const DWORD d_color = color.to_dword();
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[4];
    verticies[0] = { position.x, position.y, position.z,  d_color };
    verticies[1] = { position.x + size.x, position.y, position.z,  d_color };
    verticies[2] = { position.x, position.y + size.y, position.z,  d_color };
    verticies[3] = { position.x + size.x, position.y + size.y,position.z,  d_color };

    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 4);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_rectangle(const vec3& position, const vec3& size, const Color& c1, const Color& c2, const Color& c3, const Color& c4)
{
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[4];
    verticies[0] = { position.x, position.y, position.z,  c1.to_dword() };
    verticies[1] = { position.x + size.x, position.y, position.z,  c2.to_dword() };
    verticies[2] = { position.x, position.y + size.y, position.z,  c3.to_dword() };
    verticies[3] = { position.x + size.x, position.y + size.y, position.z,  c4.to_dword() };

    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 4);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_rectangle(const vec3& position, const vec3& size, const Color& c1, const Color& c2)
{
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[4];
    verticies[0] = { position.x, position.y, position.z, c2.to_dword() };
    verticies[1] = { position.x + size.x, position.y, position.z,c2.to_dword() };
    verticies[2] = { position.x, position.y + size.y, position.z, c1.to_dword() };
    verticies[3] = { position.x + size.x, position.y + size.y, position.z, c1.to_dword() };

    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 4);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_triangle(const vec2& p1, const vec2& p2, const vec2& p3, const Color& color)
{
    const DWORD d_color = color.to_dword();
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[3];
    verticies[0] = { p1.x, p1.y, 0.f,  d_color };
    verticies[1] = { p2.x, p2.y, 0.f,  d_color };
    verticies[2] = { p3.x, p3.y, 0.f,  d_color };

    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 3);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_triangle(const vec2& p1, const vec2& p2, const vec2& p3, const Color& color1, const Color& color2, const Color& color3)
{
  
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[3];
    verticies[0] = { p1.x, p1.y, 1.f,  color1.to_dword() };
    verticies[1] = { p2.x, p2.y, 1.f,  color2.to_dword() };
    verticies[2] = { p3.x, p3.y, 1.f, color3.to_dword() };

    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 3);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_triangle(const vec2& p1, const vec2& p2, const vec2& p3, Color colors[3])
{
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[3];
    verticies[0] = { p1.x, p1.y, 1.f,  colors[0].to_dword() };
    verticies[1] = { p2.x, p2.y, 1.f,  colors[0].to_dword() };
    verticies[2] = { p3.x, p3.y, 1.f,  colors[0].to_dword() };

    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 3);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_triangle(const std::pair<const vec2&, const Color&>& p1, const std::pair<const vec2&, const Color&>& p2, const std::pair<const vec2&, const Color&>& p3)
{
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[3];
    verticies[0] = { p1.first.x,p1.first.y, 1.f, p1.second.to_dword() };
    verticies[1] = { p2.first.x,p2.first.y, 1.f,  p2.second.to_dword() };
    verticies[2] = { p3.first.x,p3.first.y, 1.f,  p3.second.to_dword() };

    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 3);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_triangle(const vec3& p1, const vec3& p2, const vec3& p3, const Color& color)
{
    const DWORD d_color = color.to_dword();
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[3];
    verticies[0] = { p1.x, p1.y, p1.z,  d_color };
    verticies[1] = { p2.x, p2.y, p2.z,  d_color };
    verticies[2] = { p3.x, p3.y, p3.z,  d_color };

    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 3);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_triangle(const vec3& p1, const vec3& p2, const vec3& p3, const Color& color1, const Color& color2, const Color& color3)
{
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[3];
    verticies[0] = { p1.x, p1.y, p1.z,  color1.to_dword() };
    verticies[1] = { p2.x, p2.y, p2.z,  color2.to_dword() };
    verticies[2] = { p3.x, p3.y, p3.z, color3.to_dword() };

    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 3);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_triangle(const vec3& p1, const vec3& p2, const vec3& p3, Color colors[3])
{
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[3];
    verticies[0] = { p1.x, p1.y, p1.z,  colors[0].to_dword() };
    verticies[1] = { p2.x, p2.y, p2.z,  colors[0].to_dword() };
    verticies[2] = { p3.x, p3.y, p3.z,  colors[0].to_dword() };

    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 3);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_triangle(const std::pair<const vec3&, const Color&>& p1, const std::pair<const vec3&, const Color&>& p2, const std::pair<const vec3&, const Color&>& p3)
{
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[3];
    verticies[0] = { p1.first.x,p1.first.y,p1.first.z, p1.second.to_dword() };
    verticies[1] = { p2.first.x,p2.first.y,p2.first.z,  p2.second.to_dword() };
    verticies[2] = { p3.first.x,p3.first.y,p3.first.z,  p3.second.to_dword() };

    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 3);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_line(const vec2& position, const vec2& end_position, const Color& color)
{
    const DWORD d_color = color.to_dword();
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[2];
    verticies[0] = { position.x, position.y, 0.f,  d_color };
    verticies[1] = { end_position.x,end_position.y, 0.f,  d_color };
    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 2);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_LINELIST, 0, 1);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_line(const vec3& position, const vec3& end_position, const Color& color, const Color& color2)
{
    CUSTOMVERTEX* verticies = new CUSTOMVERTEX[2];
    verticies[0] = { position.x, position.y,position.z,  color.to_dword() };
    verticies[1] = { end_position.x,end_position.y, end_position.z,  color2.to_dword() };
    void* p_lock = nullptr;
    this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
    memcpy(p_lock, verticies, sizeof(CUSTOMVERTEX) * 2);
    this->vertex_buffer->Unlock();
    p_lock = nullptr;
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->DrawPrimitive(D3DPT_LINELIST, 0, 1);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[]verticies;
    verticies = nullptr;
}
void renderer::draw_circle(const vec3& pos, const float& radius,const int steps, const Color& color)
{
    float value = 2.0f * D3DX_PI / (float)steps;
    float x_position = pos.x;
    float y_position = pos.y;
    float old_x = pos.x;
    float old_y = pos.y - radius;
    const DWORD col = color.to_dword();
    void* p_lock = nullptr;
    CUSTOMVERTEX vertecies[3];
    for (float i = 0.0f; i <= 2.0f * D3DX_PI + value; i += value)
    {
        x_position = pos.x + radius * sin(i);
        y_position = pos.y - radius * cos(i);
        vertecies[0] = { x_position, y_position, pos.z, col };
        vertecies[1] = { pos.x, pos.y, pos.z, col };
        vertecies[2] = { old_x, old_y, pos.z, col };     
        this->vertex_buffer->Lock(0, 0, (void**)&p_lock, 0);
        memcpy(p_lock, &vertecies, sizeof(CUSTOMVERTEX) * 3);
        this->vertex_buffer->Unlock();
        this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
        this->d3d9_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
        this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
        p_lock = nullptr;
        old_x = x_position;
        old_y = y_position;
    } 
}
void renderer::draw_cube(const vec3& pos, const Color& color)
{
    const DWORD bottom =color.to_dword();
   
        CUSTOMVERTEX vertices[] ={
        {pos.x, pos.y, pos.z, bottom},
        {pos.x, -pos.y, pos.z, bottom},//1
        {-pos.x, pos.y, pos.z, bottom},
        {-pos.x, -pos.y, pos.z, bottom},//3
        {-pos.x, pos.y, -pos.z, bottom},
        {-pos.x, -pos.y, -pos.z, bottom},//5
        {pos.x, pos.y, -pos.z, bottom},
        {pos.x, -pos.y, -pos.z, bottom},
       
    };
    short indices[] ={
       /*front*/
   1,0,2,
   3,1,2,
   /*left */
   1,0,6,
   7,1,6,
   /**/
   3,2,4,
    4,5,3,
    /**/
    1,7,5,
    3,1,5,
    /**/
    0,6,4,
    2,0,4,
    /**/
    7,6,4,
    5,7,4,
    };
    void* plock = nullptr;
    this->index_buffer->Lock(0, 0, reinterpret_cast<void**>(&plock), 0);
    memcpy(plock, &indices, sizeof(indices));
    this->index_buffer->Unlock();
    this->vertex_buffer->Lock(0, 0, reinterpret_cast<void**>(&plock), 0);
    memcpy(plock, &vertices, sizeof(vertices));
    this->vertex_buffer->Unlock();
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->SetIndices(this->index_buffer.get());   
    this->d3d9_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,0,8,0,12);
    this->d3d9_device->SetIndices(nullptr);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    plock = nullptr;
}
void renderer::draw_pyramide(const vec3& pos, const Color& color)
{
    const DWORD color_d = color.to_dword();
    CUSTOMVERTEX vertices[] = {
        {pos.x, 0, pos.z, color_d},
        {pos.x, 0, -pos.z, color_d},//1
        {-pos.x, 0, -pos.z, color_d},
        {-pos.x, 0, pos.z,  color_d},//3

        {0, pos.y,0,  color_d},
    };
    short indices[] = {
    0,1,4,
    0,4,3,
    3,4,2,
    2,4,1,
    1,2,0,
    2,3,0,
    };
    void* plock = nullptr;
    this->index_buffer->Lock(0, 0, reinterpret_cast<void**>(&plock), 0);
    memcpy(plock, &indices, sizeof(indices));
    this->index_buffer->Unlock();
    this->vertex_buffer->Lock(0, 0, reinterpret_cast<void**>(&plock), 0);
    memcpy(plock, &vertices, sizeof(vertices));
    this->vertex_buffer->Unlock();
    this->d3d9_device->SetStreamSource(0, this->vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    this->d3d9_device->SetIndices(this->index_buffer.get());
    this->d3d9_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 5, 0, 6);
    this->d3d9_device->SetIndices(nullptr);
    this->d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    plock = nullptr;
}
void renderer::draw_poly(vec3* vertices, int vertices_num, int poly_num, const Color& color)
{
    assert(vertices_num < max_vertex_count);//vertex count check
    const DWORD d_color = color.to_dword();
    CUSTOMVERTEX* c_vertex = new CUSTOMVERTEX[vertices_num];
    for (int i = 0; i < vertices_num;i++)
    {
        c_vertex[i].x = vertices[i].x;
        c_vertex[i].y = vertices[i].y;
        c_vertex[i].z = vertices[i].z;
        c_vertex[i].color = d_color;
    }
    void* pointer = nullptr;
    vertex_buffer->Lock(0, 0, reinterpret_cast<void**>(&pointer), 0);//get pointer for vertex buffer
    memcpy(pointer, c_vertex, sizeof(CUSTOMVERTEX) * vertices_num);//copy c_vertex data to vertex buffer
    vertex_buffer->Unlock();
    d3d9_device->SetStreamSource(0, vertex_buffer.get(), 0, sizeof(CUSTOMVERTEX));
    d3d9_device->DrawPrimitive(current_poly_primitive, 0, poly_num);
    d3d9_device->SetStreamSource(0, nullptr, 0, 0);
    delete[] c_vertex;
    pointer = nullptr;
}

void renderer::set_poly_type(const D3DPRIMITIVETYPE& type)
{
    this->current_poly_primitive = type;
}

void renderer::set_poly_color(const Color& color)
{
    this->poly_color = color.to_dword();
}
