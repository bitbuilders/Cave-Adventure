
#pragma once

#include <limits>
#include <cmath>
#include <numbers>
#include <algorithm>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "imgui.h"

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

    inline bool NearlyZero(const sf::Vector2f& Vec)
    {
        return NearlyZero(Vec.x) && NearlyZero(Vec.y);
    }

    inline float Pow(float X, float Y)
    {
        return std::powf(X, Y);
    }

    inline float Square(float X)
    {
        return Pow(X, 2.0f);
    }

    inline sf::Vector2f Rotate(const sf::Vector2f& Vec, float Deg)
    {
        float cos = std::cosf(Deg * DEG_TO_RAD);
        float sin = std::sinf(Deg * DEG_TO_RAD);

        return
        { Vec.x * cos - Vec.y * sin,
            Vec.x * sin + Vec.y * cos };
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

    inline float Lerp(float A, float B, float T)
    {
        return A + (B - A) * T;
    }

    inline int Lerp(int A, int B, float T)
    {
        return A + static_cast<int>(std::floorf(static_cast<float>(B - A) * T));
    }

    inline sf::Vector2f Lerp(const sf::Vector2f& A, const sf::Vector2f& B, float T)
    {
        auto x = Lerp(A.x, B.x, T);
        auto y = Lerp(A.y, B.y, T);

        return {x, y};
    }

    inline sf::Color Lerp(const sf::Color& A, const sf::Color& B, float T)
    {
        auto r = Lerp(A.r, B.r, T);
        auto g = Lerp(A.g, B.g, T);
        auto b = Lerp(A.b, B.b, T);
        auto a = Lerp(A.a, B.a, T);

        return sf::Color(r, g, b ,a);
    }

    inline ImVec4 Lerp(const ImVec4& A, const ImVec4& B, float T)
    {
        auto x = Lerp(A.x, B.x, T);
        auto y = Lerp(A.y, B.y, T);
        auto z = Lerp(A.z, B.z, T);
        auto w = Lerp(A.w, B.w, T);

        return ImVec4(x, y, z, w);
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

    /* Map a value from one range (R1), to a new range (R2) */
    inline float Map(float X, float R1Min, float R1Max, float R2Min, float R2Max)
    {
        if (NearlyEqual(R1Min, R1Max))
        {
            return X;
        }

        float r1Percent = (X - R1Min) / (R1Max - R1Min);

        return Lerp(R2Min, R2Max, r1Percent);
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
