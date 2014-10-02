/**
@file     cglMathCamera.h
@brief    Mathematics for computer graphics camera definition module
@date     Created on 12/09/2014
@project  Task1
@author   Sergeev Artemiy
*/

#ifndef __CGLMATHCAMERA_INCLUDED__
#define __CGLMATHCAMERA_INCLUDED__

#include <math.h>

#include "cglmathdef.h"

namespace cglmath
{
  /* Basic template class of camera */
  template<class TYPE> class TCamera
  {
  public:
    /*** Camera parameters ***/
    TVector<TYPE> location;   /* Position of camera */
    TVector<TYPE> look_at;    /* Look at_vec point of camera */
    TVector<TYPE> direction;  /* Direction of camera */
    TVector<TYPE> up;         /* Identity vector of camera, which direction is up */
    TVector<TYPE> right;      /* Identity vector of camera, which direction is right */

    /*** Camera project plane parameters ***/
    TYPE projection_distance; /* Project plane distance */
    TYPE width;               /* width of project plane */
    TYPE height;              /* height of project plane */
    TYPE saved_PPH;           /* Saved width of project plane */
    TYPE saved_PPW;           /* Saved height of project plane */
    TYPE far_clip;            /* far_clip distance */
    TYPE aspect;              /* Ration aspect of project plane */
    INT screen_width;         /* Screen width */
    INT screen_height;        /* Screen height */
    
    /*** Camera matrices ***/
    TTransform<TYPE> camera_matrix;       /* Camera coordinate system translate */
    TTransform<TYPE> projection_matrix;        /* Perspective projection matrix */

    /*** Camera constructors ***/
    
    /* Empty constructor */
    TCamera() {}
    
    /* Camera constructor
     * pos        - camera position
     * at_vec     - look at or direction vector
     * up_vec     - up vector
     * is_look_at - if TRUE - at_vec is a look at vector, otherwise - at_vec is direction
     * project_plane_w
     */
    TCamera( TVector<TYPE> & pos, TVector<TYPE> & at_vec, TVector<TYPE> & up_vec, bool is_look_at,
             TYPE project_plane_w = 0.4, TYPE project_plane_h = 0.3,
             TYPE proj_dist = 1.0, TYPE far_dist = 10000.0,
             INT  screen_w = 320, INT screen_h = 240) 
      : location(pos.x, pos.y, pos.z), up(up_vec.x, up_vec.y, up_vec.z)
      , far_clip(far_dist)
      , screen_width(screen_w)
      , screen_height(screen_h)
    {
      saved_PPH = height = project_plane_h;
      saved_PPW = width = project_plane_w;

      direction = at_vec;
      look_at = at_vec;
      if (is_look_at)
        direction = at_vec - pos;
      else
        look_at += location;
      
      direction.normalize();
      right = direction % up;
      up = right % direction;
      up.normalize();
      right.normalize();

      update_matrices();

      projection_distance = proj_dist;
      update_proj_data();
    }

    /*** Camera methods ***/

    TMatrix<TYPE> get_projection_matrix()
    {
      return projection_matrix.matrix;
    }

    TMatrix<TYPE> get_view_matrix()
    {
      return camera_matrix.matrix;
    }

    /***
    * Updations camera methods.
    ***/

    /* Update camera position function */
    TCamera & update_position( TVector<TYPE> const & pos )
    {
      location = pos;
      update_matrices();

      return *this;
    }

    /* Update camera direction by look at_vec point function */
    TCamera & update_direction( TVector<TYPE> const & at_vec )
    {
      look_at = at_vec;
      update_look_at_loc_up();

      return *this;
    }

    /* Update camera direction position up vectors function */
    TCamera & update_dir_loc_up( void )
    {
      direction.normalize();
      right = direction % up;
      right.normalize();
      up = right % direction;

      update_matrices();

      return *this;
    }

    /* Update camera look at_vec point position up vectors function */
    TCamera & update_look_at_loc_up( void )
    {
      direction = look_at - location;
      update_dir_loc_up();

      return *this;
    }

    /* Update camera matrices function */
    TCamera & update_matrices( void )
    {
      /* Make view transformation matrix */
      camera_matrix.matrix = TMatrix<TYPE>(right.x,      up.x,      direction.x,
                                           right.y,      up.y,      direction.y,
                                           right.z,      up.z,      direction.z,
                                           -location & right, -location & up, -location & direction);

      /* Make inverse transformation matrix */
      camera_matrix.inv_matrix = TMatrix<TYPE>(right.x, right.y, right.z,
                                               up.x,    up.y,    up.z,
                                               direction.x,   direction.y,   direction.z,
                                               location.x,   location.y,   location.z);

      return *this;
    }


    /* Update projection data of camera function */
    void update_proj_data( void )
    {
      TYPE x_scale, y_scale, scale;

      /* Rescale project plane size for keep proportional aspect ratio */
      x_scale = saved_PPW / screen_width;
      y_scale = saved_PPH / screen_height;
      if (x_scale > y_scale)
        scale = y_scale;
      else
        scale = x_scale;
      width = scale * screen_width;
      height = scale * screen_height;

      aspect = width / height;

      /* Evaluate perspective projection matrix */
      projection_matrix =
        TMatrix<TYPE>(static_cast<TYPE>(2.0 * projection_distance / width),      0,             0,
                        0,          static_cast<TYPE>(2.0 * projection_distance / height),      0,
                        0,                  0,       static_cast<TYPE>(far_clip / (far_clip - projection_distance)),
                        0,                  0,       static_cast<TYPE>(far_clip * projection_distance / (projection_distance - far_clip)),
                        0,                  0,             1, 0);

    }

    /* Set project plane parameters function */
    TCamera & set_proj_params( TYPE W, TYPE H,
                              TYPE proj_dist = 1, TYPE far_dist = 10000 )
    {
      saved_PPW = W;
      saved_PPH = H;
      projection_distance = proj_dist;
      far_clip = far_dist;
      update_proj_data();

      return *this;
    }

    /* Set near far params function */
    TCamera & set_near_far( TYPE proj_dist, TYPE far_dist )
    {
      projection_distance = proj_dist;
      far_clip = far_dist;
      update_proj_data();

      return *this;
    }

    /* Set camera parameters constructor */
    void set_camera( TVector<TYPE> & pos, TVector<TYPE> & at_vec, TVector<TYPE> & up_vec, bool is_look_at,
                     TYPE project_plane_w = 0.4, TYPE project_plane_h = 0.3,
                     TYPE proj_dist = 1.0, TYPE far_dist = 10000.0,
                     INT screen_w = 320, INT screen_h = 240)
    {
      *this = TCamera(pos, at_vec, up_vec, is_look_at, project_plane_w, project_plane_h, proj_dist, far_dist, screen_w, screen_h);
    }

    /***
     * Camera transformations.
     ***/

    /* Move forward camera function */
    TCamera & move_forward( TYPE distance )
    {
      location += direction * distance;
      look_at += direction * distance;  /// !!!!!!
      update_matrices();

      return *this;
    }

    /* Move camera forward without moving look at point function */
    TCamera & move_to_look_at( TYPE distance, TYPE epsilon = c_threshold )
    {
      vec_t tmp = location + direction * distance;
      if ((tmp - look_at).length() > epsilon)
      {
        location = tmp;
        update_matrices();
      }
      return *this;
    }

    /* Move right camera function */
    TCamera & move_right( TYPE distance )
    {
      location += right * distance;
      look_at += right * distance;
      update_matrices();

      return *this;
    }

    /* Move up camera function */
    TCamera & move_up( TYPE distance )
    {
      location += up * distance;
      look_at += up * distance;  /// !!!!!
      update_matrices();

      return *this;
    }

    /* Rotate camera right function */
    TCamera & rotate_right( TYPE angle )
    {
      TTransform<TYPE> rot_x;
      TYPE dist = !(location - look_at);

      rot_x.set_rotate(angle, right);

      direction = rot_x.transform_vector(direction);
      up = rot_x.transform_vector(up);
      update_dir_loc_up();
      look_at = location + direction * dist;

      return *this;
    }

    /* Rotate camera direction function */
    TCamera & rotate_dir( TYPE angle )
    {
      TTransform<TYPE> rot_z;
      TYPE dist = !(location - look_at);

      rot_z.set_rotate(angle, direction);

      right = rot_z.transform_vector(right);
      up = rot_z.transform_vector(up);
      update_dir_loc_up();
      look_at = location + direction * dist;

      return *this;
    }

    /* Rotate camera function */
    TCamera& rotate_up( TYPE angle )
    {
      TTransform<TYPE> rot_y;
      TYPE dist = !(location - look_at);

      rot_y.set_rotate(angle, up);

      direction = rot_y.transform_vector(direction);
      right = rot_y.transform_vector(right);
      update_dir_loc_up();
      look_at = location + direction * dist;

      return *this;
    }

    /* Rotation camera view round up direction vector */
    void horizontal_rotate_round_look_at( TYPE angle )
    {
      TTransform<TYPE> trans;
      TYPE length;

      /* Getting matrix of rotation round right vector */
      trans.set_unit();
      trans.rotate_y(angle);

      /* Rotating camera vectors, multiplying them on rotation matrix */
      direction = trans.transform_vector(direction);
      up = trans.transform_vector(up);
      length = !(look_at - location);
      location = look_at - direction * length;
      update_dir_loc_up();
    }

    /* Rotation camera view round right direction vector */
    void vertical_rotate_round_look_at( TYPE angle )
    {
      TTransform<TYPE> trans;
      TYPE length;

      /* Getting matrix of rotation round right vector */
      trans.set_unit();
      trans.rotate(angle, right);

      /* Rotating camera vectors, multiplying them on rotation matrix */
      direction = trans.transform_vector(direction);
      right = trans.transform_vector(right);

      /* Updating camera right, direction, up vectors */
      length = !(look_at - location);

      /* Recounting location vector */
      location = look_at - direction * length;
      update_dir_loc_up();
    }
  };
}

#endif /* __CGLMATHCAMERA_INCLUDED__ */
