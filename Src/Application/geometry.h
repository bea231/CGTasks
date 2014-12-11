/**
  @file     geometry.h
  @brief    Base geometry class definition
  @date     Created on 12/09/2014
  @project  Task1
  @author   Sergeev Artemiy
*/

#ifndef __GEOMETRY_INCLUDED__
#define __GEOMETRY_INCLUDED__

#include <vector>

#include <d3d9.h>
#include "unit.h"
#include "Math/cglMath.h"

#pragma pack(push, 1)
struct material_t
{
  float ambient;
  float diffuse;
  float specular;
  float specular_power;

  material_t( float a = 0, float d = 1, float s = 0, float sp = 8 ) 
    : ambient(a)
    , diffuse(d)
    , specular(s)
    , specular_power(sp)
  {
  }

  void set( ID3DXEffect *effect )
  {
    effect->SetValue("u_material", (void *)this, sizeof(material_t));
  }
};
#pragma pack(pop)

class VerticesFactory
{
public:
  virtual vec_t operator()( float u, float v ) const
  {
    return vec_t(u, v, 0);
  }

  virtual vec_t n( float u, float v ) const
  {
     return vec_t(0, 0, 1.f);
  }
};

class SphereFactory : public VerticesFactory
{
public:
  virtual vec_t operator()(float u, float v) const
  {
    float const phi = (u * 2.f - 1.f) * cglmath::c_pif;
    float const theta = (v - 0.5f) *  cglmath::c_pif;
    float sine_theta, cosine_theta, sine_phi, cosine_phi;

    cglmath::get_sin_cos(phi, sine_phi, cosine_phi);
    cglmath::get_sin_cos(theta, sine_theta, cosine_theta);

    return vec_t(cosine_theta * cosine_phi, sine_theta, cosine_theta * sine_phi);
  }

  virtual vec_t n(float u, float v) const
  {
    return (*this)(u, v);
  }
};

class CylinderFactory : public VerticesFactory
{
public:
  CylinderFactory( float height, float radius )
    : m_height(height)
    , m_radius(radius)
  {

  }
  virtual vec_t operator()(float u, float v) const
  {
    float const phi = (u * 2.f - 1.f) * cglmath::c_pif;
    float sine_phi, cosine_phi;

    cglmath::get_sin_cos(phi, sine_phi, cosine_phi);

    return vec_t(m_radius* cosine_phi, v * m_height, m_radius * sine_phi);
  }

  virtual vec_t n(float u, float v) const
  {
    vec_t vert = (*this)(u, v);
    return vec_t(vert.x, 0, vert.z);
  }
private:
  float m_height;
  float m_radius;
};

class EllipsoidFactory : public VerticesFactory
{
public:
  EllipsoidFactory( float a = 1, float b = 2, float c = 1 )
    : m_a(a)
    , m_b(b)
    , m_c(c)
  {
  }

  virtual vec_t operator()(float u, float v) const
  {
    float const phi = (u * 2.f - 1.f) * cglmath::c_pif;
    float const theta = (v - 0.5f) *  cglmath::c_pif;
    float sine_theta, cosine_theta, sine_phi, cosine_phi;

    cglmath::get_sin_cos(phi, sine_phi, cosine_phi);
    cglmath::get_sin_cos(theta, sine_theta, cosine_theta);

    return vec_t(cosine_theta * cosine_phi / m_a, sine_theta / m_b, cosine_theta * sine_phi / m_c);
  }

  virtual vec_t n(float u, float v) const
  {
    float const phi = (u * 2.f - 1.f) * cglmath::c_pif;
    float const theta = (v - 0.5f) *  cglmath::c_pif;
    float sine_theta, cosine_theta, sine_phi, cosine_phi;

    cglmath::get_sin_cos(phi, sine_phi, cosine_phi);
    cglmath::get_sin_cos(theta, sine_theta, cosine_theta);

    return vec_t(cosine_theta * cosine_phi * m_a, sine_theta * m_b, cosine_theta * sine_phi * m_c);
  }
private:
  float m_a;
  float m_b;
  float m_c;
};

class base_geometry_t : public IAnimationUnit
{
public:
#pragma pack(push)
#pragma pack(1)
  struct vertex_t
  {
    vec_t V;
    vec_t N;
    DWORD Color;
    float u, v;
  };
#pragma pack(pop)
  static const int c_FVF;

public:
  base_geometry_t( LPDIRECT3DDEVICE9 device, unsigned int M = 10, unsigned int N = 10, VerticesFactory const &f = VerticesFactory(), 
    color_t const &color = color_t(1.f), material_t const & material = material_t(0.01f, 1, 0.2f, 8) );
  virtual ~base_geometry_t();

  virtual void render( recursive_data_t & rd );
protected:
  unsigned int m_vertices_num;
  unsigned int m_triangles_num;
  IDirect3DVertexBuffer9 *m_vertices_buf;
  IDirect3DIndexBuffer9 *m_index_buf;

  material_t m_material;
};

#endif /* __GEOMETRY_INCLUDED__ */
