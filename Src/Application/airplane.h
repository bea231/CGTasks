#ifndef __AIRPLANE_INCLUDED__
#define __AIRPLANE_INCLUDED__

#include <d3d9.h>
#include "meshes.h"
#include "lights.h"
#include "Math/cglMath.h"
#include "unit.h"

class airplane_t : public IAnimationUnit
{
public:
  airplane_t( LPDIRECT3DDEVICE9 device )
    : m_spot(vec_t(0, 0.6f, 1), vec_t(0, -1, 0), cglmath::Deg2Rad(120.f), cglmath::Deg2Rad(130.f), 500, 1.f)
  {
    x_mesh_t *mesh = new x_mesh_t();

    mesh->load( L"Res/airplane00.x", device );

    m_spot.set_falloff(0.5);
    m_spot.set_attenuation1(0.1f);
    m_spot.set_ambient(color_t(0.01f, 0.01f, 0));
    m_spot.set_diffuse(color_t(1.f, 1.f, 0));
    m_spot.set_specular(color_t(0, 0, 0));
    m_spot.set(device, 1);
    m_spot.enable(device);

    transform().scale(0.1f).rotate_y(90).translate(0, 2.f, 1 );
    *this << IAnimationUnitPtr(mesh);
  } 

  void response( recursive_data_t & rd )
  {
    transform_t t = transform_t().rotate_y(rd.timer.getDelta() * 50).translate(0, sin(rd.timer.getTime() ) * 0.04f, 0);
    transform().transform(t);
    m_spot.transform(t);
    m_spot.update(rd.device);
  }
private:
  spot_light_t m_spot;

};

#endif /* __AIRPLANE_INCLUDED__ */
