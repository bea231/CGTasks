/**
@file     cglMathTransform.h
@brief    Mathematics for computer graphics matrix declaration module
@date     Created on 12/09/2014
@project  Task1
@author   Sergeev Artemiy
*/

#ifndef __CGLMATHTRANSFORM_INCLUDED__
#define __CGLMATHTRANSFORM_INCLUDED__

#include <string.h>

#include "cglMathDef.h"

namespace cglmath
{
  template<class TYPE> class TTransform
  {
  public:
    /* Matrix and inverse matrix */
    TMatrix<TYPE> matrix, inv_matrix;

    /* Identify constructor */
    TTransform( bool Identify = true )
    {
      if (Identify)
      {
        matrix.set_unit();
        inv_matrix.set_unit();
      }
    }

    /* transform by matrix constructor */
    TTransform( const TMatrix<TYPE> &matr )
    {
      matrix = matr;
      inv_matrix = matr.inversing();
    }

    /* transform by both matrix constructor */
    TTransform( const TMatrix<TYPE> &matr, const TMatrix<TYPE> &inv_matr )
    {
      matrix = matr;
      inv_matrix = inv_matr;
    }

    /* Copying constructor */
    TTransform( const TTransform &trans )
    {
      memcpy(this, &trans, sizeof(TTransform));
    }

    /***
     * Transformation base object functions
     ***/

    /* Transform 3D point function */
    TVector<TYPE> transform_point( TVector<TYPE> const &vec ) const
    {
      return vec.transformation(*this);
    }

    /* Transform 3D vector function */
    TVector<TYPE> transform_vector( TVector<TYPE> const &vec ) const
    {
      return
         TVector<TYPE>(vec.x * matrix.M[0][0] + vec.y * matrix.M[1][0] + vec.z * matrix.M[2][0],
                       vec.x * matrix.M[0][1] + vec.y * matrix.M[1][1] + vec.z * matrix.M[2][1],
                       vec.x * matrix.M[0][2] + vec.y * matrix.M[1][2] + vec.z * matrix.M[2][2]);
    }

    /* Transform 3D normal (surface perpendicular) function */
    TVector<TYPE> transform_normal( TVector<TYPE> const &vec ) const
    {
      return TVector<TYPE>(vec.x * inv_matrix.M[0][0] + vec.y * inv_matrix.M[0][1] +
                           vec.z * inv_matrix.M[0][2],
                           vec.x * inv_matrix.M[1][0] + vec.y * inv_matrix.M[1][1] +
                           vec.z * inv_matrix.M[1][2],
                           vec.x * inv_matrix.M[2][0] + vec.y * inv_matrix.M[2][1] +
                           vec.z * inv_matrix.M[2][2]).Normalize();
    }

    /* Inverse transform 3D point function */
    TVector<TYPE> inv_transform_point( TVector<TYPE> const &vec ) const
    {
      return vec.inv_transformation(*this);
    }

    /* Inverse transform 3D vector function */
    TVector<TYPE> inv_transform_vector( TVector<TYPE> const &vec ) const
    {
      return
         TVector<TYPE>(vec.x * inv_matrix.M[0][0] + vec.y * inv_matrix.M[1][0] +
                       vec.z * inv_matrix.M[2][0],
                       vec.x * inv_matrix.M[0][1] + vec.y * inv_matrix.M[1][1] +
                       vec.z * inv_matrix.M[2][1],
                       vec.x * inv_matrix.M[0][2] + vec.y * inv_matrix.M[1][2] +
                       vec.z * inv_matrix.M[2][2]);
    }

    /* Inverse transform 3D normal (surface perpendicular) function */
    TVector<TYPE> Inv_transform_normal( TVector<TYPE> const &vec ) const
    {
      return TVector<TYPE>(vec.x * matrix.M[0][0] + vec.y * matrix.M[0][1] +
                           vec.z * matrix.M[0][2],
                           vec.x * matrix.M[1][0] + vec.y * matrix.M[1][1] +
                           vec.z * matrix.M[1][2],
                           vec.x * matrix.M[2][0] + vec.y * matrix.M[2][1] +
                           vec.z * matrix.M[2][2]).Normalize();
    }

    /***
     * Set transform to specified transformation functions
     ***/

    /* Reset transformation transform data function */
    TTransform & set_unit( void )
    {
      matrix.set_unit();
      inv_matrix.set_unit();
      return *this;
    }

    /* Set translation transform function */
    TTransform & set_translate( TYPE dx, TYPE dy, TYPE dz )
    {
      matrix.set_translate(dx, dy, dz);
      inv_matrix.set_translate(-dx, -dy, -dz);
      return *this;
    }

    /* Set translation transform function */
    TTransform & set_translate( TVector<TYPE> const &shift )
    {
      return set_translate(shift.x, shift.y, shift.z);
    }

    /* Set rotation around 'x' axis transform function */
    TTransform & set_rotate_x( TYPE angle_sine, TYPE angle_cosine )
    {
      matrix.set_rotate_x(angle_sine, angle_cosine);
      inv_matrix.set_rotate_x(-angle_sine, angle_cosine);
      return *this;
    }

    /* Set rotation around 'x' axis transform function */
    TTransform & set_rotate_x( TYPE angle_in_degree )
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
      fstp sine_val      /* sin -> sine_val */
    }
#else /* __NOUSE_SINCOS_ASM__ */
    sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return set_rotate_x(sine_val, cosine_val);
    }

    /* Set rotation around 'y' axis transform function */
    TTransform & set_rotate_y( TYPE angle_sine, TYPE angle_cosine )
    {
      matrix.set_rotate_y(angle_sine, angle_cosine);
      inv_matrix.set_rotate_y(-angle_sine, angle_cosine);
      return *this;
    }

    /* Set rotation around 'y' axis transform function */
    TTransform & set_rotate_y( TYPE angle_in_degree )
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
      fstp sine_val      /* sin -> sine_val */
    }
