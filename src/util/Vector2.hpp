#pragma once

#include <util/Math.hpp>
#include <iostream>
#include <utility>

template<typename T>
struct Vector2 {
    Vector2() = default;

    explicit Vector2(T xy) : x(xy), y(xy) {}

    Vector2(T x, T y) : x(x), y(y) {}

    Vector2<T>(const Vector2<T> &vec) : x(vec.x), y(vec.y) {}


    [[nodiscard]] Vector2<T> operator+(const Vector2<T> vec) 	const { return Vector2<T>(x + vec.x, y + vec.y); }

    [[nodiscard]] Vector2<T> operator-(const Vector2<T> vec) 	const { return Vector2<T>(x - vec.x, y - vec.y); }

    [[nodiscard]] Vector2<T> operator-() 						const { return Vector2<T>(-x, -y); }

    [[nodiscard]] Vector2<T> operator*(const T mul) 			const { return Vector2<T>(x * mul, y * mul); }

    [[nodiscard]] Vector2<T> operator/(const T div) 			const { return Vector2<T>(x / div, y / div); }


    Vector2<T>& operator+=(const Vector2<T> vec) {
        x += vec.x;
        y += vec.y;
        return *this;
    }

    Vector2<T>& operator-=(const Vector2<T> vec) {
        x -= vec.x;
        y -= vec.y;
        return *this;
    }

    Vector2<T>& operator*=(const T mul) {
        x *= mul;
        y *= mul;
        return *this;
    }

    Vector2<T>& operator/=(const T div) {
        x /= div;
        y /= div;
        return *this;
    }

    Vector2<T>& operator=(const T n) {
        x = y = n;
        return *this;
    }

    [[nodiscard]] bool operator==(const Vector2<T> vec)  const { return x == vec.x && y == vec.y; }
    [[nodiscard]] bool operator==(const T n)			 const { return x == n && y == n; }

    [[nodiscard]] bool operator!=(const Vector2<T> vec)  const { return x != vec.x || y != vec.y; }
    [[nodiscard]] bool operator!=(const T n) 			 const { return x != n || y != n; }

    [[nodiscard]] T& operator[](const int index)
    {
        return const_cast<T&>(std::as_const(*this).operator[](index));
    }

    [[nodiscard]] const T& operator[](const int index) const
    {
        switch(index) {
            case 0  : return x;
            case 1  : return y;
            default : std::cerr << "Out of bound Vector2<T>\n"; exit(-1);
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector2& v)
    {
        return os << "(\t" << v.x << "\t,\t" << v.y << "\t)";
    }

    Vector2<T>& normalize() {
        T length = this->length();

        if (length == 0)
            return *this;

        return (*this) /= length;
    }

    [[nodiscard]] T length() const {
        return static_cast<T>(sqrt(x*x + y*y));
    }

    T dot(const Vector2<T>& v) const {
        return x * v.x + y * v.y;
    }

    Vector2<T>& abs() {
        for(int i = 0; i < 2; ++i)
            (*this)[i] = std::abs((*this)[i]);

        return *this;
    }

    void rotate(double deg, const Vector2<T> origin = Vector2<T>())
    {
        deg *= DEG2RAD;

        x -= origin.x;
        y -= origin.y;

        *this = Vector2<T>(x*cos(deg) - y*sin(deg), x*sin(deg) + y*cos(deg));

        x += origin.x;
        y += origin.y;
    }

    T x{0}, y{0};
};

using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned int>;
using Vector2f = Vector2<float>;
