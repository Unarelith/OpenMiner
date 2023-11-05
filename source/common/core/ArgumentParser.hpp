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
#ifndef ARGUMENTPARSER_HPP_
#define ARGUMENTPARSER_HPP_

#include <map>
#include <string>
#include <vector>

struct Argument {
	Argument(const std::string &_shortName, const std::string &_longName, const std::string &_desc)
		: shortName(_shortName), longName(_longName), desc(_desc), hasParameter(false) {}
	Argument(const std::string &_shortName, const std::string &_longName, const std::string &_desc, const std::string &_paramName)
		: shortName(_shortName), longName(_longName), desc(_desc), paramName(_paramName), hasParameter(true) {}

	std::string shortName;
	std::string longName;
	std::string desc;
	std::string paramName;
	bool hasParameter = false;
	std::string parameter = "";
	bool isFound = false;
};

class ArgumentParser {
	public:
		ArgumentParser();
		ArgumentParser(int argc, char **argv);

		void parse();

		void printHelp();
		void debug();

		void addArgument(const std::string &name, const Argument &argument) { m_arguments.emplace(name, argument); }
		const Argument &getArgument(const std::string &name) { return m_arguments.at(name); }

	private:
		std::vector<std::string> m_argv;
		std::map<std::string, Argument> m_arguments;
};

#endif // ARGUMENTPARSER_HPP_
