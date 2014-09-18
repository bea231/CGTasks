/**
@file     cglMathDef.h
@brief    Mathematics for computer graphics basic definition module
@date     Created on 12/09/2014
@project  Task1
@author   Sergeev Artemiy
*/

#ifndef __CGLMATHDEF_INCLUDED__
#define __CGLMATHDEF_INCLUDED__

#include <math.h>

#ifndef __NO_USE_SINCOS_ASM__
#define __USE_SINCOS_ASM__
#endif /* __NO_USE_SINCOS_ASM__ */

namespace cglmath
{
  /* Useful constants */
  const double c_degree2radian = 0.01745329251994329576;
  const double c_radian2degree = 57.29577951308232087684;
  const double c_pi = 3.14159265358979323846;
  const double c_e = 2.71828182845904523536;
  const double c_sqrt2 = 1.41421356237309504880;
  const double c_threshold = 1e-6;

  const float c_degree2radianf = 0.0174533f;
  const float c_radian2degreef = 57.2957795f;
  const float c_pif            = 3.1415927f;
  const float c_ef             = 2.7182818f;
  const float c_sqrt2f         = 1.4142136f;
  const float c_thresholdf     = 1e-6f;

  /* Forward class references */
  template<class TYPE> class TVector;
  template<class TYPE> class TMatrix;
  template<class TYPE> class TTransform;
  template<class TYPE> class TCamera;

  /***
  * Helpful functions
  ***/

  /* Convert degrees to radians function */ 
  template<class TYPE> TYPE Deg2Rad( TYPE angle_in_degree )
  {
    return (TYPE)(angle_in_degree * c_degree2radian);
  }

  /* Convert radians to degrees function */
  template<class TYPE> TYPE Rad2Deg( TYPE angle_in_radian )
  {
    return (TYPE)(angle_in_radian * c_radian2degree);
  }

  /* Minimum of two elements function */
  template<class TYPE> TYPE Min( TYPE X, TYPE Y )
  {
    return X < Y ? X : Y;
  }

  /* Minimum of three elements function */
  template<class TYPE> TYPE Min3( TYPE X, TYPE Y, TYPE Z )
  {
    if (X < Y)
      if (X < Z)
        return X;
      else
        return Z;
    else
      if (Y < Z)
        return Y;
      else
        return Z;
  }

  /* Maximum of two elements function */
  template<class TYPE> TYPE Max( TYPE X, TYPE Y )
  {
    return X > Y ? X : Y;
  }

  /* Maximum of three elements function */
  template<class TYPE> TYPE Max3( TYPE X, TYPE Y, TYPE Z )
  {
    if (X > Y)
      if (X > Z)
        return X;
      else
        return Z;
    else
      if (Y > Z)
        return Y;
      else
        return Z;
  }

  /* Absolute value evaluation function */
  template<class TYPE> TYPE Abs( TYPE X )
  {
    return X < 0 ? -X : X;
  }

  /* Swap two value function */
  template<class TYPE> void Swap( TYPE &A, TYPE &B )
  {
    TYPE tmp = A;

    A = B;
    B = tmp;
  }

  /* Linear interpolation function */
  template<class TYPE, class TYPE_CLS>
  TYPE_CLS Lerp( TYPE Val, TYPE_CLS Y0, TYPE_CLS Y1 )
  {
    return Y0 * (1 - Val) + Y1 * Val;
  }

  /* Linear interpolation function */
  template<class TYPE, class TYPE_CLS>
  TYPE_CLS Lerp( TYPE Val, TYPE X0, TYPE X1, TYPE_CLS Y0, TYPE_CLS Y1 )
  {
    TYPE t;

    if (X0 <= X1)
    {
      if (Val <= X0)
        return Y0;
      if (Val >= X1)
        return Y1;

      t = (Val - X0) / (X1 - X0);
      return Y0 * (1 - t) + Y1 * t;
    }
    if (Val >= X0)
      return Y0;
    if (Val <= X1)
      return Y1;

    t = (Val - X1) / (X0 - X1);
    return Y0 * (1 - t) + Y1 * t;
  }

