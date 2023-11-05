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
#include "PlayerList.hpp"

ServerPlayer &PlayerList::addPlayer(const std::string &name, bool isNewPlayer) {
	m_players.emplace(name, ServerPlayer{name, isNewPlayer});
	return m_players.at(name);
}

ServerPlayer *PlayerList::connectPlayer(const std::string &name, ClientInfo &client, bool isSingleplayer) {
	ServerPlayer *player = nullptr;

	static bool isFirstPlayer = true;
	if (isFirstPlayer && isSingleplayer && !m_players.empty()) {
		if (m_players.size() == 1) {
			std::string oldname = m_players.begin()->second.name();
			if (oldname != name) {
				m_players.emplace(name, m_players.begin()->second);
				m_players.erase(oldname);
			}

			player = &m_players.at(name);
			player->setName(name);

			logInfo() << name << "is online";
		}
		else {
			logWarning() << "Multiple players in singleplayer mode not handled yet";
			return nullptr;
		}
	}
	else {
		auto it = m_players.find(name);
		if (it != m_players.end()) {
			if (!it->second.isOnline()) {
				player = &it->second;
				logInfo() << name << "is online";
			}
			else {
				logWarning() << name << "failed to connect: already online";
				return nullptr;
			}
		}
		else {
			player = &addPlayer(name, true);
			logInfo() << name << "is online (first connection)";
		}
	}

	player->setClient(&client);
	player->setClientID(client.id);

	client.playerName = name;

	isFirstPlayer = false;

	return player;
}

void PlayerList::disconnectPlayer(const std::string &name) {
	auto it = m_players.find(name);
	if (it != m_players.end()) {
		it->second.setClient(nullptr);
		it->second.setNewPlayer(false);
		it->second.clearLoadedChunks();
		logInfo() << name << "is offline";
	}
}

const ServerPlayer *PlayerList::getPlayer(const std::string &name) const {
	auto it = m_players.find(name);
	if (it == m_players.end())
		return nullptr;

	return &it->second;
}

ServerPlayer *PlayerList::getPlayer(const std::string &name) {
	auto it = m_players.find(name);
	if (it == m_players.end())
		return nullptr;

	return &it->second;
}

ServerPlayer *PlayerList::getPlayerFromClientID(u16 clientID) {
	for (auto &it : m_players) {
		if (it.second.clientID() == clientID)
			return &it.second;
	}

	return nullptr;
}

