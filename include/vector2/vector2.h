//
// Created by Jacopo Gasparetto on 26/10/21.
//

#ifndef VECTOR2_H
#define VECTOR2_H

#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <cmath>

namespace sf {
    constexpr float PI = 3.1415926535f;

    template<typename T>
    T rad2deg(T angle) {
        return angle * 180.f / PI;
    }

    template<typename T>
    inline float dotProduct(const Vector2<T> &lhs, const Vector2<T> &rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    template<typename T>
    inline float getMagnitude(const Vector2<T> &v) {
        return sqrt(dotProduct(v, v));
    }

    template<typename T>
    inline Vector2<T>& normalize(Vector2<T> &v) {
        auto m = getMagnitude(v);
        v.x /= m;
        v.y /= m;
        return v;
    }

    template<typename T>
    inline Vector2<T> normalizeCopy(const Vector2<T> &v) {
        Vector2<T> u {v.x, v.y};
        auto m = getMagnitude(u);
        u.x /= m;
        u.y /= m;
        return u;
    }

    template<typename T>
    inline void setMagnitude(Vector2<T> &v, T magnitude) {
        normalize(v);
        v *= magnitude;
    }

    template<typename T>
    inline void limitMagnitude(Vector2<T> &v, T magnitude) {
        if (getMagnitude(v) > magnitude)
            setMagnitude(v, magnitude);
    }


    template<typename T>
    inline T angleBetween(const Vector2<T> &lhs, const Vector2<T> &rhs) {
        auto a = getMagnitude(lhs);
        auto b = getMagnitude(rhs);
        auto cosine = dotProduct(lhs, rhs) / (a * b);  // law of cosines
        return acos(cosine);
    }

    template<typename T>
    inline float distanceBetween(const Vector2<T> &v1, const Vector2<T> &v2) {
        return getMagnitude(v1 - v2);
    }

    template<typename T>
    std::ostream &operator<<(std::ostream &out, const Vector2<T> v) {
        out << "x: " << v.x << ", y: " << v.y;
        return out;
    }
}
#endif //VECTOR2_H
