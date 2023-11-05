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
#ifndef BOX_HPP_
#define BOX_HPP_

#include <algorithm>

#include "Vector3.hpp"

template<typename T>
class Box {
	public:
		Box() = default;

		Box(T _x, T _y, T _z, T _sizeX, T _sizeY, T _sizeZ) {
			reset(_x, _y, _z, _sizeX, _sizeY, _sizeZ);
		}

		Box(const Vector3<T> &_position, const Vector3<T> &_size) {
			reset(_position.x, _position.y, _position.z, _size.x, _size.y, _size.z);
		}

		template<typename U>
		Box(const Box<U> &box)
			: Box(box.x, box.y, box.z, box.sizeX, box.sizeY, box.sizeZ) {}

		void reset(T _x, T _y, T _z, T _sizeX, T _sizeY, T _sizeZ) {
			x = _x;
			y = _y;
			z = _z;
			sizeX = _sizeX;
			sizeY = _sizeY;
			sizeZ = _sizeZ;
		}

		void reset(const Box<T> &box) { reset(box.x, box.y, box.z, box.sizeX, box.sizeY, box.sizeZ); }

		void reset(const Vector3<T> &_position, const Vector3<T> &_size) {
			reset(_position.x, _position.y, _position.z, _size.x, _size.y, _size.z);
		}

		void move(T _x, T _y, T _z) { x += _x; y += _y; z += _z; }
		void move(const Vector3<T> &d) { move(d.x, d.y, d.z); }

		bool intersects(const Box<T> &box) const {
			/*
			 * Using && here implies short-circuit evaluation, which is
			 * compiled in this case as a conditional branch. We assume that
			 * the probability of an intersection is around 50%. Testing shows
			 * that the impact of evaluating all terms is less than that of
			 * mispredicted branches, so we avoid && and use & instead, which
			 * works fine on boolean conditions.
			 */
			return (std::max(x, box.x) < std::min(x + sizeX, box.x + box.sizeX))
			     & (std::max(y, box.y) < std::min(y + sizeY, box.y + box.sizeY))
			     & (std::max(z, box.z) < std::min(z + sizeZ, box.z + box.sizeZ));
		}

		Vector3<T> position() const { return {x, y, z}; }
		Vector3<T> size() const { return {sizeX, sizeY, sizeZ}; }

		void setPosition(const Vector3<T> &vector3) { x = vector3.x; y = vector3.y; z = vector3.z; }
		void setSize(const Vector3<T> &vector3) { sizeX = vector3.x; sizeY = vector3.y; sizeZ = vector3.z; }

		Box &operator+=(const Vector3<T> &vector3) { *this = operator+(vector3); return *this; }
		Box &operator-=(const Vector3<T> &vector3) { *this = operator-(vector3); return *this; }

		T x = 0;
		T y = 0;
		T z = 0;

		T sizeX = 0;
		T sizeY = 0;
		T sizeZ = 0;

		template<typename U>
		auto operator+(const Vector3<U> &vector3) const -> Box<decltype(x + vector3.x)> {
			return {x + vector3.x, y + vector3.y, z + vector3.z, sizeX, sizeY, sizeZ};
		}

		template<typename U>
		auto operator-(const Vector3<T> &vector3) const -> Box<decltype(x - vector3.x)> {
			return {x - vector3.x, y - vector3.y, z - vector3.z, sizeX, sizeY, sizeZ};
		}
};

using IntBox = Box<int>;
using FloatBox = Box<float>;
using DoubleBox = Box<double>;

#endif // BOX_HPP_
