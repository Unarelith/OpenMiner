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
#ifndef VECTOR2_HPP_
#define VECTOR2_HPP_

#include <stdexcept>
#include <utility>

#include "common/core/IntTypes.hpp"

template<typename T>
class Vector2 {
	public:
		Vector2() = default;
		constexpr Vector2(T _x, T _y) : x(_x), y(_y) {}

		template<typename U>
		Vector2(const Vector2<U> &vector2) : x(vector2.x), y(vector2.y) {}

		T length() { return sqrt(x * x + y * y); }

		Vector2 operator-() const { return Vector2{-x, -y}; }

		Vector2 &operator=(T n)                     { x = n; y = n; return *this; }
		Vector2 &operator+=(const Vector2 &vector2) { *this = operator+(vector2); return *this; }
		Vector2 &operator-=(const Vector2 &vector2) { *this = operator-(vector2); return *this; }
		Vector2 &operator*=(T n)                    { *this = operator*(n); return *this; }
		Vector2 &operator/=(T n)                    { *this = operator/(n); return *this; }

		bool operator==(const Vector2<T> &vector2) const { return x == vector2.x && y == vector2.y; }
		bool operator!=(const Vector2<T> &vector2) const { return !operator==(vector2); }

		// Needed if Vector2 is used as a key in a std::map
		bool operator<(const Vector2<T> &vector2) const { return x < vector2.x && y < vector2.y; }
		bool operator>(const Vector2<T> &vector2) const { return x > vector2.x && y > vector2.y; }

		T x;
		T y;

		bool isZero() const { return x == 0 && y == 0; }

		// Operators
		auto dot(const Vector2<T> &vector2) const {
			return x * vector2.x + y * vector2.y;
		}

		template<typename U>
		auto operator+(const Vector2<U> &vector2) const -> Vector2<decltype(x + vector2.x)> {
			return {x + vector2.x, y + vector2.y};
		}

		template<typename U>
		auto operator+(U n) const -> Vector2<decltype(x + n)> {
			return {x + n, y + n};
		}

		template<typename U>
		auto operator-(const Vector2<U> &vector2) const -> Vector2<decltype(x - vector2.x)> {
			return {x - vector2.x, y - vector2.y};
		}

		template<typename U>
		auto operator*(U n) const -> Vector2<decltype(x * n)> {
			return {x * n, y * n};
		}

		template<typename U>
		auto operator/(U n) const -> Vector2<decltype(x / n)> {
			if(n != 0) {
				return {x / n, y / n};
			} else {
				throw std::overflow_error("Divide by zero exception");
			}
		}
};

template<typename T, typename U>
auto operator*(T n, Vector2<U> &vector2) { return vector2.operator*(n); }

using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned int>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

template<typename T>
inline void hash_combine(std::size_t &seed, const T &v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std {
	template<typename T>
	struct hash<Vector2<T>> {
		size_t operator()(const Vector2<T> &vector2) const {
			size_t hash = 0;
			hash_combine(hash, vector2.x);
			hash_combine(hash, vector2.y);
			return hash;
		}
	};
}

#endif // VECTOR2_HPP_
