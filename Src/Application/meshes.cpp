/**
@file     meshes.cpp
@brief    X-meshes class implementation
@date     Created on 22/09/2014
@project  Task2
@author   Sergeev Artemiy
*/

#include <string>

#include "meshes.h"

void A2W( std::wstring &ws, const std::string &s )
{
  std::wstring wsTmp(s.begin(), s.end());

  ws = wsTmp;
}

void x_mesh_t::load( LPCWSTR file_name, LPDIRECT3DDEVICE9 device )
{
  ID3DXBuffer *materials_buf = NULL;

  HRESULT hr = D3DXLoadMeshFromX(file_name, 0, device, NULL, &materials_buf, NULL, &m_materials_count, &m_mesh);
  if (hr != ERROR_SUCCESS)
     return;

  D3DXMATERIAL *materials_array = (D3DXMATERIAL *)materials_buf->GetBufferPointer();
  m_materials = new D3DMATERIAL9[m_materials_count];

  m_textures = new texture_t[m_materials_count];

  bool result;
  for (DWORD i = 0; i < m_materials_count; ++i)
  {
    m_materials[i] = materials_array[i].MatD3D;
    m_materials[i].Ambient = m_materials[i].Diffuse;

    std::wstring str;
    A2W(str, std::string(materials_array[i].pTextureFilename));

    result = m_textures[i].load(device, str.c_str());
  }
  if (materials_buf)
    materials_buf->Release();
  
  m_mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);
}

void x_mesh_t::render( LPDIRECT3DDEVICE9 device )
{
  device->SetTransform(D3DTS_WORLD, (D3DMATRIX *)m_transform.matrix.M);
  for (DWORD i = 0; i < m_materials_count; ++i)
  {
    device->SetMaterial(&(m_materials[i]));
    m_textures->bind(device, 0);
    m_mesh->DrawSubset(i);
  }
}

x_mesh_t::~x_mesh_t()
{
  if (m_materials)
    delete[] m_materials;
  if (m_mesh)
    m_mesh->Release();
  if (m_textures)
    delete[] m_textures;
}
