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
#include <algorithm>

#include <gk/core/Debug.hpp>

#include "ArgumentParser.hpp"

ArgumentParser::ArgumentParser() {
	addArgument("help", {"", "--help", "Display help on commandline options."});
}

ArgumentParser::ArgumentParser(int argc, char **argv) : ArgumentParser() {
	for (int i = 0 ; i < argc ; ++i) {
		m_argv.emplace_back(argv[i]);
	}
}

void ArgumentParser::parse() {
	for (std::size_t i = 0 ; i < m_argv.size() ; ++i) {
		for (auto &it : m_arguments) {
			if (m_argv[i] == it.second.longName || (!it.second.shortName.empty() && m_argv[i] == it.second.shortName)) {
				it.second.isFound = true;
				if (it.second.hasParameter && i + 1 < m_argv.size())
					it.second.parameter = m_argv[i + 1];
			}
		}
	}

	if (getArgument("help").isFound)
		printHelp();
}

void ArgumentParser::printHelp() {
	std::cout << "Usage: " << m_argv[0] << " [options]" << std::endl << std::endl;
	std::cout << "Options:" << std::endl;

	std::size_t maxLineLength = 0;
	std::vector<std::pair<std::string, const Argument &>> args;
	for (auto &it : m_arguments) {
		std::string arg;
		if (!it.second.shortName.empty())
			arg = it.second.shortName + ", " + it.second.longName;
		else
			arg = it.second.longName;

		if (it.second.hasParameter)
			arg += " <" + it.second.paramName + ">";

		args.emplace_back(arg, it.second);

		maxLineLength = std::max<std::size_t>(maxLineLength, arg.size());
	}

	for (auto &it : args) {
		if (it.first.size() < maxLineLength)
			it.first.append(maxLineLength - it.first.size(), ' ');

		std::cout << "  " << it.first << "  " << it.second.desc << std::endl;
	}
}

void ArgumentParser::debug() {
	gkDebug() << "========== ARGS ==========";
	for (auto &it : m_arguments)
		gkDebug() << it.second.longName.c_str() << "/" << it.second.shortName.c_str() << "=" <<  it.second.isFound;
	gkDebug() << "==========================";
}
