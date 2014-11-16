/**
@file     cglMathColor.h
@brief    color in RGBA space declaration module
@date     Created on 22/09/2014
@project  Task2
@author   Sergeev Artemiy
*/

#ifndef __CGLMATHCOLOR_INCLUDED__
#define __CGLMATHCOLOR_INCLUDED__

#define CGL_MAKELONG0123(B0, B1, B2, B3) \
                        (DWORD)((((DWORD)((BYTE)(B3))) << 24) | \
                        (((DWORD)((BYTE)(B2))) << 16) | \
                        (((DWORD)((BYTE)(B1))) << 8) | \
                        (DWORD)((BYTE)(B0)))
#define CGL_MAX(A, B)         (((A) > (B)) ? (A) : (B))
#define CGL_MIN(A, B)         (((A) < (B)) ? (A) : (B))

namespace cglmath
{
  /* Red, green, blue and transparent components color class */
  template <class TYPE> class TColor
  {
  public: 
    /* Red, green, blue and transparent components */
    TYPE r, g, b, a;

    /* Empty class constructor */
    explicit TColor( void ) : r(0), g(0), b(0), a(1)
    {
    }

    /* Main color class constructor */
    explicit TColor( TYPE new_r, TYPE new_g, TYPE new_b, TYPE new_a = 1 ) :
      r(new_r), g(new_g), b(new_b), a(new_a)
    {
    }

    /* color class constructor */
    explicit TColor( TYPE value ) : r(value), g(value), b(value), a(1)
    {
    }

    /* Class constructor */
    explicit TColor( unsigned long value ) :
      r(((value >> 16) & 0xFF) / 255.0), g(((value >> 8) & 0xFF) / 255.0),
      b((value & 0xFF) / 255.0), a(((value >> 24) & 0xFF) / 255.0)
    {
    }

    /* Addition colors function */
    TColor operator+( const TColor &color ) const
    {
      return TColor(r + color.r, g + color.g, b + color.b, a + color.a);
    }

    /* Addition colors with assignment function */
    TColor & operator+=( const TColor &color )
    {
      r += color.r;
      g += color.g;
      b += color.b;
      a += color.a;
      return *this;
    }

    /* Subtraction colors function */
    TColor operator-( const TColor &color ) const
    {
      return TColor(r - color.r, g - color.g, b - color.b, a - color.a);
    }

    /* Subtraction colors with assignment function */
    TColor & operator-=( const TColor &color )
    {
      r -= color.r;
      g -= color.g;
      b -= color.b;
      a -= color.a;
      return *this;
    }

    /* Multiply color by number function */
    TColor operator*( TYPE n ) const
    {
      return TColor(r * n, g * n, b * n, a * n);
    }

    /* Multiply color by number with assignment function */
    TColor & operator*=( TYPE value )
    {
      r *= value;
      g *= value;
      b *= value;
      a *= value;
      return *this;
    }

    /* Multiply color by color function */
    TColor operator*( const TColor &color ) const
    {
      return TColor(r * color.r, g * color.g, b * color.b, a * color.a);
    }

    /* Multiply color by color with assignment function */
    TColor & operator*=( const TColor &color )
    {
      r *= color.r;
      g *= color.g;
      b *= color.b;
      a *= color.a;
      return *this;
    }

    /* Divide color by coefficient function */
    TColor operator/( TYPE value ) const
    {
      return TColor(r / value, g / value, b / value, a / value);
    }

    /* Divide color by coefficient with assignment functions */
    TColor & operator/=( TYPE value )
    {
      r /= value;
      g /= value;
      b /= value;
      a /= value;
      return *this;
    }

    /* Determine maximum component function */
    TYPE maximum() const
    {
      if (r > g)
        if (b > r)
          return b;
        else
          return r;
      else
        if (b > g)
          return b;
        else
          return g;
    }

    /* Setting color function */
    void set_color( TYPE new_r, TYPE new_g, TYPE new_b, TYPE new_a = 1 )
    {
      r = new_r;
      g = new_g;
      b = new_b;
      a = new_a;
    }

    /* Clamp color function */
    TColor & clamp( TYPE min = 0, TYPE max = 1 )
    {
      if (r < min)
        r = min;
      else
        if (r > max)
          r = max;
      if (g < min)
        g = min;
      else
        if (g > max)
          g = max;
      if (b < min)
        b = min;
      else
        if (b > max)
          b = max;
      return *this;
    }

    /* Clamp color function */
    TColor clamping( TYPE min = 0, TYPE max = 1 ) const
    {
      return TColor(r < min ? min : r > max ? max : r,
                 g < min ? min : g > max ? max : g,
                 b < min ? min : b > max ? max : b);
    }

    /* Converting color to unsigned long type function */
    unsigned long ToDWORD( void ) const
    {
      double
        v0 = b * 255, v1 = g * 255, v2 = r * 255, v3 = a * 255;
      unsigned char
        B0 = v0 > 255 ? 255 : v0 < 0 ? 0 : (unsigned char)v0,
        B1 = v1 > 255 ? 255 : v1 < 0 ? 0 : (unsigned char)v1,
        B2 = v2 > 255 ? 255 : v2 < 0 ? 0 : (unsigned char)v2,
        B3 = v3 > 255 ? 255 : v3 < 0 ? 0 : (unsigned char)v3;

      return CGL_MAKELONG0123(B0, B1, B2, B3);
    }

    /* Converting color to unsigned long type function */
    operator unsigned long( void ) const
    {
      return ToDWORD();
    }
  };
}

#endif /* _TSGCOLOR_H_ */
