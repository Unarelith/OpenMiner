/*
 * =====================================================================================
 *
 *       Filename:  Vector2.hpp
 *
 *    Description:
 *
 *        Created:  12/05/2015 18:45:17
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *
 * =====================================================================================
 */
#ifndef VECTOR2_HPP_
#define VECTOR2_HPP_

#include <stdexcept>
#include <utility>

#include <SFML/System/Vector2.hpp>

#include "IntTypes.hpp"

template<typename T>
class Vector2 {
	public:
		Vector2() = default;

		Vector2(T _x, T _y) : x(_x), y(_y) {}

		template<typename U>
		Vector2(const Vector2<U> &vector2) : x(vector2.x), y(vector2.y) {}

		Vector2 operator+(const Vector2<T> &vector2) const { return Vector2{x + vector2.x, y + vector2.y}; }
		Vector2 operator-(const Vector2<T> &vector2) const { return Vector2{x - vector2.x, y - vector2.y}; }
		Vector2 operator-()                          const { return Vector2{-x, -y}; }
		Vector2 operator*(T n)                       const { return Vector2{x * n, y * n}; }

		Vector2 operator/(T n) const {
			if(n != 0) {
				return Vector2{x / n, y / n};
			} else {
				throw std::overflow_error("Divide by zero exception");
			}
		}

		Vector2& operator=(T n)                     { x = n; y = n; return *this; }
		Vector2 &operator+=(const Vector2 &vector2) { *this = operator+(vector2); return *this; }
		Vector2 &operator-=(const Vector2 &vector2) { *this = operator-(vector2); return *this; }
		Vector2 &operator*=(T n)                    { *this = operator*(n); return *this; }
		Vector2 &operator/=(T n)                    { *this = operator/(n); return *this; }

		bool operator==(const Vector2<T> &vector2) const { return x == vector2.x && y == vector2.y; }
		bool operator!=(const Vector2<T> &vector2) const { return !operator==(vector2); }

		// Needed if Vector2 is used as a key in a std::map
		bool operator<(const Vector2<T> &vector2) const { return x < vector2.x && y <= vector2.y; }
		bool operator>(const Vector2<T> &vector2) const { return x > vector2.x && y >= vector2.y; }

		T x;
		T y;


		// Conversion to/from sf::Vector2<T>:
		operator sf::Vector2<T>() const  { return {x, y}; }
		Vector2(const sf::Vector2<T> v) : Vector2(v.x, v.y) {}
};

template<typename T>
Vector2<T> operator*(T n, Vector2<T> &vector2) { return vector2.operator*(n); }

using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned int>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

#endif // VECTOR2_HPP_
