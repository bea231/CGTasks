/**
@file     meshes.h
@brief    X-meshes class definition
@date     Created on 22/09/2014
@project  Task2
@author   Sergeev Artemiy
*/

#ifndef __MESHES_INCLUDED__
#define __MESHES_INCLUDED__

#include <d3dx9mesh.h>
#include "geometry.h"
#include "texture.h"
#include "unit.h"

class x_mesh_t : public IAnimationUnit
{
public:
  x_mesh_t() : m_mesh(0), m_materials(0), m_materials_count(0), m_textures(0) {}
  void load( LPCWSTR file_name, LPDIRECT3DDEVICE9 device );
  void render( recursive_data_t & rd );
  ~x_mesh_t();
private:
  ID3DXMesh *m_mesh;
  D3DMATERIAL9 *m_materials;
  texture_t *m_textures;
  DWORD m_materials_count;
};

#endif /* __MESHES_INCLUDED__ */
