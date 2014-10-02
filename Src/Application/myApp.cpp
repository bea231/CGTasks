/**
  @file     myApp.cpp
  @brief    User's application class
  @date     Created on 10/09/2005
  @project  D3DBase
  @author   Bvs
*/

// *******************************************************************
// includes

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
  const float s_rKbd2Move = 100.f;
  const float s_rKbd2Zoom = 100.16f;
  const float s_rCarRotate = 30.f;
}

// *******************************************************************
// Methods

myApp::myApp( int nW, int nH, void* hInst, int nCmdShow )
  : cglApp(nW, nH, hInst, nCmdShow)
  , m_nPrevMouseX(-100)
  , m_nPrevMouseY(-100)
  , m_is_wireframe(false)
  , m_is_fixed_camera(false)
  , m_plane(m_pD3D->getDevice(), 500, 500, VerticesFactory())
  , m_car(m_pD3D->getDevice(), 0, 1)
  , m_point_light(vec_t(0, 2, 0))
  , m_sphere(m_pD3D->getDevice(), 25, 30, SphereFactory())
{
  for (int i = 0; i < MAX_KEYS; i++)
    m_keysPressed[i] = false;
  m_nClearColor = 0xFF222222;
  m_camera.set_camera(vec_t(50, 25, 5.f), vec_t(0.f), vec_t(0.f, 1.f, 0.f), true);
  m_camera.set_near_far(0.5, 10000.f);

  IDirect3DDevice9 *device  = m_pD3D->getDevice();
  device->SetRenderState(D3DRS_ZENABLE, true);
  device->SetRenderState(D3DRS_LIGHTING, true);
  device->SetRenderState(D3DRS_SPECULARENABLE, true);
  device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  device->SetRenderState(D3DRS_NORMALIZENORMALS, true);

  m_car.transform(transform_t().translate(0, 0, 10));

  m_sphere.transform(transform_t().scale(0.1f).translate(0, 2, 0));

  m_plane.transform(transform_t().rotate_x(-90).translate(-0.5f, 0, 0.5f).scale(50));
  m_geometry_material.Ambient = *reinterpret_cast<D3DCOLORVALUE *>(&color_t(0.f));
  m_geometry_material.Diffuse = *reinterpret_cast<D3DCOLORVALUE *>(&color_t(0.f, 0.0f, 0));
  m_geometry_material.Specular = *reinterpret_cast<D3DCOLORVALUE *>(&color_t(0.f));
  m_geometry_material.Emissive = *reinterpret_cast<D3DCOLORVALUE *>(&color_t(0.f));
  m_geometry_material.Power = 0;

  m_direction_light.set_direction(vec_t(-10, -5, 0).normalize());
  m_direction_light.set_ambient(color_t(0.f));
  m_direction_light.set_diffuse(color_t(0.7f, 0, 0));
  m_direction_light.set_specular(color_t(0.7f, 0, 0));
  m_direction_light.set(device, 2);
  m_direction_light.enable(device);

  m_point_light.set_ambient(color_t(0, 1, 0));
  m_point_light.set_diffuse(color_t(0, 1, 0));
  m_point_light.set_specular(color_t(0, 1, 0));
  m_point_light.set_attenuation1(0.1f);
  m_point_light.set_attenuation2(0.01f);
  m_point_light.set(device, 3);
  m_point_light.enable(device);
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
      zoom(zDelta * s_rMouseWheel2Zoom);
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
      case '1':
        m_direction_light.set_state( m_pD3D->getDevice(), !m_direction_light.get_state() );
        break;
      case '2':
        m_point_light.set_state( m_pD3D->getDevice(), !m_point_light.get_state() );
        break;
      case '3':
        m_car.change_headlights_state( m_pD3D->getDevice() );
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
  vec_t pos(5, 5, 5), at(0, 0, 0), up(0, 1, 0);
  float const axis_len = 1000;
  base_geometry_t::vertex_t const axis[6] =
  {
     { vec_t(0, 0, 0), vec_t(-1, 0, 0), 0xFFFF0000 },
     { vec_t(axis_len, 0, 0), vec_t(-1, 0, 0), 0xFFFF0000 },
     { vec_t(0, 0, 0), vec_t(-1, 0, 0), 0xFF00FF00 },
     { vec_t(0, axis_len, 0), vec_t(-1, 0, 0), 0xFF00FF00 },
     { vec_t(0, 0, 0), vec_t(-1, 0, 0), 0xFF0000FF },
     { vec_t(0, 0, axis_len), vec_t(-1, 0, 0), 0xFF0000FF },
  };

  
  IDirect3DDevice9 *device = m_pD3D->getDevice();
  device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)m_camera.get_projection_matrix().M);
  device->SetTransform(D3DTS_VIEW, (D3DMATRIX *)m_camera.get_view_matrix().M);
  device->SetTransform(D3DTS_WORLD, (D3DMATRIX *)matrix_t::UnitMatrix);

  device->SetRenderState(D3DRS_FILLMODE, m_is_wireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID);

  device->SetRenderState(D3DRS_LIGHTING, false);
  device->SetFVF(base_geometry_t::c_FVF);
  device->DrawPrimitiveUP(D3DPT_LINELIST, 3, axis, sizeof(base_geometry_t::vertex_t));
  device->SetRenderState(D3DRS_LIGHTING, true);

  m_car.render(device);

  device->SetMaterial(&m_geometry_material);
  m_plane.render(device);
  m_sphere.render(device);
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

  // Update object rotation
  m_car.transform(transform_t().rotate_y(s_rCarRotate * m_timer.getDelta()));
}