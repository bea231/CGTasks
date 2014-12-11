/**
@file     cglapp.h
@brief    Class cglApp definition
@date     Created on 10/09/2005
@project  D3DBase
@author   Bvs
*/

#ifndef __MYLAPP_H__632619789336566350
#define __MYLAPP_H__632619789336566350

// *******************************************************************
// includes

#include <d3dx9.h>

#include "../Library/cglApp.h"
#include "Math/cglMath.h"
#include "geometry.h"
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
    if (m_font)
      m_font->Release();
    for (unit_iterator_t it = m_units.begin(); it != m_units.end(); ++it)
        delete (*it);
    if (m_vertex_declaration)
      m_vertex_declaration->Release();
    if (m_area_light_effect)
      m_area_light_effect->Release();
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
  
  void rotate();
  void zoom();

  float m_mouse_dx;
  float m_mouse_dy;
  float m_mouse_dr;

  camera_t m_camera;

  typedef std::list<IAnimationUnit *>::iterator unit_iterator_t;
  std::list<IAnimationUnit *> m_units;

  int m_mipmap_index;
  int m_min_index;
  int m_mag_index;

  bool m_is_wireframe;
  bool m_is_fixed_camera;

  float m_bias;

  transform_t m_light_transform;

  ID3DXEffect *m_area_light_effect;
  IDirect3DVertexDeclaration9 *m_vertex_declaration;

private:
  ID3DXFont * m_font;
  void print_text( char *text, long x1, long y1, long x2, long y2, D3DCOLOR color );
  void draw_axis( IDirect3DDevice9 *device );
};


// *******************************************************************
// functions 

#endif //__CGLAPP_H__632619789336566350

