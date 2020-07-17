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

		static void initUsertype(sol::state &lua);

	private:
		ClientInfo *m_client = nullptr;

		bool m_isNewPlayer = false;
};

#endif // SERVERPLAYER_HPP_
