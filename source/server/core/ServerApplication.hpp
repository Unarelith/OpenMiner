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
#ifndef SERVERAPPLICATION_HPP_
#define SERVERAPPLICATION_HPP_

#include <gk/core/ArgumentParser.hpp>
#include <gk/core/GameClock.hpp>
#include <gk/core/EventHandler.hpp>

#include "LuaCore.hpp"
#include "Registry.hpp"
#include "ScriptEngine.hpp"
#include "Server.hpp"
#include "ServerCommandHandler.hpp"
#include "ServerModLoader.hpp"
#include "PlayerList.hpp"
#include "WorldController.hpp"

class ServerApplication {
	public:
		ServerApplication(int argc = 0, char **argv = nullptr);
		ServerApplication(gk::EventHandler &eventHandler);

		bool init();

		int run(bool isProtected = true);

		void setSingleplayer(bool isSingleplayer) { m_server.setSingleplayer(isSingleplayer); }
		void setPort(u16 port) { m_port = port; }
		void setWorldName(const std::string &worldName) { m_worldName = worldName; }
		void setLogLevel(gk::LogLevel logLevel) { m_loggerHandler.setMaxLevel(logLevel); }

	private:
		void update();
		void mainLoop();

		gk::ArgumentParser m_argumentParser;
		gk::GameClock m_clock;
		gk::EventHandler *m_eventHandler = nullptr;
		gk::LoggerHandler m_loggerHandler;

		Registry m_registry;

		u16 m_port = 4242;

		std::string m_worldName;

		WorldController m_worldController{m_registry, m_clock};
		PlayerList m_players;

		ScriptEngine m_scriptEngine;
		ServerModLoader m_modLoader{m_scriptEngine, m_registry, m_worldController, m_players};

		Server m_server;
		ServerCommandHandler m_serverCommandHandler{m_scriptEngine, m_server, m_worldController, m_players, m_registry};
};

#endif // SERVERAPPLICATION_HPP_
