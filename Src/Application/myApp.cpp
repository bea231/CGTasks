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
#include "Library/cglD3D.h"

#include <d3dx9math.h>

// *******************************************************************
// defines

namespace
{
  const float s_rMouseMove2Rotate = 0.1f;
  const float s_rMouseWheel2Zoom = 0.01f;
  const float s_rKbd2Rotate = 100.16f;
  const float s_rKbd2Zoom = 100.16f;
}

// *******************************************************************
// Methods

myApp::myApp( int nW, int nH, void* hInst, int nCmdShow )
  : cglApp(nW, nH, hInst, nCmdShow)
  , m_nPrevMouseX(-100)
  , m_nPrevMouseY(-100)
  , m_geometry(m_pD3D->getDevice(), 50, 40, TorusFactory(3, 8))
  , m_is_wireframe(false)
{
  for (int i = 0; i < MAX_KEYS; i++)
    m_keysPressed[i] = false;
  m_nClearColor = 0xFF222222;
  m_camera.set_camera(vec_t(50, 25, 5.f), vec_t(0.f), vec_t(0.f, 1.f, 0.f), true);
  m_camera.set_near_far(0.5, 10000.f);

  m_geometry.transform(transform_t().translate(vec_t(10, 10, 10)));

  m_pD3D->getDevice()->SetRenderState(D3DRS_ZENABLE, true);
  m_pD3D->getDevice()->SetRenderState(D3DRS_LIGHTING, false);
  m_pD3D->getDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
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
        m_is_wireframe = !m_is_wireframe;
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
  if (cglmath::Abs(dx) > 0.001)
    m_camera.horizontal_rotate_round_look_at(dx);
  if (cglmath::Abs(dy) > 0.001)
    m_camera.vertical_rotate_round_look_at(dy);
}

void myApp::zoom(float dr)
{
  m_camera.move_to_look_at(dr, 1);
}

void myApp::renderInternal()
{
  D3DXMATRIX proj, view;
  vec_t pos(5, 5, 5), at(0, 0, 0), up(0, 1, 0);
  float const axis_len = 1000;
  base_geometry_t::vertex_t const axis[6] =
  {
    { vec_t(0, 0, 0), 0xFFFF0000 },
    { vec_t(axis_len, 0, 0), 0xFFFF0000 },
    { vec_t(0, 0, 0), 0xFF00FF00 },
    { vec_t(0, axis_len, 0), 0xFF00FF00 },
    { vec_t(0, 0, 0), 0xFF0000FF },
    { vec_t(0, 0, axis_len), 0xFF0000FF },
  };

  m_pD3D->getDevice()->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)m_camera.get_projection_matrix().M);
  m_pD3D->getDevice()->SetTransform(D3DTS_VIEW, (D3DMATRIX *)m_camera.get_view_matrix().M);
  m_pD3D->getDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX *)matrix_t::UnitMatrix);

  m_pD3D->getDevice()->SetRenderState(D3DRS_FILLMODE, m_is_wireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID);

  m_pD3D->getDevice()->SetFVF(base_geometry_t::c_FVF);
  m_pD3D->getDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 3, axis, sizeof(base_geometry_t::vertex_t));

  m_geometry.render(m_pD3D->getDevice());
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

  /// Update object rotation
  m_geometry.transform(transform_t(false).set_rotate_z(dx).rotate_x(dy));
  /*rotate(dx, dy);
  zoom(dr); */
}