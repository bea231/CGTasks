/**
@file     cglMathMatrix.h
@brief    Mathematics for computer graphics matrix declaration module
@date     Created on 12/09/2014
@project  Task1
@author   Sergeev Artemiy
*/

#ifndef __CGLMATHMATRIX_INCLUDED__
#define __CGLMATHMATRIX_INCLUDED__

#include <string.h>
#include <math.h>

#include "cglmathdef.h"

namespace cglmath
{
  /* Base matrix of affine transformation class template declaration */
  template<class TYPE> class TMatrix
  {
  public:
    /* Global unit (identity) matrix for optimization */
    static const TYPE UnitMatrix[4][4];

    /* Matrix 4x4 data array
     * Matrix view (always affine transformation representation):
     *   a11 a12 a13 0
     *   a21 a22 a23 0
     *   a31 a32 a33 0
     *   a41 a42 a43 1
     */
    TYPE M[4][4];

    TMatrix( void ) {}

    /* Constructor by matrix */
    TMatrix( const TMatrix &matr )
    {
      memcpy(M, &matr.M, sizeof(M));
    }

    /* Constructor by components */
    TMatrix( TYPE M00, TYPE M01, TYPE M02, 
             TYPE M10, TYPE M11, TYPE M12, 
             TYPE M20, TYPE M21, TYPE M22, 
             TYPE M30 = 0, TYPE M31 = 0, TYPE M32 = 0,
             TYPE M03 = 0, TYPE M13 = 0, TYPE M23 = 0,
             TYPE M33 = 1 )
    {
      M[0][0] = M00;
      M[0][1] = M01;
      M[0][2] = M02;
      M[0][3] = M03;
      M[1][0] = M10;
      M[1][1] = M11;
      M[1][2] = M12;
      M[1][3] = M13;
      M[2][0] = M20;
      M[2][1] = M21;
      M[2][2] = M22;
      M[2][3] = M23;
      M[3][0] = M30;
      M[3][1] = M31;
      M[3][2] = M32;
      M[3][3] = M33;
    }

    /* Per components multiplication of two matrices function */
    TMatrix operator*( const TMatrix &matr ) const
    {
      TMatrix ResMatr;

      ResMatr.M[0][0] = M[0][0] * matr.M[0][0] + M[0][1] * matr.M[1][0] +
        M[0][2] * matr.M[2][0];
      ResMatr.M[1][0] = M[1][0] * matr.M[0][0] + M[1][1] * matr.M[1][0] +
        M[1][2] * matr.M[2][0];
      ResMatr.M[2][0] = M[2][0] * matr.M[0][0] + M[2][1] * matr.M[1][0] +
        M[2][2] * matr.M[2][0];
      ResMatr.M[3][0] = M[3][0] * matr.M[0][0] + M[3][1] * matr.M[1][0] +
        M[3][2] * matr.M[2][0] + matr.M[3][0];

      ResMatr.M[0][1] = M[0][0] * matr.M[0][1] + M[0][1] * matr.M[1][1] +
        M[0][2] * matr.M[2][1];
      ResMatr.M[1][1] = M[1][0] * matr.M[0][1] + M[1][1] * matr.M[1][1] +
        M[1][2] * matr.M[2][1];
      ResMatr.M[2][1] = M[2][0] * matr.M[0][1] + M[2][1] * matr.M[1][1] +
        M[2][2] * matr.M[2][1];
      ResMatr.M[3][1] = M[3][0] * matr.M[0][1] + M[3][1] * matr.M[1][1] +
        M[3][2] * matr.M[2][1] + matr.M[3][1];

      ResMatr.M[0][2] = M[0][0] * matr.M[0][2] + M[0][1] * matr.M[1][2] +
        M[0][2] * matr.M[2][2];
      ResMatr.M[1][2] = M[1][0] * matr.M[0][2] + M[1][1] * matr.M[1][2] +
        M[1][2] * matr.M[2][2];
      ResMatr.M[2][2] = M[2][0] * matr.M[0][2] + M[2][1] * matr.M[1][2] +
        M[2][2] * matr.M[2][2];
      ResMatr.M[3][2] = M[3][0] * matr.M[0][2] + M[3][1] * matr.M[1][2] +
        M[3][2] * matr.M[2][2] + matr.M[3][2];

      ResMatr.M[0][3] = 0;
      ResMatr.M[1][3] = 0;
      ResMatr.M[2][3] = 0;
      ResMatr.M[3][3] = 1;

      return ResMatr;
    }

    /* Per components multiplication of two matrices with assignment function */
    TMatrix & operator*=( const TMatrix &matr )
    {
      TMatrix ResMatr;

      ResMatr.M[0][0] = M[0][0] * matr.M[0][0] + M[0][1] * matr.M[1][0] +
        M[0][2] * matr.M[2][0];
      ResMatr.M[1][0] = M[1][0] * matr.M[0][0] + M[1][1] * matr.M[1][0] +
        M[1][2] * matr.M[2][0];
      ResMatr.M[2][0] = M[2][0] * matr.M[0][0] + M[2][1] * matr.M[1][0] +
        M[2][2] * matr.M[2][0];
      ResMatr.M[3][0] = M[3][0] * matr.M[0][0] + M[3][1] * matr.M[1][0] +
        M[3][2] * matr.M[2][0] + matr.M[3][0];

      ResMatr.M[0][1] = M[0][0] * matr.M[0][1] + M[0][1] * matr.M[1][1] +
        M[0][2] * matr.M[2][1];
      ResMatr.M[1][1] = M[1][0] * matr.M[0][1] + M[1][1] * matr.M[1][1] +
        M[1][2] * matr.M[2][1];
      ResMatr.M[2][1] = M[2][0] * matr.M[0][1] + M[2][1] * matr.M[1][1] +
        M[2][2] * matr.M[2][1];
      ResMatr.M[3][1] = M[3][0] * matr.M[0][1] + M[3][1] * matr.M[1][1] +
        M[3][2] * matr.M[2][1] + matr.M[3][1];

      ResMatr.M[0][2] = M[0][0] * matr.M[0][2] + M[0][1] * matr.M[1][2] +
        M[0][2] * matr.M[2][2];
      ResMatr.M[1][2] = M[1][0] * matr.M[0][2] + M[1][1] * matr.M[1][2] +
        M[1][2] * matr.M[2][2];
      ResMatr.M[2][2] = M[2][0] * matr.M[0][2] + M[2][1] * matr.M[1][2] +
        M[2][2] * matr.M[2][2];
      ResMatr.M[3][2] = M[3][0] * matr.M[0][2] + M[3][1] * matr.M[1][2] +
        M[3][2] * matr.M[2][2] + matr.M[3][2];

      ResMatr.M[0][3] = 0;
      ResMatr.M[1][3] = 0;
      ResMatr.M[2][3] = 0;
      ResMatr.M[3][3] = 1;

      *this = ResMatr;
      return *this;
    }

    /* Inverse matrix function */
    bool inverse( void )
    {
      TMatrix tmp;
      TYPE determinant;

      /* Obtain inverse matrix by Cramer 's rule */

      /* Get adjoint matrix */
      tmp.M[0][0] = M[1][1] * M[2][2] - M[1][2] * M[2][1];
      tmp.M[0][1] = M[0][2] * M[2][1] - M[0][1] * M[2][2];
      tmp.M[0][2] = M[0][1] * M[1][2] - M[0][2] * M[1][1];
      tmp.M[1][0] = M[1][2] * M[2][0] - M[1][0] * M[2][2];
      tmp.M[1][1] = M[0][0] * M[2][2] - M[0][2] * M[2][0];
      tmp.M[1][2] = M[0][2] * M[1][0] - M[0][0] * M[1][2];
      tmp.M[2][0] = M[1][0] * M[2][1] - M[1][1] * M[2][0];
      tmp.M[2][1] = M[0][1] * M[2][0] - M[0][0] * M[2][1];
      tmp.M[2][2] = M[0][0] * M[1][1] - M[0][1] * M[1][0];
      tmp.M[3][0] =
        -Determinant3x3(M[1][0], M[2][0], M[3][0], M[1][1], M[2][1],
          M[3][1], M[1][2], M[2][2], M[3][2]);
      tmp.M[3][1] =
        Determinant3x3(M[0][0], M[2][0], M[3][0], M[0][1], M[2][1], M[3][1],
          M[0][2], M[2][2], M[3][2]);
      tmp.M[3][2] =
        -Determinant3x3(M[0][0], M[1][0], M[3][0], M[0][1], M[1][1],
          M[3][1], M[0][2], M[1][2], M[3][2]);

      /* Obtain determinant of source left-top 3x3 matrix */
      determinant =
        Determinant3x3(M[0][0], M[0][1], M[0][2], M[1][0], M[1][1], M[1][2],
          M[2][0], M[2][1], M[2][2]);

      /* Check determinant value */
      if (determinant == 0)
        return FALSE;
      /* Copy result matrix */
      *this = tmp;
      if (determinant == 1.0)
        return TRUE;
      M[0][0] /= determinant;
      M[0][1] /= determinant;
      M[0][2] /= determinant;
      M[1][0] /= determinant;
      M[1][1] /= determinant;
      M[1][2] /= determinant;
      M[2][0] /= determinant;
      M[2][1] /= determinant;
      M[2][2] /= determinant;
      M[3][0] /= determinant;
      M[3][1] /= determinant;
      M[3][2] /= determinant;
      return TRUE;
    }

    /* inversing matrix function */
    TMatrix inversing( void ) const
    {
      TMatrix tmp;
      TYPE determinant;

      /* Obtain inverse matrix by Cramer 's rule */

      /* Get adjoint matrix */
      tmp.M[0][0] = M[1][1] * M[2][2] - M[1][2] * M[2][1];
      tmp.M[0][1] = M[0][2] * M[2][1] - M[0][1] * M[2][2];
      tmp.M[0][2] = M[0][1] * M[1][2] - M[0][2] * M[1][1];
      tmp.M[1][0] = M[1][2] * M[2][0] - M[1][0] * M[2][2];
      tmp.M[1][1] = M[0][0] * M[2][2] - M[0][2] * M[2][0];
      tmp.M[1][2] = M[0][2] * M[1][0] - M[0][0] * M[1][2];
      tmp.M[2][0] = M[1][0] * M[2][1] - M[1][1] * M[2][0];
      tmp.M[2][1] = M[0][1] * M[2][0] - M[0][0] * M[2][1];
      tmp.M[2][2] = M[0][0] * M[1][1] - M[0][1] * M[1][0];
      tmp.M[3][0] =
        -Determinant3x3(M[1][0], M[2][0], M[3][0], M[1][1], M[2][1],
          M[3][1], M[1][2], M[2][2], M[3][2]);
      tmp.M[3][1] =
        Determinant3x3(M[0][0], M[2][0], M[3][0], M[0][1], M[2][1], M[3][1],
          M[0][2], M[2][2], M[3][2]);
      tmp.M[3][2] =
        -Determinant3x3(M[0][0], M[1][0], M[3][0], M[0][1], M[1][1],
          M[3][1], M[0][2], M[1][2], M[3][2]);

      tmp.M[0][3] = 0;
      tmp.M[1][3] = 0;
      tmp.M[2][3] = 0;
      tmp.M[3][3] = 1;

      /* Obtain determinant of source left-top 3x3 matrix */
      determinant =
        Determinant3x3(M[0][0], M[0][1], M[0][2], M[1][0], M[1][1], M[1][2],
          M[2][0], M[2][1], M[2][2]);

      /* Check determinant value */
      if (determinant != 0 && determinant != 1.0)
      {
        tmp.M[0][0] /= determinant;
        tmp.M[0][1] /= determinant;
        tmp.M[0][2] /= determinant;
        tmp.M[1][0] /= determinant;
        tmp.M[1][1] /= determinant;
        tmp.M[1][2] /= determinant;
        tmp.M[2][0] /= determinant;
        tmp.M[2][1] /= determinant;
        tmp.M[2][2] /= determinant;
        tmp.M[3][0] /= determinant;
        tmp.M[3][1] /= determinant;
        tmp.M[3][2] /= determinant;
      }
      return tmp;
    }

    /***
     * Set matrix to specified transformation functions
     ***/

    /* Reset transformation matrix data function */
    TMatrix & set_unit( void )
    {
      memcpy(M, UnitMatrix, sizeof(M));
      return *this;
    }

    /* Set translation matrix function */
    TMatrix & set_translate( TYPE dx, TYPE dy, TYPE dz )
    {
      memcpy(M, UnitMatrix, sizeof(M) - 4 * sizeof(TYPE));
      M[3][0] = dx;
      M[3][1] = dy;
      M[3][2] = dz;
      M[3][3] = 1;
      return *this;
    }

    /* Set translation matrix function */
    TMatrix & set_translate( TVector<TYPE> const &shift )
    {
      return set_translate(shift.x, shift.y, shift.z);
    }

    /* Set rotation around 'x' axis matrix function */
    TMatrix & set_rotate_x( TYPE angle_sine, TYPE angle_cosine )
    {
      memcpy(M, UnitMatrix, sizeof(M));
      M[1][1] = angle_cosine;
      M[1][2] = angle_sine;
      M[2][1] = -angle_sine;
      M[2][2] = angle_cosine;
      return *this;
    }

    /* Set rotation around 'x' axis matrix function */
    TMatrix & set_rotate_x( TYPE angle_in_degree )
    {
#ifdef __USE_SINCOS_ASM__
      memcpy(M, UnitMatrix, sizeof(M));
      _asm {
        /* [EBX] is a pointer to 'this' */
        mov  EBX, this

        /* FST(0) Angle (from degree to radian) */
        fld  angle_in_degree
        fmul c_degree2radian

        /* FST(0) - cos, FST(1) - sin */
        fsincos

        fld  st(0)
        //fstp A[1][1] /* cos -> a11 */
        fstp [EBX]TMatrix.M[1 * 16 + 1 * 4]
        //fstp A[2][2] /* cos -> a22 */
        fstp [EBX]TMatrix.M[2 * 16 + 2 * 4]

        fld  st(0)     /* duplicate st(0) */
        //fstp A[1][2] /* sin -> a12 */
        fstp [EBX]TMatrix.M[1 * 16 + 2 * 4]
        fldz           /* push 0 (st(0) = 0) */
        fsubr          /* st(0) = 0 - sin() */
        //fstp A[2][1] /* -sin -> a21 */
        fstp [EBX]TMatrix.M[2 * 16 + 1 * 4]
      }
      return *this;
#else /* __NOUSE_SINCOS_ASM__ */
      double angle_in_radians = Deg2Rad(angle_in_degree);
      TYPE sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);

      return set_rotate_x(sine_val, cosine_val);
#endif /* __USE_SINCOS_ASM__ */
    }

