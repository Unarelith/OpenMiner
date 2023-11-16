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
#ifndef LOGGERHANDLER_HPP_
#define LOGGERHANDLER_HPP_

#include <thread>
#include <unordered_map>

#include "common/core/Logger.hpp"
#include "common/core/LogStream.hpp"

class LoggerHandler {
	using InstanceMap = std::unordered_map<std::thread::id, LoggerHandler *>;

	public:
		LoggerHandler() = default;

		Logger print(LogLevel level, const char *file, int line);

		LogLevel maxLevel() const { return m_maxLevel; }
		void setMaxLevel(LogLevel maxLevel) { m_maxLevel = maxLevel; }

		void setName(const std::string &name) { m_name = name; }

		void openLogFile(const std::string &filename) { m_stream.openFile(filename); }

		static LoggerHandler &getInstance();
		static void setInstance(LoggerHandler &instance);

	private:
		static InstanceMap s_instanceMap;

		std::string m_name;

		LogLevel m_maxLevel = LogLevel::Debug;

		LogStream m_stream;
};

#endif // LOGGERHANDLER_HPP_