#else /* __NOUSE_SINCOS_ASM__ */
    sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return set_rotate_y(sine_val, cosine_val);
    }

    /* Set rotation around 'z' axis transform function */
    TTransform & set_rotate_z( TYPE angle_sine, TYPE angle_cosine )
    {
      matrix.set_rotate_z(angle_sine, angle_cosine);
      inv_matrix.set_rotate_z(-angle_sine, angle_cosine);
      return *this;
    }

    /* Set rotation around 'z' axis transform function */
    TTransform & set_rotate_z( TYPE angle_in_degree )
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
      fstp sine_val      /* sin -> sine_val */
    }
#else /* __NOUSE_SINCOS_ASM__ */
    sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return set_rotate_z(sine_val, cosine_val);
    }

    /* Set rotation around arbitrary axis transform function */
    TTransform & set_rotate( TYPE angle_in_degree,
                             TYPE axis_x, TYPE axis_y, TYPE axis_z )
    {
      matrix.set_rotate(angle_in_degree, axis_x, axis_y, axis_z);
      inv_matrix.M[0][0] = matrix.M[0][0];
      inv_matrix.M[0][1] = matrix.M[1][0];
      inv_matrix.M[0][2] = matrix.M[2][0];
      inv_matrix.M[0][3] = 0;
      inv_matrix.M[1][0] = matrix.M[0][1];
      inv_matrix.M[1][1] = matrix.M[1][1];
      inv_matrix.M[1][2] = matrix.M[2][1];
      inv_matrix.M[1][3] = 0;
      inv_matrix.M[2][0] = matrix.M[0][2];
      inv_matrix.M[2][1] = matrix.M[1][2];
      inv_matrix.M[2][2] = matrix.M[2][2];
      inv_matrix.M[2][3] = 0;
      inv_matrix.M[3][0] = 0;
      inv_matrix.M[3][1] = 0;
      inv_matrix.M[3][2] = 0;
      inv_matrix.M[3][3] = 1;
      return *this;
    }

    /* Set rotation around arbitrary axis transform function */
    TTransform & set_rotate( TYPE angle_sine, TYPE angle_cosine,
                             TYPE axis_x, TYPE axis_y, TYPE axis_z )
    {
      return
        set_rotate(Rad2Deg(atan2(angle_sine, angle_cosine)), axis_x, axis_y, axis_z);
    }

    /* Set rotation around arbitrary axis transform function */
    TTransform & set_rotate( TYPE angle_sine, TYPE angle_cosine,
                             TVector<TYPE> const &vec )
    {
      return set_rotate(angle_sine, angle_cosine, vec.x, vec.y, vec.z);
    }

    /* Set rotation around arbitrary axis transform function */
    TTransform & set_rotate( TYPE angle_in_degree, TVector<TYPE> const &vec )
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

      return set_rotate(sine_val, cosine_val, vec.x, vec.y, vec.z);
    }

    /* Set scaling along axes transform function */
    TTransform & set_scale( TYPE sx, TYPE sy, TYPE sz )
    {
      matrix.set_scale(sx, sy, sz);
      inv_matrix.set_scale(1 / sx, 1 / sy, 1 / sz);
      return *this;
    }

    /* Set scaling along axes transform function */
    TTransform & set_scale( TVector<TYPE> const &scale_vec )
    {
      return set_scale(scale_vec.x, scale_vec.y, scale_vec.z);
    }

    /***
     * Apply specified transformation to transform (self-transform) functions
     ***/

    /* Translate transform function */
    TTransform & translate( TYPE dx, TYPE dy, TYPE dz )
    {
      matrix.translate(dx, dy, dz);

      inv_matrix.M[3][0] -= dx * inv_matrix.M[0][0] + dy * inv_matrix.M[1][0] + dz * inv_matrix.M[2][0];
      inv_matrix.M[3][1] -= dx * inv_matrix.M[0][1] + dy * inv_matrix.M[1][1] + dz * inv_matrix.M[2][1];
      inv_matrix.M[3][2] -= dx * inv_matrix.M[0][2] + dy * inv_matrix.M[1][2] + dz * inv_matrix.M[2][2];
      return *this;
    }

    /* Translate transform function */
    TTransform & translate( TVector<TYPE> const &shift )
    {
      return translate(shift.x, shift.y, shift.z);
    }

    /* Rotate around 'x' axis transform function */
    TTransform & rotate_x( TYPE angle_sine, TYPE angle_cosine )
    {
      TYPE a, b, c;

      matrix.rotate_x(angle_sine, angle_cosine);

      /* Pre-concatenating new inverse matrix before old one */
      a = angle_cosine * inv_matrix.M[1][0] - angle_sine * inv_matrix.M[2][0];
      b = angle_cosine * inv_matrix.M[1][1] - angle_sine * inv_matrix.M[2][1];
      c = angle_cosine * inv_matrix.M[1][2] - angle_sine * inv_matrix.M[2][2];

      inv_matrix.M[2][0] = angle_cosine * inv_matrix.M[2][0] + angle_sine * inv_matrix.M[1][0];
      inv_matrix.M[2][1] = angle_cosine * inv_matrix.M[2][1] + angle_sine * inv_matrix.M[1][1];
      inv_matrix.M[2][2] = angle_cosine * inv_matrix.M[2][2] + angle_sine * inv_matrix.M[1][2];

      inv_matrix.M[1][0] = a;
      inv_matrix.M[1][1] = b;
      inv_matrix.M[1][2] = c;

      return *this;
    }

    /* Rotate around 'x' axis transform function */
    TTransform & rotate_x( TYPE angle_in_degree )
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
      fstp sine_val      /* sin -> sine_val */
    }
