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
#include <functional>
#include <sstream>

#include "common/core/GameTime.hpp"
#include "server/core/ServerConfig.hpp"
#include "server/network/ChatCommandHandler.hpp"
#include "server/network/ClientInfo.hpp"
#include "server/network/ServerCommandHandler.hpp"
#include "server/world/WorldController.hpp"

void ChatCommandHandler::parseCommand(const std::string &str, ClientInfo &client) const {
	std::stringstream sstream;
	sstream << str;

	std::vector<std::string> command;
	std::string line;
	while (std::getline(sstream, line, ' ')) {
		command.emplace_back(line);
	}

	if (!command.empty()) {
		auto it = m_commands.find(command.at(0));
		if (it != m_commands.end()) {
			(this->*it->second)(command, client);
		}
		else {
			m_server.sendChatMessage(0, "Unrecognized command: " + command.at(0), &client);

			std::stringstream commandList;
			for (auto &it : m_commands) {
				if (commandList.tellp() != std::streampos(0))
					commandList << ", ";
				commandList << it.first;
			}

			m_server.sendChatMessage(0, "Available commands are: " + commandList.str(), &client);
		}
	}
}

void ChatCommandHandler::helpCommand(const std::vector<std::string> &command, ClientInfo &client) const {
	if (command.size() > 1) {
		m_server.sendChatMessage(0, "Usage: /help", &client);
	}
	else {
		m_server.sendChatMessage(0, "Available commands are:", &client);
		for (auto &it : m_commands) {
			m_server.sendChatMessage(0, "/" + it.first, &client);
		}
	}
}

void ChatCommandHandler::optionCommand(const std::vector<std::string> &command, ClientInfo &client) const {
	if (command.size() < 2 || command.size() > 3) {
		m_server.sendChatMessage(0, "Usage: /option <name> [<value>]", &client);
	}
	else {
		std::string name = command.at(1);

		auto it = ServerConfig::options.find(name);
		if (it != ServerConfig::options.end()) {
			if (command.size() < 3) {
				if (it->second.get_type() == sol::type::boolean) {
					bool value = it->second.as<bool>();
					m_server.sendChatMessage(0, std::string("Value: ") + (value ? "true" : "false"));
				}
				else if (it->second.get_type() == sol::type::number) {
					m_server.sendChatMessage(0, "Value: " + std::to_string(it->second.as<double>()));
				}
				else if (it->second.get_type() == sol::type::string) {
					m_server.sendChatMessage(0, "Value: " + it->second.as<std::string>());
				}
				else {
					m_server.sendChatMessage(0, "Value: nil");
				}
			}
			else {
				std::string value = command.at(2);
				if (ServerConfig::assignOption(name, value))
					m_server.sendChatMessage(0, "Value: " + value);
				else
					m_server.sendChatMessage(0, "Invalid option value");
			}
		}
		else {
			m_server.sendChatMessage(0, "Option '" + name + "' doesn't exist");
		}
	}
}

void ChatCommandHandler::stopCommand(const std::vector<std::string> &command, ClientInfo &client) const {
	if (command.size() > 1)
		m_server.sendChatMessage(0, "Usage: /stop", &client);
	else {
		m_server.sendChatMessage(0, "Stopping server...", &client);
		m_server.stopServer();
	}
}

void ChatCommandHandler::timeCommand(const std::vector<std::string> &command, ClientInfo &client) const {
	std::string subcommand;
	std::string param;
	if (command.size() == 2) {
		subcommand = command.at(1);
		if (subcommand != "set" && subcommand != "add" && subcommand != "help") {
			subcommand = "set";
			param = command.at(1);
		}
		else {
			// "/time set" or "/time add" aren't meaningful without a param
			subcommand = "help";
		}
	}
	else if (command.size() == 3) {
		subcommand = command.at(1);
		param = command.at(2);
	}
	else {
		subcommand = "help";
	}
	s64 ticks = 0;
	bool gotTicks = false;
	if (!param.empty()) {
		if (subcommand == "set") {
			static const std::unordered_map<std::string, u64> values = {
				{"day",       1000},
				{"noon",      6000},
				{"sunset",   11000},
				{"night",    13000},
				{"midnight", 18000},
				{"sunrise",   1000},
			};

			if (auto it = values.find(param) ; it != values.end()) {
				ticks = it->second;
				gotTicks = true;
			}
		}
		if (!gotTicks) {
			size_t dot_pos = param.find(':');
			if (dot_pos != std::string::npos) {
				bool minus_sign = param[0] == '-';
				if (minus_sign) {
					param = param.substr(1);
					--dot_pos;
				}
				try {
					u64 hours = std::stoull(param);
					u64 mins = std::stoull(param.substr(dot_pos + 1));
					ticks = (hours * 1000 + mins * 1000 / 60);
					if (subcommand == "set") {
						ticks = (ticks + 18000) % 24000;
					}
					else if (minus_sign) {
						ticks = -ticks;
					}
					gotTicks = true;
				}
				catch (...) {
					m_server.sendChatMessage(0, "Invalid time", &client);
					return;
				}
			}
			else {
				try {
					ticks = std::stoull(param);
					gotTicks = true;
				}
				catch (...) {
					m_server.sendChatMessage(0, "Invalid time", &client);
					return;
				}
			}
		}
	}

	if (gotTicks && subcommand == "set") {
		GameTime::setTicks(ticks);

		m_server.sendChatMessage(0, "Time set to " + std::to_string(ticks), &client);
	}
	else if (gotTicks && subcommand == "add") {
		GameTime::setTicks(GameTime::getTicks() + ticks);

		m_server.sendChatMessage(0, "Added " + std::to_string(ticks) + " to the time", &client);
	}
	else {
		m_server.sendChatMessage(0, "Usage: /time [set|add] <value>", &client);
	}
}

void ChatCommandHandler::tpCommand(const std::vector<std::string> &command, ClientInfo &client) const {
	if (command.size() != 4) {
		m_server.sendChatMessage(0, "Usage: /tp x y z", &client);
	}
	else {
		try {
			s32 x = std::stoi(command.at(1));
			s32 y = std::stoi(command.at(2));
			s32 z = std::stoi(command.at(3));

			m_server.setPlayerPosition(client.id, x, y, z);

			m_server.sendPlayerPosUpdate(client.id, true);

			m_server.sendChatMessage(0, "Teleported to " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z), &client);
		}
		catch (...) {
			m_server.sendChatMessage(0, "Invalid coordinates", &client);
		}
	}
}

void ChatCommandHandler::tpsCommand(const std::vector<std::string> &command, ClientInfo &client) const {
	if (command.size() > 1)
		m_server.sendChatMessage(0, "Usage: /tps", &client);
	else
		m_server.sendChatMessage(0, "TPS: " + std::to_string(GameTime::getTicksPerSecond()), &client);
}
