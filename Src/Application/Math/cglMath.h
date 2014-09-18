/**
@file     cglMathDef.h
@brief    Mathematics for computer graphics main declaration module
@date     Created on 12/09/2014
@project  Task1
@author   Sergeev Artemiy
*/

#ifndef __CGLMATH_INCLUDED__
#define __CGLMATH_INCLUDED__

#include "cglMathDef.h"

#include "cglMathVec.h"
#include "cglMathMatrix.h"
#include "cglMathTransform.h"
#include "cglMathCamera.h"

typedef cglmath::TCamera<float> camera_t;
typedef cglmath::TVector<float> vec_t;
typedef cglmath::TMatrix<float> matrix_t;
typedef cglmath::TTransform<float> transform_t;

#endif /* __CGLMATH_INCLUDED__ */