#else /* __NOUSE_SINCOS_ASM__ */
    sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotate_x(sine_val, cosine_val);
    }

    /* Rotate around 'y' axis transform function */
    TTransform & rotate_y( TYPE angle_sine, TYPE angle_cosine )
    {
      TYPE a, b, c;

      matrix.rotate_y(angle_sine, angle_cosine);

      /* Pre-concatenating new inverse matrix before old one */
      a = angle_cosine * inv_matrix.M[0][0] + angle_sine * inv_matrix.M[2][0];
      b = angle_cosine * inv_matrix.M[0][1] + angle_sine * inv_matrix.M[2][1];
      c = angle_cosine * inv_matrix.M[0][2] + angle_sine * inv_matrix.M[2][2];

      inv_matrix.M[2][0] = angle_cosine * inv_matrix.M[2][0] - angle_sine * inv_matrix.M[0][0];
      inv_matrix.M[2][1] = angle_cosine * inv_matrix.M[2][1] - angle_sine * inv_matrix.M[0][1];
      inv_matrix.M[2][2] = angle_cosine * inv_matrix.M[2][2] - angle_sine * inv_matrix.M[0][2];

      inv_matrix.M[0][0] = a;
      inv_matrix.M[0][1] = b;
      inv_matrix.M[0][2] = c;

      return *this;
    }

    /* Rotate around 'y' axis transform function */
    TTransform & rotate_y( TYPE angle_in_degree )
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
      fstp sine_val      /* sin -> sine_val */
    }
