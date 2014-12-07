#include <cmath>
#include "flower.h"

/* Second petal */
petal2_shared_data_t::petal2_shared_data_t( IDirect3DDevice9 * device, flower_params_t const & params )
{
  m_vertices_num = 4;
  m_triangles_num = 2;

  device->CreateVertexBuffer(sizeof(flower_vertex_t) * m_vertices_num, D3DUSAGE_WRITEONLY, FLOWER_FVF, D3DPOOL_DEFAULT, &m_vertices_buf, NULL);
  device->CreateIndexBuffer(sizeof(int) * m_triangles_num * 3, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_index_buf, NULL);

  flower_vertex_t *vertices_buf;
  int             *indices_buf;
  m_vertices_buf->Lock(0, 0, (void **)&vertices_buf, 0);
  m_index_buf->Lock(0, 0, (void **)&indices_buf, 0);

  vertices_buf[0].V = vec_t(-params.petal1_width / 2, 0, 0);
  vertices_buf[0].N = vec_t(0, 1, 0);
  vertices_buf[0].u = 0; vertices_buf[0].v = 0;
  vertices_buf[0].Color = params.petal2_color;

  vertices_buf[1].V = vec_t(-params.petal2_width / 2, 0, params.petal2_height);
  vertices_buf[1].N = vec_t(0, 1, 0);
  vertices_buf[1].u = 0; vertices_buf[0].v = 1;
  vertices_buf[1].Color = params.petal2_color;

  vertices_buf[2].V = vec_t(params.petal2_width / 2, 0, params.petal2_height);
  vertices_buf[2].N = vec_t(0, 1, 0);
  vertices_buf[2].u = 1; vertices_buf[0].v = 1;
  vertices_buf[2].Color = params.petal2_color;

  vertices_buf[3].V = vec_t(params.petal1_width / 2, 0, 0);
  vertices_buf[3].N = vec_t(0, 1, 0);
  vertices_buf[3].u = 1; vertices_buf[0].v = 0;
  vertices_buf[3].Color = params.petal2_color;

  indices_buf[0] = 0;
  indices_buf[1] = 1;
  indices_buf[2] = 3;

  indices_buf[3] = 3;
  indices_buf[4] = 1;
  indices_buf[5] = 2;

  m_vertices_buf->Unlock();
  m_index_buf->Unlock();
}

petal2_t::petal2_t( IDirect3DDevice9 *device, flower_params_t const & params, float phase )
  : m_params(params)
  , m_phase(phase)
{
  m_shared_data = create_singleton<petal2_shared_data_t>(device, params);
}

void petal2_t::response( recursive_data_t & rd )
{
  float const t = sin( rd.timer.getTime() * m_params.velocity + m_phase ) * 0.5f + 0.5f;
  set_transform( transform_t().rotate_x(-t * m_params.petal2_angle_max - (1 - t) * m_params.petal2_angle_min ).translate(0, 0, m_params.petal1_height) );
}

/* First petal */
petal1_shared_data_t::petal1_shared_data_t( IDirect3DDevice9 * device, flower_params_t const & params )
{
  m_vertices_num = 4;
  m_triangles_num = 2;

  device->CreateVertexBuffer(sizeof(flower_vertex_t) * m_vertices_num, D3DUSAGE_WRITEONLY, FLOWER_FVF, D3DPOOL_DEFAULT, &m_vertices_buf, NULL);
  device->CreateIndexBuffer(sizeof(int) * m_triangles_num * 3, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_index_buf, NULL);

  flower_vertex_t *vertices_buf;
  int             *indices_buf;
  m_vertices_buf->Lock(0, 0, (void **)&vertices_buf, 0);
  m_index_buf->Lock(0, 0, (void **)&indices_buf, 0);

  float width = 2 * params.receptacle_radius * sin(cglmath::c_pif / params.petals_count);

  vertices_buf[0].V = vec_t(-width / 2, 0, 0);
  vertices_buf[0].N = vec_t(0, 1, 0);
  vertices_buf[0].u = 0; vertices_buf[0].v = 0;
  vertices_buf[0].Color = params.petal1_color;

  vertices_buf[1].V = vec_t(-params.petal1_width / 2, 0, params.petal1_height);
  vertices_buf[1].N = vec_t(0, 1, 0);
  vertices_buf[1].u = 0; vertices_buf[0].v = 1;
  vertices_buf[1].Color = params.petal1_color;

  vertices_buf[2].V = vec_t(params.petal1_width / 2, 0, params.petal1_height);
  vertices_buf[2].N = vec_t(0, 1, 0);
  vertices_buf[2].u = 1; vertices_buf[0].v = 1;
  vertices_buf[2].Color = params.petal1_color;

  vertices_buf[3].V = vec_t(width / 2, 0, 0);
  vertices_buf[3].N = vec_t(0, 1, 0);
  vertices_buf[3].u = 1; vertices_buf[0].v = 0;
  vertices_buf[3].Color = params.petal1_color;

  indices_buf[0] = 0;
  indices_buf[1] = 1;
  indices_buf[2] = 3;

  indices_buf[3] = 3;
  indices_buf[4] = 1;
  indices_buf[5] = 2;

  m_vertices_buf->Unlock();
  m_index_buf->Unlock();
}

