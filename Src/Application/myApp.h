#ifndef __MYLAPP_H__632619789336566350
#define __MYLAPP_H__632619789336566350

/**
  @file     cglapp.h
  @brief    Class cglApp definition
  @date     Created on 10/09/2005
  @project  D3DBase
  @author   Bvs
*/

// *******************************************************************
// includes

#include "../Library/cglApp.h"
#include "Math/cglMath.h"
#include "car.h"
#include "geometry.h"
#include "lights.h"

// *******************************************************************
// defines & constants

// *******************************************************************
// classes 

// Application class
class myApp : public cglApp
{
public:
  // Constructor
  myApp(int nW, int nH, void* hInst, int nCmdShow);
  // Destructor
  virtual ~myApp() {}
  // This function performs input processing. Returns true if input is handled
  virtual bool processInput(unsigned int nMsg, int wParam, long lParam);
protected:
  virtual void update();
  virtual void renderInternal();
  
private:
  enum
  {
    MAX_KEYS = 128
  };
 
  int   m_nPrevMouseX;
  int   m_nPrevMouseY;
  bool  m_keysPressed[MAX_KEYS];
  
  void rotate(float dx, float dy);
  void zoom(float dr);

  camera_t m_camera;
  car_t m_car;
  base_geometry_t m_plane;
  base_geometry_t m_sphere;
  D3DMATERIAL9 m_geometry_material;

  direction_light_t m_direction_light;
  point_light_t m_point_light;

  bool m_is_wireframe;
  bool m_is_fixed_camera;
};


// *******************************************************************
// functions 

#endif //__CGLAPP_H__632619789336566350
