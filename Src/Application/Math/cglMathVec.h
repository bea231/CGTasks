/**
@file     cglMathVec.h
@brief    Mathematics for computer graphics vector declaration module
@date     Created on 12/09/2014
@project  Task1
@author   Sergeev Artemiy
*/

#ifndef __CGLMATHVEC_INCLUDED__
#define __CGLMATHVEC_INCLUDED__

#include <assert.h>
#include <math.h>

#include "cglMathDef.h"

namespace cglmath
{
  /* Vector type basic template class */
  template <class TYPE> class TVector
  {
  public:
    TYPE x, y, z;

    TVector() { }

    /* Constructor by equal components */
    explicit TVector( TYPE value ) : x(value), y(value), z(value)
    {
    }

    /* Constructor by components */
    TVector( TYPE x_val, TYPE y_val, TYPE z_val ) : x(x_val), y(y_val), z(z_val)
    {
    }

    /* Evaluation vector length operator */
    TYPE operator!( void ) const
    {
      return length();
    }

    /* Evaluation vector length function */
    TYPE length( void ) const
    {
      TYPE len = x * x + y * y + z * z;

      if (len > 0 && len != 1)
        len = sqrt(len);
      return len;
    }

    /* Evaluation vector squared length function */
    TYPE length2( void ) const
    {
      return x * x + y * y + z * z;
    }

    /* Dot product of two vectors operator */
    TYPE operator&( TVector const &v ) const
    {
      return dot(v);
    }

    /* Dot product of two vectors function */
    TYPE dot( TVector const &v ) const
    {
      return x * v.x + y * v.y + z * v.z;
    }

    /* Cross product of two vectors operator */
    TVector operator%( TVector const &v) const
    {
      return cross(v);
    } /* End of 'operator%' function */

    /* Cross product of two vectors function */
    TVector cross( TVector const &v ) const
    {
      return TVector(y * v.z - v.y * z, v.x * z - x * v.z, x * v.y - v.x * y);
    }

    /* Comparing two vectors function */
    bool operator==( TVector const &v ) const
    {
      return
        c_threshold > Abs(x - v.x) &&
        c_threshold > Abs(y - v.y) &&
        c_threshold > Abs(z - v.z);
    }

    /* Comparing two vectors function */
    bool operator!=( TVector const &v ) const
    {
      return
        c_threshold < Abs(x - v.x) ||
        c_threshold < Abs(y - v.y) ||
        c_threshold < Abs(z - v.z);
    }

    /* Per components addition of two vectors function */
    TVector operator+( TVector const &v ) const
    {
      return TVector(x + v.x, y + v.y, z + v.z);
    }

    /* Per components subtraction of two vectors function */
    TVector operator-( TVector const &v ) const
    {
      return TVector(x - v.x, y - v.y, z - v.z);
    }

    /* Per components multiplication of two vectors function */
    TVector operator*( TVector const &v ) const
    {
      return TVector(x * v.x, y * v.y, z * v.z);
    }

    /* Vector multiplication by number function */
    TVector operator*( TYPE val ) const
    {
      return TVector(val * x, val * y, val * z);
    }

    /* Vector division by number function */
    TVector operator/( TYPE val ) const
    {
      return TVector(x / val, y / val, z / val);
    }

    /* Inverse vector function */
    TVector operator-( void ) const
    {
      return TVector(-x, -y, -z);
    }

    /* Negate vector (itself) function */
    TVector & negate( void )
    {
      x = -x;
      y = -y;
      z = -z;

      return *this;
    }

    /* Per components addition of vector with assignment function */
    TVector & operator+=( TVector const &v )
    {
      x += v.x;
      y += v.y;
      z += v.z;

      return *this;
    }

    /* Per components subtraction of vector with assignment function */
    TVector & operator-=( TVector const &v )
    {
      x -= v.x;
      y -= v.y;
      z -= v.z;

      return *this;
    }

    /* Multiplication with assignment by number function */
    TVector & operator*=( TYPE val )
    {
      x *= val;
      y *= val;
      z *= val;

      return *this;
    }

    /* Per components multiplication with assignment by vector function */
    TVector & operator*=( TVector const &v )
    {
      x *= v.x;
      y *= v.y;
      z *= v.z;

      return *this;
    }

    /* Multiplication by matrix function */
    TVector operator*( TMatrix<TYPE> const &Matr ) const
    {
      return
        TVector(x * Matr.M[0][0] + y * Matr.M[1][0] + z * Matr.M[2][0] + Matr.M[3][0],
        x * Matr.M[0][1] + y * Matr.M[1][1] + z * Matr.M[2][1] + Matr.M[3][1],
        x * Matr.M[0][2] + y * Matr.M[1][2] + z * Matr.M[2][2] + Matr.M[3][2]);
    }

    /* Multiplication by matrix with assignment function */
    TVector & operator*=( TMatrix<TYPE> const &Matr )
    {
      TYPE a, b;

      a = x * Matr.M[0][0] + y * Matr.M[1][0] + z * Matr.M[2][0] + Matr.M[3][0];
      b = x * Matr.M[0][1] + y * Matr.M[1][1] + z * Matr.M[2][1] + Matr.M[3][1];
      z = x * Matr.M[0][2] + y * Matr.M[1][2] + z * Matr.M[2][2] + Matr.M[3][2];
      x = a;
      y = b;

      return *this;
    }

    /* Cross product with assignment function */
    TVector & operator%=( TVector const &v )
    {
      TYPE a, b;

      a = y * v.z - v.y * z;
      b = v.x * z - x * v.z;
      z = x * v.y - v.x * y;
      x = a;
      y = b;

      return *this;
    }

    /* Per components division with assignment by number */
    TVector & operator/=( TYPE val )
    {
      x /= val;
      y /= val;
      z /= val;

      return *this;
    }

    /* Normalize vector (itself) function */
    TVector & normalize( void )
    {
      TYPE len = x * x + y * y + z * z;

      if (len > 0 && len != 1)
      {
        len = sqrt(len);
        x /= len;
        y /= len;
        z /= len;
      }
      return *this;
    }

    /* Normalize vector function */
    TVector normalizing( void ) const
    {
      TYPE len = x * x + y * y + z * z;

      if (len > 0 && len != 1)
      {
          len = sqrt(len);
          return TVector(x / len, y / len, z / len);
      }
      return *this;
    }

    /* Clamp vector function */
    TVector & clamp( TYPE Min = 0, TYPE Max = 1 )
    {
      if (x < Min)
        x = Min;
      else
        if (x > Max)
          x = Max;
      if (y < Min)
        y = Min;
      else
        if (y > Max)
          y = Max;
      if (z < Min)
        z = Min;
      else
        if (z > Max)
          z = Max;
      return *this;
    }

    /* Clamp vector function */
    TVector clamping (TYPE Min = 0, TYPE Max = 1 ) const
    {
      return TVector(x < Min ? Min : x > Max ? Max : x,
        y < Min ? Min : y > Max ? Max : y,
        z < Min ? Min : z > Max ? Max : z);
    }

    /*** Vector transformation functions ***/

    /***
     * Apply specified transformation to vector (self-transform) functions
     ***/

    /* Translate vector function */
    TVector & Translate( TYPE dx, TYPE dy, TYPE dz )
    {
      x += dx;
      y += dy;
      z += dz;

      return *this;
    }

    /* Translate vector function */
    TVector & translate( TVector<TYPE> const &shift )
    {
      return Translate(shift.x, shift.y, shift.z);
    }

    /* Rotate around 'x' axis vector function */
    TVector & rotate_x( TYPE angle_sine, TYPE angle_cosine )
    {
      TYPE T;

      T = y * angle_cosine - z * angle_sine;
      z = y * angle_sine + z * angle_cosine;
      y = T;

      return *this;
    }

    /* rotate around 'x' axis vector function */
    TVector & rotate_x( TYPE angle_in_degree )
    {
      double angle_in_radians = Deg2Rad(angle_in_degree);
      TYPE sine_val, cosine_val;

#ifdef __USE_SINCOS_ASM__
      _asm {
        /* FST(0) Angle (from degree to radian) */
        fld  angle_in_radians

          /* FST(0) - cos, FST(1) - sin */
          fsincos

          fstp cosine_val    /* cos -> cosine_val */
          fstp sine_val    /* sin -> sine_val */
      }
#else /* __NOUSE_SINCOS_ASM__ */
      sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotate_x(sine_val, cosine_val);
    }

    /* Rotate around 'y' axis vector function */
    TVector & rotate_y( TYPE angle_sine, TYPE angle_cosine )
    {
      TYPE T;

      T = z * angle_cosine - x * angle_sine;
      x = z * angle_sine + x * angle_cosine;
      z = T;

      return *this;
    }

    /* Rotate around 'y' axis vector function */
    TVector & rotate_y( TYPE angle_in_degree )
    {
      double angle_in_radians = Deg2Rad(angle_in_degree);
      TYPE sine_val, cosine_val;

#ifdef __USE_SINCOS_ASM__
      _asm {
        /* FST(0) Angle (from degree to radian) */
        fld  angle_in_radians

          /* FST(0) - cos, FST(1) - sin */
          fsincos

          fstp cosine_val    /* cos -> cosine_val */
          fstp sine_val    /* sin -> sine_val */
      }
#else /* __NOUSE_SINCOS_ASM__ */
      sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotate_y(sine_val, cosine_val);
    }

    /* Rotate around 'z' axis vector function */
    TVector & rotate_z( TYPE angle_sine, TYPE angle_cosine )
    {
      TYPE T;

      T = x * angle_cosine - y * angle_sine;
      y = x * angle_sine + y * angle_cosine;
      x = T;

      return *this;
    }

    /* Rotate around 'z' axis vector function */
    TVector & rotate_z( TYPE angle_in_degree )
    {
      double angle_in_radians = Deg2Rad(angle_in_degree);
      TYPE sine_val, cosine_val;

#ifdef __USE_SINCOS_ASM__
      _asm {
        /* FST(0) Angle (from degree to radian) */
        fld  angle_in_radians

          /* FST(0) - cos, FST(1) - sin */
          fsincos

          fstp cosine_val    /* cos -> cosine_val */
          fstp sine_val    /* sin -> sine_val */
      }
#else /* __NOUSE_SINCOS_ASM__ */
      sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotate_z(sine_val, cosine_val);
    }

    /* Rotate around arbitrary axis vector function */
    TVector & rotate( TYPE angle_sine, TYPE angle_cosine,
                      TYPE axis_x, TYPE axis_y, TYPE axis_z )
    {
      return rotate(Rad2Deg(atan2(angle_sine, angle_cosine)), axis_x, axis_y, axis_z);
    }

    /* Rotate around arbitrary axis vector function */
    TVector & rotate( TYPE angle_in_degree,
                      TYPE axis_x, TYPE axis_y, TYPE axis_z )
    {
      TYPE a, b, Rm[3][3];

      BuildRotateMatrix3x3<TYPE, 3>(Rm, angle_in_degree, axis_x, axis_y, axis_z);

      a = x * Rm[0][0] + y * Rm[1][0] + z * Rm[2][0];
      b = x * Rm[0][1] + y * Rm[1][1] + z * Rm[2][1];
      z = x * Rm[0][2] + y * Rm[1][2] + z * Rm[2][2];
      x = a;
      y = b;

      return *this;
    }

    /* Rotate around arbitrary axis vector function */
    TVector & rotate( TYPE angle_sine, TYPE angle_cosine,
                      TVector<TYPE> const &vec )
    {
      return rotate(angle_sine, angle_cosine, vec.x, vec.y, vec.z);
    }

    /* Rotate around arbitrary axis vector function */
    TVector & rotate( TYPE angle_in_degree,  TVector<TYPE> const &vec )
    {
      double angle_in_radians = Deg2Rad(angle_in_degree);
      TYPE sine_val, cosine_val;

#ifdef __USE_SINCOS_ASM__
      _asm {
        /* FST(0) Angle (from degree to radian) */
        fld  angle_in_radians

          /* FST(0) - cos, FST(1) - sin */
          fsincos

          fstp cosine_val    /* cos -> cosine_val */
          fstp sine_val    /* sin -> sine_val */
      }
#else /* __USE_SINCOS_ASM__ */
      sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotate(sine_val, cosine_val, vec.x, vec.y, vec.z);
    }

    /* Scale along axes vector function */
    TVector & scale( TYPE sx, TYPE sy, TYPE sz )
    {
      x *= sx;
      y *= sy;
      z *= sz;

      return *this;
    }

    /* Scale along axes vector function */
    TVector & scale( TVector<TYPE> const &scale_vec )
    {
      return scale(scale_vec.x, scale_vec.y, scale_vec.z);
    }

    /* Transform vector by specified transformation function */
    TVector & transform( const TTransform<TYPE> &trans )
    {
      TYPE a, b;

      a = x * trans.M.M[0][0] + y * trans.M.M[1][0] +
        z * trans.M.M[2][0] + trans.M.M[3][0];
      b = x * trans.M.M[0][1] + y * trans.M.M[1][1] +
        z * trans.M.M[2][1] + trans.M.M[3][1];
      z = x * trans.M.M[0][2] + y * trans.M.M[1][2] +
        z * trans.M.M[2][2] + trans.M.M[3][2];
      x = a;
      y = b;

      return *this;
    }

    /* Inverse transform vector by specified transformation function */
    TVector & inv_transform( const TTransform<TYPE> &trans )
    {
      TYPE a, b;

      a = x * trans.InvM.M[0][0] + y * trans.InvM.M[1][0] +
        z * trans.InvM.M[2][0] + trans.InvM.M[3][0];
      b = x * trans.InvM.M[0][1] + y * trans.InvM.M[1][1] +
        z * trans.InvM.M[2][1] + trans.InvM.M[3][1];
      z = x * trans.InvM.M[0][2] + y * trans.InvM.M[1][2] +
        z * trans.InvM.M[2][2] + trans.InvM.M[3][2];
      x = a;
      y = b;

      return *this;
    }

    /***
    * Apply specified transformation to vector functions
    ***/

    /* Translation vector function */
    TVector translation( TYPE dx, TYPE dy, TYPE dz ) const
    {
      return TVector(x + dx, y + dy, z + dz);
    }

    /* Translation vector function */
    TVector translation( TVector<TYPE> const &shift ) const
    {
      return translation(shift.x, shift.y, shift.z);
    }

    /* Rotation around 'x' axis vector function */
    TVector rotation_x( TYPE angle_sine, TYPE angle_cosine ) const
    {
      return TVector(x,
        y * angle_cosine - z * angle_sine,
        y * angle_sine + z * angle_cosine);
    }

    /* Rotation around 'x' axis vector function */
    TVector rotation_x( TYPE angle_in_degree ) const
    {
      double angle_in_radians = Deg2Rad(angle_in_degree);
      TYPE sine_val, cosine_val;

#ifdef __USE_SINCOS_ASM__
      _asm {
        /* FST(0) Angle (from degree to radian) */
        fld  angle_in_radians

          /* FST(0) - cos, FST(1) - sin */
          fsincos

          fstp cosine_val    /* cos -> cosine_val */
          fstp sine_val    /* sin -> sine_val */
      }
#else /* __NOUSE_SINCOS_ASM__ */
      sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotation_x(sine_val, cosine_val);
    }

    /* Rotation around 'y' axis vector function */
    TVector rotation_y( TYPE angle_sine, TYPE angle_cosine ) const
    {
      return TVector(z * angle_sine + x * angle_cosine,
                     y,
                     z * angle_cosine - x * angle_sine);
    }

    /* Rotation around 'y' axis vector function */
    TVector rotation_y( TYPE angle_in_degree ) const
    {
      double angle_in_radians = Deg2Rad(angle_in_degree);
      TYPE sine_val, cosine_val;

#ifdef __USE_SINCOS_ASM__
      _asm {
        /* FST(0) Angle (from degree to radian) */
        fld  angle_in_radians

          /* FST(0) - cos, FST(1) - sin */
          fsincos

          fstp cosine_val    /* cos -> cosine_val */
          fstp sine_val    /* sin -> sine_val */
      }
#else /* __NOUSE_SINCOS_ASM__ */
      sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotation_y(sine_val, cosine_val);
    }

    /* Rotation around 'z' axis vector function */
    TVector rotation_z( TYPE angle_sine, TYPE angle_cosine ) const
    {
      return TVector(x * angle_cosine - y * angle_sine,
                     x * angle_sine + y * angle_cosine,
                     z);
    }

    /* Rotation around 'z' axis vector function */
    TVector rotation_z( TYPE angle_in_degree ) const
    {
      double angle_in_radians = Deg2Rad(angle_in_degree);
      TYPE sine_val, cosine_val;

#ifdef __USE_SINCOS_ASM__
      _asm {
        /* FST(0) Angle (from degree to radian) */
        fld  angle_in_radians

          /* FST(0) - cos, FST(1) - sin */
          fsincos

          fstp cosine_val    /* cos -> cosine_val */
          fstp sine_val    /* sin -> sine_val */
      }
#else /* __NOUSE_SINCOS_ASM__ */
      sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotation_z(sine_val, cosine_val);
    }

    /* Rotation around arbitrary axis vector function */
    TVector rotation( TYPE angle_sine, TYPE angle_cosine,
                      TYPE axis_x, TYPE axis_y, TYPE axis_z ) const
    {
      return rotation(Rad2Deg(atan2(angle_sine, angle_cosine)), x, y, z);
    }

    /* Rotation around arbitrary axis vector function */
    TVector rotation( TYPE angle_in_degree,
                      TYPE axis_x, TYPE axis_y, TYPE axis_z ) const
    {
      TYPE Rm[3][3];

      BuildRotateMatrix3x3<TYPE, 3>(Rm, angle_in_degree, axis_x, axis_y, axis_z);

      return TVector(x * Rm[0][0] + y * Rm[1][0] + z * Rm[2][0],
        x * Rm[0][1] + y * Rm[1][1] + z * Rm[2][1],
        x * Rm[0][2] + y * Rm[1][2] + z * Rm[2][2]);
    }

    /* Rotation around arbitrary axis vector function */
    TVector rotation( TYPE angle_sine, TYPE angle_cosine,
                      TVector<TYPE> const &vec ) const
    {
      return rotation(angle_sine, angle_cosine, vec.x, vec.y, vec.z);
    }

    /* Rotation around arbitrary axis vector function */
    TVector rotation( TYPE angle_in_degree,  TVector<TYPE> const &vec ) const
    {
      double angle_in_radians = Deg2Rad(angle_in_degree);
      TYPE sine_val, cosine_val;

#ifdef __USE_SINCOS_ASM__
      _asm {
        /* FST(0) Angle (from degree to radian) */
        fld  angle_in_radians

          /* FST(0) - cos, FST(1) - sin */
          fsincos

          fstp cosine_val    /* cos -> cosine_val */
          fstp sine_val    /* sin -> sine_val */
      }
#else /* __NOUSE_SINCOS_ASM__ */
      sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotation(sine_val, cosine_val, vec.x, vec.y, vec.z);
    }

    /* Scaling along axes vector function */
    TVector scaling( TYPE sx, TYPE sy, TYPE sz ) const
    {
      return TVector(x * sx, y * sy, z * sz);
    }

    /* Scaling along axes vector function */
    TVector scaling( TVector<TYPE> const &scale_vec) const
    {
      return scaling(scale_vec.x, scale_vec.y, scale_vec.z);
    }

    /* Transformation vector by specified transformation function */
    TVector transformation( const TTransform<TYPE> &trans ) const
    {
      return TVector(x * trans.M.M[0][0] + y * trans.M.M[1][0] +
                     z * trans.M.M[2][0] + trans.M.M[3][0],
                     x * trans.M.M[0][1] + y * trans.M.M[1][1] +
                     z * trans.M.M[2][1] + trans.M.M[3][1],
                     x * trans.M.M[0][2] + y * trans.M.M[1][2] +
        z * trans.M.M[2][2] + trans.M.M[3][2]);
    }

    /* Inverse transformation vector by specified transformation function */
    TVector inv_transformation( const TTransform<TYPE> &trans ) const
    {
      return TVector(x * trans.InvM.M[0][0] + y * trans.InvM.M[1][0] +
                     z * trans.InvM.M[2][0] + trans.InvM.M[3][0],
                     x * trans.InvM.M[0][1] + y * trans.InvM.M[1][1] +
                     z * trans.InvM.M[2][1] + trans.InvM.M[3][1],
                     x * trans.InvM.M[0][2] + y * trans.InvM.M[1][2] +
                     z * trans.InvM.M[2][2] + trans.InvM.M[3][2]);
    }
  };
}

#endif /* __CGLMATHVEC_INCLUDED__ */