    /* Set rotation around 'y' axis matrix function */
    TMatrix & set_rotate_y( TYPE angle_sine, TYPE angle_cosine )
    {
      memcpy(M, UnitMatrix, sizeof(M));
      M[0][0] = angle_cosine;
      M[0][2] = -angle_sine;
      M[2][0] = angle_sine;
      M[2][2] = angle_cosine;
      return *this;
    }

    /* Set rotation around 'y' axis matrix function */
    TMatrix & set_rotate_y( TYPE angle_in_degree )
    {
#ifdef __USE_SINCOS_ASM__
      memcpy(M, UnitMatrix, sizeof(M));
      _asm {
        /* [EBX] is a pointer to 'this' */
        mov  EBX, this

        /* FST(0) Angle (from degree to radian) */
        fld  angle_in_degree
        fmul c_degree2radian

        /* FST(0) - cos, FST(1) - sin */
        fsincos

        fld  st(0)
        //fstp A[1][1] /* cos -> a00 */
        fstp [EBX]TMatrix.M[0 * 16 + 0 * 4]
        //fstp A[2][2] /* cos -> a22 */
        fstp [EBX]TMatrix.M[2 * 16 + 2 * 4]

        fld  st(0)     /* duplicate st(0) */
        //fstp A[1][2] /* sin -> a20 */
        fstp [EBX]TMatrix.M[2 * 16 + 0 * 4]
        fldz           /* push 0 (st(0) = 0) */
        fsubr          /* st(0) = 0 - sin() */
        //fstp A[2][1] /* -sin -> a02 */
        fstp [EBX]TMatrix.M[0 * 16 + 2 * 4]
      }
      return *this;
#else /* __NOUSE_SINCOS_ASM__ */
      double angle_in_radians = Deg2Rad(angle_in_degree);
      TYPE sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);

