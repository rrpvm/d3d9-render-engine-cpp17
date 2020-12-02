#include "renderer.hpp"
#include <d3d9types.h>
#include <d3dx9math.h>
bool renderer::initialize(const HWND& hwnd, const vec2& size)
{
    assert(this->d3d9_object.get());
    D3DPRESENT_PARAMETERS d3dpp{};
    d3dpp.BackBufferWidth = (int)size.x;
    d3dpp.BackBufferHeight = (int)size.y;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality = 0;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.Windowed = true;
    d3dpp.EnableAutoDepthStencil = false;
    d3dpp.AutoDepthStencilFormat = D3DFMT_A8R8G8B8;
    d3dpp.Flags = 0;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    IDirect3DDevice9* d3d9_tmp = nullptr;
    IDirect3DVertexBuffer9* vertex_buffer_tmp = nullptr;
    HRESULT hr = this->d3d9_object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3d9_tmp);
    assert(d3d9_tmp);
    this->d3d9_device = std::unique_ptr<IDirect3DDevice9>(d3d9_tmp);
    this->d3d9_device->CreateVertexBuffer(1024 * sizeof(CUSTOMVERTEX), 0, CUSTOMFVF | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &vertex_buffer_tmp, nullptr);
    this->vertex_buffer = std::unique_ptr<IDirect3DVertexBuffer9>(vertex_buffer_tmp);
    assert(vertex_buffer_tmp); 
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
  //  this->draw_triangle(vec3(2.5f, -3.0f,90.0f), vec3(0.0f, 3.0f,90.f), vec3(-2.5f, -3.0f,90.0f), Color::red(),Color::green(), Color::white());
    this->draw_rectangle(vec2(2.5f, -3.0f),vec2(5.f, 5.f),Color::white(),Color::red(),Color::green(), Color::gray_b());
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
  //  this->d3d9_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  //  this->d3d9_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
   // this->d3d9_device->SetRenderState(D3DRS_ZENABLE, FALSE);
   // this->d3d9_device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
}
void renderer::set_viewport()
{   
    D3DXMATRIX matView; 
    D3DXMatrixLookAtLH(&matView,
        &D3DXVECTOR3(0.0f, 0.0f, 10.0f),    // the camera position
        &D3DXVECTOR3(0.0f, 0.0f, 0.0f),    // the look-at position
        &D3DXVECTOR3(0.0f, 1.0f, 1.0f));    // the up direction
    d3d9_device->SetTransform(D3DTS_VIEW, &matView);   
    D3DXMATRIX matProjection; 
    D3DXMatrixPerspectiveFovLH(&matProjection,
        D3DXToRadian(90),    // the horizontal field of view
        (FLOAT)1280 / (FLOAT)720, // aspect ratio
        1.0f,    // the near view-plane
        200.0f);    // the far view-plane
    d3d9_device->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection*/
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
}
void renderer::draw_rectangle(const vec3& position, const vec3& size, const Color& c1, const Color& c2, const Color& c3, const Color& c4)
{
}
void renderer::draw_rectangle(const vec3& position, const vec3& size, const Color& c1, const Color& c2)
{
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
