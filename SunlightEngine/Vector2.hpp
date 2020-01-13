#pragma once

#include <SunlightEngine/Math.hpp>
#include <iostream>
#include <utility>

namespace Sun {

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

		Vector2<T>& normalize() {
			T length = this->length();

			if (length == 0)
				return *this;

			return (*this) /= length;
		}

		[[nodiscard]] T length() const {
			return static_cast<T>(sqrt(x*x + y*y));
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

	//typedef Vector2<short> 				Vector2s;
	typedef Vector2<int> 					Vector2i;
	//typedef Vector2<long> 				Vector2l;
	//typedef Vector2<long long> 			Vector2ll;

	//typedef Vector2<unsigned short> 		Vector2us;
	typedef Vector2<unsigned int> 			Vector2u;
	//typedef Vector2<unsigned long> 		Vector2ul;
	//typedef Vector2<unsigned long long>	Vector2ull;

	typedef Vector2<float> 					Vector2f;
	//typedef Vector2<double> 				Vector2d;
	//typedef Vector2<long double>			Vector2ld;

} // namespace Sun