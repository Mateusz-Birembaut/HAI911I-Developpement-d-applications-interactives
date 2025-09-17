#ifndef VEC3_H
#define VEC3_H

#include <math.h>

template<typename T>
struct Vec3{
    T x{};
    T y{};
    T z{};

    Vec3<T> operator-(const Vec3<T>& v) const { return Vec3<T>{x - v.x, y - v.y, z - v.z}; }
    Vec3<T> operator+(const Vec3<T>& v) const { return Vec3<T>{x + v.x, y + v.y, z + v.z}; }
    Vec3<T>& operator+=(const Vec3<T>& v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vec3<T> operator*(T s) const { return Vec3<T>{x*s, y*s, z*s}; }

    Vec3<T> cross(const Vec3& v) const { return Vec3<T>{ y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x };}

    void normalize() {
        T len = std::sqrt(x*x + y*y + z*z);
        if (len > 0) { x /= len; y /= len; z /= len; }
    }
};

template<>
struct Vec3<bool> {
    Vec3() = delete;
};

#endif // VEC3_H
