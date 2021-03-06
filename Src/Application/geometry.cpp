/**
  @file     geometry.cpp
  @brief    Base geometry class implementation
  @date     Created on 12/09/2014
  @project  Task1
  @author   Sergeev Artemiy
*/

#include "geometry.h"

const int base_geometry_t::c_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;

static DWORD vec_to_color( vec_t const & v)
{
  vec_t norm_v = (v.normalizing() * 0.5 + vec_t(0.5)) * 255;
  return D3DCOLOR_XRGB((int)norm_v.x, (int)norm_v.y, (int)norm_v.z);
}

base_geometry_t::base_geometry_t( LPDIRECT3DDEVICE9 device, unsigned int M, unsigned int N, VerticesFactory const &f, color_t const &color )
  : m_vertices_num(M * N)
  , m_triangles_num((M - 1) * (N - 1) * 2)
{
  float const delta_u = 1.f / (M - 1);
  float const delta_v = 1.f / (N - 1);
  vertex_t *vertices_buf;
  int      *indices_buf;

  device->CreateVertexBuffer(sizeof(vertex_t) * M * N, D3DUSAGE_WRITEONLY, c_FVF, D3DPOOL_DEFAULT, &m_vertices_buf, NULL);
  device->CreateIndexBuffer(sizeof(int) * 2 * 3 * (M - 1) * (N - 1), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_index_buf, NULL);

  m_vertices_buf->Lock(0, 0, (void **)&vertices_buf, 0);
  m_index_buf->Lock(0, 0, (void **)&indices_buf, 0);

  for (unsigned int i = 0; i < M - 1; ++i)
  {
    for (unsigned int j = 0; j < N - 1; ++j)
    {
      vertices_buf[i * N + j].V = f(delta_u * i, delta_v * j);
      vertices_buf[i * N + j].N = f.n(delta_u * i, delta_v * j);
      vertices_buf[i * N + j].u = delta_u * i;
      vertices_buf[i * N + j].v = 1 - delta_v * j;
      vertices_buf[i * N + j].Color = color;

      /* First triangle */
      indices_buf[6 * ((N - 1) * i + j)] = i * N + j;
      indices_buf[6 * ((N - 1) * i + j) + 1] = i * N + j + 1;
      indices_buf[6 * ((N - 1) * i + j) + 2] = (i + 1) * N + j;

      /* Second triangle */
      indices_buf[6 * ((N - 1) * i + j) + 3] = (i + 1) * N + j;
      indices_buf[6 * ((N - 1) * i + j) + 4] = i * N + j + 1;
      indices_buf[6 * ((N - 1) * i + j) + 5] = (i + 1) * N + j + 1;
    }
  }

  for (unsigned int j = 0; j < N - 1; ++j)
  {
    vertices_buf[(M - 1) * N + j].V = f(delta_u * (M - 1), delta_v * j);
    vertices_buf[(M - 1) * N + j].N = f.n(delta_u * (M - 1), delta_v * j);
    vertices_buf[(M - 1) * N + j].u = delta_u * (M - 1);
    vertices_buf[(M - 1) * N + j].v = 1 - delta_v * j;
    vertices_buf[(M - 1) * N + j].Color = color;
  }
  for (unsigned int i = 0; i < M; ++i)
  {
    vertices_buf[i * N + N - 1].V = f(delta_u * i, delta_v * (N - 1));
    vertices_buf[i * N + N - 1].N = f.n(delta_u * i, delta_v * (N - 1));
    vertices_buf[i * N + N - 1].u = delta_u * i;
    vertices_buf[i * N + N - 1].v = 1 - delta_v * (N - 1);
    vertices_buf[i * N + N - 1].Color = color;
  }

  m_vertices_buf->Unlock();
  m_index_buf->Unlock();
}

base_geometry_t::~base_geometry_t()
{
  m_vertices_buf->Release();
  m_index_buf->Release();
}

void base_geometry_t::render( recursive_data_t & rd )
{
  rd.device->SetFVF(c_FVF);
  rd.device->SetIndices(m_index_buf);
  rd.device->SetStreamSource(0, m_vertices_buf, 0, sizeof(vertex_t));
  rd.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vertices_num, 0, m_triangles_num);
}