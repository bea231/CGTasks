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
#include "Math/cglMath.h"

class geometry_t
{
public:
  geometry_t() {}
  virtual ~geometry_t() = 0 {}

  virtual void render( LPDIRECT3DDEVICE9 device ) = 0;
  virtual void transform( transform_t const &t )
  {
    m_transform.transform(t);
  }
  virtual void set_transform(transform_t const &t)
  {
    m_transform = t;
  }
 protected:
  transform_t m_transform;
};

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

class base_geometry_t : public geometry_t
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
  base_geometry_t( LPDIRECT3DDEVICE9 device, unsigned int M = 10, unsigned int N = 10, VerticesFactory const &f = VerticesFactory() );
  virtual ~base_geometry_t();

  virtual void render( LPDIRECT3DDEVICE9 device );
protected:
  unsigned int m_vertices_num;
  unsigned int m_triangles_num;
  IDirect3DVertexBuffer9 *m_vertices_buf;
  IDirect3DIndexBuffer9 *m_index_buf;

};

#endif /* __GEOMETRY_INCLUDED__ */
