
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2010-2011 Francois Beaune
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#ifndef APPLESEED_FOUNDATION_MATH_SCALAR_H
#define APPLESEED_FOUNDATION_MATH_SCALAR_H

// appleseed.foundation headers.
#ifdef APPLESEED_FOUNDATION_USE_SSE
#include "foundation/platform/sse.h"
#endif
#include "foundation/platform/types.h"

// Standard headers.
#include <cassert>
#include <cmath>
#include <cstddef>
#include <limits>

namespace foundation
{

// todo: implement feq_ulp() and fz_ulp(), to compare scalars with precision
// expressed in ulp. Most probably an integer based comparison. Reference:
// http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm.

// todo: implement fast equivalents of pow(), pow2() and log().


//
// Constants.
//

// Various constants in double precision.
static const double Pi          = 3.1415926535897932;
static const double TwoPi       = 6.2831853071795865;
static const double HalfPi      = 1.5707963267948966;
static const double RcpPi       = 0.3183098861837907;
static const double RcpTwoPi    = 0.1591549430918953;
static const double RcpHalfPi   = 0.6366197723675813;


//
// Conversion operations.
//

// Convert an angle from degrees to radians.
template <typename T>
T deg_to_rad(const T angle);

// Convert an angle from radians to degrees.
template <typename T>
T rad_to_deg(const T angle);


//
// Arithmetic operations.
//

// Return the absolute value of the argument. This function complements
// the standard function std::abs() which is not necessary available for
// compiler-dependent types (e.g. __int64).
template <typename T>
T abs(const T x);

// Return the square of the argument.
template <typename T>
T square(const T x);

// Compile-time integer exponentiation of the form X^P.
// Example: static_pow_int<2, 8>::value evaluates to 2^8=256.
template <int X, int P>
struct static_pow_int
{
    static const int value = X * static_pow_int<X, P - 1>::value;
};
template <int X>
struct static_pow_int<X, 0>
{
    static const int value = 1;
};

// Runtime integer exponentiation of the form x^p.
template <typename T>
T pow_int(const T x, size_t p);

// Return the smallest power of 2 larger than a given integer x (x > 0).
template <typename T> T next_pow2(T x);
template <> int64 next_pow2<int64>(int64 x);
template <> uint64 next_pow2<uint64>(uint64 x);

// Return true if a given integer x is a power of 2.
template <typename T>
bool is_pow2(const T x);

// Return the base-2 logarithm of a given integer.
template <typename T>
T log2(T x);

// Return the factorial of a given integer.
template <typename T>
T factorial(T x);

// Clamp the argument to [min, max].
template <typename T>
T clamp(const T x, const T min, const T max);

// Clamp the argument to [0,1].
template <typename T>
T saturate(const T x);

// Wrap the argument back to [0,1).
template <typename T>
T wrap(const T x);

// Normalize an angle into [0, 2*Pi).
template <typename T>
T normalize_angle(const T angle);

// Return the integer part of a floating-point value.
template <typename Int> Int truncate(const float x);
template <typename Int> Int truncate(const double x);

// Round x to the nearest integer with Round Half Away from Zero tie breaking rule.
// Reference: http://en.wikipedia.org/wiki/Rounding#Round_half_away_from_zero.
template <typename Int, typename T>
Int round(const T x);

// Compute a % n or fmod(a, n) and always return a non-negative value.
template <typename T>
T mod(const T a, const T n);

// linearstep() returns 0 for x < a, 1 for x > b, and generates
// a linear transition from 0 to 1 between x = a and x = b.
template <typename T>
T linearstep(const T a, const T b, const T x);

// smoothstep() returns 0 for x < a, 1 for x > b, and generates
// a smooth, C-infinite transition from 0 to 1 between x = a and
// x = b. The function has zero first derivatives at both x = a
// and x = b.
template <typename T>
T smoothstep(const T a, const T b, const T x);

// mix() returns a for x < 0, b for x > 1, and performs a linear
// blend between values a and b when x is between 0 and 1.
template <typename T, typename U>
T mix(const T a, const T b, const U x);

// lerp() returns the linear interpolation (1 - x) * a + x * b.
template <typename T, typename U>
T lerp(const T a, const T b, const U x);

// fit() remaps a variable x from the range [min_x, max_x] to the
// range [min_y, max_y]. When x is outside the [min_x, max_x] range,
// a linear extrapolation outside the [min_y, max_y] range is used.
template <typename T>
T fit(
    const T x,
    const T min_x,
    const T max_x,
    const T min_y,
    const T max_y);


//
// Robust floating-point tests.
//

// Default epsilon values for floating-point tests.
template <typename T> T default_eps();          // intentionally left unimplemented
template <> inline float default_eps<float>()   { return 1.0e-6f; }
template <> inline double default_eps<double>() { return 1.0e-14; }

// Approximate equality tests.
bool feq(
    const int       lhs,
    const int       rhs,
    const int       eps = 0);   // eps is not used, always test for exact equality
bool feq(
    const float     lhs,
    const float     rhs,
    const float     eps = default_eps<float>());
bool feq(
    const double    lhs,
    const double    rhs,
    const double    eps = default_eps<double>());

// Approximate zero tests.
bool fz(
    const int       lhs,
    const int       eps = 0);   // eps is not used, always test for exact zero
bool fz(
    const float     lhs,
    const float     eps = default_eps<float>());
bool fz(
    const double    lhs,
    const double    eps = default_eps<double>());


//
// Conversion operations implementation.
//

template <typename T>
inline T deg_to_rad(const T angle)
{
    return angle * T(Pi / 180.0);
}

template <typename T>
inline T rad_to_deg(const T angle)
{
    return angle * T(180.0 / Pi);
}


//
// Arithmetic operations implementation.
//

template <typename T>
inline T abs(const T x)
{
    return x < T(0) ? -x : x;
}

template <typename T>
inline T square(const T x)
{
    return x * x;
}

template <typename T>
inline T pow_int(const T x, size_t p)
{
    // todo: implement exponentiation by squaring for large values of p.
    // Reference: http://en.wikipedia.org/wiki/Exponentiation_by_squaring.

    T y = 1;

    while (p--)
        y *= x;

    return y;
}

template <typename T>
inline T next_pow2(T x)
{
    assert(x > 0);
    --x;
    x |= x >> 16;
    x |= x >> 8;
    x |= x >> 4;
    x |= x >> 2;
    x |= x >> 1;
    return x + 1;
}

template <>
inline int64 next_pow2<int64>(int64 x)
{
    assert(x > 0);
    --x;
    x |= x >> 32;
    x |= x >> 16;
    x |= x >> 8;
    x |= x >> 4;
    x |= x >> 2;
    x |= x >> 1;
    return x + 1;
}

template <>
inline uint64 next_pow2<uint64>(uint64 x)
{
    assert(x > 0);
    --x;
    x |= x >> 32;
    x |= x >> 16;
    x |= x >> 8;
    x |= x >> 4;
    x |= x >> 2;
    x |= x >> 1;
    return x + 1;
}

template <typename T>
inline bool is_pow2(const T x)
{
    return (x & (x - 1)) == 0;
}

template <typename T>
inline T log2(T x)
{
    assert(x > 0);

    T n = 0;

    while (x >>= 1)
        ++n;

    return n;
}

template <typename T>
inline T factorial(T x)
{
    assert(x >= 0);

    T fac = 1;

    while (x > 1)
    {
        fac *= x;
        --x;
    }

    return fac;
}

template <typename T>
inline T clamp(const T x, const T min, const T max)
{
    assert(min <= max);
    return x <= min ? min :
           x >= max ? max :
           x;
}

template <typename T>
inline T saturate(const T x)
{
    return x <= T(0.0) ? T(0.0) :
           x >= T(1.0) ? T(1.0) :
           x;
}

template <typename T>
inline T wrap(const T x)
{
    const T y = std::fmod(x, T(1.0));
    return y < T(0.0) ? y + T(1.0) : y;
}

template <typename T>
inline T normalize_angle(const T angle)
{
    const T a = std::fmod(angle, T(TwoPi));
    return a < T(0.0) ? a + T(TwoPi) : a;
}

template <typename Int>
inline Int truncate(const float x)
{
#ifdef APPLESEED_FOUNDATION_USE_SSE
    return static_cast<Int>(_mm_cvttss_si32(_mm_load_ss(&x)));
#else
    return static_cast<Int>(x);
#endif
}

template <typename Int>
inline Int truncate(const double x)
{
#ifdef APPLESEED_FOUNDATION_USE_SSE
    return static_cast<Int>(_mm_cvttsd_si32(_mm_load_sd(&x)));
#else
    return static_cast<Int>(x);
#endif
}

template <typename Int, typename T>
inline Int round(const T x)
{
    return truncate<Int>(x < T(0.0) ? x - T(0.5) : x + T(0.5));
}

template <typename T>
inline T mod(const T a, const T n)
{
    const T m = a % n;
    return m < 0 ? n + m : m;
}

template <>
inline float mod(const float a, const float n)
{
    const float m = std::fmod(a, n);
    return m < 0.0f ? n + m : m;
}

template <>
inline double mod(const double a, const double n)
{
    const double m = std::fmod(a, n);
    return m < 0.0 ? n + m : m;
}

template <typename T>
inline T linearstep(const T a, const T b, const T x)
{
    assert(a < b);
    return x <= a ? T(0.0) :
           x >= b ? T(1.0) :
           (x - a) / (b - a);
}

template <typename T>
inline T smoothstep(const T a, const T b, const T x)
{
    assert(a < b);

    if (x <= a) return T(0.0);
    if (x >= b) return T(1.0);

    const T y = (x - a) / (b - a);
    return y * y * (T(3.0) - y - y);
}

template <typename T, typename U>
inline T mix(const T a, const T b, const U x)
{
    return x <= U(0.0) ? a :
           x >= U(1.0) ? b :
           lerp(a, b, x);
}

template <typename T, typename U>
inline T lerp(const T a, const T b, const U x)
{
    return (U(1.0) - x) * a + x * b;
}

template <typename T>
inline T fit(
    const T x,
    const T min_x,
    const T max_x,
    const T min_y,
    const T max_y)
{
    assert(min_x != max_x);
    assert(min_y != max_y);

    const T k = (x - min_x) / (max_x - min_x);
    return min_y * (T(1.0) - k) + max_y * k;
}


//
// Robust floating-point tests implementation.
//

namespace scalar_impl
{