#else /* __NOUSE_SINCOS_ASM__ */
    sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotate_y(sine_val, cosine_val);
    }

    /* Rotate around 'z' axis transform function */
    TTransform & rotate_z( TYPE angle_sine, TYPE angle_cosine )
    {
      TYPE a, b, c;

      matrix.rotate_z(angle_sine, angle_cosine);

      /* Pre-concatenating new inverse matrix before old one */
      a = angle_cosine * inv_matrix.M[0][0] - angle_sine * inv_matrix.M[1][0];
      b = angle_cosine * inv_matrix.M[0][1] - angle_sine * inv_matrix.M[1][1];
      c = angle_cosine * inv_matrix.M[0][2] - angle_sine * inv_matrix.M[1][2];

      inv_matrix.M[1][0] = angle_cosine * inv_matrix.M[1][0] + angle_sine * inv_matrix.M[0][0];
      inv_matrix.M[1][1] = angle_cosine * inv_matrix.M[1][1] + angle_sine * inv_matrix.M[0][1];
      inv_matrix.M[1][2] = angle_cosine * inv_matrix.M[1][2] + angle_sine * inv_matrix.M[0][2];

      inv_matrix.M[0][0] = a;
      inv_matrix.M[0][1] = b;
      inv_matrix.M[0][2] = c;

      return *this;
    }

    /* Rotate around 'z' axis transform function */
    TTransform & rotate_z( TYPE angle_in_degree )
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
      fstp sine_val      /* sin -> sine_val */
    }
#else /* __NOUSE_SINCOS_ASM__ */
    sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotate_z(sine_val, cosine_val);
    }

    /* Rotate around arbitrary axis transform function */
    TTransform & rotate( TYPE angle_in_degree,
                         TYPE axis_x, TYPE axis_y, TYPE axis_z )
    {
      TYPE Rm[3][3], Tm[4][4];

      BuildRotateMatrix3x3<TYPE, 3>(Rm, angle_in_degree, axis_x, axis_y, axis_z);

      Tm[0][0] = matrix.M[0][0] * Rm[0][0] + matrix.M[0][1] * Rm[1][0] +
        matrix.M[0][2] * Rm[2][0];
      Tm[1][0] = matrix.M[1][0] * Rm[0][0] + matrix.M[1][1] * Rm[1][0] +
        matrix.M[1][2] * Rm[2][0];
      Tm[2][0] = matrix.M[2][0] * Rm[0][0] + matrix.M[2][1] * Rm[1][0] +
        matrix.M[2][2] * Rm[2][0];
      Tm[3][0] = matrix.M[3][0] * Rm[0][0] + matrix.M[3][1] * Rm[1][0] +
        matrix.M[3][2] * Rm[2][0];

      Tm[0][1] = matrix.M[0][0] * Rm[0][1] + matrix.M[0][1] * Rm[1][1] +
        matrix.M[0][2] * Rm[2][1];
      Tm[1][1] = matrix.M[1][0] * Rm[0][1] + matrix.M[1][1] * Rm[1][1] +
        matrix.M[1][2] * Rm[2][1];
      Tm[2][1] = matrix.M[2][0] * Rm[0][1] + matrix.M[2][1] * Rm[1][1] +
        matrix.M[2][2] * Rm[2][1];
      Tm[3][1] = matrix.M[3][0] * Rm[0][1] + matrix.M[3][1] * Rm[1][1] +
        matrix.M[3][2] * Rm[2][1];

      Tm[0][2] = matrix.M[0][0] * Rm[0][2] + matrix.M[0][1] * Rm[1][2] +
        matrix.M[0][2] * Rm[2][2];
      Tm[1][2] = matrix.M[1][0] * Rm[0][2] + matrix.M[1][1] * Rm[1][2] +
        matrix.M[1][2] * Rm[2][2];
      Tm[2][2] = matrix.M[2][0] * Rm[0][2] + matrix.M[2][1] * Rm[1][2] +
        matrix.M[2][2] * Rm[2][2];
      Tm[3][2] = matrix.M[3][0] * Rm[0][2] + matrix.M[3][1] * Rm[1][2] +
        matrix.M[3][2] * Rm[2][2];

      Tm[0][3] = 0;
      Tm[1][3] = 0;
      Tm[2][3] = 0;
      Tm[3][3] = 1;

      memcpy(matrix.M, Tm, sizeof(matrix));

      Tm[0][0] = Rm[0][0] * inv_matrix.M[0][0] + Rm[0][1] * inv_matrix.M[1][0] +
        Rm[0][2] * inv_matrix.M[2][0];
      Tm[0][1] = Rm[0][0] * inv_matrix.M[0][1] + Rm[0][1] * inv_matrix.M[1][1] +
        Rm[0][2] * inv_matrix.M[2][1];
      Tm[0][2] = Rm[0][0] * inv_matrix.M[0][2] + Rm[0][1] * inv_matrix.M[1][2] +
        Rm[0][2] * inv_matrix.M[2][2];

      Tm[1][0] = Rm[1][0] * inv_matrix.M[0][0] + Rm[1][1] * inv_matrix.M[1][0] +
        Rm[1][2] * inv_matrix.M[2][0];
      Tm[1][1] = Rm[1][0] * inv_matrix.M[0][1] + Rm[1][1] * inv_matrix.M[1][1] +
        Rm[1][2] * inv_matrix.M[2][1];
      Tm[1][2] = Rm[1][0] * inv_matrix.M[0][2] + Rm[1][1] * inv_matrix.M[1][2] +
        Rm[1][2] * inv_matrix.M[2][2];

      Tm[2][0] = Rm[2][0] * inv_matrix.M[0][0] + Rm[2][1] * inv_matrix.M[1][0] +
        Rm[2][2] * inv_matrix.M[2][0];
      Tm[2][1] = Rm[2][0] * inv_matrix.M[0][1] + Rm[2][1] * inv_matrix.M[1][1] +
        Rm[2][2] * inv_matrix.M[2][1];
      Tm[2][2] = Rm[2][0] * inv_matrix.M[0][2] + Rm[2][1] * inv_matrix.M[1][2] +
        Rm[2][2] * inv_matrix.M[2][2];

      inv_matrix.M[0][0] = Tm[0][0];
      inv_matrix.M[0][1] = Tm[0][1];
      inv_matrix.M[0][2] = Tm[0][2];

      inv_matrix.M[1][0] = Tm[1][0];
      inv_matrix.M[1][1] = Tm[1][1];
      inv_matrix.M[1][2] = Tm[1][2];

      inv_matrix.M[2][0] = Tm[2][0];
      inv_matrix.M[2][1] = Tm[2][1];
      inv_matrix.M[2][2] = Tm[2][2];

      return *this;
    }

    /* Rotate around arbitrary axis transform function */
    TTransform & rotate( TYPE angle_sine, TYPE angle_cosine,
                         TYPE axis_x, TYPE axis_y, TYPE axis_z )
    {
      return
        rotate(Rad2Deg(atan2(angle_sine, angle_cosine)), axis_x, axis_y, axis_z);
    }

    /* Rotate around arbitrary axis transform function */
    TTransform & rotate( TYPE angle_sine, TYPE angle_cosine,
                         TVector<TYPE> const &vec )
    {
      return rotate(angle_sine, angle_cosine, vec.x, vec.y, vec.z);
    }

    /* Rotate around arbitrary axis transform function */
    TTransform & rotate( TYPE angle_in_degree, TVector<TYPE> const &vec )
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
      fstp sine_val      /* sin -> sine_val */
    }
