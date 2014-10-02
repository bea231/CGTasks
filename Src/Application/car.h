#ifndef __CAR_INCLUDED__
#define __CAR_INCLUDED__

#include <d3d9.h>
#include "meshes.h"
#include "lights.h"
#include "Math/cglMath.h"

class car_t
{
public:
  car_t( LPDIRECT3DDEVICE9 device, unsigned int light1, unsigned int light2 )
     : m_headlight_right( vec_t( 8.5f, -0.3f, 2 ), vec_t( 1, 0, 0 ), cglmath::Deg2Rad( 25.f ), cglmath::Deg2Rad( 35.f ) )
     , m_headlight_left( vec_t( 8.5f, -0.3f, -2 ), vec_t( 1, 0, 0 ), cglmath::Deg2Rad( 25.f ), cglmath::Deg2Rad( 35.f ) )
  {
    m_car_geometry.load( L"Res/car00.x", device );

    m_car_geometry.transform( transform_t().rotate_z( 180 ) );
    transform( transform_t().translate( 0, 2.6f, 0 ) );

    m_headlight_left.set_ambient( color_t( 1, 1, 0 ) );
    m_headlight_left.set_diffuse( color_t( 1, 1, 0 ) );
    m_headlight_left.set_specular( color_t( 1, 1, 0 ) );
    m_headlight_left.set_attenuation1( 0.1f );
    m_headlight_left.set( device, light1 );
    m_headlight_left.enable( device );

    m_headlight_right.set_ambient( color_t( 1, 1, 0 ) );
    m_headlight_right.set_diffuse( color_t( 1, 1, 0 ) );
    m_headlight_right.set_specular( color_t( 1, 1, 0 ) );
    m_headlight_right.set_attenuation1( 0.1f );
    m_headlight_right.set( device, light2 );
    m_headlight_right.enable( device );
  }

  void transform( transform_t &t )
  {
    m_car_geometry.transform( t );
    m_headlight_left.transform( t );
    m_headlight_right.transform( t );
  }

  void render( LPDIRECT3DDEVICE9 device )
  {
    m_headlight_left.update( device );
    m_headlight_right.update( device );
    m_car_geometry.render( device );
  }

  void change_headlights_state( LPDIRECT3DDEVICE9 device )
  {
    m_headlight_left.set_state(device, !m_headlight_left.get_state());
    m_headlight_right.set_state(device, !m_headlight_right.get_state());
    m_car_geometry.render( device );
  }
private:
  x_mesh_t m_car_geometry;
  spot_light_t m_headlight_right;
  spot_light_t m_headlight_left;
};

#endif /* __CAR_INCLUDED__ */
