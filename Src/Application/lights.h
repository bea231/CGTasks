/// HEADER

#ifndef __LIGHTS_INCLUDED__
#define __LIGHTS_INCLUDED__

#include <cstring>
#include <d3d9.h>
#include "Math/cglMath.h"

class light_t
{
public:
  light_t() : m_enabled( -1 )
  {
    memset(&m_light, 0, sizeof(D3DLIGHT9));
  }

  virtual ~light_t() = 0 {}

  /* Add light source to device */
  virtual void set( LPDIRECT3DDEVICE9 device, unsigned int index )
  {
    device->SetLight( m_index = index, &m_light );
    m_enabled = false;
  }

  /* Update previously light source to device */
  virtual void update( LPDIRECT3DDEVICE9 device )
  {
    if (m_enabled >= 0)
      device->SetLight( m_index, &m_light );
  }

  virtual void enable( LPDIRECT3DDEVICE9 device )
  {
    if (m_enabled >= 0)
    {
      m_enabled = true;
      device->LightEnable( m_index, m_enabled );
    }
  }

  virtual void disable( LPDIRECT3DDEVICE9 device )
  {
    if (m_enabled >= 0)
    {
      m_enabled = false;
      device->LightEnable( m_index, m_enabled );
    }
  }

  virtual void set_state( LPDIRECT3DDEVICE9 device, bool state )
  {
    if (m_enabled >= 0)
    {
      device->LightEnable( m_index, m_enabled = state );
    }
  }

  virtual bool get_state( void ) const
  {
    //device->GetLightEnable(m_index, &m_enable);
    if (m_enabled >= 0)
       return !!m_enabled;
    return false;
  }
protected:
  D3DLIGHT9 m_light;
  unsigned int m_index;
  char m_enabled;
};

class direction_light_t : public light_t
{
public:
  direction_light_t( vec_t const & direction = vec_t( 0, -1, 0 ),
                     color_t const &ambient = color_t( 1.f ),
                     color_t const &diffuse = color_t( 1.f ),
                     color_t const &specular = color_t( 1.f ) )
  {
    m_light.Type = D3DLIGHT_DIRECTIONAL;
    m_light.Direction = *reinterpret_cast<D3DVECTOR const *>(&direction);
    m_light.Ambient = *reinterpret_cast<D3DCOLORVALUE const *>(&ambient);
    m_light.Diffuse = *reinterpret_cast<D3DCOLORVALUE const *>(&diffuse);
    m_light.Specular = *reinterpret_cast<D3DCOLORVALUE const *>(&specular);
  }

  void set_ambient( color_t const &ambient )
  {
    m_light.Ambient = *reinterpret_cast<D3DCOLORVALUE const *>(&ambient);
  }

  void set_diffuse( color_t const &diffuse )
  {
    m_light.Diffuse = *reinterpret_cast<D3DCOLORVALUE const *>(&diffuse);
  }

  void set_specular( color_t const &specular )
  {
    m_light.Specular = *reinterpret_cast<D3DCOLORVALUE const *>(&specular);
  }

  void set_direction( vec_t const & direction )
  {
    m_light.Direction = *reinterpret_cast<D3DVECTOR const *>(&direction);
  }
};

class spot_light_t : public light_t
{
public:
  spot_light_t( vec_t const & pos,
                     vec_t const & direction,
                     float theta,
                     float phi = 0,
                     float range = 100,
                     float falloff = 1,
                     float attenuation0 = 1,
                     float attenuation1 = 0,
                     float attenuation2 = 0,
                     color_t const &ambient = color_t( 1.f ),
                     color_t const &diffuse = color_t( 1.f ),
                     color_t const &specular = color_t( 1.f ) )
  {
    m_light.Type = D3DLIGHT_SPOT;
    m_light.Theta = theta;
    m_light.Phi = phi;
    m_light.Range = range;
    m_light.Falloff = falloff;
    m_light.Position = *reinterpret_cast<D3DVECTOR const *>(&pos);
    m_light.Direction = *reinterpret_cast<D3DVECTOR const *>(&direction);
    m_light.Ambient = *reinterpret_cast<D3DCOLORVALUE const *>(&ambient);
    m_light.Diffuse = *reinterpret_cast<D3DCOLORVALUE const *>(&diffuse);
    m_light.Specular = *reinterpret_cast<D3DCOLORVALUE const *>(&specular);
    m_light.Attenuation0 = attenuation0;
    m_light.Attenuation1 = attenuation1;
    m_light.Attenuation2 = attenuation2;
  }

