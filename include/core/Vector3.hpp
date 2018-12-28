/*
 * =====================================================================================
 *
 *       Filename:  Vector3.hpp
 *
 *    Description:
 *
 *        Created:  29/06/2018 06:57:12
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef VECTOR3_HPP_
#define VECTOR3_HPP_

#include <stdexcept>
#include <utility>

#include "IntTypes.hpp"

template<typename T>
class Vector3 {
	public:
		Vector3() = default;

		Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

		template<typename U>
		Vector3(const Vector3<U> &vector3) : x(vector3.x), y(vector3.y), z(vector3.z) {}

		Vector3 operator+(const Vector3<T> &vector3) const { return Vector3{x + vector3.x, y + vector3.y, z + vector3.z}; }
		Vector3 operator-(const Vector3<T> &vector3) const { return Vector3{x - vector3.x, y - vector3.y, z - vector3.z}; }
		Vector3 operator-()                          const { return Vector3{-x, -y, -z}; }
		Vector3 operator*(T n)                       const { return Vector3{x * n, y * n, z * n}; }

		Vector3 operator/(T n) const {
			if(n != 0) {
				return Vector3{x / n, y / n, z / n};
			} else {
				throw std::overflow_error("Divide by zero exception");
			}
		}

		Vector3& operator=(T n)                     { x = n; y = n; z = n; return *this; }
		Vector3 &operator+=(const Vector3 &vector3) { *this = operator+(vector3); return *this; }
		Vector3 &operator-=(const Vector3 &vector3) { *this = operator-(vector3); return *this; }
		Vector3 &operator*=(T n)                    { *this = operator*(n); return *this; }
		Vector3 &operator/=(T n)                    { *this = operator/(n); return *this; }

		bool operator==(const Vector3<T> &vector3) const { return x == vector3.x && y == vector3.y && z == vector3.z; }
		bool operator!=(const Vector3<T> &vector3) const { return !operator==(vector3); }

		// Needed if Vector3 is used as a key in a std::map
		bool operator<(const Vector3<T> &vector3) const { return x < vector3.x && y <= vector3.y && z <= vector3.z; }
		bool operator>(const Vector3<T> &vector3) const { return x > vector3.x && y >= vector3.y && z >= vector3.z; }

		T x;
		T y;
		T z;
};

template<typename T>
Vector3<T> operator*(T n, Vector3<T> &vector3) { return vector3.operator*(n); }

namespace std {
	template<typename T>
	struct hash<Vector3<T>> {
		size_t operator()(const Vector3<T>& vector3) const {
			std::hash<T> hash;
			auto h1 = hash(vector3.x);
			auto h2 = hash(vector3.y);
			auto h3 = hash(vector3.z);

			return std::hash<T>{}(h1 ^ (h2 << h3) ^ h3);
		}
	};
}

using Vector3i = Vector3<int>;
using Vector3u = Vector3<unsigned int>;
using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;

#endif // VECTOR3_HPP_
