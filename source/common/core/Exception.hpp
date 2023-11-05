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
#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <exception>
#include <string>

#include "Debug.hpp"
#include "Utils.hpp"

#define EXCEPTION(...) (Exception(__LINE__, _FILE, __VA_ARGS__))

class Exception {
	public:
		template<typename... Args>
		Exception(u16 line, const std::string &filename, Args... args) noexcept {
			m_errorMsg = Logger::textColor(LoggerColor::Red, true);
			m_errorMsg += "at " + filename + ":" + std::to_string(line) + ": ";
			m_errorMsg += utils::makeString(std::forward<Args>(args)...);
			m_errorMsg += Logger::textColor();
		}

		virtual ~Exception() = default;

		virtual const char *what() const noexcept {
			return m_errorMsg.c_str();
		}

	private:
		std::string m_errorMsg;
};

#endif // EXCEPTION_HPP_
