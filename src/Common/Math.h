#pragma once
#include "ResultCode.h"

/// <summary>
/// Math helper utilities
/// </summary>
namespace TVisu
{
    // vector
    template<class T>
    struct Vector2
    {
        T x;
        T y;
    };

    typedef Vector2<uint32_t>   Vector2UInt32;
    typedef Vector2<float>      Vector2Float;

    template<class T>
    struct Vector3
    {
        T x;
        T y;
        T z;
    };

    typedef Vector3<uint32_t>   Vector3UInt32;
    typedef Vector3<float>      Vector3Float;

    template<class T>
    struct Color
    {
        T r;
        T g;
        T b;
        T a;
    };

    typedef Color<uint8_t>      ColorByte;
    typedef Color<float>        ColorFloat;
}