  void set_ambient( color_t const &ambient )
  {
    m_light.Ambient = *reinterpret_cast<D3DCOLORVALUE const *>(&ambient);
  }

  void set_diffuse( color_t const &diffuse )
  {
    m_light.Diffuse = *reinterpret_cast<D3DCOLORVALUE const *>(&diffuse);
  }

  void set_specular( color_t const &specular )
  {
    m_light.Specular = *reinterpret_cast<D3DCOLORVALUE const *>(&specular);
  }

  void set_direction( vec_t const & direction )
  {
    m_light.Direction = *reinterpret_cast<D3DVECTOR const *>(&direction);
  }

  void set_position( vec_t const & position )
  {
    m_light.Position = *reinterpret_cast<D3DVECTOR const *>(&position);
  }

  void set_theta( float theta )
  {
    m_light.Theta = theta;
  }

  void set_phi( float phi )
  {
    m_light.Phi = phi;
  }

  void set_falloff( float falloff )
  {
    m_light.Falloff = falloff;
  }

  void set_range( float range )
  {
    m_light.Range = range;
  }

  void set_attenuation0( float attenuation0 )
  {
    m_light.Attenuation0 = attenuation0;
  }

  void set_attenuation1( float attenuation1 )
  {
    m_light.Attenuation1 = attenuation1;
  }

  void set_attenuation2( float attenuation2 )
  {
    m_light.Attenuation2 = attenuation2;
  }

  void transform( transform_t &t )
  {
    vec_t const pos = t.transform_point( *reinterpret_cast<vec_t const *>(&m_light.Position) );
    m_light.Position = *reinterpret_cast<D3DVECTOR const *>(&pos);

    vec_t const direction = t.transform_vector( *reinterpret_cast<vec_t const *>(&m_light.Direction) );
    m_light.Direction = *reinterpret_cast<D3DVECTOR const *>(&direction);
  }
};

class point_light_t : public light_t
{
public:
  point_light_t( vec_t const & pos,
     float range = 100,
     float falloff = 1,
     float attenuation0 = 1,
     float attenuation1 = 0,
     float attenuation2 = 0,
     color_t const &ambient = color_t( 1.f ),
     color_t const &diffuse = color_t( 1.f ),
     color_t const &specular = color_t( 1.f ) )
  {
    m_light.Type = D3DLIGHT_POINT;
    m_light.Range = range;
    m_light.Falloff = falloff;
    m_light.Position = *reinterpret_cast<D3DVECTOR const *>(&pos);
    m_light.Ambient = *reinterpret_cast<D3DCOLORVALUE const *>(&ambient);
    m_light.Diffuse = *reinterpret_cast<D3DCOLORVALUE const *>(&diffuse);
    m_light.Specular = *reinterpret_cast<D3DCOLORVALUE const *>(&specular);
    m_light.Attenuation0 = attenuation0;
    m_light.Attenuation1 = attenuation1;
    m_light.Attenuation2 = attenuation2;
  }

  void set_ambient( color_t const &ambient )
  {
    m_light.Ambient = *reinterpret_cast<D3DCOLORVALUE const *>(&ambient);
  }

  void set_diffuse( color_t const &diffuse )
  {
    m_light.Diffuse = *reinterpret_cast<D3DCOLORVALUE const *>(&diffuse);
  }

  void set_specular( color_t const &specular )
  {
    m_light.Specular = *reinterpret_cast<D3DCOLORVALUE const *>(&specular);
  }

  void set_position( vec_t const & position )
  {
    m_light.Position = *reinterpret_cast<D3DVECTOR const *>(&position);
  }

  void set_falloff( float falloff )
  {
    m_light.Falloff = falloff;
  }

  void set_range( float range )
  {
    m_light.Range = range;
  }

  void set_attenuation0( float attenuation0 )
  {
    m_light.Attenuation0 = attenuation0;
  }

  void set_attenuation1( float attenuation1 )
  {
    m_light.Attenuation1 = attenuation1;
  }

  void set_attenuation2( float attenuation2 )
  {
    m_light.Attenuation2 = attenuation2;
  }

  void transform( transform_t &t )
  {
    vec_t const pos = t.transform_point( *reinterpret_cast<vec_t const *>(&m_light.Position) );
    m_light.Position = *reinterpret_cast<D3DVECTOR const *>(&pos);

    vec_t const direction = t.transform_vector( *reinterpret_cast<vec_t const *>(&m_light.Direction) );
    m_light.Direction = *reinterpret_cast<D3DVECTOR const *>(&direction);
  }
};

#endif /* __LIGHTS_INCLUDED__ */
