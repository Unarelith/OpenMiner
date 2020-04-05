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

#include "ServerPlayer.hpp"

class PlayerList {
	using Container = std::unordered_map<u16, ServerPlayer>;
	using Iterator = Container::iterator;
	using ConstIterator = Container::const_iterator;

	public:
		ServerPlayer &addPlayer(ClientInfo &client);
		void removePlayer(u16 id);

		const ServerPlayer *getPlayer(u16 id) const;
		ServerPlayer *getPlayer(u16 id);

		Iterator begin() { return m_players.begin(); }
		Iterator end() { return m_players.end(); }

		ConstIterator begin() const { return m_players.begin(); }
		ConstIterator end() const { return m_players.end(); }

	private:
		Container m_players;
};

#endif // PLAYERLIST_HPP_
