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
#ifndef SERVERPLAYER_HPP_
#define SERVERPLAYER_HPP_

#include <unordered_set>

#include <gk/core/Debug.hpp>

#include "ClientInfo.hpp"
#include "Player.hpp"

class ServerPlayer : public Player {
	public:
		ServerPlayer(const std::string &name, bool isNewPlayer) {
			m_name = name;
			m_isNewPlayer = isNewPlayer;
		}

		const ClientInfo *client() const { return m_client; }
		void setClient(ClientInfo *client) { m_client = client; }

		bool isOnline() const { return m_client != nullptr; }
		bool isNewPlayer() const { return m_isNewPlayer; }
		void setNewPlayer(bool isNewPlayer) { m_isNewPlayer = isNewPlayer; }

		bool isReady() const { return m_isReady; }
		void setReady(bool isReady) { m_isReady = isReady; }

		bool isChunkLoaded(const gk::Vector3i &chunk) { return m_loadedChunks.find(chunk) != m_loadedChunks.end(); }
		void addLoadedChunk(const gk::Vector3i &chunk) { m_loadedChunks.emplace(chunk); }
		void removeLoadedChunk(const gk::Vector3i &chunk) { m_loadedChunks.erase(chunk); }
		void clearLoadedChunks() { m_loadedChunks.clear(); }
		const std::unordered_set<gk::Vector3i> &loadedChunks() const { return m_loadedChunks; }

		static void initUsertype(sol::state &lua);

	public:
		gk::Vector3i lastChunkUpdate{0, 0, 0}; // FIXME

		std::unordered_set<gk::Vector3i> sentChunks; // Cleared every tick

	private:
		ClientInfo *m_client = nullptr;

		bool m_isNewPlayer = false;
		bool m_isReady = false; // Is player ready to receive chunks?

		std::unordered_set<gk::Vector3i> m_loadedChunks;
};

#endif // SERVERPLAYER_HPP_
