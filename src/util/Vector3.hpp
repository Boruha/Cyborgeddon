#pragma once

#include <util/Math.hpp>
#include <utility>
#include <iostream>

// TODO : quiza este arhcivo pertenezca a util

namespace Sun {

	template<typename T>
	struct Vector3 {
		Vector3() = default;

		explicit Vector3(T xyz) : x(xyz), y(xyz), z(xyz) {}

		Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

		Vector3<T>(const Vector3<T> &vec) : x(vec.x), y(vec.y), z(vec.z) {}


		[[nodiscard]] Vector3<T> operator+(const Vector3<T> &vec) 	const { return Vector3<T>(x + vec.x, y + vec.y, z + vec.z); }

		[[nodiscard]] Vector3<T> operator-(const Vector3<T> &vec) 	const { return Vector3<T>(x - vec.x, y - vec.y, z - vec.z); }

		[[nodiscard]] Vector3<T> operator-() 						const { return Vector3<T>(-x, -y, -z); }

		[[nodiscard]] Vector3<T> operator*(const T mul) 			const { return Vector3<T>(x * mul, y * mul, z * mul); }

		[[nodiscard]] Vector3<T> operator/(const T div) 			const { return Vector3<T>(x / div, y / div, z / div); }


		Vector3<T>& operator+=(const Vector3<T> &vec) {
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}

		Vector3<T>& operator-=(const Vector3<T> &vec) {
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}

		Vector3<T>& operator*=(const T mul) {
			x *= mul;
			y *= mul;
			z *= mul;
			return *this;
		}

		Vector3<T>& operator/=(const T div) {
			x /= div;
			y /= div;
			z /= div;
			return *this;
		}

		Vector3<T>& operator=(const T n) {
			x = y = z = n;
			return *this;
		}

		[[nodiscard]] bool operator==(const Vector3<T> &vec) const { return x == vec.x && y == vec.y && z == vec.z; }
		[[nodiscard]] bool operator==(const T n) 			 const { return x == n && y == n && z == n; }

		[[nodiscard]] bool operator!=(const Vector3<T> &vec) const { return x != vec.x || y != vec.y || z != vec.z; }
		[[nodiscard]] bool operator!=(const T n) 			 const { return x != n || y != n || z != n; }

		[[nodiscard]] T& operator[](const int index)
		{
			return const_cast<T&>(std::as_const(*this).operator[](index));
		}

		[[nodiscard]] const T& operator[](const int index) const
		{
			switch(index) {
				case 0  : return x;
				case 1  : return y;
				case 2  : return z;
				default : std::cout << "Out of bound Vector3<T>\n"; exit(-1);
			}
		}

		friend std::ostream& operator<<(std::ostream& os, const Vector3& v)
		{
			return os << "(\t" << v.x << "\t,\t" << v.y << "\t,\t" << v.z << "\t)";
		}

		[[nodiscard]] T length() const {
			return static_cast<T>(sqrt(x*x + y*y + z*z));
		}

		Vector3<T>& normalize() {
			T length = this->length();

			if (length == 0)
				return *this;

			return (*this) /= length;
		}

		[[nodiscard]] T getRotationYfromXZ() const {
			return static_cast<T>(std::atan2(x, z) * RAD2DEG);
		}

		[[nodiscard]] Vector3<T>& getXZfromRotationY(double deg) {
			deg *= DEG2RAD;

			x = sin(deg);
			z = cos(deg);

			return *this;
		}

		Vector3<T>& abs() {

			x = std::abs(x);
			y = std::abs(y);
			z = std::abs(z);

			return *this;
		}

		void rotateXZ(double deg, const Vector3<T> origin = Vector3<T>())
        {
		    deg *= DEG2RAD;

		    x -= origin.x;
		    z -= origin.z;

		    *this = Vector3<T>(x*cos(deg) - z*sin(deg), y, x*sin(deg) + z * cos(deg));

		    x += origin.x;
		    z += origin.z;
        }

        void rotateXY(double deg, const Vector3<T> origin = Vector3<T>())
        {
            deg *= DEG2RAD;

            x -= origin.x;
            y -= origin.y;

            *this = Vector3<T>(x*cos(deg) - y*sin(deg), x*sin(deg) + y*cos(deg), z);

            x += origin.x;
            y += origin.y;
        }

        void rotateYZ(double deg, const Vector3<T> origin = Vector3<T>())
        {
            deg *= DEG2RAD;

            y -= origin.y;
            z -= origin.z;

            *this = Vector3<T>(x, y*cos(deg) - z*sin(deg), y*sin(deg) + z*cos(deg));

            y += origin.y;
            z += origin.z;
        }

		T x{0}, y{0}, z{0};
	};

	using Vector3i = Vector3<int>;
	using Vector3u = Vector3<unsigned int>;
	using Vector3f = Vector3<float>;

} // namespace Sun