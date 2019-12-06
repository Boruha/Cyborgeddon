#pragma once

namespace Sun {

	template<typename T>
	struct Vector2 {
		T x{0}, y{0};

		Vector2() = default;

		explicit Vector2(T xy) : x(xy), y(xy) {}

		Vector2(T x, T y) : x(x), y(y) {}

		Vector2<T>(const Vector2<T> &vec) : x(vec.x), y(vec.y) {}


		Vector2<T> operator+(const Vector2<T> &vec) const { return Vector2<T>(x + vec.x, y + vec.y); }

		Vector2<T> operator-(const Vector2<T> &vec) const { return Vector2<T>(x + vec.x, y + vec.y); }

		Vector2<T> operator-() const { return Vector2<T>(-x, -y); }

		Vector2<T> operator*(const T &mul) const { return Vector2<T>(x * mul, y * mul); }

		Vector2<T> operator/(const T &div) const { return Vector2<T>(x / div, y / div); }


		Vector2<T> &operator+=(const Vector2<T> &vec) {
			x += vec.x;
			y += vec.y;
			return *this;
		}

		Vector2<T> &operator-=(const Vector2<T> &vec) {
			x -= vec.x;
			y -= vec.y;
			return *this;
		}

		Vector2<T> &operator*=(const T &mul) {
			x *= mul;
			y *= mul;
			return *this;
		}

		Vector2<T> &operator/=(const T &div) {
			x *= div;
			y *= div;
			return *this;
		}


		bool operator==(const Vector2<T> &vec) { return x == vec.x && y == vec.y; }

		bool operator!=(const Vector2<T> &vec) { return x != vec.x || y != vec.y; }

		Vector2<T> &normalize() {
			T sum = x * x + y * y;

			if (sum == 0)
				return *this;

			x /= sqrt(sum);
			y /= sqrt(sum);

			return *this;
		}
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