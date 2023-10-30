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
#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <regex>
#include <sstream>
#include <vector>

namespace utils {

template <typename T>
std::string toString(const T value, const int n = 6) {
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << value;
	return out.str();
}

bool regexMatch(const std::string &str, const std::string &regex);

template<typename... Args>
std::string makeString(Args &&...args) {
	std::ostringstream stream;
	std::vector<int> tmp{0, ((void)(stream << args << " "), 0)...};

	return stream.str();
}

std::string getCurrentTime(const char *format);

} // namespace utils

#endif // UTILS_HPP_
