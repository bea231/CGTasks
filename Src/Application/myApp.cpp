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
#include <cstdlib>
#include <windows.h>
#include <windowsx.h>
#include <zmouse.h>

#include <cmath>

#include "myApp.h"
#include "../Library/cglD3D.h"

#include <d3dx9math.h>

#include "airplane.h"
#include "flower.h"

// *******************************************************************
// defines

namespace
{
  const float s_rMouseMove2Rotate = 0.3f;
  const float s_rMouseWheel2Zoom = 0.6f;
  const float s_rKbd2Rotate = 100.16f;
  const float s_rKbd2Move = 30.f;
  const float s_rKbd2Zoom = 30.16f;
  const float s_rCarRotate = 30.f;
}


static int s_mipmap[3] = {D3DTEXF_POINT, D3DTEXF_LINEAR, D3DTEXF_NONE};
static int s_minmag[2] = {D3DTEXF_POINT, D3DTEXF_LINEAR};

// *******************************************************************
// Methods

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
{
  for (int i = 0; i < MAX_KEYS; i++)
    m_keysPressed[i] = false;
  m_nClearColor = 0xFF222222;
  m_camera.set_camera(vec_t(5, 5, 5.f), vec_t(0.f), vec_t(0.f, 1.f, 0.f), true);
  m_camera.set_near_far(0.5, 10000.f);

  IDirect3DDevice9 *device  = m_pD3D->getDevice();
  device->SetRenderState(D3DRS_SPECULARENABLE, true);
  device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  device->SetRenderState(D3DRS_NORMALIZENORMALS, true);

  device->SetSamplerState( 0, D3DSAMP_MIPFILTER, s_mipmap[m_mipmap_index] );

  direction_light.set_ambient(color_t(0.1f));
  direction_light.set_diffuse(color_t(0.6f));
  direction_light.set_specular(color_t(0.1f));
  direction_light.set_direction(vec_t(-1, -1, 0.01f));
  direction_light.set(device, 0);
  direction_light.enable(device);

  /*** Add units to render ***/
  m_units.push_back((IAnimationUnit *)new base_plane_t(device));

  m_units.push_back((IAnimationUnit *)new airplane_t(device));

  flower_params_t params;
  params.petal2_height = 0.1f;
  params.petal2_width = 0.03f;
  params.petal2_color = color_t(1, 0, 0);
  params.petal2_angle_min = 5.f;
  params.petal2_angle_max = 20.f;

  params.petal1_height = 0.1f;
  params.petal1_width = 0.1f;
  params.petal1_color = color_t(0x00CC00UL);
  params.petal1_angle_min = 5.f;
  params.petal1_angle_max = 60.f;

  params.receptacle_radius = 0.12f;
  params.receptacle_color = color_t(0xFFCC33UL);

  params.petals_count = 12;
  params.velocity = 1.f;

  params.stem_thickness = 0.03f;
  params.stem_length = 0.7f;
  params.stem_color = color_t(0x003300UL);
  
  for (size_t i = 0; i < 10; ++i)
    for (size_t j = 0; j < 10; ++j)
    {
      params.velocity =  (rand() / (float)RAND_MAX + 0.1f) * 2;
      flower_t *flower = new flower_t(device, params);
      float const x = rand() / (float)RAND_MAX * 40 - 20;
      float const z = rand() / (float)RAND_MAX * 40 - 20;
      flower->transform().translate(x, 0, z);
      m_units.push_back((IAnimationUnit *)flower);
    }
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
        rotate(s_rMouseMove2Rotate * (xPos - m_nPrevMouseX), 
               s_rMouseMove2Rotate * (yPos - m_nPrevMouseY));
      }
      
      m_nPrevMouseX = xPos;
      m_nPrevMouseY = yPos;
      break;
    }

    // Process mouse wheel message
    case WM_MOUSEWHEEL:
    {
      int zDelta = (int)((signed short)(HIWORD(wParam)));
      zoom(zDelta * s_rMouseWheel2Zoom * m_timer.getDelta());
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
      case 'Q':
        m_camera.move_up(-s_rKbd2Move * m_timer.getDelta() );
        break;
      case 'E':
        m_camera.move_up(s_rKbd2Move * m_timer.getDelta() );
        break;
      case 'W':
        m_camera.move_forward( s_rKbd2Move * m_timer.getDelta() );
        break;
      case 'A':
        m_camera.move_right( -s_rKbd2Move * m_timer.getDelta() );
        break;
      case 'S':
        m_camera.move_forward( -s_rKbd2Move * m_timer.getDelta() );
        break;
      case 'D':
        m_camera.move_right( s_rKbd2Move * m_timer.getDelta() );
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

void myApp::rotate(float dx, float dy)
{
  if (m_is_fixed_camera)
  {
    if (cglmath::Abs(dx) > 0.001)
      m_camera.horizontal_rotate_round_look_at(dx);
    if (cglmath::Abs(dy) > 0.001)
      m_camera.vertical_rotate_round_look_at(dy);
  }
  else
  {
    m_camera.rotate_right(dy / 2);
    m_camera.rotate_up(dx / 2);
  }
}

void myApp::zoom(float dr)
{
  if (m_is_fixed_camera)
    m_camera.move_to_look_at(dr, 1);
}

void myApp::renderInternal()
{
  D3DXMATRIX proj, view;
  IDirect3DDevice9 *device = m_pD3D->getDevice();
  device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)m_camera.get_projection_matrix().M);
  device->SetTransform(D3DTS_VIEW, (D3DMATRIX *)m_camera.get_view_matrix().M);

  recursive_data_t rd(device, m_camera, m_timer, transform_t());
  for (unit_iterator_t it = m_units.begin(); it != m_units.end(); ++it)
    (*it)->treat_as_unit(rd);

  char buf[1000] = {0};
  sprintf_s(buf, "MipMap: %s\nMin filter: %s\nMagFilter: %s\nMipMap bias: %f",
             m_mipmap_index == 0 ? "D3DTEXF_POINT" : m_mipmap_index == 1 ? "D3DTEXF_LINEAR" : "D3DTEXF_NONE",
             m_min_index == 0 ? "D3DTEXF_POINT" : "D3DTEXF_LINEAR",
             m_mag_index == 0 ? "D3DTEXF_POINT" : "D3DTEXF_LINEAR",
             m_bias);
  print_text(buf, 0, 0, 1000, 100, color_t(.5f, 0.5f, 0.5f));

  float const axis_len = 1000;
  struct axis_vertex
  {
    vec_t v;
    DWORD color;
  } const axis[6] =
  {
    { vec_t( 0, 0, 0 ), 0xFFFF0000 },
    { vec_t( axis_len, 0, 0 ), 0xFFFF0000 },
    { vec_t( 0, 0, 0 ), 0xFF00FF00 },
    { vec_t( 0, axis_len, 0 ), 0xFF00FF00 },
    { vec_t( 0, 0, 0 ), 0xFF0000FF },
    { vec_t( 0, 0, axis_len ), 0xFF0000FF },
  };
  device->SetTransform(D3DTS_WORLD, (D3DMATRIX *)matrix_t::UnitMatrix);
  device->SetRenderState( D3DRS_LIGHTING, false );
  device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );
  device->DrawPrimitiveUP( D3DPT_LINELIST, 3, axis, sizeof( axis_vertex ) );
  device->SetRenderState( D3DRS_LIGHTING, true );
}

void myApp::update()
{
  // Call predecessor update
  cglApp::update();

  // Process keyboard
  float dx = 0.0f;
  float dy = 0.0f;
  float dr = 0.0f;
  if (m_keysPressed[VK_LEFT])
    dx -= s_rKbd2Rotate * m_timer.getDelta();
  if (m_keysPressed[VK_RIGHT])
    dx += s_rKbd2Rotate * m_timer.getDelta();
  if (m_keysPressed[VK_UP])
    dy -= s_rKbd2Rotate * m_timer.getDelta();
  if (m_keysPressed[VK_DOWN])
    dy += s_rKbd2Rotate * m_timer.getDelta();
  if (m_keysPressed[VK_SUBTRACT])
    dr -= s_rKbd2Zoom * m_timer.getDelta();
  if (m_keysPressed[VK_ADD])
    dr += s_rKbd2Zoom * m_timer.getDelta();

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