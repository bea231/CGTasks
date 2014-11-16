/**
@file     texture.cpp
@brief    Texture object class declaration
@date     Created on 14/11/2014
@project  Task3
@author   Sergeev Artemiy
*/

#include <D3DX9.h>
#include "texture.h"


texture_t::texture_t() : m_texture(0)
{

}

texture_t::texture_t( IDirect3DDevice9 * device, LPCWSTR file_name ) : m_texture(0)
{
  load(device, file_name);
}

texture_t::~texture_t()
{
  if (m_texture)
    m_texture->Release();
}

bool texture_t::load( IDirect3DDevice9 * device, LPCWSTR file_name )
{
  return D3DXCreateTextureFromFile(device, file_name, &m_texture) == ERROR_SUCCESS;
}

bool texture_t::load_mipmaped( IDirect3DDevice9 * device, std::vector<LPCWSTR> file_names )
{
  D3DXIMAGE_INFO info;
  D3DXGetImageInfoFromFile(file_names[0], &info);
  IDirect3DTexture9 *temp_tex;

  size_t mipmap_count = file_names.size();
  if (D3DXCreateTexture(device, info.Width, info.Height, mipmap_count, 0, D3DFMT_R8G8B8, D3DPOOL_DEFAULT, &m_texture) != ERROR_SUCCESS)
    return false;
  if (D3DXCreateTexture(device, info.Width, info.Height, mipmap_count, 0, D3DFMT_R8G8B8, D3DPOOL_SYSTEMMEM, &temp_tex) != ERROR_SUCCESS)
  {
    m_texture->Release();
    return false;
  }

  for (size_t i = 0; i < mipmap_count; ++i)
  {
    IDirect3DSurface9 *surface;
    temp_tex->GetSurfaceLevel(i, &surface);
    D3DXLoadSurfaceFromFile(surface, NULL, NULL, file_names[i], NULL, D3DX_DEFAULT, 0, NULL);
    surface->Release();
  }

  device->UpdateTexture(temp_tex, m_texture);
  temp_tex->Release();

  return true;
}

void texture_t::bind( IDirect3DDevice9 * device, DWORD unit )
{
  device->SetTexture(unit, m_texture);
}