petal1_t::petal1_t( IDirect3DDevice9 *device, flower_params_t const & params, float phase )
  : m_params(params)
  , m_phase(phase)
{
  m_shared_data = create_singleton<petal1_shared_data_t>(device, params);
}

void petal1_t::response( recursive_data_t & rd )
{
  float const t = sin(rd.timer.getTime() * m_params.velocity + m_phase) * 0.5f + 0.5f;
  set_transform(transform_t().rotate_x(-t * m_params.petal1_angle_max - (1 - t) * m_params.petal1_angle_min));
}

/* Petal */
petal_t::petal_t( IDirect3DDevice9 *device, flower_params_t const & params, float phase )
{
  petal1_t *petal1 = new petal1_t( device, params, phase );
  petal2_t *petal2 = new petal2_t( device, params, phase );

  *petal1 << IAnimationUnitPtr( petal2 );
  *this << IAnimationUnitPtr( petal1 );
}

void petal_t::response( recursive_data_t & rd )
{
}

void petal_t::render( recursive_data_t & rd )
{
}

/* Receptacle */
receptacle_shared_data_t::receptacle_shared_data_t( IDirect3DDevice9 * device, flower_params_t const & params )
{
  m_vertices_num = params.petals_count + 1;
  m_triangles_num = params.petals_count;

  device->CreateVertexBuffer(sizeof(flower_vertex_t) * m_vertices_num, D3DUSAGE_WRITEONLY, FLOWER_FVF, D3DPOOL_DEFAULT, &m_vertices_buf, NULL);
  device->CreateIndexBuffer(sizeof(int) * m_triangles_num * 3, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_index_buf, NULL);

  flower_vertex_t *vertices_buf;
  int             *indices_buf;
  m_vertices_buf->Lock(0, 0, (void **)&vertices_buf, 0);
  m_index_buf->Lock(0, 0, (void **)&indices_buf, 0);

  float const delta = 2 * cglmath::c_pif / params.petals_count;
  for (unsigned int i = 0; i < m_triangles_num; ++i)
  {
    vertices_buf[i].V = vec_t(params.receptacle_radius * cos(i * delta), 0, params.receptacle_radius * sin(i * delta));
    vertices_buf[i].N = vec_t(0, 1, 0);
    vertices_buf[i].u = cos(i * delta) * 0.5f + 0.5f; vertices_buf[0].v = sin(i * delta) * 0.5f + 0.5f;
    vertices_buf[i].Color = params.receptacle_color;

    indices_buf[3 * i] = m_vertices_num - 1;
    indices_buf[3 * i + 1] = i;
    indices_buf[3 * i + 2] = (i + 1) % params.petals_count;
  }
  vertices_buf[m_vertices_num - 1].V = vec_t(0, 0, 0);
  vertices_buf[m_vertices_num - 1].N = vec_t(0, 1, 0);
  vertices_buf[m_vertices_num - 1].u = 0.5; vertices_buf[0].v = 0.5;
  vertices_buf[m_vertices_num - 1].Color = params.receptacle_color;

  m_vertices_buf->Unlock();
  m_index_buf->Unlock();
}

receptacle_t::receptacle_t( IDirect3DDevice9 *device, flower_params_t const & params )
{
  m_shared_data = create_singleton<receptacle_shared_data_t>(device, params);
}

void receptacle_t::response( recursive_data_t & rd )
{
}

/* Stem */
stem_shared_data_t::stem_shared_data_t( IDirect3DDevice9 * device, flower_params_t const & params )
  : m_geometry(device, 50, 50, CylinderFactory(params.stem_length, params.stem_thickness), params.stem_color)
{
}

stem_t::stem_t( IDirect3DDevice9 *device, flower_params_t const & params )
  : m_shared_data(create_singleton<stem_shared_data_t>(device, params))
{
}

void stem_t::response( recursive_data_t & rd )
{
}

void stem_t::render( recursive_data_t & rd )
{
  m_shared_data->m_geometry.render(rd);
}

/* Flower */
flower_t::flower_t( IDirect3DDevice9 *device, flower_params_t const & params )
{
  stem_t *stem = new stem_t(device, params);
  receptacle_t *receptacle = new receptacle_t(device, params);
  receptacle->transform().translate(0, params.stem_length, 0);

  float const delta = 360.f / params.petals_count;
  for (size_t i = 0; i < params.petals_count; ++i)
  {
    petal_t *petal = new petal_t(device, params, sin(i * cglmath::c_pif / params.petals_count));
    petal->transform().translate(0, 0, params.receptacle_radius * cos(cglmath::Deg2Rad(delta * 0.5f))).rotate_y(90.f - (i + 0.5f) * delta);
    *receptacle << IAnimationUnitPtr(petal);
  }

  *stem << IAnimationUnitPtr(receptacle);
  *this << IAnimationUnitPtr(stem);
}

void flower_t::render( recursive_data_t & rd )
{
}

void flower_t::response( recursive_data_t & rd )
{
}
