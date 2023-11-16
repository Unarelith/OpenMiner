/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef VECTOR3_HPP_
#define VECTOR3_HPP_

#include <stdexcept>
#include <utility>

#include "common/core/IntTypes.hpp"
#include "common/core/Vector2.hpp"

template<typename T>
class Vector3 {
	public:
		Vector3() = default;
		constexpr Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

		template<typename U>
		Vector3(const Vector2<U> &vector2, U _z) : x(vector2.x), y(vector2.y), z(_z) {}

		template<typename U>
		Vector3(const Vector3<U> &vector3) : x(vector3.x), y(vector3.y), z(vector3.z) {}

		T length() { return sqrt(x * x + y * y + z * z); }

		Vector3 operator-() const { return Vector3{-x, -y, -z}; }

		Vector3 &operator=(T n)                     { x = n; y = n; z = n; return *this; }
		Vector3 &operator+=(const Vector3 &vector3) { *this = operator+(vector3); return *this; }
		Vector3 &operator-=(const Vector3 &vector3) { *this = operator-(vector3); return *this; }
		Vector3 &operator*=(T n)                    { *this = operator*(n); return *this; }
		Vector3 &operator/=(T n)                    { *this = operator/(n); return *this; }

		bool operator==(const Vector3<T> &vector3) const { return x == vector3.x && y == vector3.y && z == vector3.z; }
		bool operator!=(const Vector3<T> &vector3) const { return !operator==(vector3); }

		// Needed if Vector3 is used as a key in a std::map
		bool operator<(const Vector3<T> &vector3) const { return x < vector3.x && y < vector3.y && z < vector3.z; }
		bool operator>(const Vector3<T> &vector3) const { return x > vector3.x && y > vector3.y && z > vector3.z; }

		Vector2<T> xy() const { return Vector2<T>(x, y); }

		T x;
		T y;
		T z;

		void reset(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }
		bool isZero() const { return x == 0 && y == 0 && z == 0; }

		// Operators
		auto dot(const Vector3<T> &vector3) const {
			return x * vector3.x + y * vector3.y + z * vector3.z;
		}

		template<typename U>
		auto operator+(const Vector3<U> &vector3) const -> Vector3<decltype(x + vector3.x)> {
			return {x + vector3.x, y + vector3.y, z + vector3.z};
		}

		template<typename U>
		auto operator+(U n) const -> Vector3<decltype(x + n)> {
			return {x + n, y + n, z + n};
		}

		template<typename U>
		auto operator-(const Vector3<U> &vector3) const -> Vector3<decltype(x - vector3.x)> {
			return {x - vector3.x, y - vector3.y, z - vector3.z};
		}

		template<typename U>
		auto operator*(U n) const -> Vector3<decltype(x * n)> {
			return {x * n, y * n, z * n};
		}

		template<typename U>
		auto operator/(U n) const -> Vector3<decltype(x / n)> {
			if(n != 0) {
				return {x / n, y / n, z / n};
			} else {
				throw std::overflow_error("Divide by zero exception");
			}
		}
};

template<typename T, typename U>
Vector3<T> operator*(T n, Vector3<U> &vector3) { return vector3.operator*(n); }

using Vector3i = Vector3<int>;
using Vector3u = Vector3<unsigned int>;
using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;

namespace std {
	template<typename T>
	struct hash<Vector3<T>> {
		size_t operator()(const Vector3<T> &vector3) const {
			std::hash<T> hash;
			auto h1 = hash(vector3.x);
			auto h2 = hash(vector3.y);
			auto h3 = hash(vector3.z);

			return std::hash<T>{}(static_cast<T>(h1 ^ (h2 << h3) ^ h3));
		}
	};
}

#endif // VECTOR3_HPP_
