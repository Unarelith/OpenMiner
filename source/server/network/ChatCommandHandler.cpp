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

#include "ChatCommandHandler.hpp"
#include "ClientInfo.hpp"
#include "ServerCommandHandler.hpp"
#include "WorldController.hpp"

void ChatCommandHandler::parseCommand(const std::string &str, ClientInfo &client) const {
	std::stringstream sstream;
	sstream << str;

	std::vector<std::string> command;
	std::string line;
	while (std::getline(sstream, line, ' ')) {
		command.emplace_back(line);
	}

	std::unordered_map<std::string, decltype(&ChatCommandHandler::teleportationCommand)> commands = {
		{"tp", &ChatCommandHandler::teleportationCommand},
		{"save", &ChatCommandHandler::saveCommand},
		{"load", &ChatCommandHandler::loadCommand},
		{"stop", &ChatCommandHandler::stopCommand},
		{"option", &ChatCommandHandler::optionCommand},
	};

	if (!command.empty()) {
		auto it = commands.find(command.at(0));
		if (it != commands.end()) {
			(this->*it->second)(command, client);
		}
		else {
			m_server.sendChatMessage(0, "Unrecognized command: " + command.at(0));

			std::stringstream commandList;
			for (auto &it : commands) {
				if (commandList.tellp() != std::streampos(0))
					commandList << ", ";
				commandList << it.first;
			}

			m_server.sendChatMessage(0, "Available commands are: " + commandList.str());
		}
	}
}

void ChatCommandHandler::teleportationCommand(const std::vector<std::string> &command, ClientInfo &client) const {
	if (command.size() != 4) {
		m_server.sendChatMessage(0, "Usage: /tp x y z", &client);
	}
	else {
		s32 x = std::stoi(command.at(1));
		s32 y = std::stoi(command.at(2));
		s32 z = std::stoi(command.at(3));

		m_server.setPlayerPosition(client.id, x, y, z);

		m_server.sendPlayerPosUpdate(client.id, true);

		m_server.sendChatMessage(0, "Teleported to " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z), &client);
	}
}

void ChatCommandHandler::saveCommand(const std::vector<std::string> &command, ClientInfo &client) const {
	if (command.size() != 2) {
		m_server.sendChatMessage(0, "Usage: /save <name>", &client);
	}
	else {
		std::string name = command.at(1);

		m_worldController.save(name);

		m_server.sendChatMessage(0, "Saved '" + name + "'", &client);
	}
}

void ChatCommandHandler::loadCommand(const std::vector<std::string> &command, ClientInfo &client) const {
	if (command.size() != 2) {
		m_server.sendChatMessage(0, "Usage: /load <name>", &client);
	}
	else {
		std::string name = command.at(1);

		m_worldController.load(name);

		m_server.sendChatMessage(0, "Loaded '" + name + "'", &client);
	}
}

void ChatCommandHandler::stopCommand(const std::vector<std::string> &, ClientInfo &client) const {
	m_server.sendChatMessage(0, "Stopping server...", &client);
	m_server.stopServer();
}

#include "ServerConfig.hpp"

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
