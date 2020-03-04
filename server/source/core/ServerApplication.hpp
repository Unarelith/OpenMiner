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

#include "LuaCore.hpp"
#include "Registry.hpp"
#include "ScriptEngine.hpp"
#include "Server.hpp"
#include "ServerCommandHandler.hpp"
#include "ServerPlayer.hpp"
#include "ServerWorld.hpp"

class ServerApplication {
	public:
		ServerApplication(int argc = 0, char **argv = nullptr);

		void init();

		int run(bool isProtected = true);

		void setSingleplayer(bool isSingleplayer) { m_server.setSingleplayer(isSingleplayer); }

	private:
		void update();
		void mainLoop();

		gk::ArgumentParser m_argumentParser;
		gk::GameClock m_clock;

		Server m_server;
		ServerWorld m_world;
		std::unordered_map<u16, ServerPlayer> m_players;

		ScriptEngine m_scriptEngine;
		LuaCore m_luaCore;

		Registry m_registry;

		int m_port = 4242;

		ServerCommandHandler m_serverCommandHandler{m_scriptEngine, m_server, m_world, m_players, m_registry};
};

#endif // SERVERAPPLICATION_HPP_
