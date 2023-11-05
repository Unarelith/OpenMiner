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
#ifndef RECT_HPP_
#define RECT_HPP_

#include <algorithm>

#include "Vector2.hpp"

template<typename T>
class Rect {
	public:
		Rect() = default;
		Rect(T _x, T _y, T _sizeX, T _sizeY)
			: x(_x), y(_y), sizeX(_sizeX), sizeY(_sizeY) {}

		Rect(const Vector2<T> &position, const Vector2<T> &size)
			: x(position.x), y(position.x), sizeX(size.x), sizeY(size.y) {}

		template<typename U>
		explicit Rect(const Rect<U> &rect)
			: Rect(rect.x, rect.y, rect.sizeX, rect.sizeY) {}

		//--------------------------------------------------------------------------------
		// This check is partially inclusive. If the point lies on
		// the right or bottom edge of the rectangle, this function
		// will return false.
		//--------------------------------------------------------------------------------
		bool contains(T _x, T _y) const {
			T x2 = x + sizeX;
			T y2 = y + sizeY;

			return (_x >= x) && (_x < x2) && (_y >= y) && (_y < y2);
		}

		//--------------------------------------------------------------------------------
		// This check is partially inclusive. If the point lies on
		// the right or bottom edge of the rectangle, this function
		// will return false.
		//--------------------------------------------------------------------------------
		bool contains(const Vector2<T> &point) const {
			return contains(point.x, point.y);
		}

		//--------------------------------------------------------------------------------
		// Assumes size is always positive.
		//--------------------------------------------------------------------------------
		bool intersects(const Rect<T> &rect) const {
			/*
			 * Using && here implies short-circuit evaluation, which is
			 * compiled in this case as a conditional branch. We assume that
			 * the probability of an intersection is around 50%. Testing shows
			 * that the impact of evaluating all terms is less than that of
			 * mispredicted branches, so we avoid && and use & instead, which
			 * works fine on boolean conditions.
			 */
			return (std::max(x, rect.x) < std::min(x + sizeX, rect.x + rect.sizeX))
			     & (std::max(y, rect.y) < std::min(y + sizeY, rect.y + rect.sizeY));
		}

		//--------------------------------------------------------------------------------
		// Assumes size is always positive.
		//
		// Returns the intersection if rectangles overlap, {0,0,0,0} otherwise.
		//--------------------------------------------------------------------------------
		Rect<T> intersection(const Rect<T> &rect) {
			T x1 = std::max(x, rect.x);
			T y1 = std::max(y, rect.y);
			T sx = std::min(x + sizeX, rect.x + rect.sizeX) - x1;
			T sy = std::min(y + sizeY, rect.y + rect.sizeY) - y1;
			if (sx <= 0 || sy <= 0) {
				return Rect<T>{0, 0, 0, 0};
			}
			return Rect<T>{x1, y1, sx, sy};
		}

		//--------------------------------------------------------------------------------
		// Assumes size is always positive.
		//
		// Returns 1 for horizontal, 2 for vertical, 0 for no intersection
		//--------------------------------------------------------------------------------
		int intersectionDirection(const Rect<T> &rect) const {
			T x2  = x + sizeX;
			T rx2 = rect.x + rect.sizeX;
			T y2  = y + sizeY;
			T ry2 = rect.y + rect.sizeY;
			T interLeft   = std::max(x, rect.x);
			T interTop    = std::max(y, rect.y);
			T interRight  = std::min(x2, rx2);
			T interBottom = std::min(y2, ry2);

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

		bool operator==(const Rect<T> &rect) const { return x == rect.x && y == rect.y && sizeX == rect.sizeX && sizeY == rect.sizeY; }
		bool operator!=(const Rect<T> &rect) const { return !operator==(rect); }

		T x = 0;
		T y = 0;
		T sizeX = 0;
		T sizeY = 0;
};

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

#endif // RECT_HPP_
