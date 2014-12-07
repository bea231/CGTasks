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

class texture_binder_t
{
public:
  texture_binder_t( IDirect3DDevice9 *device, texture_t & texture, DWORD stage )
    : m_stage(stage)
    , m_device(device)
  {
    m_device->GetTexture(stage, &m_prev_tex);
    texture.bind(device, stage);
  }

  ~texture_binder_t()
  {
    m_device->SetTexture(m_stage, m_prev_tex);
  }
private:
  DWORD m_stage;
  IDirect3DDevice9 *m_device;
  IDirect3DBaseTexture9 *m_prev_tex;
};

class texture_saver_t
{
public:
  texture_saver_t( IDirect3DDevice9 *device, DWORD stage )
    : m_stage(stage)
    , m_device(device)
  {
    m_device->GetTexture(stage, &m_prev_tex);
  }

  ~texture_saver_t()
  {
    m_device->SetTexture(m_stage, m_prev_tex);
  }
private:
  DWORD m_stage;
  IDirect3DDevice9 *m_device;
  IDirect3DBaseTexture9 *m_prev_tex;
};

#define auto_texture_saver_t(device, stage) texture_saver_t const texture_saver_t##__COUNTER__(device, stage)
#define auto_texture_binder_t(device, texture, stage) texture_binder_t const texture_binder_t##__COUNTER__(device, texture, stage)

#endif /* __TEXTURE_INCLUDED__ */
