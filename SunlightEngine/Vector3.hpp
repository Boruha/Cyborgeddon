#pragma once

#include <SunlightEngine/Math.hpp>

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

		[[nodiscard]] Vector3<T> operator*(const T &mul) 			const { return Vector3<T>(x * mul, y * mul, z * mul); }

		[[nodiscard]] Vector3<T> operator/(const T &div) 			const { return Vector3<T>(x / div, y / div, z / div); }


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

		Vector3<T>& operator*=(const T &mul) {
			x *= mul;
			y *= mul;
			z *= mul;
			return *this;
		}

		Vector3<T>& operator/=(const T &div) {
			x /= div;
			y /= div;
			z /= div;
			return *this;
		}

		Vector3<T>& operator=(const T& n) {
			x = y = z = n;
			return *this;
		}

		[[nodiscard]] bool operator==(const Vector3<T> &vec) const { return x == vec.x && y == vec.y && z == vec.z; }
		[[nodiscard]] bool operator==(const T n) 			 const { return x == n && y == n && z == n; }

		[[nodiscard]] bool operator!=(const Vector3<T> &vec) const { return x != vec.x || y != vec.y || z != vec.z; }
		[[nodiscard]] bool operator!=(const T n) 			 const { return x != n || y != n || z != n; }

		[[nodiscard]] T length() const {
			return static_cast<T>(sqrt(x*x + y*y + z*z));
		}

		Vector3<T>& normalize() {
			T length = this->length();

			if (length == 0)
				return *this;

			return (*this) /= length;
		}

		[[nodiscard]] T getRotationY() const {
			T cos = z / this->length();
			double angle = std::acos(cos) * RAD2DEG;

			if(x < 0)
				angle = cos >= 0 ? -angle : 360 - angle;

			return static_cast<T>(angle);
		}

		void rotateXZ(double deg, const Vector3<T>& origin = Vector3<T>())
        {
		    deg *= DEG2RAD;

		    x -= origin.x;
		    z -= origin.z;

		    *this = Vector3<T>(x*cos(deg) - z*sin(deg), y, x*sin(deg) + z * cos(deg));

		    x += origin.x;
		    z += origin.z;
        }

        void rotateXY(double deg, const Vector3<T>& origin = Vector3<T>())
        {
            deg *= DEG2RAD;

            x -= origin.x;
            y -= origin.y;

            *this = Vector3<T>(x*cos(deg) - y*sin(deg), x*sin(deg) + y*cos(deg), z);

            x += origin.x;
            y += origin.y;
        }

        void rotateYZ(double deg, const Vector3<T>& origin = Vector3<T>())
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

	//typedef Vector3<short> 				Vector3s;
	typedef Vector3<int> 					Vector3i;
	//typedef Vector3<long> 				Vector3l;
	//typedef Vector3<long long> 			Vector3ll;

	//typedef Vector3<unsigned short> 		Vector3us;
	typedef Vector3<unsigned int> 			Vector3u;
	//typedef Vector3<unsigned long> 		Vector3ul;
	//typedef Vector3<unsigned long long>	Vector3ull;

	typedef Vector3<float> 					Vector3f;
	//typedef Vector3<double> 				Vector3d;
	//typedef Vector3<long double>			Vector3ld;

} // namespace Sun