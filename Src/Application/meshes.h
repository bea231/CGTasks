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

class x_mesh_t : public geometry_t
{
public:
  void load( LPCWSTR file_name, LPDIRECT3DDEVICE9 device );
  void render( LPDIRECT3DDEVICE9 device );
  ~x_mesh_t();
private:
  ID3DXMesh *m_mesh;
  D3DMATERIAL9 *m_materials;
  DWORD m_materials_count;
};

#endif /* __MESHES_INCLUDED__ */