#else /* __NOUSE_SINCOS_ASM__ */
    sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotate(sine_val, cosine_val, vec.x, vec.y, vec.z);
    }

    /* Scale along axes transform function */
    TTransform & scale(TYPE s)
    {
      return scale(s, s, s);
    }

    TTransform & scale( TYPE sx, TYPE sy, TYPE sz )
    {
      matrix.scale(sx, sy, sz);
      
      inv_matrix.M[0][0] /= sx;
      inv_matrix.M[0][1] /= sx;
      inv_matrix.M[0][2] /= sx;

      inv_matrix.M[1][0] /= sy;
      inv_matrix.M[1][1] /= sy;
      inv_matrix.M[1][2] /= sy;

      inv_matrix.M[2][0] /= sz;
      inv_matrix.M[2][1] /= sz;
      inv_matrix.M[2][2] /= sz;

      return *this;
    }

    /* Scale along axes transform function */
    TTransform & scale( TVector<TYPE> const &scale_vec )
    {
      return scale(scale_vec.x, scale_vec.y, scale_vec.z);
    }

    /* Transform transform by specified transformation function */
    TTransform & transform( const TTransform<TYPE> &trans )
    {
      matrix.transform(trans);
      inv_matrix.inv_transform(trans);
      return *this;
    }

    /* Inverse transform transform by specified transformation function */
    TTransform & inv_transform( const TTransform<TYPE> &trans )
    {
      matrix.inv_transform(trans);
      inv_matrix.transform(trans);
      return *this;
    }

    /***
     * Apply specified transformation to transform functions
     ***/

    /* Translation transform function */
    TTransform translation( TYPE dx, TYPE dy, TYPE dz ) const
    {
      TTransform res(*this);

      return res.translate(dx, dy, dz);
    }

    /* Translation transform function */
    TTransform translation( TVector<TYPE> const &shift ) const
    {
      return translation(shift.x, shift.y, shift.z);
    }

    /* Rotation around 'x' axis transform function */
    TTransform rotation_x( TYPE angle_sine, TYPE angle_cosine ) const
    {
      TTransform res(*this);

      return res.rotate_x(angle_sine, angle_cosine);
    }

    /* Rotation around 'x' axis transform function */
    TTransform rotation_x( TYPE angle_in_degree ) const
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
      fstp sine_val      /* sin -> sine_val */
    }
