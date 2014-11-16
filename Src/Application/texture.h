/**
@file     texture.h
@brief    Texture object class definition
@date     Created on 14/11/2014
@project  Task3
@author   Sergeev Artemiy
*/

#ifndef __TEXTURE_INCLUDED__
#define __TEXTURE_INCLUDED__

#include <D3D9.h>

#include <vector>

class texture_t
{
public:
  texture_t();
  texture_t( IDirect3DDevice9 * device, LPCWSTR file_name );
  ~texture_t();

  bool is_loaded() { return m_texture != NULL; }
  bool load( IDirect3DDevice9 * device, LPCWSTR file_name );
  bool load_mipmaped( IDirect3DDevice9 * device, std::vector<LPCWSTR> file_names );
  void bind( IDirect3DDevice9 * device, DWORD unit );

  static void unbind( IDirect3DDevice9 *device, DWORD unit = 0 )
  {
    device->SetTexture(unit, NULL);
  }
private:
  IDirect3DTexture9 *m_texture;
  
};

#endif /* __TEXTURE_INCLUDED__ */