  /* Clamp value function */
  template<class TYPE>
  TYPE Clamp(TYPE Val, TYPE Min, TYPE Max)
  {
    if (Val < Min)
      return Min;
    else if (Val > Max)
      return Max;
    else
      return Val;
  }

  /* Matrix 3x3 determinant evaluation function */
  template<class TYPE>
  TYPE Determinant3x3( const TYPE M11, const TYPE M12, const TYPE M13,
                       const TYPE M21, const TYPE M22, const TYPE M23,
                       const TYPE M31, const TYPE M32, const TYPE M33 )
  {
    return M11 * M22 * M33 + M12 * M23 * M31 + M13 * M21 * M32 -
      M11 * M23 * M32 - M12 * M21 * M33 - M13 * M22 * M31;
  }

  /* Construct 3x3 matrix of rotation around arbitrary axis function */
  template<class TYPE, INT N>
  VOID BuildRotateMatrix3x3(TYPE RotMatr[N][N], const TYPE AngleInDegree,
    const TYPE AxisX, const TYPE AxisY, const TYPE AxisZ)
  {
    TYPE
      s, h, vx, vy, vz, len, Angle;

    len = AxisX * AxisX + AxisY * AxisY + AxisZ * AxisZ;

    Angle = AngleInDegree / 2;

#ifdef __USE_SINCOS_ASM__
    _asm {
      /* FST(0) Angle (from degree to radian) */
      fld  Angle
        fmul c_degree2radian

        /* FST(0) - cos, FST(1) - sin */
        fsincos

        fstp s    /* cos -> cosine */
        fstp h    /* sin -> sine */
    }
#else /* __USE_SINCOS_ASM__ */
    s = cos(AngleInDegree);
    h = sin(AngleInDegree);
#endif /* __USE_SINCOS_ASM__ */

    if (Abs(len) > c_threshold && Abs(len - 1) > c_threshold)
    {
      len = sqrt(len);
      vx = AxisX * h / len;
      vy = AxisY * h / len;
      vz = AxisZ * h / len;
    }
    else
    {
      vx = h * AxisX;
      vy = h * AxisY;
      vz = h * AxisZ;
    }

    RotMatr[0][0] = 1 - 2 * (vy * vy + vz * vz);
    RotMatr[0][1] = 2 * vx * vy - 2 * s * vz;
    RotMatr[0][2] = 2 * s * vy + 2 * vx * vz;

    RotMatr[1][0] = 2 * vx * vy + 2 * s * vz;
    RotMatr[1][1] = 1 - 2 * (vx * vx + vz * vz);
    RotMatr[1][2] = -2 * s * vx + 2 * vy * vz;

    RotMatr[2][0] = -2 * s * vy + 2 * vx * vz;
    RotMatr[2][1] = 2 * s * vx + 2 * vy * vz;
    RotMatr[2][2] = 1 - 2 * (vx * vx + vy * vy);
  }

  template<class TYPE>
  void get_sin_cos( TYPE const angle_in_radians, TYPE &sine, TYPE &cosine )
  {
    TYPE s, h;
#ifdef __USE_SINCOS_ASM__
    _asm {
      /* FST(0) Angle (from degree to radian) */
      fld  angle_in_radians


      /* FST(0) - cos, FST(1) - sin */
      fsincos

      fstp s    /* cos -> cosine */
      fstp h      /* sin -> sine */
    }
#else /* __USE_SINCOS_ASM__ */
    s = cos(AngleInDegree);
    h = sin(AngleInDegree);
#endif /* __USE_SINCOS_ASM__ */
    sine = h;
    cosine = s;
  }
}

#endif /* __CGLMATHDEF_INCLUDED__ */