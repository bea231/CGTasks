/**
@file     unit.h
@brief    Animation unit class definition
@date     Created on 20/11/2014
@project  Task4
@author   Sergeev Artemiy
*/

#ifndef __UNIT_INCLUDED__
#define __UNIT_INCLUDED__

#include <d3d9.h>
#include <list>
#include <memory>

#include "Math/cglMath.h"
#include "../Library/cglTimer.h"
#include "Math/cglMath.h"

class myApp;

struct recursive_data_t
{
  transform_t world_transform;
  camera_t    camera;
  cglTimer    timer;

  IDirect3DDevice9 *device;

  recursive_data_t() {};
  recursive_data_t( IDirect3DDevice9 *_device, camera_t &_camera, cglTimer &_timer, transform_t &_world_transform )
     : device(_device)
     , camera(_camera)
     , timer(_timer)
     , world_transform(_world_transform)
  {

  }
};

class IAnimationUnit
{
public:
  virtual ~IAnimationUnit() = 0 {}
  virtual void render( recursive_data_t & rd ) {};
  virtual void response( recursive_data_t & rd ) {};

  IAnimationUnit & operator << ( std::unique_ptr<IAnimationUnit> unit )
  {
    m_units.push_back(std::move(unit));
    return *this;
  }

  transform_t const & get_transform()
  {
    return m_transform;
  }

  transform_t & transform()
  {
    return m_transform;
  }

  void set_transform( transform_t const & transform )
  {
     m_transform = transform;
  }
protected:
  transform_t m_transform;
private:
  void treat_as_unit( recursive_data_t &rd )
  {
    transform_t const saved_transform = rd.world_transform;

    rd.world_transform = m_transform * rd.world_transform;
    rd.device->SetTransform( D3DTS_WORLD, (D3DMATRIX *)rd.world_transform.matrix.M );
    render( rd );
    response( rd );
    for (auto it = m_units.begin(); it != m_units.end(); ++it)
      (*it)->treat_as_unit( rd );
    rd.world_transform = saved_transform;
  }
private:
  std::list<std::unique_ptr<IAnimationUnit>> m_units;

  friend myApp;
};

typedef std::unique_ptr<IAnimationUnit> IAnimationUnitPtr;

#endif /* __UNIT_INCLUDED__ */ 