      return set_rotate_y(sine_val, cosine_val);
#endif /* __USE_SINCOS_ASM__ */
    }

    /* Set rotation around 'z' axis matrix function */
    TMatrix & set_rotate_z( TYPE angle_sine, TYPE angle_cosine )
    {
      memcpy(M, UnitMatrix, sizeof(M));
      M[0][0] = angle_cosine;
      M[0][1] = angle_sine;
      M[1][0] = -angle_sine;
      M[1][1] = angle_cosine;
      return *this;
    }

    /* Set rotation around 'z' axis matrix function */
    TMatrix & set_rotate_z( TYPE angle_in_degree )
    {
#ifdef __USE_SINCOS_ASM__
      memcpy(M, UnitMatrix, sizeof(M));

      _asm {
        /* [EBX] is a pointer to 'this' */
        mov  EBX, this

        /* FST(0) Angle (from degree to radian) */
        fld  angle_in_degree
        fmul c_degree2radian

        /* FST(0) - cos, FST(1) - sin */
        fsincos

        fld  st(0)
        //fstp A[1][1] /* cos -> a00 */
        fstp [EBX]TMatrix.M[0 * 16 + 0 * 4]
        //fstp A[2][2] /* cos -> a11 */
        fstp [EBX]TMatrix.M[1 * 16 + 1 * 4]

        fld  st(0)     /* duplicate st(0) */
        //fstp A[1][2] /* sin -> a01 */
        fstp [EBX]TMatrix.M[0 * 16 + 1 * 4]
        fldz           /* push 0 (st(0) = 0) */
        fsubr          /* st(0) = 0 - sin() */
        //fstp A[2][1] /* -sin -> a10 */
        fstp [EBX]TMatrix.M[1 * 16 + 0 * 4]
      }
      return *this;
#else /* __NOUSE_SINCOS_ASM__ */
      double angle_in_radians = Deg2Rad(angle_in_degree);
      TYPE sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);

      return set_rotate_x(sine_val, cosine_val);
