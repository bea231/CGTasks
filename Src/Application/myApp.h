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

#include <d3dx9.h>

#include "../Library/cglApp.h"
#include "Math/cglMath.h"
#include "geometry.h"
#include "lights.h"
#include "texture.h"

#include "unit.h"

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
  virtual ~myApp() 
  {
    m_font->Release();
    for (unit_iterator_t it = m_units.begin(); it != m_units.end(); ++it)
        delete (*it);
  }
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

  direction_light_t direction_light;

  typedef std::list<IAnimationUnit *>::iterator unit_iterator_t;
  std::list<IAnimationUnit *> m_units;

  int m_mipmap_index;
  int m_min_index;
  int m_mag_index;

  bool m_is_wireframe;
  bool m_is_fixed_camera;

  float m_bias;

  ID3DXFont * m_font;

  void print_text( char *text, long x1, long y1, long x2, long y2, D3DCOLOR color );
};


// *******************************************************************
// functions 

#endif //__CGLAPP_H__632619789336566350

