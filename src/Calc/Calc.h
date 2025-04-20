
#pragma once

#include <limits>
#include <cmath>
#include <numbers>

#define PI std::numbers::pi_v<float>

#define DEG_TO_RAD 0.0174532925f
#define RAD_TO_DEG 57.295779513f

#define BIG_NUMBER 3.402823466e+36f
#define SMALL_NUMBER 1.175494351e-36f

/** Max size a float can be */
#define FLOAT_MAX std::numeric_limits<float>::max()
/** Smallest positive size of a float */
#define FLOAT_MIN std::numeric_limits<float>::min()
/** Negative max limit of a float */
#define FLOAT_LOW std::numeric_limits<float>::lowest()
/** Infinity */
#define FLOAT_INF std::numeric_limits<float>::infinity()

namespace Math
{
    inline bool NearlyEqual(const float A, const float B, const float Tolerance = 0.0001f)
    {
        return std::fabs(A - B) < Tolerance;
    }

    inline bool NearlyZero(float A, float Tolerance = 0.0001f)
    {
        return std::fabs(A) < Tolerance;
    }

    constexpr float Deg(float Rad)
    {
        return Rad * RAD_TO_DEG;
    }

    constexpr float Rad(float Deg)
    {
        return Deg * DEG_TO_RAD;
    }
}
