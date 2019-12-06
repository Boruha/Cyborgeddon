#pragma once

namespace Sun {

	template<typename T>
	struct Vector3 {
		T x{0}, y{0}, z{0};

		Vector3() = default;

		explicit Vector3(T xyz) : x(xyz), y(xyz), z(xyz) {}

		Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

		Vector3<T>(const Vector3<T> &vec) : x(vec.x), y(vec.y), z(vec.z) {}


		Vector3<T> operator+(const Vector3<T> &vec) const { return Vector3<T>(x + vec.x, y + vec.y, z + vec.z); }

		Vector3<T> operator-(const Vector3<T> &vec) const { return Vector3<T>(x + vec.x, y + vec.y, z - vec.z); }

		Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }

		Vector3<T> operator*(const T &mul) const { return Vector3<T>(x * mul, y * mul, z * mul); }

		Vector3<T> operator/(const T &div) const { return Vector3<T>(x / div, y / div, z / div); }


		Vector3<T> &operator+=(const Vector3<T> &vec) {
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}

		Vector3<T> &operator-=(const Vector3<T> &vec) {
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}

		Vector3<T> &operator*=(const T &mul) {
			x *= mul;
			y *= mul;
			z *= mul;
			return *this;
		}

		Vector3<T> &operator/=(const T &div) {
			x *= div;
			y *= div;
			z /= div;
			return *this;
		}


		bool operator==(const Vector3<T> &vec) { return x == vec.x && y == vec.y && z == vec.z; }

		bool operator!=(const Vector3<T> &vec) { return x != vec.x || y != vec.y || z != vec.z; }

		Vector3<T> &normalize() {
			T sum = x * x + y * y + z * z;

			if (sum == 0)
				return *this;

			x /= sqrt(sum);
			y /= sqrt(sum);
			z /= sqrt(sum);

			return *this;
		}
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