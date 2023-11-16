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
#include "common/core/Exception.hpp"
#include "common/core/LoggerHandler.hpp"

LoggerHandler::InstanceMap LoggerHandler::s_instanceMap;

Logger LoggerHandler::print(LogLevel level, const char *file, int line) {
	return {m_stream, level >= m_maxLevel ? level : LogLevel::None, file, line, m_name};
}

LoggerHandler &LoggerHandler::getInstance() {
	if (s_instanceMap.empty())
		throw EXCEPTION("LoggerHandler is not initialized");

	return *s_instanceMap.at(std::this_thread::get_id());
}

void LoggerHandler::setInstance(LoggerHandler &instance) {
	s_instanceMap.emplace(std::this_thread::get_id(), &instance);
}
