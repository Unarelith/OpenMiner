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
#ifndef LOGSTREAM_HPP_
#define LOGSTREAM_HPP_

#include <fstream>
#include <iostream>
#include <string>

class LogStream {
	public:
		void openFile(const std::string &filename) {
			m_file.open(filename, std::ofstream::out | std::ofstream::trunc);
			if (!m_file.is_open())
				std::cerr << "Can't open log file: '" << filename << "'" << std::endl;
		}

		LogStream &operator<<(const std::string &str) {
			std::cout << str;
			if (m_file.is_open())
				m_file << str;
			return *this;
		}

		LogStream &operator<<(int i) {
			std::cout << i;
			if (m_file.is_open())
				m_file << i;
			return *this;
		}

		LogStream &operator<<(char c) {
			std::cout << c;
			if (m_file.is_open())
				m_file << c;
			return *this;
		}

		LogStream &operator<<(std::ostream &(*f)(std::ostream &)) {
			f(std::cout);
			if (m_file.is_open())
				f(m_file);
			return *this;
		}

	private:
		std::ofstream m_file;
};

#endif // LOGSTREAM_HPP_
