#ifndef __FLOWER_INCLUDED__
#define __FLOWER_INCLUDED__

#include <memory>
#include "unit.h"
#include "texture.h"
#include "singletone.h"
#include "geometry.h"

struct flower_params_t
{
  float velocity;
  unsigned int petals_count;

  /* Stem params */
  float stem_length;
  float stem_thickness;
  color_t stem_color;

  /* Receptacle params */
  float receptacle_radius;
  color_t receptacle_color;

  /* First petal params */
  float petal1_angle_min;
  float petal1_angle_max;
  float petal1_height;
  float petal1_width;
  color_t petal1_color;

  /* Second petal params */
  float petal2_angle_min;
  float petal2_angle_max;
  float petal2_height;
  float petal2_width;
  color_t petal2_color;
};

#pragma pack(push)
#pragma pack(1)
struct flower_vertex_t
{
  vec_t V;
  vec_t N;
  DWORD Color;
  float u, v;
};
#pragma pack(pop)

#define FLOWER_FVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct flower_geometry_shared_data_t
{
public:
  virtual ~flower_geometry_shared_data_t()
  {
    m_vertices_buf->Release();
    m_index_buf->Release();
  }
  IDirect3DVertexBuffer9 *m_vertices_buf;
  IDirect3DIndexBuffer9 *m_index_buf;
  unsigned int m_vertices_num;
  unsigned int m_triangles_num;
};
typedef std::shared_ptr<flower_geometry_shared_data_t> flower_geometry_shared_data_ptr_t;

struct flower_geometry_t : public IAnimationUnit
{
public:
  void render( recursive_data_t & rd )
  {
    rd.device->SetFVF(FLOWER_FVF);
    rd.device->SetIndices(m_shared_data->m_index_buf);
    rd.device->SetStreamSource(0, m_shared_data->m_vertices_buf, 0, sizeof(flower_vertex_t));
    rd.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_shared_data->m_vertices_num, 0, m_shared_data->m_triangles_num);
  }
protected:
  flower_geometry_shared_data_ptr_t m_shared_data;
};

/* Petal 2 */
struct petal2_shared_data_t : flower_geometry_shared_data_t
{
  petal2_shared_data_t( IDirect3DDevice9 * device, flower_params_t const & params );
};
typedef std::shared_ptr<petal2_shared_data_t> petal2_shared_data_ptr_t;

class petal2_t : public flower_geometry_t
{
public:
  petal2_t( IDirect3DDevice9 *device, flower_params_t const & params, float phase );

  void response( recursive_data_t & rd );
private:
  flower_params_t m_params;
  float m_phase;
};

/* Petal 1 */
struct petal1_shared_data_t : flower_geometry_shared_data_t
{
public:
  petal1_shared_data_t( IDirect3DDevice9 * device, flower_params_t const & params );
};
typedef std::shared_ptr<petal1_shared_data_t> petal1_shared_data_ptr_t;

class petal1_t : public flower_geometry_t
{
public:
  petal1_t( IDirect3DDevice9 *device, flower_params_t const & params, float phase );
  void response( recursive_data_t & rd );
private:
  flower_params_t m_params;
  float m_phase;
};

class petal_t : public IAnimationUnit
{
public:
  petal_t( IDirect3DDevice9 *device, flower_params_t const & params, float phase );
  void response( recursive_data_t & rd );
  void render( recursive_data_t & rd );
private:
  flower_params_t m_params;
};

/* Receptacle */
struct receptacle_shared_data_t : flower_geometry_shared_data_t
{
public:
  receptacle_shared_data_t( IDirect3DDevice9 * device, flower_params_t const & params );
};
typedef std::shared_ptr<receptacle_shared_data_t> receptacle_shared_data_ptr_t;

class receptacle_t : public flower_geometry_t
{
public:
  receptacle_t( IDirect3DDevice9 *device, flower_params_t const & params );
  void response( recursive_data_t & rd );
};

/* Stem */

struct stem_shared_data_t
{
public:
  stem_shared_data_t( IDirect3DDevice9 * device, flower_params_t const & params );
  base_geometry_t m_geometry;
};
typedef std::shared_ptr<stem_shared_data_t> stem_shared_data_ptr_t;

class stem_t : public IAnimationUnit
{
public:
  stem_t( IDirect3DDevice9 *device, flower_params_t const & params );
  void response( recursive_data_t & rd );
  void render( recursive_data_t & rd );
private:
  stem_shared_data_ptr_t m_shared_data;
};

/* Flower */
class flower_t : public IAnimationUnit
{
public:
  flower_t( IDirect3DDevice9 *device, flower_params_t const & params );
  void render( recursive_data_t & rd );
  void response( recursive_data_t & rd );
};

class base_plane_t : public IAnimationUnit
{
public:
  base_plane_t( IDirect3DDevice9 * device )
    : m_geom( device, 500, 500, VerticesFactory() )
    , m_texture(device, L"Res/ground00.bmp")
  {
    transform().rotate_x( -90 ).translate( -0.5f, 0, 0.5f ).scale( 50 ).translate( 0, -0.1f, 0 );
  }

  void render( recursive_data_t &rd )
  {
    auto_texture_binder_t(rd.device, m_texture, 0);
    m_geom.render(rd);
  }
private:
  base_geometry_t m_geom;
  texture_t m_texture;
};

#endif /* __FLOWER_INCLUDED__ */
