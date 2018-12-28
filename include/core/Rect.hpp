/*
 * =====================================================================================
 *
 *       Filename:  Rect.hpp
 *
 *    Description:
 *
 *        Created:  15/09/2014 22:13:37
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *
 * =====================================================================================
 */
#ifndef RECT_HPP_
#define RECT_HPP_

#include <algorithm>

#include "Vector2.hpp"

template<typename T>
class Rect {
	public:
		Rect() = default;

		Rect(T _x, T _y, T _width, T _height) {
			reset(_x, _y, _width, _height);
		}

		Rect(const Vector2<T> &_position, const Vector2<T> &_size) {
			reset(_position.x, _position.y, _size.x, _size.y);
		}

		template<typename U>
		Rect(const Rect<U> &rect)
			: Rect(rect.x, rect.y, rect.width, rect.height) {}

		void reset(T _x, T _y, T _width, T _height) {
			x = _x;
			y = _y;
			width = _width;
			height = _height;
		}

		void reset(Rect<T> rect) { reset(rect.x, rect.y, rect.width, rect.height); }

		void move(T _x, T _y) { x += _x; y += _y; }
		void move(Vector2<T> d) { move(d.x, d.y); }

		bool intersects(const Rect<T> &rect) const {
			T r1MinX = std::min(x, static_cast<T>(x + width));
			T r1MaxX = std::max(x, static_cast<T>(x + width));
			T r1MinY = std::min(y, static_cast<T>(y + height));
			T r1MaxY = std::max(y, static_cast<T>(y + height));

			T r2MinX = std::min(rect.x, static_cast<T>(rect.x + rect.width));
			T r2MaxX = std::max(rect.x, static_cast<T>(rect.x + rect.width));
			T r2MinY = std::min(rect.y, static_cast<T>(rect.y + rect.height));
			T r2MaxY = std::max(rect.y, static_cast<T>(rect.y + rect.height));

			T interLeft   = std::max(r1MinX, r2MinX);
			T interTop    = std::max(r1MinY, r2MinY);
			T interRight  = std::min(r1MaxX, r2MaxX);
			T interBottom = std::min(r1MaxY, r2MaxY);

			return interLeft < interRight && interTop < interBottom;
		}

		T intersectionDirection(const Rect<T> &rect) const {
			T r1MinX = std::min(x, static_cast<T>(x + width));
			T r1MaxX = std::max(x, static_cast<T>(x + width));
			T r1MinY = std::min(y, static_cast<T>(y + height));
			T r1MaxY = std::max(y, static_cast<T>(y + height));

			T r2MinX = std::min(rect.x, static_cast<T>(rect.x + rect.width));
			T r2MaxX = std::max(rect.x, static_cast<T>(rect.x + rect.width));
			T r2MinY = std::min(rect.y, static_cast<T>(rect.y + rect.height));
			T r2MaxY = std::max(rect.y, static_cast<T>(rect.y + rect.height));

			T interLeft   = std::max(r1MinX, r2MinX);
			T interTop    = std::max(r1MinY, r2MinY);
			T interRight  = std::min(r1MaxX, r2MaxX);
			T interBottom = std::min(r1MaxY, r2MaxY);

			if(interLeft < interRight && interTop < interBottom) {
				if(interRight - interLeft < interBottom - interTop) {
					return 1;
				} else {
					return 2;
				}
			} else {
				return 0;
			}
		}

		Vector2<T> position() const { return {x, y}; }

		void setPosition(Vector2<T> vector2) { x = vector2.x; y = vector2.y; }

		Rect operator+(const Vector2<T> &vector2) const { return Rect{x + vector2.x, y + vector2.y, width, height}; }
		Rect operator-(const Vector2<T> &vector2) const { return Rect{x - vector2.x, y - vector2.y, width, height}; }

		Rect &operator+=(const Vector2<T> &vector2) { *this = operator+(vector2); return *this; }
		Rect &operator-=(const Vector2<T> &vector2) { *this = operator-(vector2); return *this; }

		T x = 0;
		T y = 0;
		T width = 0;
		T height = 0;
};

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

#endif // RECT_HPP_
