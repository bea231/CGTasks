/**
  @file     myApp.cpp
  @brief    User's application class
  @date     Created on 10/09/2005
  @project  D3DBase
  @author   Bvs
*/

// *******************************************************************
// includes

#include <vector>
#include <math.h>
#include <stdlib.h>

#include <windows.h>
#include <windowsx.h>
#include <zmouse.h>

#include "myApp.h"
#include "../Library/cglD3D.h"

#include <d3dx9math.h>

// *******************************************************************
// defines

namespace
{
  const float s_rMouseMove2Rotate = 0.3f;
  const float s_rMouseWheel2Zoom = 0.03f;
  const float s_rKbd2Rotate = 100.16f;
  const float s_rKbd2Move = 140.f;
  const float s_rKbd2Zoom = 30.16f;
  const float s_rCarRotate = 30.f;
}


static int s_mipmap[3] = {D3DTEXF_POINT, D3DTEXF_LINEAR, D3DTEXF_NONE};
static int s_minmag[2] = {D3DTEXF_POINT, D3DTEXF_LINEAR};

// *******************************************************************
// Methods

float rnd( float a = 0, float b = 1 )
{
  return rand() / (float)RAND_MAX * (b - a) + a;
}

myApp::myApp( int nW, int nH, void* hInst, int nCmdShow )
  : cglApp(nW, nH, hInst, nCmdShow)
  , m_nPrevMouseX(-100)
  , m_nPrevMouseY(-100)
  , m_is_wireframe(false)
  , m_is_fixed_camera(false)
  , m_mipmap_index(0)
  , m_min_index(0)
  , m_mag_index(0)
  , m_bias(0)
  , m_font(0)
  , m_mouse_dx(0)
  , m_mouse_dy(0)
  , m_mouse_dr(0)
{
  for (int i = 0; i < MAX_KEYS; i++)
    m_keysPressed[i] = false;
  m_nClearColor = 0xFF111111;
  m_camera.set_camera(vec_t(100, 100, -100.f), vec_t(0.f), vec_t(0.f, 1.f, 0.f), true);
  m_camera.set_near_far(0.5, 10000.f);

  IDirect3DDevice9 *device  = m_pD3D->getDevice();
  device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  device->SetRenderState(D3DRS_LIGHTING, false);

  device->SetSamplerState( 0, D3DSAMP_MIPFILTER, s_mipmap[m_mipmap_index] );

  /*** Add units to render ***/
  base_geometry_t * plane = new base_geometry_t(device, 2, 2, VerticesFactory(), color_t(0.8f, 0, 0));
  plane->transform().translate(-0.5f, -0.5f, 0).rotate_x(-90).scale(120);
  m_units.push_back(plane);

  for (size_t i = 0; i < 20; ++i)
  {
    float const r = rnd(0.1f);
    float const g = rnd(0.1f);
    float const b = rnd(0.1f);

    float const x = rnd(-45, 30);
    float const y = rnd(5, 30);
    float const z = rnd(-45, 30);
    float const radius = rnd(1, 7);

    material_t const m(0.01f, 1, 1 - radius / 10.f, 4 * radius);

    base_geometry_t * sphere = new base_geometry_t( device, 40, 30, SphereFactory(), color_t(r, g, b), m);
    sphere->transform().scale(radius).translate(x, y, z);
    m_units.push_back( sphere );
  }
  base_geometry_t * sphere = new base_geometry_t( device, 40, 30, SphereFactory());
  sphere->transform().scale(30).translate(60, 50, 60 );
  m_units.push_back( sphere ); 

  m_light_transform.set_unit().translate(-0.5f, -0.5f, 0).scale(30).rotate_x(-45).rotate_y(45).translate(35, 25, 35);
  base_geometry_t * light_plane = new base_geometry_t(device, 2, 2, VerticesFactory());
  light_plane->transform().transform(m_light_transform);
  m_units.push_back(light_plane);

  ID3DXBuffer *errors;
  HRESULT hr = D3DXCreateEffectFromFile(device, L"Res/area_light.fx", NULL, NULL, D3DXSHADER_DEBUG, NULL, &m_area_light_effect, &errors);
  if (hr != D3D_OK || errors)
  {
     MessageBox(NULL, L"Cannot load effect", L"ERROR", MB_ICONERROR | MB_OK);
     PostQuitMessage(EXIT_FAILURE);
  }

  D3DVERTEXELEMENT9 decl[] = {
     {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
     {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
     {0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
     {0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
     D3DDECL_END()
  };
  device->CreateVertexDeclaration(decl, &m_vertex_declaration);
}

bool myApp::processInput(unsigned int nMsg, int wParam, long lParam)
{
  // Handle input
  switch (nMsg)
  {
    // Process mouse move message
    case WM_MOUSEMOVE:
    {
      // Get mouse x & y (to the upper-left corner of the client area)
      int xPos = GET_X_LPARAM(lParam); 
      int yPos = GET_Y_LPARAM(lParam); 

      if ((wParam & MK_LBUTTON) != 0 && m_nPrevMouseX >= 0) // 
      {
        m_mouse_dx += s_rMouseMove2Rotate * (xPos - m_nPrevMouseX);
        m_mouse_dy += s_rMouseMove2Rotate * (yPos - m_nPrevMouseY);
      }
      
      m_nPrevMouseX = xPos;
      m_nPrevMouseY = yPos;
      break;
    }

    // Process mouse wheel message
    case WM_MOUSEWHEEL:
    {
      int zDelta = (int)((signed short)(HIWORD(wParam)));
      m_mouse_dr += zDelta * s_rMouseWheel2Zoom;
      break;
    }
    
    case WM_KEYDOWN:
    {
      // Save flags for continuous controls
      if (wParam < MAX_KEYS)
        m_keysPressed[wParam] = true;
      switch (wParam)
      {
      case VK_SPACE:
        m_is_fixed_camera = !m_is_fixed_camera;
        m_camera.look_at = vec_t(0, 0, 0);
        m_camera.up = vec_t(0, 1, 0);
        m_camera.update_look_at_loc_up();
        break;
      case 'W':
        m_camera.move_forward(s_rKbd2Move * m_timer.getDelta());
        break;
      case 'S':
        m_camera.move_forward(-s_rKbd2Move * m_timer.getDelta());
        break;
      case 'A':
        m_camera.move_right(s_rKbd2Move * m_timer.getDelta());
        break;
      case 'D':
        m_camera.move_right(-s_rKbd2Move * m_timer.getDelta());
        break;
      case 'E':
        m_camera.move_up(s_rKbd2Move * m_timer.getDelta());
        break;
      case 'Q':
         m_camera.move_up(-s_rKbd2Move * m_timer.getDelta());
         break;
      case 'M':
        m_mipmap_index = (m_mipmap_index + 1) % 3;
        m_pD3D->getDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, s_mipmap[m_mipmap_index]);
        break;
      case 'F':
        m_min_index = (m_min_index + 1) % 2;
        m_pD3D->getDevice()->SetSamplerState( 0, D3DSAMP_MINFILTER , s_minmag[m_min_index] );
        break;
      case 'G':
        m_mag_index = (m_mag_index + 1) % 2;
        m_pD3D->getDevice()->SetSamplerState( 0, D3DSAMP_MAGFILTER, s_minmag[m_mag_index] );
        break;
      case VK_F1:
        m_is_wireframe = !m_is_wireframe;
        m_pD3D->getDevice()->SetRenderState(D3DRS_FILLMODE, m_is_wireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
        break;
      case VK_ADD:
      case VK_OEM_PLUS:
        if (m_keysPressed[VK_SHIFT])
        {
          m_bias += 0.2f;
          m_pD3D->getDevice()->SetSamplerState( 0, D3DSAMP_MIPMAPLODBIAS, *reinterpret_cast<DWORD *>(&m_bias) );
        }
        break;
      case VK_OEM_MINUS:
      case VK_SUBTRACT:
        if (m_keysPressed[VK_SHIFT])
        {
          m_bias -= 0.2f;
          m_pD3D->getDevice()->SetSamplerState( 0, D3DSAMP_MIPMAPLODBIAS, *reinterpret_cast<DWORD *>(&m_bias) );
        }
        break;
      }
      break;
    }

    case WM_KEYUP:
    {
      // Save flags for continuous controls
      if (wParam < MAX_KEYS)
        m_keysPressed[wParam] = false;
      break;
    }
  }
  
  return cglApp::processInput(nMsg, wParam, lParam);
}

void myApp::rotate()
{
  if (m_is_fixed_camera)
  {
    if (cglmath::Abs(m_mouse_dx) > 0.001)
      m_camera.horizontal_rotate_round_look_at(m_mouse_dx);
    if (cglmath::Abs(m_mouse_dy) > 0.001)
      m_camera.vertical_rotate_round_look_at(m_mouse_dy);
  }
  else
  {
    m_camera.rotate_right(m_mouse_dy / 2);
    m_camera.rotate_y(-m_mouse_dx / 2);
  }

  m_mouse_dx = 0;
  m_mouse_dy = 0;
}

void myApp::zoom()
{
  if (m_is_fixed_camera)
    m_camera.move_to_look_at(m_mouse_dr, 1);
  m_mouse_dr = 0;
}

void myApp::draw_axis( IDirect3DDevice9 *device )
{
  float const axis_len = 1000;
  struct axis_vertex_t
  {
     vec_t v;
     DWORD color;
  } static const axis[6] =
  {
     { vec_t(0, 0, 0), 0xFFFF0000 },
     { vec_t(axis_len, 0, 0), 0xFFFF0000 },
     { vec_t(0, 0, 0), 0xFF00FF00 },
     { vec_t(0, axis_len, 0), 0xFF00FF00 },
     { vec_t(0, 0, 0), 0xFF0000FF },
     { vec_t(0, 0, axis_len), 0xFF0000FF },
  };
  device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)m_camera.get_projection_matrix().M);
  device->SetTransform(D3DTS_VIEW, (D3DMATRIX *)m_camera.get_view_matrix().M);
  device->SetTransform(D3DTS_WORLD, (D3DMATRIX *)matrix_t::UnitMatrix);
  device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
  device->DrawPrimitiveUP(D3DPT_LINELIST, 3, axis, sizeof(axis_vertex_t));
}

void myApp::renderInternal()
{
  IDirect3DDevice9 *device = m_pD3D->getDevice();

  draw_axis(device);

  HRESULT hr;
  device->SetVertexDeclaration(m_vertex_declaration);
  hr = m_area_light_effect->SetTechnique("area_light_technique");
  hr = m_area_light_effect->SetMatrix("u_view_projection_matrix", (D3DXMATRIX *)(m_camera.get_view_matrix() * m_camera.get_projection_matrix()).M);
  vec_t const cam = m_camera.location;
  hr = m_area_light_effect->SetVector("u_camera_pos", &(D3DXVECTOR4(cam.x, cam.y, cam.z, 1)));

  hr = m_area_light_effect->SetMatrix("u_light_plane_matrix", (D3DXMATRIX *)m_light_transform.matrix.M);
  hr = m_area_light_effect->SetMatrix("u_light_plane_inv_matrix", (D3DXMATRIX *)m_light_transform.inv_matrix.M);

  recursive_data_t rd(device, m_area_light_effect, m_camera, m_timer, transform_t());

  size_t num_passes = 0;
  m_area_light_effect->Begin(&num_passes, 0);
  for(size_t i = 0; i < num_passes; ++i)
  { 
    m_area_light_effect->BeginPass(i);
    for (unit_iterator_t it = m_units.begin(); it != m_units.end(); ++it)
      (*it)->treat_as_unit(rd);
    m_area_light_effect->EndPass();
  }
  m_area_light_effect->End();
}

void myApp::update()
{
  // Call predecessor update
  cglApp::update();

  rotate();
  zoom();
}

void myApp::print_text( char *text, long x1, long y1, long x2, long y2, D3DCOLOR color )
{
  RECT rect;

  if (m_font == NULL)
    D3DXCreateFont( m_pD3D->getDevice(), 17, 0, FW_NORMAL, 4, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, TEXT( "Times New Roman" ), &m_font );

  rect.left = x1;
  rect.top = y1;
  rect.right = x2;
  rect.bottom = y2;
  // Вывод текста
  m_font->DrawTextA( NULL, text, -1, &rect, DT_LEFT, color );
}