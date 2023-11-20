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
#ifndef PLAYERLIST_HPP_
#define PLAYERLIST_HPP_

#include <unordered_map>

#include "server/world/ServerPlayer.hpp"

class PlayerList {
	using PlayerMap = std::unordered_map<std::string, ServerPlayer>;
	using Iterator = PlayerMap::iterator;
	using ConstIterator = PlayerMap::const_iterator;

	public:
		ServerPlayer &addPlayer(const std::string &name, bool isNewPlayer);
		ServerPlayer *connectPlayer(const std::string &name, ClientInfo &client, bool isSingleplayer);
		void disconnectPlayer(const std::string &name);

		const ServerPlayer *getPlayer(const std::string &name) const;
		ServerPlayer *getPlayer(const std::string &name);

		ServerPlayer *getPlayerFromClientID(u16 clientID);

		Iterator begin() { return m_players.begin(); }
		Iterator end() { return m_players.end(); }

		ConstIterator begin() const { return m_players.begin(); }
		ConstIterator end() const { return m_players.end(); }

		std::size_t size() const { return m_players.size(); }

	private:
		PlayerMap m_players;
};

#endif // PLAYERLIST_HPP_