#endif /* __USE_SINCOS_ASM__ */
    }

    /* Set rotation around arbitrary axis matrix function */
    TMatrix & set_rotate( TYPE angle_in_degree,
                          TYPE axis_x, TYPE axis_y, TYPE axis_z )
    {
      BuildRotateMatrix3x3<TYPE, 4>(M, angle_in_degree, axis_x, axis_y, axis_z);

      M[0][3] = 0;
      M[1][3] = 0;
      M[2][3] = 0;
      M[3][0] = 0;
      M[3][1] = 0;
      M[3][2] = 0;
      M[3][3] = 1;

      return *this;
    }

    /* Set rotation around arbitrary axis matrix function */
    TMatrix & set_rotate( TYPE angle_sine, TYPE angle_cosine,
                          TYPE axis_x, TYPE axis_y, TYPE axis_z )
    {
      return
        set_rotate(Rad2Deg(atan2(angle_sine, angle_cosine)), axis_x, axis_y, axis_z);
    }

    /* Set rotation around arbitrary axis matrix function */
    TMatrix & set_rotate( TYPE angle_sine, TYPE angle_cosine,
                          TVector<TYPE> const &vec )
    {
      return set_rotate(angle_sine, angle_cosine, vec.x, vec.y, vec.z);
    }

    /* Set rotation around arbitrary axis matrix function */
    TMatrix & set_rotate( TYPE angle_in_degree, TVector<TYPE> const &vec )
    {
      return set_rotate(angle_in_degree, vec.x, vec.y, vec.z);
    }

    /* Set scaling along axes matrix function */
    TMatrix & set_scale( TYPE sx, TYPE sy, TYPE sz )
    {
      memcpy(M, UnitMatrix, sizeof(M));
      M[0][0] = sx;
      M[1][1] = sy;
      M[2][2] = sz;
      return *this;
    }

    /* Set scaling along axes matrix function */
    TMatrix & set_scale( TVector<TYPE> const &scale_vec )
    {
      return set_scale(scale_vec.x, scale_vec.y, scale_vec.z);
    }

    /***
     * Apply specified transformation to matrix (self-transform) functions
     ***/

    /* Translate matrix function */
    TMatrix & translate( TYPE dx, TYPE dy, TYPE dz )
    {
      M[3][0] += dx;
      M[3][1] += dy;
      M[3][2] += dz;
      return *this;
    }

    /* Translate matrix function */
    TMatrix & translate( TVector<TYPE> const &shift )
    {
      return translate(shift.x, shift.y, shift.z);
    }

    /* Rotate around 'x' axis matrix function */
    TMatrix & rotate_x( TYPE angle_sine, TYPE angle_cosine )
    {
      TYPE RM01, RM11, RM21, RM31, RM02, RM12, RM22, RM32;

      /* Calculate changing matrix elements */
      RM01 = M[0][1] * angle_cosine - M[0][2] * angle_sine;
      RM11 = M[1][1] * angle_cosine - M[1][2] * angle_sine;
      RM21 = M[2][1] * angle_cosine - M[2][2] * angle_sine;
      RM31 = M[3][1] * angle_cosine - M[3][2] * angle_sine;

      RM02 = M[0][1] * angle_sine + M[0][2] * angle_cosine;
      RM12 = M[1][1] * angle_sine + M[1][2] * angle_cosine;
      RM22 = M[2][1] * angle_sine + M[2][2] * angle_cosine;
      RM32 = M[3][1] * angle_sine + M[3][2] * angle_cosine;

      /* Store changed elements to matrix */
      M[0][1] = RM01;
      M[1][1] = RM11;
      M[2][1] = RM21;
      M[3][1] = RM31;

      M[0][2] = RM02;
      M[1][2] = RM12;
      M[2][2] = RM22;
      M[3][2] = RM32;

      return *this;
    }

    /* Rotate around 'x' axis matrix function */
    TMatrix & rotate_x( TYPE angle_in_degree )
    { 
      TYPE sine_val, cossine_val;

#ifdef __USE_SINCOS_ASM__
      /* Assembler sine and cosine calculate */
      _asm {
        fld angle_in_degree
        fmul c_degree2radian

        fsincos

        fstp cosine_val
        fstp sine_val
      }
#else /* __NOUSE_SINCOS_ASM__ */
      double angle_in_radians = Deg2Rad(angle_in_degree); 

      sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */
      return rotate_x(sine_val, cosine_val);
    }

    /* Rotate around 'y' axis matrix function */
    TMatrix & rotate_y( TYPE angle_sine, TYPE angle_cosine )
    {
      TYPE RM00, RM10, RM20, RM30, RM02, RM12, RM22, RM32;

      /* Calculate changing matrix elements */
      RM00 = M[0][2] * angle_sine + M[0][0] * angle_cosine;
      RM10 = M[1][2] * angle_sine + M[1][0] * angle_cosine;
      RM20 = M[2][2] * angle_sine + M[2][0] * angle_cosine;
      RM30 = M[3][2] * angle_sine + M[3][0] * angle_cosine;

      RM02 = M[0][2] * angle_cosine - M[0][0] * angle_sine;
      RM12 = M[1][2] * angle_cosine - M[1][0] * angle_sine;
      RM22 = M[2][2] * angle_cosine - M[2][0] * angle_sine;
      RM32 = M[3][2] * angle_cosine - M[3][0] * angle_sine;

      /* Store changed elements to matrix */
      M[0][0] = RM00;
      M[1][0] = RM10;
      M[2][0] = RM20;
      M[3][0] = RM30;

      M[0][2] = RM02;
      M[1][2] = RM12;
      M[2][2] = RM22;
      M[3][2] = RM32;
      
      return *this;
    }

    /* Rotate around 'y' axis matrix function */
    TMatrix & rotate_y( TYPE angle_in_degree )
    {
      TYPE sine_val, cosine_val;

#ifdef __USE_SINCOS_ASM__
      /* Assembler sine and cosine calculate */
      _asm {
        fld angle_in_degree
        fmul c_degree2radian

        fsincos

        fstp cosine_val
        fstp sine_val
      }
#else /* __NOUSE_SINCOS_ASM__ */
      double angle_in_radians = Deg2Rad(angle_in_degree); 

      sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */
      return rotate_y(sine_val, cosine_val);
    }

    /* Rotate around 'z' axis matrix function */
    TMatrix & rotate_z( TYPE angle_sine, TYPE angle_cosine )
    {
      TYPE RM00, RM10, RM20, RM30, RM01, RM11, RM21, RM31;

      /* Calculate changing matrix elements */
      RM00 = M[0][0] * angle_cosine - M[0][1] * angle_sine;
      RM10 = M[1][0] * angle_cosine - M[1][1] * angle_sine;
      RM20 = M[2][0] * angle_cosine - M[2][1] * angle_sine;
      RM30 = M[3][0] * angle_cosine - M[3][1] * angle_sine;

      RM01 = M[0][0] * angle_sine + M[0][1] * angle_cosine;
      RM11 = M[1][0] * angle_sine + M[1][1] * angle_cosine;
      RM21 = M[2][0] * angle_sine + M[2][1] * angle_cosine;
      RM31 = M[3][0] * angle_sine + M[3][1] * angle_cosine;

      /* Store changed elements to matrix */
      M[0][0] = RM00;
      M[1][0] = RM10;
      M[2][0] = RM20;
      M[3][0] = RM30;

      M[0][1] = RM01;
      M[1][1] = RM11;
      M[2][1] = RM21;
      M[3][1] = RM31;

      return *this;
    }

    /* Rotate around 'z' axis matrix function */
    TMatrix & rotate_z( TYPE angle_in_degree )
    {
      TYPE sine_val, cosine_val;

#ifdef __USE_SINCOS_ASM__
      /* Assembler sine and cosine calculate */
      _asm {
        fld angle_in_degree
        fmul c_degree2radian

        fsincos

        fstp cosine_val
        fstp sine_val
      }
#else /* __USE_SINCOS_ASM__ */
      double angle_in_radians = Deg2Rad(angle_in_degree); 

      sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */
      return rotate_z(sine_val, cosine_val);
    }

    /* Rotate around arbitrary axis matrix function */
    TMatrix & rotate( TYPE angle_in_degree,
                    TYPE axis_x, TYPE axis_y, TYPE axis_z )
    {
      TYPE Rm[3][3], Tm[4][4];

      BuildRotateMatrix3x3<TYPE, 3>(Rm, angle_in_degree, axis_x, axis_y, axis_z);

      Tm[0][0] = M[0][0] * Rm[0][0] + M[0][1] * Rm[1][0] + M[0][2] * Rm[2][0];
      Tm[1][0] = M[1][0] * Rm[0][0] + M[1][1] * Rm[1][0] + M[1][2] * Rm[2][0];
      Tm[2][0] = M[2][0] * Rm[0][0] + M[2][1] * Rm[1][0] + M[2][2] * Rm[2][0];
      Tm[3][0] = M[3][0] * Rm[0][0] + M[3][1] * Rm[1][0] + M[3][2] * Rm[2][0];

      Tm[0][1] = M[0][0] * Rm[0][1] + M[0][1] * Rm[1][1] + M[0][2] * Rm[2][1];
      Tm[1][1] = M[1][0] * Rm[0][1] + M[1][1] * Rm[1][1] + M[1][2] * Rm[2][1];
      Tm[2][1] = M[2][0] * Rm[0][1] + M[2][1] * Rm[1][1] + M[2][2] * Rm[2][1];
      Tm[3][1] = M[3][0] * Rm[0][1] + M[3][1] * Rm[1][1] + M[3][2] * Rm[2][1];

      Tm[0][2] = M[0][0] * Rm[0][2] + M[0][1] * Rm[1][2] + M[0][2] * Rm[2][2];
      Tm[1][2] = M[1][0] * Rm[0][2] + M[1][1] * Rm[1][2] + M[1][2] * Rm[2][2];
      Tm[2][2] = M[2][0] * Rm[0][2] + M[2][1] * Rm[1][2] + M[2][2] * Rm[2][2];
      Tm[3][2] = M[3][0] * Rm[0][2] + M[3][1] * Rm[1][2] + M[3][2] * Rm[2][2];

      Tm[0][3] = 0;
      Tm[1][3] = 0;
      Tm[2][3] = 0;
      Tm[3][3] = 1;

      memcpy(M, Tm, sizeof(M));
      return *this;
    }

    /* Rotate around arbitrary axis matrix function */
    TMatrix & rotate( TYPE angle_sine, TYPE angle_cosine,
                      TYPE axis_x, TYPE axis_y, TYPE axis_z )
    {
      return
        rotate(Rad2Deg(atan2(angle_sine, angle_cosine)), axis_x, axis_y, axis_z);
    }

    /* Rotate around arbitrary axis matrix function */
    TMatrix & rotate( TYPE angle_sine, TYPE angle_cosine,
                      TVector<TYPE> const &vec )
    {
      return rotate(angle_sine, angle_cosine, vec.x, vec.y, vec.z);
    }

    /* Rotate around arbitrary axis matrix function */
    TMatrix & rotate( TYPE angle_in_degree, TVector<TYPE> const &vec )
    {
      return rotate(angle_in_degree, vec.x, vec.y, vec.z);
    }

    /* Scale along axes matrix function */
    TMatrix & scale( TYPE sx, TYPE sy, TYPE sz )
    {
      M[0][0] *= sx;
      M[1][0] *= sx;
      M[2][0] *= sx;
      M[3][0] *= sx;

      M[0][1] *= sy;
      M[1][1] *= sy;
      M[2][1] *= sy;
      M[3][1] *= sy;

      M[0][2] *= sz;
      M[1][2] *= sz;
      M[2][2] *= sz;
      M[3][2] *= sz;
      return *this;
    }

    /* Scale along axes matrix function */
    TMatrix & scale( TVector<TYPE> const &scale_vec )
    {
      return scale(scale_vec.x, scale_vec.y, scale_vec.z);
    }

    /* Transform matrix by specified transformation function */
    TMatrix & transform( TTransform<TYPE> const &trans )
    {
      *this *= trans.matrix;
      return *this;
    }

    /* Inverse transform matrix by specified transformation function */
    TMatrix & inv_transform( TTransform<TYPE> const &trans )
    {
      *this = trans.inv_matrix * *this;
      return *this;
    }

    /***
     * Apply specified transformation to matrix functions
     ***/

    /* Translation matrix function */
    TMatrix translation( TYPE dx, TYPE dy, TYPE dz ) const
    {
      TMatrix res;

      memcpy(&res, this, sizeof(M) - 4 * sizeof(TYPE));
      res.M[3][0] = M[3][0] + dx;
      res.M[3][1] = M[3][1] + dy;
      res.M[3][2] = M[3][2] + dz;
      res.M[3][3] = 1;
      return res;
    }

    /* Translation matrix function */
    TMatrix translation( TVector<TYPE> const &shift ) const
    {
      return translation(shift.x, shift.y, shift.z);
    }

    /* rotation around 'x' axis matrix function */
    TMatrix rotation_x( TYPE angle_sine, TYPE angle_cosine ) const
    {
      return
        TMatrix(M[0][0], 
              M[0][1] * angle_cosine - M[0][2] * angle_sine,
              M[0][1] * angle_sine + M[0][2] * angle_cosine,
              M[1][0],
              M[1][1] * angle_cosine - M[1][2] * angle_sine,
              M[1][1] * angle_sine + M[1][2] * angle_cosine,
              M[2][0],
              M[2][1] * angle_cosine - M[2][2] * angle_sine,
              M[2][1] * angle_sine + M[2][2] * angle_cosine,
              M[3][0],
              M[3][1] * angle_cosine - M[3][2] * angle_sine,
              M[3][1] * angle_sine + M[3][2] * angle_cosine);
    }

    /* Rotation around 'x' axis matrix function */
    TMatrix rotation_x( TYPE angle_in_degree ) const
    {
      TYPE sine_val, cosine_val;

#ifdef __USE_SINCOS_ASM__
      /* Assembler sine and cosine calculate */
      _asm {
        fld angle_in_degree
        fmul c_degree2radian

        fsincos

        fstp cosine_val
        fstp sine_val
      }
#else /* __NOUSE_SINCOS_ASM__ */
      double angle_in_radians = Deg2Rad(angle_in_degree); 

      sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */
      return rotation_x(sine_val, cosine_val);
    }

    /* Rotation around 'y' axis matrix function */
    TMatrix rotation_y( TYPE angle_sine, TYPE angle_cosine ) const
    {
      return
        TMatrix(M[0][2] * angle_sine + M[0][0] * angle_cosine,
              M[0][1],
              M[0][2] * angle_cosine - M[0][0] * angle_sine,
              M[1][2] * angle_sine + M[1][0] * angle_cosine,
              M[1][1],
              M[1][2] * angle_cosine - M[1][0] * angle_sine,
              M[2][2] * angle_sine + M[2][0] * angle_cosine,
              M[2][1],
              M[2][2] * angle_cosine - M[2][0] * angle_sine,
              M[3][2] * angle_sine + M[3][0] * angle_cosine,
              M[3][1],
              M[3][2] * angle_cosine - M[3][0] * angle_sine);
    }

    /* Rotation around 'y' axis matrix function */
    TMatrix rotation_y( TYPE angle_in_degree ) const
    {
      TYPE sine_val, cosine_val;

#ifdef __USE_SINCOS_ASM__
      /* Assembler sine and cosine calculate */
      _asm {
        fld angle_in_degree
        fmul c_degree2radian

        fsincos

        fstp cosine_val
        fstp sine_val
      }
#else /* __NOUSE_SINCOS_ASM__ */
      double angle_in_radians = Deg2Rad(angle_in_degree); 

      sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */
      return rotation_y(sine_val, cosine_val);
    }

    /* Rotation around 'z' axis matrix function */
    TMatrix rotation_z( TYPE angle_sine, TYPE angle_cosine ) const
    {
      return
        TMatrix(M[0][0] * angle_cosine - M[0][1] * angle_sine,
              M[0][0] * angle_sine + M[0][1] * angle_cosine,
              M[0][2],
              M[1][0] * angle_cosine - M[1][1] * angle_sine,
              M[1][0] * angle_sine + M[1][1] * angle_cosine,
              M[1][2],
              M[2][0] * angle_cosine - M[2][1] * angle_sine,
              M[2][0] * angle_sine + M[2][1] * angle_cosine,
              M[2][2],
              M[3][0] * angle_cosine - M[3][1] * angle_sine,
              M[3][0] * angle_sine + M[3][1] * angle_cosine,
              M[3][2]);
    }

    /* Rotation around 'z' axis matrix function */
    TMatrix rotation_z( TYPE angle_in_degree ) const
    {
      TYPE sine_val, cosine_val;

#ifdef __USE_SINCOS_ASM__
      /* Assembler sine and cosine calculate */
      _asm {
        fld angle_in_degree
        fmul c_degree2radian

        fsincos

        fstp cosine_val
        fstp sine_val
      }
#else /* __NOUSE_SINCOS_ASM__ */
      double angle_in_radians = Deg2Rad(angle_in_degree); 

      sine_val = sin(angle_in_radians), cosine_val = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */
      return rotation_z(sine_val, cosine_val);
    }

    /* Rotation around arbitrary axis matrix function */
    TMatrix rotation( TYPE angle_in_degree,
                      TYPE axis_x, TYPE axis_y, TYPE axis_z ) const
    {
      TYPE
        angle, s, h,
        vx, vy, vz, len,
        Rm[3][3];

      TYPE sine_val, cosine_val;

      angle /= 2;
#ifdef __USE_SINCOS_ASM__
      /* Assembler sine and cosine calculate */
      _asm {
        fld angle
        fmul c_degree2radian

        fsincos

        fstp s
        fstp h
      }
#else /* __NOUSE_SINCOS_ASM__ */
      double angle = Deg2Rad(angle_in_degree); 

      h = sin(angle_in_radians), s = cos(angle_in_radians);
#endif /* __USE_SINCOS_ASM__ */ 

      len = axis_x * axis_x + axis_y * axis_y + axis_z * axis_z;
      if (COM_ABS(len) > Threshold && COM_ABS(len - 1) > Threshold)
      {
        len = sqrt(len);
        vx = axis_x * h / len;
        vy = axis_y * h / len;
        vz = axis_z * h / len;
      }
      else
      {
        vx = h * axis_x;
        vy = h * axis_y;
        vz = h * axis_z;
      }

      Rm[0][0] = 1 - 2 * (vy * vy + vz * vz);
      Rm[0][1] = 2 * vx * vy - 2 * s * vz;
      Rm[0][2] = 2 * s * vy + 2 * vx * vz;

      Rm[1][0] = 2 * vx * vy + 2 * s * vz;
      Rm[1][1] = 1 - 2 * (vx * vx + vz * vz);
      Rm[1][2] = -2 * s * vx + 2 * vy * vz;

      Rm[2][0] = -2 * s * vy + 2 * vx * vz;
      Rm[2][1] = 2 * s * vx + 2 * vy * vz;
      Rm[2][2] = 1 - 2 * (vx * vx + vy * vy);

      return
        TMatrix(M[0][0] * Rm[0][0] + M[0][1] * Rm[1][0] + M[0][2] * Rm[2][0],
              M[0][0] * Rm[0][1] + M[0][1] * Rm[1][1] + M[0][2] * Rm[2][1],
              M[0][0] * Rm[0][2] + M[0][1] * Rm[1][2] + M[0][2] * Rm[2][2],
              M[1][0] * Rm[0][0] + M[1][1] * Rm[1][0] + M[1][2] * Rm[2][0],
              M[1][0] * Rm[0][1] + M[1][1] * Rm[1][1] + M[1][2] * Rm[2][1],
              M[1][0] * Rm[0][2] + M[1][1] * Rm[1][2] + M[1][2] * Rm[2][2],
              M[2][0] * Rm[0][0] + M[2][1] * Rm[1][0] + M[2][2] * Rm[2][0],
              M[2][0] * Rm[0][1] + M[2][1] * Rm[1][1] + M[2][2] * Rm[2][1],
              M[2][0] * Rm[0][2] + M[2][1] * Rm[1][2] + M[2][2] * Rm[2][2],
              M[3][0] * Rm[0][0] + M[3][1] * Rm[1][0] + M[3][2] * Rm[2][0],
              M[3][0] * Rm[0][1] + M[3][1] * Rm[1][1] + M[3][2] * Rm[2][1],
              M[3][0] * Rm[0][2] + M[3][1] * Rm[1][2] + M[3][2] * Rm[2][2]);
    }

    /* Rotation around arbitrary axis matrix function */
    TMatrix rotation( TYPE angle_sine, TYPE angle_cosine,
                    TYPE axis_x, TYPE axis_y, TYPE axis_z ) const
    {
      return
        rotation(Rad2Deg(atan2(angle_sine, angle_cosine)), axis_x, axis_y, axis_z);
    }

    /* Rotation around arbitrary axis matrix function */
    TMatrix rotation( TYPE angle_sine, TYPE angle_cosine,
                    TVector<TYPE> const &vec ) const
    {
      return rotation(angle_sine, angle_cosine, vec.x, vec.y, vec.z);
    }

    /* Rotation around arbitrary axis matrix function */
    TMatrix rotation( TYPE angle_in_degree, TVector<TYPE> const &vec ) const
    {
      return rotation(angle_in_degree, vec.x, vec.y, vec.z);
    }

    /* Scaling along axes matrix function */
    TMatrix scaling( TYPE sx, TYPE sy, TYPE sz ) const
    {
      return
        TMatrix(M[0][0] * sx, M[0][1] * sy, M[0][2] * sz, 
                M[1][0] * sx, M[1][1] * sy, M[1][2] * sz, 
                M[2][0] * sx, M[2][1] * sy, M[2][2] * sz,
                M[3][0] * sx, M[3][1] * sy, M[3][2] * sz);
    }

    /* Scaling along axes matrix function */
    TMatrix scaling( TVector<TYPE> const &scale_vec ) const
    {
      return scaling(scale_vec.x, scale_vec.y, scale_vec.z);
    }

    /* Transformation matrix by specified transformation function */
    TMatrix transformation( TTransform<TYPE> const &trans ) const
    {
      return *this * trans.M;
    }

    /* Inverse transformation matrix by specified transformation function */
    TMatrix inv_transformation( TTransform<TYPE> const &trans )
    {
      return trans.InvM * *this;
    }
  };

  /* Static class unit (identity) matrix */
  template<class TYPE> const TYPE TMatrix<TYPE>::UnitMatrix[4][4] =
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  };
}

#endif /* __CGLMATHMATRIX_INCLUDED__ */
