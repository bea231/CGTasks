#ifndef __CAR_INCLUDED__
#define __CAR_INCLUDED__

#include <d3d9.h>
#include "meshes.h"
#include "lights.h"
#include "Math/cglMath.h"

class airplane_t
{
public:
  airplane_t( LPDIRECT3DDEVICE9 device )
  {
    m_airplane_geometry.load( L"Res/airplane00.x", device );

    transform( transform_t().rotate_y(90).translate( 0, 1.6f, 0 ) );
  }

  void transform( transform_t &t )
  {
    m_airplane_geometry.transform( t );
  }

  void render( LPDIRECT3DDEVICE9 device )
  {
    m_airplane_geometry.render( device );
  }
private:
  x_mesh_t m_airplane_geometry;
};

#endif /* __CAR_INCLUDED__ */
