
#pragma once

#include <limits>
#include <cmath>
#include <numbers>
#include <algorithm>

#include <SFML/System/Vector2.hpp>

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

    inline float Pow(float X, float Y)
    {
        return std::powf(X, Y);
    }

    inline float Square(float X)
    {
        return Pow(X, 2.0f);
    }

    inline sf::Vector2f ClampMagnitude(const sf::Vector2f& Vec, float Magnitude)
    {
        const float length = Vec.lengthSquared();
        if (NearlyZero(length))
        {
            return Vec;
        }

        if (length > Square(Magnitude))
        {
            return Vec.normalized() * Magnitude;
        }

        return Vec;
    }

    inline float Clamp(float X, float Min, float Max)
    {
        return std::fmax(std::fmin(X, Max), Min);
    }

    inline int Clamp(int X, int Min, int Max)
    {
        return std::max(std::min(X, Max), Min);
    }

    inline float Clamp01(float X)
    {
        return Clamp(X, 0.0f, 1.0f);
    }

    constexpr bool IsBetweenIncl(int X, int Min, int Max)
    {
        return X >= Min && X <= Max;
    }

    constexpr bool IsBetweenExcl(int X, int Min, int Max)
    {
        return X >= Min && X < Max;
    }

    constexpr bool IsWithinIncl(int X, int Max)
    {
        return X >= 0 && X <= Max;
    }

    constexpr bool IsWithinExcl(int X, int Max)
    {
        return X >= 0 && X < Max;
    }

    /* -1, 0, 1 */
    constexpr float Sign(float X)
    {
        return NearlyZero(X) ? 0.0f : (X > 0.0f ? 1.0f : -1.0f);
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