#else /* __NOUSE_SINCOS_ASM__ */
    sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotation_x(sine_val, cosine_val);
    }

    /* Rotation around 'y' axis transform function */
    TTransform rotation_y( TYPE angle_sine, TYPE angle_cosine ) const
    {
      TTransform res(*this);

      return res.rotate_y(angle_sine, angle_cosine);
    }

    /* Rotation around 'y' axis transform function */
    TTransform rotation_y( TYPE angle_in_degree ) const
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
      fstp sine_val      /* sin -> sine_val */
    }
#else /* __NOUSE_SINCOS_ASM__ */
    sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotation_y(sine_val, cosine_val);
    }

    /* Rotation around 'z' axis transform function */
    TTransform rotation_z( TYPE angle_sine, TYPE angle_cosine ) const
    {
      TTransform res(*this);

      return res.rotate_z(angle_sine, angle_cosine);
    }

    /* Rotation around 'z' axis transform function */
    TTransform rotation_z( TYPE angle_in_degree ) const
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
      fstp sine_val      /* sin -> sine_val */
    }
#else /* __NOUSE_SINCOS_ASM__ */
    sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotation_z(sine_val, cosine_val);
    }

    /* Rotation around arbitrary axis transform function */
    TTransform rotation( TYPE angle_in_degree,
                         TYPE axis_x, TYPE axis_y, TYPE axis_z ) const
    {
      TTransform res(*this);

      return res.rotate(angle_in_degree, axis_x, axis_y, axis_z);
    }

    /* Rotation around arbitrary axis transform function */
    TTransform rotation( TYPE angle_sine, TYPE angle_cosine,
                         TYPE axis_x, TYPE axis_y, TYPE axis_z ) const
    {
      return
        rotation(Rad2Deg(atan2(angle_sine, angle_cosine)), axis_x, axis_y, axis_z);
    }

    /* Rotation around arbitrary axis transform function */
    TTransform rotation( TYPE angle_sine, TYPE angle_cosine, TVector<TYPE> const &vec ) const
    {
      return rotation(angle_sine, angle_cosine, vec.x, vec.y, vec.z);
    }

    /* Rotation around arbitrary axis transform function */
    TTransform rotation( TYPE angle_in_degree, TVector<TYPE> const &vec ) const
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
      fstp sine_val      /* sin -> sine_val */
    }
#else /* __NOUSE_SINCOS_ASM__ */
    sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */

      return rotation(sine_val, cosine_val, vec.x, vec.y, vec.z);
    }

    /* Scaling along axes transform function */
    TTransform scaling( TYPE sx, TYPE sy, TYPE sz ) const
    {
      TTransform res(*this);

      return res.scale(sx, sy, sz);
    }

    /* Scaling along axes transform function */
    TTransform scaling( TVector<TYPE> const &scale_vec ) const
    {
      return scaling(scale_vec.x, scale_vec.y, scale_vec.z);
    }

    /* Transformation transform by specified transformation function */
    TTransform transformation( const TTransform<TYPE> &trans ) const
    {
      TTransform res(*this);

      return res.transform(trans);
    }

    /* Inverse transformation transform by specified transformation function */
    TTransform inv_transformation( const TTransform<TYPE> &trans ) const
    {
      TTransform res(*this);

      return res.inv_transform(trans);
    }

    /* Multiplication of two transformations function */
    TTransform operator*( const TTransform &trans ) const
    {
      return transformation(trans);
    }

    /* Multiplication of two transformations function */
    TTransform & operator*=( const TTransform &trans )
    {
      return transform(trans);
    }
  };
}

#endif /* __CGLMATHTRANSFORM_INCLUDED__ */