    // Actual implementation of foundation::feq(), as a templatized class.
    template <typename T>
    inline bool feq(const T lhs, const T rhs, const T eps)
    {
        // Handle case where lhs is exactly +0.0 or -0.0.
        if (lhs == T(0.0))
            return std::abs(rhs) < eps;

        // Handle case where rhs is exactly +0.0 or -0.0.
        if (rhs == T(0.0))
            return std::abs(lhs) < eps;

        const T abs_lhs = std::abs(lhs);
        const T abs_rhs = std::abs(rhs);

        // No equality if lhs/rhs overflows.
        if (abs_rhs < T(1.0) &&
            abs_lhs > abs_rhs * std::numeric_limits<T>::max())
            return false;

        // No equality if lhs/rhs underflows.
        if (abs_rhs > T(1.0) &&
            abs_lhs < abs_rhs * std::numeric_limits<T>::min())
            return false;

        // There is equality if the ratio lhs/rhs is close enough to 1.
        return
            (lhs / rhs) > (T(1.0) - eps) &&
            (lhs / rhs) < (T(1.0) + eps);
    }

}

inline bool feq(
    const int       lhs,
    const int       rhs,
    const int       eps)
{
    return lhs == rhs;
}

inline bool feq(
    const float     lhs,
    const float     rhs,
    const float     eps)
{
    return scalar_impl::feq<float>(lhs, rhs, eps);
}

inline bool feq(
    const double    lhs,
    const double    rhs,
    const double    eps)
{
    return scalar_impl::feq<double>(lhs, rhs, eps);
}

inline bool fz(
    const int       lhs,
    const int       eps)
{
    return lhs == 0;
}

inline bool fz(
    const float     lhs,
    const float     eps)
{
    return std::abs(lhs) < eps;
}

inline bool fz(
    const double    lhs,
    const double    eps)
{
    return std::abs(lhs) < eps;
}

}       // namespace foundation

#endif  // !APPLESEED_FOUNDATION_MATH_SCALAR_H
