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
#ifndef LOGGERUTILS_HPP_
#define LOGGERUTILS_HPP_

#include <ostream>

#include "IntTypes.hpp"

enum class LoggerColor : u8 {
	White = 0,
	Red = 31,
	Blue = 36
};

//======================================================================================
// LoggerColor
//======================================================================================
std::ostream &operator<<(std::ostream &stream, LoggerColor color);

//======================================================================================
// Vector2
//======================================================================================
#include "Vector2.hpp"

template<typename T>
std::ostream &operator<<(std::ostream &stream, const Vector2<T> &vec) {
	return stream << "Vector2(" << vec.x << ", " << vec.y << ")";
}

//======================================================================================
// Vector3
//======================================================================================
#include "Vector3.hpp"

template<typename T>
std::ostream &operator<<(std::ostream &stream, const Vector3<T> &vec) {
	return stream << "Vector3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}

//======================================================================================
// Color
//======================================================================================
#include "Color.hpp"

std::ostream &operator<<(std::ostream &stream, const Color &color);

//======================================================================================
// Rect
//======================================================================================
#include "Rect.hpp"

template<typename T>
std::ostream &operator<<(std::ostream &stream, const Rect<T> &rect) {
	return stream << "Rect(" << rect.x << ", " << rect.y << ", " << rect.sizeX << ", " << rect.sizeY << ")";
}

//======================================================================================
// Box
//======================================================================================
#include "Box.hpp"

template<typename T>
std::ostream &operator<<(std::ostream &stream, const Box<T> &box) {
	return stream << "Box(" << box.x << ", " << box.y << ", " << box.z << ", " << box.sizeX << ", " << box.sizeY << ", " << box.sizeZ << ")";
}

#endif // LOGGERUTILS_HPP_
